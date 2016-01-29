#pragma once
#include "API.h"

CU_NS_BEGIN

enum AccessMode
{
	ACCESS_READ = 0x01,
	ACCESS_WRITE = 0x02,
	ACCESS_BOTH = ACCESS_READ | ACCESS_WRITE
};

// todo:大小端处理
class CU_API Stream : public Ref
{
public:
	virtual ~Stream(){}
	virtual bool canRead() const = 0;
	virtual bool canWrite() const = 0;
	virtual bool canSeek() const = 0;
	virtual uint read(void* buf, uint size, uint count) = 0;
	virtual uint write(const void* buf, uint size, uint count) = 0;
	virtual bool seek(long off, int origin = SEEK_SET) = 0;
	virtual void skip(uint len) = 0;
	virtual void close(void) = 0;
	virtual void flush(void) = 0;
	virtual uint length(void) const = 0;
	virtual uint position(void) const = 0;			// tell 相对于文件首的偏移
	virtual bool eof(void) const = 0;
	virtual const String& extension() const = 0;	// 后缀
	virtual char* readLine(char* str, int num);
	void skipWhiteSpace();							// 忽略空白字符
	void skipLine(uint len = 1);					// 忽略行
	bool readMagic(uint32& magic);					// 读取标识，最多4个字符
	uint readVariant();
	void writeVariant(uint data);

	uint read(String& data);
	uint write(const String& data);

public:
	// 移动到开头
	inline void rewind(){ seek(0, SEEK_SET); }

	inline uint read(void* buf, uint size) { return read(buf, 1, size); }
	inline uint write(const void* buf, uint size) { return write(buf, 1, size); }

	template<typename T>
	inline uint read(T& t)
	{
		return this->read(&t, sizeof(t));
	}
	template<typename T>
	inline uint write(const T& t)
	{
		return this->write(&t, sizeof(T));
	}
	template<typename T, int N>
	inline uint read(T t[N])
	{
		return this->read(t, sizeof(T) * N);
	}
	template<typename T, int N>
	inline uint write(const T t[N])
	{
		return this->write(t, sizeof(T) * N);
	}
};

CU_NS_END
