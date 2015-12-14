#pragma once
#include "GpuBuffer.h"
#include "Texture.h"

CU_NS_BEGIN

// µ×²ãäÖÈ¾½Ó¿Ú
class CU_API Grahpics
{
public:
	virtual void draw() = 0;
	virtual void drawInstance() = 0;
	virtual void dispatch(int x, int y, int z) = 0;
};

CU_NS_END