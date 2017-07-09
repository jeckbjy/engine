//! Process
#include "Cute/Platform.h"

#if defined(CUTE_OS_FAMILY_WINDOWS)
#include "Cute/Process.h"
#include <shellapi.h>

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// ChildProcess
//////////////////////////////////////////////////////////////////////////
static bool SetupInheritHandle(HANDLE& hDst, HANDLE hSrc, HANDLE hProc, DWORD stdHandleID)
{
  HANDLE hTmp = hSrc;
  if (hTmp == INVALID_HANDLE_VALUE)
      hTmp = GetStdHandle(stdHandleID);

  if (hTmp != INVALID_HANDLE_VALUE)
  {
      DuplicateHandle(hProc, hTmp, hProc, &hDst, 0, TRUE, DUPLICATE_SAME_ACCESS);
      if (hSrc != INVALID_HANDLE_VALUE)
          ::CloseHandle(hSrc);

      return true;
  }
  else
  {
      hDst = NULL;
      return false;
  }
}

// Based on code from https://blogs.msdn.microsoft.com/twistylittlepassagesallalike/2011/04/23/everyone-quotes-command-line-arguments-the-wrong-way/
static String EscapeArg(const String& arg)
{
    bool containsQuotableChar = std::string::npos != arg.find_first_of(" \t\n\v\"");
    // Assume args that start and end with quotes are already quoted and do not require further quoting.
    // There is probably code out there written before launch() escaped the arguments that does its own
    // escaping of arguments. This ensures we do not interfere with those arguments.
    bool isAlreadyQuoted = arg.size() > 1 && '\"' == arg[0] && '\"' == arg[arg.size() - 1];
    bool isNeedsEscaping = containsQuotableChar && !isAlreadyQuoted;
    
    if (isNeedsEscaping)
    {
        String quotedArg("\"");
        for (String::const_iterator it = arg.begin();; ++it)
        {
            unsigned backslashCount = 0;
            while (it != arg.end() && '\\' == *it)
            {
                ++it;
                ++backslashCount;
            }
            
            if (it == arg.end())
            {
                quotedArg.append(2 * backslashCount, '\\');
                break;
            }
            else if ('"' == *it)
            {
                quotedArg.append(2 * backslashCount + 1, '\\');
                quotedArg.push_back('"');
            }
            else
            {
                quotedArg.append(backslashCount, '\\');
                quotedArg.push_back(*it);
            }
        }
        quotedArg.push_back('"');
        return quotedArg;
    }
    else
    {
        return arg;
    }
}

class ChildProcessImpl
{
public:
    ChildProcessImpl(HANDLE proc, pid_t pid): m_process(proc), m_pid(pid) {}
public:
    HANDLE m_process;
    pid_t  m_pid;
};

int ChildProcess::wait() const
{
    if (m_impl == 0)
        return 0;
    
    return 0;
}

void ChildProcess::kill()
{
    
}

bool ChildProcess::isRunning() const
{
    return false;
}

bool ChildProcess::launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs)
{
    STARTUPINFO startupInfo;
    GetStartupInfo(&startupInfo);
    startupInfo.cb = sizeof(STARTUPINFO);
    startupInfo.lpReserved = NULL;
    startupInfo.lpDesktop = NULL;
    startupInfo.lpTitle = NULL;
    startupInfo.dwFlags = STARTF_FORCEOFFFEEDBACK;
    startupInfo.cbReserved2 = 0;
    startupInfo.lpReserved2 = NULL;
    
    HANDLE hProc = GetCurrentProcess();
    bool mustInheritHandles = false;
    if (SetupInheritHandle(startupInfo.hStdInput, pipes != 0 ? pipes[0] : NULL, hProc, STD_INPUT_HANDLE))
    {
        mustInheritHandles = true;
    }
    
    if (SetupInheritHandle(startupInfo.hStdOutput, pipes != 0 ? pipes[1] : NULL, hProc, STD_OUTPUT_HANDLE))
    {
        mustInheritHandles = true;
    }
    
    if (SetupInheritHandle(startupInfo.hStdError, pipes != 0 ? pipes[2] : NULL, hProc, STD_ERROR_HANDLE))
    {
        mustInheritHandles = true;
    }
    
    if (mustInheritHandles)
    {
        startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    }
    
    String tempCmdLine = cmd;
    for (Args::const_iterator it = args.begin(); it != args.end(); ++it)
    {
        tempCmdLine.append(" ");
        tempCmdLine.append(EscapeArg(*it));
    }
    
    TString cmdLine, initDir;
    ConvertToSystem(cmd, cmdLine);
    ConvertToSystem(directory, initDir);
    
    const TCHAR* workingDirectory = initDir.empty() ? 0 : initDir.c_str();
    
    const char* pEnv = 0;
    std::vector<char> envChars;
    if (!envs.empty())
    {
        envChars = GetEnvironmentBuffer(envs);
        pEnv = &envChars[0];
    }
    
    DWORD creationFlags = GetConsoleWindow() ? 0 : CREATE_NO_WINDOW;
    PROCESS_INFORMATION processInfo;
    BOOL rc = CreateProcess(
                            NULL,   // applicationName
                            &cmdLine[0],
                            NULL,   // processAttributes
                            NULL,   // threadAttributes
                            mustInheritHandles,
                            creationFlags,
                            (LPVOID)pEnv,
                            workingDirectory,
                            &startupInfo,
                            &processInfo);
    if (startupInfo.hStdInput)  CloseHandle(startupInfo.hStdInput);
    if (startupInfo.hStdOutput) CloseHandle(startupInfo.hStdOutput);
    if (startupInfo.hStdError)  CloseHandle(startupInfo.hStdError);
    if (rc)
    {
        CloseHandle(processInfo.hThread);
        m_impl = new ChildProcessImpl(processInfo.hProcess, processInfo.dwProcessId);
        return true;
    }
    else {
        //          throw SystemException("Cannot launch process", cmd);
        return false;
    }
    
}

//////////////////////////////////////////////////////////////////////////
// process
//////////////////////////////////////////////////////////////////////////
void Process::times(long& userTime, long& kernelTime)
{
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
}

pid_t Process::id()
{
    return GetCurrentProcessId();
}

void Process::kill(PID pid)
{
    HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProc)
    {
        if (TerminateProcess(hProc, 0) == 0)
        {
            CloseHandle(hProc);
            return;
            // throw SystemException("cannot kill process");
        }
        CloseHandle(hProc);
    }
    // else
    // {
    //     switch (GetLastError())
    //     {
    //     case ERROR_ACCESS_DENIED:
    //         throw NoPermissionException("cannot kill process");
    //     case ERROR_INVALID_PARAMETER:
    //         throw NotFoundException("cannot kill process");
    //     default:
    //         throw SystemException("cannot kill process");
    //     }
    // }
}

bool Process::isRunning(PID pid)
{
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    bool result = true;
    DWORD exitCode;
    BOOL rc = GetExitCodeProcess(hProc, &exitCode);
    if (!rc || exitCode != STILL_ACTIVE) result = false;
    return result;
}

void Process::terminate()
{
#if CUTE_COMPILER_MSVC && CUTE_CHECK_MEMORY_LEAKS
    _CrtDumpMemoryLeaks();
#endif
    
    // bullet in the head in case there's a problem shutting down..
    ExitProcess (1);
}

void Process::hide()
{
    // not support
}

void Process::setPriority(int prio)
{
    static int lastPriority = -1;
    if (lastPriority != prio)
    {
        lastPriority = prio;
        
        DWORD prio_win;
        switch (prio)
        {
            case PRIO_LOW:      prio_win = IDLE_PRIORITY_CLASS;   break;
            case PRIO_NORMAL:   prio_win = NORMAL_PRIORITY_CLASS; break;
            case PRIO_HIGH:     prio_win = HIGH_PRIORITY_CLASS;   break;
            case PRIO_REALTIME: prio_win = REALTIME_PRIORITY_CLASS; break;
            default: assert(false);
        }
        
        SetPriorityClass(GetCurrentProcess(), prio_win);
    }
}

void Process::setForeground()
{
    
}

bool Process::isForeground()
{
    return false;
}

bool Process::isDebuggerPresent()
{
    return IsDebuggerPresent() == TRUE;
}

bool Process::isRunningInWine()
{
    HMODULE ntdll = GetModuleHandleA ("ntdll");
    return ntdll != 0 && GetProcAddress (ntdll, "wine_get_version") != NULL;
}

bool Process::open(const String& url, const String& params)
{
    HINSTANCE hInstance = ShellExecuteA(0, 0, url.c_str(), params.c_str(), 0, SW_SHOWDEFAULT);
    
    return hInstance > (HINSTANCE) 32;
}

bool Process::sendmail(const String& addr, const String& subject, const String& body, const StringArray& attaches)
{
    // DynamicLibrary dll ("MAPI32.dll");
    // JUCE_LOAD_WINAPI_FUNCTION (dll, MAPISendMail, mapiSendMail,
    //                            ULONG, (LHANDLE, ULONG, lpMapiMessage, ::FLAGS, ULONG))
    
    // if (mapiSendMail == nullptr)
    //     return false;
    
    // MapiMessage message = { 0 };
    // message.lpszSubject = (LPSTR) emailSubject.toRawUTF8();
    // message.lpszNoteText = (LPSTR) bodyText.toRawUTF8();
    
    // MapiRecipDesc recip = { 0 };
    // recip.ulRecipClass = MAPI_TO;
    // String targetEmailAddress_ (targetEmailAddress);
    // if (targetEmailAddress_.isEmpty())
    //     targetEmailAddress_ = " "; // (Windows Mail can't deal with a blank address)
    // recip.lpszName = (LPSTR) targetEmailAddress_.toRawUTF8();
    // message.nRecipCount = 1;
    // message.lpRecips = &recip;
    
    // HeapBlock<MapiFileDesc> files;
    // files.calloc ((size_t) filesToAttach.size());
    
    // message.nFileCount = (ULONG) filesToAttach.size();
    // message.lpFiles = files;
    
    // for (int i = 0; i < filesToAttach.size(); ++i)
    // {
    //     files[i].nPosition = (ULONG) - 1;
    //     files[i].lpszPathName = (LPSTR) filesToAttach[i].toRawUTF8();
    // }
    
    // return mapiSendMail (0, 0, &message, MAPI_DIALOG | MAPI_LOGON_UI, 0) == SUCCESS_SUCCESS;
    return false;
}

CUTE_NS_END

#endif
