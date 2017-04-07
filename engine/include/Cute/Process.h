#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Process
{
public:
	typedef pid_t PID;
	typedef std::vector<String>			Args;
	typedef std::map<String, String>	Envs;

	static void		times(long& userTime, long& kernelTime);
	static error_t	wait(handle_t handle);
	static error_t	kill(handle_t handle, int sig = SIGKILL);
	static bool		isRunning(handle_t handle);

	static pid_t	id();
	static pid_t	id(handle_t handle);

	static handle_t process();
	static handle_t process(pid_t pid);

	static handle_t launch(const String& cmd);
	static handle_t launch(const String& cmd, const Args& args);
	static handle_t launch(const String& cmd, const Args& args, const String& directory);
	static handle_t launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3]);
	static handle_t launch(const String& cmd, const Args& args, const String& directory, handle_t pipes[3], const Envs& envs);

	static bool		open(const String& file, const String& mode);
};

CUTE_NS_END
