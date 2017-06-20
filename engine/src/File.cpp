//! FileSystem
#include "Cute/File.h"
#include "Cute/String.h"
#include "Cute/Exception.h"
#include "Cute/Path.h"
#include "Cute/DirectoryIterator.h"

CUTE_NS_BEGIN

bool File::readAll(const String& path, String& data, bool tryRemoveBom)
{
	if (!exists(path))
		return false;
	size_t size = (size_t)getSize(path);
	if (size == 0)
		return true;

	if (tryRemoveBom)
	{
		// read bom
	}

	data.resize(size);
	FILE* file = fopen(data.c_str(), "r");
	fread(&data[0], 1, size, file);
	fclose(file);
	return true;
}

#ifdef CUTE_OS_FAMILY_WINDOWS
//////////////////////////////////////////////////////////////////////////
// windows
//////////////////////////////////////////////////////////////////////////
#undef GetFileAttributes
#undef GetFileAttributesEx
#undef GetDiskFreeSpaceEx
#undef RemoveDirectory
#undef DeleteFile
#undef MoveFileEx
#undef MoveFile
#undef CopyFile
#undef CreateFile
#undef CreateDirectory
#undef SetFileAttributes

#ifdef CUTE_WIN32_UTF8
#define SetFileAttributes	SetFileAttributesW
#define GetFileAttributes	GetFileAttributesW
#define GetFileAttributesEx GetFileAttributesExW
#define GetDiskFreeSpaceEx	GetDiskFreeSpaceExW
#define RemoveDirectory		RemoveDirectoryW
#define DeleteFile			DeleteFileW
#define MoveFileEx			MoveFileExW
#define MoveFile			MoveFileW
#define CopyFile			CopyFileW
#define CreateFile			CreateFileW
#define CreateDirectory		CreateDirectoryW
#else
#define SetFileAttributes	SetFileAttributesA
#define GetFileAttributes	GetFileAttributesA
#define GetFileAttributesEx GetFileAttributesExA
#define GetDiskFreeSpaceEx	GetDiskFreeSpaceExA
#define RemoveDirectory		RemoveDirectoryA
#define DeleteFile			DeleteFileA
#define MoveFileEx			MoveFileExA
#define MoveFile			MoveFileA
#define CopyFile			CopyFileA
#define CreateFile			CreateFileA
#define CreateDirectory		CreateDirectoryA
#endif

#define CreateFileT(tpath,access,share,disp) CreateFile(tpath, access,share,0, disp, 0, 0)

bool File::exists(const String& path)
{
	TCONVERT(path);
	DWORD attr = GetFileAttributes(LPCTSTR(path));
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_NOT_READY:
		case ERROR_INVALID_DRIVE:
			return false;
		default:
			error(path);
		}
	}

	return true;
}


bool File::canRead(const String& path)
{
	TCONVERT(path);
	DWORD attr = GetFileAttributes(LPCTSTR(path));
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		switch (GetLastError())
		{
		case ERROR_ACCESS_DENIED:
			return false;
		default:
			error(path);
		}
	}
	return true;
}

bool File::canWrite(const String& path)
{
	TCONVERT(path);
	DWORD attr = GetFileAttributes(LPCTSTR(path));
	if (attr == INVALID_FILE_ATTRIBUTES)
		error(path);
	return (attr & FILE_ATTRIBUTE_READONLY) == 0;
}

bool File::canExecute(const String& path)
{
	return endsWith<String>(path, ".exe");
}

bool File::isFile(const String& path)
{
	return !isDirectory(path) && !isDevice(path);
}

bool File::isLink(const String& path)
{
	return false;
}

bool File::isDirectory(const String& path)
{
	TCONVERT(path);
	DWORD attr = GetFileAttributes(LPCTSTR(path));
	if (attr == INVALID_FILE_ATTRIBUTES)
		error(path);

	return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool File::isDevice(const String& path)
{
	return
		path.compare(0, 4, "\\\\.\\") == 0 ||
		icompare(path, "CON") == 0 ||
		icompare(path, "PRN") == 0 ||
		icompare(path, "AUX") == 0 ||
		icompare(path, "NUL") == 0 ||
		(
		(icompare(path, 0, 3, "LPT") == 0 || icompare(path, 0, 3, "COM") == 0)
		&& path.size() == 4
		&& path[3] > 0x30
		&& isdigit(path[3])
		);
}

bool File::isHidden(const String& path)
{
	TCONVERT(path);
	DWORD attr = GetFileAttributes(LPCTSTR(path));
	if (attr == INVALID_FILE_ATTRIBUTES)
		error(path);
	return (attr & FILE_ATTRIBUTE_HIDDEN) != 0;
}

uint64 File::getCreationTime(const String& path)
{
	TCONVERT(path);
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (GetFileAttributesEx(LPCTSTR(path), GetFileExInfoStandard, &fad) == 0)
		error(path);
	return (uint64)ConvertTimeToEngine(fad.ftCreationTime.dwLowDateTime, fad.ftCreationTime.dwHighDateTime);
}

uint64 File::getLastModified(const String& path)
{
	TCONVERT(path);
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (GetFileAttributesEx(LPCTSTR(path), GetFileExInfoStandard, &fad) == 0)
		error(path);
	return (uint64)ConvertTimeToEngine(fad.ftLastWriteTime.dwLowDateTime, fad.ftLastWriteTime.dwHighDateTime);
}

void File::setLastModified(const String& path, uint64 ts)
{
	TCONVERT(path);
	HANDLE handle = CreateFileT(LPCTSTR(path), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
	if (handle == INVALID_HANDLE_VALUE)
		error(path);

	HandleScope scope(handle);

	FILETIME ft;
	ConvertTimeToSystem(ts, ft.dwLowDateTime, ft.dwHighDateTime);
	if (SetFileTime(handle, 0, &ft, &ft) == 0)
		error(path);
}

void File::setWriteable(const String& path, bool flag /* = true */)
{
	TCONVERT(path);
	DWORD attr = GetFileAttributes(LPCTSTR(path));
	if (attr == INVALID_FILE_ATTRIBUTES)
		error(path);

	if (flag)
		attr &= ~FILE_ATTRIBUTE_READONLY;
	else
		attr |= FILE_ATTRIBUTE_READONLY;

	if (SetFileAttributes(LPCTSTR(path), attr) == FALSE)
		error(path);
}

void File::setExecutable(const String& path, bool flag /* = true */)
{
	// not supported
}

void File::setReadOnly(const String& path, bool flag /* = true */)
{
	setWriteable(path, !flag);
}

void File::setSize(const String& path, uint64 size)
{
	TCONVERT(path);
	HANDLE handle = CreateFileT(LPCTSTR(path), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
	if (handle == INVALID_HANDLE_VALUE)
		error(path);
	HandleScope scope(handle);

	LARGE_INTEGER li;
	li.QuadPart = size;
	if (SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		error(path);
	if (SetEndOfFile(handle) == 0)
		error(path);

}

uint64 File::getSize(const String& path)
{
	TCONVERT(path);
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (GetFileAttributesEx(LPCTSTR(path), GetFileExInfoStandard, &fad) == FALSE)
		error(path);
	LARGE_INTEGER li;
	li.LowPart = fad.nFileSizeLow;
	li.HighPart = fad.nFileSizeHigh;
	return li.QuadPart;
}

uint64_t File::totalSpace(const String& path)
{
	TCONVERT(path);
	ULARGE_INTEGER space;
	if (!GetDiskFreeSpaceEx(LPCTSTR(path), NULL, &space, NULL))
		error(path);
	return space.QuadPart;
}

uint64_t File::usableSpace(const String& path)
{
	TCONVERT(path);
	ULARGE_INTEGER space;
	if (!GetDiskFreeSpaceEx(LPCTSTR(path), &space, NULL, NULL))
		error(path);
	return space.QuadPart;
}

uint64_t File::freeSpace(const String& path)
{
	TCONVERT(path);
	ULARGE_INTEGER space;
	if (!GetDiskFreeSpaceEx(LPCTSTR(path), NULL, NULL, &space))
		error(path);
	return space.QuadPart;
}


void File::copy(const String& src, const String& dst)
{
	TCONVERT(src);
	TCONVERT(dst);
	if (CopyFile(LPCTSTR(src), LPCTSTR(dst), FALSE) == FALSE)
		error(src);
}

void File::move(const String& src, const String& dst)
{
	TCONVERT(src);
	TCONVERT(dst);
	if (MoveFile(LPCTSTR(src), LPCTSTR(dst)) == FALSE)
		error(src);
}

void File::rename(const String& src, const String& dst)
{
	TCONVERT(src);
	TCONVERT(dst);
	if (MoveFileEx(LPCTSTR(src), LPCTSTR(dst), MOVEFILE_REPLACE_EXISTING) == FALSE)
		error(src);
}

void File::_remove(const String& path)
{
	TCONVERT(path);
	BOOL result;
	if (isDirectory(path))
	{
		result = RemoveDirectory(LPCTSTR(path));
	}
	else
	{
		result = DeleteFile(LPCTSTR(path));
	}

	if (result == FALSE)
		error(path);
}

bool File::createFile(const String& path)
{
	TCONVERT(path);
	HANDLE hFile = CreateFile(LPCTSTR(path), GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return true;
	}
	else if (GetLastError() == ERROR_FILE_EXISTS)
		return false;
	else
		error(path);
	return false;
}

bool File::createDirectory(const String& path)
{
	TCONVERT(path);

	// check exists
	DWORD attr = GetFileAttributes(LPCTSTR(path));
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
		return false;

	if (CreateDirectory(LPCTSTR(path), 0) == 0)
		error(path);
	return true;
}

void File::error(const String& path)
{
	DWORD err = GetLastError();
	switch (err)
	{
	case ERROR_FILE_NOT_FOUND:
		throw FileNotFoundException(path, err);
	case ERROR_PATH_NOT_FOUND:
	case ERROR_BAD_NETPATH:
	case ERROR_CANT_RESOLVE_FILENAME:
	case ERROR_INVALID_DRIVE:
		throw PathNotFoundException(path, err);
	case ERROR_ACCESS_DENIED:
		throw FileAccessDeniedException(path, err);
	case ERROR_ALREADY_EXISTS:
	case ERROR_FILE_EXISTS:
		throw FileExistsException(path, err);
	case ERROR_INVALID_NAME:
	case ERROR_DIRECTORY:
	case ERROR_FILENAME_EXCED_RANGE:
	case ERROR_BAD_PATHNAME:
		throw PathSyntaxException(path, err);
	case ERROR_FILE_READ_ONLY:
		throw FileReadOnlyException(path, err);
	case ERROR_CANNOT_MAKE:
		throw CreateFileException(path, err);
	case ERROR_DIR_NOT_EMPTY:
		throw FileException("directory not empty", path, err);
	case ERROR_WRITE_FAULT:
		throw WriteFileException(path, err);
	case ERROR_READ_FAULT:
		throw ReadFileException(path, err);
	case ERROR_SHARING_VIOLATION:
		throw FileException("sharing violation", path, err);
	case ERROR_LOCK_VIOLATION:
		throw FileException("lock violation", path, err);
	case ERROR_HANDLE_EOF:
		throw ReadFileException("EOF reached", path, err);
	case ERROR_HANDLE_DISK_FULL:
	case ERROR_DISK_FULL:
		throw WriteFileException("disk is full", path, err);
	case ERROR_NEGATIVE_SEEK:
		throw FileException("negative seek", path, err);
	default:
		throw FileException(path, err);
	}
}

#else
//////////////////////////////////////////////////////////////////////////
// posix
//////////////////////////////////////////////////////////////////////////
bool File::exists(const String& path)
{
	struct stat st;
	return stat(path.c_str(), &st) == 0;
}

bool File::canRead(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IRUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IRGRP) != 0;
		else
			return (st.st_mode & S_IROTH) != 0 || geteuid() == 0;
	}
	else error(path);
	return false;
}

bool File::canWrite(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IWUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IWGRP) != 0;
		else
			return (st.st_mode & S_IWOTH) != 0 || geteuid() == 0;
	}
	else error(path);
	return false;
}

bool File::canExecute(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid() || geteuid() == 0)
			return (st.st_mode & S_IXUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IXGRP) != 0;
		else
			return (st.st_mode & S_IXOTH) != 0;
	}
	else error(path);
	return false;
}

bool File::isFile(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return S_ISREG(st.st_mode);
	else
		error(path);
	return false;
}

bool File::isDirectory(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return S_ISDIR(st.st_mode);
	else
		error(path);
	return false;
}

bool File::isDevice(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
	else
		error(path);
	return false;
}

bool File::isHidden(const String& path)
{
	Path p(path);
	p.makeFile();

	return p.getFileName()[0] == '.';
}

bool File::isLink(const String& path)
{
	struct stat st;
	if (lstat(path.c_str(), &st) == 0)
		return S_ISLNK(st.st_mode);
	else
		error(path);
	return false;
}

uint64 File::getCreationTime(const String& path)
{
	// first, platforms with birthtime attributes
#if defined(__APPLE__) && defined(st_birthtime) // st_birthtime is available only on 10.5
	// a macro st_birthtime makes sure there is a st_birthtimespec (nano sec precision)
	struct stat64 st;
	if (stat64(path.c_str(), &st) == 0)
		return timespec2Microsecs(st.st_birthtimespec);
#elif defined(__FreeBSD__) && defined(st_birthtime)
	// a macro st_birthtime makes sure there is a st_birthtimespec (nano sec precision)
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return timespec2Microsecs(st.st_birthtimespec);
#elif defined(__FreeBSD__)
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return st.st_birthtime * 1000000;
	// then platforms with POSIX 2008-09 compatibility (nanosec precision) (linux 2.6 and later)
#elif (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) \
	|| (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700)
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return timespec2Microsecs(st.st_ctim);
	// finally try just stat with status change with precision to the second.
#else
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return st.st_ctime * 1000000;
#endif 
	else
		error(path);
	return 0;
}

uint64 File::getLastModified(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0) {
#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) \
	|| (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) \
	|| defined(_BSD_SOURCE) || defined(_SVID_SOURCE)
		return timespec2Microsecs(st.st_mtim);
#elif defined(__APPLE__)
		return timespec2Microsecs(st.st_mtimespec);
#else
		return st.st_mtime * 1000000;
#endif
	}
	else
		error(path);

	return 0;
}

void File::setLastModified(const String& path, uint64 ts)
{
	struct timeval tb[2];
	tb[0].tv_sec = ts/ 1000000;
	tb[0].tv_usec = ts % 1000000;
	tb[1] = tb[0];
	if (utimes(path.c_str(), tb) != 0)
		error(path);
}

void File::setWriteable(const String& path, bool flag /* = true */)
{
	struct stat st;
	if (stat(path.c_str(), &st) != 0)
		error(path);
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
		error(path);
}

void File::setExecutable(const String& path, bool flag /* = true */)
{
	struct stat st;
	if (stat(path.c_str(), &st) != 0)
		error(path);
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
		error(path);
}

void File::setReadOnly(const String& path, bool flag /* = true */)
{
	setWriteable(path, !flag);
}

void File::setSize(const String& path, uint64 size)
{
	if (truncate(path.c_str(), size) != 0)
		error(path);
}

uint64 File::getSize(const String& path)
{
	struct stat st;
	if (stat(path.c_str(), &st) == 0)
		return st.st_size;
	else
		error(path);
	return 0;
}

uint64 File::totalSpace(const String& path)
{
	struct statfs stats;
	if (statfs(const_cast<char*>(path.c_str()), &stats) != 0)
		error(path);

	return (uint64)stats.f_blocks * (uint64)stats.f_bsize;
}

uint64 File::usableSpace(const String& path)
{
	struct statfs stats;
	if (statfs(const_cast<char*>(path.c_str()), &stats) != 0)
		error(path);

	return (uint64)stats.f_bavail * (uint64)stats.f_bsize;
}

uint64_t File::freeSpace(const String& path)
{
	struct statfs stats;
	if (statfs(const_cast<char*>(src.c_str()), &stats) != 0)
		error(src);

	return (uint64)stats.f_bfree * (uint64)stats.f_bsize;
}

void File::copy(const String& src, const String& dst)
{
	int sd = open(src.c_str(), O_RDONLY);
	if (sd == -1) error(src);

	struct stat st;
	if (fstat(sd, &st) != 0)
	{
		close(sd);
		error(src);
	}
	const long blockSize = st.st_blksize;

	int dd = open(dst.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	if (dd == -1)
	{
		close(sd);
		error(dst);
	}
	std::vector<char> buffer(blockSize);
	try
	{
		int n;
		while ((n = read(sd, buffer.begin(), blockSize)) > 0)
		{
			if (write(dd, buffer.begin(), n) != n)
				error(dst);
		}
		if (n < 0)
			error(src);
	}
	catch (...)
	{
		close(sd);
		close(dd);
		throw;
	}
	close(sd);
	if (fsync(dd) != 0)
	{
		close(dd);
		error(dst);
	}
	if (close(dd) != 0)
		error(dst);
}

void File::move(const String& src, const String& dst)
{
	if (rename(src.c_str(), dst.c_str()) != 0)
		error(src);
}

void File::rename(const String& src, const String& dst)
{
	if (rename(src.c_str(), dst.c_str()) != 0)
		error(src);
}

void File::_remove(const String& path)
{
	int rc;
	if (!isLink(path) && isDirectory(path))
		rc = rmdir(path.c_str());
	else
		rc = unlink(path.c_str());

	if (rc)
		error(path);
}

bool File::createFile(const String& path)
{
	int n = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	if (n == -1 && errno == EEXIST)
		return false;
	else
		error(path);
	return false;
}

bool File::createDirectory(const String& path)
{
	if (exists(path) && isDirectory(path))
		return false;
	if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0)
		error(path);
	return true;
}

void File::error(const String& path)
{
	switch (errno)
	{
	case EIO:
		throw IOException(path, errno);
	case EPERM:
		throw FileAccessDeniedException("insufficient permissions", path, errno);
	case EACCES:
		throw FileAccessDeniedException(path, errno);
	case ENOENT:
		throw FileNotFoundException(path, errno);
	case ENOTDIR:
		throw OpenFileException("not a directory", path, errno);
	case EISDIR:
		throw OpenFileException("not a file", path, errno);
	case EROFS:
		throw FileReadOnlyException(path, errno);
	case EEXIST:
		throw FileExistsException(path, errno);
	case ENOSPC:
		throw FileException("no space left on device", path, errno);
	case EDQUOT:
		throw FileException("disk quota exceeded", path, errno);
#if !defined(_AIX)
	case ENOTEMPTY:
		throw FileException("directory not empty", path, errno);
#endif
	case ENAMETOOLONG:
		throw PathSyntaxException(path, errno);
	case ENFILE:
	case EMFILE:
		throw FileException("too many open files", path, errno);
	default:
		throw FileException(std::strerror(errno), path, errno);
	}
}
#endif

void File::remove(const String& path, bool recursive /* = false */)
{
	if (recursive && !isLink(path) && isDirectory(path))
	{
		// 遍历并删除
		DirectoryIterator it(path);
		DirectoryIterator end;
		while (it != end)
		{
			remove(it.name());
			++it;
		}
	}

	_remove(path);
}

void File::createDirectories(const String& path)
{
	if (!exists(path))
	{
		Path p(path);
		p.makeDirectory();
		if (p.depth() > 1)
		{
			p.makeParent();
			createDirectories(p.toString());
		}

		createDirectory(path);
	}
}

void File::list(const String& path, std::vector<String>& files)
{
	files.clear();
	DirectoryIterator it(path);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(it.name());
		++it;
	}
}

CUTE_NS_END
