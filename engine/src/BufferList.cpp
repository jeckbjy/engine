//! Memory
#include "Cute/BufferList.h"
#include "Cute/SocketDefs.h"

CUTE_NS_BEGIN

struct BufferList::buff_t
{
	size_t refs;	// 引用计数
	size_t size;	// 内存大小
	char*  base;	// 数据基址
};

struct BufferList::node_t
{
	node_t* prev;
	node_t* next;
	buff_t* buff;
	char*	data;
	size_t	size;
	size_t	writable() const { return buff->size - size - (data - buff->base); }
};

BufferList::BufferList(size_t alloc)
	: m_head(0)
	, m_curr(0)
	, m_offs(0)
	, m_cpos(0)
	, m_size(0)
	, m_alloc(alloc)
{

}

BufferList::BufferList(const BufferList& other)
	: m_head(0)
	, m_curr(0)
	, m_offs(0)
	, m_cpos(0)
	, m_size(0)
	, m_alloc(other.m_alloc)
{
	merge(other);
}

BufferList::~BufferList()
{
	release();
}

BufferList& BufferList::operator =(const BufferList& other)
{
	release();
	merge(other);
	return *this;
}

void BufferList::merge(const BufferList& other)
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

void BufferList::split(BufferList& other)
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

bool BufferList::write(const void* buf, size_t len)
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
		put(m_curr, m_offs, buf, len);

	seek(len, SEEK_CUR);

	return true;
}

bool BufferList::read(void* buf, size_t len)
{
	if (get(m_curr, m_offs, buf, len))
	{
		seek(len, SEEK_CUR);
		return true;
	}

	return false;
}

bool BufferList::peek(void* buf, size_t len)
{
	if (m_cpos + len > m_size)
		return false;

	return get(m_curr, m_offs, buf, len);
	return true;
}

void BufferList::seek(long len, int origin /* = SEEK_CUR */)
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

void BufferList::insert(size_t length)
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

bool BufferList::erase(size_t length)
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

void BufferList::release()
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

void BufferList::discard()
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

void BufferList::compact()
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

void BufferList::concat()
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

bool BufferList::recv(socket_t sock)
{
	if (m_head == NULL)
		new_node(m_alloc);

	// 分配内存失败
	if (m_head == NULL)
		return false;

	seek(0, SEEK_END);

	node_t* tail = m_head->prev;
	check_copy(tail);

	char*  buff = tail->data + tail->size;
	size_t size = tail->writable();

	bool result = true;
	int ret;
	for (;;)
	{
		if (size == 0)
		{
			tail = new_node(m_alloc);
			if (tail == NULL)
				return false;

			buff = tail->data + tail->size;
			size = tail->writable();
		}

		// 写入
		ret = sock_recv(sock, buff, size);
		if (ret < 0)
		{
			result = false;
			break;
		}

		m_size += ret;
	}

	seek(0, SEEK_END);

	return result;
}

bool BufferList::send(socket_t sock)
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

size_t BufferList::find(char data, size_t offset /* = 0 */)
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

size_t BufferList::find(char* data, size_t offset /* = 0 */)
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
			if (compare(node, i, data, src_len))
				return (cpos + i);
		}

		cpos += node->size;

		node = node->next;
		offs = 0;

	} while (node != m_head);

	return NPOS;
}

size_t BufferList::rfind(char data, size_t offset /* = 0 */)
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

size_t BufferList::rfind(char* data, size_t offset /* = 0 */)
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
			if (compare(node, i, data, src_len))
				return (cpos + i);
		}

		node = node->prev;
		offs = node->size;
		cpos -= node->size;

	} while (node != tail);

	return NPOS;
}

bool BufferList::expand(size_t len)
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

bool BufferList::get(node_t* node, size_t offs, void* data, size_t length) const
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

bool BufferList::put(node_t* node, size_t offs, const void* data, size_t length)
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

bool BufferList::compare(node_t* node, size_t offs, const char* data, size_t length)
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

BufferList::buff_t* BufferList::new_buff(size_t length, const char* data)
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

BufferList::node_t* BufferList::new_node(buff_t* buff, size_t node_size, char* data)
{
	if (!buff)
		return NULL;

	node_t* node = new node_t;

	set_node(node, buff, node_size, data);
	push_node(node);

	return node;
}

BufferList::node_t* BufferList::new_node(size_t buff_size, size_t node_size)
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

void BufferList::set_node(node_t* node, buff_t* buff, size_t node_size, char* data)
{
	++buff->refs;

	node->buff = buff;
	node->data = data ? data : buff->base;
	node->size = node_size;
}

void BufferList::free_buff(buff_t* buff)
{
	if (--buff->refs == 0)
		free(buff);
}

void BufferList::free_node(node_t* node)
{
	// 注:外部会修改node索引和curr等数据
	free_buff(node->buff);
	delete node;
}

void BufferList::push_node(node_t* node)
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

void BufferList::check_copy(node_t* node)
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

size_t BufferList::calc_size(size_t length)
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

BufferList::node_t* BufferList::get_node(size_t& ret_offs, size_t& cpos, long length, int origin)
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

String BufferList::toString() const
{
	String str;
	if (m_size > 0)
	{
		str.resize(m_size);
		get(m_head, 0, &str[0], m_size);
	}

	return str;
}

String BufferList::toString(size_t length) const
{
	String str;
	if (m_size > 0)
	{
		if (length > m_size)
			length = m_size;
		str.resize(length);
		get(m_head, 0, &str[0], length);
	}

	return str;
}

bool BufferList::read7Bit(uint64_t& data)
{
	data = 0;
	char off = 0;
	char tmp;
	do
	{
		if (off >= 64)
			return false;
		if (!read(tmp))
			return false;
		data |= ((uint64_t)(tmp & 0x7F)) << off;
		off += 7;
	} while (tmp & 0x80);

	return true;
}

uint BufferList::write7Bit(uint64_t data)
{
	char buff[10];
	//高位标识：0表示结尾,1表示后边还有数据
	size_t count = 0;
	while (data > 0x7F)
	{
		buff[count++] = ((uint8_t)(data)& 0x7F) | 0x80;
		data >>= 7;
	}
	buff[count++] = data & 0x7F;

	write(buff, count);
	return count;
}

bool BufferList::read8Bit(uint64_t& data, size_t count)
{
	// 通过个数读取int,低字节放在前边,高字节在后边
	data = 0;
	if (count == 0)
		return true;

	char buff[8];
	if (!read(buff, count))
		return false;

	size_t off = 0;
	for (size_t i = 0; i < count; ++i)
	{
		data |= ((uint64_t(buff[i])) << off);
		off += 8;
	}

	return true;
}

uint BufferList::write8Bit(uint64_t data)
{
	if (data == 0)
		return 0;

	char buff[8];
	uint count = 0;
	while (data > 0)
	{
		buff[count++] = (char)(data & 0xFF);
		data >>= 8;
	}

	write(buff, count);
	return count;
}

CUTE_NS_END
