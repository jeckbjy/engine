#pragma once
#include "Function.h"
#include "SmartPtr.h"

CU_NS_BEGIN

// 修正？？
template<class S>
class DelegateBase 
{
protected:
	struct ICallable : public Ref
	{};
	SharedPtr<ICallable> m_fun;
};

template<class R>
class DelegateBase<R()>
{
protected:
	struct ICallable : public Ref
	{
		virtual ~ICallable(){}
		virtual R call() = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		Function<F> m_fun;
		TCallable(F fun) :m_fun(fun){}
		R call(){ return m_fun(); }
		void set(void* obj) { m_fun.setObject(obj); }
		bool empty() const { return !m_fun; }
	};
	SharedPtr<ICallable> m_fun;
public:
	DelegateBase(ICallable* fun) :m_fun(fun){}
	inline R operator()() { return m_fun->call(); }
};

template<class R, class T1>
class DelegateBase<R(T1)>
{
protected:
	struct ICallable : public Ref
	{
		virtual ~ICallable(){}
		virtual R call(T1) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};
	template<class F>
	struct TCallable : public ICallable
	{
		Function<F> m_fun;
		TCallable(F fun) :m_fun(fun){}
		R call(T1 t1){ return m_fun(t1); }
		void set(void* obj) { m_fun.setObject(obj); }
		bool empty() const { return !m_fun; }
	};
	SharedPtr<ICallable> m_fun;
public:
	DelegateBase(ICallable* fun) :m_fun(fun){}
	inline R operator()(T1 t1) { return m_fun->call(t1); }
};

template<class R, class T1, class T2>
class DelegateBase<R(T1, T2)>
{
protected:
	struct ICallable : public Ref
	{
		virtual ~ICallable(){}
		virtual R call(T1, T2) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};
	template<class F>
	struct TCallable : public ICallable
	{
		Function<F> m_fun;
		TCallable(F fun) :m_fun(fun){}
		R call(T1 t1, T2 t2){ return m_fun(t1, t2); }
		void set(void* obj) { m_fun.setObject(obj); }
		bool empty() const { return !m_fun; }
	};
	SharedPtr<ICallable> m_fun;
public:
	DelegateBase(ICallable* fun) :m_fun(fun){}
	inline R operator()(T1 t1, T2 t2) { return m_fun->call(t1, t2); }
};

template<class R, class T1, class T2, class T3>
class DelegateBase<R(T1, T2, T3)>
{
protected:
	struct ICallable : public Ref
	{
		virtual ~ICallable(){}
		virtual R call(T1, T2, T3) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		Function<F> m_fun;
		TCallable(F fun) :m_fun(fun){}
		R call(T1 t1, T2 t2, T3 t3){ return m_fun(t1, t2, t3); }
		void set(void* obj) { m_fun.setObject(obj); }
		bool empty() const { return !m_fun; }
	};
	SharedPtr<ICallable> m_fun;
public:
	DelegateBase(ICallable* fun) :m_fun(fun){}
	inline R operator()(T1 t1, T2 t2, T3 t3) { return m_fun->call(t1, t2, t3); }
};

template<class R, class T1, class T2, class T3, class T4>
class DelegateBase<R(T1, T2, T3, T4)>
{
protected:
	struct ICallable : public Ref
	{
		virtual ~ICallable(){}
		virtual R call(T1, T2, T3, T4) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};
	template<class F>
	struct TCallable : public ICallable
	{
		Function<F> m_fun;
		TCallable(F fun) :m_fun(fun){}
		R call(T1 t1, T2 t2, T3 t3, T4 t4){ return m_fun(t1, t2, t3, t4); }
		void set(void* obj) { m_fun.setObject(obj); }
		bool empty() const { return !m_fun; }
	};
	SharedPtr<ICallable> m_fun;
public:
	DelegateBase(ICallable* fun) :m_fun(fun){}
	inline R operator()(T1 t1, T2 t2, T3 t3, T4 t4) { return m_fun->call(t1, t2, t3, t4); }
};

template<class R, class T1, class T2, class T3, class T4, class T5>
class DelegateBase<R(T1, T2, T3, T4, T5)>
{
protected:
	struct ICallable : public Ref
	{
		virtual ~ICallable(){}
		virtual R call(T1, T2, T3, T4, T5) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};
	template<class F>
	struct TCallable : public ICallable
	{
		Function<F> m_fun;
		TCallable(F fun) :m_fun(fun){}
		R call(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5){ return m_fun(t1, t2, t3, t4, t5); }
		void set(void* obj) { m_fun.setObject(obj); }
		bool empty() const { return !m_fun; }
	};
	SharedPtr<ICallable> m_fun;
public:
	DelegateBase(ICallable* fun) :m_fun(fun){}
	inline R operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { return m_fun->call(t1, t2, t3, t4, t5); }
};

template<class R, class T1, class T2, class T3, class T4, class T5, class T6>
class DelegateBase<R(T1, T2, T3, T4, T5, T6)>
{
protected:
	struct ICallable : public Ref
	{
		virtual ~ICallable(){}
		virtual R call(T1, T2, T3, T4, T5, T6) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};
	template<class F>
	struct TCallable : public ICallable
	{
		Function<F> m_fun;
		TCallable(F fun) :m_fun(fun){}
		R call(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6){ return m_fun(t1, t2, t3, t4, t5, t6); }
		void set(void* obj) { m_fun.setObject(obj); }
		bool empty() const { return !m_fun; }
	};
	SharedPtr<ICallable> m_fun;
public:
	DelegateBase(ICallable* fun) :m_fun(fun){}
	inline R operator()(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) { return m_fun->call(t1, t2, t3, t4, t5, t6); }
};

//////////////////////////////////////////////////////////////////////////
// 签名一致即可
//////////////////////////////////////////////////////////////////////////
template<class S>
class Delegate : public DelegateBase<S>
{
public:
	Delegate() :DelegateBase(NULL){}
	template<typename F>
	Delegate(F fun) : DelegateBase<S>(new TCallable<F>(fun)){}
	template<typename F>
	Delegate(F fun, void* obj) : DelegateBase<S>(new TCallable<F>(fun)) { setObject(obj); }

	inline void setObject(void* obj) { m_fun->set(obj); }
	inline void reset() { m_fun.reset(); }
	inline bool empty() const 
	{
		return !m_fun || m_fun->empty(); 
	}
	inline bool operator!() const { return empty(); }
	inline operator bool() const { return !empty(); }
};

CU_NS_END