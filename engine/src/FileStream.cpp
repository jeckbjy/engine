//! Stream
#include "Cute/FileStream.h"

CUTE_NS_BEGIN

FileStream::FileStream()
	: m_file(NULL)
	, m_canRead(false)
	, m_canWrite(false)
{
}

FileStream::FileStream(const String& path, const char* mode /* = "r" */)
	: m_file(NULL)
	, m_canRead(false)
	, m_canWrite(false)
{
	open(path, mode);
}

FileStream::~FileStream()
{
	close();
}

bool FileStream::canRead() const
{
	return m_file != NULL && m_canRead;
}

bool FileStream::canWrite() const
{
	return m_file != NULL && m_canWrite;
}

bool FileStream::canSeek() const
{
	return m_file != NULL;
}

bool FileStream::open(const String& path, const char* mode)
{
	m_file = fopen(path.c_str(), mode);
	if (m_file)
	{
		const char* s = mode;
		while (s != NULL && *s != '\0')
		{
			if (*s == 'r')
				m_canRead = true;
			else if (*s == 'w')
				m_canWrite = true;
			//else if (*s == '+')
			//{
			//	m_canRead = true;
			//	m_canWrite = true;
			//}
			++s;
		}
	}

	return true;
}

void FileStream::close()
{
	if (m_file != 0)
	{
		fclose(m_file);
		m_file = 0;
	}
}

bool FileStream::flush()
{
	if (m_file)
		return fflush(m_file) == 0;

	return false;
}

bool FileStream::rewind()
{
	if (m_file)
	{
		::rewind(m_file);
		return true;
	}

	return false;
}

bool FileStream::seek(long offset, int origin)
{
	return fseek(m_file, offset, origin) == 0;
}

long FileStream::read(void* buffer, long count)
{
	return fread(buffer, 1, count, m_file);
}

long FileStream::write(const void* buffer, long count)
{
	return fwrite(buffer, 1, count, m_file);
}

long FileStream::length() const
{
	long pos = position();
	fseek(m_file, 0, SEEK_END);
	long len = ftell(m_file);
	fseek(m_file, pos, SEEK_SET);
	return len;
}

long FileStream::position() const
{
	return ftell(m_file);
}

bool FileStream::eof() const
{
	return feof(m_file) != 0;
}

CUTE_NS_END