#pragma once
#include "SortedVector.h"

CU_NS_BEGIN

template<typename KEY, typename VAL>
class KeyedVector
{
public:
	typedef KEY key_type;
	typedef VAL value_type;

	KeyedVector();
	~KeyedVector();

	void resize(size_t count);
	void reserve(size_t count);

	void reverse();
	void clear() { m_vector.clear(); }
	bool empty() const { return m_vector.empty(); }

	size_t size() const { return m_vector.size(); }
	size_t capacity() const { return m_vector.capacity(); }

	const VAL& valueAt(size_t index) const;
	const KEY& keyAt(size_t index) const;

	KEY& keyAt(size_t index);
	VAL& valueAt(size_t index);

	ssize_t indexOfKey(const KEY& key) const;
	ssize_t indexOfValue(const VAL& val) const;

	ssize_t add(const KEY& key, const VAL& item);
	ssize_t remove(const KEY& key);
	ssize_t removeAt(size_t index, size_t count = 1);
	ssize_t replace(const KEY& key, const VAL& item);
	ssize_t replace(size_t index, const VAL& item);

private:
	// 使用指针减少结构体拷贝???
	struct kv_pair
	{
		KEY key;
		VAL val;
		inline bool operator < (const kv_pair& o) const
		{
			return key < o.key;
		}
	};

	SortedVector<kv_pair> m_vector;
};

CU_NS_END