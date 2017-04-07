#pragma once
#include "Cute/Foundation.h"
#include "Cute/Timestamp.h"

CUTE_NS_BEGIN

class CUTE_CORE_API LogFile
{
public:
	LogFile(const String& filePath);
	~LogFile();

	void write(const String& text, bool flush = true);

	uint64_t size() const;
	Timestamp creationDate() const;
	const String& path() const;

private:
#ifdef _WIN32
	typedef HANDLE	file_t;
#else
	typedef FILE*	file_t;
#endif
	file_t		m_file;
	String		m_path;
	Timestamp	m_creationDate;
};

inline Timestamp LogFile::creationDate() const
{
	return m_creationDate;
}

inline const String& LogFile::path() const
{
	return m_path;
}

CUTE_NS_END