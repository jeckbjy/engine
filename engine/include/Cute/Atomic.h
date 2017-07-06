#pragma once
#include "Cute/Types.h"
#include "Cute/CompilerSupport.h"

CUTE_NS_BEGIN

// just int32 and int64,todo:support for int8,int16 and so on
// https://developer.apple.com/legacy/library/documentation/Darwin/Reference/ManPages/man3/OSAtomicCompareAndSwap32.3.html
// https://gcc.gnu.org/onlinedocs/gcc-4.4.5/gcc/Atomic-Builtins.html
// https://msdn.microsoft.com/zh-cn/library/2ddez55b(v=vs.140).aspx
template<typename Type>
class Atomic
{
public:
    Atomic() noexcept : m_value(0){}
    Atomic(const Type value) noexcept : m_value(value){}
    Atomic(const Atomic& other) noexcept : m_value(other.m_value){}
    ~Atomic() noexcept {}
    
    Type get() const noexcept;
    void set(Type value) noexcept { exchange(value); }
    Type exchange(Type value) noexcept;
    
    bool compareAndSetBool  (Type newValue, Type cmpValue) noexcept;
    Type compareAndSetValue (Type newValue, Type cmpValue) noexcept;
    
    Atomic& operator=(const Atomic& other) noexcept { exchange(other.get()); return *this; }
    Atomic& operator=(const Type value) noexcept    { exchange(value);       return *this; }
    Type operator += (Type value) noexcept;
    Type operator -= (Type value) noexcept;
    Type operator ++ () noexcept;
    Type operator -- () noexcept;
    // better to just use pre-increment with atomics..
    Type operator ++ (int) noexcept;
    Type operator -- (int) noexcept;
    
    bool operator !() const noexcept { return get() == 0; }
    operator Type  () const noexcept { return get(); }

    /** Implements a memory read/write barrier. */
    static void memoryBarrier() noexcept;

private:
#if defined(CUTE_BIT64)
    CUTE_ALIGN(8)
#else
    CUTE_ALIGN(4)
#endif
    volatile Type m_value;
};

#include "Atomic.inl"

//typedef Atomic<int>   AtomicInt;
//typedef Atomic<int8>  Atomic8;
//typedef Atomic<int16> Atomic16;
typedef Atomic<int32> Atomic32;
typedef Atomic<int64> Atomic64;

CUTE_NS_END


//#if defined(CUTE_OS_MAC)
//#include <libkern/OSAtomic.h>
//#endif
//
//CUTE_NS_BEGIN
//
//#if   defined(CUTE_OS_FAMILY_WINDOWS)
//typedef LONG	atomic_t;
//#elif defined(CUTE_OS_MAC)
//typedef int32_t atomic_t;
//#else
//typedef int		atomic_t;
//#endif
//
//// 原子操作
//class CUTE_CORE_API Atomic
//{
//public:
//	Atomic() :m_counter(0){}
//	Atomic(atomic_t t) :m_counter(t){}
//	~Atomic(){}
//
//	Atomic(const Atomic& other) :m_counter(other.m_counter){}
//	Atomic& operator=(const Atomic& other);
//	Atomic& operator=(atomic_t value);
//
//	atomic_t inc();
//	atomic_t dec();
//	atomic_t test_and_set(atomic_t data);
//
//	atomic_t operator ++();
//	atomic_t operator ++(int);
//	atomic_t operator --();
//	atomic_t operator --(int);
//
//	inline atomic_t value() const { return m_counter; }
//	inline operator atomic_t() const{ return m_counter; }
//	inline bool operator !() const { return m_counter == 0; }
//
//private:
//	volatile atomic_t m_counter;
//};
//
////////////////////////////////////////////////////////////////////////////
//// inline
////////////////////////////////////////////////////////////////////////////
//#if defined(CUTE_OS_FAMILY_WINDOWS)
//
//#define CUTE_ATOMIC_SET(ptr, v)				InterlockedExchange(ptr, v)
//#define CUTE_ATOMIC_INC(ptr)				InterlockedIncrement(ptr)
//#define CUTE_ATOMIC_DEC(ptr)				InterlockedDecrement(ptr)
//#define CUTE_ATOMIC_TEST_AND_SET(ptr, v)	InterlockedCompareExchange(ptr, v, *ptr)
//
//#elif defined(CUTE_OS_MAC)
//
//#define CUTE_ATOMIC_SET(ptr, v)				OSAtomicTestAndSet(v, ptr)
//#define CUTE_ATOMIC_INC(ptr)				OSAtomicIncrement32(ptr)
//#define CUTE_ATOMIC_DEC(ptr)				OSAtomicDecrement32(ptr)
//#define CUTE_ATOMIC_TEST_AND_SET(ptr, v)	OSAtomicTestAndSet(v, ptr)
//
//#elif defined(CUTE_OS_FAMILY_UNIX)
//
//#define CUTE_ATOMIC_SET(ptr, v)				__sync_lock_test_and_set(ptr, v)
//#define CUTE_ATOMIC_INC(ptr)				__sync_add_and_fetch(ptr, 1)
//#define CUTE_ATOMIC_DEC(ptr)				__sync_sub_and_fetch(ptr, 1)
//#define CUTE_ATOMIC_TEST_AND_SET(ptr, v)	__sync_lock_test_and_set(ptr, v)
//
//#endif
//
//inline Atomic& Atomic::operator =(const Atomic& other)
//{
//	CUTE_ATOMIC_SET(&m_counter, other.value());
//	return *this;
//}
//
//inline Atomic& Atomic::operator=(atomic_t value)
//{
//	CUTE_ATOMIC_SET(&m_counter, value);
//	return *this;
//}
//
//inline atomic_t Atomic::inc()
//{
//	return CUTE_ATOMIC_INC(&m_counter);
//}
//
//inline atomic_t Atomic::dec()
//{
//	return CUTE_ATOMIC_DEC(&m_counter);
//}
//
//inline atomic_t Atomic::test_and_set(atomic_t data)
//{
//	return CUTE_ATOMIC_TEST_AND_SET(&m_counter, data);
//}
//
//inline atomic_t Atomic::operator++()
//{
//	return CUTE_ATOMIC_INC(&m_counter);
//}
//
//inline atomic_t Atomic::operator++(int)
//{
//	atomic_t result = CUTE_ATOMIC_INC(&m_counter);
//	return --result;
//}
//
//inline atomic_t Atomic::operator--()
//{
//	return CUTE_ATOMIC_DEC(&m_counter);
//}
//
//inline atomic_t Atomic::operator--(int)
//{
//	atomic_t result = CUTE_ATOMIC_DEC(&m_counter);
//	return ++result;
//}
//
//#undef CU_ATOMIC_SET
//#undef CU_ATOMIC_INC
//#undef CU_ATOMIC_DEC
//#undef CU_ATOMIC_TEST_AND_SET
//
//CUTE_NS_END
