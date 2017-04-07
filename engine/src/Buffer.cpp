//! Memory
#include "Cute/Buffer.h"
#include "Cute/Exception.h"
#include <algorithm>

CUTE_NS_BEGIN

Buffer::Buffer(size_t len)
	: m_data(0)
	, m_size(0)
	, m_capacity(0)
	, m_own(true)
{
	reserve(len, false);
}

Buffer::Buffer(const void* data, size_t leng)
	: m_data((char*)data)
	, m_size(leng)
	, m_capacity(leng)
	, m_own(false)
{
}

Buffer::~Buffer()
{
	if (m_data != NULL && m_own)
	{
		free(m_data);
		m_data = NULL;
	}
}

void Buffer::swap(Buffer& other)
{
	std::swap(m_data, other.m_data);
	std::swap(m_size, other.m_size);
	std::swap(m_capacity, other.m_capacity);
	std::swap(m_own, other.m_own);
}

void Buffer::get(void* buf, size_t len, size_t off /* = 0 */)
{
	assert(off + len <= m_size);
	std::memcpy(buf, m_data + off, len);
}

void Buffer::put(const void* buf, size_t len, size_t off /* = 0 */)
{
	assert(off + len <= m_size);
	std::memcpy(m_data + off, buf, len);
}

void Buffer::append(const void* buf, size_t len)
{
	// Á½±¶Ôö³¤
	size_t newSize = m_size + len;
	if (newSize > m_capacity)
	{
		size_t newCapacity = std::max(newSize, m_size * 2);
		reserve(newCapacity, true);
	}

	std::memcpy(m_data + m_size, buf, len);
	m_size = newSize;
}

void Buffer::assign(const void* buf, size_t len)
{
	resize(len);
	std::memcpy(m_data, buf, len);
}

void Buffer::resize(size_t newSize, bool preserveContent /* = true */)
{
	reserve(newSize, preserveContent);
	m_size = newSize;
}

void Buffer::reserve(size_t newCapacity, bool preserveContent /* = true */)
{
	if (!m_own)
		throw InvalidAccessException("Cannot resize buffer which does not own its storage.");

	if (newCapacity > m_capacity)
	{
		char* ptr = (char*)malloc(newCapacity);
		if (m_data != 0)
		{
			if (preserveContent)
				std::memcpy(ptr, m_data, m_size);
			free(m_data);
		}
		m_data = ptr;
		m_capacity = newCapacity;
	}
}

size_t Buffer::find(char data, size_t offset /* = 0 */)
{
	assert(offset <= m_size);
	for (size_t i = offset; i < m_size; ++i)
	{
		if (m_data[i] = data)
			return i;
	}

	return NPOS;
}

size_t Buffer::find(char* data, size_t offset /* = 0 */)
{
	assert(offset <= m_size);
	for (size_t i = offset; i < m_size; ++i)
	{
		if (strcmp(m_data + i, data) == 0)
			return i;
	}

	return NPOS;
}

size_t Buffer::rfind(char data, size_t offset /* = 0 */)
{
	assert(offset <= m_size);
	for (size_t i = m_size - offset; i >= 0; --i)
	{
		if (m_data[i] = data)
			return i;
	}

	return NPOS;
}

size_t Buffer::rfind(char* data, size_t offset /* = 0 */)
{
	assert(offset <= m_size);
	for (size_t i = m_size - offset; i >= 0; --i)
	{
		if (strcmp(m_data + i, data) == 0)
			return i;
	}

	return NPOS;
}

void Buffer::clear()
{
	if (m_own)
	{
		m_size = 0;
	}
	else
	{
		m_data = 0;
		m_size = 0;
		m_capacity = 0;
		m_own = true;
	}
}

bool Buffer::operator ==(const Buffer& other) const
{
	if (this != &other)
	{
		if (m_size == other.m_size)
		{
			if (std::memcmp(m_data, other.m_data, m_size) == 0)
				return true;
		}

		return false;
	}

	return true;
}

CUTE_NS_END