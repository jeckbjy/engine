#pragma once
#include "Cute/Foundation.h"
#include "Cute/Exception.h"
#include "Cute/Atomic.h"

CUTE_NS_BEGIN

/// Simple ReferenceCounter object, does not delete itself when count reaches 0.
class ReferenceCounter
{
public:
	ReferenceCounter() : m_cnt(1)
	{
	}

	void duplicate()
	{
		++m_cnt;
	}

	int release()
	{
		return --m_cnt;
	}

	int refs() const
	{
		return m_cnt.value();
	}

private:
	Atomic m_cnt;
};

/// The default release policy for SharedPtr, which
/// simply uses the delete operator to delete an object.
template <class C>
class ReleasePolicy
{
public:
	/// Delete the object.
	/// Note that pObj can be 0.
	static void release(C* pObj)
	{
		delete pObj;
	}
};

/// The release policy for SharedPtr holding arrays.
template <class C>
class ReleaseArrayPolicy
{
public:
	/// Delete the object.
	/// Note that pObj can be 0.
	static void release(C* pObj)
	{
		delete[] pObj;
	}
};

/// SharedPtr is a "smart" pointer for classes implementing
/// reference counting based garbage collection.
/// SharedPtr is thus similar to AutoPtr. Unlike the
/// AutoPtr template, which can only be used with
/// classes that support reference counting, SharedPtr
/// can be used with any class. For this to work, a
/// SharedPtr manages a reference count for the object
/// it manages.
/// 
/// SharedPtr works in the following way:
/// If an SharedPtr is assigned an ordinary pointer to
/// an object (via the constructor or the assignment operator),
/// it takes ownership of the object and the object's reference 
/// count is initialized to one.
/// If the SharedPtr is assigned another SharedPtr, the
/// object's reference count is incremented by one.
/// The destructor of SharedPtr decrements the object's
/// reference count by one and deletes the object if the
/// reference count reaches zero.
/// SharedPtr supports dereferencing with both the ->
/// and the * operator. An attempt to dereference a null
/// SharedPtr results in a NullPointerException being thrown.
/// SharedPtr also implements all relational operators and
/// a cast operator in case dynamic casting of the encapsulated data types
/// is required.
template <class C, class RC = ReferenceCounter, class RP = ReleasePolicy<C> >
class SharedPtr
{
public:
	SharedPtr() : m_counter(new RC), m_ptr(0)
	{
	}

	SharedPtr(C* ptr) :m_counter(new RC), m_ptr(ptr)
	{
	}

	template <class Other, class OtherRP>
	SharedPtr(const SharedPtr<Other, RC, OtherRP>& ptr) : m_counter(ptr._pCounter), m_ptr(const_cast<Other*>(ptr.get()))
	{
		m_counter->duplicate();
	}

	SharedPtr(const SharedPtr& ptr) : m_counter(ptr.m_counter), m_ptr(ptr.m_ptr)
	{
		m_counter->duplicate();
	}

	~SharedPtr()
	{
		try
		{
			release();
		}
		catch (...)
		{
			cute_unexpected();
		}
	}

	SharedPtr& assign(C* ptr)
	{
		if (get() != ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	SharedPtr& assign(const SharedPtr& ptr)
	{
		if (&ptr != this)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	template <class Other, class OtherRP>
	SharedPtr& assign(const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		if (ptr.get() != m_ptr)
		{
			SharedPtr tmp(ptr);
			swap(tmp);
		}
		return *this;
	}

	SharedPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	SharedPtr& operator = (const SharedPtr& ptr)
	{
		return assign(ptr);
	}

	template <class Other, class OtherRP>
	SharedPtr& operator = (const SharedPtr<Other, RC, OtherRP>& ptr)
	{
		return assign<Other>(ptr);
	}

	void swap(SharedPtr& ptr)
	{
		std::swap(m_ptr, ptr.m_ptr);
		std::swap(m_counter, ptr.m_counter);
	}

	template <class Other>
	SharedPtr<Other, RC, RP> cast() const
		/// Casts the SharedPtr via a dynamic cast to the given type.
		/// Returns an SharedPtr containing NULL if the cast fails.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.cast<Sub>();
		///    cute_assert (sub.get());
	{
		Other* pOther = dynamic_cast<Other*>(m_ptr);
		if (pOther)
			return SharedPtr<Other, RC, RP>(m_counter, pOther);
		return SharedPtr<Other, RC, RP>();
	}

	template <class Other>
	SharedPtr<Other, RC, RP> unsafeCast() const
		/// Casts the SharedPtr via a static cast to the given type.
		/// Example: (assume class Sub: public Super)
		///    SharedPtr<Super> super(new Sub());
		///    SharedPtr<Sub> sub = super.unsafeCast<Sub>();
		///    cute_assert (sub.get());
	{
		Other* pOther = static_cast<Other*>(m_ptr);
		return SharedPtr<Other, RC, RP>(m_counter, pOther);
	}

	C* operator -> ()
	{
		return deref();
	}

	const C* operator -> () const
	{
		return deref();
	}

	C& operator * ()
	{
		return *deref();
	}

	const C& operator * () const
	{
		return *deref();
	}

	C* get()
	{
		return m_ptr;
	}

	const C* get() const
	{
		return m_ptr;
	}

	operator C* ()
	{
		return m_ptr;
	}

	operator const C* () const
	{
		return m_ptr;
	}

	bool operator ! () const
	{
		return m_ptr == 0;
	}

	bool isNull() const
	{
		return m_ptr == 0;
	}

	bool operator == (const SharedPtr& ptr) const
	{
		return get() == ptr.get();
	}

	bool operator == (const C* ptr) const
	{
		return get() == ptr;
	}

	bool operator == (C* ptr) const
	{
		return get() == ptr;
	}

	bool operator != (const SharedPtr& ptr) const
	{
		return get() != ptr.get();
	}

	bool operator != (const C* ptr) const
	{
		return get() != ptr;
	}

	bool operator != (C* ptr) const
	{
		return get() != ptr;
	}

	bool operator < (const SharedPtr& ptr) const
	{
		return get() < ptr.get();
	}

	bool operator < (const C* ptr) const
	{
		return get() < ptr;
	}

	bool operator < (C* ptr) const
	{
		return get() < ptr;
	}

	bool operator <= (const SharedPtr& ptr) const
	{
		return get() <= ptr.get();
	}

	bool operator <= (const C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator <= (C* ptr) const
	{
		return get() <= ptr;
	}

	bool operator > (const SharedPtr& ptr) const
	{
		return get() > ptr.get();
	}

	bool operator > (const C* ptr) const
	{
		return get() > ptr;
	}

	bool operator > (C* ptr) const
	{
		return get() > ptr;
	}

	bool operator >= (const SharedPtr& ptr) const
	{
		return get() >= ptr.get();
	}

	bool operator >= (const C* ptr) const
	{
		return get() >= ptr;
	}

	bool operator >= (C* ptr) const
	{
		return get() >= ptr;
	}

	int refs() const
	{
		return m_counter->refs();
	}

private:
	C* deref() const
	{
		if (!m_ptr)
			throw NullPointerException();

		return m_ptr;
	}

	void release()
	{
		cute_assert_dbg(m_counter);
		int i = m_counter->release();
		if (i == 0)
		{
			RP::release(m_ptr);
			m_ptr = 0;

			delete m_counter;
			m_counter = 0;
		}
	}

	SharedPtr(RC* pCounter, C* ptr) 
		: m_counter(pCounter), m_ptr(ptr)
	{
		cute_assert_dbg(m_counter);
		m_counter->duplicate();
	}

private:
	RC* m_counter;
	C*  m_ptr;

	template <class OtherC, class OtherRC, class OtherRP> friend class SharedPtr;
};


template <class C, class RC, class RP>
inline void swap(SharedPtr<C, RC, RP>& p1, SharedPtr<C, RC, RP>& p2)
{
	p1.swap(p2);
}

CUTE_NS_END