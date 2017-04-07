#pragma once
#include "Cute/Ascii.h"

CUTE_NS_BEGIN

CUTE_CORE_API void format(String& result, const char* fmt, va_list& va);
CUTE_CORE_API void format(String& result, const char* fmt, ...);
CUTE_CORE_API String format(const char* fmt, ...);

class CUTE_CORE_API StringUtils
{
public:
	static bool startsWith(const String& str, const String& prefix);
	static bool endsWith(const String& str, const String& suffix);

	static String trim(const String& str);
	static String trimLeft(const String& str);
	static String trimRight(const String& str);

	static String toUpper(const String& str);
	static String toLower(const String& str);

	static String remove(const String& str, const String& key, size_t start = 0);
	static String replace(const String& str, const String& from, const String& to, size_t start = 0);

	static String cat(const String& s1, const String& s2);
	static String cat(const String& s1, const String& s2, const String& s3);
	static String cat(const String& s1, const String& s2, const String& s3, const String& s4);

	static String& trimInPlace(String& str);
	static String& trimLeftInPlace(String& str);
	static String& trimRightInPlace(String& str);

	static String& toUpperInPlace(String& str);
	static String& toLowerInPlace(String& str);

	static String& removeInPlace(String& str, const String& key, size_t start = 0);
	static String& replaceInPlace(String& str, const String& from, const String& to, size_t start = 0);
};

/// Tests whether the string starts with the given prefix.
template <class S>
bool startsWith(const S& str, const S& prefix)
{
	return str.size() >= prefix.size() && equal(prefix.begin(), prefix.end(), str.begin());
}

/// Tests whether the string ends with the given suffix.
template <class S>
bool endsWith(const S& str, const S& suffix)
{
	return str.size() >= suffix.size() && equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

template <class S>
S trimLeft(const S& str)
{
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();

	while (it != end && Ascii::isSpace(*it)) ++it;
	return S(it, end);
}

template <class S>
S& trimLeftInPlace(S& str)
{
	typename S::iterator it = str.begin();
	typename S::iterator end = str.end();

	while (it != end && Ascii::isSpace(*it)) ++it;
	str.erase(str.begin(), it);
	return str;
}

template <class S>
S trimRight(const S& str)
{
	int pos = int(str.size()) - 1;

	while (pos >= 0 && Ascii::isSpace(str[pos])) --pos;
	return S(str, 0, pos + 1);
}

template <class S>
S& trimRightInPlace(S& str)
{
	int pos = int(str.size()) - 1;

	while (pos >= 0 && Ascii::isSpace(str[pos])) --pos;
	str.resize(pos + 1);

	return str;
}

template <class S>
S trim(const S& str)
{
	int first = 0;
	int last = int(str.size()) - 1;

	while (first <= last && Ascii::isSpace(str[first])) ++first;
	while (last >= first && Ascii::isSpace(str[last])) --last;

	return S(str, first, last - first + 1);
}

template <class S>
S& trimInPlace(S& str)
{
	int first = 0;
	int last = int(str.size()) - 1;

	while (first <= last && Ascii::isSpace(str[first])) ++first;
	while (last >= first && Ascii::isSpace(str[last])) --last;

	str.resize(last + 1);
	str.erase(0, first);

	return str;
}

template <class S>
S toUpper(const S& str)
{
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();

	S result;
	result.reserve(str.size());
	while (it != end) result += static_cast<typename S::value_type>(Ascii::toUpper(*it++));
	return result;
}

template <class S>
S& toUpperInPlace(S& str)
{
	typename S::iterator it = str.begin();
	typename S::iterator end = str.end();

	while (it != end) { *it = static_cast<typename S::value_type>(Ascii::toUpper(*it)); ++it; }
	return str;
}

template <class S>
S toLower(const S& str)
{
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();

	S result;
	result.reserve(str.size());
	while (it != end) result += static_cast<typename S::value_type>(Ascii::toLower(*it++));
	return result;
}

template <class S>
S& toLowerInPlace(S& str)
{
	typename S::iterator it = str.begin();
	typename S::iterator end = str.end();

	while (it != end) { *it = static_cast<typename S::value_type>(Ascii::toLower(*it)); ++it; }
	return str;
}

/// Returns a copy of str with all characters in
/// from replaced by the corresponding (by position)
/// characters in to. If there is no corresponding
/// character in to, the character is removed from
/// the copy. 
template <class S>
S translate(const S& str, const S& from, const S& to)
{
	S result;
	result.reserve(str.size());
	typename S::const_iterator it = str.begin();
	typename S::const_iterator end = str.end();
	typename S::size_type toSize = to.size();
	while (it != end)
	{
		typename S::size_type pos = from.find(*it);
		if (pos == S::npos)
		{
			result += *it;
		}
		else
		{
			if (pos < toSize) result += to[pos];
		}
		++it;
	}
	return result;
}

/// Replaces in str all occurrences of characters in from
/// with the corresponding (by position) characters in to.
/// If there is no corresponding character, the character
/// is removed.
template <class S>
S& translateInPlace(S& str, const S& from, const S& to)
{
	str = translate(str, from, to);
	return str;
}

template <class S>
S& replaceInPlace(S& str, const S& from, const S& to, typename S::size_type start = 0)
{
	assert(from.size() > 0);

	S result;
	typename S::size_type pos = 0;
	result.append(str, 0, start);
	do
	{
		pos = str.find(from, start);
		if (pos != S::npos)
		{
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + from.length();
		}
		else result.append(str, start, str.size() - start);
	} while (pos != S::npos);
	str.swap(result);
	return str;
}

template <class S>
S& replaceInPlace(S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{
	assert(*from);

	S result;
	typename S::size_type pos = 0;
	typename S::size_type fromLen = std::strlen(from);
	result.append(str, 0, start);
	do
	{
		pos = str.find(from, start);
		if (pos != S::npos)
		{
			result.append(str, start, pos - start);
			result.append(to);
			start = pos + fromLen;
		}
		else result.append(str, start, str.size() - start);
	} while (pos != S::npos);
	str.swap(result);
	return str;
}

template <class S>
S& replaceInPlace(S& str, const typename S::value_type from, const typename S::value_type to = 0, typename S::size_type start = 0)
{
	if (from == to) return str;

	typename S::size_type pos = 0;
	do
	{
		pos = str.find(from, start);
		if (pos != S::npos)
		{
			if (to) str[pos] = to;
			else str.erase(pos, 1);
		}
	} while (pos != S::npos);

	return str;
}

template <class S>
S& removeInPlace(S& str, const typename S::value_type ch, typename S::size_type start = 0)
{
	return replaceInPlace(str, ch, 0, start);
}

/// Replace all occurrences of from (which must not be the empty string)
/// in str with to, starting at position start.
template <class S>
S replace(const S& str, const S& from, const S& to, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, from, to, start);
	return result;
}

template <class S>
S replace(const S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, from, to, start);
	return result;
}

template <class S>
S replace(const S& str, const typename S::value_type from, const typename S::value_type to = 0, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, from, to, start);
	return result;
}

template <class S>
S remove(const S& str, const typename S::value_type ch, typename S::size_type start = 0)
{
	S result(str);
	replaceInPlace(result, ch, 0, start);
	return result;
}

////////////////////////////////////////////////////////////////////////////
/// Concatenates strings.
//////////////////////////////////////////////////////////////////////////
template <class S>
S cat(const S& s1, const S& s2)
{
	S result = s1;
	result.reserve(s1.size() + s2.size());
	result.append(s2);
	return result;
}

template <class S>
S cat(const S& s1, const S& s2, const S& s3)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size());
	result.append(s2);
	result.append(s3);
	return result;
}

template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size() + s4.size());
	result.append(s2);
	result.append(s3);
	result.append(s4);
	return result;
}

template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4, const S& s5)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size() + s4.size() + s5.size());
	result.append(s2);
	result.append(s3);
	result.append(s4);
	result.append(s5);
	return result;
}

template <class S>
S cat(const S& s1, const S& s2, const S& s3, const S& s4, const S& s5, const S& s6)
{
	S result = s1;
	result.reserve(s1.size() + s2.size() + s3.size() + s4.size() + s5.size() + s6.size());
	result.append(s2);
	result.append(s3);
	result.append(s4);
	result.append(s5);
	result.append(s6);
	return result;
}

template <class S, class It>
S cat(const S& delim, const It& begin, const It& end)
{
	S result;
	for (It it = begin; it != end; ++it)
	{
		if (!result.empty()) result.append(delim);
		result += *it;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
// icompare
//////////////////////////////////////////////////////////////////////////
/// Case-insensitive string comparison
template <class S, class It>
int icompare(
	const S& str,
	typename S::size_type pos,
	typename S::size_type n,
	It it2,
	It end2)
{
	typename S::size_type sz = str.size();
	if (pos > sz) pos = sz;
	if (pos + n > sz) n = sz - pos;
	It it1 = str.begin() + pos;
	It end1 = str.begin() + pos + n;
	while (it1 != end1 && it2 != end2)
	{
		typename S::value_type c1(static_cast<typename S::value_type>(Ascii::toLower(*it1)));
		typename S::value_type c2(static_cast<typename S::value_type>(Ascii::toLower(*it2)));
		if (c1 < c2)
			return -1;
		else if (c1 > c2)
			return 1;
		++it1; ++it2;
	}

	if (it1 == end1)
		return it2 == end2 ? 0 : -1;
	else
		return 1;
}

// A special optimization for an often used case.
template <class S>
int icompare(const S& str1, const S& str2)
{
	typename S::const_iterator it1(str1.begin());
	typename S::const_iterator end1(str1.end());
	typename S::const_iterator it2(str2.begin());
	typename S::const_iterator end2(str2.end());
	while (it1 != end1 && it2 != end2)
	{
		typename S::value_type c1(static_cast<typename S::value_type>(Ascii::toLower(*it1)));
		typename S::value_type c2(static_cast<typename S::value_type>(Ascii::toLower(*it2)));
		if (c1 < c2)
			return -1;
		else if (c1 > c2)
			return 1;
		++it1; ++it2;
	}

	if (it1 == end1)
		return it2 == end2 ? 0 : -1;
	else
		return 1;
}

template <class S>
int icompare(const S& str1, typename S::size_type n1, const S& str2, typename S::size_type n2)
{
	if (n2 > str2.size()) n2 = str2.size();
	return icompare(str1, 0, n1, str2.begin(), str2.begin() + n2);
}

template <class S>
int icompare(const S& str1, typename S::size_type n, const S& str2)
{
	if (n > str2.size()) n = str2.size();
	return icompare(str1, 0, n, str2.begin(), str2.begin() + n);
}

template <class S>
int icompare(const S& str1, typename S::size_type pos, typename S::size_type n, const S& str2)
{
	return icompare(str1, pos, n, str2.begin(), str2.end());
}

template <class S>
int icompare(
	const S& str1,
	typename S::size_type pos1,
	typename S::size_type n1,
	const S& str2,
	typename S::size_type pos2,
	typename S::size_type n2)
{
	typename S::size_type sz2 = str2.size();
	if (pos2 > sz2) pos2 = sz2;
	if (pos2 + n2 > sz2) n2 = sz2 - pos2;
	return icompare(str1, pos1, n1, str2.begin() + pos2, str2.begin() + pos2 + n2);
}

template <class S>
int icompare(
	const S& str1,
	typename S::size_type pos1,
	typename S::size_type n,
	const S& str2,
	typename S::size_type pos2)
{
	typename S::size_type sz2 = str2.size();
	if (pos2 > sz2) pos2 = sz2;
	if (pos2 + n > sz2) n = sz2 - pos2;
	return icompare(str1, pos1, n, str2.begin() + pos2, str2.begin() + pos2 + n);
}

template <class S>
int icompare(
	const S& str,
	typename S::size_type pos,
	typename S::size_type n,
	const typename S::value_type* ptr)
{
	//poco_check_ptr(ptr);
	typename S::size_type sz = str.size();
	if (pos > sz) pos = sz;
	if (pos + n > sz) n = sz - pos;
	typename S::const_iterator it = str.begin() + pos;
	typename S::const_iterator end = str.begin() + pos + n;
	while (it != end && *ptr)
	{
		typename S::value_type c1(static_cast<typename S::value_type>(Ascii::toLower(*it)));
		typename S::value_type c2(static_cast<typename S::value_type>(Ascii::toLower(*ptr)));
		if (c1 < c2)
			return -1;
		else if (c1 > c2)
			return 1;
		++it; ++ptr;
	}

	if (it == end)
		return *ptr == 0 ? 0 : -1;
	else
		return 1;
}

template <class S>
int icompare(
	const S& str,
	typename S::size_type pos,
	const typename S::value_type* ptr)
{
	return icompare(str, pos, str.size() - pos, ptr);
}

template <class S>
int icompare(
	const S& str,
	const typename S::value_type* ptr)
{
	return icompare(str, 0, str.size(), ptr);
}

//////////////////////////////////////////////////////////////////////////
// case-insensitive string equality
//////////////////////////////////////////////////////////////////////////
template <typename charT>
struct i_char_traits : public std::char_traits<charT>
{
	inline static bool eq(charT c1, charT c2)
	{
		return Ascii::toLower(c1) == Ascii::toLower(c2);
	}

	inline static bool ne(charT c1, charT c2)
	{
		return !eq(c1, c2);
	}

	inline static bool lt(charT c1, charT c2)
	{
		return Ascii::toLower(c1) < Ascii::toLower(c2);
	}

	static int compare(const charT* s1, const charT* s2, size_t n)
	{
		for (int i = 0; i < n && s1 && s2; ++i, ++s1, ++s2)
		{
			if (Ascii::toLower(*s1) == Ascii::toLower(*s2)) continue;
			else if (Ascii::toLower(*s1) < Ascii::toLower(*s2)) return -1;
			else return 1;
		}

		return 0;
	}

	static const charT* find(const charT* s, int n, charT a)
	{
		while (n-- > 0 && Ascii::toLower(*s) != Ascii::toLower(a)) { ++s; }
		return s;
	}
};

/// Case-insensitive std::string counterpart.
typedef std::basic_string<char, i_char_traits<char> > istring;

/// Case-insensitive substring; searches for a substring without regards to case.
template<typename T>
std::size_t isubstr(const T& str, const T& sought)
{
	typename T::const_iterator it = std::search(str.begin(), str.end(),
		sought.begin(), sought.end(),
		i_char_traits<typename T::value_type>::eq);

	if (it != str.end()) return it - str.begin();
	else return static_cast<std::size_t>(T::npos);
}

/// Case-insensitive less-than functor; useful for standard maps
/// and sets with std::strings keys and case-insensitive ordering
/// requirement.
struct CILess
{
	inline bool operator() (const std::string& s1, const std::string& s2) const
	{
		return icompare(s1, s2) < 0;
	}
};

CUTE_NS_END
