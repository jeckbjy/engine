#pragma once
#include "API.h"

CU_NS_BEGIN

// �ڴ治�����������ڴ�,дʵ����
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
	// �ӵ�ǰλ�÷ֲ������,���ŵ�other��β
	void split(Buffer& other);

	// ��Ե�ǰ�α�λ��
	bool peek(void* data, size_t length);
	bool read(void* data, size_t length);
	bool write(const void* data, size_t length);

	// ������ڵ�ǰ�α�
	bool get(void* data, size_t length, size_t offset = 0);
	bool put(const void* data, size_t length, size_t offset = 0);
	void insert(const void* data, size_t length, size_t offset);
	void erase(size_t length, size_t offset);
	bool expand(size_t length);	// ����
	void seek(long length, int origin = SEEK_CUR);

	// ��������
	void release();
	void discard();		// ������ǰλ��֮ǰ������
	void compact();		// �������Ƶ���ʼλ��
	void concat();		// �ϲ���һ�����ݿ�

	// socket ����ȫ��
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
	node_t* m_head;		// ˫������ͷ
	node_t* m_curr;		// ��ǰ�α�ڵ�
	size_t	m_offs;		// �����curr��ƫ��
	size_t	m_cpos;		// position,��������
	size_t	m_size;		// �ܴ�С
	size_t	m_mark;		// ����ʶʹ��,û�κ�����
	size_t	m_alloc;	// �����ڴ��С
};

CU_NS_END
