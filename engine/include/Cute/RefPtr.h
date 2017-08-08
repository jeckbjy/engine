#pragma once
#include "Cute/Platform.h"
//#include "Cute/Foundation.h"
#include "Cute/Exception.h"

CUTE_NS_BEGIN

template <class C>
class RefPtr
{
public:
	RefPtr()
		: m_ptr(0)
	{
	}

	RefPtr(C* ptr)
		: m_ptr(ptr)
	{
//		if (m_ptr)
//			m_ptr->incRef();
	}

	RefPtr(const RefPtr& ptr)
		: m_ptr(ptr.m_ptr)
	{
		if (m_ptr)
			m_ptr->retain();
	}

	~RefPtr()
	{
		if (m_ptr)
			m_ptr->release();
	}

	RefPtr& assign(C* ptr)
	{
		if (m_ptr != ptr)
		{
			if (m_ptr) m_ptr->release();
			m_ptr = ptr;
			if (m_ptr) m_ptr->retain();
		}
		return *this;
	}

	RefPtr& assign(const RefPtr& ptr)
	{
		if (&ptr != this)
		{
			if (m_ptr) m_ptr->release();
			m_ptr = ptr.m_ptr;
			if (m_ptr) m_ptr->retain();
		}
		return *this;
	}

	RefPtr& operator = (C* ptr)
	{
		return assign(ptr);
	}

	RefPtr& operator = (const RefPtr& ptr)
	{
		return assign(ptr);
	}

	void swap(RefPtr& ptr)
	{
		std::swap(m_ptr, ptr.m_ptr);
	}

	/// Casts the AutoPtr via a dynamic cast to the given type.
	/// Returns an AutoPtr containing NULL if the cast fails.
	/// Example: (assume class Sub: public Super)
	///    RefPtr<Super> super(new Sub());
	///    RefPtr<Sub> sub = super.cast<Sub>();
	template <class Other>
	RefPtr<Other> cast() const
	{
		Other* pOther = static_cast<Other*>(m_ptr);
		return RefPtr<Other>(pOther);
	}

	C* operator -> ()
	{
		if (m_ptr)
			return m_ptr;
		else
			throw NullPointerException();
	}

	const C* operator -> () const
	{
		if (m_ptr)
			return m_ptr;
		else
			throw NullPointerException();
	}

	C& operator * ()
	{
		if (m_ptr)
			return *m_ptr;
		else
			throw NullPointerException();
	}

	const C& operator * () const
	{
		if (m_ptr)
			return *m_ptr;
		else
			throw NullPointerException();
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

	bool isOne() const
	{
		return m_ptr != 0 && m_ptr->isOne();
	}

	bool operator == (const RefPtr& ptr) const
	{
		return m_ptr == ptr.m_ptr;
	}

	//bool operator == (const C* ptr) const
	//{
	//	return m_ptr == ptr;
	//}

	//bool operator == (C* ptr) const
	//{
	//	return m_ptr == ptr;
	//}

	bool operator != (const RefPtr& ptr) const
	{
		return m_ptr != ptr.m_ptr;
	}

	bool operator != (const C* ptr) const
	{
		return m_ptr != ptr;
	}

	//bool operator != (C* ptr) const
	//{
	//	return m_ptr != ptr;
	//}

	bool operator < (const RefPtr& ptr) const
	{
		return m_ptr < ptr.m_ptr;
	}

	bool operator < (const C* ptr) const
	{
		return m_ptr < ptr;
	}

	//bool operator < (C* ptr) const
	//{
	//	return m_ptr < ptr;
	//}

	bool operator <= (const RefPtr& ptr) const
	{
		return m_ptr <= ptr.m_ptr;
	}

	bool operator <= (const C* ptr) const
	{
		return m_ptr <= ptr;
	}

	//bool operator <= (C* ptr) const
	//{
	//	return m_ptr <= ptr;
	//}

	bool operator > (const RefPtr& ptr) const
	{
		return m_ptr > ptr.m_ptr;
	}

	bool operator > (const C* ptr) const
	{
		return m_ptr > ptr;
	}

	//bool operator > (C* ptr) const
	//{
	//	return m_ptr > ptr;
	//}

	bool operator >= (const RefPtr& ptr) const
	{
		return m_ptr >= ptr.m_ptr;
	}

	bool operator >= (const C* ptr) const
	{
		return m_ptr >= ptr;
	}

	//bool operator >= (C* ptr) const
	//{
	//	return m_ptr >= ptr;
	//}

private:
	C* m_ptr;
};

template <class C>
inline void swap(RefPtr<C>& p1, RefPtr<C>& p2)
{
	p1.swap(p2);
}

CUTE_NS_END
