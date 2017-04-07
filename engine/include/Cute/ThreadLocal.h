#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API TLSAbstractSlot
{
public:
	TLSAbstractSlot();
	virtual ~TLSAbstractSlot();
};

/// The Slot template wraps another class
/// so that it can be stored in a ThreadLocalStorage
/// object. This class is used internally, and you
/// must not create instances of it yourself.
template <class C>
class TLSSlot : public TLSAbstractSlot
{
public:
	TLSSlot()
		: m_value()
	{
	}

	~TLSSlot()
	{
	}

	C& value()
	{
		return m_value;
	}

private:
	TLSSlot(const TLSSlot&);
	TLSSlot& operator = (const TLSSlot&);

	C m_value;
};

/// This class manages the local storage for each thread.
/// Never use this class directly, always use the
/// ThreadLocal template for managing thread local storage.
class CUTE_CORE_API ThreadLocalStorage
{
public:
	ThreadLocalStorage();
	~ThreadLocalStorage();

	TLSAbstractSlot*& get(const void* key);

	static ThreadLocalStorage& current();
	static void clear();

private:
	typedef std::map<const void*, TLSAbstractSlot*> TLSMap;

	TLSMap m_map;

	friend class Thread;
};

/// This template is used to declare type safe thread
/// local variables. It can basically be used like
/// a smart pointer class with the special feature
/// that it references a different object
/// in every thread. The underlying object will
/// be created when it is referenced for the first
/// time.
/// See the NestedDiagnosticContext class for an
/// example how to use this template.
/// Every thread only has access to its own
/// thread local data. There is no way for a thread
/// to access another thread's local data.
template <class C>
class ThreadLocal
{
	typedef TLSSlot<C> Slot;

public:
	ThreadLocal()
	{
	}

	~ThreadLocal()
	{
	}

	C* operator -> ()
	{
		return &get();
	}

	C& operator * ()
	{
		return get();
	}

	C& get()
	{
		TLSAbstractSlot*& p = ThreadLocalStorage::current().get(this);
		if (!p) p = new Slot;
		return static_cast<Slot*>(p)->value();
	}

private:
	ThreadLocal(const ThreadLocal&);
	ThreadLocal& operator = (const ThreadLocal&);
};

CUTE_NS_END