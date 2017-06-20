#pragma once
#include "Cute/Chunk.h"

CUTE_NS_BEGIN

// 大内存块
class CUTE_CORE_API ChunkPool
{
public:
	static Chunk* alloc(size_t size);
	static void free(Chunk* chunk);
	static ChunkPool& get();

	ChunkPool();
	~ChunkPool();

private:
	size_t m_minChunk;
	size_t m_maxChunk;
	size_t m_maxMemory;
	size_t m_memory;
	Chunk* m_chunks[1];
};

CUTE_NS_END
