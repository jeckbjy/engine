#pragma once
#include "Cute/Foundation.h"

#if defined(CUTE_OS_FAMILY_POSIX)
#	include <dirent.h>
#endif

CUTE_NS_BEGIN

/// The DirectoryIterator class is used to enumerate
/// all files in a directory.
///
/// DirectoryIterator has some limitations:
///   * only forward iteration (++) is supported
///   * an iterator copied from another one will always
///     point to the same file as the original iterator,
///     even is the original iterator has been advanced
///     (all copies of an iterator share their state with
///     the original iterator)
///   * because of this you should only use the prefix 
///     increment operator
class CUTE_CORE_API DirectoryIterator
{
public:
	DirectoryIterator();
	DirectoryIterator(const String& path);
	~DirectoryIterator();

	bool operator == (const DirectoryIterator& iterator) const;
	bool operator != (const DirectoryIterator& iterator) const;

	DirectoryIterator& operator = (const String& path);
	DirectoryIterator& operator ++ ();   // prefix

	const String& operator*();
	const String& current();
	String name() const;

private:
	void assign(const String& path);
	void next();
	void close();

private:
	DirectoryIterator(const DirectoryIterator& iterator);
	DirectoryIterator& operator = (const DirectoryIterator& it);

private:
#if defined(CUTE_OS_FAMILY_WINDOWS)
	HANDLE      m_handle;
#else
	DIR*		m_handle;
#endif
	String	m_current;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline DirectoryIterator& DirectoryIterator::operator ++()
{
	next();
	return *this;
}

inline const String& DirectoryIterator::operator *()
{
	return m_current;
}

inline const String& DirectoryIterator::current()
{
	return m_current;
}

inline bool DirectoryIterator::operator ==(const DirectoryIterator& iterator) const
{
	return m_current == iterator.m_current;
}

inline bool DirectoryIterator::operator !=(const DirectoryIterator& iterator) const
{
	return m_current != iterator.m_current;
}

CUTE_NS_END
