#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Chunk
{
public:
	static Chunk* create(uint32_t size);
	static Chunk* create(char* data, uint32_t size);

private:
	uint32_t	gather : 1;	// �Ƿ���һ���ڴ�
	uint32_t	free : 1;	// �Ƿ���Ҫ�ͷ�
	uint32_t	refs : 30;	// ���ü���
	uint32_t	size;		// ���ݴ�С,����ͷ����
	char*		base;		// ���ݻ�ַ
};

CUTE_NS_END
