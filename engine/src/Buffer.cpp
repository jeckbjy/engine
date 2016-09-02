#include "Buffer.h"

CU_NS_BEGIN

// 底层内存块
struct buff_t
{
	size_t refs;	// 引用计数
	size_t size;	// 内存大小
	char*  base;	// 数据基址
};

// 如果是写buf则
struct node_t
{
	node_t* prev;
	node_t* next;
	buff_t* buff;	// 实际内存
	char*	data;	// 数据起始
	size_t	size;	// 数据长度

	size_t	writable() const { return buff->size - size - (data - buff->base); }
};

// 会自动过滤0数据
//struct itor_t
//{
//	node_t* node;	// 当前节点
//	char*	data;	// 当前可用数据
//	size_t	leng;	// 当前可用长度
//	size_t	nums;	// 剩余数据长度
//	size_t	index;	// 累计总偏移
//	itor_t(node_t* node, size_t offs, size_t nums)
//		: node(node)
//		, nums(nums)
//		, index(0)
//	{
//		assert(offs <= node->size);
//		// 忽略结尾，直接跳掉下一个
//		if (node->size == offs)
//		{
//			node = node->next;
//			data = node->data;
//			leng = node->size;
//		}
//		else
//		{// 正常偏移
//			data = node->data + offs;
//			leng = node->size - offs;
//		}
//		
//		// 太长截断
//		if (leng > nums)
//			leng = nums;
//	}
//
//	void next()
//	{
//		// 向前移动
//		nums  -= leng;
//		data  += leng;
//		index += leng;
//		// 更新节点
//		if (nums != 0)
//		{
//			node = node->next;
//			data = node->data;
//			leng = node->size;
//			if (leng > nums)
//				leng = nums;
//		}
//	}
//	bool eof() const { return nums == 0; }
//	size_t offset() const { return data - node->data; }
//};

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

	// 拷贝
	if (head)
	{
		node_t* node = head;
		node_t* temp;
		do
		{
			temp = new_node(node->buff, node->size, node->data);
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

	// 分拆成两个
	node_t* node = m_head;
	do 
	{
		if (node == m_curr)
		{
			other.new_node(node->buff, m_offs, node->data);

			// 恢复链表
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
	{// 扩充容量
		if (!expand(epos - m_size))
			return false;
	}

	if (buf != 0)
		_write(m_curr, m_offs, buf, len);

	seek(len, SEEK_CUR);

	return true;
}

void Buffer::insert(size_t length)
{
	if (m_curr == NULL)
		return;

	size_t canWriteSize = m_curr->writable();

	if (length < canWriteSize)
	{
		check_copy(m_curr);
		m_curr->size += length;
	}
	else
	{
		// 重新创建个，并拷贝
		buff_t* buff = new_buff(calc_size(m_curr->size + length));

		// 剩余数据
		char*  ptr = m_curr->data + m_offs;
		size_t len = m_curr->size - m_offs;

		memcpy(buff->base, m_curr->data, m_offs);
		memcpy(buff->base + m_offs + length, ptr, len);

		free_buff(m_curr->buff);
		set_node(m_curr, buff, m_curr->size + length);
	}

	m_size += length;

	//// 填充数据
	//if (data)
	//	memcpy(ptr, data, length);
}

bool Buffer::erase(size_t length)
{
	if (length == 0)
		return true;

	if (m_size - m_cpos < length)
		return false;

	node_t* curr;
	size_t curr_data;
	do 
	{
		check_copy(m_curr);
		curr_data = m_curr->size - m_offs;

		if (curr_data > length)
		{
			char* data = m_curr->data + m_offs;
			memcpy(data, data + length, curr_data - length);
			m_curr->size -= length;
			length = 0;
		}
		else
		{
			m_curr->size -= length;
			length -= curr_data;
		}

		curr = m_curr;

		if (length > 0 || m_curr->size == 0)
		{
			m_curr = m_curr->next;
			m_offs = 0;
		}
		
		// 删除为零的
		if (curr->size == 0)
		{
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			free_node(curr);
		}

	} while (m_curr != m_head && length != 0);

	m_size -= length;

	return true;
}

bool Buffer::expand(size_t len)
{
	// 扩容但并不修改当前游标
	if (len == 0)
		return true;

	if (!m_head)
	{
		new_node(len, len);
		return m_size == len;
	}

	node_t* tail = m_head->prev;
	size_t count = tail->writable();

	if (count > len)
	{
		check_copy(tail);
		tail->size += len;
		m_size += len;
		return true;
	}

	// 需要重新分配内存
	if (m_alloc == 0)
	{
		// 全部重新分配realloc
		buff_t* buff = new_buff(m_size + len);
		memcpy(buff->base, tail->data, tail->size);
		free_buff(tail->buff);
		set_node(tail, buff, tail->size);
	}
	else
	{
		check_copy(tail);

		do
		{
			count = tail->writable();

			if (count > 0)
			{
				if (count > len)
					count = len;
				tail->size += count;
				len -= count;
				m_size += count;
			}

			if (len > 0)
			{
				tail = new_node(len);
				if (tail == NULL)
					return false;
			}

		} while (len > 0);
	}
	
	return true;
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
		// 断开链
		m_head->prev->next = 0;
		// 释放
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
	// 删除0-cpos之间的数据
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

	// 修正数据
	assert(m_cpos == 0);
	m_head->prev = tail;
	tail->prev = m_head;
	m_curr = m_head;
	m_offs = 0;
}

void Buffer::compact()
{
	// 压缩，将数据移动到buff头部
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
	// 只有一个，无需合并
	if (!m_head || m_head->prev == m_head)
		return;

	size_t size = m_size;
	size_t cpos = m_cpos;

	// 创建并拷贝到一起
	buff_t* buff = new_buff(calc_size(size));
	seek(0, SEEK_SET);
	peek(buff->base, size);

	release();
	
	new_node(buff, m_size);
	m_offs = cpos;
	m_cpos = cpos;
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

node_t* Buffer::new_node(buff_t* buff, size_t node_size, char* data)
{
	if (!buff)
		return NULL;

	node_t* node = new node_t;

	set_node(node, buff, node_size, data);
	push_node(node);

	return node;
}

node_t* Buffer::new_node(size_t buff_size, size_t node_size)
{
	if (buff_size == 0)
		buff_size = m_alloc;

	buff_t* buff = new_buff(calc_size(buff_size), NULL);
	if (!buff)
		return NULL;

	node_t* node = new node_t;

	set_node(node, buff, node_size, buff->base);
	push_node(node);

	return node;
}

void Buffer::set_node(node_t* node, buff_t* buff, size_t node_size, char* data)
{
	++buff->refs;

	node->buff = buff;
	node->data = data ? data : buff->base;
	node->size = node_size;
}

void Buffer::free_buff(buff_t* buff)
{
	if (--buff->refs == 0)
		free(buff);
}

void Buffer::free_node(node_t* node)
{
	// 注:外部会修改node索引和curr等数据
	free_buff(node->buff);
	delete node;
}

void Buffer::push_node(node_t* node)
{
	m_size += node->size;

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
	// 唯一持有,全拷贝还是部分拷贝？？
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
	size_t alloc = m_alloc ? m_alloc : 1024;

	if (length == 0)
		return alloc;

	if (m_alloc == 0)
	{
		return length * 2;
	}
	else
	{
		size_t bytes = m_alloc;
		if (length > m_alloc)
		{
			size_t count = (int)(length / m_alloc);
			bytes = m_alloc * count;
		}

		return bytes;
	}
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

	if (cpos <= m_head->size)
	{// 头部
		ret_node = m_head;
		ret_offs = cpos;
	}
	else if (cpos >= (m_size - tail->size))
	{// 尾部
		ret_node = tail;
		ret_offs = m_size - cpos;
	}
	else
	{// 从当前位置查找
		if (!m_curr)
		{
			m_curr = m_head;
			m_offs = 0;
			m_cpos = 0;
		}
		// find
		if (m_cpos < cpos)
		{
			// 从前向后:---m_cpos---cpos---
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
			// 从后向前
			node_t* node = m_curr;
			size_t  leng = m_offs;
			size_t  nums = m_cpos - cpos;
			while (nums > 0)
			{
				if (leng > nums)
				{
					// 结果
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
		new_node(m_alloc);

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
			tail = new_node(m_alloc);
			if (tail == NULL)
				return;
			buff = tail->data + tail->size;
			size = tail->writable();
		}

		// 写入
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

	// 从当前发送
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

	// cpos相对于node起点
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

	// cpos相对于node起点
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

	// 相对于node起点
	cpos -= offs;

	node_t* tail = m_head->prev;
	// 逆向遍历
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

	// 相对于node起始位置
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

bool Buffer::_read(node_t* node, size_t offs, void* data, size_t length) const
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

		src_ptr += count;
		src_len -= count;

		if (src_len == 0)
			return true;

		node = node->next;
		offs = 0;

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

String Buffer::toString() const
{
	if (m_size == 0)
		return STR_EMPTY;
	String str;
	str.resize(m_size);
	_read(m_head, 0, &str[0], m_size);
	return str;
}

CU_NS_END
