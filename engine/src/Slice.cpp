//! Core
#include "Cute/Slice.h"

CUTE_NS_BEGIN

Slice::Slice()
	: m_data("")
	, m_size(0)
{
}

Slice::Slice(const char* str)
	: m_data(str)
	, m_size(strlen(str))
{
}

Slice::Slice(const char* beg, const char* end)
	: m_data(beg)
	, m_size(end - beg)
{
}

Slice::Slice(const char* str, size_t n)
	: m_data(str)
	, m_size(n)
{
}

Slice::Slice(const String& str)
	: m_data(str.c_str())
	, m_size(str.size())
{
}

void Slice::clear()
{
	m_data = "";
	m_size = 0;
}

void Slice::pop(size_t n)
{
	assert(n <= m_size);
	m_data += n;
	m_size -= n;
}

bool Slice::startsWith(const Slice& rhs) const
{
	return (m_size >= rhs.m_size) && (memcmp(m_data, rhs.m_data, rhs.m_size) == 0);
}

bool Slice::endsWith(const Slice& rhs) const
{
	return (m_size >= rhs.m_size) && (memcmp(m_data + m_size - rhs.m_size, rhs.m_data, rhs.m_size) == 0);
}

int Slice::compare(const Slice& b) const
{
	const size_t min_len = (m_size < b.m_size) ? m_size : b.m_size;
	int r = memcmp(m_data, b.m_data, min_len);
	if (r == 0) 
	{
		if (m_size < b.m_size)
			r = -1;
		else if (m_size > b.m_size) 
			r = +1;
	}

	return r;
}

CUTE_NS_END
