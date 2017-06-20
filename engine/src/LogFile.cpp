//! Logging
#include "Cute/LogFile.h"
#include "Cute/File.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

LogFile::LogFile(const String& filePath)
	: m_path(filePath)
	, m_file(INVALID_HANDLE_VALUE)
{

#ifdef _WIN32
	m_file = CreateFileA(m_path.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_file == INVALID_HANDLE_VALUE)
		throw OpenFileException(m_path);

	SetFilePointer(m_file, 0, 0, FILE_END);

#else
	m_file = fopen(m_path.c_str(), "a");
	if (!m_file) 
		throw OpenFileException(m_path);
#endif

	if (File::exists(filePath))
	{
		if (0 == size())
			m_creationDate = File::getLastModified(m_path);
		else
			m_creationDate = File::getCreationTime(m_path);
	}
}

LogFile::~LogFile()
{
#ifdef _WIN32
	CloseHandle(m_file);
#else
	fclose(m_file);
#endif
}

void LogFile::write(const String& text, bool flush /* = true */)
{
#ifdef _WIN32
	DWORD bytesWritten;
	BOOL res = WriteFile(m_file, text.data(), (DWORD)text.size(), &bytesWritten, NULL);
	if (!res)
		throw WriteFileException(m_path);

	res = WriteFile(m_file, "\r\n", 2, &bytesWritten, NULL);
	if (!res)
		throw WriteFileException(m_path);

	if (flush)
	{
		res = FlushFileBuffers(m_file);
		if (!res)
			throw WriteFileException(m_path);
	}
#else
	int rc = fputs(text.c_str(), m_file);
	if (rc == EOF)
		throw WriteFileException(m_path);

	rc = fputc('\n', m_file);
	if (rc == EOF)
		throw WriteFileException(m_path);
	if (flush)
	{
		rc = fflush(m_file);
		if (rc == EOF)
			throw WriteFileException(m_path);
	}
#endif
}

uint64_t LogFile::size() const
{
#ifdef _WIN32
	if (INVALID_HANDLE_VALUE == m_file)
	{
		if (File::exists(m_path))
			return File::getSize(m_path);
		else 
			return 0;
	}
	else
	{
		LARGE_INTEGER li;
		li.HighPart = 0;
		li.LowPart = SetFilePointer(m_file, 0, &li.HighPart, FILE_CURRENT);
		return li.QuadPart;
	}
#else
	return ftell(m_file);
#endif
}

CUTE_NS_END
