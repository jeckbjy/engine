#include "CProcess.h"
#include "Util.h"
#ifdef CU_OS_WIN
#include <shellapi.h>
#endif

CU_NS_BEGIN

pid_t Process::currentID()
{
#ifdef CU_OS_WIN
	return GetCurrentProcessId();
#else
	return getpid();
#endif
}

void Process::times(long& userTime, long& kernelTime)
{
#ifdef CU_OS_WIN
	FILETIME ftCreation;
	FILETIME ftExit;
	FILETIME ftKernel;
	FILETIME ftUser;

	if (GetProcessTimes(GetCurrentProcess(), &ftCreation, &ftExit, &ftKernel, &ftUser) != 0)
	{
		ULARGE_INTEGER time;
		time.LowPart = ftKernel.dwLowDateTime;
		time.HighPart = ftKernel.dwHighDateTime;
		kernelTime = long(time.QuadPart / 10000000L);
		time.LowPart = ftUser.dwLowDateTime;
		time.HighPart = ftUser.dwHighDateTime;
		userTime = long(time.QuadPart / 10000000L);
	}
	else
	{
		userTime = kernelTime = -1;
	}
#else
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	userTime = usage.ru_utime.tv_sec;
	kernelTime = usage.ru_stime.tv_sec;
#endif
}

int Process::wait(pid_t pid)
{
#ifdef CU_OS_WIN
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if(!hProc)
		return 0;
	DWORD rc = WaitForSingleObject(hProc, INFINITE);
	if (rc != WAIT_OBJECT_0)
		throw std::runtime_error("Wait failed for process");

	DWORD exitCode;
	if (GetExitCodeProcess(hProc, &exitCode) == 0)
		throw std::runtime_error("Cannot get exit code for process");

	return exitCode;
#else
	int status;
	int rc;
	do
	{
		rc = waitpid(_pid, &status, 0);
	} while (rc < 0 && errno == EINTR);
	if (rc != _pid)
		throw std::runtime_error("Cannot wait for process");
		//throw SystemException("Cannot wait for process", NumberFormatter::format(_pid));
	return WEXITSTATUS(status);
#endif
}

void Process::kill(pid_t pid)
{
#ifdef CU_OS_WIN
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (hProc)
	{
		if (TerminateProcess(hProc, 0) == 0)
		{
			CloseHandle(hProc);
			throw std::runtime_error("cannot kill process");
		}
		CloseHandle(hProc);
	}
	else
	{
		switch (GetLastError())
		{
		case ERROR_ACCESS_DENIED:
			throw std::runtime_error("cannot kill process");
		case ERROR_INVALID_PARAMETER:
			throw std::runtime_error("cannot kill process");
		default:
			throw std::runtime_error("cannot kill process");
		}
	}
#else

#endif
}

bool Process::isRunning(pid_t pid)
{
#ifdef CU_OS_WIN
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	bool result = true;
	DWORD exitCode;
	BOOL rc = GetExitCodeProcess(hProc, &exitCode);
	if (!rc || exitCode != STILL_ACTIVE) result = false;
	return result;
#else
	if (::kill(pid, 0) == 0)
		return true;
	else
		return false;
#endif
}

handle_t Process::getProcess(pid_t pid)
{
#ifdef CU_OS_WIN
	return OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
#else
	return (handle_t)pid;
#endif
}

pid_t Process::launch(const String& cmd, const Args& args, const Env& env, const char* dir /* = NULL */)
{
#ifdef CU_OS_WIN
	// utf8 to Unicode
	WString wcmd = Util::utf8_to_wstring(cmd);
	WString wdir = Util::utf8_to_wstring(dir);

	BOOL bInheritHandles = FALSE;
	STARTUPINFOW startupInfo;
	GetStartupInfoW(&startupInfo);
	PROCESS_INFORMATION processInfo;
	DWORD flags = GetConsoleWindow() ? 0 : CREATE_NO_WINDOW;
	BOOL rc = CreateProcessW(
		NULL, 
		(LPWSTR)wcmd.c_str(),
		NULL,
		NULL,
		bInheritHandles,
		flags,
		NULL,
		wdir.c_str(),
		&startupInfo,
		&processInfo);
	if (!rc)
		return 0;
	CloseHandle(processInfo.hThread);
	return processInfo.dwProcessId;
#else
	pid_t pid = fork();
	if (pid < 0)
		return -1;
	if (pid == 0)
	{
		if (path)
		{
			if (chdir(path) != 0)
				_exit(72);
		}
		// 构造参数
		std::vector<char*> argv;
		argv.reserve(args.size() + 2);
		argv.push_back((char*)cmd.c_str());
		for (Args::const_iterator it = args.begin(); it != args.end(); ++it)
		{
			argv.push_back((char*)it->c_str());
		}
		argv[argv.size() - 1] = NULL;
		// set environment variables

		execvp(argv[0], &argv[0]);
		_exit(72);
	}
	return pid;
#endif
}

bool Process::open(const String& file, const String& mode)
{
#ifdef CU_OS_WIN
	bool success = (size_t)ShellExecuteA(0, mode.data(), file.data(), 0, 0, SW_SHOW) > 32;
#else

#ifdef(__APPLE__)
	const char* cmd = "/usr/bin/open";
#else
	const char* cmd = "/usr/bin/xdg-open";
#endif

	Args args;
	args.push_back(file);
	Env  env;
	bool success = (launch(cmd, args, env) == 0);
#endif
	return success;
}

CU_NS_END