#pragma once
#include "Function.h"
#include "SmartPtr.h"
#include "List.h"

CU_NS_BEGIN

template<class S>
class DelegateBase
{
	struct ICallable;
	typedef List<ICallable> FuncList;
	FuncList m_funs;
};

#ifdef CU_CPP11
// 支持变长参数
template<class R, class... Args>
class DelegateBase<R(Args...)>
{
public:
	R operator()(Args... args){ return call<R>(args...); }

protected:
	struct ICallable;
	typedef List<ICallable> FuncList;

	struct ICallable : public ListNode<ICallable>
	{
		virtual ~ICallable(){}
		virtual R call(Args...) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		TCallable(F fun, void* owner){}
		TCallable(Function<F> fun) : m_fun(fun){}
		R call(Args... args) { return m_fun(args...); }
		void set(void* owner) { m_fun.setOwner(owner); }
		bool empty() const { return !m_fun; }

		Function<F> m_fun;
	};

	template<typename R>
	typename std::enable_if<std::is_void<R>::value>::type
		call(Args... args)
	{
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			itor->call(args...);
		}
	}

	template<typename R>
	typename std::enable_if<!std::is_void<R>::value, R>::type
		call(Args... args)
	{
		R result;
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			result = itor->call(args);
		}
		return result;
	}

	FuncList m_funs;
};

#else
// 不支持变长参数
template<class R>
class DelegateBase<R()>
{
public:
	R operator()(void){ return call<R>(); }

protected:
	struct ICallable;
	typedef List<ICallable> FuncList;

	struct ICallable : public ListNode<ICallable>
	{
		virtual ~ICallable(){}
		virtual R call() = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		TCallable(F fun, void* owner){}
		TCallable(Function<F> fun) : m_fun(fun){}
		R call() { return m_fun(); }
		void set(void* owner) { m_fun.setOwner(owner); }
		bool empty() const { return !m_fun; }

		Function<F> m_fun;
	};

	template<typename R>
	typename std::enable_if<std::is_void<R>::value>::type
		call()
	{
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			itor->call();
		}
	}

	template<typename R>
	typename std::enable_if<!std::is_void<R>::value, R>::type
		call()
	{
		R result;
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			result = itor->call();
		}
		return result;
	}

	FuncList m_funs;
};

template<class R, class T0>
class DelegateBase<R(T0)>
{
public:
	R operator()(T0 t0){ return call<R>(t0); }

protected:
	struct ICallable;
	typedef List<ICallable> FuncList;

	struct ICallable : public ListNode<ICallable>
	{
		virtual ~ICallable(){}
		virtual R call(T0) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		TCallable(F fun, void* owner){}
		TCallable(Function<F> fun) : m_fun(fun){}
		R call(T0 t0) { return m_fun(t0); }
		void set(void* owner) { m_fun.setOwner(owner); }
		bool empty() const { return !m_fun; }

		Function<F> m_fun;
	};

	template<typename R>
	typename std::enable_if<std::is_void<R>::value>::type
		call(T0 t0)
	{
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			itor->call(t0);
		}
	}

	template<typename R>
	typename std::enable_if<!std::is_void<R>::value, R>::type
		call(T0 t0)
	{
		R result;
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			result = itor->call(t0);
		}
		return result;
	}

	FuncList m_funs;
};

template<class R, class T0, class T1>
class DelegateBase<R(T0, T1)>
{
public:
	R operator()(T0 t0, T1 t1){ return call<R>(t0, t1); }

protected:
	struct ICallable;
	typedef List<ICallable> FuncList;

	struct ICallable : public ListNode<ICallable>
	{
		virtual ~ICallable(){}
		virtual R call(T0, T1) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		TCallable(F fun, void* owner){}
		TCallable(Function<F> fun) : m_fun(fun){}
		R call(T0 t0, T1 t1) { return m_fun(t0, t1); }
		void set(void* owner) { m_fun.setOwner(owner); }
		bool empty() const { return !m_fun; }

		Function<F> m_fun;
	};

	template<typename R>
	typename std::enable_if<std::is_void<R>::value, void>::type
		call(T0 t0, T1 t1)
	{
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			itor->call(t0, t1);
		}
	}

	template<typename R>
	typename std::enable_if<!std::is_void<R>::value, R>::type
		call(T0 t0, T1 t1)
	{
		R result;
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			result = itor->call(t0, t1);
		}

		return result;
	}

	FuncList m_funs;
};

template<class R, class T0, class T1, class T2>
class DelegateBase<R(T0, T1, T2)>
{
public:
	R operator()(T0 t0, T1 t1, T2 t2){ return call<R>(t0, t1, t2); }

protected:
	struct ICallable;
	typedef List<ICallable> FuncList;

	struct ICallable : public ListNode<ICallable>
	{
		virtual ~ICallable(){}
		virtual R call(T0, T1, T2) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		TCallable(F fun, void* owner){}
		TCallable(Function<F> fun) : m_fun(fun){}
		R call(T0 t0, T1 t1, T2 t2) { return m_fun(t0, t1, t2); }
		void set(void* owner) { m_fun.setOwner(owner); }
		bool empty() const { return !m_fun; }

		Function<F> m_fun;
	};

	template<typename R>
	typename std::enable_if<std::is_void<R>::value, void>::type
		call(T0 t0, T1 t1, T2 t2)
	{
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			itor->call(t0, t1, t2);
		}
	}

	template<typename R>
	typename std::enable_if<!std::is_void<R>::value, R>::type
		call(T0 t0, T1 t1, T2 t2)
	{
		R result;
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			result = itor->call(t0, t1, t2);
		}

		return result;
	}

	FuncList m_funs;
};

template<class R, class T0, class T1, class T2, class T3>
class DelegateBase<R(T0, T1, T2, T3)>
{
public:
	R operator()(T0 t0, T1 t1, T2 t2, T3 t3){ return call<R>(t0, t1, t2, t3); }

protected:
	struct ICallable;
	typedef List<ICallable> FuncList;

	struct ICallable : public ListNode<ICallable>
	{
		virtual ~ICallable(){}
		virtual R call(T0, T1, T2, T3) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		TCallable(F fun, void* owner){}
		TCallable(Function<F> fun) : m_fun(fun){}
		R call(T0 t0, T1 t1, T2 t2, T3 t3) { return m_fun(t0, t1, t2, t3); }
		void set(void* owner) { m_fun.setOwner(owner); }
		bool empty() const { return !m_fun; }

		Function<F> m_fun;
	};

	template<typename R>
	typename std::enable_if<std::is_void<R>::value, void>::type
		call(T0 t0, T1 t1, T2 t2, T3 t3)
	{
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			itor->call(t0, t1, t2, t3);
		}
	}

	template<typename R>
	typename std::enable_if<!std::is_void<R>::value, R>::type
		call(T0 t0, T1 t1, T2 t2, T3 t3)
	{
		R result;
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			result = itor->call(t0, t1, t2, t3);
		}

		return result;
	}

	FuncList m_funs;
};

#endif

// 允许有多个,如果返回值非void，则返回最后一个结果
// setOwner只会修改第一个Owner
// 不支持拷贝
template<class S>
class Delegate : public DelegateBase<S>
{
public:
	Delegate(){}
	
	template<typename F>
	Delegate(F fun, void* obj = NULL)
	{
		add(Function<F>(fun, obj)); 
	}

	~Delegate() { reset(); }

	template<typename F>
	Delegate& operator+=(Function<F> fun) 
	{
		add(fun);
		return *this; 
	}

	// 尚不需要支持equal接口
	//template<typename F>
	//Delegate& operator -=(Function<F> fun)
	//{
	//	for (FuncList::itor itor = m_funs.begin(); itor != m_funs.end(); ++itor)
	//	{
	//		if (itor->equal(fun))
	//		{
	//			m_funs.erase(itor);
	//			break;
	//		}
	//	}
	//	return *this;
	//}

	template<typename F>
	Delegate& operator=(Function<F> fun)
	{
		reset();
		add(fun);
		return *this;
	}

	template<typename F>
	Delegate& operator=(F fun)
	{
		reset();
		add(Function<F>(fun));
		return *this;
	}

	template<typename F>
	inline void add(Function<F> fun)
	{
		m_funs.push_back(new TCallable<F>(fun));
	}

	void setOwner(void* obj) 
	{
		if (!m_funs.empty())
			m_funs.begin()->set(obj); 
	}

	void reset()
	{
		for (FuncList::iterator itor = m_funs.begin(); itor != m_funs.end(); ++itor)
		{
			delete (*itor);
		}
		m_funs.clear();
	}

	bool empty() const
	{
		return m_funs.empty();
	}

	bool operator!() const
	{
		return empty();
	}

	operator bool() const
	{
		return !empty();
	}
};

CU_NS_END
