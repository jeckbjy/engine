#pragma once
#include "API.h"

CU_NS_BEGIN

/*
�ص㣺
1:�ڴ治����
2:zero copy:�����ڴ棬дʱ����
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
	// ɾ��ͷ�����ݵ�cpos��
	void discard();
	// �������ƶ���ͷ��,
	void compact();
	// ���buff�ϲ���һ��
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
	// �ײ��ڴ��
	struct buff_t
	{
		size_t refs;	// ����,��>1ʱдʱ��Ҫ����
		size_t leng;	// �����ڴ���С
		char*  base;	// ���ݻ�ַ
	};

	// �����дbuf��
	struct node_t
	{
		node_t* prev;
		node_t* next;
		buff_t* buff;
		char*	data;	// ������ʼ
		size_t	leng;	// ���ݳ���
		node_t() :prev(0), next(0), buff(0), data(0), leng(0){}
	};

	// ���Զ�����0����
	struct itor_t
	{
		node_t* node;	// ��ǰ�ڵ�
		char*	data;	// ��ǰ��������
		size_t	size;	// ��ǰ���ó���
		size_t	leng;	// ʣ�����ݳ���
		size_t	index;	// �ۼ�ƫ��
		itor_t(node_t* node, size_t offs, size_t leng);
		void next();
		bool has_data() const { return leng > 0; }
		size_t offset() const { return data - node->data; }
	};
	// ����ͷ
	node_t* m_head;
	node_t* m_curr;
	size_t	m_offs;		// �����curr��ƫ��
	size_t	m_cpos;		// position
	size_t	m_size;		// �ܴ�С
	size_t	m_alloc;	// �����ڴ��С
};

CU_NS_END
