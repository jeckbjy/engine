#pragma once
#include "Cute/Foundation.h"
#include "Cute/FuncTraits.h"
#include <typeinfo>

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// MemberFunction
//////////////////////////////////////////////////////////////////////////
#ifdef CUTE_VARIADIC
template<class F, class S>
class mem_func
{
protected:
	typedef typename func_traits<F>::class_t C;
	F m_fun;
	C m_obj;
};

template<class F, class R, class... Args>
class mem_func<F, R(Args...)>
{
	typedef typename func_traits<F>::class_t C;
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
	F m_fun;
	C m_obj;
};
#else
template<class F>
class mem_func_base
{
protected:
	typedef typename func_traits<F>::class_t C;
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
// Function:ȫ�ֺ���ָ����߳�Ա����ָ����߷º���,��������ָ��
//1���ڳ�ʼ��ʱ����ȷ�������ܶ�̬�ı�
//2����Delegate����:Delegateֻ���������ǩ�����ɰ�����ȫ�ֺ�������̬��������Ա����
//////////////////////////////////////////////////////////////////////////
// �º���:equal:�����ֵ�ַ��ֻҪtypeinfoһ�¼���
template<typename F, typename E = void>
class Function
{
public:
	Function(F fun) : m_fun(fun){}
	Function(F fun, void*) : m_fun(fun){}

	inline F	get() { return m_fun; }
	inline void setOwner(void* obj){}
	inline bool equal(void*) const { return true; }
	inline const std::type_info& type() const { return typeid(F); }
	inline operator bool() const { return true; }

	inline bool operator!() const { return false; }
	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
	// ����
	inline operator F() const { return m_fun; }

protected:
	F m_fun;
};

// ��ͨ����ָ��
template<typename F>
class Function<F, typename std::enable_if<std::is_pointer<F>::value>::type>
{
public:
	Function(F fun) :m_fun(fun){}
	Function(F fun, void* obj) :m_fun(fun){}

	inline F	get() { return m_fun; }
	inline void setOwner(void* obj){}
	inline bool equal(void* fun_ptr) const { return m_fun == fun_ptr; }
	inline const std::type_info& type() const { return typeid(F); }

	inline operator bool() const { return m_fun != NULL; }
	inline bool operator!() const { return !m_fun; }
	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }

	// ����
	inline operator F() const { return m_fun; }
private:
	F m_fun;
};

// ��Ա����ָ��
template<typename F>
class Function<F, typename std::enable_if<std::is_member_function_pointer<F>::value>::type>
	: public mem_func<F, typename func_traits<F>::signature_t>
{
public:
	Function(F fun, void* obj = NULL) :mem_func(fun, (class_t*)obj){}

	inline F	get() { return m_fun; }
	inline void setOwner(void* obj) { m_obj = (class_t*)obj; }
	inline bool equal(void* fun_ptr) const { return m_fun == fun_ptr; }
	inline const std::type_info& type() const { return typeid(F); }

	inline operator bool() const { return m_fun && m_obj; }
	inline bool operator!() const { return !(m_fun && m_obj); }
	inline bool operator ==(const Function& rhs) const { return m_fun == rhs.m_fun; }
	inline bool operator !=(const Function& rhs) const { return m_fun != rhs.m_fun; }
};

CUTE_NS_END
