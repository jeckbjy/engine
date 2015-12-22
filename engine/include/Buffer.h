#pragma once
#include "API.h"

CU_NS_BEGIN

/*
特点：
1:内存不连续
2:zero copy:共享内存，写时拷贝
*/
class CU_API Buffer
{
	struct buff_t;
	struct node_t;
public:
	Buffer(size_t bytes = 10);
	~Buffer();

	void slice(Buffer& buf, size_t off, size_t len);
	bool append(const Buffer& buf);

	bool peek(void* buf, size_t len);
	bool read(void* buf, size_t len);
	bool write(const void* buf, size_t len);
	bool append(const void* buf, size_t len);
	bool expand(size_t len);
	void seek(long len, int origin = SEEK_CUR);
	// 删除头部数据到cpos处
	void discard();
	// 将数据移动到头部,
	void compact();
	// 多个buff合并成一个
	void concat();

	bool eof() const { return m_cpos >= m_size; }
	bool empty() const { return m_size == 0; }
	size_t size() const { return m_size; }
	size_t position() const { return m_cpos; }

	char*  data() { return m_head->data; }
	char*  chunk_data() { return m_curr->data; }
	size_t chunk_size() const { return m_curr->leng; }

private:
	node_t* alloc(size_t len);
	void destroy(node_t* node);
	void push_back(node_t* node);
	void check_copy(node_t* node);
	void seek(node_t* node, size_t offs, long len);

private:
	// 底层内存块
	struct buff_t
	{
		size_t refs;	// 引用,当>1时写时需要拷贝
		size_t leng;	// 整个内存块大小
		char*  base;	// 数据基址
	};

	// 如果是写buf则
	struct node_t
	{
		node_t* prev;
		node_t* next;
		buff_t* buff;
		char*	data;	// 数据起始
		size_t	leng;	// 数据长度
		node_t() :prev(0), next(0), buff(0), data(0), leng(0){}
	};

	// 会自动过滤0数据
	struct itor_t
	{
		node_t* node;	// 当前节点
		char*	data;	// 当前可用数据
		size_t	size;	// 当前可用长度
		size_t	leng;	// 剩余数据长度
		size_t	index;	// 累计偏移
		itor_t(node_t* node, size_t offs, size_t leng);
		void next();
		bool has_data() const { return leng > 0; }
		size_t offset() const { return data - node->data; }
	};
	// 链表头
	node_t* m_head;
	node_t* m_curr;
	size_t	m_offs;		// 相对于curr的偏移
	size_t	m_cpos;		// position
	size_t	m_size;		// 总大小
	size_t	m_alloc;	// 分配内存大小
};

CU_NS_END
