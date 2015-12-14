#pragma once
#include "API.h"
#include "StringUtil.h"

CU_NS_BEGIN

class CU_API Util
{
public:
	static uint bitcount(unsigned int n);	// 1的个数
	static uint hash_bkdr(const char* data, size_t len);

	static String exeName(bool removeDebugFlag = true);	// 获取程序名,去除后缀和"_D"标识
	static String localIP();		// 获得一个本地IP
	static size_t cpuCount();		// 获取cpu个数

	static uint64_t time();		// 当前毫秒时间：millsecond
	static void msleep(long millsec);

	static bool readUtf8Bom(std::ifstream& ifs);
	static void toHex(String& result, const char* data, std::size_t len);	// 字节转化为16进制字符
	static void toPosixPath(String& path);	// 将"\"转化为"/"
	static bool exists(const String& path);	// 文件是否存在
	static bool isDir(const String& path);	// 是否是目录,不能以"/"结尾
	static bool isFile(const String& path);	// 是否是文件
	static bool createDir(const String& path, bool recursive = true);	// 创建目录，以"/"分割
	static bool createFile(const String& path, bool checkDir = true);	// 创建文件，checkDir:自动创建目录
};

CU_NS_END