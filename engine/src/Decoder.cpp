//! Server
#include "Cute/Decoder.h"
#include "Cute/Stream.h"

CUTE_NS_BEGIN

Decoder::Decoder(Stream* stream)
	: m_stream(stream)
	, m_spos(stream->position())
	, m_epos(stream->size())
	, m_hint(0)
	, m_tag(0)
	, m_value(0)
	, m_extra(false)
{
}

Decoder::Decoder(Stream* stream, size_t length)
	: m_stream(stream)
	, m_spos(stream->position())
	, m_epos(m_spos + length)
	, m_hint(0)
	, m_tag(0)
	, m_value(0)
	, m_extra(false)
{
}

Decoder::~Decoder()
{
}

bool Decoder::decode(Message* msg)
{
	msg->decode(*this);
	return true;
}

uint64 Decoder::read7Bit()
{
	uint64 value;
	if (read7Bit(value))
		return value;

	return 0;
}

bool Decoder::eof() const
{
	return m_stream->position() >= m_epos;
}

bool Decoder::readTag()
{
	char	flag;
	bool	extra;
	size_t	tagOff = 0;
	uint64	value = 0;
	uint64	temp = 0;

	if (!readData(&flag, 1))
		return false;

	// parse extra
	extra = (flag & 0x80) != 0;

	// parse tag
	tagOff = flag & 0x60;
	if (tagOff == 3)
	{
		if (!read7Bit(temp))
			return false;
		tagOff += (size_t)temp;
		tagOff += 2;
	}

	tagOff += 1;

	// parse value
	value = (flag & 0x0F);
	if (flag & 0x10)
	{
		if (!read7Bit(temp))
			return false;
		value |= (temp << 4);
	}

	// set
	m_tag += tagOff;
	m_value = value;
	m_extra = extra;
	return true;
}

bool Decoder::read7Bit(uint64& data)
{
	data = 0;
	char off = 0;
	char tmp;
	do
	{
		if (off >= 64)
			return false;
		if (!this->readData(&tmp, 1))
			return false;
		data |= (uint64_t)(tmp & 0x7F) << off;
		off += 7;
	} while (tmp & 0x80);

	return true;
}

bool Decoder::readData(char* buffer, size_t length)
{
	if (m_stream->position() + length > m_epos)
		return false;

	return m_stream->read(buffer, length);
}

bool Decoder::readHead(size_t tag)
{
	while (tag < m_tag)
	{
		// skip
		if (m_extra && m_value > 0)
		{
			m_stream->seek((long)m_value, SEEK_CUR);
		}

		if (!readTag())
			return false;
	}

	return m_tag == tag;
}

void Decoder::readField(Message& msg)
{
	if (m_extra == false)
		return;

	Decoder decoder(m_stream, (size_t)m_value);
	msg.decode(decoder);
}

void Decoder::readField(String& str)
{
	if (m_extra == false)
		return;
	str.clear();
	str.resize((size_t)m_value);
	readData(&str[0], (size_t)m_value);
}

void Decoder::readField(int8& data)
{
	data = (int8)decodei64(m_value);
}

void Decoder::readField(int16& data)
{
	data = (int16)decodei64(m_value);
}

void Decoder::readField(int32& data)
{
	data = (int32)decodei64(m_value);
}

void Decoder::readField(int64& data)
{
	data = (int64)decodei64(m_value);
}

void Decoder::readField(uint8& data)
{
	data = (uint8)m_value;
}

void Decoder::readField(uint16& data)
{
	data = (uint16)m_value;
}

void Decoder::readField(uint32& data)
{
	data = (uint32)m_value;
}

void Decoder::readField(uint64& data)
{
	data = (uint64)m_value;
}

void Decoder::readField(float& data)
{
	data = decodef32(m_value);
}

void Decoder::readField(double& data)
{
	data = decodef64(m_value);
}

void Decoder::readItem(Message& data)
{
	size_t size = (size_t)read7Bit();
	size_t epos = m_stream->position() + size;
	Decoder decoder(m_stream, size);
	data.decode(decoder);
	m_stream->seek(epos, SEEK_SET);
}

void Decoder::readItem(String& data)
{
	size_t size = (size_t)read7Bit();
	data.clear();
	data.resize(size);
	readData(&data[0], size);
}

void Decoder::readItem(int8& data)
{
	data = (int8)decodei64(read7Bit());
}

void Decoder::readItem(int16& data)
{
	data = (int16)decodei64(read7Bit());
}

void Decoder::readItem(int32& data)
{
	data = (int32)decodei64(read7Bit());
}

void Decoder::readItem(int64& data)
{
	data = (int64)decodei64(read7Bit());
}

void Decoder::readItem(uint8& data)
{
	data = (uint8)read7Bit();
}

void Decoder::readItem(uint16& data)
{
	data = (uint16)read7Bit();
}

void Decoder::readItem(uint32& data)
{
	data = (uint32)read7Bit();
}

void Decoder::readItem(uint64& data)
{
	data = (uint64)read7Bit();
}

void Decoder::readItem(float& data)
{
	data = decodef32(read7Bit());
}

void Decoder::readItem(double& data)
{
	data = decodef64(read7Bit());
}

CUTE_NS_END
