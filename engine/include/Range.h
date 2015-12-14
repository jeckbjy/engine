#pragma once
#include "API.h"

CU_NS_BEGIN

// todo:rename to slice??
template<class Iter>
class Range
{
public:
	typedef Iter iterator;
	typedef Iter const_iterator;
	typedef typename std::iterator_traits<Iter> traits_t;
	typedef typename traits_t::value_type value_type;
	typedef typename traits_t::reference  reference;
	typedef typename traits_t::pointer	  pointer;
	typedef std::char_traits<typename std::remove_const<value_type>::type> traits_type;

	static const size_t npos = static_cast<size_t>(-1);

public:
	Range() :m_beg(), m_end(){}
	Range(Iter start, Iter end) :m_beg(start), m_end(end){}
	Range(Iter start, size_t len) :m_beg(start), m_end(start + len){}

	void clear(){
		m_beg = Iter(); 
		m_end = Iter(); 
	}
	void set(Iter start, Iter end){
		m_beg = start;
		m_end = end;
	}
	void set(Iter start, size_t len){
		m_beg = start;
		m_end = start + len;
	}
	size_t size() const { return m_end - m_end; }

	bool empty() const { return m_beg == m_end; }
	Iter data() const { return m_beg; }
	Iter begin() const { return m_beg; }
	Iter end() const { return m_end; }
	Iter cbegin() const{ return m_beg; }
	Iter cend() const { return m_end; }

	void swap(Range& rhs);
	Range slice(size_t first, size_t length = npos) const;

	// tokenize,comporess??
	Range split_step(const Range& delimiter, bool ignore = false);
	Range split_step(const value_type& delimiter, bool ignore = false);

	size_t find(const Range<Iter>& other, size_t off = 0) const;
	size_t find(const value_type& value, size_t off = 0) const;
	size_t rfind(const Range<Iter>& other, size_t off = 0) const;
	size_t rfind(const value_type& value, size_t off = 0) const;
	size_t find_first_of(const Range<Iter>& other, size_t off = 0) const;
	int compare(const Range<Iter>& rhs) const;

	value_type& at(size_t i) { return m_beg[i]; }
	const value_type& at(size_t i) const { return m_beg[i]; }
	value_type& operator[](size_t i){ return m_beg[i]; }
	const value_type& operator[](size_t i) const { return m_beg[i]; }

private:
	Iter m_beg, m_end;
};

template<class T>
inline size_t qfind(const Range<T>& haystack, const typename Range<T>::value_type& needle)
{
	auto pos = std::find(haystack.begin(), haystack.end(), needle);
	return pos == haystack.end() ? std::string::npos : pos - haystack.data();
}

template<class T>
inline size_t rfind(const Range<T>& haystack, const typename Range<T>::value_type& needle)
{
	for (auto i = haystack.size(); i-- > 0;) {
		if (haystack[i] == needle) {
			return i;
		}
	}
	return std::string::npos;
}
// 查找其中任意一个
template<class T, class Cmp = std::equal_to<typename Range<T>::value_type> >
inline size_t qfind_first_of(const Range<T>& haystack, const Range<T>& needles, Cmp eq = Cmp())
{
	auto ret = std::find_first_of(haystack.begin(), haystack.end(), needles.begin(), needles.end(), eq);
	return ret == haystack.end() ? std::string::npos : ret - haystack.begin();
}

template<>
inline size_t rfind(const Range<const char*>& haystack, const char& needle)
{
	auto pos = static_cast<const char*>(::memrchr(haystack.data(), needle, haystack.size()));
	return pos == nullptr ? std::string::npos : pos - haystack.data();
}
//////////////////////////////////////////////////////////////////////////
// impl
//////////////////////////////////////////////////////////////////////////
template<class T>
void swap(Range<T>& lhs, Range<T>& rhs)
{
	lhs.swap(rhs);
}

template<typename Iter>
void Range<Iter>::swap(Range& rhs)
{
	std::swap(m_beg, rhs.m_beg);
	std::swap(m_end, rhs.m_end);
}

template<typename Iter>
Range<Iter> Range<Iter>::slice(size_t first, size_t length /* = npos */) const
{
	assert(first < size());
	return Range(m_beg + first, std::min(length, size() - first));
}

template<typename Iter>
size_t Range<Iter>::find(const Range<Iter>& other, size_t off) const
{
	return quick_find(*this, other, off);
}

template<typename Iter>
size_t Range<Iter>::find(const value_type& value, size_t pos) const
{
	if (pos == 0)
		return qfind(*this, value);
	size_t ret = qfind(slice(pos), value);
	return ret == npos ? ret : ret + pos;
}

template<typename Iter>
size_t Range<Iter>::rfind(const Range<Iter>& other, size_t off) const
{
	return quick_rfind(*this, other, off);
}

template<typename Iter>
size_t Range<Iter>::rfind(const value_type& value, size_t pos) const
{
	if (pos == 0)
		return rfind(*this, value);
	else
		return rfind(slice(0, size() - pos), value);
}

template<typename Iter>
size_t Range<Iter>::find_first_of(const Range<Iter>& other, size_t pos) const
{
	if (pos == 0)
		return qfind_first_of(*this, other);
	size_t ret = qfind_first_of(slice(pos), value);
	return ret == npos ? ret : ret + pos;
}

template<typename Iter>
Range<Iter> Range<Iter>::split_step(const Range& delimiter, bool ignore /* = false */)
{
	size_t pos = find(delimiter);
	if (ignore)
	{// 忽略开始的delimiter
		while (pos == 0)
		{
			m_beg += delimiter.size();
			pos = find(delimiter);
		}
	}
	Range result(m_beg, pos == npos ? size() : pos);
	m_beg = result.end() == m_end ? m_end : std::next(result.end(), delimiter.size());
	return result;
}

template<typename Iter>
Range<Iter> Range<Iter>::split_step(const value_type& delimiter, bool ignore /* = false */)
{
	auto pos = std::find(m_beg, m_end, delimiter);
	if (ignore)
	{
		while (pos == m_beg)
		{
			++m_beg;
			pos = std::find(m_beg, m_end, delimiter);
		}
	}
	Range result(m_beg, pos);
	m_beg = (pos == m_end) ? m_end : std::next(pos);
	return result;
}

template<typename Iter>
int Range<Iter>::compare(const Range<Iter>& o) const
{
	const size_type tsize = this->size();
	const size_type osize = o.size();
	const size_type msize = std::min(tsize, osize);
	int r = traits_type::compare(data(), o.data(), msize);
	if (r == 0 && tsize != osize) {
		// We check the signed bit of the subtraction and bit shift it
		// to produce either 0 or 2. The subtraction yields the
		// comparison values of either -1 or 1.
		r = (static_cast<int>((osize - tsize) >> (CHAR_BIT * sizeof(size_t) - 1)) << 1) - 1;
	}
	return r;
}

//////////////////////////////////////////////////////////////////////////
// define
//////////////////////////////////////////////////////////////////////////
//typedef Range<const char*> StringPiece;

CU_NS_END