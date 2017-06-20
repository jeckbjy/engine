#pragma once
#include "Cute/Foundation.h"
#include "Cute/Function.h"
#include "Cute/List.h"

CUTE_NS_BEGIN

template<class S>
class DelegateBase
{
	struct ICallable;
	typedef List<ICallable> FuncList;
	FuncList m_funs;
};

#ifdef CUTE_VARIADIC
// 支持变长参数
template<class R, class... Args>
class DelegateBase<R(Args...)>
{
public:
	R operator()(Args... args){ return call<R>(args...); }

protected:
	struct ICallable;
	typedef List<ICallable> FuncList;

	struct ICallable : public ListHook
	{
		virtual ~ICallable(){}
		virtual R call(Args...) = 0;
		virtual void set(void*) = 0;
		virtual bool empty() const = 0;
		virtual bool equal(void* ptr) const = 0;
		virtual const type_info& type() const = 0;
	};

	template<class F>
	struct TCallable : public ICallable
	{
		TCallable(F fun, void* owner){}
		TCallable(Function<F> fun) : m_fun(fun){}
		R call(Args... args) { return m_fun(args...); }
		void set(void* owner) { m_fun.setOwner(owner); }
		bool empty() const { return !m_fun; }
		bool equal(void* ptr) const { return m_fun.equal(ptr); }
		const type_info& type() const { return m_fun.type(); }

		Function<F> m_fun;
	};

	R call(Args... args)
	{
		if (!m_funs.empty())
		{
			FuncList::Iterator last = m_funs.back();
			for (FuncList::Iterator itor = m_funs.begin(); itor != last; ++itor)
			{
				itor->call(args...);
			}

			return last->call(args...);
		}

		return R();
	}

	FuncList m_funs;
};

#else
// 不支持变长参数
#define CUTE_DELEGATE(N, P)																			\
template<class R CUTE_COMMA_IF(N) CUTE_LIST_TYPENAME(N)>											\
class DelegateBase<R(CUTE_LIST_TYPE(N))>															\
{																									\
public:																								\
	R operator()(CUTE_LIST_DEFS(N)){ return call<R>(CUTE_LIST_ARGS(N)); }							\
																									\
protected:																							\
	struct ICallable;																				\
	typedef List<ICallable> FuncList;																\
																									\
	struct ICallable : public ListHook																\
	{																								\
		virtual ~ICallable(){}																		\
		virtual R call(CUTE_LIST_DEFS(N)) = 0;														\
		virtual void set(void*) = 0;																\
		virtual bool empty() const = 0;																\
		virtual bool equal(void* ptr) const = 0;													\
		virtual const type_info& type() const = 0;													\
	};																								\
																									\
	template<class F>																				\
	struct TCallable : public ICallable																\
	{																								\
		TCallable(F fun, void* owner){}																\
		TCallable(Function<F> fun) : m_fun(fun){}													\
		R call(CUTE_LIST_DEFS(N)) { return m_fun(CUTE_LIST_ARGS(N)); }								\
		void set(void* owner) { m_fun.setOwner(owner); }											\
		bool empty() const { return !m_fun; }														\
		bool equal(void* ptr) const { return m_fun.equal(ptr); }									\
		const type_info& type() const { return m_fun.type(); }										\
																									\
		Function<F> m_fun;																			\
	};																								\
																									\
	R call(CUTE_LIST_DEFS(N))																		\
	{																								\
		if (!m_funs.empty())																		\
		{																							\
			FuncList::Iterator last = m_funs.back();												\
			for (FuncList::Iterator itor = m_funs.begin(); itor != last; ++itor)					\
			{																						\
				itor->call(CUTE_LIST_ARGS(N));														\
			}																						\
																									\
			return last->call(CUTE_LIST_ARGS(N));													\
		}																							\
																									\
		return R();																					\
	}																								\
																									\
	FuncList m_funs;																				\
};

CUTE_REPEAT(CUTE_PARAM_MAX, CUTE_DELEGATE)
#undef CUTE_DELEGATE
#endif

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

	// 暂时不支持删除
	//template<typename F>
	//Delegate& operator-=(F fun)
	//{
	//	const type_info& type = typeid(F);
	//	void* ptr = func_traits::getAddress(fun);
	//	for (FuncList::Iterator itor = m_funs.begin(); itor != m_funs.end();)
	//	{
	//		if (itor->type() == type && itor->equal(ptr))
	//		{
	//			itor = m_funs.erase(itor);
	//		}
	//		else
	//		{
	//			++itor;
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

CUTE_NS_END
