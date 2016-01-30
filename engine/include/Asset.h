#pragma once
#include "Object.h"

CU_NS_BEGIN

class Stream;
class CU_API Asset : public Object
{
public:
	Asset(){}
	~Asset(){}
};

// 文件加载器
class CU_API Importer : public Object
{
public:
	virtual ~Importer(){}

	// extension与check二选一实现即可,多个后缀名时使用check函数实现
	virtual const char* extension() const { return NULL; }
	virtual bool check(const String& ext) const { return false; }

	virtual Asset* load(Stream* stream) = 0;
};

CU_NS_END