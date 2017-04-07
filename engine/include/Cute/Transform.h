#pragma once
#include "Cute/Component.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Transform : public Component
{
	DECLARE_RTTI(Transform, Component, OID_TRANSFORM)
public:
	Transform();
	~Transform();

protected:
};

CUTE_NS_END
