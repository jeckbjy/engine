#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct TypeTraitBase
{
	typedef T type_t;
};

template<typename T>
struct TypeTraitBase<T&>
{
	typedef T type_t;
};

template<typename T>
struct TypeTraitBase<T*>
{
	typedef T type_t;
};

template<typename T>
struct TypeTrait : public TypeTraitBase<typename std::remove_cv<T>::type>
{
	typedef type_t*				pointer_t;
	typedef type_t&				reference_t;
	typedef const type_t		const_type_t;
	typedef const pointer_t		const_pointer_t;
	typedef const reference_t	const_reference_t;
};

// 对于没有实现c++11的编译器
template<bool _Test, class T = void>
struct EnableIf { };

template<class T>
struct EnableIf<true, T> { typedef T type; };

template<bool _Test, class T = void>
struct DisableIf { };
template<class T>
struct DisableIf<false, T> { typedef T type; };

template<bool _Test, class T1, class T2>
struct Conditional { typedef T2 type; };

template<class T1, class T2>
struct Conditional<true, T1, T2> { typedef T1 type; };

struct TrueType
{
	enum
	{
		VALUE = 1
	};
};

struct FalseType
{
	enum
	{
		VALUE = 0
	};
};

/// Use this struct to determine if a template type is a reference.
template <typename T>
struct IsReference : public FalseType { };

template <typename T>
struct IsReference<T&> : public TrueType { };

template <typename T>
struct IsReference<const T&> : public TrueType { };

/// Use this struct to determine if a template type is a const type.
template <typename T>
struct IsConst : public FalseType{};

template <typename T>
struct IsConst<const T&> : public TrueType{ };

template <typename T>
struct IsConst<const T> : public TrueType { };

/// Specialization for const char arrays
template <typename T, int i>
struct IsConst<const T[i]> : public TrueType { };

template<typename T>
struct IsFloat : public FalseType { };

template<>
struct IsFloat<float> : public TrueType{ };

template<>
struct IsFloat<double> : public TrueType{ };

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
template<typename T> 
struct _IsInteger : public FalseType {};

template<> struct _IsInteger<char>				: public TrueType{};
template<> struct _IsInteger<signed char>		: public TrueType{};
template<> struct _IsInteger<unsigned char>		: public TrueType{};
template<> struct _IsInteger<signed short>		: public TrueType{};
template<> struct _IsInteger<unsigned short>	: public TrueType{};
template<> struct _IsInteger<signed int>		: public TrueType{};
template<> struct _IsInteger<unsigned int>		: public TrueType{};
template<> struct _IsInteger<signed long>		: public TrueType{};
template<> struct _IsInteger<unsigned long>		: public TrueType{};
template<> struct _IsInteger<signed long long>	: public TrueType{};
template<> struct _IsInteger<unsigned long long>: public TrueType{};

template<typename T>
struct IsInteger : public _IsInteger<typename std::remove_cv<T>::type>
{
};

//////////////////////////////////////////////////////////////////////////
// 获得地址
//////////////////////////////////////////////////////////////////////////

// 获得真实的地址,避免T重载了&操作符
// Get an object address without triggering operator & overloading
template<typename T>
inline T* getAddress(T& obj)
{
	return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(obj)));
}

// 通过全局指针或成员指针获取真实指针 Address<>::get
template<typename T>
struct FieldAddress{};

template<typename T>
struct FieldAddress<T*> { static inline T* get(T* field, void*) { return field; } };

template<typename O, typename T>
struct FieldAddress<T O::*> { static inline T* get(T* field, void* obj){ return &(static_cast<O*>(obj)->*(field)); } };

//////////////////////////////////////////////////////////////////////////
// 成员指针转化(地址偏移)转化为偏移
//////////////////////////////////////////////////////////////////////////
template<class Parent, class Member>
inline std::ptrdiff_t offset_from_pointer_to_member(const Member Parent::* ptr_to_member)
{
#if defined(_MSC_VER) || (defined(_WIN32) && defined(CUTE_COMPILER_INTEL))
	//MSVC compliant compilers use their the first 32 bits as offset (even in 64 bit mode)
	union caster_union
	{
		const Member Parent::* ptr_to_member;
		int32_t offset;
	} caster;

	//MSVC ABI can use up to 3 int32 to represent pointer to member data
	//with virtual base classes, in those cases there is no simple to
	//obtain the address of the parent. So static assert to avoid runtime errors
	assert(sizeof(caster) == sizeof(int32_t));

	caster.ptr_to_member = ptr_to_member;
	return std::ptrdiff_t(caster.offset);

#elif defined(__GNUC__)|| defined(__HP_aCC) || defined(CUTE_COMPILER_INTEL) || defined(__IBMCPP__) || defined(__DECCXX)
	//Additional info on MSVC behaviour for the future. For 2/3 int ptr-to-member 
	//types dereference seems to be:
	//
	// vboffset = [compile_time_offset if 2-int ptr2memb] /
	//            [ptr2memb.i32[2] if 3-int ptr2memb].
	// vbtable = *(this + vboffset);
	// adj = vbtable[ptr2memb.i32[1]];
	// var = adj + (this + vboffset) + ptr2memb.i32[0];
	//
	//To reverse the operation we need to
	// - obtain vboffset (in 2-int ptr2memb implementation only)
	// - Go to Parent's vbtable and obtain adjustment at index ptr2memb.i32[1]
	// - parent = member - adj - vboffset - ptr2memb.i32[0]
	//
	//Even accessing to RTTI we might not be able to obtain this information
	//so anyone who thinks it's possible, please send a patch.

	//This works with gcc, msvc, ac++, ibmcpp
	const Parent * const parent = 0;
	const char *const member = static_cast<const char*>(static_cast<const void*>(&(parent->*ptr_to_member)));
	return std::ptrdiff_t(member - static_cast<const char*>(static_cast<const void*>(parent)));
#else
	//This is the traditional C-front approach: __MWERKS__, __DMC__, __SUNPRO_CC
	union caster_union
	{
		const Member Parent::* ptr_to_member;
		std::ptrdiff_t offset;
	} caster;
	caster.ptr_to_member = ptr_to_member;
	return caster.offset - 1;
#endif
}

template<class Parent, class Member>
inline Parent *parent_from_member(Member *member, const Member Parent::* ptr_to_member)
{
	return static_cast<Parent*>
		(
		static_cast<void*>
		(
		static_cast<char*>(static_cast<void*>(member)) - offset_from_pointer_to_member(ptr_to_member)
		)
		);
}

template<class Parent, class Member>
inline const Parent *parent_from_member(const Member *member, const Member Parent::* ptr_to_member)
{
	return static_cast<const Parent*>
		(
		static_cast<const void*>
		(
		static_cast<const char*>(static_cast<const void*>(member)) - offset_from_pointer_to_member(ptr_to_member)
		)
		);
}

CUTE_NS_END
