#pragma once
#include "Cute/Types.h"

CUTE_NS_BEGIN

// 元组
template<typename T0 = void, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void>
struct Tuple
{
	Tuple(T0& t0, T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) :t0(t0), t1(t1), t2(t2), t3(t3), t4(t4), t5(t5){}
	T0 t0;
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;
	T5 t5;
};

template<typename T0, typename T1, typename T2, typename T3, typename T4>
struct Tuple<T0, T1, T2, T3, T4, void>
{
	Tuple(T0& t0, T1& t1, T2& t2, T3& t3, T4& t4) :t0(t0), t1(t1), t2(t2), t3(t3), t4(t4){}
	T0 t0;
	T1 t1;
	T2 t2;
	T3 t3;
	T4 t4;
};

template<typename T0, typename T1, typename T2, typename T3>
struct Tuple<T0, T1, T2, T3, void, void>
{
	Tuple(T0& t0, T1& t1, T2& t2, T3& t3) :t0(t0), t1(t1), t2(t2), t3(t3){}
	T0 t0;
	T1 t1;
	T2 t2;
	T3 t3;
};

template<typename T0, typename T1, typename T2>
struct Tuple<T0, T1, T2, void, void, void>
{
	Tuple(T0& t0, T1& t1, T2& t2) :t0(t0), t1(t1), t2(t2){}
	T0 t0;
	T1 t1;
	T2 t2;
};

template<typename T0, typename T1>
struct Tuple<T0, T1, void, void, void, void>
{
	Tuple(T0& t0, T1& t1) :t0(t0), t1(t1){}
	T0 t0;
	T1 t1;
};

template<typename T0>
struct Tuple<T0, void, void, void, void, void>
{
	Tuple(T0& t0) :t0(t0){}
	T0 t0;
};

template<>
struct Tuple<void, void, void, void, void, void>
{
	Tuple(){}
};

CUTE_NS_END