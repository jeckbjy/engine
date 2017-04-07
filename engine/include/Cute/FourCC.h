#pragma once
#include "Cute/API.h"
#include "Cute/Types.h"

CUTE_NS_BEGIN

// Four-Character Codes
class CUTE_CORE_API FourCC
{
public:
	FourCC():m_code(0){}
	FourCC(int code) : m_code((uint32_t)code) {}
	FourCC(uint32_t code) : m_code(code){}
	FourCC(const char* s) : m_code(CUTE_FOURCC(s[0], s[1], s[2], s[3])) { }
	FourCC(char a, char b, char c, char d) : m_code(CUTE_FOURCC(a, b, c, d)) {}

	String toString() const;

	void operator=(uint32_t cc) { m_code = cc; }
	operator uint32_t() const { return m_code; }
	operator int() const { return m_code; }

private:
	uint32_t m_code;
};

inline String FourCC::toString() const
{
	String str;
	str.resize(4);
	str[0] = (char)((m_code & 0xFF000000) >> 24);
	str[1] = (char)((m_code & 0x00FF0000) >> 16);
	str[2] = (char)((m_code & 0x0000FF00) >> 8);
	str[3] = (char)(m_code & 0x000000FF);
	return str;
}

CUTE_NS_END

