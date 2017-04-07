#pragma once
#include "Cute/Component.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Camera : public Component
{
	DECLARE_RTTI(Camera, Object, OID_CAMERA)
public:
	Camera();
	~Camera();

protected:
};

CUTE_NS_END