#pragma once
#include "Cute/Platform.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <queue>

#ifdef CUTE_CPP11
#include <atomic>
#include <unordered_map>
#include <unordered_set>
#elif defined __GNUC__ || defined __APPLE__
#include <ext/hash_map>
#include <ext/hash_set>
namespace std{
    using namespace __gnu_cxx;
}
#else
#include <hash_map>
#include <hash_set>
#endif

CUTE_NS_BEGIN

typedef int8_t			int8;
typedef int16_t			int16;
typedef int32_t			int32;
typedef int64_t			int64;
typedef uint8_t			uint8;
typedef uint16_t		uint16;
typedef uint32_t		uint32;
typedef uint64_t		uint64;
typedef int8_t			sint8;
typedef int16_t			sint16;
typedef int32_t			sint32;
typedef int64_t			sint64;
typedef float			float32;
typedef double			float64;

typedef signed char		schar;
typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;

typedef std::string		String;
typedef std::wstring	WString;
typedef char			UTF8Char;
typedef String			UTF8String;

#if defined(__SIZEOF_WCHAR_T__)//gcc
#	if (__SIZEOF_WCHAR_T__ == 2)
    typedef wchar_t                      UTF16Char;
    typedef std::wstring                 UTF16String;
    typedef uint32_t                     UTF32Char;
    typedef std::basic_string<UTF32Char> UTF32String;
#	elif (__SIZEOF_WCHAR_T__ == 4)
    typedef uint16_t                     UTF16Char;
    typedef std::basic_string<UTF16Char> UTF16String;
    typedef wchar_t                      UTF32Char;
    typedef std::wstring                 UTF32String;
#	endif
#else
    typedef wchar_t                      UTF16Char;
    typedef std::wstring                 UTF16String;
    typedef uint32_t                     UTF32Char;
    typedef std::basic_string<UTF32Char> UTF32String;
#endif

// STL Define
#ifdef CUTE_CPP11

//template<typename T, typename A = std::allocator<T> >
//using List = std::list<T, A>;

template<typename T, typename A = std::allocator<T> >
using Vector = std::vector<T, A>;

template<typename T, typename A = std::allocator<T> >
using Deque = std::deque<T, A>;

template<typename T, typename A = std::allocator<T>>
using Stack = std::stack<T, std::deque<T, A> >;

template<typename T, typename A = std::allocator<T> >
using Queue = std::queue<T, std::deque<T, A> >;

template<typename T, typename P = std::less<T>, typename A = std::allocator<T> >
using Set = std::set<T, P, A>;

template<typename K, typename V, typename P = std::less<T>, typename A = std::allocator<T> >
using Map = std::map<K, V, P, A>;

/** An associative container containing an ordered set of key-value pairs where multiple elements can have the same key. */
template <typename K, typename V, typename P = std::less<K>, typename A = std::allocator<std::pair<const K, V>>>
using MultiMap = std::multimap<K, V, P, A>;

template<typename T, typename H = std::hash<T>, typename C = std::equal_to<T>, typename A = std::allocator<T> >
using HashSet = std::unordered_set<T, H, C, A>;

/** An associative container containing an ordered set of key-value pairs. Usually faster than Map for larger data sets. */
template <typename K, typename V, typename H = std::hash<K>, typename C = std::equal_to<K>, typename A = std::allocator<std::pair<const K, V>>>
using HashMap = std::unordered_map<K, V, H, C, A>;

#else
// 继承实现
//template<typename T, typename A = std::allocator<T> >
//class List : public std::list<T, A>{};

template<typename T, typename A = std::allocator<T> >
class Vector : public std::vector<T, A>{};

template<typename T, typename A = std::allocator<T> >
class Deque : public std::deque<T, A> {};

template<typename T, typename A = std::allocator<T> >
class Stack : public std::stack<T, std::deque<T, A> >{};

template<typename T, typename A = std::allocator<T> >
class Queue : public std::queue<T, std::deque<T, A> >{};

template<typename T, typename P = std::less<T>, typename A = std::allocator<T> >
class Set : public std::set<T, P, A> {};

template<typename K, typename V, typename P = std::less<K>, typename A = std::allocator<std::pair<const K, V> > >
class Map : public std::map<K, V, P, A> {};

/** An associative container containing an ordered set of key-value pairs where multiple elements can have the same key. */
template <typename K, typename V, typename P = std::less<K>, typename A = std::allocator<std::pair<const K, V> > >
class MultiMap : public std::multimap<K, V, P, A> { };

template<typename T, typename H = std::hash<T>, typename A = std::allocator<T> >
class HashSet : public std::hash_set<T, H, A> { };

/** An associative container containing an ordered set of key-value pairs. Usually faster than Map for larger data sets. */
template <typename K, typename V, typename H = std::hash<K>, typename A = std::allocator<std::pair<const K, V> > >
class HashMap : public std::hash_map<K, V, H, A> { };

#endif

CUTE_NS_END
