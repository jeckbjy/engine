#pragma once
#include "API.h"

CU_NS_BEGIN

// {x,y,width, height}??
template<typename T>
struct CU_API TRect
{
	T left, top, right, bottom;
	TRect() :left(0), top(0),right(0),bottom(0){}
};

typedef TRect<float> Rect;
typedef TRect<int>	 RectI;

CU_NS_END