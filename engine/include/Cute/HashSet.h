//! Collection
#pragma once
#include "Cute/Foundation.h"
#include "Cute/HashTable.h"
#include <utility>

CUTE_NS_BEGIN

template <class Value, class HashFunc = std::hash<Value> >
class HashSet
	/// This class implements a set using a LinearHashTable.
	///
	/// A HashSet can be used just like a std::set.
{
public:
	typedef Value        ValueType;
	typedef Value&       Reference;
	typedef const Value& ConstReference;
	typedef Value*       Pointer;
	typedef const Value* ConstPointer;
	typedef HashFunc     Hash;

	typedef HashTable<ValueType, Hash>		Table;
	typedef typename Table::Iterator		Iterator;
	typedef typename Table::ConstIterator	ConstIterator;

	HashSet()
	{
	}

	HashSet(std::size_t initialReserve)
		: m_table(initialReserve)
	{
	}

	HashSet(const HashSet& set)
		: m_table(set.m_table)
	{
	}

	~HashSet()
	{
	}

	HashSet& operator = (const HashSet& table)
	{
		HashSet tmp(table);
		swap(tmp);
		return *this;
	}

	void swap(HashSet& set)
	{
		m_table.swap(set.m_table);
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

	ConstIterator find(const ValueType& value) const
	{
		return m_table.find(value);
	}

	Iterator find(const ValueType& value)
	{
		return m_table.find(value);
	}

	std::size_t count(const ValueType& value) const
	{
		return m_table.count(value);
	}

	std::pair<Iterator, bool> insert(const ValueType& value)
	{
		return m_table.insert(value);
	}

	void erase(Iterator it)
	{
		m_table.erase(it);
	}

	void erase(const ValueType& value)
	{
		m_table.erase(value);
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

private:
	Table m_table;
};

CUTE_NS_END
