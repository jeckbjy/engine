#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// static file operation
class CUTE_CORE_API Files
{
public:
    static bool readAll(const String& path, String& data, bool removeBom = true);
    
    static bool createFile(const String& path);
    static bool createDirectory(const String& path);
    static bool createDirectories(const String& path);
    static bool remove(const String& path, bool recursive = false);
    
    static bool copy(const String& src, const String& dst);
    static bool move(const String& src, const String& dst);
    static bool rename(const String& src, const String& dst);
    static bool moveToTrash(const String& path);

    static bool exists(const String& path);
    static bool isFile(const String& path);
    static bool isDirectory(const String& path);
    static bool isDevice(const String& path);
    static bool isHidden(const String& path);
    static bool isLink(const String& path);

    static bool isOnHardDisk(const String& path);
    static bool isOnCDRomDrive(const String& path);
    static bool isOnRemovableDrive(const String& path);

    static bool isReadable(const String& path);
    static bool isWritable(const String& path);
    static bool isExecutable(const String& path);

    static bool setReadOnly(const String& path, bool flag = true);
    static bool setWritable(const String& path, bool flag = true);
    static bool setExecutable(const String& path, bool flag = true);

    static int64 getCreationTime(const String& path);
    static int64 getLastAccessTime(const String& path);
    static int64 getLastModificationTime(const String& path);
 
    static bool  setCreationTime(const String& path, int64 time);
    static bool  setLastAccessTime(const String& path, int64 time);
    static bool  setLastModificationTime(const String& path, int64 time);
 
    static bool  setSize(const String& path, int64 size);
    static int64 getSize(const String& path);
    static int64 getTotalSpace(const String& path);
    static int64 getUsableSpace(const String& path);
    static int64 getFreeSpace(const String& path);
};

CUTE_NS_END
