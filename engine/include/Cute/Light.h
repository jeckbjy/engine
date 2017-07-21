#pragma once
#include "Cute/Component.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Light : public Component
{
	DECLARE_RTTI(Light, Component, OID_LIGHT);
public:
	Light();
	~Light();

protected:

};

CUTE_NS_END
