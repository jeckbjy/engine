#pragma once
#include "Types.h"

CU_NS_BEGIN

// 注意:不一定以\0结尾
class CU_API StringPiece
{
protected:
	typedef const char* type_t;
	type_t m_str;
	size_t m_len;

public:
	typedef char value_type;
	static const size_t npos = (size_t)-1;

	StringPiece() 
		: m_str(0), m_len(0){}
	StringPiece(const char* str, size_t len = 0) 
		: m_str(str), m_len(len ? len : strlen(str)){}
	StringPiece(const uint8_t* str, size_t len = 0) 
		: m_str((const char*)str), m_len(len ? len : strlen(m_str)){}
	StringPiece(const String& str, size_t len = 0) 
		: m_str(str.data()), m_len(len ? len : str.size()){}

	void set(const char* str){ m_str = str; m_len = strlen(str); }
	void set(const char* str, size_t len) { m_str = str; m_len = len; }
	void set(const void* buf, size_t len) { m_str = (type_t)buf; m_len = len; }
	void set(const String& str, size_t len = 0) { m_str = str.data(); m_len = len ? len : str.size(); }

	type_t begin() const { return m_str; }
	type_t end()  const { return m_str + m_len; }
	type_t data() const { return m_str; }
	size_t size() const { return m_len; }
	bool empty() const { return m_len == 0; }
	void clear() { m_str = 0; m_len = 0; }

	char back() const { return m_str[m_len - 1]; }
	char at(size_t index) const { return m_str[index]; }
	char back_at(size_t index) const { return m_str[m_len - index - 1]; }

	size_t find(char c, size_t off = 0) const;
	size_t find(const StringPiece& str, size_t off = 0, bool sensitive = false) const;
	size_t rfind(char c, size_t off = 0) const;
	size_t rfind(const StringPiece& str, size_t off = 0) const;
	size_t find_first_of(const StringPiece& str, size_t off = 0) const;

	StringPiece slice(size_t first, size_t length = npos) const { return StringPiece(m_str + first, std::min(length, m_len - first)); }
	StringPiece split_step(const StringPiece& delimiter, bool ignore = false);
	StringPiece split_step(char delimter, bool ignore = false);

	bool startsWith(const StringPiece& rhs) const;
	bool endsWith(const StringPiece& rhs) const;

	String toString() const { return m_str ? String(m_str, m_len) : String(); }

	int compare(const StringPiece& rhs) const;

	bool operator==(const StringPiece& rhs) const { return m_len == rhs.m_len && memcmp(m_str, rhs.m_str, m_len) == 0; }
	bool operator!=(const StringPiece& rhs) const { return !(*this == rhs); }
	bool operator< (const StringPiece& rhs) const { return compare(rhs) <  0; }
	bool operator<=(const StringPiece& rhs) const { return compare(rhs) <= 0; }
	bool operator> (const StringPiece& rhs) const { return compare(rhs) >  0; }
	bool operator>=(const StringPiece& rhs) const { return compare(rhs) >= 0; }

	char operator[](size_t index) const { return m_str[index]; }
};

inline bool StringPiece::startsWith(const StringPiece& rhs) const
{
	return m_len >= rhs.m_len && memcmp(m_str, rhs.m_str, rhs.m_len) == 0;
}
inline bool StringPiece::endsWith(const StringPiece& rhs) const
{
	return m_len >= rhs.m_len && memcmp(m_str + m_len - rhs.m_len, rhs.m_str, rhs.m_len) == 0;
}

inline int StringPiece::compare(const StringPiece& rhs) const 
{
	int r = memcmp(m_str, rhs.m_str, m_len <= rhs.m_len ? m_len : rhs.m_len);
	if (r == 0){
		if (m_len < rhs.m_len) r = -1;
		else if (m_len > rhs.m_len) r = 1;
	}
	return r;
}

CU_NS_END