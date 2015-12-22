#include "Buffer.h"

CU_NS_BEGIN

Buffer::itor_t::itor_t(node_t* node, size_t offs, size_t leng)
:node(node), leng(leng), index(0)
{
	// 要求leng一定足够，否则会异常
	if (node->leng > offs)
	{
		data = node->data + offs;
		size = node->leng - offs;
	}
	else
	{
		node = node->next;
		data = node->data;
		size = node->leng;
	}
	if (size > leng)
		size = leng;
}

void Buffer::itor_t::next()
{
	assert(leng > 0);
	index += size;
	leng -= size;
	node = node->next;
	if (size > leng)
		size = leng;
}

Buffer::Buffer(size_t bytes /* = 0 */)
: m_head(0)
, m_curr(0)
, m_offs(0)
, m_cpos(0)
, m_size(0)
, m_alloc(bytes)
{

}

Buffer::~Buffer()
{
	while (m_head)
		destroy(m_head);
}

void Buffer::slice(Buffer& buf, size_t off, size_t len)
{
	assert(off + len < m_size);
	seek(off, SEEK_SET);
	node_t* node = m_curr;
	size_t  offs = m_offs;
	while (len > 0)
	{
		size_t count = node->leng - offs;
		if (count > 0)
		{
			if (count > len)
				count = len;
			node_t* temp = new node_t;
			temp->buff = node->buff;
			temp->data = node->data + offs;
			temp->leng = count;
			buf.push_back(temp);
			len -= count;
			offs = 0;
		}
		node = node->next;
		assert(node != m_head);
	}

	buf.m_curr = buf.m_head;
	buf.m_offs = 0;
	buf.m_cpos = 0;
	buf.m_size = len;
}

bool Buffer::append(const Buffer & buf)
{
	// 追加到尾部
	if (buf.empty())
		return true;
	node_t* head = buf.m_head;
	node_t* node = head;
	while (node)
	{
		if (node->leng > 0)
		{
			node_t* temp = new node_t;
			temp->buff = node->buff;
			temp->data = node->data;
			temp->leng = node->leng;
			push_back(temp);
		}
		node = node->next;
		if (node == head)
			break;
	}
	return true;
}

bool Buffer::peek(void* buf, size_t len)
{
	if (m_cpos + len > m_size)
		return false;
	char* ptr = (char*)buf;

	itor_t itor(m_curr, m_offs, len);
	while (itor.has_data())
	{
		memcpy(ptr + itor.index, itor.data, itor.size);
		itor.next();
	}

	return true;
}

bool Buffer::read(void* buf, size_t len)
{
	if (peek(buf, len))
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

	if (buf == 0)
	{// 无数据时，仅扩容和移动游标
		seek(len, SEEK_CUR);
	}
	else
	{// 写入数据
		const char* ptr = (const char*)buf;
		itor_t itor(m_curr, m_offs, len);
		while (itor.has_data())
		{
			memcpy(itor.data, ptr + itor.index, itor.size);
			itor.next();
		}
		// 修改游标
		m_curr = itor.node;
		m_offs = itor.data - m_curr->data;
		m_cpos += len;
	}

	return true;
}

bool Buffer::append(const void* buf, size_t len)
{
	if (len == 0)
		return true;
	// seek 到尾部
	if (m_cpos != m_size)
	{
		seek(0, SEEK_END);
	}
	return write(buf, len);
}

bool Buffer::expand(size_t len)
{
	// 扩容但并不修改当前游标
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
		tail = alloc(len);
	}

	while (tail && len > 0)
	{
		// 计算可用空间
		size_t count = tail->buff->leng - tail->leng;
		if (count > 0)
		{
			if (count > len)
				count = len;
			tail->leng += count;
			len -= count;
			m_size += count;
		}
		tail = alloc(len);
	}

	return len == 0;
}

void Buffer::seek(long len, int origin /* = SEEK_BEG */)
{
	size_t cpos;
	switch (origin)
	{
	case SEEK_SET: cpos = len; break;
	case SEEK_CUR: cpos = m_cpos + len; break;
	case SEEK_END: cpos = m_size - len; break;
	default:
		assert(false);
	}
	if (cpos > m_size || cpos == m_cpos)
		return;
	// 计算
	if (origin == SEEK_CUR && m_curr)
	{
		seek(m_curr, m_offs, len);
	}
	else
	{
		size_t halfs = m_size >> 2;
		if (cpos < halfs)
			seek(m_head, 0, cpos);
		else
			seek(m_head->prev, m_head->prev->leng, (long)cpos - (long)m_size);
	}
}

void Buffer::seek(node_t* node, size_t offs, long len)
{
	if (len > 0)
	{
		// 从前向后
		itor_t itor(node, offs, len);
		while (itor.has_data())
			itor.next();
		m_curr = itor.node;
		m_offs = itor.offset();
	}
	else
	{
		// 从后向前
		long size = (long)offs;
		do 
		{
			len -= size;
			node = node->next;
			size = (long)node->leng;
		} while (len > 0);
		m_curr = node;
		m_offs = (size_t)(-len);
	}
}

void Buffer::discard()
{
	// 删除0-cpos之间的数据
	if (m_cpos == 0)
		return;
	m_size -= m_cpos;
	while (m_cpos > 0 && m_head)
	{
		if (m_head->leng > m_cpos)
		{// 删除数据
			m_head->data += m_cpos;
			m_cpos = 0;
		}
		else
		{// 删除node
			m_cpos -= m_head->leng;
			destroy(m_head);
		}
	}
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
	memcpy(base, m_head->data, m_head->leng);
	m_head->data = base;
}

void Buffer::concat()
{
	// 只有一个，无需合并
	if (!m_head || m_head->prev == m_head)
		return;
	buff_t* buff = new buff_t;
	buff->refs = 1;
	buff->base = (char*)malloc(m_size);
	buff->leng = m_size;
	seek(0, SEEK_SET);
	peek(buff->base, m_size);
	while (m_head)
	{
		destroy(m_head);
	}
	node_t* node = new node_t;
	node->prev = node;
	node->next = node;
	node->buff = buff;
	node->data = buff->base;
	node->leng = buff->leng;
	m_head = node;
	m_curr = node;
	m_offs = m_cpos;
}

Buffer::node_t* Buffer::alloc(size_t len)
{
	if (len == 0)
		return 0;
	len = (std::max)(len, m_alloc);
	buff_t* buff = (buff_t*)malloc(sizeof(buff_t));
	if (buff == 0)
		return 0;
	char* data = (char*)malloc(len);
	if (data == 0)
	{
		free(buff);
		return 0;
	}
	buff->base = data;
	buff->leng = len;
	buff->refs = 0;

	// 创建节点
	node_t* node = new node_t;
	node->buff = buff;
	node->data = buff->base;
	node->leng = 0;

	push_back(node);
	return node;
}

void Buffer::destroy(node_t* node)
{
	buff_t* buff = node->buff;
	if (--buff->refs == 0)
	{
		free(buff->base);
		free(buff);
	}
	// 释放node
	if (node->next == node)
	{// 只有1个了
		m_head = 0;
	}
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		if (m_head == node)
			m_head = node->next;
	}

	if (node == m_curr)
	{
		m_curr = 0;
	}
	// 释放
	delete node;
}

void Buffer::push_back(node_t* node)
{
	++node->buff->refs;
	m_size += node->leng;
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
	}
}

void Buffer::check_copy(node_t* node)
{
	// 唯一持有,全拷贝还是部分拷贝？？
	buff_t* buff = node->buff;
	if (buff->refs < 2)
		return;
	assert(node->leng > 0);
	buff_t* temp = new buff_t;
	temp->refs = 1;
	temp->leng = node->leng;
	temp->base = (char*)malloc(node->leng);
	memcpy(temp->base, node->data, node->leng);
	--buff->refs;
	node->buff = temp;
	node->data = temp->base;
}

CU_NS_END
