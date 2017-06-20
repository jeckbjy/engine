#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Chunk
{
public:
	static Chunk* create(uint32_t size);
	static Chunk* create(char* data, uint32_t size);

private:
	uint32_t	gather : 1;	// 是否是一块内存
	uint32_t	free : 1;	// 是否需要释放
	uint32_t	refs : 30;	// 引用计数
	uint32_t	size;		// 数据大小,不含头数据
	char*		base;		// 数据基址
};

CUTE_NS_END
