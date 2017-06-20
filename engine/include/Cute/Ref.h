#pragma once
#include "Cute/Atomic.h"

CUTE_NS_BEGIN

// 防止模板膨胀
template<class T>
class RefBase
{
public:
	RefBase(T counter = 0)
		: m_counter(counter)
	{
	}

	~RefBase()
	{
	}

	inline void incRef() const
	{
		++m_counter;
	}

	inline T getRef() const
	{
		return m_counter;
	}

	inline bool isOne() const
	{
		return m_counter == 1;
	}

protected:
	inline bool decRefBase() const
	{
		if (--m_counter <= 0)
			return true;

		return false;
	}

private:
	mutable T m_counter;
};

template<class T>
class Ref : public RefBase<int>
{
public:
	void decRef() const
	{
		if (decRefBase())
			delete static_cast<const T*>(this);
	}

protected:
	Ref(){}
	~Ref(){}
};

template<class T>
class SafeRef : public RefBase<Atomic>
{
public:
	void decRef() const
	{
		if (decRefBase())
			delete static_cast<const T*>(this);
	}

protected:
	SafeRef(){}
	~SafeRef(){}
};

CUTE_NS_END
