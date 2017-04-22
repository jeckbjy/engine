// module FileSystem
#include "Cute/DirectoryIterator.h"
#include "Cute/Path.h"
#include "Cute/File.h"

CUTE_NS_BEGIN

DirectoryIterator::DirectoryIterator()
	: m_handle(INVALID_HANDLE_VALUE)
{
}

DirectoryIterator::DirectoryIterator(const String& path)
	: m_handle(INVALID_HANDLE_VALUE)
{
	assign(path);
}

DirectoryIterator::~DirectoryIterator()
{
	close();
}

void DirectoryIterator::assign(const String& path)
{
	close();
#if defined(CUTE_OS_FAMILY_WINDOWS)
	Path p(path);
	p.makeDirectory();
	String findPath = p.toString();
	findPath.append("*");

	WIN32_FIND_DATAA	fd;
	m_handle = FindFirstFileA(findPath.c_str(), &fd);
	if (m_handle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_NO_MORE_FILES)
			File::error(path);
	}
	else
	{
		m_current = fd.cFileName;
		if (m_current == "." || m_current == "..")
			next();
	}
#else
	Path p(path);
	p.makeFile();

#if defined(CUTE_VXWORKS)
	m_handle = opendir(const_cast<char*>(p.toString().c_str()));
#else
	m_handle = opendir(p.toString().c_str());
#endif
	if (!m_handle)
		File::error(path);
	next();

#endif
}

void DirectoryIterator::next()
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
	WIN32_FIND_DATAA	fd;
	do
	{
		if (FindNextFileA(m_handle, &fd) != 0)
			m_current = fd.cFileName;
		else
			m_current.clear();
	} while (m_current == "." || m_current == "..");
#else
	do
	{
		struct dirent* pEntry = readdir(m_handle);
		if (pEntry)
			m_current = pEntry->d_name;
		else
			m_current.clear();
	} while (m_current == "." || m_current == "..");
#endif
}

void DirectoryIterator::close()
{
	if (m_handle != INVALID_HANDLE_VALUE)
	{
#if defined(CUTE_OS_FAMILY_WINDOWS)
		FindClose(m_handle);
#else
		closedir(m_handle);
#endif
		m_handle = INVALID_HANDLE_VALUE;
		m_current.clear();
	}
}

String DirectoryIterator::name() const
{
	Path path(m_current);
	return path.getFileName();
}

CUTE_NS_END