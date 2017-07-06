#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

#if defined(CUTE_OS_FAMILY_WINDOWS)
typedef WIN32_FILE_ATTRIBUTE_DATA   stat_t;
#elif defined(CUTE_OS_FAMILY_LINUX) || ( CUTE_OS_IOS && !__DARWIN_ONLY_64_BIT_INO_T)
// (this iOS stuff is to avoid a simulator bug)
typedef struct stat 	stat_t;
#define get_stat 		stat
#elif defined(CUTE_OS_FAMILY_POSIX)
typedef struct stat 	stat_t;
#define get_stat 		stat
#else
#define CUTE_NOT_SUPPORT_DIR 1
#endif

#if defined(CUTE_OS_FAMILY_WINDOWS)
typedef DWORD           file_attr_t;
#else
typedef stat_t          file_attr_t;
#endif

class CUTE_CORE_API FileInfo
{
public:
    FileInfo();
	FileInfo(const String& path);
	~FileInfo();

	bool assign(const String& path);
	
	bool exists() const;

	bool isFile() const;
    bool isDirectory() const;
    bool isDevice() const;
    bool isHidden() const;
    bool isLink() const;
    bool isOnHardDisk() const;
    bool isOnCDRomDrive() const;
    bool isOnRemovableDrive() const;

    bool isReadable() const;
    bool isWritable() const;
    bool isExecutable() const;

    int64 getCreationTime() const;
    int64 getLastAccessTime() const;
    int64 getLastModificationTime() const;

	int64 getSize() const;
	int64 getTotalSpace() const;
	int64 getUsableSpace() const;
	int64 getFreeSpace() const;
    
    const String& toString() const;
    
    operator const String& () const;
    
private:
    const file_attr_t& getAttr() const;

private:
	friend class DirectoryIteratorImpl;
	String 	m_path;
	stat_t	m_data;
};

inline const file_attr_t& FileInfo::getAttr() const
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
    return m_data.dwFileAttributes;
#else
    return m_data;
#endif
}

inline const String& FileInfo::toString() const
{
    return m_path;
}

inline FileInfo::operator const String&() const
{
    return m_path;
}

// class CUTE_CORE_API FileInfo
// {
// public:
// #ifdef CUTE_OS_FAMILY_WINDOWS
// 	typedef DWORD attr_t;
// #else
// 	typedef unsigned short attr_t;
// #endif
// 	FileInfo();
// 	FileInfo(const String& path);
// 	~FileInfo();

// 	void refresh();

// 	bool exists() const;
// 	bool canRead() const;
// 	bool canWrite() const;
// 	bool canExecute() const;

// 	bool isFile() const;
// 	bool isLink() const;
// 	bool isDirectory() const;
// 	bool isDevice() const;
// 	bool isHidden() const;

// 	uint64 getCreationTime() const;
// 	uint64 getLastModified() const;
// 	void   setLastModified(uint64 time);

// 	uint64 getSize() const;
// 	void setSize(uint64 size);
// 	void setWriteable(bool flag = true);
// 	void setExecutable(bool flag = true);
// 	void setReadOnly(bool flag = true);

// 	void handleLastError();

// 	attr_t getAttributes() const;

// private:
// #ifdef CUTE_OS_FAMILY_WINDOWS
// 	typedef WIN32_FILE_ATTRIBUTE_DATA FileData;
// #else
// 	typedef struct stat FileData;
// #endif
// 	TString	 m_path;
// 	FileData m_data;
// 	error_t	 m_error;
// };

CUTE_NS_END
