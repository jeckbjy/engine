//! Process
#include "Cute/Process.h"
// #include "Cute/Exception.h"

CUTE_NS_BEGIN

std::vector<char> ChildProcess::GetEnvironmentBuffer(const std::map<String, String>& env)
{
  std::vector<char> envbuf;
  std::size_t pos = 0;

  for (std::map<String, String>::const_iterator it = env.begin(); it != env.end(); ++it)
  {
      std::size_t envlen = it->first.length() + it->second.length() + 1;

      envbuf.resize(pos + envlen + 1);
      std::copy(it->first.begin(), it->first.end(), &envbuf[pos]);
      pos += it->first.length();
      envbuf[pos] = '=';
      ++pos;
      std::copy(it->second.begin(), it->second.end(), &envbuf[pos]);
      pos += it->second.length();

      envbuf[pos] = '\0';
      ++pos;
  }

  envbuf.resize(pos + 1);
  envbuf[pos] = '\0';

  return envbuf;
}

CUTE_NS_END

//形式如:key1=value1\0key2=value2\0...\0
//static std::vector<char> GetEnvironmentBuffer(const std::map<String, String>& env)
//{
//  std::vector<char> envbuf;
//  std::size_t pos = 0;
//
//  for (std::map<String, String>::const_iterator it = env.begin(); it != env.end(); ++it)
//  {
//      std::size_t envlen = it->first.length() + it->second.length() + 1;
//
//      envbuf.resize(pos + envlen + 1);
//      std::copy(it->first.begin(), it->first.end(), &envbuf[pos]);
//      pos += it->first.length();
//      envbuf[pos] = '=';
//      ++pos;
//      std::copy(it->second.begin(), it->second.end(), &envbuf[pos]);
//      pos += it->second.length();
//
//      envbuf[pos] = '\0';
//      ++pos;
//  }
//
//  envbuf.resize(pos + 1);
//  envbuf[pos] = '\0';
//
//  return envbuf;
//}
//
//#ifdef CUTE_OS_FAMILY_WINDOWS
////////////////////////////////////////////////////////////////////////////
//// Windows
////////////////////////////////////////////////////////////////////////////
//#undef STARTUPINFO
//#undef CreateProcess
//#undef GetStartupInfo
//#undef ShellExecute
//
//#ifdef CUTE_WIN32_UTF8
//#define STARTUPINFO           STARTUPINFOW
//#define GetStartupInfo        GetStartupInfoW
//#define CreateProcess     CreateProcessW
//#define ShellExecute      ShellExecuteW
//#else
//#define STARTUPINFO           STARTUPINFOA
//#define GetStartupInfo        GetStartupInfoA
//#define CreateProcess     CreateProcessA
//#define ShellExecute      ShellExecuteA
//#endif
//
//static bool SetupInheritHandle(HANDLE& hDst, HANDLE hSrc, HANDLE hProc, DWORD stdHandleID)
//{
//  HANDLE hTmp = hSrc;
//  if (hTmp == INVALID_HANDLE_VALUE)
//      hTmp = GetStdHandle(stdHandleID);
//
//  if (hTmp != INVALID_HANDLE_VALUE)
//  {
//      DuplicateHandle(hProc, hTmp, hProc, &hDst, 0, TRUE, DUPLICATE_SAME_ACCESS);
//      if (hSrc != INVALID_HANDLE_VALUE)
//          ::CloseHandle(hSrc);
//
//      return true;
//  }
//  else
//  {
//      hDst = NULL;
//      return false;
//  }
//}
//
//// Based on code from https://blogs.msdn.microsoft.com/twistylittlepassagesallalike/2011/04/23/everyone-quotes-command-line-arguments-the-wrong-way/
//static String EscapeArg(const String& arg)
//{
//  bool containsQuotableChar = std::string::npos != arg.find_first_of(" \t\n\v\"");
//  // Assume args that start and end with quotes are already quoted and do not require further quoting.
//  // There is probably code out there written before launch() escaped the arguments that does its own
//  // escaping of arguments. This ensures we do not interfere with those arguments.
//  bool isAlreadyQuoted = arg.size() > 1 && '\"' == arg[0] && '\"' == arg[arg.size() - 1];
//  bool isNeedsEscaping = containsQuotableChar && !isAlreadyQuoted;
//
//  if (isNeedsEscaping)
//  {
//      String quotedArg("\"");
//      for (String::const_iterator it = arg.begin();; ++it)
//      {
//          unsigned backslashCount = 0;
//          while (it != arg.end() && '\\' == *it)
//          {
//              ++it;
//              ++backslashCount;
//          }
//
//          if (it == arg.end())
//          {
//              quotedArg.append(2 * backslashCount, '\\');
//              break;
//          }
//          else if ('"' == *it)
//          {
//              quotedArg.append(2 * backslashCount + 1, '\\');
//              quotedArg.push_back('"');
//          }
//          else
//          {
//              quotedArg.append(backslashCount, '\\');
//              quotedArg.push_back(*it);
//          }
//      }
//      quotedArg.push_back('"');
//      return quotedArg;
//  }
//  else
//  {
//      return arg;
//  }
//}
//
//void Process::times(long& userTime, long& kernelTime)
//{
//  FILETIME ftCreation;
//  FILETIME ftExit;
//  FILETIME ftKernel;
//  FILETIME ftUser;
//
//  if (GetProcessTimes(GetCurrentProcess(), &ftCreation, &ftExit, &ftKernel, &ftUser) != 0)
//  {
//      ULARGE_INTEGER time;
//      time.LowPart = ftKernel.dwLowDateTime;
//      time.HighPart = ftKernel.dwHighDateTime;
//      kernelTime = long(time.QuadPart / 10000000L);
//      time.LowPart = ftUser.dwLowDateTime;
//      time.HighPart = ftUser.dwHighDateTime;
//      userTime = long(time.QuadPart / 10000000L);
//  }
//  else
//  {
//      userTime = kernelTime = -1;
//  }
//}
//
//error_t Process::wait(handle_t handle)
//{
//  DWORD rc = WaitForSingleObject(handle, INFINITE);
//  if (rc != WAIT_OBJECT_0)
//      throw SystemException("Wait failed for process");
//
//  DWORD exitCode;
//  if (GetExitCodeProcess(handle, &exitCode) == 0)
//      throw SystemException("Cannot get exit code for process");
//
//  return exitCode;
//}
//
//error_t Process::kill(handle_t handle, int sig /* = SIGKILL */)
//{
//  if (handle != NULL)
//  {
//      DWORD result = ERROR_SUCCESS;
//      if (TerminateProcess(handle, 0) == FALSE)
//      {
//          result = GetLastError();
//      }
//      CloseHandle(handle);
//      return (int)result;
//  }
//  else
//  {
//      return ERROR_INVALID_HANDLE;
//  }
//}
//
//bool Process::isRunning(handle_t handle)
//{
//  bool result = true;
//  DWORD exitCode;
//  BOOL rc = GetExitCodeProcess(handle, &exitCode);
//  if (!rc || exitCode != STILL_ACTIVE)
//      result = false;
//  return result;
//}
//
//pid_t Process::id()
//{
//  return GetCurrentProcessId();
//}
//
//pid_t Process::id(handle_t handle)
//{
//  // https://www.codeproject.com/articles/21926/getting-process-id-from-process-handle
//  return GetProcessId(handle);
//}
//
//handle_t Process::process()
//{
//  return GetCurrentProcess();
//}
//
//handle_t Process::process(pid_t pid)
//{
//  HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
//  if (hProc == NULL)
//  {
//      switch (GetLastError())
//      {
//      case ERROR_ACCESS_DENIED:
//          throw NoPermissionException("cannot kill process");
//      case ERROR_INVALID_PARAMETER:
//          throw NotFoundException("cannot kill process");
//      default:
//          throw SystemException("cannot kill process");
//      }
//  }
//
//  return hProc;
//}
//
//handle_t Process::launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs)
//{
//  STARTUPINFO startupInfo;
//  GetStartupInfo(&startupInfo);
//  startupInfo.cb = sizeof(STARTUPINFO);
//  startupInfo.lpReserved = NULL;
//  startupInfo.lpDesktop = NULL;
//  startupInfo.lpTitle = NULL;
//  startupInfo.dwFlags = STARTF_FORCEOFFFEEDBACK;
//  startupInfo.cbReserved2 = 0;
//  startupInfo.lpReserved2 = NULL;
//
//  HANDLE hProc = GetCurrentProcess();
//  bool mustInheritHandles = false;
//  if (SetupInheritHandle(startupInfo.hStdInput, pipes != 0 ? pipes[0] : NULL, hProc, STD_INPUT_HANDLE))
//  {
//      mustInheritHandles = true;
//  }
//
//  if (SetupInheritHandle(startupInfo.hStdOutput, pipes != 0 ? pipes[1] : NULL, hProc, STD_OUTPUT_HANDLE))
//  {
//      mustInheritHandles = true;
//  }
//
//  if (SetupInheritHandle(startupInfo.hStdError, pipes != 0 ? pipes[2] : NULL, hProc, STD_ERROR_HANDLE))
//  {
//      mustInheritHandles = true;
//  }
//
//  if (mustInheritHandles)
//  {
//      startupInfo.dwFlags |= STARTF_USESTDHANDLES;
//  }
//
//  String tempCmdLine = cmd;
//  for (Args::const_iterator it = args.begin(); it != args.end(); ++it)
//  {
//      tempCmdLine.append(" ");
//      tempCmdLine.append(EscapeArg(*it));
//  }
//
//  TString cmdLine, initDir;
//  ConvertToSystem(cmd, cmdLine);
//  ConvertToSystem(directory, initDir);
//
//  const TCHAR* workingDirectory = initDir.empty() ? 0 : initDir.c_str();
//
//  const char* pEnv = 0;
//  std::vector<char> envChars;
//  if (!envs.empty())
//  {
//      envChars = GetEnvironmentBuffer(envs);
//      pEnv = &envChars[0];
//  }
//
//  DWORD creationFlags = GetConsoleWindow() ? 0 : CREATE_NO_WINDOW;
//  PROCESS_INFORMATION processInfo;
//  BOOL rc = CreateProcess(
//      NULL,   // applicationName
//      &cmdLine[0],
//      NULL,   // processAttributes
//      NULL,   // threadAttributes
//      mustInheritHandles,
//      creationFlags,
//      (LPVOID)pEnv,
//      workingDirectory,
//      &startupInfo,
//      &processInfo);
//  if (startupInfo.hStdInput)  CloseHandle(startupInfo.hStdInput);
//  if (startupInfo.hStdOutput) CloseHandle(startupInfo.hStdOutput);
//  if (startupInfo.hStdError)  CloseHandle(startupInfo.hStdError);
//  if (rc)
//  {
//      CloseHandle(processInfo.hThread);
//      return processInfo.hProcess;
//  }
//  else throw SystemException("Cannot launch process", cmd);
//}
//
//bool Process::open(const String& file, const String& mode)
//{
//  bool result;
//  TString tfile, tmode;
//
//  result = (size_t)ShellExecute(0, tmode.data(), tfile.data(), 0, 0, SW_SHOW) > 32;
//  return result;
//}
//#else
////////////////////////////////////////////////////////////////////////////
//// posix
////////////////////////////////////////////////////////////////////////////
//void Process::times(long& userTime, long& kernelTime)
//{
//  struct rusage usage;
//  getrusage(RUSAGE_SELF, &usage);
//  userTime = usage.ru_utime.tv_sec;
//  kernelTime = usage.ru_stime.tv_sec;
//}
//
//error_t Process::wait(handle_t handle)
//{
//  pid_t pid = handle;
//  int status;
//  int rc;
//  do
//  {
//      rc = waitpid(pid, &status, 0);
//  } while (rc < 0 && errno == EINTR);
//  if (rc != pid)
//      throw SystemException("Cannot wait for process");
//  return WEXITSTATUS(status);
//}
//
//error_t Process::kill(handle_t handle, int sig /* = SIGKILL */)
//{
//  return ::kill(handle, sig);
//}
//
//bool Process::isRunning(handle_t handle)
//{
//  return ::kill(handle, 0) == 0;
//}
//
//pid_t Process::id()
//{
//  return getpid();
//}
//
//pid_t Process::id(handle_t handle)
//{
//  return handle;
//}
//
//handle_t Process::process()
//{
//  return getpid();
//}
//
//handle_t Process::process(pid_t pid)
//{
//  return pid;
//}
//
//handle_t Process::launch(const String& command, const Args& args, const String& initialDirectory, handle_t pipes[3], const Envs& envs)
//{
//  // argv
//  std::vector<char*> argv(args.size() + 2);
//  int i = 0;
//  argv[i++] = const_cast<char*>(command.c_str());
//  for (Args::const_iterator it = args.begin(); it != args.end(); ++it)
//  {
//      argv[i++] = const_cast<char*>(it->c_str());
//  }
//  argv[i] = NULL;
//
//  // env
//  std::vector<char> envChars;
//  if (!envs.empty())
//  {
//      envChars = GetEnvironmentBuffer(envs);
//  }
//
//#ifdef __QNX__
//  if (initialDirectory.empty())
//  {
//      /// use QNX's spawn system call which is more efficient than fork/exec.
//      struct inheritance inherit;
//      std::memset(&inherit, 0, sizeof(inherit));
//      inherit.flags = SPAWN_ALIGN_DEFAULT | SPAWN_CHECK_SCRIPT | SPAWN_SEARCH_PATH;
//      int fdmap[3];
//      if (pipes != NULL)
//      {
//          fdmap[0] = pipes[0] ? pipes[0] : 0;
//          fdmap[1] = pipes[1] ? pipes[1] : 1;
//          fdmap[2] = pipes[2] ? pipes[2] : 2;
//      }
//      else
//      {
//          fdmap[0] = 0;
//          fdmap[1] = 1;
//          fdmap[2] = 2;
//      }
//
//      char** envPtr = 0;
//      std::vector<char*> envPtrs;
//      if (!envs.empty())
//      {
//          envPtrs.reserve(envs.size() + 1);
//          char* p = &envChars[0];
//          while (*p)
//          {
//              envPtrs.push_back(p);
//              while (*p) ++p;
//              ++p;
//          }
//          envPtrs.push_back(0);
//          envPtr = &envPtrs[0];
//      }
//
//      int pid = spawn(command.c_str(), 3, fdmap, &inherit, argv, envPtr);
//      if (pid == -1)
//          throw SystemException("cannot spawn", command);
//
//      return pid;
//  }
//#endif
//
//  // user fork
//  const char* pInitialDirectory = initialDirectory.empty() ? 0 : initialDirectory.c_str();
//
//  int pid = fork();
//  if (pid < 0)
//  {
//      throw SystemException("Cannot fork process for", command);
//  }
//  else if (pid == 0)
//  {
//      if (pInitialDirectory)
//      {
//          if (chdir(pInitialDirectory) != 0)
//          {
//              _exit(72);
//          }
//      }
//
//      // set environment variables
//      char* p = &envChars[0];
//      while (*p)
//      {
//          putenv(p);
//          while (*p) ++p;
//          ++p;
//      }
//
//      if (pipes != NULL)
//      {
//          // setup redirection
//          if (pipes[0] != 0)
//          {
//              dup2(pipes[0], STDIN_FILENO);
//              close(pipes[0]);
//          }
//
//          // outPipe and errPipe may be the same, so we dup first and close later
//          if (pipes[1] != 0)
//              dup2(pipes[1], STDOUT_FILENO);
//
//          if (pipes[2] != 0)
//              dup2(pipes[2], STDERR_FILENO);
//
//          if (pipes[1] != 0)
//              close(pipes[1]);
//
//          if (pipes[2] != 0)
//              close(pipes[2]);
//      }
//
//      // close all open file descriptors other than stdin, stdout, stderr
//      for (int fd = 3; fd < sysconf(_SC_OPEN_MAX); ++fd)
//      {
//          close(fd);
//      }
//
//      execvp(argv[0], &argv[0]);
//      _exit(72);
//  }
//
//  return pid;
//}
//
//bool Process::open(const String& file, const String& mode)
//{
//#if defined(__APPLE__)
//  const char* cmd = "/usr/bin/open";
//#else
//  const char* cmd = "/usr/bin/xdg-open";
//#endif
//
//  bool result;
//  Args args;
//  args.push_back(file);
//  result = (launch(cmd, args) == 0);
//  return result;
//}
//
//#endif
//
//handle_t Process::launch(const String& cmd)
//{
//  Args args;
//  return launch(cmd, args);
//}
//
//handle_t Process::launch(const String& cmd, const Args& args)
//{
//  String initialDirectory;
//  return launch(cmd, args, initialDirectory);
//}
//
//handle_t Process::launch(const String& cmd, const Args& args, const String& directory)
//{
//  return launch(cmd, args, directory, NULL);
//}
//
//handle_t Process::launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3])
//{
//  Envs envs;
//  return launch(cmd, args, directory, pipes, envs);
//}

//CUTE_NS_END
