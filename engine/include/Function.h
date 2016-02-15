#pragma once
#include "FuncTraits.h"

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// member function helper
//////////////////////////////////////////////////////////////////////////
#ifdef CU_CPP11
// 支持变长参数
template<class F, class S>
class mem_func
{
};

template<class F, class R, class... Args>
class mem_func<F, R(Args...)>
{
protected:
	typedef typename mem_func_traits<F>::class_t C;
	F	m_fun;
	C*	m_obj;

public:
	mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
	R operator()(Args... args) 
	{
		if (m_obj)
			return (m_obj->*m_fun)(args...); 
	}
};

#else

template<class F>
class mem_func_base
{
protected:
	typedef typename mem_func_traits<F>::class_t C;
	F  m_fun;
	C* m_obj;

public:
	mem_func_base(F fun, C* obj) :m_fun(fun), m_obj(obj){}
};

template<class F, class S>
class mem_func : public mem_func_base<F>
{
};

template<class F, class R>
class mem_func<F, R()> : public mem_func_base<F>
{
public:
	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
	inline R operator()() { return (m_obj->*m_fun)(); }
};

template<class F, class R, class T0>
class mem_func<F, R(T0)> : public mem_func_base<F>
{
public:
	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
	inline R operator()(T0 t0) { return (m_obj->*m_fun)(t0); }
};

template<class F, class R, class T0, class T1>
class mem_func<F, R(T0, T1)> : public mem_func_base<F>
{
public:
	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
	inline R operator()(T0 t0, T1 t1) { return (m_obj->*m_fun)(t0, t1); }
};

template<class F, class R, class T0, class T1, class T2>
class mem_func<F, R(T0, T1, T2)> : public mem_func_base<F>
{
public:
	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
	inline R operator()(T0 t0, T1 t1, T2 t2) { return (m_obj->*m_fun)(t0, t1, t2); }
};

template<class F, class R, class T0, class T1, class T2, class T3>
class mem_func<F, R(T0, T1, T2, T3)> : public mem_func_base<F>
{
public:
	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
	inline R operator()(T0 t0, T1 t1, T2 t2, T3 t3) { return (m_obj->*m_fun)(t0, t1, t2, t3); }
};

template<class F, class R, class T0, class T1, class T2, class T3, class T4>
class mem_func<F, R(T0, T1, T2, T3, T4)> : public mem_func_base<F>
{
public:
	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
	inline R operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) { return (m_obj->*m_fun)(t0, t1, t2, t3, t4); }
};

template<class F, class R, class T0, class T1, class T2, class T3, class T4, class T5>
class mem_func<F, R(T0, T1, T2, T3, T4, T5)> : public mem_func_base<F>
{
public:
	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
	inline R operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { return (m_obj->*m_fun)(t0, t1, t2, t3, t4, t5); }
};
#endif

//////////////////////////////////////////////////////////////////////////
// function
//////////////////////////////////////////////////////////////////////////
// 仿函数
template<typename F, typename E = void>
class Function
{
public:
	Function(F fun) :m_fun(fun){}
	Function(F fun, void* obj) :m_fun(fun){}

	inline void setOwner(void* obj){}
	inline bool operator!() const { return false; }
	inline operator bool() const { return true; }
	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
protected:
	F m_fun;
};

// 普通函数
template<typename F>
class Function<F, typename std::enable_if<std::is_pointer<F>::value>::type>
{
public:
	Function(F fun) :m_fun(fun){}
	Function(F fun, void* obj) :m_fun(fun){}
	inline void setOwner(void* obj){}
	inline bool operator!() const { return !m_fun; }
	inline operator bool() const { return m_fun != NULL; }
	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
	// 转移
	inline operator F() const { return m_fun; }
private:
	F m_fun;
};

// 成员函数指针
template<typename F>
class Function<F, typename std::enable_if<std::is_member_function_pointer<F>::value>::type>
	: public mem_func<F, typename mem_func_traits<F>::signature_t>
{
public:
	typedef typename func_traits<F>		traits_t;
	typedef typename traits_t::class_t	class_t;

public:
	Function(F fun, void* obj = NULL) :mem_func(fun, (class_t*)obj){}
	inline void setOwner(void* obj) { m_obj = (class_t*)obj; }
	inline bool operator!() const { return !(m_fun && m_obj); }
	inline operator bool() const { return m_fun && m_obj; }
	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
};

CU_NS_END
