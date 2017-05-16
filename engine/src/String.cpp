#include "Cute/String.h"
#include "Cute/TextIterator.h"
#include "Cute/TextConverter.h"
#include "Cute/UTF8Encoding.h"
#include "Cute/UTF16Encoding.h"
#include "Cute/UTF32Encoding.h"

CUTE_NS_BEGIN

const String Strings::BLANK;

String Strings::format(const char* fmt, ...)
{
	std::string result;
	va_list va;
	va_start(va, fmt);
	format(result, fmt, va);
	va_end(va);
	return result;
}

void Strings::format(String& result, const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	format(result, fmt, va);
	va_end(va);
}

void Strings::format(String& result, const char* fmt, va_list& va)
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

void Strings::toUTF8(const UTF16String& from, UTF8String& to)
{
	to.clear();
	UTF16Encoding encodingIn;
	UTF8Encoding  encodingOut;
	TextConverter converter(encodingIn, encodingOut);
	converter.convert(from.data(), (int)from.length() * sizeof(UTF16Char), to);
}

void Strings::toUTF8(const UTF32String& from, UTF8String& to)
{
	to.clear();
	UTF32Encoding encodingIn;
	UTF8Encoding  encodingOut;
	TextConverter converter(encodingIn, encodingOut);
	converter.convert(from.data(), (int)from.length() * sizeof(UTF32Char), to);
}

void Strings::toUTF16(const UTF8String& from, UTF16String& to)
{
	to.clear();
	to.reserve(from.size());
	UTF8Encoding encoding;
	TextIterator itor(from, encoding);
	while (itor)
	{
		int cc = *itor++;
		if (cc <= 0xffff)
		{
			to += (UTF16Char)cc;
		}
		else
		{
			cc -= 0x10000;
			to += (UTF16Char)((cc >> 10) & 0x3ff) | 0xd800;
			to += (UTF16Char)(cc & 0x3ff) | 0xdc00;
		}
	}
}

void Strings::toUTF32(const UTF8String& from, UTF32String& to)
{
	to.clear();
	to.reserve(from.size());
	UTF8Encoding encoding;
	TextIterator itor(from, encoding);
	while (itor)
	{
		int cc = *itor++;
		to += (UTF32Char)cc;
	}
}

CUTE_NS_END
