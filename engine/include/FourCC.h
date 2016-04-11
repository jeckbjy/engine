#pragma once
#include "Types.h"
#include "Macro.h"

CU_NS_BEGIN

class FourCC
{
public:
	FourCC() :m_fourCC(0){}
	FourCC(int cc) :m_fourCC((uint32_t)cc){}
	FourCC(uint32_t cc) :m_fourCC(cc){}
	FourCC(const char* cc){ set(cc); }
	FourCC(char a, char b, char c, char d){ set(a, b, c, d); }

	void set(const char* s)
	{
		set(s[0], s[1], s[2], s[3]); 
	}
	void set(char a, char b, char c, char d){ m_fourCC = CU_FOURCC(a, b, c, d); }

	String toString() const;

	void operator=(uint32_t cc){ m_fourCC = cc; }
	void operator=(const char* cc) { set(cc); }
	operator uint32_t() const { return m_fourCC; }

private:
	uint32_t m_fourCC;
};

inline String FourCC::toString() const
{
	String str;
	str.resize(4);
	str[0] = (char)((m_fourCC & 0xFF000000) >> 24);
	str[1] = (char)((m_fourCC & 0x00FF0000) >> 16);
	str[2] = (char)((m_fourCC & 0x0000FF00) >> 8);
	str[3] = (char)(m_fourCC & 0x000000FF);
	return str;
}

CU_NS_END