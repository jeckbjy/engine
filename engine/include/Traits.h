#pragma once
#include "Types.h"

CU_NS_BEGIN

// 获取原始类型
template<typename T>
struct _type_traits
{
	typedef T  value_t;
};

template<typename T>
struct _type_traits<T*>
{
	typedef T  value_t;
};

template<typename T>
struct _type_traits<T&>
{
	typedef T  value_t;
};

template<typename T>
struct type_trait : public _type_traits<std::remove_cv<T>::type>
{
	typedef value_t* pointer_t;
	typedef value_t& reference_t;

	typedef const value_t  cvalue_t;
	typedef const value_t* cpointer_t;
	typedef const value_t& creference_t;
};

// 对于没有实现c++11的编译器
template<bool _Test, class T = void>
struct EnableIf { };

template<class T>
struct EnableIf<true, T> { typedef T type; };

template<bool _Test, class T = void>
struct DisableIf { };
template<class T>
struct DisableIf<false, T> { typedef T type; };

template<bool _Test, class T1, class T2>
struct Conditional { typedef T2 type; };

template<class T1, class T2>
struct Conditional<true, T1, T2> { typedef T1 type; };

CU_NS_END