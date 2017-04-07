#pragma once
#include "Cute/Stream.h"

CUTE_NS_BEGIN

class CUTE_CORE_API FileStream : public Stream
{
public:
	FileStream();
	FileStream(const String& path, int mode = 0);
	~FileStream();

	bool canRead() const;
	bool canWrite() const;
	bool canSeek() const;

	long read(void* buffer, long count);
	long write(const void* buffer, long count);
	bool seek(long offset, int origin);

	bool open(const String& path);
	void close();
	void flush();

	long length() const;
	long position() const;
	bool eof() const;

protected:
	FILE* m_file;
};

CUTE_NS_END