#pragma once
#include "API.h"

CU_NS_BEGIN

#define SEEK_BEG 4
// ·ÇÁ¬Ðø
class CU_API Buffer
{
public:
	Buffer(size_t bytes = 0);
	~Buffer();

	bool peek(void* buf, size_t len) const;
	bool read(void* buf, size_t len) const;
	bool write(const void* buf, size_t len);
	bool write(const Buffer& buf);
	bool append(const void* buf, size_t len);
	void seek(long len, int origin = SEEK_BEG);
	bool advance(size_t len);
	bool reserve(size_t len);
	void resize(size_t len);
	void discard();
	void concat();

	void rewind() { seek(0, SEEK_BEG); }
	void skip(size_t len) { seek((long)len, SEEK_CUR); }
	bool eof() const { return m_cpos >= m_epos; }
	bool empty() const { return m_spos == m_epos; }
	size_t size() const { return m_epos - m_spos; }
	size_t space() const { return m_epos - m_cpos; }
	size_t position() const { return m_cpos - m_spos; }
	size_t cursor() const { return m_cpos;  }

	char*  chunk_data() { return m_curr->data; }
	size_t chunk_size() { return m_curr->spos + m_curr->size - m_cpos; }

private:
	struct chunk
	{
		size_t user : 1;
		size_t refs : 31;
		size_t spos;
		size_t size;
		chunk* prev;
		chunk* next;
		char*  data;
	};
	chunk* m_head;
	chunk* m_curr;
	size_t m_bytes;
	size_t m_caps;
	size_t m_spos;
	size_t m_epos;
	size_t m_cpos;
};

CU_NS_END
