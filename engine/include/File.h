#pragma once
#include "API.h"
#include "Stream.h"

CU_NS_BEGIN

enum FileMode
{
	FMODE_CREATE,
	FMODE_APPEND,
	FMODE_OPEN,
	FMODE_OPEN_CREATE,
	FMODE_TRUNCATE,
};

enum FileAccess
{
	FACCESS_READ,
	FACCESS_WRITE,
	FACCESS_READ_WRITE,
};

enum FileShare
{
	FSHARE_DELETE,
	FSHARE_INHERITABLE,
	FSHARE_NONE,
	FSHARE_READ,
	FSHARE_WRITE,
	FSHARE_READ_WRITE,
};

// 打开文件并自动过滤bom
class CU_API File : public Stream
{
public:
	File();
	File(const String& path, int mode = FMODE_OPEN_CREATE);
	~File();
	
	uint read(void* buf, uint size, uint count);
	uint write(const void* buf, uint size, uint count);

	bool open(const String& path, int mode = 0);
	void close();
	void flush();
	bool seek(long offset, int origin);
	bool is_open() const;
	uint position() const;
	uint length() const { return m_size; }
	handle_t native() const { return m_handle; }
private:
	FILE*	m_handle;
	uint	m_size;
};

CU_NS_END