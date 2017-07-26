#pragma once
#include "Cute/Ref.h"
//#include "Cute/RefPtr.h"
#include "Cute/RTTI.h"
#include "Cute/Declare.h"
#include "Cute/String.h"

CUTE_NS_BEGIN

/*
1:RTTI,Property
2:Ref
3:Signal/Slot
*/
class CUTE_CORE_API Object : public SafeRef<Object>
{
	DECLARE_RTTI(Object, RTTIRoot, OID_OBJECT)
public:
	Object();
	virtual ~Object();

	virtual String toString() const;

	bool isKindOf(const RTTI* rtti) const
	{
		return getRTTI()->isKindOf(rtti);
	}

	template<typename T>
	bool isKindOf() const
	{
		return isKindOf(T::getStaticRTTI());
	}

	template<typename T>
	T* cast() const
	{
		return isKindOf<T>() ? (T*)(this) : NULL;
	}
};

template<typename TYPE, int ID = OID_ANY>
class TObject : public Object
{
    DECLARE_RTTI(TYPE, Object, ID)
//public:
//    typedef RefPtr<TYPE> Ptr;
};

CUTE_NS_END
