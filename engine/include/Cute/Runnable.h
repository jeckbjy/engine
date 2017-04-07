#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// The Runnable interface with the run() method
/// must be implemented by classes that provide
/// an entry point for a thread.
class CUTE_CORE_API Runnable
{
public:
	Runnable();
	virtual ~Runnable();

	/// Do whatever the thread needs to do. Must
	/// be overridden by subclasses.
	virtual void run() = 0;
};

template <class Functor>
class FunctorRunnable : public Runnable
{
public:
	FunctorRunnable(const Functor& functor)
		: m_functor(functor)
	{
	}

	~FunctorRunnable()
	{
	}

	void run()
	{
		m_functor();
	}

private:
	Functor m_functor;
};

CUTE_NS_END