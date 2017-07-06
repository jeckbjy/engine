//! Core

#if   defined(CUTE_OS_MAC) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 2))
#   include <libkern/OSAtomic.h>
// Older OSX builds using gcc4.1 or earlier,mac just has compare and bool,no for val
#   define CUTE_ATOMIC_MAC  1
#   define CUTE_ATOMIC_MEMORY_BARRIER()         OSMemoryBarrier()

//#   define CUTE_ATOMIC_SET32(ptr, v)            OSAtomicTestAndSet(v, ptr)
#   define CUTE_ATOMIC_GET32(ptr)               OSAtomicAdd32Barrier((int32_t) 0, (volatile int32_t*)ptr)
#   define CUTE_ATOMIC_INC32(ptr)               OSAtomicIncrement32Barrier ((volatile int32_t*)ptr)
#   define CUTE_ATOMIC_DEC32(ptr)               OSAtomicDecrement32Barrier((volatile int32_t*)ptr)
#   define CUTE_ATOMIC_ADD32(ptr, v)            OSAtomicAdd32Barrier(v,(volatile int32_t*)ptr)
#   define CUTE_ATOMIC_CMP_BOOL32(ptr, v, cmp)  OSAtomicCompareAndSwap32Barrier(cmp, v, ptr)
#   define CUTE_ATOMIC_CMP_VAL32(ptr, v, cmp)   OSAtomicCompareAndSwap32Barrier(cmp, v, ptr)

//#   define CUTE_ATOMIC_SET64(ptr, v)            OSAtomicTestAndSet(v, ptr)
#   define CUTE_ATOMIC_GET64(ptr)               OSAtomicAdd64Barrier((int32_t) 0, (volatile int32_t*)ptr)
#   define CUTE_ATOMIC_INC64(ptr)               OSAtomicIncrement64Barrier ((volatile int32_t*)ptr)
#   define CUTE_ATOMIC_DEC64(ptr)               OSAtomicDecrement64Barrier((volatile int32_t*)ptr)
#   define CUTE_ATOMIC_ADD64(ptr, v)            OSAtomicAdd64Barrier(v,(volatile int32_t*)ptr)
#   define CUTE_ATOMIC_CMP_BOOL64(ptr, v, cmp)  OSAtomicCompareAndSwap64Barrier(cmp, v, ptr)
#   define CUTE_ATOMIC_CMP_VAL64(ptr, v, cmp)   OSAtomicCompareAndSwap64Barrier(cmp, v, ptr)

//#   define CUTE_ATOMIC_TEST_AND_SET(ptr, v)   OSAtomicTestAndSet(v, ptr)

#elif defined(CUTE_COMPILER_GCC) || defined(CUTE_COMPILER_CLANG)
#   define CUTE_ATOMIC_GCC  1
#   define CUTE_ATOMIC_MEMORY_BARRIER()         __sync_synchronize()

#   define CUTE_ATOMIC_GET32(ptr)               __sync_add_and_fetch((volatile int32*)ptr, 0)
#   define CUTE_ATOMIC_INC32(ptr)               __sync_add_and_fetch((volatile int32*)ptr, 1)
#   define CUTE_ATOMIC_DEC32(ptr)               __sync_sub_and_fetch((volatile int32*)ptr, -1)
#   define CUTE_ATOMIC_ADD32(ptr, v)            __sync_add_and_fetch((volatile int32*)ptr, v)
#   define CUTE_ATOMIC_CMP_BOOL32(ptr, v, cmp)  __sync_bool_compare_and_swap((volatile int32*)ptr, v, cmp)
#   define CUTE_ATOMIC_CMP_VAL32(ptr, v, cmp)   __sync_val_compare_and_swap((volatile int32*)ptr, v, cmp)

#   define CUTE_ATOMIC_GET64(ptr)               __sync_add_and_fetch((volatile int64*)ptr, 0)
#   define CUTE_ATOMIC_INC64(ptr)               __sync_add_and_fetch((volatile int64*)ptr, 1)
#   define CUTE_ATOMIC_DEC64(ptr)               __sync_sub_and_fetch((volatile int64*)ptr, -1)
#   define CUTE_ATOMIC_ADD64(ptr, v)            __sync_add_and_fetch((volatile int64*)ptr, v)
#   define CUTE_ATOMIC_CMP_BOOL64(ptr, v, cmp)  __sync_bool_compare_and_swap((volatile int64*)ptr, v, cmp)
#   define CUTE_ATOMIC_CMP_VAL64(ptr, v, cmp)   __sync_val_compare_and_swap((volatile int64*)ptr, v, cmp)

//#   define CUTE_ATOMIC_TEST_AND_SET(ptr, v) __sync_lock_test_and_set(ptr, v)

#elif defined(CUTE_OS_FAMILY_WINDOWS)
#   define CUTE_ATOMIC_MEMORY_BARRIER()         _ReadWriteBarrier()

#   define CUTE_ATOMIC_INC32(ptr)               _InterlockedIncrement(ptr)
#   define CUTE_ATOMIC_DEC32(ptr)               _InterlockedDecrement(ptr)
#   define CUTE_ATOMIC_ADD32(ptr, v)            _InterlockedExchangeAdd(ptr, v)
#   define CUTE_ATOMIC_EXCHANGE32(ptr, v)       _InterlockedExchange(ptr, v)
#   define CUTE_ATOMIC_CMP_BOOL32(ptr, v, cmp)  _InterlockedCompareExchange(ptr, v, cmp) == cmp
#   define CUTE_ATOMIC_CMP_VAL32(ptr, v, cmp)   _InterlockedCompareExchange(ptr, v, cmp)

#   define CUTE_ATOMIC_INC64(ptr)               _InterlockedIncrement64(ptr)
#   define CUTE_ATOMIC_DEC64(ptr)               _InterlockedDecrement64(ptr)
#   define CUTE_ATOMIC_ADD64(ptr, v)            _InterlockedExchangeAdd64(ptr, v)
#   define CUTE_ATOMIC_EXCHANGE64(ptr, v)       _InterlockedExchange64(ptr, v)
#   define CUTE_ATOMIC_CMP_BOOL64(ptr, v, cmp)  _InterlockedCompareExchange64(ptr, v, cmp) == cmp
#   define CUTE_ATOMIC_CMP_VAL64(ptr, v, cmp)   _InterlockedCompareExchange64(ptr, v, cmp)

//#   define CUTE_ATOMIC_TEST_AND_SET(ptr, v)    InterlockedCompareExchange(ptr, v, *ptr)

#endif

template<typename Type, std::size_t SIZE>
struct AtomicTraitsBase
{
//    inline static int32 get(uint32_t* ptr)
};

template<typename Type>
struct AtomicTraitsBase<Type, 4>
{
    inline static Type exchange(volatile Type* value, Type other) noexcept
    {
        return CUTE_ATOMIC_EXCHANGE32(value, other);
    }

    inline static Type get(const volatile Type* value) noexcept
    {
        return CUTE_ATOMIC_GET32(value);
    }
    
    inline static Type inc(volatile Type* value) noexcept
    {
        return CUTE_ATOMIC_INC32(value);
    }
    
    inline static Type dec(volatile Type* value) noexcept
    {
        return CUTE_ATOMIC_DEC32(value);
    }
    
    inline static Type add(volatile Type* value, Type other) noexcept
    {
        return CUTE_ATOMIC_ADD32(value, other);
    }

    inline static Type cmp_val(volatile Type* value, Type other, Type compared) noexcept
    {
        return CUTE_ATOMIC_CMP_VAL32(value, other, compared);
    }

    inline static bool cmp_bool(volatile Type* value, Type other, Type compared) noexcept
    {
        return CUTE_ATOMIC_CMP_BOOL32(value, other, compared);
    }
};

template<typename Type>
struct AtomicTraitsBase<Type, 8>
{
    inline static Type exchange(volatile Type* value, Type other) noexcept
    {
        return CUTE_ATOMIC_EXCHANGE64(value, other);
    }

    inline static Type get(const volatile Type* value) noexcept
    {
        return CUTE_ATOMIC_GET64(value);
    }
    
    inline static Type inc(volatile Type* value) noexcept
    {
        return CUTE_ATOMIC_INC64(value);
    }
    
    inline static Type dec(volatile Type* value) noexcept
    {
        return CUTE_ATOMIC_GET64(value);
    }
    
    inline static Type add(volatile Type* value, Type other) noexcept
    {
        return CUTE_ATOMIC_ADD64(value, other);
    }

    inline static Type cmp_val(volatile Type* value, Type other, Type compared) noexcept
    {
        return CUTE_ATOMIC_CMP_VAL64(value, other, compared);
    }

    inline static bool cmp_bool(volatile Type* value, Type other, Type compared) noexcept
    {
        return CUTE_ATOMIC_CMP_BOOL64(value, other, compared);
    }
};

template <typename Type>
struct AtomicTraits : public AtomicTraitsBase<Type, sizeof(Type)>
{
};

//===============
// inline impl

template <typename Type>
inline void Atomic<Type>::memoryBarrier() noexcept
{
    CUTE_ATOMIC_MEMORY_BARRIER();
}

template <typename Type>
inline Type Atomic<Type>::get() const noexcept
{
    return AtomicTraits<Type>::get(&m_value);
}

template <typename Type>
inline Type Atomic<Type>::exchange(Type value) noexcept
{
#if CUTE_ATOMIC_MAC || CUTE_ATOMIC_GCC
    Type current = m_value;
    while (! compareAndSetBool (value, current)) { current = m_value; }
    return current;
#else
    return AtomicTraits<Type>::exchange(&m_value, value);
#endif
}

template <typename Type>
inline bool Atomic<Type>::compareAndSetBool(Type newValue, Type cmpValue) noexcept
{
    return AtomicTraits<Type>::cmp_bool(&m_value, newValue, cmpValue);
}

template <typename Type>
inline Type Atomic<Type>::compareAndSetValue(Type newValue, Type cmpValue) noexcept
{
#if defined(CUTE_ATOMIC_MAC)
    for (;;) // Annoying workaround for only having a bool CAS operation..
    {
        if (compareAndSetBool (newValue, valueToCompare))
            return valueToCompare;

        const Type result = value;
        if (result != valueToCompare)
            return result;
    }
#else
    return AtomicTraits<Type>::cmp_val(&m_value, newValue, cmpValue);
#endif
}


template <typename Type>
inline Type Atomic<Type>::operator +=(Type value) noexcept
{
    return AtomicTraits<Type>::add(&m_value, value);
}

template <typename Type>
inline Type Atomic<Type>::operator-=(Type value) noexcept
{
    return AtomicTraits<Type>::add(&m_value, -value);
}

template <typename Type>
inline Type Atomic<Type>::operator++() noexcept
{
    return AtomicTraits<Type>::inc(&m_value);
}

template <typename Type>
inline Type Atomic<Type>::operator--() noexcept
{
    return AtomicTraits<Type>::dec(&m_value);
}

template <typename Type>
inline Type Atomic<Type>::operator++(int) noexcept
{
    Type result = AtomicTraits<Type>::inc(&m_value);
    return --result;
}

template <typename Type>
Type Atomic<Type>::operator--(int) noexcept
{
    Type result = AtomicTraits<Type>::dec(&m_value);
    return ++result;
}
