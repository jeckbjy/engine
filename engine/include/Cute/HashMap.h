//! Collection
#pragma once
#include "Cute/HashTable.h"
#include <utility>

CUTE_NS_BEGIN

/// This class implements a map using a LinearHashTable.
///
/// A HashMap can be used just like a std::map.
template <class Key, class Value, class HashFunc = std::hash<Key> >
class HashMap
{
private:
	struct Entry
	{
		Key		first;
		Value	second;
		Entry() :first(), second(){}
		Entry(const Key& key) :first(key), second(){}
		Entry(const Key& key, const Value& value) :first(key), second(value){}

		bool operator == (const Entry& entry) const
		{
			return first == entry.first;
		}

		bool operator != (const Entry& entry) const
		{
			return first != entry.first;
		}
	};

	struct EntryHash
	{
		std::size_t operator () (const Entry& entry) const
		{
			return m_func(entry.first);
		}

	private:
		HashFunc m_func;
	};

public:
	typedef Key                 KeyType;
	typedef Value               ValueType;
	typedef Value&				Reference;
	typedef const Value&        ConstReference;
	typedef Value*              Pointer;
	typedef const Value*		ConstPointer;

	typedef HashTable<Entry, EntryHash> Table;
	typedef Table::Iterator				Iterator;
	typedef Table::ConstIterator		ConstIterator;

	HashMap()
	{
	}

	HashMap(std::size_t initialReserve)
		: m_table(initialReserve)
	{
	}

	HashMap& operator = (const HashMap& map)
	{
		HashMap tmp(map);
		swap(tmp);
		return *this;
	}

	void swap(HashMap& map)
	{
		m_table.swap(map.m_table);
	}

	ConstIterator begin() const
	{
		return m_table.begin();
	}

	ConstIterator end() const
	{
		return m_table.end();
	}

	Iterator begin()
	{
		return m_table.begin();
	}

	Iterator end()
	{
		return m_table.end();
	}

	ConstIterator find(const KeyType& key) const
	{
		ValueType value(key);
		return m_table.find(value);
	}

	Iterator find(const KeyType& key)
	{
		ValueType value(key);
		return m_table.find(value);
	}

	std::size_t count(const KeyType& key) const
	{
		ValueType value(key);
		return m_table.find(value) != m_table.end() ? 1 : 0;
	}

	std::pair<Iterator, bool> insert(const PairType& pair)
	{
		ValueType value(pair.first, pair.second);
		return m_table.insert(value);
	}

	std::pair<Iterator, bool> insert(const ValueType& value)
	{
		return m_table.insert(value);
	}

	void erase(Iterator it)
	{
		m_table.erase(it);
	}

	void erase(const KeyType& key)
	{
		Iterator it = find(key);
		m_table.erase(it);
	}

	void clear()
	{
		m_table.clear();
	}

	std::size_t size() const
	{
		return m_table.size();
	}

	bool empty() const
	{
		return m_table.empty();
	}

	ConstReference operator [] (const KeyType& key) const
	{
		ConstIterator it = m_table.find(key);
		if (it != m_table.end())
			return it->second;
		else
			throw NotFoundException();
	}

	Reference operator [] (const KeyType& key)
	{
		ValueType value(key);
		std::pair<Iterator, bool> res = m_table.insert(value);
		return res.first->second;
	}

private:
	Table m_table;
};

CUTE_NS_END
