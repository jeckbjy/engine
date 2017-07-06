//! FileSystem
#include "Cute/Files.h"
#include "./FileHelper.h"

CUTE_NS_BEGIN

bool Files::readAll(const String& path, String& data, bool removeBom)
{
    return true;
}

bool Files::createFile(const String& path)
{
    return FileHelper::createFile(path);
}

bool Files::createDirectory(const String& path)
{
    return FileHelper::createDirectory(path);
}

bool Files::createDirectories(const String& path)
{
    return true;
}

bool Files::remove(const String& path, bool recursive)
{
    stat_t st;
    if(!FileHelper::getFileAttr(path, st))
    {
        return false;
    }
    
    if(!recursive)
    {
        return FileHelper::remove(path, st);
    }
    return true;
}

bool Files::copy(const String& src, const String& dst)
{
    return FileHelper::copy(src, dst);
}

bool Files::move(const String& src, const String& dst)
{
    return FileHelper::move(src, dst);
}

bool Files::rename(const String& src, const String& dst)
{
    return FileHelper::rename(src, dst);
}

bool Files::moveToTrash(const String& path)
{
    return true;
}

bool Files::exists(const String& path)
{
    return FileHelper::exists(path);
}

bool Files::isFile(const String& path)
{
    file_attr_t attr;
    if(FileHelper::getFileAttr(path, attr))
        return FileHelper::isFile(attr);

    return false;
}

bool Files::isDirectory(const String& path)
{
    file_attr_t attr;
    if(FileHelper::getFileAttr(path, attr))
        return FileHelper::isDirectory(attr);

    return false;
}

bool Files::isDevice(const String& path)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
    return FileHelper::isDevice(path);
#else
    file_attr_t attr;
    if(FileHelper::getFileData(path, attr))
        return FileHelper::isDevice(attr);

    return false;
#endif
}

bool Files::isHidden(const String& path)
{
#if defined(CUTE_OS_FAMILY_WINDOWS)
    file_attr_t attr;
    if(FileHelper::getFileAttr(path, attr))
        return FileHelper::isHidden(attr);
#else
    return FileHelper::isHidden(path);
#endif


    return false;
}

bool Files::isLink(const String& path)
{
    file_attr_t attr;
    if(FileHelper::getFileAttr(path, attr))
        return FileHelper::isLink(attr);

    return false;
}

bool Files::isOnHardDisk(const String& path)
{
    return true;
}

bool Files::isOnCDRomDrive(const String& path)
{
    return true;
}

bool Files::isOnRemovableDrive(const String& path)
{
    return true;
}

bool Files::isReadable(const String& path)
{
    file_attr_t attr;
    if(FileHelper::getFileAttr(path, attr))
        return FileHelper::isLink(attr);

    return false;
}

bool Files::isWritable(const String& path)
{
    file_attr_t attr;
    if(FileHelper::getFileAttr(path, attr))
        return FileHelper::isWritable(attr);

    return false;
}

bool Files::isExecutable(const String& path)
{
    file_attr_t attr;
    if(FileHelper::getFileAttr(path, attr))
        return FileHelper::isExecutable(attr);
    
    return false;
}

bool Files::setReadOnly(const String& path, bool flag)
{
    return setWritable(path, !flag);
}

bool Files::setWritable(const String& path, bool flag)
{
    return FileHelper::setWritable(path, flag);
}

bool Files::setExecutable(const String& path, bool flag)
{
    return FileHelper::setExecutable(path, flag);
}

int64 Files::getCreationTime(const String& path)
{
    stat_t st;
    if(FileHelper::getFileData(path, st))
        return FileHelper::getCreationTime(st);

    return 0;
}

int64 Files::getLastAccessTime(const String& path)
{
    stat_t st;
    if(FileHelper::getFileData(path, st))
        return FileHelper::getLastAccessTime(st);

    return 0;
}

int64 Files::getLastModificationTime(const String& path)
{
    stat_t st;
    if(FileHelper::getFileData(path, st))
        return FileHelper::getLastModificationTime(st);

    return 0;
}

bool Files::setCreationTime(const String& path, int64 time)
{
    return FileHelper::setCreationTime(path, time);
}

bool Files::setLastAccessTime(const String& path, int64 time)
{
    return FileHelper::setLastAccessTime(path, time);
}

bool Files::setLastModificationTime(const String& path, int64 time)
{
    return FileHelper::setLastModificationTime(path, time);
}

bool Files::setSize(const String& path, int64 size)
{
    return FileHelper::setSize(path, size);
}

int64 Files::getSize(const String& path)
{
    stat_t st;
    if(FileHelper::getFileData(path, st))
        return FileHelper::getSize(st);

    return 0;
}

int64 Files::getTotalSpace(const String& path)
{
    return FileHelper::getTotalSpace(path);
}

int64 Files::getUsableSpace(const String& path)
{
    return FileHelper::getUsableSpace(path);
}

int64 Files::getFreeSpace(const String& path)
{
    return FileHelper::getFreeSpace(path);
}

CUTE_NS_END
