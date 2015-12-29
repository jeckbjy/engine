#pragma once
#include "API.h"
#ifdef CU_OS_APPLE
#include <libkern/OSAtomic.h>
#endif

CU_NS_BEGIN

// todo£º
//class CU_API AtomicFlag
//{
//public:
//#ifdef CU_OS_WIN
//	typedef LONG	type_t;
//#elif defined(CU_OS_MAC)
//	typedef int32_t type_t;
//#else
//	typedef bool	type_t;
//#endif
//	AtomicFlag():m_flag(0){}
//	AtomicFlag(bool flag) :m_flag(flag){}
//	bool test_and_set();
//	void clear();
//private:
//	volatile type_t m_flag;
//};

// Ô­×Ó²Ù×÷
class CU_API Atomic
{
public:
#ifdef CU_OS_WIN
	typedef LONG type_t;
#elif defined(CU_OS_MAC)
	typedef int32_t type_t;
#else
	typedef int type_t;
#endif

	Atomic() :m_counter(0){}
	Atomic(type_t t) :m_counter(t){}
	~Atomic(){}

	Atomic(const Atomic& other) :m_counter(other.m_counter){}
	Atomic& operator=(const Atomic& other);
	Atomic& operator=(type_t value);

	type_t inc();
	type_t dec();
	type_t test_and_set(type_t data);

	type_t operator ++();
	type_t operator ++(int);
	type_t operator --();
	type_t operator --(int);

	inline type_t value() const { return m_counter; }
	inline operator type_t() const{ return m_counter; }
	inline bool operator !() const { return m_counter == 0; }

private:
	volatile type_t m_counter;
};

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
#if defined(CU_OS_WIN)

#define CU_ATOMIC_SET(ptr, v)				InterlockedExchange(ptr, v)
#define CU_ATOMIC_INC(ptr)					InterlockedIncrement(ptr)
#define CU_ATOMIC_DEC(ptr)					InterlockedDecrement(ptr)
#define CU_ATOMIC_TEST_AND_SET(ptr, v)		InterlockedCompareExchange(ptr, v, *ptr)

#elif defined(CU_OS_APPLE)

#define CU_ATOMIC_SET(ptr, v)				OSAtomicTestAndSet(v, ptr)
#define CU_ATOMIC_INC(ptr)					OSAtomicIncrement32(ptr)
#define CU_ATOMIC_DEC(ptr)					OSAtomicDecrement32(ptr)
#define CU_ATOMIC_TEST_AND_SET(ptr, v)		OSAtomicTestAndSet(v, ptr)

#elif defined(CU_OS_POSIX)

#define CU_ATOMIC_SET(ptr, v)				__sync_lock_test_and_set(ptr, v)
#define CU_ATOMIC_INC(ptr)					__sync_add_and_fetch(ptr, 1)
#define CU_ATOMIC_DEC(ptr)					__sync_sub_and_fetch(ptr, 1)
#define CU_ATOMIC_TEST_AND_SET(ptr, v)		__sync_lock_test_and_set(ptr, v)

#endif

inline Atomic& Atomic::operator =(const Atomic& other)
{
	CU_ATOMIC_SET(&m_counter, other.value());
	return *this;
}

inline Atomic& Atomic::operator=(Atomic::type_t value)
{
	CU_ATOMIC_SET(&m_counter, value);
	return *this;
}

inline Atomic::type_t Atomic::inc()
{
	return CU_ATOMIC_INC(&m_counter);
}

inline Atomic::type_t Atomic::dec()
{
	return CU_ATOMIC_DEC(&m_counter);
}

inline Atomic::type_t Atomic::test_and_set(type_t data)
{
	return CU_ATOMIC_TEST_AND_SET(&m_counter, data);
}

inline Atomic::type_t Atomic::operator++()
{
	return CU_ATOMIC_INC(&m_counter);
}

inline Atomic::type_t Atomic::operator++(int)
{
	type_t result = CU_ATOMIC_INC(&m_counter);
	return --result;
}

inline Atomic::type_t Atomic::operator--()
{
	return CU_ATOMIC_DEC(&m_counter);
}

inline Atomic::type_t Atomic::operator--(int)
{
	type_t result = CU_ATOMIC_DEC(&m_counter);
	return ++result;
}

#undef CU_ATOMIC_SET
#undef CU_ATOMIC_INC
#undef CU_ATOMIC_DEC
#undef CU_ATOMIC_TEST_AND_SET

CU_NS_END