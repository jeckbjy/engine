//! FileSystem
#if defined(CUTE_OS_FAMILY_WINDOWS)

inline bool getFileAttr(const String& path, file_attr_t& attr)
{
    attr = GetFileAttributesA(path.c_str());
    return attr == INVALID_FILE_ATTRIBUTES;
}

inline bool getFileData(const String& path, WIN32_FILE_ATTRIBUTE_DATA& fad)
{
    return GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &fad) == TRUE;
}

inline DWORD getAttributes(const String& path)
{
    return GetFileAttributesA(path.c_str());
}

inline bool getAttributesEx(const String& path, WIN32_FILE_ATTRIBUTE_DATA& fad)
{
    return GetFileAttributesEx (path.c_str(), GetFileExInfoStandard, &fad) == TRUE;
}

inline bool setAttributes(const String& path, DWORD newAttrs, bool flag)
{
    DWORD oldAttrs = getAttributes(path);
    if(oldAttrs == INVALID_FILE_ATTRIBUTES)
        return false;

    if(flag)
        oldAttrs &= ~newAttrs;
    else
        oldAttrs |=  newAttrs;

    return SetFileAttributesA(path.c_str(), oldAttrs) == true;
}

inline bool exists(const String& path)
{
    return getAttributes(path) != INVALID_FILE_ATTRIBUTES;
}


inline bool isFile(DWORD attr)
{
    return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

inline bool isDirectory(DWORD attr)
{
    return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

inline bool isLink(DWORD attr)
{
    return (attr & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
}

 inline bool isDevice(const String& path)
 {
    return
        path.istartsWith("\\\\.\\") || 
        path.iequalsAny(["CON", "PRN", "AUX", "NUL"]) ||
        (path.istartsWithAny(["LPT", "COM"] && path.size() == 4 && path[3] > 0x30 && isdigit(path[3])
        );
 }

inline bool isHidden(DWORD attr)
{
    return (attr & FILE_ATTRIBUTE_HIDDEN) != 0;
}

inline bool isReadable(DWORD attr)
{
    return attr != INVALID_FILE_ATTRIBUTES;
}

inline bool isWritable(DWORD attr)
{
    return (attr & FILE_ATTRIBUTE_READONLY) == 0;
}

inline bool isExecutable()
{
    return true;
}

inline bool setWritable(const String& path, bool flag)
{
    return setAttributes(path, FILE_ATTRIBUTE_READONLY, flag);
}

inline bool setExecutable(const String&, bool)
{
    return false;
}

inline FILETIME* toFileTime (const int64 time, FILETIME* const ft) noexcept
{
    if (time <= 0)
        return NULL;

    reinterpret_cast<ULARGE_INTEGER*> (ft)->QuadPart = (ULONGLONG) (time * 10000 + 116444736000000000LL);
    return ft;
}

inline bool setFileTimes(const String& path, int64 modifyTime, int64 accessTime, int64 creationTime)
{
    bool ok = false;
    HANDLE h = CreateFileA (m_path.data(), GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (h != INVALID_HANDLE_VALUE)
    {
        FILETIME m, a, c;

        ok = SetFileTime (h,
                          toFileTime (creationTime, &c),
                          toFileTime (accessTime, &a),
                          toFileTime (modificationTime, &m)) != 0;

        CloseHandle (h);
    }

    return ok;
}

inline int64 fromFileTime(const FILETIME* const ft)
{
    // static_assert (sizeof (ULARGE_INTEGER) == sizeof (FILETIME),
    //                "ULARGE_INTEGER is too small to hold FILETIME: please report!");

    return (int64) ((reinterpret_cast<const ULARGE_INTEGER*> (ft)->QuadPart - 116444736000000000LL) / 10000);
}

inline bool getFileTimes(const stat_t& st, int64* modifyTime, int64* accessTime, int64* creationTime)
{
    if(modifyTime)
        *modifyTime = fromFileTime(&st.ftLastWriteTime);
    if(accessTime)
        *accessTime = fromFileTime(&st.ftLastAccessTime);
    if(creationTime)
        *creationTime = fromFileTime(&st.ftCreationTime);
}

inline bool setSize(const String& path, int64 size)
{
    HANDLE handle = CreateFile(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
    if (handle == INVALID_HANDLE_VALUE)
        return false;

    bool result = false;
    LARGE_INTEGER li;
    li.QuadPart = size;
    if (SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN) != INVALID_SET_FILE_POINTER)
    {
        if(SetEndOfFile(handle) != 0)
            result = true;
    }

    CloseHandle(handle);
    return result;
}

inline int64 getSize(const stat_t& fad)
{
    LARGE_INTEGER li;
    li.LowPart  = fad.nFileSizeLow;
    li.HighPart = fad.nFileSizeHigh;
    return li.QuadPart;
}

inline int64 getTotalSpace(const String& path)
{
    ULARGE_INTEGER space;
    if (GetDiskFreeSpaceExA(path.c_str(), NULL, &space, NULL))
        return space.QuadPart;

    return 0;
}

inline int64 getUsableSpace(const String& path)
{
    ULARGE_INTEGER space;
    if (GetDiskFreeSpaceExA(path.c_str(), &space, NULL, NULL))
        return space.QuadPart;

    return 0;
}

inline int64 getFreeSpace(const String& path)
{
    ULARGE_INTEGER space;
    if (GetDiskFreeSpaceExA(path.c_str(), NULL, NULL, &space))
        return space.QuadPart;

    return 0;
}

inline bool copy(const String& src, const String& dst)
{
    return CopyFile(src.c_str(), dst.c_str(), FALSE) == TRUE;
}

inline bool move(const String& src, const String& dst)
{
    return MoveFile(src.c_str(), dst.c_str()) == TRUE;
}

inline bool moveToTrash(const String& path)
{
    SHFILEOPSTRUCT fos = { 0 };
    fos.wFunc = FO_DELETE;
    fos.pFrom = path.c_str();
    fos.fFlags = FOF_ALLOWUNDO | FOF_NOERRORUI | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_RENAMEONCOLLISION;

    return SHFileOperation (&fos) == 0;
}

inline bool rename(const String& src, const String& dst)
{
    return MoveFileEx(src.c_str(), dst.c_str(), MOVEFILE_REPLACE_EXISTING) == TRUE;
}

inline bool remove(const String& path, DWORD attr)
{
    if(isDirectory(attr))
        return RemoveDirectory(path.c_str()) == TRUE;
    else
        return DeleteFile(path.c_str()) == TRUE;
}

inline bool createFile(const String& path)
{
    HANDLE file = CreateFileA(path.c_str(), , GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
    if(file != INVALID_HANDLE_VALUE)
    {
        CloseHandle(file);
        return true;
    }
    else
    {
        return false;
    }
}

inline bool createDirectory(const String& path)
{
    return CreateDirectory(path.c_str(), 0) == TRUE;
}

#endif
