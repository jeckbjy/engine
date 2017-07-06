//! FileSystem
#pragma once
#include "Cute/Foundation.h"
#include "Cute/FileInfo.h"

#if defined(CUTE_OS_FAMILY_POSIX)
#   include <sys/stat.h>
#   include <unistd.h>
#   if defined(CUTE_OS_FAMILY_BSD)
#   include <sys/param.h>
#   include <sys/mount.h>
#   else
#   include <sys/statfs.h>
#   endif

#   include "Cute/Paths.h"
#endif

CUTE_NS_BEGIN

namespace FileHelper {
    # if defined(CUTE_OS_FAMILY_WINDOWS)
    #   include "./FileHelper_Win32.inl"
    # elif defined(CUTE_OS_FAMILY_POSIX)
    #   include "./FileHelper_Posix.inl"
    # endif

    inline int64 getCreationTime(const stat_t& st)
    {
        int64 creationTime = 0;
        getFileTimes(st, 0, 0, &creationTime);
        return creationTime;
    }
    
    inline int64 getLastAccessTime(const stat_t& st)
    {
        int64 accessTime = 0;
        getFileTimes(st, 0, &accessTime, 0);
        return accessTime;
    }
    
    inline int64 getLastModificationTime(const stat_t& st)
    {
        int64 modifyTime = 0;
        getFileTimes(st, &modifyTime, 0, 0);
        return modifyTime;
    }
    
    inline bool setCreationTime(const String& path, int64 time)
    {
        return setFileTimes(path, 0, 0, time);
    }
    
    inline bool setLastAccessTime(const String& path, int64 time)
    {
        return setFileTimes(path, 0, time, 0);
    }
    
    inline bool setLastModificationTime(const String& path, int64 time)
    {
        return setFileTimes(path, time, 0, 0);
    }
}

CUTE_NS_END
