//! Stream
#include "Cute/Stream.h"

CUTE_NS_BEGIN

Stream::Stream()
{

}

Stream::~Stream()
{

}

bool Stream::readLine(String& data)
{
	if (!canRead())
		return false;

	char c;
	char buffer[1024];
	long offset = 0;
	do 
	{
		if (!read(c))
			break;

		buffer[offset++] = c;
		if (offset == 1024)
		{
			data.append(buffer, offset);
			offset = 0;
		}

	} while (c != '\n');

	if (offset != 0)
	{
		data.append(buffer, offset);
		offset = 0;
	}

	// 忽略'\r'
	if (!data.empty() && data.back() == '\r')
	{
		data.erase(data.size() - 1);
	}

	return true;
}

bool Stream::skipLine()
{
	if (!canRead())
		return false;

	char c;
	do
	{
		if (!read(c))
			break;

	} while (c != '\n');

	return true;
}

bool Stream::read7Bit(uint64& data)
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
		data |= (uint64)(tmp & 0x7F) << off;
		off += 7;
	} while (tmp & 0x80);

	return true;
}

uint Stream::write7Bit(uint64 value)
{
	char buffer[10];
	//外部确保buff足够，int32最多5位，int64最多10位
	//高位标识：0表示结尾,1表示后边还有数据
	size_t count = 0;
	while (value > 0x7F)
	{
		buffer[count++] = ((uint8_t)(value)& 0x7F) | 0x80;
		value >>= 7;
	}
	buffer[count++] = value & 0x7F;
	write(buffer, count);
	return count;
}

CUTE_NS_END
