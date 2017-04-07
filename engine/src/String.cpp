#include "Cute/String.h"

CUTE_NS_BEGIN

void format(String& result, const char* fmt, va_list& va)
{
	int size = vsnprintf(0, 0, fmt, va);
	if (size > 0)
	{
		result.resize(size + 1);
		size = vsnprintf(&result[0], size + 1, fmt, va);
		if (size == -1)
			result.clear();
		else
			result[size] = '\0';
	}
}

void format(String& result, const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	format(result, fmt, va);
	va_end(va);
}

String format(const char* fmt, ...)
{
	std::string result;
	va_list va;
	va_start(va, fmt);
	format(result, fmt, va);
	va_end(va);
	return result;
}

CUTE_NS_END