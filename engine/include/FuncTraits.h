#pragma once
#include "Tuple.h"
// 函数萃取

CU_NS_BEGIN
//////////////////////////////////////////////////////////////////////////
// signature 萃取
//////////////////////////////////////////////////////////////////////////
template<typename S>
struct signature_traits
{
	static const uint length = 0;
	typedef Tuple<> params_t;
	typedef void result_t;
	template<typename F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun();
	}
};

template<typename R>
struct signature_traits<R()>
{
	static const uint length = 0;
	typedef Tuple<> params_t;
	typedef R result_t;

	template<typename F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun();
	}
};

template<class R, class T0>
struct signature_traits<R(T0)>
{
	static const uint length = 1;
	typedef Tuple<T0> params_t;
	typedef R result_t;
	typedef T0 param0_t;

	template<typename F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun(args.t0);
	}
};

template<class R, class T0, class T1>
struct signature_traits<R(T0, T1)>
{
	static const uint length = 2;
	typedef Tuple<T0, T1> params_t;
	typedef R result_t;
	typedef T0 param0_t;
	typedef T1 param1_t;

	template<class F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun(args.t0, args.t1);
	}
};

template<class R, class T0, class T1, class T2>
struct signature_traits<R(T0, T1, T2)>
{
	static const uint length = 3;
	typedef Tuple<T0, T1, T2> params_t;
	typedef R result_t;
	typedef T0 param0_t;
	typedef T1 param1_t;
	typedef T2 param2_t;

	template<class F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun(args.t0, args.t1, args.t2);
	}
};

template<class R, class T0, class T1, class T2, class T3>
struct signature_traits<R(T0, T1, T2, T3)>
{
	static const uint length = 4;
	typedef Tuple<T0, T1, T2, T3> params_t;
	typedef R result_t;
	typedef T0 param0_t;
	typedef T1 param1_t;
	typedef T2 param2_t;
	typedef T3 param3_t;


	template<class F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun(args.t0, args.t1, args.t2, args.t3);
	}
};

template<class R, class T0, class T1, class T2, class T3, class T4>
struct signature_traits<R(T0, T1, T2, T3, T4)>
{
	static const uint length = 5;
	typedef Tuple<T0, T1, T2, T3, T4> params_t;
	typedef R result_t;
	typedef T0 param0_t;
	typedef T1 param1_t;
	typedef T2 param2_t;
	typedef T3 param3_t;
	typedef T4 param4_t;

	template<class F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun(args.t0, args.t1, args.t2, args.t3, args.t4);
	}
};

template<class R, class T0, class T1, class T2, class T3, class T4, class T5>
struct signature_traits<R(T0, T1, T2, T3, T4, T5)>
{
	static const uint length = 6;
	typedef Tuple<T0, T1, T2, T3, T4, T5> params_t;
	typedef R result_t;
	typedef T0 param0_t;
	typedef T1 param1_t;
	typedef T2 param2_t;
	typedef T3 param3_t;
	typedef T4 param4_t;
	typedef T5 param5_t;

	template<class F>
	inline static result_t call(F& fun, params_t& args)
	{
		return fun(args.t0, args.t1, args.t2, args.t3, args.t4, args.t5);
	}
};

//////////////////////////////////////////////////////////////////////////
// 成员函数萃取
//////////////////////////////////////////////////////////////////////////
template<class F>
struct mem_func_traits
{

};

template<class C, class R>
struct mem_func_traits<R(C::*)()>
{
	typedef R(signature_t)();
	typedef C class_t;
};

template<class C, class R>
struct mem_func_traits<R(C::*)() const>
{
	typedef R(signature_t)();
	typedef C class_t;
};

template<class C, class R, class T0>
struct mem_func_traits<R(C::*)(T0)>
{
	typedef R(signature_t)(T0);
	typedef C class_t;
};

template<class C, class R, class T0>
struct mem_func_traits<R(C::*)(T0) const>
{
	typedef R(signature_t)(T0);
	typedef C class_t;
};

template<class C, class R, class T0, class T1>
struct mem_func_traits<R(C::*)(T0, T1)>
{
	typedef R(signature_t)(T0, T1);
	typedef C class_t;
};

template<class C, class R, class T0, class T1>
struct mem_func_traits<R(C::*)(T0, T1) const>
{
	typedef R(signature_t)(T0, T1);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2>
struct mem_func_traits<R(C::*)(T0, T1, T2)>
{
	typedef R(signature_t)(T0, T1, T2);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2>
struct mem_func_traits<R(C::*)(T0, T1, T2) const>
{
	typedef R(signature_t)(T0, T1, T2);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2, class T3>
struct mem_func_traits<R(C::*)(T0, T1, T2, T3)>
{
	typedef R(signature_t)(T0, T1, T2, T3);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2, class T3>
struct mem_func_traits<R(C::*)(T0, T1, T2, T3) const>
{
	typedef R(signature_t)(T0, T1, T2, T3);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2, class T3, class T4>
struct mem_func_traits<R(C::*)(T0, T1, T2, T3, T4)>
{
	typedef R(signature_t)(T0, T1, T2, T3, T4);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2, class T3, class T4>
struct mem_func_traits<R(C::*)(T0, T1, T2, T3, T4) const>
{
	typedef R(signature_t)(T0, T1, T2, T3, T4);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2, class T3, class T4, class T5>
struct mem_func_traits<R(C::*)(T0, T1, T2, T3, T4, T5)>
{
	typedef R(signature_t)(T0, T1, T2, T3, T4, T5);
	typedef C class_t;
};

template<class C, class R, class T0, class T1, class T2, class T3, class T4, class T5>
struct mem_func_traits<R(C::*)(T0, T1, T2, T3, T4, T5) const>
{
	typedef R(signature_t)(T0, T1, T2, T3, T4, T5);
	typedef C class_t;
};

//////////////////////////////////////////////////////////////////////////
// function traits
//////////////////////////////////////////////////////////////////////////
// 函数签名？
template<class F, class Enable = void>
struct func_traits : signature_traits<F>
{

};

template<class F>
struct func_traits<F, typename std::enable_if<std::is_pointer<F>::value>::type>
	: signature_traits<typename std::remove_pointer<F>::type >
{
	static const bool isGlobal = true;
	typedef F func_t;
};

template<class F>
struct func_traits<F, typename std::enable_if<std::is_member_function_pointer<F>::value>::type>
	: signature_traits<typename mem_func_traits<F>::signature_t>
{
	static const bool isGlobal = false;
	typedef F func_t;
	typedef typename mem_func_traits<F>::class_t class_t;
};

CU_NS_END