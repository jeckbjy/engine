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

// �ļ�������
class CU_API Importer : public Object
{
public:
	virtual ~Importer(){}

	// extension��check��ѡһʵ�ּ���,�����׺��ʱʹ��check����ʵ��
	virtual const char* extension() const { return NULL; }
	virtual bool check(const String& ext) const { return false; }

	virtual Asset* load(Stream* stream) = 0;
};

CU_NS_END