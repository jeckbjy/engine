#pragma once
#include "API.h"

CU_NS_BEGIN

// {x,y,width, height}??
template<typename T>
struct CU_API TRect
{
	T x, y, width, height;
	TRect() :x(0), y(0), width(0), height(0){}
	TRect(T left, T top, T w, T h) :x(left), y(top), width(w), height(h){}
};

typedef TRect<float> Rect;
typedef TRect<int>	 RectI;

CU_NS_END