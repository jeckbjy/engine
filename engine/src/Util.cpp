#include "Util.h"

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <system_error>
#endif

#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>

CU_NS_BEGIN

#define TWO(c) (0x1u << (c))
#define MASK(c) (((unsigned int)(-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x,c) ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))

uint Util::bitcount(unsigned int n)
{
	// http://gurmeet.net/puzzles/fast-bit-counting-routines/
	n = COUNT(n, 0);
	n = COUNT(n, 1);
	n = COUNT(n, 2);
	n = COUNT(n, 3);
	n = COUNT(n, 4);
	//n = COUNT(n, 5) ;	// for 64-bit integers 
	return n;
}

uint Util::hash_bkdr(const char* data, size_t len)
{
	uint seed = 131;
	uint code = 0;
	for (size_t i = 0; i < len; ++i)
	{
		code = code * seed + (*data++);
	}
	return code & 0x7FFFFFFF;
}

String Util::exeName(bool removeDebugFlag)
{
	String path;

#ifdef _WIN32
	char szTmp[_MAX_PATH];
	GetModuleFileNameA(NULL, szTmp, _MAX_PATH);
	// 路径解析
	path = szTmp;
	String::size_type pos = path.find_last_of("\\");
	if (pos != String::npos)
	{
		path = path.substr(pos + 1);
	}
	// .exe解析
	String::size_type extPos = path.find_last_of(".");
	if (extPos != String::npos)
		path = path.substr(0, extPos);
#else
#	define _MAX_PATH 255
	char szModuleName[_MAX_PATH] = { 0 };
	readlink("/proc/self/exe", szModuleName, _MAX_PATH);
	int i = (int)strlen(szModuleName) - 1;
	while (szModuleName[i] != '.' && i > 0)
	{
		--i;
	}

	if (i > 0)
	{
		szModuleName[i] = 0;
	}
	i = (int)strlen(szModuleName) - 1;
	while (szModuleName[i] != '/')
	{
		--i;
	}
	path.assign(szModuleName + i + 1);
#endif

	// debug 解析
	if (removeDebugFlag && path.size() >= 2 && path[path.size() - 2] == '_')
	{
		//char back = path.back();
		char back = path[path.size() - 1];
		if (back == 'D' || back == 'd')
			path = path.substr(0, path.size() - 2);
	}

	return path;
}

String Util::localIP()
{
	// 只保留一个
	static String result;

	if (!result.empty())
		return result;

	char name[100];
	if (gethostname(name, sizeof(name)) == 0)
	{
		hostent* he = gethostbyname(name);
		if (he != 0)
		{
			for (int i = 0; he->h_addr_list[i] != 0; ++i)
			{
				in_addr addr;
				memcpy(&addr, he->h_addr_list[i], sizeof(in_addr));
				result = inet_ntoa(addr);
				if (result != "127.0.0.1")
					return result;
			}
		}

	}

	result = "127.0.0.1";

	return result;
}

size_t Util::cpuCount()
{
#ifdef _MSC_VER
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
#else
	return sysconf(_SC_NPROCESSORS_CONF);//_SC_NPROCESSORS_ONLN,正在运行的cpu个数
#endif
}

uint64_t Util::time()
{
#ifdef _WIN32
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	return ts.QuadPart / 10000;
#else
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		throw std::runtime_error("cannot get time of day");
	return tv.tv_sec * 1000 + tv.tv_usec * 0.001;
#endif
}

void Util::msleep(long millsec)
{
#ifdef _WIN32
	Sleep(millsec);
#else
	usleep(millsec * 1000);
#endif
}

bool Util::readUtf8Bom(std::ifstream& ifs)
{
	if (!ifs.is_open())
		return false;
	static uint8_t utf8_bom[3] = { 0xEF, 0xBB, 0xBF };
	char bom[3] = { 0, 0, 0 };
	ifs.read(bom, 3);
	bool utf8 = false;
	if (ifs.gcount() == 3 && memcmp(bom, utf8_bom, 3) == 0)
		utf8 = true;
	// 不是utf8从头重新开始读
	if (!utf8)
		ifs.seekg(0, std::ios::beg);

	return utf8;
}

void Util::toHex(String& result, const char* data, std::size_t len)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (uint32_t i = 0; i < len; ++i)
	{
		ss << std::hex << std::setw(2) << (uint32_t)data[i];
	}

	result = ss.str();
}

void Util::toPosixPath(String& path)
{
	String::size_type pos = 0;
	for (;;)
	{
		pos = path.find("\\", pos);
		if (pos == String::npos)
			break;
		path[pos] = '/';
	}
}

bool Util::exists(const String& path)
{
	// 可以使用判断是否是目录(fileStat.st_mode & S_IFDIR)
	struct stat fileStat;
	return (stat(path.c_str(), &fileStat) == 0);
}

bool Util::isDir(const String& path)
{
#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif
	struct stat fileStat;
	if (stat(path.c_str(), &fileStat) == 0)
		return S_ISDIR(fileStat.st_mode);
	return false;
}

bool Util::isFile(const String& path)
{
#ifndef S_ISREG
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif
	struct stat fileStat;
	if (stat(path.c_str(), &fileStat) == 0)
		return S_ISREG(fileStat.st_mode);
	return false;
}

bool Util::createDir(const String& path, bool recursive /* = true */)
{
	// 如果以"/"结尾则忽略掉
	if (path.size() >= 1 && path[path.size() - 1] == '/')
	{
		String parent = path.substr(0, path.size() - 1);
		return createDir(parent);
	}

	// 已经存在
	if (isDir(path))
		return true;

	// 先判断父目录是否存在
	if (recursive)
	{
		// 创建失败，判断父目录
		String::size_type pos = path.find_last_of('/');
		if (pos != String::npos)
		{
			String parent = path.substr(0, pos);
			if (!createDir(parent))
				return false;
		}
	}

#ifdef _WIN32
	return _mkdir(path.c_str()) == 0;
#else
	return mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#endif
}

bool Util::createFile(const String& path, bool checkDir)
{
	// 已经存在
	if (exists(path))
		return true;
	// 先直接创建
#ifdef _WIN32
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return true;
	}
	if (!checkDir || GetLastError() != ERROR_PATH_NOT_FOUND)
		return false;

	// 创建目录
	std::size_t pos = path.find_last_of('/');
	if (pos == String::npos)
		return false;
	String dir = path.substr(0, pos);
	if (!createDir(dir, true))
		return false;

	// 再创建一次
	hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return true;
	}

	return false;
#else

	int n = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	if (!checkDir || errno != ENOTDIR)
		return false;

	// 创建目录
	size_t pos = path.find_last_of('/');
	if (pos == String::npos)
		return false;
	String dir = path.substr(0, pos);
	if (!createDir(dir, true))
		return false;

	n = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	return false;
#endif
}

void Util::vformat(String& result, const char* fmt, va_list& va)
{
	int size = vsnprintf(0, 0, fmt, va);
	if (size > 0)
	{
		result.resize(size + 1);
		size = vsnprintf(&result[0], size + 1, fmt, va);
		if (size == -1)
			result.clear();
		else
			result[size] = '\0';
	}
}

void Util::vformat(String& result, const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vformat(result, fmt, va);
	va_end(va);
}

String Util::vformat(const char* fmt, ...)
{
	std::string result;
	va_list va;
	va_start(va, fmt);
	vformat(result, fmt, va);
	va_end(va);
	return result;
}

void Util::split(StringList& tokens, const String& str, const String& delimiter /* = "|" */, bool compress /* = true */)
{
	size_t last_pos = 0;
	size_t pos;
	do
	{
		pos = str.find_first_of(delimiter, last_pos);
		if (pos == std::string::npos)
		{
			if (last_pos != str.length())
				tokens.push_back(str.substr(last_pos, str.length() - last_pos));
			break;
		}
		tokens.push_back(str.substr(last_pos, pos - last_pos));

		if (compress)
			last_pos = str.find_first_not_of(delimiter, pos);
		else
			last_pos = pos + delimiter.length();

		if (last_pos == std::string::npos || last_pos == str.length())
		{// delimiters是最后结尾
			tokens.push_back("");
			break;
		}

	} while (true);
}

void Util::toUpper(String& str)
{
	for (String::iterator it = str.begin(); it != str.end(); ++it)
		*it = ::toupper(*it);
}

void Util::toLower(String& str)
{
	for (String::iterator it = str.begin(); it != str.end(); ++it)
		*it = ::tolower(*it);
}

void Util::trim(String& str)
{
	size_t first = 0;
	size_t last = str.size() - 1;

	while (first <= last && ::isspace(str[first])) ++first;
	while (last >= first && ::isspace(str[last])) --last;

	str.resize(last + 1);
	str.erase(0, first);
}

void Util::trimLeft(String& str)
{
	size_t first = 0;
	size_t last = str.size() - 1;
	while (first <= last && ::isspace(str[first]))
		++first;
	str.erase(0, first);
}

void Util::trimRight(String& str)
{
	size_t last = str.size() - 1;
	while (last >= 0 && ::isspace(str[last]))
		--last;
	str.resize(last + 1);
}

bool Util::startsWith(const String& str, const String& pattern, bool lowerCase /* = true */)
{
	size_t len = pattern.size();
	if (len == 0 || str.size() < len)
		return false;

	if (lowerCase)
		return strncasecmp(str.c_str(), pattern.c_str(), len) == 0;
	else
		return strncmp(str.c_str(), pattern.c_str(), len) == 0;
}

bool Util::endsWith(const String& str, const String& pattern, bool lowerCase /* = true */)
{
	size_t len = pattern.size();
	if (len == 0 || str.size() < len)
		return false;
	const char* ptr = str.c_str() + (str.size() - len);
	if (lowerCase)
		return strncasecmp(ptr, pattern.c_str(), len) == 0;
	else
		return strncmp(ptr, pattern.c_str(), len) == 0;
}

CU_NS_END