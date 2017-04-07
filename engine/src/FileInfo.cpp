//! FileSystem
#include "Cute/FileInfo.h"

CUTE_NS_BEGIN

//#ifdef CUTE_OS_FAMILY_WINDOWS
//#undef GetFileAttributes
//#undef GetFileAttributesEx
//#ifdef CUTE_WIN32_UTF8
//#define GetFileAttributes	GetFileAttributesW
//#define GetFileAttributesEx GetFileAttributesExW
//#else
//#define GetFileAttributes	GetFileAttributesA
//#define GetFileAttributesEx GetFileAttributesExA
//#endif
//#endif
//
//#ifdef _WIN32
//#define INVALID_FILEINFO ERROR_INVALID_DRIVE
//#else
//#define INVALID_FILEINFO ENOENT
//#endif
//
//FileInfo::FileInfo()
//	: m_error(INVALID_FILEINFO)
//{
//}
//
//FileInfo::FileInfo(const String& path)
//	: m_error(INVALID_FILEINFO)
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//	WinConvertToSystem(path, m_path);
//#else
//	m_path = path;
//#endif
//
//	refresh();
//}
//
//void FileInfo::refresh()
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//	if (GetFileAttributesEx(m_path.c_str(), GetFileExInfoStandard, &m_data) == 0)
//	{
//		m_error = GetLastError();
//	}
//	else
//	{
//		m_error = ERROR_SUCCESS;
//	}
//#else
//	if (stat(m_path.c_str(), &m_data) != 0)
//	{
//		m_error = errno;
//	}
//	else
//	{
//		m_error = 0;
//	}
//#endif
//}
//
//bool FileInfo::exists() const
//{
//	return m_error == 0;
//}
//
//bool FileInfo::canRead() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//	return getAttributes() != INVALID_FILE_ATTRIBUTES;
//#else
//	if (exists())
//	{
//		if (m_data.st_uid == geteuid())
//			return (m_data.st_mode & S_IRUSR) != 0;
//		else if (m_data.st_gid == getegid())
//			return (m_data.st_mode & S_IRGRP) != 0;
//		else
//			return (m_data.st_mode & S_IROTH) != 0 || geteuid() == 0;
//	}
//	return false;
//#endif
//}
//
//bool FileInfo::canWrite() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//bool FileInfo::canExecute() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//bool FileInfo::isFile() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//bool FileInfo::isLink() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//bool FileInfo::isDirectory() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//bool FileInfo::isDevice() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//bool FileInfo::isHidden() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//uint64 FileInfo::getCreationTime() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//uint64 FileInfo::getLastModified() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//void FileInfo::setLastModified(uint64 time)
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//uint64 FileInfo::getSize() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//void FileInfo::setSize(uint64 size)
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//void FileInfo::setWriteable(bool flag /* = true */)
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//void FileInfo::setExecutable(bool flag /* = true */)
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//void FileInfo::setReadOnly(bool flag /* = true */)
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//#else
//#endif
//}
//
//FileInfo::attr_t FileInfo::getAttributes() const
//{
//#ifdef CUTE_OS_FAMILY_WINDOWS
//	if (m_error != ERROR_SUCCESS)
//		return INVALID_FILE_ATTRIBUTES;
//	else
//		return m_data.dwFileAttributes;
//#else
//	if (m_error != 0)
//		return 0;
//	else
//		return m_data.st_mode;
//#endif
//}

CUTE_NS_END
