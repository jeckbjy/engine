#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API SharedMemory
{
public:
	enum AccessMode
	{
		AM_READ = 0,
		AM_WRITE
	};

	SharedMemory(const String& name, size_t size, AccessMode mode, const void* addrHint = 0, bool server = true);
	SharedMemory(const String& file, AccessMode mode, const void* addrHint = 0);
	~SharedMemory();

	void swap(SharedMemory& other);
	char* begin() const;
	char* end() const;

private:
	void map(const void* addrHint);
	void unmap();
	void close();

private:
	SharedMemory(const SharedMemory& other);
	SharedMemory& operator = (const SharedMemory& other);

private:
	String	m_name;
	char*	m_address;
	size_t	m_size;
	int 	m_mode;
	handle_t m_handle;
#ifdef _WIN32
	HANDLE	m_file;
#else
	bool	m_fileMapped;
	bool	m_server;
#endif
};

// inlines
inline char* SharedMemory::begin() const
{
	return m_address;
}

inline char* SharedMemory::end() const
{
	return m_address + m_size;
}

CUTE_NS_END
