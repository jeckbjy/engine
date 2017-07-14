#pragma once
#include "Cute/Object.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Component : public Object
{
	DECLARE_RTTI(Component, Object, OID_COMPONENT);
public:
	Component();
	~Component();

protected:

};

CUTE_NS_END
