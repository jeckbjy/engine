//! FileSystem
#if defined(CUTE_OS_FAMILY_POSIX)

inline bool getFileAttr(const String& path, file_attr_t& st)
{
    return get_stat(path.c_str(), &st) == 0;
}

inline bool getFileData(const String& path, stat_t& st)
{
    return get_stat(path.c_str(), &st) == 0;
}

inline bool exists(const String& path)
{
    stat_t st;
    return get_stat(path.c_str(), &st) == 0;
}

inline bool isFile(const stat_t& st)
{
    return S_ISREG(st.st_mode);
}

inline bool isDirectory(const stat_t& st)
{
    return S_ISDIR(st.st_mode);
}

inline bool isLink(const stat_t& st)
{
    return S_ISLNK(st.st_mode);
}

inline bool isDevice(const stat_t& st)
{
    return S_ISCHR(st.st_mode);
}

 inline bool isHidden(const String path)
 {// path
    String filename = Paths::getFileName(path);
    return !filename.empty() && filename[0] == '.';
 }

inline bool isReadable(const stat_t& st)
{
    if (st.st_uid == geteuid())
        return (st.st_mode & S_IRUSR) != 0;
    else if (st.st_gid == getegid())
        return (st.st_mode & S_IRGRP) != 0;
    else
        return (st.st_mode & S_IROTH) != 0 || geteuid() == 0;
}

inline bool isWritable(const stat_t& st)
{
    if (st.st_uid == geteuid())
        return (st.st_mode & S_IWUSR) != 0;
    else if (st.st_gid == getegid())
        return (st.st_mode & S_IWGRP) != 0;
    else
        return (st.st_mode & S_IWOTH) != 0 || geteuid() == 0;
}

inline bool isExecutable(const stat_t& st)
{
    if (st.st_uid == geteuid() || geteuid() == 0)
        return (st.st_mode & S_IXUSR) != 0;
    else if (st.st_gid == getegid())
        return (st.st_mode & S_IXGRP) != 0;
    else
        return (st.st_mode & S_IXOTH) != 0;
}

inline bool setWritable(const String& path, bool flag)
{
    struct stat st;
    if (get_stat(path.c_str(), &st) != 0) 
        return false;

    mode_t mode;
    if (flag)
    {
        mode = st.st_mode | S_IWUSR;
    }
    else
    {
        mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
        mode = st.st_mode & ~wmask;
    }
    if (chmod(path.c_str(), mode) != 0) 
        return false;
    
    return true;
}

inline bool setExecutable(const String& path, bool flag)
{
    struct stat st;
    if (get_stat(path.c_str(), &st) != 0) 
        return false;

    mode_t mode;
    if (flag)
    {
        mode = st.st_mode | S_IXUSR;
    }
    else
    {
        mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
        mode = st.st_mode & ~wmask;
    }

    if (chmod(path.c_str(), mode) != 0) 
        return false;
    
    return true;
}

inline bool setFileTimes(const String& path, int64 modifyTime, int64 accessTime, int64 /*creationTime*/)
{
    stat_t info;

    if ((modifyTime != 0 || accessTime != 0) && get_stat(path.c_str(), &info))
    {
        struct utimbuf times;
        times.actime  = accessTime != 0 ? static_cast<time_t> (accessTime / 1000) : static_cast<time_t> (info.st_atime);
        times.modtime = modifyTime != 0 ? static_cast<time_t> (modifyTime / 1000) : static_cast<time_t> (info.st_mtime);

        return utime (path.c_str(), &times) == 0;
    }

    return false;
}

inline bool getFileTimes(const stat_t& st, int64* modifyTime, int64* accessTime, int64* creationTime)
{
    if(modifyTime)
        *modifyTime = (int64) st.st_mtime * 1000;

    if(accessTime)
        *accessTime = (int64) st.st_atime * 1000;
    
    if(creationTime)
        *creationTime = (int64) st.st_ctime * 1000;
    
    return true;
}

inline bool setSize(const String& path, int64 size)
{
    return truncate(path.c_str(), size) == 0;
}

inline int64 getSize(const stat_t& st)
{
    return st.st_size;
}

inline int64 getTotalSpace(const String& path)
{
    struct statfs stats;
    if (statfs(const_cast<char*>(path.c_str()), &stats) != 0)
        return 0;

    return (int64)stats.f_blocks * (int64)stats.f_bsize;
}

inline int64 getUsableSpace(const String& path)
{
    struct statfs stats;
    if (statfs(const_cast<char*>(path.c_str()), &stats) != 0)
        return 0;

    return (int64)stats.f_bavail * (int64)stats.f_bsize;
}

inline int64 getFreeSpace(const String& path)
{
    struct statfs stats;
    if (statfs(const_cast<char*>(path.c_str()), &stats) != 0)
        return 0;

    return (int64)stats.f_bfree * (int64)stats.f_bsize;
}

inline bool getSpace(const String& path, int64* totalSpace, int64* usableSpace, int64* freeSpace)
{
    struct statfs stats;
    if (statfs(const_cast<char*>(path.c_str()), &stats) != 0)
        return false;

    if(totalSpace != NULL)
        *totalSpace = (int64)stats.f_blocks * (int64)stats.f_bsize;

    if(usableSpace != NULL)
        *usableSpace = (int64)stats.f_bavail * (int64)stats.f_bsize;

    if(freeSpace != NULL)
        *freeSpace = (int64)stats.f_bfree * (int64)stats.f_bsize;

    return true;
}

inline bool copy(const String& src, const String& dst)
{
    int sd = open(src.c_str(), O_RDONLY);
    if (sd == -1)
        return false;

    struct stat st;
    if (fstat(sd, &st) != 0) 
    {
        close(sd);
        return false;
    }

    const long blockSize = st.st_blksize;

    int dd = open(dst.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
    if (dd == -1)
    {
        close(sd);
        return false;
    }

    char* buffer = new char[blockSize];
    bool result = true;
    long n;
    while ((n = read(sd, buffer, blockSize)) > 0)
    {
        if (write(dd, buffer, n) != n)
        {
            result = false;
            break;
        } 
    }

    if (n < 0)
    {
        result = false;
    }

    close(sd);
    if (fsync(dd) != 0) 
        result = false;

    if (close(dd) != 0)
        result = false;

    return result;
}

inline bool move(const String& src, const String& dst)
{
    return ::rename(src.c_str(), dst.c_str()) == 0;
}

inline bool moveToTrash(const String& path)
{
    return false;
}

inline bool rename(const String& src, const String& dst)
{
    return ::rename(src.c_str(), dst.c_str()) == 0;
}

inline bool remove(const String& path, const stat_t& st)
{
    int rc;
    if(!isLink(st) && isDirectory(st))
        rc = rmdir(path.c_str());
    else
        rc = unlink(path.c_str());

    return rc == 0;
}

inline bool createFile(const String& path)
{
    int n = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (n != -1)
    {
        close(n);
        return true;
    }

    return false;
}

inline bool createDirectory(const String& path)
{
    int rc = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    return rc == 0;
}

#endif
