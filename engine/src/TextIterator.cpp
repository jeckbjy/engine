// module Text
#include "Cute/TextIterator.h"
#include "Cute/TextEncoding.h"
#include <algorithm>

CUTE_NS_BEGIN

TextIterator::TextIterator()
	: m_encoding(0)
{
}

TextIterator::TextIterator(const String& str, const TextEncoding& encoding)
	: m_encoding(&encoding)
	, m_itor(str.begin())
	, m_end(str.end())
{
}

TextIterator::TextIterator(const StringIterator& begin, const StringIterator& end, const TextEncoding& encoding)
	: m_encoding(&encoding)
	, m_itor(begin)
	, m_end(end)
{
}

TextIterator::TextIterator(const String& str)
	: m_encoding(0)
	, m_itor(str.end())
	, m_end(str.end())
{
}

TextIterator::~TextIterator()
{
}

TextIterator::TextIterator(const TextIterator& it)
	: m_encoding(it.m_encoding)
	, m_itor(it.m_itor)
	, m_end(it.m_end)
{
}

TextIterator& TextIterator::operator = (const TextIterator& it)
{
	if (&it != this)
	{
		m_encoding = it.m_encoding;
		m_itor = it.m_itor;
		m_end = it.m_end;
	}

	return *this;
}

void TextIterator::swap(TextIterator& it)
{
	std::swap(m_encoding, it.m_encoding);
	std::swap(m_itor, it.m_itor);
	std::swap(m_end, it.m_end);
}

TextIterator TextIterator::end() const
{
	TextIterator it;
	it.m_itor = m_end;
	it.m_end = m_end;
	return it;
}

int TextIterator::operator * () const
{
	cute_check_ptr(m_encoding);
	cute_assert(m_itor != m_end);
	String::const_iterator it = m_itor;

	unsigned char buffer[TextEncoding::MAX_SEQUENCE_LENGTH];
	unsigned char* p = buffer;

	if (it != m_end)
		*p++ = *it++;
	else
		*p++ = 0;

	int read = 1;
	int n = m_encoding->queryConvert(buffer, 1);

	while (-1 > n && (m_end - it) >= -n - read)
	{
		while (read < -n && it != m_end)
		{
			*p++ = *it++;
			read++;
		}
		n = m_encoding->queryConvert(buffer, read);
	}

	if (-1 > n)
	{
		return -1;
	}
	else
	{
		return n;
	}
}

TextIterator& TextIterator::operator ++ ()
{
	cute_check_ptr(m_encoding);
	cute_assert(m_itor != m_end);

	unsigned char buffer[TextEncoding::MAX_SEQUENCE_LENGTH];
	unsigned char* p = buffer;

	if (m_itor != m_end)
		*p++ = *m_itor++;
	else
		*p++ = 0;

	int read = 1;
	int n = m_encoding->sequenceLength(buffer, 1);

	while (-1 > n && (m_end - m_itor) >= -n - read)
	{
		while (read < -n && m_itor != m_end)
		{
			*p++ = *m_itor++;
			read++;
		}
		n = m_encoding->sequenceLength(buffer, read);
	}
	while (read < n && m_itor != m_end)
	{
		m_itor++;
		read++;
	}

	return *this;
}

TextIterator TextIterator::operator ++ (int)
{
	TextIterator prev(*this);
	operator ++ ();
	return prev;
}

CUTE_NS_END