#pragma once
#include "Cute/Ref.h"
#include "Cute/RTTI.h"
#include "Cute/PreDeclare.h"

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

CUTE_NS_END
