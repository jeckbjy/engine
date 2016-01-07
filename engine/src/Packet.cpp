#include "Packet.h"

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// global function
//////////////////////////////////////////////////////////////////////////
static size_t encode_var(char* buff, uint64_t value)
{
	// 外部确保buff足够，int32最多5位，int64最多10位
	//高位标识：0表示结尾,1表示后边还有数据
	size_t count = 0;
	while (value > 0x7F)
	{
		buff[count++] = ((uint8_t)(value)& 0x7F) | 0x80;
		value >>= 7;
	}
	buff[count++] = value & 0x7F;
	return count;
}

//////////////////////////////////////////////////////////////////////////
// pt_encoder
//////////////////////////////////////////////////////////////////////////
void pt_encoder::encode(pt_message& msg)
{
	// define
	size_t bpos, ipos, epos;
	// clear
	m_tag = 0;
	m_indexs.clear();
	// reserve size
	m_stream->write(NULL, 20);
	// serialize
	bpos = m_stream->position();
	write_msg(msg);
	// write sizes
	ipos = m_stream->position();
	for (TagVec::reverse_iterator itor = m_indexs.rbegin(); itor != m_indexs.rend(); ++itor)
	{
		if (itor->leng > 0)
			write_var(itor->leng);
	}
	epos = m_stream->position();
	// write head
	char buff[20];
	size_t count = 0;
	count += encode_var(buff, ipos - bpos);
	count += encode_var(buff + count, epos - ipos);
	count += encode_var(buff + count, msg.msgid());
	size_t spos = bpos - count;
	m_stream->seek(spos, SEEK_SET);
	m_stream->discard();
	m_stream->write(buff, count);
	m_stream->seek(spos, SEEK_SET);
}

void pt_encoder::write_msg(const pt_message& msg)
{
	size_t tag;
	tag = m_tag;
	m_tag = 0;
	msg.encode(*this);
	m_tag = tag;
}

void pt_encoder::write_buf(const char* buf, size_t len)
{
	m_stream->write(buf, len);
}

void pt_encoder::write_var(uint64_t value)
{
	char buff[10];
	size_t count = encode_var(buff, value);
	m_stream->write(buff, count);
}

void pt_encoder::write_beg(size_t& index, size_t tag)
{
	index = m_indexs.size();
	Tag info;
	info.tpos = m_stream->position();
	write_tag(tag, 0);
	info.bpos = m_stream->position();
	info.leng = 0;
	m_indexs.push_back(info);
}

void pt_encoder::write_end(size_t& index)
{
	Tag info = m_indexs[index];
	size_t epos = m_stream->position();
	size_t leng = epos - info.bpos;
	// modify
	char flag;
	m_stream->seek(info.tpos, SEEK_SET);
	m_stream->peek(&flag, 1);
	flag |= (leng & 0x0F);
	leng >>= 4;
	if (leng > 0)
	{
		flag |= 0x10;
		info.leng = leng;
	}
	m_stream->write(&flag, 1);
	m_stream->seek(epos, SEEK_SET);
}

void pt_encoder::write_tag(size_t tag, uint64_t val)
{
	// 编码规则:注意：数据区使用加减法，可以优化小数据
	// 最高位标识类型：0：uint64，1：带有length的复杂类型
	// 2，3位标识tag,0-2,3标识后边读取,tag至少为1，故可以先减1再保存
	// 0:表示没有数据了，1表示还有数据，低5位标识数据：0-15直接存储,剩余数据紧随或者放到index中
	bool ext = (val == 0);
	char flag = ext ? 0x80 : 0;
	if (tag > 0)
	{
		tag -= 1;
		// tag:占2bit
		if (tag < 3)
		{
			flag |= (char)tag << 5;
			tag = 0;
		}
		else
		{
			flag |= 0x60;
			tag -= 2;
		}
	}
	// val:存储数据的低4位
	flag |= (val & 0x0F);
	val >>= 4;
	if (val > 0)
		flag |= 0x10;
	// flag+tag+val
	m_stream->write(&flag, 1);
	if (tag > 0)
		write_var(tag);
	if (!ext && val > 0)
		write_var(val);
}

bool pt_encoder::write_field(const pt_message& msg, size_t tag)
{
	size_t index;
	write_beg(index, tag);
	write_msg(msg);
	write_end(index);
	return true;
}

bool pt_encoder::write_field(const pt_str& str, size_t tag)
{
	if (str.empty())
		return false;
	size_t index;
	write_beg(index, tag);
	m_stream->write(str.data(), str.size());
	write_end(index);
	return true;
}

void pt_encoder::write_item(const pt_str& str)
{
	write_var(str.size());
	if (!str.empty())
		m_stream->write(str.data(), str.size());
}

void pt_encoder::write_item(const pt_message& msg)
{
	size_t index;
	write_beg(index, 0);
	write_msg(msg);
	write_end(index);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
pt_decoder::pt_decoder(Buffer* stream)
	: m_stream(stream)
	, m_tag(0)
{
	m_epos = m_stream->size();
}

bool pt_decoder::parse()
{
	bool result = false;
	uint64_t temp;
	m_stream->seek(0, SEEK_SET);
	do 
	{
		// temp
		if (!read_var(temp))
			break;
		m_body_len = (uint)temp;
		if (!read_var(temp))
			break;
		m_tail_len = (uint)temp;
		m_head_len = m_stream->position();
		size_t len = m_head_len + m_body_len + m_tail_len;
		result = len <= m_stream->size();
	} while(true);
	m_stream->seek(0, SEEK_SET);
	return result;
}

bool pt_decoder::decode(pt_message& msg)
{
	// head_len,body_len,tail_len:must be right
	// define
	uint64_t temp;
	size_t length, spos, epos;
	length = m_head_len + m_body_len + m_tail_len;
	spos = m_stream->position();
	epos = m_stream->position() + length;
	bool result = true;
	// clear
	m_tag = 0;
	m_epos = m_stream->size();
	m_sizes.clear();
	// read size
	// ignore head
	m_stream->seek(m_head_len, SEEK_CUR);
	// read all sizes
	if (m_tail_len > 0)
	{
		m_stream->seek(m_body_len, SEEK_CUR);
		while (m_stream->position() < epos)
		{
			if (!read_var(temp))
			{
				result = false;
				break;
			}
			m_sizes.push_back((size_t)temp);
		}
		m_stream->seek(spos + m_head_len, SEEK_SET);
	}
	// read body
	if (result && m_body_len > 0)
	{
		size_t tpos = suspend(m_body_len);
		read_msg(msg);
		recovery(tpos);
	}
	// seek to right pos
	m_stream->seek(epos, SEEK_SET);

	return result;
}

Buffer pt_decoder::split()
{
	Buffer buf;
	size_t len = msglen();
	if (len > 0)
		m_stream->front(buf, len);
	return buf;
}

size_t pt_decoder::suspend(size_t len)
{
	size_t epos = m_epos;
	m_epos = m_stream->position() + len;
	assert(epos <= m_epos);
	return epos;
}

void pt_decoder::recovery(size_t epos)
{
	assert(epos >= m_stream->position());
	m_epos = epos;
}

bool pt_decoder::eof() const
{
	return m_stream->position() >= m_epos;
}

bool pt_decoder::read_buf(char* buf, size_t len)
{
	if (m_stream->position() + len > m_epos)
		return false;
	return m_stream->read(buf, len);
}

bool pt_decoder::read_var(uint64_t& data)
{
	data = 0;
	char off = 0;
	char tmp;
	do
	{
		if (off >= 64)
			return false;
		if (!this->read_buf(&tmp, 1))
			return false;
		data |= (uint64_t)(tmp & 0x7F) << off;
		off += 7;
	} while (tmp & 0x80);

	return true;
}

void pt_decoder::read_msg(pt_message& msg)
{
	size_t tag;
	tag = m_tag;
	m_tag = 0;
	msg.decode(*this);
	m_tag = tag;
}

bool pt_decoder::pre_read(size_t tag)
{
	if (!eof())
		return false;

	if (m_tag == 0)
	{
		if (!read_tag(true))
			return false;
	}
	// 读取并忽略无效数据
	while (tag > m_tag)
	{
		// skip length content
		if (!read_tag(true))
			return false;
	}
	// 如果不存在则使用默认数据
	m_tag -= tag;
	if (m_tag != 0)
		m_data = 0;
	return true;
}

bool pt_decoder::read_tag(bool use_tag)
{
	uint64_t temp;

	char flag;
	if (!read_buf(&flag, 1))
		return false;
	// parse type
	m_type = (flag & 0x80) != 0 ? T_MSG : T_VAL;
	// parse tags
	if (use_tag)
	{
		size_t tag = flag & 0x60;
		if (tag == 3)
		{
			if (!read_var(temp))
				return false;
			tag += (size_t)temp + 2;
		}
		tag += 1;
		m_tag += tag;
	}
	// parse data
	m_data = flag & 0x0F;
	if (flag & 0x10)
	{// 还有额外数据:普通数值则紧随读取，否则从index中读取
		if (m_type == T_VAL)
		{
			if (!read_var(temp))
				return false;
		}
		else
		{
			if (m_sizes.empty())
				return false;
			temp = m_sizes.back();
			m_sizes.pop_back();
		}
		m_data = (temp << 4) | m_data;
	}
	return true;
}

void pt_decoder::read_field(pt_str& str)
{
	if (m_type != T_MSG)
		return;
	str.clear();
	size_t len = (size_t)m_data;
	if (len)
	{
		str.resize(len);
		read_buf(&str[0], len);
	}
}

void pt_decoder::read_field(pt_message& msg)
{
	if (m_type != T_MSG || !m_data)
		return;
	size_t epos;
	epos = suspend((size_t)m_data);
	read_msg(msg);
	recovery(epos);
}

void pt_decoder::read_item(pt_str& str)
{
	uint64_t temp;
	if (!read_var(temp))
		return;
	str.clear();
	if (temp > 0)
	{
		str.resize((size_t)temp);
		read_buf(&str[0], (size_t)temp);
	}
}

void pt_decoder::read_item(pt_message& msg)
{
	read_tag(false);
	if (m_type != T_MSG || !m_data)
		return;
	size_t epos;
	epos = suspend((size_t)m_data);
	read_msg(msg);
	recovery(epos);
}

CU_NS_END
