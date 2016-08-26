#include "Buffer.h"

CU_NS_BEGIN

// �ײ��ڴ��
struct buff_t
{
	size_t refs;	// ���ü���
	size_t size;	// �ڴ��С
	char*  base;	// ���ݻ�ַ
};

// �����дbuf��
struct node_t
{
	node_t* prev;
	node_t* next;
	buff_t* buff;	// ʵ���ڴ�
	char*	data;	// ������ʼ
	size_t	size;	// ���ݳ���

	size_t	writable() const { return buff->size - size - (data - buff->base); }
};

// ���Զ�����0����
struct itor_t
{
	node_t* node;	// ��ǰ�ڵ�
	char*	data;	// ��ǰ��������
	size_t	leng;	// ��ǰ���ó���
	size_t	nums;	// ʣ�����ݳ���
	size_t	index;	// �ۼ���ƫ��
	itor_t(node_t* node, size_t offs, size_t nums)
		: node(node)
		, nums(nums)
		, index(0)
	{
		assert(offs <= node->size);
		// ���Խ�β��ֱ��������һ��
		if (node->size == offs)
		{
			node = node->next;
			data = node->data;
			leng = node->size;
		}
		else
		{// ����ƫ��
			data = node->data + offs;
			leng = node->size - offs;
		}
		
		// ̫���ض�
		if (leng > nums)
			leng = nums;
	}

	void next()
	{
		// ��ǰ�ƶ�
		nums  -= leng;
		data  += leng;
		index += leng;
		// ���½ڵ�
		if (nums != 0)
		{
			node = node->next;
			data = node->data;
			leng = node->size;
			if (leng > nums)
				leng = nums;
		}
	}
	bool eof() const { return nums == 0; }
	size_t offset() const { return data - node->data; }
};

Buffer::Buffer(size_t alloc /* = 1024 */)
	: m_head(0)
	, m_curr(0)
	, m_offs(0)
	, m_cpos(0)
	, m_size(0)
	, m_alloc(alloc)
{
	if (alloc == 0)
		m_alloc = 1024;
}

Buffer::~Buffer()
{
	release();
}

Buffer::Buffer(const Buffer& other)
	: m_head(0)
	, m_curr(0)
	, m_offs(0)
	, m_cpos(0)
	, m_size(0)
	, m_alloc(other.m_alloc)
{
	merge(other);
}

Buffer& Buffer::operator =(const Buffer& other)
{
	release();
	merge(other);
	return *this;
}

void Buffer::merge(const Buffer& other)
{
	node_t* head = other.m_head;

	// ����
	if (head)
	{
		node_t* node = head;
		node_t* temp;
		do
		{
			temp = new node_t;
			set_node(temp, node->buff, node->size);
			push_node(temp);

			node = node->next;
		} while (node == head);
	}
}

void Buffer::split(Buffer& other)
{
	if (!m_head)
		return;

	node_t* head = m_curr;
	node_t* tail = m_head->prev;

	// �ֲ������
	node_t* node = m_head;
	do 
	{
		if (node == m_curr)
		{
			node_t* temp = new node_t();
			temp->buff = node->buff;
			temp->data = node->data;
			temp->size = m_offs;
			other.push_node(temp);

			// �ָ�����
			m_curr->data += m_offs;
			m_head = m_curr;
			m_head->prev = tail;
			tail->prev = m_head;
			m_cpos = 0;
			m_offs = 0;
			break;
		}
		else
		{
			node_t* temp = node->next;
			other.push_node(node);
			node = temp;
		}

	} while (node != m_curr);
}

bool Buffer::peek(void* buf, size_t len)
{
	if (m_cpos + len > m_size)
		return false;

	return _read(m_curr, m_offs, buf, len);
}

bool Buffer::read(void* buf, size_t len)
{
	if (_read(m_curr, m_offs, buf, len))
	{
		seek(len, SEEK_CUR);
		return true;
	}

	return false;
}

bool Buffer::write(const void* buf, size_t len)
{
	if (len == 0)
		return true;
	size_t epos = m_cpos + len;
	if (epos > m_size)
	{// ��������
		if (!expand(epos - m_size))
			return false;
	}

	if (buf != 0)
		_write(m_curr, m_offs, buf, len);

	seek(len, SEEK_CUR);

	return true;
}

bool Buffer::get(void* data, size_t length, size_t offset /* = 0 */)
{
	if (offset + length >= m_size)
		return false;

	size_t offs, cpos;
	node_t* node = get_node(offs, cpos, offset, SEEK_SET);

	return _read(node, offs, data, length);
}

bool Buffer::put(const void* data, size_t length, size_t offset /* = 0 */)
{
	if (offset + length >= m_size)
		return false;

	size_t offs, cpos;
	node_t* node = get_node(offs, cpos, offset, SEEK_SET);
	if (!node)
		return false;

	return _write(node, offs, data, length);
}

void Buffer::insert(const void* data, size_t length, size_t offset /* = 0 */)
{
	size_t cpos = m_cpos;
	seek(offset, SEEK_SET);
	size_t writable = m_curr->writable();

	char*  ptr = m_curr->data + m_offs;
	size_t len = m_curr->size - m_offs;

	if (length < writable)
	{
		check_copy(m_curr);
		m_curr->size += length;
	}
	else
	{
		// ���´�������������
		buff_t* buff = new_buff(calc_size(m_curr->size + length));
		memcpy(buff->base, m_curr->data, m_offs);
		memcpy(buff->base + m_offs + length, ptr, len);
		free_buff(m_curr->buff);
		set_node(m_curr, buff, m_curr->size + length);
		m_curr->buff = buff;
		m_curr->data = buff->base;
	}

	// �������
	if (data)
		memcpy(ptr, data, length);

	seek(cpos, SEEK_SET);
}

void Buffer::erase(size_t length, size_t offset)
{
	// todo:��ҪУ���Ƿ���Ч
	size_t cpos = m_cpos;
	seek(offset, SEEK_SET);

	if (m_offs + length <= m_curr->size)
	{
		check_copy(m_curr);

		char* data = m_curr->data + m_offs;
		memcpy(data, data + length, m_curr->size - m_offs - length);
	}

	seek(cpos, SEEK_SET);
}

bool Buffer::expand(size_t len)
{
	// ���ݵ������޸ĵ�ǰ�α�
	if (len == 0)
		return true;

	node_t* tail;
	if (m_head)
	{
		tail = m_head->prev;
		check_copy(tail);
	}
	else
	{
		tail = new_node(len);
	}
	
	while (tail && len > 0)
	{
		// ������ÿռ�
		size_t count = tail->buff->size - tail->size;
		if (count > 0)
		{
			if (count > len)
				count = len;
			tail->size += count;
			len -= count;
			m_size += count;
		}
		tail = new_node(len);
	}
	
	return len == 0;
}

void Buffer::seek(long len, int origin /* = SEEK_CUR */)
{
	node_t* node;
	size_t  offs, cpos;
	node = get_node(offs, cpos, len, SEEK_SET);
	if (node != NULL)
	{
		m_curr = node;
		m_offs = offs;
		m_cpos = cpos;
	}
}

void Buffer::release()
{
	if (m_head != 0)
	{
		// �Ͽ���
		m_head->prev->next = 0;
		// �ͷ�
		node_t* temp;
		node_t* node = m_head;
		while (node)
		{
			temp = node;
			node = node->next;
			free_node(temp);
		}

		m_head = 0;
		m_curr = 0;
		m_offs = 0;
		m_cpos = 0;
		m_size = 0;
	}
}

void Buffer::discard()
{
	// ɾ��0-cpos֮�������
	if (m_cpos == 0)
		return;

	if (m_cpos == m_size)
	{
		release();
		return;
	}

	node_t* temp;
	node_t* tail = m_head->prev;
	m_size -= m_cpos;
	while (m_cpos > 0)
	{
		if (m_head->size > m_cpos)
		{
			m_head->data += m_cpos;
			m_cpos = 0;
		}
		else
		{
			temp = m_head;
			m_cpos -= m_head->size;
			m_head = m_head->next;
			free_node(temp);
		}
	}

	// ��������
	assert(m_cpos == 0);
	m_head->prev = tail;
	tail->prev = m_head;
	m_curr = m_head;
	m_offs = 0;
}

void Buffer::compact()
{
	// ѹ�����������ƶ���buffͷ��
	if (empty())
		return;
	concat();
	if (m_head->data == m_head->buff->base)
		return;
	char* base = m_head->buff->base;
	memcpy(base, m_head->data, m_head->size);
	m_head->data = base;
}

void Buffer::concat()
{
	// ֻ��һ��������ϲ�
	if (!m_head || m_head->prev == m_head)
		return;

	// ������������һ��
	buff_t* buff = new_buff(calc_size(m_size));
	seek(0, SEEK_SET);
	peek(buff->base, m_size);

	node_t* node = new node_t;
	node->prev = node;
	node->next = node;
	set_node(node, buff, m_size);

	release();
	m_head = node;
	m_curr = node;
	m_offs = m_cpos;
}

node_t* Buffer::new_node(size_t length)
{
	if (length == 0)
		length = m_alloc;

	buff_t* buff = new_buff(calc_size(length), NULL);
	if (buff == NULL)
		return NULL;

	node_t* node = new node_t;
	set_node(node, buff, 0);
	push_node(node);

	return node;
}

buff_t* Buffer::new_buff(size_t length, const char* data)
{
	if (length == NULL)
		return NULL;

	char* chunk = (char*)malloc(sizeof(buff_t) + length);
	buff_t* buff = (buff_t*)chunk;
	buff->base = chunk + sizeof(buff_t);
	buff->size = length;
	buff->refs = 0;

	if (data)
		memcpy(buff->base, data, length);

	return buff;
}

void Buffer::set_node(node_t* node, buff_t* buff, size_t length)
{
	++buff->refs;
	m_size += node->size;

	node->buff = buff;
	node->data = buff->base;
	node->size = length;
}

void Buffer::free_buff(buff_t* buff)
{
	if (--buff->refs == 0)
		free(buff);
}

void Buffer::free_node(node_t* node)
{
	// ע:�ⲿ���޸�node������curr������
	free_buff(node->buff);
	delete node;
}

void Buffer::push_node(node_t* node)
{
	if (m_head)
	{
		node_t* tail = m_head->prev;
		tail->next = node;
		node->prev = tail;
		node->next = m_head;
		m_head->prev = node;
	}
	else
	{
		node->prev = node;
		node->next = node;
		m_head = node;
		m_curr = node;
		m_offs = 0;
		m_cpos = 0;
	}
}

void Buffer::check_copy(node_t* node)
{
	// Ψһ����,ȫ�������ǲ��ֿ�������
	buff_t* buff = node->buff;
	if (buff->refs < 2)
		return;

	assert(node->size > 0);
	buff_t* temp = new_buff(buff->size, buff->base);
	free_buff(buff);
	set_node(node, temp, node->size);
}

size_t Buffer::calc_size(size_t length)
{
	if (length == 0)
		return 0;

	size_t bytes = m_alloc;
	if (length > m_alloc)
	{
		size_t count = (int)(length / m_alloc);
		bytes = m_alloc * count;
	}

	return bytes;
}

node_t* Buffer::get_node(size_t& ret_offs, size_t& cpos, long length, int origin)
{
	node_t* ret_node;

	ret_node = NULL;
	ret_offs = 0;
	cpos = 0;

	if (m_head == NULL)
		return NULL;

	switch (origin)
	{
	case SEEK_SET: cpos = length; break;
	case SEEK_CUR: cpos = m_cpos + length; break;
	case SEEK_END: cpos = m_size - length; break;
	default:
		assert(false);
	}

	if (cpos > m_size)
		return ret_node;

	if (cpos == m_cpos)
	{
		ret_node = m_curr;
		ret_offs = m_offs;
		return ret_node;
	}

	node_t* tail = m_head->prev;

	if (cpos < m_head->size)
	{// ͷ��
		ret_node = m_head;
		ret_offs = cpos;
	}
	else if (cpos >= (m_size - tail->size))
	{// β��
		ret_node = tail;
		ret_offs = m_size - cpos;
	}
	else
	{// �ӵ�ǰλ�ò���
		if (!m_curr)
		{
			m_curr = m_head;
			m_offs = 0;
			m_cpos = 0;
		}
		// find
		if (m_cpos < cpos)
		{
			// ��ǰ���:---m_cpos---cpos---
			node_t* node = m_curr;
			size_t  offs = m_offs;
			size_t  nums = cpos - m_cpos;
			size_t  count;
			while (nums > 0)
			{
				count = node->size - offs;

				if (count > nums)
				{
					ret_node = node;
					ret_offs = offs + nums;
					break;
				}
				else
				{
					nums -= count;
					node = node->next;
					offs = 0;
				}
			}
		}
		else
		{
			// �Ӻ���ǰ
			node_t* node = m_curr;
			size_t  leng = m_offs;
			size_t  nums = m_cpos - cpos;
			while (nums > 0)
			{
				if (leng > nums)
				{
					// ���
					ret_node = node;
					ret_offs = leng - nums;
					break;
				}
				else
				{
					nums -= leng;
					node = node->prev;
					leng = node->size;
				}
			}
		}
	}

	return ret_node;
}

void Buffer::recv(socket_t sock)
{
	if (m_head == NULL)
		new_node();

	if (m_head == NULL)
		return;

	seek(0, SEEK_END);

	node_t* tail = m_head->prev;
	check_copy(tail);

	char*  buff = tail->data + tail->size;
	size_t size = tail->writable();

	int ret;
	for (;;)
	{
		if (size == 0)
		{
			tail = new_node();
			if (tail == NULL)
				return;
			buff = tail->data + tail->size;
			size = tail->writable();
		}

		// д��
		ret = sock_recv(sock, buff, size);
		if (ret < 0)
			break;

		m_size += ret;
	}

	seek(0, SEEK_END);
}

bool Buffer::send(socket_t sock)
{
	if (m_size == 0)
		return true;

	// �ӵ�ǰ����
	char*  buf;
	size_t len;
	while (m_cpos < m_size)
	{
		buf = m_curr->data + m_offs;
		len = m_curr->size - m_offs;
		int ret = sock_send(sock, buf, len);
		if (ret < 0)
			return false;

		m_cpos += len;
		m_offs += len;

		if (m_offs == m_curr->size)
		{
			m_curr = m_curr->next;
			m_offs = 0;
		}
	}

	discard();
	return true;
}

uint Buffer::find(char data, size_t offset /* = 0 */)
{
	if (!m_head)
		return NPOS;

	size_t offs, cpos;
	node_t* node = get_node(offs, cpos, offset);
	if (node == NULL)
		return NPOS;

	// cpos�����node���
	cpos -= offs;

	do 
	{
		for (size_t i = offs; i < node->size; ++i)
		{
			if (*(node->data + i) = data)
				return (cpos + i);
		}

		cpos += node->size;

		node = node->next;
		offs = 0;

	} while (node != m_head);

	return NPOS;
}

uint Buffer::find(char* data, size_t offset /* = 0 */)
{
	if (!m_head)
		return NPOS;

	size_t offs, cpos;
	node_t* node = get_node(offs, cpos, offset);
	if (node == NULL)
		return NPOS;

	// cpos�����node���
	cpos -= offs;

	size_t src_len = strlen(data);

	do
	{
		for (size_t i = offs; i < node->size; ++i)
		{
			if (_compare(node, i, data, src_len))
				return (cpos + i);
		}

		cpos += node->size;

		node = node->next;
		offs = 0;

	} while (node != m_head);

	return NPOS;
}

uint Buffer::rfind(char data, size_t offset /* = 0 */)
{
	if (!m_head)
		return NPOS;

	size_t offs, cpos;
	node_t* node = get_node(offs, cpos, offset, SEEK_END);
	if (node == NULL)
		return NPOS;

	// �����node���
	cpos -= offs;

	node_t* tail = m_head->prev;
	// �������
	do 
	{
		for (size_t i = offs - 1; i >= 0; --i)
		{
			if (*(node->data + i) == data)
				return (cpos + i);
		}

		node = node->prev;
		offs = node->size;
		cpos -= offs;
		
	} while (node != tail);

	return NPOS;
}

uint Buffer::rfind(char* data, size_t offset /* = 0 */)
{
	if (!m_head)
		return NPOS;

	size_t offs, cpos;
	node_t* node = get_node(offs, cpos, offset, SEEK_END);
	if (node == NULL)
		return NPOS;

	// �����node��ʼλ��
	cpos -= offs;
	node_t* tail = m_head->prev;

	size_t src_len = strlen(data);

	do
	{
		for (size_t i = offs - 1; i >= 0; --i)
		{
			if (_compare(node, i, data, src_len))
				return (cpos + i);
		}

		node = node->prev;
		offs = node->size;
		cpos -= node->size;

	} while (node != tail);

	return NPOS;
}

bool Buffer::_read(node_t* node, size_t offs, void* data, size_t length)
{
	if (node == NULL)
		return false;

	char* src_ptr;
	char* dst_ptr;
	size_t src_len, dst_len;
	size_t count;

	dst_ptr = (char*)data;
	dst_len = length;

	do 
	{
		src_ptr = node->data + offs;
		src_len = node->size - offs;
		count = src_len > dst_len ? dst_len : src_len;

		memcpy(dst_ptr, src_ptr, count);
		dst_ptr += count;
		dst_len -= count;

		node = node->next;

		if (dst_len == 0)
			return true;
		
	} while (node != m_head);

	return false;
}

bool Buffer::_write(node_t* node, size_t offs, const void* data, size_t length)
{
	if (node == NULL)
		return false;

	const char* src_ptr;
	char* dst_ptr;
	size_t src_len, dst_len;
	size_t count;

	src_ptr = (const char*)data;
	src_len = length;

	do
	{
		dst_ptr = node->data + offs;
		dst_len = node->size - offs;
		count = src_len > dst_len ? dst_len : src_len;

		memcpy(dst_ptr, src_ptr, count);
		dst_ptr += count;
		dst_len -= count;

		node = node->next;

		if (src_len == 0)
			return true;

	} while (node != m_head);

	return false;
}

bool Buffer::_compare(node_t* node, size_t offs, const char* data, size_t length)
{
	if (node == NULL)
		return false;

	const char* src_ptr;
	char* dst_ptr;
	size_t src_len, dst_len;
	size_t count;

	src_ptr = data;
	src_len = length;

	do
	{
		dst_ptr = node->data + offs;
		dst_len = node->size - offs;
		count = src_len > dst_len ? dst_len : src_len;

		if (memcmp(dst_ptr, src_ptr, count) != 0)
			return false;

		dst_ptr += count;
		dst_len -= count;

		node = node->next;

		if (src_len == 0)
			return true;

	} while (node != m_head);

	return false;
}

CU_NS_END
