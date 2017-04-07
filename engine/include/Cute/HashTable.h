//! Collection
#pragma once
#include "Cute/Foundation.h"
#include <vector>
#include <utility>
#include <cstddef>
#include <algorithm>

CUTE_NS_BEGIN

/// This class implements a linear hash table.
///
/// In a linear hash table, the available address space
/// grows or shrinks dynamically. A linear hash table thus
/// supports any number of insertions or deletions without
/// lookup or insertion performance deterioration.
///
/// Linear hashing was discovered by Witold Litwin in 1980
/// and described in the paper LINEAR HASHING: A NEW TOOL FOR FILE AND TABLE ADDRESSING.
///
/// For more information on linear hashing, see <http://en.wikipedia.org/wiki/Linear_hash>.
///
/// The HashTable is not thread safe.
///
/// Value must support comparison for equality.
///
/// Find, insert and delete operations are basically O(1) with regard
/// to the total number of elements in the table, and O(N) with regard
/// to the number of elements in the bucket where the element is stored.
/// On average, every bucket stores one element; the exact number depends
/// on the quality of the hash function. In most cases, the maximum number of
/// elements in a bucket should not exceed 3.
template<class Value, class HashFunc = std::hash<Value> >
class HashTable
{
public:
	typedef Value               ValueType;
	typedef Value&              Reference;
	typedef const Value&        ConstReference;
	typedef Value*              Pointer;
	typedef const Value*        ConstPointer;
	typedef HashFunc            Hash;
	typedef std::vector<Value>  Bucket;
	typedef std::vector<Bucket> BucketVec;
	typedef typename Bucket::iterator    BucketIterator;
	typedef typename BucketVec::iterator BucketVecIterator;

	class ConstIterator : public std::iterator<std::forward_iterator_tag, Value>
	{
	public:
		ConstIterator() : _initialized(false)
		{
		}

		ConstIterator(const BucketVecIterator& vecIt, const BucketVecIterator& endIt, const BucketIterator& buckIt) :
			_vecIt(vecIt),
			_endIt(endIt),
			_buckIt(buckIt),
			_initialized(true)
		{
		}

		ConstIterator(const ConstIterator& it) :
			_vecIt(it._vecIt),
			_endIt(it._endIt),
			_buckIt(it._buckIt),
			_initialized(it._initialized)

		{
		}

		ConstIterator& operator = (const ConstIterator& it)
		{
			ConstIterator tmp(it);
			swap(tmp);
			return *this;
		}

		void swap(ConstIterator& it)
		{
			using std::swap;
			// uninitialized iterators crash when swapped
			if (_initialized)
			{
				swap(_vecIt, it._vecIt);
				swap(_endIt, it._endIt);
				swap(_buckIt, it._buckIt);
				swap(_initialized, it._initialized);
			}
			else
			{
				_vecIt = it._vecIt;
				_endIt = it._endIt;
				_buckIt = it._buckIt;
				_initialized = it._initialized;
			}
		}

		bool operator == (const ConstIterator& it) const
		{
			return _vecIt == it._vecIt && (_vecIt == _endIt || _buckIt == it._buckIt);
		}

		bool operator != (const ConstIterator& it) const
		{
			return _vecIt != it._vecIt || (_vecIt != _endIt && _buckIt != it._buckIt);
		}

		const typename Bucket::value_type& operator * () const
		{
			return *_buckIt;
		}

		const typename Bucket::value_type* operator -> () const
		{
			return &*_buckIt;
		}

		ConstIterator& operator ++ () // prefix
		{
			if (_vecIt != _endIt)
			{
				++_buckIt;
				while (_vecIt != _endIt && _buckIt == _vecIt->end())
				{
					++_vecIt;
					if (_vecIt != _endIt) _buckIt = _vecIt->begin();
				}
			}
			return *this;
		}

		ConstIterator operator ++ (int) // postfix
		{
			ConstIterator tmp(*this);
			++*this;
			return tmp;
		}

	protected:
		BucketVecIterator _vecIt;
		BucketVecIterator _endIt;
		BucketIterator    _buckIt;
		bool              _initialized;

		friend class HashTable;
	};

	class Iterator : public ConstIterator
	{
	public:
		Iterator()
		{
		}

		Iterator(const BucketVecIterator& vecIt, const BucketVecIterator& endIt, const BucketIterator& buckIt) :
			ConstIterator(vecIt, endIt, buckIt)
		{
		}

		Iterator(const Iterator& it) :
			ConstIterator(it)
		{
		}

		Iterator& operator = (const Iterator& it)
		{
			Iterator tmp(it);
			ConstIterator::swap(tmp);
			return *this;
		}

		void swap(Iterator& it)
		{
			ConstIterator::swap(it);
		}

		typename Bucket::value_type& operator * ()
		{
			return *this->_buckIt;
		}

		const typename Bucket::value_type& operator * () const
		{
			return *this->_buckIt;
		}

		typename Bucket::value_type* operator -> ()
		{
			return &*this->_buckIt;
		}

		const typename Bucket::value_type* operator -> () const
		{
			return &*this->_buckIt;
		}

		Iterator& operator ++ () // prefix
		{
			ConstIterator::operator ++ ();
			return *this;
		}

		Iterator operator ++ (int) // postfix
		{
			Iterator tmp(*this);
			++*this;
			return tmp;
		}

		friend class HashTable;
	};

	HashTable(std::size_t initialReserve = 64) :
		m_split(0),
		m_front(1),
		m_size(0)
	{
		m_buckets.reserve(calcSize(initialReserve));
		m_buckets.push_back(Bucket());
	}

	HashTable(const HashTable& table) :
		m_buckets(table.m_buckets),
		m_split(table.m_split),
		m_front(table.m_front),
		m_size(table.m_size)
	{
	}

	~HashTable()
	{
	}

	HashTable& operator = (const HashTable& table)
	{
		HashTable tmp(table);
		swap(tmp);
		return *this;
	}

	void swap(HashTable& table)
	{
		using std::swap;
		swap(m_buckets, table.m_buckets);
		swap(m_split, table.m_split);
		swap(m_front, table.m_front);
		swap(m_size, table.m_size);
	}

	ConstIterator begin() const
	{
		BucketVecIterator it(m_buckets.begin());
		BucketVecIterator itEnd(m_buckets.end());
		while (it != itEnd && it->empty())
		{
			++it;
		}
		if (it == itEnd)
			return end();
		else
			return ConstIterator(it, itEnd, it->begin());
	}

	ConstIterator end() const
	{
		return ConstIterator(m_buckets.end(), m_buckets.end(), m_buckets.front().end());
	}

	Iterator begin()
	{
		BucketVecIterator it(m_buckets.begin());
		BucketVecIterator itEnd(m_buckets.end());
		while (it != itEnd && it->empty())
		{
			++it;
		}
		if (it == itEnd)
			return end();
		else
			return Iterator(it, itEnd, it->begin());
	}

	Iterator end()
	{
		return Iterator(m_buckets.end(), m_buckets.end(), m_buckets.front().end());
	}

	ConstIterator find(const Value& value) const
	{
		std::size_t addr = bucketAddress(value);
		BucketVecIterator it(m_buckets.begin() + addr);
		BucketIterator buckIt(std::find(it->begin(), it->end(), value));
		if (buckIt != it->end())
			return ConstIterator(it, m_buckets.end(), buckIt);
		else
			return end();
	}

	Iterator find(const Value& value)
	{
		std::size_t addr = bucketAddress(value);
		BucketVecIterator it(m_buckets.begin() + addr);
		BucketIterator buckIt(std::find(it->begin(), it->end(), value));
		if (buckIt != it->end())
			return Iterator(it, m_buckets.end(), buckIt);
		else
			return end();
	}

	std::size_t count(const Value& value) const
	{
		return find(value) != end() ? 1 : 0;
	}

	/// Inserts an element into the table.
	///
	/// If the element already exists in the table,
	/// a pair(iterator, false) with iterator pointing to the 
	/// existing element is returned.
	/// Otherwise, the element is inserted an a 
	/// pair(iterator, true) with iterator
	/// pointing to the new element is returned.
	std::pair<Iterator, bool> insert(const Value& value)
	{
		std::size_t hash = m_hash(value);
		std::size_t addr = bucketAddressForHash(hash);
		BucketVecIterator it(m_buckets.begin() + addr);
		BucketIterator buckIt(std::find(it->begin(), it->end(), value));
		if (buckIt == it->end())
		{
			split();
			addr = bucketAddressForHash(hash);
			it = m_buckets.begin() + addr;
			buckIt = it->insert(it->end(), value);
			++m_size;
			return std::make_pair(Iterator(it, m_buckets.end(), buckIt), true);
		}
		else
		{
			return std::make_pair(Iterator(it, m_buckets.end(), buckIt), false);
		}
	}

	void erase(Iterator it)
	{
		if (it != end())
		{
			it._vecIt->erase(it._buckIt);
			--m_size;
			merge();
		}
	}

	void erase(const Value& value)
	{
		Iterator it = find(value);
		erase(it);
	}

	void clear()
	{
		HashTable emptyTable;
		swap(emptyTable);
	}

	std::size_t size() const
	{
		return m_size;
	}

	bool empty() const
	{
		return m_size == 0;
	}

	std::size_t buckets() const
	{
		return m_buckets.size();
	}

protected:
	std::size_t bucketAddress(const Value& value) const
	{
		std::size_t n = m_hash(value);
		if (n % m_front >= m_split)
			return n % m_front;
		else
			return n % (2 * m_front);
	}

	std::size_t bucketAddressForHash(std::size_t hash)
	{
		if (hash % m_front >= m_split)
			return hash % m_front;
		else
			return hash % (2 * m_front);
	}

	void split()
	{
		if (m_split == m_front)
		{
			m_split = 0;
			m_front *= 2;
			m_buckets.reserve(m_front * 2);
		}
		Bucket tmp;
		m_buckets.push_back(tmp);
		m_buckets[m_split].swap(tmp);
		++m_split;
		for (BucketIterator it = tmp.begin(); it != tmp.end(); ++it)
		{
			using std::swap;
			std::size_t addr = bucketAddress(*it);
			m_buckets[addr].push_back(Value());
			swap(*it, m_buckets[addr].back());
		}
	}

	void merge()
	{
		if (m_split == 0)
		{
			m_front /= 2;
			m_split = m_front;
		}
		--m_split;
		Bucket tmp;
		tmp.swap(m_buckets.back());
		m_buckets.pop_back();
		for (BucketIterator it = tmp.begin(); it != tmp.end(); ++it)
		{
			using std::swap;
			std::size_t addr = bucketAddress(*it);
			m_buckets[addr].push_back(Value());
			swap(*it, m_buckets[addr].back());
		}
	}

	static std::size_t calcSize(std::size_t initialSize)
	{
		std::size_t size = 32;
		while (size < initialSize) size *= 2;
		return size;
	}

private:
	// Evil hack: _buckets must be mutable because both ConstIterator and Iterator hold 
	// ordinary iterator's (not const_iterator's).
	mutable BucketVec m_buckets;
	std::size_t	m_split;
	std::size_t m_front;
	std::size_t m_size;
	HashFunc    m_hash;
};

CUTE_NS_END