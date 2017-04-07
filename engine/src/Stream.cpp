//! Stream
#include "Cute/Stream.h"

CUTE_NS_BEGIN

Stream::Stream()
{

}

Stream::~Stream()
{

}

void Stream::rewind()
{
	seek(0, SEEK_SET);
}

bool Stream::eof() const
{
	return position() >= length();
}

bool Stream::readLine(String& data)
{
	return true;
}

bool Stream::skipLine()
{
	char c;
	do 
	{
		if (!read(c))
			return false;
	} while (c != '\n');

	return true;
}

CUTE_NS_END