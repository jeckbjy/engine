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
	static const uint npos = (uint)-1;
	Buffer(size_t bytes = 1024);
	~Buffer();

	void front(Buffer& buf,size_t len);
	bool append(const Buffer& buf);

	bool peek(void* buf, size_t len);
	bool read(void* buf, size_t len);
	bool write(const void* buf, size_t len);
	bool append(const void* buf, size_t len);
	bool expand(size_t len);
	//void advance(size_t len);
	void seek(long len, int origin = SEEK_CUR);

	void release();
	// ɾ��ͷ�����ݵ�cpos��
	void discard();
	// �������ƶ���ͷ��,
	void compact();
	// ���buff�ϲ���һ��
	void concat();
	uint find(char data, size_t off = 0);

	bool eof() const { return m_cpos >= m_size; }
	bool empty() const { return m_size == 0; }
	uint size() const { return m_size; }
	uint position() const { return m_cpos; }

	char* chunk_data() { return m_curr->data; }
	uint  chunk_size() const { return m_curr->leng; }
	
	// ����������,û�лᴴ��,for recv
	void get_free(char*& data, uint& leng);

	String toString();

private:
	node_t* alloc(size_t len);
	void destroy(node_t* node);
	void push_back(node_t* node);
	void check_copy(node_t* node);

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
		size_t	writable() const { return buff->leng - leng - (data - buff->base); }
	};

	// ���Զ�����0����
	struct itor_t
	{
		node_t* node;	// ��ǰ�ڵ�
		char*	data;	// ��ǰ��������
		size_t	leng;	// ��ǰ���ó���
		size_t	nums;	// ʣ�����ݳ���
		size_t	index;	// �ۼ���ƫ��
		itor_t(node_t* node, size_t offs, size_t nums);
		void next();
		void to_end();
		bool eof() const { return nums == 0; }
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
