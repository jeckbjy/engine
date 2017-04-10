#pragma once
#include "Cute/Platform.h"
#include "Cute/PreHeader.h"

#if CUTE_OS == CUTE_OS_MAC_OS_X
#include <libkern/OSAtomic.h>
#endif

CUTE_NS_BEGIN

#ifdef CUTE_OS_FAMILY_WINDOWS
typedef LONG	atomic_t;
#elif defined(CUTE_OS == CUTE_OS_MAC_OS_X)
typedef int32_t atomic_t;
#else
typedef int		atomic_t;
#endif

// Ô­×Ó²Ù×÷
class CUTE_CORE_API Atomic
{
public:
	Atomic() :m_counter(0){}
	Atomic(atomic_t t) :m_counter(t){}
	~Atomic(){}

	Atomic(const Atomic& other) :m_counter(other.m_counter){}
	Atomic& operator=(const Atomic& other);
	Atomic& operator=(atomic_t value);

	atomic_t inc();
	atomic_t dec();
	atomic_t test_and_set(atomic_t data);

	atomic_t operator ++();
	atomic_t operator ++(int);
	atomic_t operator --();
	atomic_t operator --(int);

	inline atomic_t value() const { return m_counter; }
	inline operator atomic_t() const{ return m_counter; }
	inline bool operator !() const { return m_counter == 0; }

private:
	volatile atomic_t m_counter;
};

//////////////////////////////////////////////////////////////////////////
// inline
//////////////////////////////////////////////////////////////////////////
#if defined(CUTE_OS_FAMILY_WINDOWS)

#define CUTE_ATOMIC_SET(ptr, v)				InterlockedExchange(ptr, v)
#define CUTE_ATOMIC_INC(ptr)				InterlockedIncrement(ptr)
#define CUTE_ATOMIC_DEC(ptr)				InterlockedDecrement(ptr)
#define CUTE_ATOMIC_TEST_AND_SET(ptr, v)	InterlockedCompareExchange(ptr, v, *ptr)

#elif defined(CUTE_OS == CUTE_OS_MAC_OS_X)

#define CUTE_ATOMIC_SET(ptr, v)				OSAtomicTestAndSet(v, ptr)
#define CUTE_ATOMIC_INC(ptr)				OSAtomicIncrement32(ptr)
#define CUTE_ATOMIC_DEC(ptr)				OSAtomicDecrement32(ptr)
#define CUTE_ATOMIC_TEST_AND_SET(ptr, v)	OSAtomicTestAndSet(v, ptr)

#elif defined(CUTE_OS_FAMILY_UNIX)

#define CUTE_ATOMIC_SET(ptr, v)				__sync_lock_test_and_set(ptr, v)
#define CUTE_ATOMIC_INC(ptr)				__sync_add_and_fetch(ptr, 1)
#define CUTE_ATOMIC_DEC(ptr)				__sync_sub_and_fetch(ptr, 1)
#define CUTE_ATOMIC_TEST_AND_SET(ptr, v)	__sync_lock_test_and_set(ptr, v)

#endif

inline Atomic& Atomic::operator =(const Atomic& other)
{
	CUTE_ATOMIC_SET(&m_counter, other.value());
	return *this;
}

inline Atomic& Atomic::operator=(atomic_t value)
{
	CUTE_ATOMIC_SET(&m_counter, value);
	return *this;
}

inline atomic_t Atomic::inc()
{
	return CUTE_ATOMIC_INC(&m_counter);
}

inline atomic_t Atomic::dec()
{
	return CUTE_ATOMIC_DEC(&m_counter);
}

inline atomic_t Atomic::test_and_set(atomic_t data)
{
	return CUTE_ATOMIC_TEST_AND_SET(&m_counter, data);
}

inline atomic_t Atomic::operator++()
{
	return CUTE_ATOMIC_INC(&m_counter);
}

inline atomic_t Atomic::operator++(int)
{
	atomic_t result = CUTE_ATOMIC_INC(&m_counter);
	return --result;
}

inline atomic_t Atomic::operator--()
{
	return CUTE_ATOMIC_DEC(&m_counter);
}

inline atomic_t Atomic::operator--(int)
{
	atomic_t result = CUTE_ATOMIC_DEC(&m_counter);
	return ++result;
}

#undef CU_ATOMIC_SET
#undef CU_ATOMIC_INC
#undef CU_ATOMIC_DEC
#undef CU_ATOMIC_TEST_AND_SET

CUTE_NS_END