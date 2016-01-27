#include "Stream.h"

CU_NS_BEGIN

char* Stream::readLine(char* str, int num)
{
	if (num <= 0)
		return NULL;
	char c = 0;
	size_t maxCharsToRead = num - 1;
	for (size_t i = 0; i < maxCharsToRead; ++i)
	{
		size_t result = read(&c, 1, 1);
		if (result != 1)
		{
			str[i] = '\0';
			break;
		}
		if (c == '\n')
		{
			str[i] = c;
			str[i + 1] = '\0';
			break;
		}
		else if (c == '\r')
		{
			str[i] = c;
			// next may be '\n'
			size_t pos = position();

			char nextChar = 0;
			if (read(&nextChar, 1, 1) != 1)
			{
				// no more characters
				str[i + 1] = '\0';
				break;
			}
			if (nextChar == '\n')
			{
				if (i == maxCharsToRead - 1)
				{
					str[i + 1] = '\0';
					break;
				}
				else
				{
					str[i + 1] = nextChar;
					str[i + 2] = '\0';
					break;
				}
			}
			else
			{
				seek(pos, SEEK_SET);
				str[i + 1] = '\0';
				break;
			}
		}
		str[i] = c;
	}
	return str; // what if first read failed?
}

void Stream::skipWhiteSpace()
{
	char c;
	do
	{
		if (read(c) != 1)
		{// 没有数据了
			return;
		}
	} while (isspace(c));
	if (!seek(-1, SEEK_CUR))
		throw std::runtime_error("Failed to seek backwards one character after skipping whitespace.");
}

void Stream::skipLine(uint len)
{
	char c;
	while (len > 0)
	{
		do
		{
			if (read(c) != 1)
				return;
		} while (c != '\n');
		--len;
	}
}

bool Stream::readMagic(int magic, size_t num /* = 4 */)
{
	assert(num <= 4);
	size_t pos = position();
	char buf[4] = { 0 };
	if (read(buf, num) == num)
	{
		// 判断是否相同
		if (memcmp(&magic, buf, num) == 0)
			return true;
	}
	// 读取失败，返回
	seek(pos, SEEK_SET);
	return false;
}

uint Stream::readVariantInt()
{
	return 0;
}

void Stream::writeVariantInt(uint data)
{
	// 写入
}

uint Stream::read(String& data)
{
	return 0;
}

uint Stream::write(const String& data)
{
	return 0;
}

CU_NS_END
