#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class TextEncoding;

class CUTE_CORE_API TextConverter
{
public:
	typedef int(*Transform)(int);

	TextConverter(const TextEncoding& inEncoding, const TextEncoding& outEncoding, int defaultChar = '?');
	~TextConverter();

	/// Converts the source string from inEncoding to outEncoding
	/// and appends the result to destination. Every character is
	/// passed to the transform function.
	/// If a character cannot be represented in outEncoding, defaultChar
	/// is used instead.
	/// Returns the number of encoding errors (invalid byte sequences
	/// in source).
	int convert(const String& source, String& destination, Transform trans = 0);
	int convert(const void* source, int length, String& destination, Transform trans = 0);

private:
	TextConverter();
	TextConverter(const TextConverter&);
	TextConverter& operator = (const TextConverter&);

	const TextEncoding& m_inEncoding;
	const TextEncoding& m_outEncoding;
	int                 m_defaultChar;
};

CUTE_NS_END