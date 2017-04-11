#pragma once
#include "Cute/Foundation.h"
#include "Cute/Object.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Stream : public Object
{
	DECLARE_RTTI(Stream, Object, OID_ANY)
public:
	Stream();
	virtual ~Stream();

	virtual bool canWrite() const = 0;
	virtual bool canRead() const = 0;
	virtual bool canSeek() const = 0;

	virtual long write(const void* data, long size) = 0;
	virtual long read(void* data, long size) = 0;
	virtual bool seek(long offset, int origin = SEEK_SET) = 0;

	virtual void close() = 0;
	virtual bool flush() = 0;
	virtual bool rewind() = 0;
	virtual bool eof() const = 0;

	virtual long length() const = 0;
	virtual long position() const = 0;

	virtual bool readLine(String& data);
	virtual bool skipLine();

	bool read7Bit(uint64& data);
	uint write7Bit(uint64 data);

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

	template<typename T>
	bool readBig(T& t)
	{
		bool result = read(t);

#if CUTE_ENDIAN == CUTE_ENDIAN_LITTLE
		t = ByteOrder::fromBigEndian(t);
#endif
		return result;
	}

	template<typename T>
	bool readLittle(T& t)
	{
		bool result = read(t);

#if CUTE_ENDIAN == CUTE_ENDIAN_BIG
		t = ByteOrder::fromLittleEndian(t);
#endif
		return result;
	}
};

CUTE_NS_END
