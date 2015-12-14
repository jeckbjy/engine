#pragma once
#include "Component.h"

CU_NS_BEGIN

class CU_API Drawable : public Component
{
	DECLARE_RTTI(Drawable, Component, "DRAW")

public:
	Drawable();
	~Drawable();

private:

};

CU_NS_END