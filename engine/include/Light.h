#pragma once
#include "Component.h"
#include "Vector3.h"

CU_NS_BEGIN

class CU_API Light : public Component, public ListNode<Light, 2>
{
	DECLARE_RTTI(Light, Component, TYPE_LIGHT);
public:
	enum Type
	{
		UNKNOWN,
		DIRECTIONAL,
		POINT,
		SPOT
	};

	Light();
	~Light();

protected:
	Vector3 m_color;
};

CU_NS_END
