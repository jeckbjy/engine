#include "Cute/Debugger.h"
#include <sstream>
#include <cstdlib>
#include <cstdio>

#if defined(CUTE_OS_FAMILY_WINDOWS)
#include "Cute/String.h"
#elif defined(CUTE_OS_FAMILY_UNIX)
#	include <unistd.h>
#	include <signal.h>
#elif defined(CUTE_OS_FAMILY_VMS)
#	include <lib$routines.h>
#	include <ssdef.h>
#endif

CUTE_NS_BEGIN

/// Returns true if a debugger is available, false otherwise.
/// On Windows, this function uses the IsDebuggerPresent()
/// function.
/// On Unix, this function returns true if the environment
/// variable CUTE_ENABLE_DEBUGGER is set.
/// On OpenVMS, this function always returns true in debug,
/// mode, false otherwise.
bool Debugger::isAvailable()
{
#if defined(_DEBUG)
#if defined(CUTE_OS_FAMILY_WINDOWS)
#	if defined(_WIN32_WCE) && (_WIN32_WCE >= 0x600)
	BOOL isDebuggerPresent;
	if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent))
	{
		return isDebuggerPresent ? true : false;
	}
	return false;
#	else
	return IsDebuggerPresent() ? true : false;
#	endif

#elif defined(CUTE_OS == CUTE_OS_VXWORKS)
	return false;
#elif defined(CUTE_OS_FAMILY_UNIX)
	return std::getenv("CUTE_ENABLE_DEBUGGER") ? true : false;
#elif defined(CUTE_OS_FAMILY_VMS)
	return true;
#endif

#else
	return false;
#endif
}


void Debugger::message(const String& msg)
{
#if defined(_DEBUG)
	std::fputs("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n", stderr);
	std::fputs(msg.c_str(), stderr);
	std::fputs("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n", stderr);
#if defined(CUTE_OS_FAMILY_WINDOWS)
	if (isAvailable())
	{
#if defined(CUTE_WIN32_UTF8)
		std::wstring umsg;
		Strings::toUTF16(msg, umsg);
		umsg += '\n';
		OutputDebugStringW(umsg.c_str());
#else
		OutputDebugStringA(msg.c_str());
		OutputDebugStringA("\n");
#endif
	}
#elif defined(CUTE_OS_FAMILY_UNIX)
#elif defined(CUTE_OS_FAMILY_VMS)
#endif
#endif
}


void Debugger::message(const String& msg, const char* file, int line)
{
#if defined(_DEBUG)
	std::ostringstream str;
	str << msg << " [in file \"" << file << "\", line " << line << "]";
	message(str.str());
#endif
}


void Debugger::enter()
{
#if defined(_DEBUG)
#if defined(CUTE_OS_FAMILY_WINDOWS)
	if (isAvailable())
	{
		DebugBreak();
	}
#elif defined(CUTE_VXWORKS)
	{
		// not supported
	}
#elif defined(CUTE_OS_FAMILY_UNIX)
	if (isAvailable())
	{
		kill(getpid(), SIGINT);
	}
#elif defined(CUTE_OS_FAMILY_VMS)
	{
		const char* cmd = "\012SHOW CALLS";
		lib$signal(SS$_DEBUG, 1, cmd);
	}
#endif
#endif
}

/// Breaks into the debugger, if it is available.
/// On Windows, this is done using the DebugBreak() function.
/// On Unix, the SIGINT signal is raised.
/// On OpenVMS, the SS$_DEBUG signal is raised.
void Debugger::enter(const String& msg)
{
#if defined(_DEBUG)
	message(msg);
	enter();
#endif
}

void Debugger::enter(const String& msg, const char* file, int line)
{
#if defined(_DEBUG)
	message(msg, file, line);
	enter();
#endif
}

void Debugger::enter(const char* file, int line)
{
#if defined(_DEBUG)
	message("BREAK", file, line);
	enter();
#endif
}

CUTE_NS_END
