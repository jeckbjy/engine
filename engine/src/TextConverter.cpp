// module Text
#include "Cute/TextConverter.h"
#include "Cute/TextIterator.h"
#include "Cute/TextEncoding.h"

CUTE_NS_BEGIN

namespace {
	int nullTransform(int ch)
	{
		return ch;
	}
}

TextConverter::TextConverter(const TextEncoding& inEncoding, const TextEncoding& outEncoding, int defaultChar) 
	: m_inEncoding(inEncoding)
	, m_outEncoding(outEncoding)
	, m_defaultChar(defaultChar)
{
}

TextConverter::~TextConverter()
{
}

int TextConverter::convert(const std::string& source, std::string& destination, Transform trans)
{
	if (trans == 0)
		trans = &nullTransform;

	int errors = 0;
	TextIterator it(source, m_inEncoding);
	TextIterator end(source);
	unsigned char buffer[TextEncoding::MAX_SEQUENCE_LENGTH];

	while (it != end)
	{
		int c = *it;
		if (c == -1) { ++errors; c = m_defaultChar; }
		c = trans(c);
		int n = m_outEncoding.convert(c, buffer, sizeof(buffer));
		if (n == 0) n = m_outEncoding.convert(m_defaultChar, buffer, sizeof(buffer));
		cute_assert(n <= sizeof(buffer));
		destination.append((const char*)buffer, n);
		++it;
	}
	return errors;
}

int TextConverter::convert(const void* source, int length, std::string& destination, Transform trans)
{
	cute_check_ptr(source);

	if (trans == 0)
		trans = &nullTransform;

	int errors = 0;
	const unsigned char* it = (const unsigned char*)source;
	const unsigned char* end = (const unsigned char*)source + length;
	unsigned char buffer[TextEncoding::MAX_SEQUENCE_LENGTH];

	while (it < end)
	{
		int n = m_inEncoding.queryConvert(it, 1);
		int uc;
		int read = 1;

		while (-1 > n && (end - it) >= -n)
		{
			read = -n;
			n = m_inEncoding.queryConvert(it, read);
		}

		if (-1 > n)
		{
			it = end;
		}
		else
		{
			it += read;
		}

		if (-1 >= n)
		{
			uc = m_defaultChar;
			++errors;
		}
		else
		{
			uc = n;
		}

		uc = trans(uc);
		n = m_outEncoding.convert(uc, buffer, sizeof(buffer));
		if (n == 0) n = m_outEncoding.convert(m_defaultChar, buffer, sizeof(buffer));
		cute_assert(n <= sizeof(buffer));
		destination.append((const char*)buffer, n);
	}
	return errors;
}

CUTE_NS_END