#pragma once
#include "API.h"
#include "Array.h"

CU_NS_BEGIN

typedef Array<float> AnimValue;
class CU_API Animatable
{
public:
	Animatable(){}
	virtual ~Animatable(){}

	virtual void setValue(const String& name, const AnimValue& values, float weight = 1.0f){}
};

CU_NS_END

