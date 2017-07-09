//! FileSystem
#include "Cute/DirectoryIterator.h"

CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_WINDOWS)

class DirectoryIteratorImpl
{
public:
	typedef DirectoryIteratorImpl Impl;

	void open(Impl* parent, const String& path)
	{
		m_parent = parent;
		m_path = path;
		m_handle = INVALID_HANDLE_VALUE;
	}

	void close()
	{
		m_parent = NULL;

		if(m_handle != INVALID_HANDLE_VALUE)
		{
			FindClose(m_handle);
			m_handle = INVALID_HANDLE_VALUE;
		}
	}

	void destory()
	{
		if(m_parent != NULL)
		{
			delete m_parent;
			m_parent = NULL;
		}

		this->close();
	}

	bool next(FileInfo& result)
	{
		WIN32_FIND_DATAA findData;
		if(m_handle == INVALID_HANDLE_VALUE)
		{
			m_handle = FindFirstFileA(m_path.c_str(), &findData);
			if(m_handle == INVALID_HANDLE_VALUE)
				return false;
		}
		else
		{
			if(FindNextFileA(m_handle, &findData))
				return false;
		}

		// copy FindData to FinleInfo
		// 手动获取
        result.assign(findData.cFileName);

		return true;
	}

	Impl* getParent()
	{
		return m_parent;
	}

	void  setParent(Impl* parent)
	{
		m_parent = parent;
	}

private:
	Impl* 	m_parent;
	HANDLE 	m_handle;
	String 	m_path;
};

#elif defined(CUTE_OS_FAMILY_POSIX)
 #include <dirent.h>
class DirectoryIteratorImpl
{
public:
	typedef DirectoryIteratorImpl Impl; 
	void open(Impl* parent, const String& path)
	{
		m_parent = parent;
		m_path = path;
		m_dir = opendir(path.c_str());
	}

	void close()
	{
		m_parent = NULL;
		if(m_dir != NULL)
		{
			closedir(m_dir);
			m_dir = NULL;
		}
	}

	void destory()
	{
		if(m_parent != NULL)
		{
			delete m_parent;
			m_parent = NULL;
		}

		this->close();
	}

	bool next(FileInfo& result)
	{
		if(m_dir == NULL)
			return false;

		struct dirent* const de = readdir(m_dir);
		if(de == NULL)
			return false;

        result.assign(de->d_name);
		return true;
	}

	Impl* getParent()
	{
		return m_parent;
	}

	void  setParent(Impl* parent)
	{
		m_parent = parent;
	}

private:
	Impl* 	m_parent;
	DIR* 	m_dir;
	String 	m_path;
};
#else
#error "not support DirectoryIterator!!"
#endif

DirectoryIterator::DirectoryIterator(const String& path, bool recursive, const String& pattern, int filters)
	: m_recursive(recursive)
	, m_filters(filters)
	, m_pattern(pattern)
	, m_finder(new Impl())
{
    m_finder->open(NULL, path);
}

DirectoryIterator::~DirectoryIterator()
{
	// 递归释放
	if(m_finder != NULL)
		delete m_finder;

	if(m_free != NULL)
		delete m_free;
}

bool DirectoryIterator::next()
{
	if(m_finder == NULL)
		return false;

	for(;;)
	{
		//
		if(m_finder->next(m_current))
		{// found, check match and create sub finder if is directory
            const String& path = m_current.toString();
			if(path == "." || path == "..")
				continue;

			if(this->isIgnoreHidden() && m_current.isHidden())
				continue;

			bool matches = false;
			if(m_current.isDirectory())
			{
				matches = (m_filters & SCAN_FILTER_DIR) != 0;

				if(m_recursive)
				{
					Impl* parent = m_finder;
					if(m_free != NULL)
					{// use cache
						m_finder = m_free;
						m_free = m_free->getParent();
					}
					else
					{
						m_finder= new Impl();
					}

					m_finder->open(parent, m_current.toString());
				}
			}
			else
			{
				matches = (m_filters & SCAN_FILTER_FILE) != 0;
			}

			// todo: check wildcards
			if(matches)
			{
				return true;
			}
		}
		else
		{// not found, return parent directory
			Impl* parent = m_finder->getParent();

			m_finder->close();
			m_finder->setParent(m_free);
			
			m_free = m_finder;
			m_finder = parent;

			// check finish
			if(m_finder == NULL)
				return false;
		}
	}
}

CUTE_NS_END

// // module FileSystem
// #include "Cute/DirectoryIterator.h"
// #include "Cute/Path.h"
// #include "Cute/File.h"

// CUTE_NS_BEGIN

// DirectoryIterator::DirectoryIterator()
// 	: m_handle(INVALID_HANDLE_VALUE)
// {
// }

// DirectoryIterator::DirectoryIterator(const String& path)
// 	: m_handle(INVALID_HANDLE_VALUE)
// {
// 	assign(path);
// }

// DirectoryIterator::~DirectoryIterator()
// {
// 	close();
// }

// void DirectoryIterator::assign(const String& path)
// {
// 	close();
// #if defined(CUTE_OS_FAMILY_WINDOWS)
// 	Path p(path);
// 	p.makeDirectory();
// 	String findPath = p.toString();
// 	findPath.append("*");

// 	WIN32_FIND_DATAA	fd;
// 	m_handle = FindFirstFileA(findPath.c_str(), &fd);
// 	if (m_handle == INVALID_HANDLE_VALUE)
// 	{
// 		if (GetLastError() != ERROR_NO_MORE_FILES)
// 			File::error(path);
// 	}
// 	else
// 	{
// 		m_current = fd.cFileName;
// 		if (m_current == "." || m_current == "..")
// 			next();
// 	}
// #else
// 	Path p(path);
// 	p.makeFile();

// #if defined(CUTE_VXWORKS)
// 	m_handle = opendir(const_cast<char*>(p.toString().c_str()));
// #else
// 	m_handle = opendir(p.toString().c_str());
// #endif
// //	if (!m_handle)
// //		File::error(path);
// 	next();

// #endif
// }

// void DirectoryIterator::next()
// {
// #if defined(CUTE_OS_FAMILY_WINDOWS)
// 	WIN32_FIND_DATAA	fd;
// 	do
// 	{
// 		if (FindNextFileA(m_handle, &fd) != 0)
// 			m_current = fd.cFileName;
// 		else
// 			m_current.clear();
// 	} while (m_current == "." || m_current == "..");
// #else
// 	do
// 	{
// 		struct dirent* pEntry = readdir(m_handle);
// 		if (pEntry)
// 			m_current = pEntry->d_name;
// 		else
// 			m_current.clear();
// 	} while (m_current == "." || m_current == "..");
// #endif
// }

// void DirectoryIterator::close()
// {
// 	if (m_handle != INVALID_HANDLE_VALUE)
// 	{
// #if defined(CUTE_OS_FAMILY_WINDOWS)
// 		FindClose(m_handle);
// #else
// 		closedir(m_handle);
// #endif
// 		m_handle = INVALID_HANDLE_VALUE;
// 		m_current.clear();
// 	}
// }

// String DirectoryIterator::name() const
// {
// 	Path path(m_current);
// 	return path.getFileName();
// }

// CUTE_NS_END
