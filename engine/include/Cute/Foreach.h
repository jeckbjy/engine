#pragma once
#include "Cute/Platform.h"

CUTE_NS_BEGIN

#if defined(foreach)
#undefine foreach
#endif

#if defined(CUTE_CPP11)
#define foreach(item, container) for (item : container)
#else
// for none c++11
// http://cplusplus.bordoon.com/boost_foreach_techniques.html
///////////////////////////////////////////////////////////////////////////////
namespace detail{
// base type declare
struct auto_base
{
    operator bool() const { return false; }
};

template<typename T>
struct auto_any : auto_base
{
    auto_any(T const& t):item(t){}
    mutable T item;
};

template<typename T>
T& auto_cast(auto_base const& any)
{
    return static_cast<auto_any<T> const&>(any).item;
}

///////////////////////////////////////////////////////////////////////////////
// FOREACH helper function
template<typename T>
auto_any<typename T::const_iterator> begin(const T& t)
{
    return t.begin();
}

template<typename T>
auto_any<typename T::const_iterator> end(const T& t)
{
    return t.end();
}

template<typename T>
bool fail(const auto_base& cur, const auto_base& end, T&)
{
    typedef typename T::iterator iter_type;
    return auto_cast<iter_type>(cur) != auto_cast<iter_type>(end);
}

template<typename T>
void next(const auto_base& cur, T&)
{
    ++auto_cast<typename T::iterator>(cur);
}

template<typename T>
typename T::reference deref(const auto_base& cur, T&)
{
    return *auto_cast<typename T::iterator>(cur);
}

template<typename T>
typename T::const_reference deref(const auto_base& cur, const T&)
{
    return *auto_cast<typename T::iterator>(cur);
}

} // end detail

// not all compiler support if with init-statement;need test in old complier!!
#define foreach(item, container)    \
    for(Cute::detail::auto_base const &cur = Cute::detail::begin(container), &end = Cute::detail::end(container); \
        Cute::detail::fail(cur, end, container);            \
        Cute::detail::next(cur, container))                 \
        if ( item = Cute::detail::deref(cur, container))

// inner for loop,but cannot use break!!
#define foreach_nobreak(item, container)    \
    for(Cute::detail::auto_base const &cur = Cute::detail::begin(container), &end = Cute::detail::end(container); \
        Cute::detail::fail(cur, end, container);            \
        Cute::detail::next(cur, container))                 \
        if (bool ugly_and_unique_break = false) {} else     \
        for(item = Cute::detail::deref(cur, container); !ugly_and_unique_break; ugly_and_unique_break = false) \

#endif

CUTE_NS_END
