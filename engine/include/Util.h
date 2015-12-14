#pragma once
#include "API.h"
#include "StringUtil.h"

CU_NS_BEGIN

class CU_API Util
{
public:
	static uint bitcount(unsigned int n);	// 1�ĸ���
	static uint hash_bkdr(const char* data, size_t len);

	static String exeName(bool removeDebugFlag = true);	// ��ȡ������,ȥ����׺��"_D"��ʶ
	static String localIP();		// ���һ������IP
	static size_t cpuCount();		// ��ȡcpu����

	static uint64_t time();		// ��ǰ����ʱ�䣺millsecond
	static void msleep(long millsec);

	static bool readUtf8Bom(std::ifstream& ifs);
	static void toHex(String& result, const char* data, std::size_t len);	// �ֽ�ת��Ϊ16�����ַ�
	static void toPosixPath(String& path);	// ��"\"ת��Ϊ"/"
	static bool exists(const String& path);	// �ļ��Ƿ����
	static bool isDir(const String& path);	// �Ƿ���Ŀ¼,������"/"��β
	static bool isFile(const String& path);	// �Ƿ����ļ�
	static bool createDir(const String& path, bool recursive = true);	// ����Ŀ¼����"/"�ָ�
	static bool createFile(const String& path, bool checkDir = true);	// �����ļ���checkDir:�Զ�����Ŀ¼
};

CU_NS_END