#pragma once
#include "API.h"

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
class CU_API File
{
public:
	File();
	File(const String& path, int mode = FMODE_OPEN_CREATE);
	~File();
	
	uint read(void* dst, size_t len);
	uint write(const void* data, size_t len);

	bool open(const String& path, int mode);
	void close();
	void flush();
	void seek(long pos);
	bool is_open() const;
	uint size() const { return m_size; }
	handle_t native() const { return m_handle; }
private:
	FILE*	m_handle;
	uint	m_size;
};

CU_NS_END