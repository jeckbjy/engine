#include "Buffer.h"

CU_NS_BEGIN

Buffer::itor_t::itor_t(node_t* node, size_t offs, size_t nums)
:node(node), nums(nums), index(0)
{
	assert(offs <= node->leng);
	// 忽略结尾，直接跳掉下一个
	if (node->leng == offs)
	{
		node = node->next;
		data = node->data;
		leng = node->leng;
	}
	else
	{// 正常偏移
		data = node->data + offs;
		leng = node->leng - offs;
	}

	// 太长截断
	if (leng > nums)
		leng = nums;
}

void Buffer::itor_t::next()
{
	// 向前移动
	nums  -= leng;
	data  += leng;
	index += leng;
	// 更新节点
	if (nums != 0)
	{
		node = node->next;
		data = node->data;
		leng = node->leng;
		if (leng > nums)
			leng = nums;
	}
}

void Buffer::itor_t::to_end()
{
	while (!eof())
		next();
}

Buffer::Buffer(size_t bytes)
: m_head(0)
, m_curr(0)
, m_offs(0)
, m_cpos(0)
, m_size(0)
, m_alloc(bytes)
{
	assert(bytes > 0);
}

Buffer::~Buffer()
{
	release();
}

void Buffer::front(Buffer& buf, size_t len)
{
	// buf必须是空buf,用于传出
	assert(buf.empty());
	node_t* temp;
	node_t* node;
	itor_t itor(m_head, 0, len);
	while (!itor.eof())
	{
		node = itor.node;
		temp = new node_t();
		temp->buff = node->buff;
		temp->data = itor.data;
		temp->leng = itor.leng;
		buf.push_back(temp);
		itor.next();
	}
	buf.seek(0, SEEK_SET);
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
	while (!itor.eof())
	{
		memcpy(ptr + itor.index, itor.data, itor.leng);
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
		while (!itor.eof())
		{
			memcpy(itor.data, ptr + itor.index, itor.leng);
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

//void Buffer::advance(size_t len)
//{
//	// 扩展
//	node_t* tail = m_head->prev;
//	assert(len <= tail->writable() && tail->buff->refs < 2);
//	tail->leng += len;
//}

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

	if (cpos == 0 || cpos < m_head->leng)
	{// 头部
		m_curr = m_head;
		m_offs = cpos;
	}
	else if (cpos == m_size)
	{// 忽略末尾无效的数据段,通常应该没有
		node_t* tail = m_head->prev;
		while (tail->leng == 0)
			tail = tail->prev;
		m_curr = tail;
		m_offs = tail->leng;
	}
	else
	{// 非头非尾
		if (!m_curr)
		{
			m_curr = m_head;
			m_offs = 0;
			m_cpos = 0;
		}
		// find
		if (m_cpos < cpos)
		{
			// 从前向后
			itor_t itor(m_curr, m_offs, cpos - m_cpos);
			while (!itor.eof())
				itor.next();
			m_curr = itor.node;
			m_offs = itor.offset();
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
					leng -= nums;
					nums = 0;
				}
				else
				{
					nums -= leng;
					node = node->prev;
					leng = node->leng;
				}
			}
			// 设置
			m_curr = node;
			m_offs = leng;
		}
	}

	m_cpos = cpos;
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
			destroy(temp);
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
		if (m_head->leng > m_cpos)
		{
			m_head->data += m_cpos;
			m_cpos = 0;
		}
		else
		{
			temp = m_head;
			m_cpos -= m_head->leng;
			m_head = m_head->next;
			destroy(temp);
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
	node_t* node = new node_t;
	node->prev = node;
	node->next = node;
	node->buff = buff;
	node->data = buff->base;
	node->leng = buff->leng;

	release();
	m_head = node;
	m_curr = node;
	m_offs = m_cpos;
}

uint Buffer::find(char ch, size_t off /* = 0 */)
{
	if (!m_head)
		return npos;
	itor_t pre_itor(m_head, 0, off);
	pre_itor.to_end();
	itor_t itor(pre_itor.node, pre_itor.offset(), m_size - off);
	while (!itor.eof())
	{
		char* ptr = itor.data;
		char* end = itor.data + itor.leng;
		while (ptr != end)
		{
			if (*ptr == ch)
				return off + itor.index + (ptr - itor.data);
			++ptr;
		}
		itor.next();
	}

	return npos;
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
	// 注:外部会修改node索引和curr等数据
	buff_t* buff = node->buff;
	if (--buff->refs == 0)
	{
		free(buff->base);
		free(buff);
	}

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

void Buffer::get_free(char*& data, uint& leng)
{
	node_t* tail = m_head ? m_head->prev : 0;
	if (tail)
	{
		check_copy(tail);
		leng = tail->buff->leng - tail->leng - (tail->data - tail->buff->base);
		data = tail->data + tail->leng;
		if (leng > 0)
			return;
	}
	// 需要创建新的
	tail = alloc(m_alloc);
	data = tail->buff->base;
	leng = tail->buff->leng;
}

String Buffer::toString()
{
	if (empty())
		return "";
	size_t len = size();
	String str;
	str.resize(len);
	write(&str[0], len);
	return str;
}

CU_NS_END
