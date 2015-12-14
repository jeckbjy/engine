#include "StringPiece.h"
#include "Algo.h"

CU_NS_BEGIN

#ifdef _WIN32
void* memrchr(const void* s, int c, size_t n)
{
	const char* beg = (const char*)s;
	const char* end = beg + n;
	while (end != beg)
	{
		if (*end == c)
			return (void*)end;
		--end;
	}
	return 0;
}

#endif

struct AsciiCaseSensitive {
	bool operator()(char lhs, char rhs) const {
		return lhs == rhs;
	}
};

struct AsciiCaseInsensitive {
	bool operator()(char lhs, char rhs) const {
		char k = lhs ^ rhs;
		if (k == 0) return true;
		if (k != 32) return false;
		k = lhs | rhs;
		return (k >= 'a' && k <= 'z');
	}
};

size_t StringPiece::find(char c, size_t off /* = 0 */) const
{
	assert(off <= m_len);
	const char* r;
	if (off == 0)
		r = (const char*)memchr(m_str, c, m_len);
	else
		r = (const char*)memchr(m_str + off, c, m_len - off);
	return r == 0 ? npos : r - m_str;
}

size_t StringPiece::find(const StringPiece& str, size_t off /* = 0 */, bool sensitive) const
{
	if (sensitive)
		return quick_find(*this, str, off, AsciiCaseSensitive());
	else
		return quick_find(*this, str, off, AsciiCaseInsensitive());
}

size_t StringPiece::rfind(char c, size_t off /* = 0 */) const
{
	assert(off <= m_len);
	const char* r = (const char*)memrchr(m_str, c, m_len - off);
	return r == 0 ? npos : r - m_str;
}

CU_NS_END