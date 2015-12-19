#include "Buffer.h"

CU_NS_BEGIN

Buffer::Buffer(size_t bytes /* = 0 */)
{

}

Buffer::~Buffer()
{

}

bool Buffer::peek(void* buf, size_t len) const
{
	return true;
}

bool Buffer::read(void* buf, size_t len) const
{
	return true;
}

bool Buffer::write(const void* buf, size_t len)
{
	return true;
}

bool Buffer::write(const Buffer & buf)
{
	// merge
	return false;
}

bool Buffer::append(const void* buf, size_t len)
{
	if (m_cpos != m_epos)
		seek(m_epos, SEEK_SET);
	return write(buf, len);
}

void Buffer::seek(long len, int origin /* = SEEK_BEG */)
{

}

bool Buffer::advance(size_t len)
{
	return true;
}

bool Buffer::reserve(size_t len)
{
	return true;
}

void Buffer::resize(size_t len)
{

}

void Buffer::discard()
{

}

void Buffer::concat()
{

}

CU_NS_END
