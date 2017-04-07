//! Stream
#include "Cute/FileStream.h"

CUTE_NS_BEGIN

FileStream::FileStream()
{

}

FileStream::FileStream(const String& path, int mode /* = 0 */)
{
}

FileStream::~FileStream()
{

}

bool FileStream::canRead() const
{
	return true;
}

bool FileStream::canWrite() const
{
	return true;
}

bool FileStream::canSeek() const
{
	return true;
}

bool FileStream::open(const String& path)
{
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

void FileStream::flush()
{
	if (m_file)
		fflush(m_file);
}

bool FileStream::seek(long offset, int origin)
{
	return fseek(m_file, offset, origin) != -1;
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