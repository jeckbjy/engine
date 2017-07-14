//! Process
#include "Cute/Platform.h"
#include "Cute/Ref.h"

#if defined(CUTE_OS_FAMILY_POSIX)
#include "Cute/Process.h"
#include "Cute/Exception.h"

#if defined(CUTE_OS_MAC)
#include <sys/sysctl.h>
#endif

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// ChildProcess
//////////////////////////////////////////////////////////////////////////
class ChildProcessImpl : public Ref<ChildProcessImpl>
{
public:
    ChildProcessImpl(pid_t pid): m_pid(pid) {}
    
public:
    pid_t m_pid;
};

ChildProcess::ChildProcess()
    : m_impl(NULL)
{
}

ChildProcess::~ChildProcess()
{
    if(m_impl != NULL)
        m_impl->decRef();
}

int ChildProcess::wait() const
{
    if (m_impl == 0)
        return 0;
    
    return 0;
}

void ChildProcess::kill()
{
    if(m_impl)
        Process::kill(m_impl->m_pid);
}

bool ChildProcess::isRunning() const
{
    if(m_impl)
        return Process::isRunning(m_impl->m_pid);
    else
        return false;
}

bool ChildProcess::launch(const String& command, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs)
{
    // argv
    std::vector<char*> argv(args.size() + 2);
    int i = 0;
    argv[i++] = const_cast<char*>(command.c_str());
    for (Args::const_iterator it = args.begin(); it != args.end(); ++it)
    {
        argv[i++] = const_cast<char*>(it->c_str());
    }
    argv[i] = NULL;
    
    // env
    std::vector<char> envChars;
    if (!envs.empty())
    {
        envChars = GetEnvironmentBuffer(envs);
    }
    
#ifdef __QNX__
    if (initialDirectory.empty())
    {
        /// use QNX's spawn system call which is more efficient than fork/exec.
        struct inheritance inherit;
        std::memset(&inherit, 0, sizeof(inherit));
        inherit.flags = SPAWN_ALIGN_DEFAULT | SPAWN_CHECK_SCRIPT | SPAWN_SEARCH_PATH;
        int fdmap[3];
        if (pipes != NULL)
        {
            fdmap[0] = pipes[0] ? pipes[0] : 0;
            fdmap[1] = pipes[1] ? pipes[1] : 1;
            fdmap[2] = pipes[2] ? pipes[2] : 2;
        }
        else
        {
            fdmap[0] = 0;
            fdmap[1] = 1;
            fdmap[2] = 2;
        }
        
        char** envPtr = 0;
        std::vector<char*> envPtrs;
        if (!envs.empty())
        {
            envPtrs.reserve(envs.size() + 1);
            char* p = &envChars[0];
            while (*p)
            {
                envPtrs.push_back(p);
                while (*p) ++p;
                ++p;
            }
            envPtrs.push_back(0);
            envPtr = &envPtrs[0];
        }
        
        int pid = spawn(command.c_str(), 3, fdmap, &inherit, argv, envPtr);
        if (pid == -1)
            return false;
//            throw SystemException("cannot spawn", command);
        
        m_impl = new ChildProcessImpl(pid);
        m_impl->incRef();
        return true;
        //    return pid;
    }
#endif
    
    // user fork
    const char* pInitialDirectory = directory.empty() ? 0 : directory.c_str();
    
    int pid = fork();
    if (pid < 0)
    {
        return false;
//        throw SystemException("Cannot fork process for", command);
    }
    else if (pid == 0)
    {
        if (pInitialDirectory)
        {
            if (chdir(pInitialDirectory) != 0)
            {
                _exit(72);
            }
        }
        
        // set environment variables
        char* p = &envChars[0];
        while (*p)
        {
            putenv(p);
            while (*p) ++p;
            ++p;
        }
        
        if (pipes != NULL)
        {
            // setup redirection
            if (pipes[0] != 0)
            {
                dup2(pipes[0], STDIN_FILENO);
                close(pipes[0]);
            }
            
            // outPipe and errPipe may be the same, so we dup first and close later
            if (pipes[1] != 0)
                dup2(pipes[1], STDOUT_FILENO);
            
            if (pipes[2] != 0)
                dup2(pipes[2], STDERR_FILENO);
            
            if (pipes[1] != 0)
                close(pipes[1]);
            
            if (pipes[2] != 0)
                close(pipes[2]);
        }
        
        // close all open file descriptors other than stdin, stdout, stderr
        for (int fd = 3; fd < sysconf(_SC_OPEN_MAX); ++fd)
        {
            close(fd);
        }
        
        execvp(argv[0], &argv[0]);
        _exit(72);
    }
    
    m_impl = new ChildProcessImpl(pid);
    m_impl->incRef();
    //  return pid;
    return true;
}

//////////////////////////////////////////////////////////////////////////
// process
//////////////////////////////////////////////////////////////////////////
void Process::times(long& userTime, long& kernelTime)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    userTime   = usage.ru_utime.tv_sec;
    kernelTime = usage.ru_stime.tv_sec;
}

pid_t Process::id()
{
    return getpid();
}

void Process::kill(PID pid)
{
    if (::kill(pid, SIGKILL) != 0)
    {
        switch (errno)
        {
            case ESRCH:
                throw NotFoundException("cannot kill process");
            case EPERM:
                throw NoPermissionException("cannot kill process");
            default:
                throw SystemException("cannot kill process");
        }
    }
}

bool Process::isRunning(PID pid)
{
    return ::kill(pid, 0) == 0;
}

void Process::terminate()
{
    std::exit(EXIT_FAILURE);
    // #if defined(CUTE_OS_ANDROID)
    //     _exit(EXIT_FAILURE);
    // #else
    //     std::_Exit(EXIT_FAILURE);
    // #endif
}

void Process::hide()
{
    // not support
}

void Process::setPriority(int prior)
{
    const int policy = (prior <= PRIO_NORMAL) ? SCHED_OTHER : SCHED_RR;
    const int minp = sched_get_priority_min (policy);
    const int maxp = sched_get_priority_max (policy);
    
    struct sched_param param;
    
    switch (prior)
    {
        case PRIO_LOW:
        case PRIO_NORMAL:    param.sched_priority = 0; break;
        case PRIO_HIGH:      param.sched_priority = minp + (maxp - minp) / 4; break;
        case PRIO_REALTIME:  param.sched_priority = minp + (3 * (maxp - minp) / 4); break;
        default: assert(false); break;
    }
    
    pthread_setschedparam (pthread_self(), policy, &param);
}

void Process::setForeground()
{
    // not support
}

bool Process::isForeground()
{
    return false;
}

bool Process::isDebuggerPresent()
{
#if defined(CUTE_OS_MAC)
    struct kinfo_proc info;
    int m[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    size_t sz = sizeof (info);
    sysctl (m, 4, &info, &sz, 0, 0);
    return (info.kp_proc.p_flag & P_TRACED) != 0;
#elif defined(CUTE_OS_FAMILY_LINUX)
    // StringArray lines;
    // File ("/proc/self/status").readLines (lines);
    
    // for (int i = lines.size(); --i >= 0;) // (NB - it's important that this runs in reverse order)
    //     if (lines[i].upToFirstOccurrenceOf (":", false, false).trim().equalsIgnoreCase ("TracerPid"))
    //         return (lines[i].fromFirstOccurrenceOf (":", false, false).trim().getIntValue() > 0);
    
    // return false;
#else
    return false;
#endif
}

bool Process::isRunningInWine()
{
    return false;
}

bool Process::open(const String& url, const String& params)
{
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
    return false;
}

bool Process::sendmail(const String& addr, const String& subject, const String& body, const StringArray& attaches)
{
    return false;
}

CUTE_NS_END

#endif
