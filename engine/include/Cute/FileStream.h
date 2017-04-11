#pragma once
#include "Cute/Stream.h"

CUTE_NS_BEGIN

enum class FileMode
{
	Append,
	Create,
	CreateNew,
	Open,
	OpenOrCreate,
	Truncate,
};

enum class FileAccess
{
	Read,
	Write,
	ReadWrite,
};

class CUTE_CORE_API FileStream : public Stream
{
public:
	FileStream();
	FileStream(const String& path, const char* mode = "r");
	~FileStream();

	bool canWrite() const;
	bool canRead() const;
	bool canSeek() const;

	long write(const void* data, long size);
	long read(void* data, long size);
	bool seek(long offset, int origin = SEEK_SET);

	bool open(const String& path, const char* mode = "r");
	void close();
	bool flush();
	bool rewind();
	bool eof() const;

	long length() const;
	long position() const;

protected:
	FILE* m_file;
	bool  m_canRead;
	bool  m_canWrite;
};

CUTE_NS_END
