//! Process
#include "Cute/SharedMemory.h"
#include "Cute/String.h"
#include "Cute/Files.h"
#include "Cute/Error.h"
#include "Cute/Exception.h"

#ifndef _WIN32
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <sys/mman.h>
#	include <fcntl.h>
#	include <unistd.h>
#endif

CUTE_NS_BEGIN

SharedMemory::SharedMemory(const String& name, size_t size, AccessMode mode, const void* addrHint /* = 0 */, bool server /* = true */)
	: m_address(0)
	, m_size(size)
	, m_mode(mode)
	, m_handle(INVALID_HANDLE_VALUE)
{
#ifdef _WIN32
	m_name = name;
	m_file = INVALID_HANDLE_VALUE;

	DWORD pageMode = (mode == AM_WRITE ? PAGE_READWRITE : PAGE_READONLY);
	m_handle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, pageMode, 0, (DWORD)m_size, m_name.c_str());

	if (!m_handle)
	{
		DWORD dwRetVal = GetLastError();
		if (pageMode != PAGE_READONLY || dwRetVal != 5)
			throw SystemException(Strings::format("Cannot create shared memory object %s [Error %d: %s]", m_name, static_cast<int>(dwRetVal), Error::getMessage(dwRetVal)));

		m_handle = OpenFileMappingA(PAGE_READONLY, FALSE, m_name.c_str());
		if (!m_handle)
		{
			dwRetVal = GetLastError();
			throw SystemException(Strings::format("Cannot open shared memory object %s [Error %d: %s]", m_name, static_cast<int>(dwRetVal), Error::getMessage(dwRetVal)));
		}
	}

	map(addrHint);

#else
	m_name = "/";
	m_fileMapped = false;
	m_server = server;

#if defined(CUTE_OS_HPUX)
	m_name.append("tmp/");
#endif
	m_name.append(name);

	int flags = m_server ? O_CREAT : 0;
	if (m_mode == SharedMemory::AM_WRITE)
		flags |= O_RDWR;
	else
		flags |= O_RDONLY;

	// open the shared memory segment
	m_handle = ::shm_open(m_name.c_str(), flags, S_IRUSR | S_IWUSR);
	if (m_handle == -1)
		throw SystemException("Cannot create shared memory object", m_name);

	// now set the correct size for the segment
	if (m_server && -1 == ::ftruncate(m_handle, size))
	{
		::close(m_handle);
		m_handle = -1;
		::shm_unlink(m_name.c_str());
		throw SystemException("Cannot resize shared memory object", m_name);
	}
	map(addrHint);
#endif
}

SharedMemory::SharedMemory(const String& file, AccessMode mode, const void* addrHint)
	: m_name(file)
	, m_address(0)
	, m_size(0)
	, m_mode(mode)
	, m_handle(INVALID_HANDLE_VALUE)
{
	if (!Files::exists(file) || !Files::isFile(file))
		throw FileNotFoundException(file);

	m_size = (size_t)Files::getSize(file);
#ifdef _WIN32
	m_file = INVALID_HANDLE_VALUE;

	DWORD pageMode = PAGE_READONLY;
	DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	DWORD fileMode = GENERIC_READ;

	if (mode == SharedMemory::AM_WRITE)
	{
		pageMode = PAGE_READWRITE;
		fileMode |= GENERIC_WRITE;
	}

	m_file = CreateFileA(m_name.c_str(), fileMode, shareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_file == INVALID_HANDLE_VALUE)
		throw OpenFileException("Cannot open memory mapped file", m_name);

	m_handle = CreateFileMapping(m_file, NULL, pageMode, 0, 0, NULL);
	if (!m_handle)
	{
		DWORD dwRetVal = GetLastError();
		CloseHandle(m_file);
		m_file = INVALID_HANDLE_VALUE;
		throw SystemException(Strings::format("Cannot map file into shared memory %s [Error %d: %s]", m_name, (int)dwRetVal, Error::getMessage(dwRetVal)));
	}
	map(addrHint);
#else
	m_fileMapped = true;
	m_server = false;

	int flag = O_RDONLY;
	if (mode == SharedMemory::AM_WRITE)
		flag = O_RDWR;
	m_handle = ::open(m_name.c_str(), flag);
	if (-1 == m_handle)
		throw OpenFileException("Cannot open memory mapped file", m_name);

	map(addrHint);
#endif
}

SharedMemory::~SharedMemory()
{
	unmap();
	close();
}

void SharedMemory::map(const void* addrHint)
{
#ifdef _WIN32
	DWORD access = m_mode == AM_WRITE ? FILE_MAP_WRITE : FILE_MAP_READ;
	LPVOID addr = MapViewOfFile(m_handle, access, 0, 0, m_size);
	if (!addr)
	{
		DWORD dwRetVal = GetLastError();
		throw SystemException(Strings::format("Cannot map shared memory object %s [Error %d: %s]", m_name, (int)dwRetVal, Error::getMessage(dwRetVal)));
	}

	m_address = static_cast<char*>(addr);
#else
	int access = PROT_READ;
	if (m_mode == SharedMemory::AM_WRITE)
		access |= PROT_WRITE;

	void* addr = ::mmap(const_cast<void*>(addrHint), m_size, access, MAP_SHARED, m_handle, 0);
	if (addr == MAP_FAILED)
		throw SystemException("Cannot map file into shared memory", m_name);

	m_address = static_cast<char*>(addr);
#endif
}

void SharedMemory::unmap()
{
	if (m_address)
	{
#ifdef _WIN32
		UnmapViewOfFile(m_address);
#else
		::munmap(m_address, m_size);
#endif
		m_address = 0;
	}
}

void SharedMemory::close()
{
#ifdef _WIN32
	if (m_address)
	{
		UnmapViewOfFile(m_address);
		m_address = 0;
	}
#else
	if (m_handle != -1)
	{
		::close(m_handle);
		m_handle = -1;
	}
	if (!m_fileMapped && m_server)
	{
		::shm_unlink(m_name.c_str());
	}
#endif
}

CUTE_NS_END
