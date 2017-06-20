#pragma once
#include "Cute/Foundation.h"
#include "Cute/ByteOrder.h"

CUTE_NS_BEGIN

// 内存不连续，共享内存,写实拷贝
// SEEK_CUR SEEK_SET SEEK_END
class CUTE_CORE_API BufferList
{
	struct buff_t;
	struct node_t;
public:
	BufferList(size_t alloc = 1024);
	BufferList(const BufferList& other);
	~BufferList();

	BufferList& operator = (const BufferList& other);

	void merge(const BufferList& other);
	// 从当前位置分拆成两个,并放到other结尾
	void split(BufferList& other);

	bool write(const void* data, size_t len);
	bool read(void* data, size_t len);
	bool peek(void* data, size_t len);
	void seek(long length, int origin);

	void insert(size_t length);
	bool erase(size_t length);

	void release();
	void discard();
	void compact();
	void concat();

	// socket
	bool recv(socket_t sock);
	bool send(socket_t sock);

	size_t find(char data, size_t offset = 0);
	size_t find(char* data, size_t offset = 0);
	size_t rfind(char data, size_t offset = 0);
	size_t rfind(char* data, size_t offset = 0);

	bool eof() const;
	bool empty() const;
	size_t size() const;
	size_t position() const;

	String toString() const;
	String toString(size_t length) const;

	bool read7Bit(uint64_t& data);
	uint write7Bit(uint64_t data);
	bool read8Bit(uint64_t& data, size_t count);
	uint write8Bit(uint64_t data);

	template<typename T>
	bool read7Bit(T& data)
	{
		uint64 temp;
		if (!read7Bit(temp))
			return false;

		if (temp > (uint64)std::numeric_limits<T>::max())
			return false;

		data = (T)temp;
		return true;
	}

	template<typename T>
	bool read(T& data)
	{
		return read(&data, sizeof(data));
	}

	template<typename T>
	bool write(T& data)
	{
		return write(&data, sizeof(data));
	}

	template<typename T>
	bool readBig(T& data)
	{
		if (!read(&data, sizeof(data)))
			return false;
		data = ByteOrder::fromBigEndian(data);
		return true;
	}

	template<typename T>
	bool writeBig(T& data)
	{
		T t = ByteOrder::toBigEndian(data);
		return write(&data, sizeof(data));
	}

	template<typename T>
	bool readLittle(T& data)
	{
		if (!read(&data, sizeof(data)))
			return false;
		data = ByteOrder::fromLittleEndian(data);
		return true;
	}

	template<typename T>
	bool writeLittle(T& data)
	{
		T t = ByteOrder::toLittleEndian(data);
		return write(&data, sizeof(data));
	}

private:
	size_t  calc_size(size_t length);
	node_t* get_node(size_t& offs, size_t& cpos, long length, int origin = SEEK_SET);
	buff_t* new_buff(size_t length, const char* data = NULL);
	node_t* new_node(buff_t* buff, size_t node_size = 0, char* data = NULL);
	node_t* new_node(size_t buff_size, size_t node_size = 0);
	void set_node(node_t* node, buff_t* buff, size_t node_size = 0, char* data = NULL);
	void free_buff(buff_t* buff);
	void free_node(node_t* node);
	void push_node(node_t* node);
	void check_copy(node_t* node);

	bool expand(size_t length);
	bool get(node_t* node, size_t offs, void* data, size_t length) const;
	bool put(node_t* node, size_t offs, const void* data, size_t length);
	bool compare(node_t* node, size_t offs, const char* data, size_t length);

private:
	node_t* m_head;		// 链表头
	node_t* m_curr;		// 当前位置
	size_t	m_offs;		// 当前偏移
	size_t	m_cpos;		// 当前位置,相对于起点
	size_t	m_size;		// 总大小
	size_t	m_alloc;	// 分配内存大小,0表示内存连续,2倍分配
};

//
// inline
//
inline bool BufferList::eof() const
{
	return m_cpos >= m_size;
}

inline bool BufferList::empty() const
{
	return m_size == 0;
}

inline size_t BufferList::size() const
{
	return m_size;
}

inline size_t BufferList::position() const
{
	return m_cpos;
}

CUTE_NS_END
