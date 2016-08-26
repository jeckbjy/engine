#include "Packet.h"

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// global function
//////////////////////////////////////////////////////////////////////////
static size_t encode_var(char* buff, uint64_t value)
{
	//外部确保buff足够，int32最多5位，int64最多10位
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

static size_t decode_var(uint64_t& value, char* buff, size_t len, size_t start)
{
	value = 0;
	char off = 0;
	char tmp;
	char* ptr = buff + start;
	char* end = buff + len;
	do
	{
		if (off >= 64)
			return 0;

		if (ptr >= end)
			return 0;

		tmp = *ptr;
		value |= (uint64_t)(tmp & 0x7F) << off;
		off += 7;
		++ptr;
	} while (tmp & 0x80);

	return off;
}

static size_t encode_tag(char* buff, size_t tag, uint64_t val, bool ext)
{
	// 头编码:head+body: type(1bit)|tag(2bit)|data(5bit) + [tag]+[data]
	// ext(1bit): 0:uint64,1:length+body
	// tag(2bit): 3:表示还有后边还有数据
	// val(5bit): 存储数据的低4位，高位0标识结束，1标识还有数据

	// encode head
	char flag = ext ? 0x80 : 0;

	// tag
	assert(tag > 0);
	tag -= 1;
	if (tag < 3)
	{
		flag |= (char)(tag << 5);
		tag = 0;
	}
	else
	{
		flag |= 0x60;
		tag -= 2;
	}
	// val:
	flag |= (val & 0x0F);
	val >>= 4;
	if (val > 0)
		flag |= 0x10;

	// encode body
	size_t length = 1;
	buff[0] = flag;

	if (tag > 0)
		length += encode_var(buff + length, tag);

	if (val > 0)
		length += encode_var(buff + length, val);

	return length;
}

//////////////////////////////////////////////////////////////////////////
// encoder
//////////////////////////////////////////////////////////////////////////
pt_encoder::pt_encoder(pt_stream* stream)
	: m_tag(0)
	, m_stream(stream)
{
}

void pt_encoder::encode(pt_msg& msg)
{
	// 先预留空间
	size_t spos;
	spos = m_stream->position();
	m_stream->write(NULL, 32);

	// 序列化
	write_msg(msg);

	// 计算head:flag(1byte)+msgid+length
	size_t leng = m_stream->position() - spos - 20;

	// 写入head并删除多余字节
	char buff[32];
	size_t count = 1;
	count += encode_var(buff + count, leng);
	count += encode_var(buff + count, msg.msgid());
	if (msg.uid > 0)
		count += encode_var(buff + count, msg.uid);

	buff[0] = count;
	// offset
	m_stream->write(buff, count);
}

void pt_encoder::write_msg(const pt_msg& msg)
{
	size_t tag;
	tag = m_tag;
	m_tag = 0;
	msg.encode(*this);
	m_tag = tag;
}

void pt_encoder::write_var(uint64_t value)
{
	char buff[10];
	size_t count = encode_var(buff, value);
	m_stream->write(buff, count);
}

void pt_encoder::write_buf(const char* buf, size_t len)
{
	m_stream->write(buf, len);
}

void pt_encoder::write_tag(size_t tag, uint64_t val, bool ext)
{
	char data[20];
	size_t size = encode_tag(data, tag, val, ext);
	m_stream->write(data, size);
}

void pt_encoder::write_beg(size_t& spos, size_t tag)
{
	spos = m_stream->position();
	m_stream->seek(2, SEEK_CUR);
}

bool pt_encoder::write_end(size_t& spos, size_t tag)
{
	size_t epos = m_stream->position();
	size_t leng = epos - spos - 2;
	if (leng == 0)
	{
		m_stream->seek(-2, SEEK_CUR);
		return false;
	}
	else
	{
		char	data[20];
		size_t	size;
		if (tag != 0)
			size = encode_tag(data, tag, leng, true);
		else
			size = encode_var(data, leng);

		if (size == 1)
			m_stream->erase(spos + 1, 1);
		else if (size > 2)
			m_stream->insert(spos + 2, size - 2);

		m_stream->put(data, size, spos);

		return true;
	}
}

bool pt_encoder::write_field(const pt_msg& msg, size_t tag)
{
	size_t spos;
	write_beg(spos, tag);
	write_msg(msg);
	return write_end(spos, tag);
}

bool pt_encoder::write_field(const pt_str& str, size_t tag)
{
	if (str.empty())
		return false;
	size_t len = str.size();
	write_tag(tag, len, true);
	write_buf(str.data(), len);
	return true;
}

void pt_encoder::write_item(const pt_msg& msg)
{
	size_t spos;
	write_beg(spos, 0);
	write_msg(msg);
	write_end(spos, 0);
}

void pt_encoder::write_item(const pt_str& str)
{
	size_t size = str.size();
	write_var(size);
	if (size != 0)
		write_buf(str.data(), size);
}

//////////////////////////////////////////////////////////////////////////
// decoder
//////////////////////////////////////////////////////////////////////////
pt_decoder::pt_decoder(pt_stream* stream)
	: m_stream(stream)
	, m_tag(0)
{
	m_epos = m_stream->size();
}

bool pt_decoder::parse()
{
	char buff[32];
	if (!m_stream->peek(buff, 1))
		return false;

	char count = buff[0] + 1;

	if (count > 32)
		return false;

	if (!m_stream->peek(buff, count))
		return false;

	uint64_t tmp;
	size_t offset = 1;
	offset += decode_var(tmp, buff, count, offset);
	m_msglen = (size_t)tmp;
	offset += decode_var(tmp, buff, count, offset);
	m_msgid = (size_t)tmp;
	offset += decode_var(tmp, buff, count, offset);
	m_uid = tmp;

	m_head = count;
	return true;
}

bool pt_decoder::decode(pt_msg& msg)
{
	// 先忽略head长度
	m_stream->seek(m_head, SEEK_CUR);
	read_msg(msg);
	return false;
}

bool pt_decoder::eof() const
{
	return m_stream->position() >= m_epos;
}

void pt_decoder::suspend(size_t& epos, size_t len)
{
	epos = m_epos;
	m_epos = m_stream->position() + len;
	assert(epos <= m_epos);
}

void pt_decoder::recovery(size_t epos)
{
	assert(epos >= m_stream->position());
	m_epos = epos;
}

bool pt_decoder::pre_read(size_t tag)
{
	if (eof())
		return false;

	if (m_tag == 0 && !read_tag())
		return false;

	// 读取并忽略无效数据
	while (tag > m_tag)
	{
		// skip content
		if (!read_tag())
			return false;
	}

	// 如果不存在则使用默认数据
	m_tag -= tag;
	if (m_tag != 0)
		m_val = 0;

	return true;
}

bool pt_decoder::read_tag()
{
	char flag;
	size_t	 tag;
	uint64_t tmp;
	if (!read_buf(&flag, 1))
		return false;

	m_ext = (flag & 0x80) != 0;

	// parse tag
	tag = flag & 0x60;
	if (tag == 3)
	{
		if (!read_var(tmp))
			return false;
		tag += (size_t)tmp;
		tag += 2;
	}
	tag += 1;
	m_tag += tag;

	// parse value
	m_val = (flag & 0x0F);
	if (flag & 0x10)
	{
		if (!read_var(tmp))
			return false;
		m_val |= (tmp << 4);
	}

	return true;
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

void pt_decoder::read_msg(pt_msg& msg)
{
	if (m_val == 0)
		return;

	size_t tag, epos;

	suspend(epos, (size_t)m_val);

	tag = m_tag;
	m_tag = 0;
	msg.decode(*this);
	m_tag = tag;

	recovery(epos);
}

void pt_decoder::read_field(pt_msg& msg)
{
	if (!m_ext)
		return;

	read_msg(msg);
}

void pt_decoder::read_field(pt_str& str)
{
	if (!m_ext || !m_val)
		return;

	size_t len = (size_t)m_val;

	str.clear();
	str.resize(len);
	read_buf(&str[0], len);
}

void pt_decoder::read_item(pt_msg& msg)
{
	if (!read_var(m_val))
		return;

	read_msg(msg);
}

CU_NS_END
