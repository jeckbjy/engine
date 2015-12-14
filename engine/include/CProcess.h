#pragma once
#include "API.h"
#include "StringPiece.h"

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

	static pid_t launch(const StringPiece& cmd, const Args& args, const Env& env, const char* dir = NULL);
	static bool  open(const StringPiece& file, const StringPiece& mode);
};

CU_NS_END