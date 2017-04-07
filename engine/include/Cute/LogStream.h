#pragma once
#include "Cute/Foundation.h"
#include <sstream>

CUTE_NS_BEGIN

// 可以有一个分隔符
class CUTE_CORE_API LogStream
{
public:
	LogStream();
	~LogStream();

	LogStream& operator << (const int8_t& value);
	LogStream& operator << (const uint8_t& value);

	void clear();
	String str() const;

private:
	char				m_separator;
	std::stringstream	m_stream;
};

inline void LogStream::clear()
{
	m_stream.str().clear();
}

inline String LogStream::str() const
{
	return m_stream.str();
}

CUTE_NS_END