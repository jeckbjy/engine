#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Slice
{
public:
	Slice();
	Slice(const char* str);
	Slice(const char* beg, const char* end);
	Slice(const char* str, size_t n);
	Slice(const String& str);
	
	void clear();
	void pop(size_t n);

	int compare(const Slice& ths) const;

	bool startsWith(const Slice& rhs) const;
	bool endsWith(const Slice& rhs) const;

	bool empty() const;

	const char* data() const;
	size_t		size() const;

	const char* begin() const;
	const char*	end() const;

	char operator[](size_t n) const;

	bool operator == (const Slice& rhs) const;
	bool operator != (const Slice& rhs) const;

private:
	const char* m_data;
	size_t		m_size;
};

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
inline bool Slice::empty() const
{
	return m_size == 0;
}

inline const char* Slice::data() const
{
	return m_data;
}

inline const char* Slice::begin() const
{
	return m_data;
}

inline const char* Slice::end() const
{
	return m_data + m_size;
}

inline size_t Slice::size() const
{
	return m_size;
}

inline char Slice::operator [] (size_t n) const
{
	return m_data[n];
}

inline bool Slice::operator ==(const Slice& rhs) const
{
	return ((m_size == rhs.m_size) && (memcmp(m_data, rhs.m_data, m_size) == 0));
}

inline bool Slice::operator !=(const Slice& rhs) const
{
	return !(*this == rhs);
}

CUTE_NS_END
