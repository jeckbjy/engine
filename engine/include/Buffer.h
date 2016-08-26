#pragma once
#include "API.h"

CU_NS_BEGIN

// 内存不连续，共享内存,写实拷贝
struct buff_t;
struct node_t;
class CU_API Buffer
{
public:
	Buffer(size_t alloc = 1024);
	~Buffer();

	Buffer(const Buffer& other);
	Buffer& operator=(const Buffer& other);

	void merge(const Buffer& other);
	// 从当前位置分拆成两个,并放到other结尾
	void split(Buffer& other);

	// 相对当前游标位置
	bool peek(void* data, size_t length);
	bool read(void* data, size_t length);
	bool write(const void* data, size_t length);

	// 不相对于当前游标
	bool get(void* data, size_t length, size_t offset = 0);
	bool put(const void* data, size_t length, size_t offset = 0);
	void insert(const void* data, size_t length, size_t offset);
	void erase(size_t length, size_t offset);
	bool expand(size_t length);	// 扩容
	void seek(long length, int origin = SEEK_CUR);

	// 其他操作
	void release();
	void discard();		// 丢弃当前位置之前的数据
	void compact();		// 将数据移到起始位置
	void concat();		// 合并成一个数据块

	// socket 接收全部
	void recv(socket_t sock);
	bool send(socket_t sock);

	uint find(char data, size_t offset = 0);
	uint find(char* data, size_t offset = 0);

	uint rfind(char data, size_t offset = 0);
	uint rfind(char* data, size_t offset = 0);

	bool eof() const { return m_cpos >= m_size; }
	bool empty() const { return m_size == 0; }
	uint size() const { return m_size; }
	uint position() const { return m_cpos; }

private:
	size_t  calc_size(size_t length);
	node_t* get_node(size_t& offs, size_t& cpos, long length, int origin = SEEK_SET);
	node_t* new_node(size_t length = 0);
	buff_t* new_buff(size_t length, const char* data = NULL);
	void set_node(node_t* node, buff_t* buff, size_t length = 0);
	void free_buff(buff_t* buff);
	void free_node(node_t* node);
	void push_node(node_t* node);
	void check_copy(node_t* node);

	bool _read(node_t* node, size_t offs, void* data, size_t length);
	bool _write(node_t* node, size_t offs, const void* data, size_t length);
	bool _compare(node_t* node, size_t offs, const char* data, size_t length);

private:
	node_t* m_head;		// 双向链表头
	node_t* m_curr;		// 当前游标节点
	size_t	m_offs;		// 相对于curr的偏移
	size_t	m_cpos;		// position,相对于起点
	size_t	m_size;		// 总大小
	size_t	m_mark;		// 纯标识使用,没任何限制
	size_t	m_alloc;	// 分配内存大小
};

CU_NS_END
