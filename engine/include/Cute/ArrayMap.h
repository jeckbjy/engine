#pragma once
#include "Cute/Platform.h"
#include <algorithm>

CUTE_NS_BEGIN

/// map_value_compare
///
/// Our adapter for the comparison function in the template parameters.
///
template <typename Key, typename Value, typename Compare>
class map_value_compare : public binary_function<Value, Value, bool>
{
public:
	Compare c;

	map_value_compare(const Compare& x)
		: c(x) {}

public:
	bool operator()(const Value& a, const Value& b) const
	{
		return c(a.first, b.first);
	}

	bool operator()(const Value& a, const Key& b) const
	{
		return c(a.first, b);
	}

	bool operator()(const Key& a, const Value& b) const
	{
		return c(a, b.first);
	}

	bool operator()(const Key& a, const Key& b) const
	{
		return c(a, b);
	}

}; // map_value_compare

template<
	typename K, 
	typename V, 
	typename Cmp = std::less<K>, 
	typename Alloc = std::allocator, 
	typename Container = std::vector<std::pair<K, V>, Alloc> >
class ArrayMap : public Container
{
public:
	typedef ArrayMap<K, V, Cmp, Alloc, Container>		this_type;
	typedef Container									base_type;
	typedef Alloc										allocator_type;
	typedef K											key_type;
	typedef V											mapped_type;
	typedef std::pair<K, V>								value_type;
	typedef Cmp											key_compare;
	typedef map_value_compare<K, V, Cmp>				value_compare;
	typedef value_type*									pointer;
	typedef const value_type*							const_pointer;
	typedef value_type&									reference;
	typedef const value_type&							const_reference;
	typedef typename base_type::size_type				size_type;
	typedef typename base_type::difference_type			difference_type;
	typedef typename base_type::iterator				iterator;
	typedef typename base_type::const_iterator			const_iterator;
	typedef typename base_type::reverse_iterator		reverse_iterator;
	typedef typename base_type::const_reverse_iterator	const_reverse_iterator;
	typedef std::pair<iterator, bool>					insert_return_type;
	typedef std::pair<iterator, iterator>				range;
	typedef std::pair<const_iterator, const_iterator>	const_range;

public:
	ArrayMap();
	ArrayMap(const allocator_type& allocator);
	ArrayMap(const key_compare& cmp, const allocator_type& allocator = allocator_type());
	template<typename InputIterator>
	ArrayMap(InputIterator first, InputIterator last);
	template<typename InputIterator>
	ArrayMap(InputIterator first, InputIterator last, const key_compare& compare);

	this_type& operator=(const this_type& x);
	//this_type& operator=(this_type&& x);

	void swap(this_type& x);

	const key_compare&		key_comp() const;
	key_compare&			key_comp();

	const value_compare&	value_comp() const;
	value_compare&			value_comp();

	insert_return_type		insert(const value_type& value);
	iterator				insert(const_iterator position, const value_type& value);

	template<typename InputIterator>
	void					insert(InputIterator first, InputIterator last);

	iterator				erase(const_iterator position);
	iterator				erase(const_iterator first, const_iterator last);
	size_type				erase(const key_type& key);

	reverse_iterator		erase(const_reverse_iterator position);
	reverse_iterator		erase(const_reverse_iterator first, const_reverse_iterator last);

	iterator				find(const key_type& key);
	const_iterator			find(const key_type& key) const;

	template<typename U, typename Predicate>
	iterator				find_as(const U& u, Predicate predicate);

	template<typename U, typename Predicate>
	const_iterator			find_as(const U& u, Predicate predicate);

	iterator				lower_bound(const key_type& k);
	const_iterator			lower_bound(const key_type& k) const;

	iterator				upper_bound(const key_type& k);
	const_iterator			upper_bound(const key_type& k) const;

	range					equal_range(const key_type& k);
	const_range				equal_range(const key_type& k) const;

	size_type				count(const key_type& key) const;

	mapped_type& operator[](const key_type& k);

protected:
	value_compare mValueCompare;
};

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::ArrayMap()
	: base_type(), mValueCompare(C())
{
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::ArrayMap(const allocator_type& allocator)
	: base_type(allocator), mValueCompare(C())
{
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::ArrayMap(const key_compare& comp, const allocator_type& allocator)
	: base_type(allocator), mValueCompare(comp)
{
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::ArrayMap(const this_type& x)
	: base_type(x), mValueCompare(x.mValueCompare)
{
}

template<typename K, typename V, typename C, typename A, typename RAC>
template <typename InputIterator>
inline ArrayMap<K, V, C, A, RAC>::ArrayMap(InputIterator first, InputIterator last)
	: base_type(), mValueCompare(C())
{
	insert(first, last);
}

template<typename K, typename V, typename C, typename A, typename RAC>
template <typename InputIterator>
inline ArrayMap<K, V, C, A, RAC>::ArrayMap(InputIterator first, InputIterator last, const key_compare& compare)
	: base_type(), mValueCompare(compare)
{
	insert(first, last);
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>&
	ArrayMap<K, V, C, A, RAC>::operator =(const this_type& x)
{
	base_type::operator=(x);
	mValueCompare = value_compare(x.mValueCompare);

	return *this;
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline void ArrayMap<K, V, C, A, RAC>::swap(this_type& x)
{
	base_type::swap(x);
	std::swap(mValueCompare, x.mValueCompare);
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::key_compare& 
	ArrayMap<K, V, C, A, RAC>::key_comp(this_type& x)
{
	return mValueCompare.c;
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline const ArrayMap<K, V, C, A, RAC>::key_compare& 
	ArrayMap<K, V, C, A, RAC>::key_comp(this_type& x) const
{
	return mValueCompare.c;
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::value_compare& 
	ArrayMap<K, V, C, A, RAC>::value_comp(this_type& x)
{
	return mValueCompare;
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline const ArrayMap<K, V, C, A, RAC>::value_compare& 
	ArrayMap<K, V, C, A, RAC>::value_comp(this_type& x) const
{
	return mValueCompare;
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::insert_return_type 
	ArrayMap<K, V, C, A, RAC>::insert(const value_type& value)
{
	const iterator itLB(lower_bound(value.first));

	if ((itLB != end()) && !mValueCompare(value, *itLB))
		return std::pair<iterator, bool>(itLB, false);

	return std::pair<iterator, bool>(base_type::insert(itLB, value), true);
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::iterator
	ArrayMap<K, V, C, A, RAC>::insert(const_iterator position, const value_type& value)
{
	// We assume that the user knows what he is doing and has supplied us with
	// a position that is right where value should be inserted (put in front of). 
	// We do a test to see if the position is correct. If so then we insert, 
	// if not then we ignore the input position.

	if ((position == end()) || mValueCompare(value, *position))  // If the element at position is greater than value...
	{
		if ((position == begin()) || mValueCompare(*(position - 1), value)) // If the element before position is less than value...
			return base_type::insert(position, value);
	}

	// In this case we either have an incorrect position or value is already present.
	// We fall back to the regular insert function. An optimization would be to detect
	// that the element is already present, but that's only useful if the user supplied
	// a good position but a present element.
	const std::pair<typename vector_map<K, T, C, A, RAC>::iterator, bool> result = insert(value);

	return result.first;
}

template<typename K, typename V, typename C, typename A, typename RAC>
template <typename InputIterator>
inline void ArrayMap<K, V, C, A, RAC>::insert(InputIterator first, InputIterator last)
{
	// To consider: Improve the speed of this by getting the length of the 
	//              input range and resizing our container to that size
	//              before doing the insertions. We can't use reserve
	//              because we don't know if we are using a vector or not.
	//              Alternatively, force the user to do the reservation.
	// To consider: When inserting values that come from a container
	//              like this container, use the property that they are 
	//              known to be sorted and speed up the inserts here.
	for (; first != last; ++first)
		insert(*first);
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::iterator
	ArrayMap<K, V, C, A, RAC>::erase(const_iterator position)
{
	// Note that we return iterator and not void. This allows for more efficient use of 
	// the container and is consistent with the C++ language defect report #130 (DR 130)
	return base_type::erase(position);
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::iterator
	ArrayMap<K, V, C, A, RAC>::erase(const_iterator first, const_iterator last)
{
	return base_type::erase(first, last);
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::size_type
	ArrayMap<K, V, C, A, RAC>::erase(const key_type& k)
{
	const iterator it(find(k));

	if (it != end()) // If it exists...
	{
		erase(it);
		return 1;
	}
	return 0;
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::reverse_iterator
	ArrayMap<K, V, C, A, RAC>::erase(const_reverse_iterator position)
{
	// Note that we return iterator and not void. This allows for more efficient use of 
	// the container and is consistent with the C++ language defect report #130 (DR 130)
	return reverse_iterator(base_type::erase((++position).base()));
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::reverse_iterator
	ArrayMap<K, V, C, A, RAC>::erase(const_reverse_iterator first, const_reverse_iterator last)
{
	return reverse_iterator(base_type::erase((++last).base(), (++first).base()));
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::iterator
	ArrayMap<K, V, C, A, RAC>::find(const key_type& k)
{
	const std::pair<iterator, iterator> pairIts(equal_range(k));

	if (pairIts.first != pairIts.second)
		return pairIts.first;
	return end();
}

template<typename K, typename V, typename C, typename A, typename RAC>
inline ArrayMap<K, V, C, A, RAC>::const_iterator
	ArrayMap<K, V, C, A, RAC>::find(const key_type& k) const
{
	const std::pair<const_iterator, const_iterator> pairIts(equal_range(k));

	if (pairIts.first != pairIts.second)
		return pairIts.first;
	return end();
}

template<typename K, typename V, typename C, typename A, typename RAC>
template <typename U, typename Predicate>
inline ArrayMap<K, V, C, A, RAC>::iterator
	ArrayMap<K, V, C, A, RAC>::find_as(const U& u, Predicate /*predicate*/) const
{
	// To do: Implement this.
	return find(u);
}

template<typename K, typename V, typename C, typename A, typename RAC>
template <typename U, typename Predicate>
inline ArrayMap<K, V, C, A, RAC>::const_iterator
	ArrayMap<K, V, C, A, RAC>::find_as(const U& u, Predicate /*predicate*/) const
{
	// To do: Implement this.
	return find(u);
}

template <typename K, typename V, typename C, typename A, typename RAC>
inline typename ArrayMap<K, V, C, A, RAC>::size_type
	ArrayMap<K, T, C, A, RAC>::count(const key_type& k) const
{
	const const_iterator it(find(k));
	return (it != end()) ? (size_type)1 : (size_type)0;
}

template <typename K, typename T, typename C, typename A, typename RAC>
inline typename ArrayMap<K, T, C, A, RAC>::iterator
	ArrayMap<K, T, C, A, RAC>::lower_bound(const key_type& k)
{
	return std::lower_bound(begin(), end(), k, mValueCompare);
}


template <typename K, typename T, typename C, typename A, typename RAC>
inline typename ArrayMap<K, T, C, A, RAC>::const_iterator
	ArrayMap<K, T, C, A, RAC>::lower_bound(const key_type& k) const
{
	return std::lower_bound(begin(), end(), k, mValueCompare);
}


template <typename K, typename T, typename C, typename A, typename RAC>
inline typename ArrayMap<K, T, C, A, RAC>::iterator
	ArrayMap<K, T, C, A, RAC>::upper_bound(const key_type& k)
{
	return std::upper_bound(begin(), end(), k, mValueCompare);
}


template <typename K, typename T, typename C, typename A, typename RAC>
inline typename ArrayMap<K, T, C, A, RAC>::const_iterator
	ArrayMap<K, T, C, A, RAC>::upper_bound(const key_type& k) const
{
	return std::upper_bound(begin(), end(), k, mValueCompare);
}


template <typename K, typename T, typename C, typename A, typename RAC>
inline std::pair<typename ArrayMap<K, T, C, A, RAC>::iterator, typename ArrayMap<K, T, C, A, RAC>::iterator>
	ArrayMap<K, T, C, A, RAC>::equal_range(const key_type& k)
{
	// The resulting range will either be empty or have one element,
	// so instead of doing two tree searches (one for lower_bound and 
	// one for upper_bound), we do just lower_bound and see if the 
	// result is a range of size zero or one.
	const iterator itLower(lower_bound(k));

	if ((itLower == end()) || mValueCompare(k, *itLower)) // If at the end or if (k is < itLower)...
		return eastl::pair<iterator, iterator>(itLower, itLower);

	iterator itUpper(itLower);
	return std::pair<iterator, iterator>(itLower, ++itUpper);
}


template <typename K, typename T, typename C, typename A, typename RAC>
inline std::pair<typename ArrayMap<K, T, C, A, RAC>::const_iterator, typename ArrayMap<K, T, C, A, RAC>::const_iterator>
	ArrayMap<K, T, C, A, RAC>::equal_range(const key_type& k) const
{
	// The resulting range will either be empty or have one element,
	// so instead of doing two tree searches (one for lower_bound and 
	// one for upper_bound), we do just lower_bound and see if the 
	// result is a range of size zero or one.
	const const_iterator itLower(lower_bound(k));

	if ((itLower == end()) || mValueCompare(k, *itLower)) // If at the end or if (k is < itLower)...
		return std::pair<const_iterator, const_iterator>(itLower, itLower);

	const_iterator itUpper(itLower);
	return std::pair<const_iterator, const_iterator>(itLower, ++itUpper);
}


template <typename K, typename T, typename C, typename A, typename RAC>
inline typename ArrayMap<K, T, C, A, RAC>::mapped_type&
	ArrayMap<K, T, C, A, RAC>::operator[](const key_type& k)
{
	iterator itLB(lower_bound(k));

	if ((itLB == end()) || key_comp()(k, (*itLB).first))
		itLB = insert(itLB, value_type(k, mapped_type()));
	return (*itLB).second;
}

//////////////////////////////////////////////////////////////////////////
// global operators
//////////////////////////////////////////////////////////////////////////
template <typename K, typename T, typename C, typename A, typename RAC>
inline bool operator==(const ArrayMap<K, T, C, A, RAC>& a, const ArrayMap<K, T, C, A, RAC>& b)
{
	return (a.size() == b.size()) && equal(b.begin(), b.end(), a.begin());
}

template <typename K, typename T, typename C, typename A, typename RAC>
inline bool operator!=(const ArrayMap<K, T, C, A, RAC>& a, const ArrayMap<K, T, C, A, RAC>& b)
{
	return !(a == b);
}

template <typename K, typename T, typename C, typename A, typename RAC>
inline bool operator<(const ArrayMap<K, T, C, A, RAC>& a, const ArrayMap<K, T, C, A, RAC>& b)
{
	return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), a.value_comp());
}

template <typename K, typename T, typename C, typename A, typename RAC>
inline bool operator>(const ArrayMap<K, T, C, A, RAC>& a, const ArrayMap<K, T, C, A, RAC>& b)
{
	return b < a;
}

template <typename K, typename T, typename C, typename A, typename RAC>
inline bool operator<=(const ArrayMap<K, T, C, A, RAC>& a, const ArrayMap<K, T, C, A, RAC>& b)
{
	return !(b < a);
}

template <typename K, typename T, typename C, typename A, typename RAC>
inline bool operator>=(const ArrayMap<K, T, C, A, RAC>& a, const ArrayMap<K, T, C, A, RAC>& b)
{
	return !(a < b);
}

template <typename K, typename T, typename C, typename A, typename RAC>
inline void swap(const ArrayMap<K, T, C, A, RAC>& a, const ArrayMap<K, T, C, A, RAC>& b)
{
	a.swap(b);
}

CUTE_NS_END
