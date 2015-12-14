#pragma once
#include "Object.h"

CU_NS_BEGIN

// node??
class CU_API Component : public Object
{
	DECLARE_RTTI(Component, Object, "COMP")
public:
	Component();
	~Component();

protected:
	bool m_enable;
};

CU_NS_END