#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// The Debugger class provides an interface to the debugger.
/// The presence of a debugger can be checked for,
/// messages can be written to the debugger's log window
/// and a break into the debugger can be enforced.
/// The methods only work if the program is compiled
/// in debug mode (the macro _DEBUG is defined).
class CUTE_CORE_API Debugger
{
public:
	static bool isAvailable();

	static void message(const String& msg);
	static void message(const String& msg, const char* file, int line);

	static void enter();
	static void enter(const String& msg);
	static void enter(const String& msg, const char* file, int line);
	static void enter(const char* file, int line);
};

CUTE_NS_END
