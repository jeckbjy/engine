#pragma once
#include "Cute/Foundation.h"
#include "Cute/Object.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Stream : public Object
{
public:
	Stream();
	virtual ~Stream();

	virtual bool canRead() const = 0;
	virtual bool canWrite() const = 0;
	virtual bool canSeek() const = 0;

	virtual long read(void* buffer, long count) = 0;
	virtual long write(const void* buffer, long count) = 0;
	virtual bool seek(long offset, int origin = SEEK_SET) = 0;
	virtual void close() = 0;
	virtual void flush() = 0;
	virtual long length() const = 0;
	virtual long position() const = 0;
	virtual void rewind();
	virtual bool eof() const;
	virtual bool readLine(String& data);
	virtual bool skipLine();

	bool read7Bit(uint64_t& data);
	uint write7Bit(uint64_t data);

	bool read8Bit(uint64_t& data, size_t count);
	uint write8Bit(uint64_t data);

	template<typename T>
	bool read(T& t)
	{
		return this->read(&t, sizeof(T)) == 0;
	}

	template<typename T>
	bool write(const T& t)
	{
		return this->write(&t, sizeof(T)) == 0;
	}

	long size() const
	{
		return length();
	}
};

CUTE_NS_END