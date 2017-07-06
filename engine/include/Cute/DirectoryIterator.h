#pragma once
#include "Cute/Foundation.h"
#include "Cute/FileInfo.h"

CUTE_NS_BEGIN

enum FileScanFilter
{
	SCAN_FILTER_FILE			= 0x01,
	SCAN_FILTER_DIR				= 0x02,
	SCAN_FILTER_FILE_AND_DIR	= 0x03,
	SCAN_FILTER_IGNORE_HIDDEN	= 0x04,	
};

//==============================================================================
/**
    Searches through the files in a directory, returning each file that is found.

    A DirectoryIterator will search through a directory and its subdirectories using
    a wildcard filepattern match.

    If you may be scanning a large number of files, it's usually smarter to use this
    class than File::findChildFiles() because it allows you to stop at any time, rather
    than having to wait for the entire scan to finish before getting the results.

    It also provides an estimate of its progress, using a (highly inaccurate!) algorithm.
*/
class DirectoryIteratorImpl;
class CUTE_CORE_API DirectoryIterator
{
public:
	DirectoryIterator(const String& path, bool recursive = false, const String& pattern = "*", int filters = SCAN_FILTER_FILE);
	~DirectoryIterator();

	bool next();

	const FileInfo& get() const;
	const String& toString() const;

	bool isIgnoreHidden() const;

	const String& operator*() const;

private:
	typedef DirectoryIteratorImpl Impl;
	friend class DirectoryIteratorImpl;

	const bool 	 m_recursive;
	const int 	 m_filters;
	const String m_pattern;
	FileInfo     m_current;
	Impl* 		 m_finder;
	Impl*		 m_free;		// 空闲的
};

inline bool DirectoryIterator::isIgnoreHidden() const
{
	return (m_filters & SCAN_FILTER_IGNORE_HIDDEN) != 0;
}

inline const FileInfo& DirectoryIterator::get() const
{
	return this->m_current;
}

inline const String& DirectoryIterator::toString() const
{
	return this->m_current.toString();
}

inline const String& DirectoryIterator::operator*() const
{
	return this->m_current.toString();
}

CUTE_NS_END

// #pragma once
// #include "Cute/Foundation.h"

// #if defined(CUTE_OS_FAMILY_POSIX)
// #	include <dirent.h>
// #endif

// CUTE_NS_BEGIN

// /// The DirectoryIterator class is used to enumerate
// /// all files in a directory.
// ///
// /// DirectoryIterator has some limitations:
// ///   * only forward iteration (++) is supported
// ///   * an iterator copied from another one will always
// ///     point to the same file as the original iterator,
// ///     even is the original iterator has been advanced
// ///     (all copies of an iterator share their state with
// ///     the original iterator)
// ///   * because of this you should only use the prefix 
// ///     increment operator
// class CUTE_CORE_API DirectoryIterator
// {
// public:
// 	DirectoryIterator();
// 	DirectoryIterator(const String& path);
// 	~DirectoryIterator();

// 	bool operator == (const DirectoryIterator& iterator) const;
// 	bool operator != (const DirectoryIterator& iterator) const;

// 	DirectoryIterator& operator = (const String& path);
// 	DirectoryIterator& operator ++ ();   // prefix

// 	const String& operator*();
// 	const String& current();
// 	String name() const;

// private:
// 	void assign(const String& path);
// 	void next();
// 	void close();

// private:
// 	DirectoryIterator(const DirectoryIterator& iterator);
// 	DirectoryIterator& operator = (const DirectoryIterator& it);

// private:
// #if defined(CUTE_OS_FAMILY_WINDOWS)
// 	HANDLE      m_handle;
// #else
// 	DIR*		m_handle;
// #endif
// 	String	m_current;
// };

// //////////////////////////////////////////////////////////////////////////
// //
// //////////////////////////////////////////////////////////////////////////
// inline DirectoryIterator& DirectoryIterator::operator ++()
// {
// 	next();
// 	return *this;
// }

// inline const String& DirectoryIterator::operator *()
// {
// 	return m_current;
// }

// inline const String& DirectoryIterator::current()
// {
// 	return m_current;
// }

// inline bool DirectoryIterator::operator ==(const DirectoryIterator& iterator) const
// {
// 	return m_current == iterator.m_current;
// }

// inline bool DirectoryIterator::operator !=(const DirectoryIterator& iterator) const
// {
// 	return m_current != iterator.m_current;
// }

// CUTE_NS_END
