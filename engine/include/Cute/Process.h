#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class ChildProcessImpl;
class CUTE_CORE_API ChildProcess
{
public:
    typedef StringArray                Args;
    typedef std::map<String, String>   Envs;
    ChildProcess();
    ~ChildProcess();

    pid_t id() const;
    int   wait() const;
    void  kill();
    bool  isRunning() const;

private:
    bool  launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs);

private:
    std::vector<char> GetEnvironmentBuffer(const std::map<String, String>& env);
    typedef ChildProcessImpl Impl;
    friend class Process;
    Impl* m_impl;
};

class CUTE_CORE_API Process
{
public:
    typedef pid_t                      PID;
    typedef StringArray                Args;
    typedef std::map<String, String>   Envs;

    enum Priority
    {
        PRIO_LOW,
        PRIO_NORMAL,
        PRIO_HIGH,
        PRIO_REALTIME,
    };

    static void times(long& userTime, long& kernelTime);
    static PID  id();
    static void kill(PID pid);
    static bool isRunning(PID pid);

    static void terminate();
    static void hide();

    static void setPriority(int prio);
    static void setForeground();
    static bool isForeground();

    static bool isDebuggerPresent();
    static bool isRunningInWine();

    static bool open(const String& url, const String& params);
    static bool sendmail(const String& addr, const String& subject, const String& body, const StringArray& attaches);

    static ChildProcess launch(const String& cmd);
    static ChildProcess launch(const String& cmd, const Args& args);
    static ChildProcess launch(const String& cmd, const Args& args, const String& directory);
    static ChildProcess launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3]);
    static ChildProcess launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs);
};

//
// inlines
//
inline  ChildProcess Process::launch(const String& cmd)
{
    return launch(cmd, Args());
}

inline ChildProcess Process::launch(const String& cmd, const Args& args)
{
    return launch(cmd, args, String());
}

inline ChildProcess Process::launch(const String& cmd, const Args& args, const String& directory)
{
    return launch(cmd, args, directory, NULL);
}

inline ChildProcess Process::launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3])
{
    return launch(cmd, args, directory, pipes, Envs());
}

inline ChildProcess Process::launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs)
{
    ChildProcess process;
    process.launch(cmd, args, directory, pipes, envs);
    return process;
}

//class CUTE_CORE_API Process
//{
//public:
//  typedef pid_t PID;
//  typedef std::vector<String>         Args;
//  typedef std::map<String, String>    Envs;
//
//  static void     times(long& userTime, long& kernelTime);
//  static error_t  wait(handle_t handle);
//  static error_t  kill(handle_t handle, int sig = SIGKILL);
//  static bool     isRunning(handle_t handle);
//
//  static pid_t    id();
//  static pid_t    id(handle_t handle);
//
//  static handle_t process();
//  static handle_t process(pid_t pid);
//
//  static handle_t launch(const String& cmd);
//  static handle_t launch(const String& cmd, const Args& args);
//  static handle_t launch(const String& cmd, const Args& args, const String& directory);
//  static handle_t launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3]);
//  static handle_t launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs);
//
//  static bool     open(const String& file, const String& mode);
//};

CUTE_NS_END
