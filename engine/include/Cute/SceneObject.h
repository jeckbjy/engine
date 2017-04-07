#pragma once
#include "Cute/Object.h"

CUTE_NS_BEGIN

class CUTE_CORE_API SceneObject : public Object
{
	DECLARE_RTTI(SceneObject, Object, OID_SCENE_OBJECT)
public:
	SceneObject();
	~SceneObject();

protected:
};

CUTE_NS_END