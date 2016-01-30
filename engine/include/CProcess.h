#pragma once
#include "API.h"

CU_NS_BEGIN

class CU_API Process
{
public:
	typedef std::vector<String>	Args;
	typedef std::map<String, String> Env;

	static pid_t currentID();
	static void times(long& userTime, long& kernelTime);
	static void kill(pid_t pid);
	static int  wait(pid_t pid);
	static bool isRunning(pid_t pid);
	static handle_t getProcess(pid_t pid);

	static pid_t launch(const String& cmd, const Args& args, const Env& env, const char* dir = NULL);
	static bool  open(const String& file, const String& mode);
};

CU_NS_END