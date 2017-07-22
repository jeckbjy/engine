#pragma once
#include "Cute/Foundation.h"
#include "Cute/Ref.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Buffer : public Ref<Buffer>
{
public:
	Buffer();
	Buffer(size_t len);
	Buffer(const void* data, size_t len);
	~Buffer();

	void swap(Buffer& other);

	void get(void* buf, size_t len, size_t off = 0);
	void put(const void* buf, size_t len, size_t off = 0);
	void append(const void* buf, size_t len);
	void assign(const void* buf, size_t len);

	void resize(size_t newCapacity, bool preserveContent = true);
	void reserve(size_t newCapacity, bool preserveContent = true);

	size_t find(char data, size_t offset = 0);
	size_t find(char* data, size_t offset = 0);
	size_t rfind(char data, size_t offset = 0);
	size_t rfind(char* data, size_t offset = 0);

	void clear();
	bool empty() const;

	size_t size() const;
	size_t capacity() const;

	char* begin();
	char* end();

	const char* begin() const;
	const char* end() const;

	char* data();
	const char* data() const;

	char& at(size_t index);
	const char& at(size_t index) const;

	char& operator[](size_t index);
	const char& operator[](size_t index) const;

	bool operator == (const Buffer& other) const;
	bool operator != (const Buffer& other) const;

protected:
	char*	m_data;
	size_t	m_size;
	size_t	m_capacity;
	bool	m_own;
};

//
// inlines
//
inline bool Buffer::empty() const
{
	return m_size == 0;
}

inline size_t Buffer::size() const
{
	return m_size;
}

inline size_t Buffer::capacity() const
{
	return m_capacity;
}

inline char* Buffer::begin()
{
	return m_data;
}

inline char* Buffer::end()
{
	return m_data + m_size;
}

inline const char* Buffer::begin() const
{
	return m_data;
}

inline const char* Buffer::end() const
{
	assert(m_data != NULL);
	return m_data + m_size;
}

inline char* Buffer::data()
{
	return m_data;
}

inline const char* Buffer::data() const
{
	return m_data;
}

inline char& Buffer::at(size_t index)
{
	assert(index < m_size);
	return m_data[index];
}

inline const char& Buffer::at(size_t index) const
{
	assert(index < m_size);
	return m_data[index];
}

inline char& Buffer::operator [](size_t index)
{
	assert(index < m_size);
	return m_data[index];
}

inline const char& Buffer::operator [](size_t index) const
{
	assert(index < m_size);
	return m_data[index];
}

inline bool Buffer::operator != (const Buffer& other) const
{
	return !(*this == other);
}

CUTE_NS_END