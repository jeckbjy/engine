#pragma once
#include "Cute/Foundation.h"
#include "Cute/FuncTraits.h"
#include <typeinfo>

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// Function:全局函数指针或者成员函数指针或者仿函数,仅代表函数指针
//1：在初始化时必须确定，不能动态改变
//2：与Delegate区别:Delegate只需给定函数签名即可绑定任意全局函数，静态函数，成员函数
//////////////////////////////////////////////////////////////////////////
// 仿函数:equal:不区分地址，只要typeinfo一致即可
template<typename F, typename E = void>
class Function
{
public:
	Function(F fun) : m_fun(fun){}
	Function(F fun, void*) : m_fun(fun){}

    inline const std::type_info& type() const { return typeid(F); }
    
	inline F	get() { return m_fun; }
	inline bool equal(void*) const { return true; }
    inline void setObject(void* obj){}

	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
    inline bool operator !() const { return false; }
    inline operator bool() const { return true; }
    
	// 调用
	inline operator F() const { return m_fun; }

protected:
	F m_fun;
};

// 普通函数指针
template<typename F>
class Function<F, typename std::enable_if<std::is_pointer<F>::value>::type>
{
public:
	Function(F fun) :m_fun(fun){}
	Function(F fun, void* obj) :m_fun(fun){}

    inline const std::type_info& type() const { return typeid(F); }
    
	inline F	get() { return m_fun; }
	inline bool equal(void* fun_ptr) const { return m_fun == fun_ptr; }
    inline void setObject(void* obj){}

	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
    inline bool operator !() const { return !m_fun; }
    inline operator bool() const { return m_fun != NULL; }
	// 调用
	inline operator F() const { return m_fun; }
    
private:
	F m_fun;
};

template<class F, class C, class S>
class mem_func{};

// 成员函数指针
template<typename F>
class Function<F, typename std::enable_if<std::is_member_function_pointer<F>::value>::type>
    : public mem_func<F, typename func_traits<F>::class_t, typename func_traits<F>::signature_t>
{
    typedef typename func_traits<F>::class_t     class_t;
    typedef typename func_traits<F>::signature_t signature_t;
public:
	Function(F fun, void* obj = NULL) :mem_func<F, class_t, signature_t>(fun, (class_t*)obj){}

    inline const std::type_info& type() const { return typeid(F); }
    
	inline F	get() { return this->m_fun; }
	inline bool equal(void* fun_ptr) const { return this->m_fun == fun_ptr; }
    inline void setObject(void* obj) { this->m_obj = (class_t*)obj; }

	inline bool operator ==(const Function& rhs) const { return this->m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return this->m_fun != rhs.m_fun; }
    inline bool operator !() const { return !(this->m_fun && this->m_obj); }
    inline operator bool() const { return this->m_fun && this->m_obj; }
};

////////////////////////////////////////////////////////////////////////////
//// MemberFunction
////////////////////////////////////////////////////////////////////////////
#ifdef CUTE_VARIADIC
template<class F, class C, class R, class... Args>
class mem_func<F, C, R(Args...)>
{
public:
	mem_func(F fun, C* obj)
		: m_fun(fun)
		, m_obj(obj)
	{
	}

	R operator()(Args... args)
	{
		return (m_obj->*m_fun)(args...);
	}
    
protected:
	F  m_fun;
	C* m_obj;
};
#else

template<class F, class C, class R>
class mem_func<F, C, R()>
{
public:
	mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
	R operator()() { return (m_obj->*m_fun)(); }
    
protected:
    F  m_fun;
    C* m_obj;
};

template<class F, class C, class R, class T0>
class mem_func<F, C, R(T0)>
{
public:
    mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
    R operator()(T0 t0) { return (m_obj->*m_fun)(t0); }
    
protected:
    F  m_fun;
    C* m_obj;
};

template<class F, class C, class R, class T0, class T1>
class mem_func<F, C, R(T0, T1)>
{
public:
    mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
    R operator()(T0 t0, T1 t1) { return (m_obj->*m_fun)(t0, t1); }
    
protected:
    F  m_fun;
    C* m_obj;
};

template<class F, class C, class R, class T0, class T1, class T2>
class mem_func<F, C, R(T0, T1, T2)>
{
public:
    mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
    R operator()(T0 t0, T1 t1, T2 t2) { return (m_obj->*m_fun)(t0, t1, t2); }
    
protected:
    F  m_fun;
    C* m_obj;
};

template<class F, class C, class R, class T0, class T1, class T2, class T3>
class mem_func<F, C, R(T0, T1, T2, T3)>
{
public:
    mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
    R operator()(T0 t0, T1 t1, T2 t2, T3 t3) { return (m_obj->*m_fun)(t0, t1, t2, t3); }
    
protected:
    F  m_fun;
    C* m_obj;
};

template<class F, class C, class R, class T0, class T1, class T2, class T3, class T4>
class mem_func<F, C, R(T0, T1, T2, T3, T4)>
{
public:
    mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
    R operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) { return (m_obj->*m_fun)(t0, t1, t2, t3, t4); }
    
protected:
    F  m_fun;
    C* m_obj;
};

template<class F, class C, class R, class T0, class T1, class T2, class T3, class T4, class T5>
class mem_func<F, C, R(T0, T1, T2, T3, T4, T5)>
{
public:
    mem_func(F fun, C* obj) :m_fun(fun), m_obj(obj){}
    R operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { return (m_obj->*m_fun)(t0, t1, t2, t3, t4, t5); }
    
protected:
    F  m_fun;
    C* m_obj;
};

#endif


////////////////////////////////////////////////////////////////////////////
//// MemberFunction
////////////////////////////////////////////////////////////////////////////
//#ifdef CUTE_VARIADIC
//template<class F, class S>
//class mem_func
//{
//protected:
//	typedef typename func_traits<F>::class_t C;
//	F m_fun;
//	C m_obj;
//};
//
//template<class F, class R, class... Args>
//class mem_func<F, R(Args...)>
//{
//	typedef typename func_traits<F>::class_t C;
//public:
//	mem_func(F fun, C* obj)
//		: m_fun(fun)
//		, m_obj(obj)
//	{
//	}
//
//	R operator()(Args... args)
//	{
//		return (m_obj->*m_fun)(args...);
//	}
//protected:
//	F m_fun;
//	C m_obj;
//};
//#else
//template<class F>
//class mem_func_base
//{
//protected:
//	typedef typename func_traits<F>::class_t C;
//	F  m_fun;
//	C* m_obj;
//
//public:
//	mem_func_base(F fun, C* obj) :m_fun(fun), m_obj(obj){}
//};
//
//template<class F, class S>
//class mem_func : public mem_func_base<F>
//{
//};
//
//template<class F, class R>
//class mem_func<F, R()> : public mem_func_base<F>
//{
//public:
//	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
//	inline R operator()() { return (m_obj->*m_fun)(); }
//};
//
//template<class F, class R, class T0>
//class mem_func<F, R(T0)> : public mem_func_base<F>
//{
//public:
//	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
//	inline R operator()(T0 t0) { return (m_obj->*m_fun)(t0); }
//};
//
//template<class F, class R, class T0, class T1>
//class mem_func<F, R(T0, T1)> : public mem_func_base<F>
//{
//public:
//	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
//	inline R operator()(T0 t0, T1 t1) { return (m_obj->*m_fun)(t0, t1); }
//};
//
//template<class F, class R, class T0, class T1, class T2>
//class mem_func<F, R(T0, T1, T2)> : public mem_func_base<F>
//{
//public:
//	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
//	inline R operator()(T0 t0, T1 t1, T2 t2) { return (m_obj->*m_fun)(t0, t1, t2); }
//};
//
//template<class F, class R, class T0, class T1, class T2, class T3>
//class mem_func<F, R(T0, T1, T2, T3)> : public mem_func_base<F>
//{
//public:
//	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
//	inline R operator()(T0 t0, T1 t1, T2 t2, T3 t3) { return (m_obj->*m_fun)(t0, t1, t2, t3); }
//};
//
//template<class F, class R, class T0, class T1, class T2, class T3, class T4>
//class mem_func<F, R(T0, T1, T2, T3, T4)> : public mem_func_base<F>
//{
//public:
//	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
//	inline R operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4) { return (m_obj->*m_fun)(t0, t1, t2, t3, t4); }
//};
//
//template<class F, class R, class T0, class T1, class T2, class T3, class T4, class T5>
//class mem_func<F, R(T0, T1, T2, T3, T4, T5)> : public mem_func_base<F>
//{
//public:
//	mem_func(F fun, C* obj) :mem_func_base(fun, obj){}
//	inline R operator()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { return (m_obj->*m_fun)(t0, t1, t2, t3, t4, t5); }
//};
//
//#endif
//
////////////////////////////////////////////////////////////////////////////
//// Function:全局函数指针或者成员函数指针或者仿函数,仅代表函数指针
////1：在初始化时必须确定，不能动态改变
////2：与Delegate区别:Delegate只需给定函数签名即可绑定任意全局函数，静态函数，成员函数
////////////////////////////////////////////////////////////////////////////
//// 仿函数:equal:不区分地址，只要typeinfo一致即可
//template<typename F, typename E = void>
//class Function
//{
//public:
//	Function(F fun) : m_fun(fun){}
//	Function(F fun, void*) : m_fun(fun){}
//
//	inline F	get() { return m_fun; }
//	inline void setOwner(void* obj){}
//	inline bool equal(void*) const { return true; }
//	inline const std::type_info& type() const { return typeid(F); }
//	inline operator bool() const { return true; }
//
//	inline bool operator!() const { return false; }
//	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
//	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
//	// 调用
//	inline operator F() const { return m_fun; }
//
//protected:
//	F m_fun;
//};
//
//// 普通函数指针
//template<typename F>
//class Function<F, typename std::enable_if<std::is_pointer<F>::value>::type>
//{
//public:
//	Function(F fun) :m_fun(fun){}
//	Function(F fun, void* obj) :m_fun(fun){}
//
//	inline F	get() { return m_fun; }
//	inline void setOwner(void* obj){}
//	inline bool equal(void* fun_ptr) const { return m_fun == fun_ptr; }
//	inline const std::type_info& type() const { return typeid(F); }
//
//	inline operator bool() const { return m_fun != NULL; }
//	inline bool operator!() const { return !m_fun; }
//	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
//	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
//
//	// 调用
//	inline operator F() const { return m_fun; }
//private:
//	F m_fun;
//};
//
//// 成员函数指针
//template<typename F>
//class Function<F, typename std::enable_if<std::is_member_function_pointer<F>::value>::type>
//	: public mem_func<F, typename func_traits<F>::signature_t>
//{
//public:
//	Function(F fun, void* obj = NULL) :mem_func(fun, (class_t*)obj){}
//
//	inline F	get() { return m_fun; }
//	inline void setOwner(void* obj) { m_obj = (class_t*)obj; }
//	inline bool equal(void* fun_ptr) const { return m_fun == fun_ptr; }
//	inline const std::type_info& type() const { return typeid(F); }
//
//	inline operator bool() const { return m_fun && m_obj; }
//	inline bool operator!() const { return !(m_fun && m_obj); }
//	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
//	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
//};

CUTE_NS_END
