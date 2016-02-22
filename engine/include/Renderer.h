#pragma once
#include "Object.h"
#include "Graphics.h"

CU_NS_BEGIN

// 高层渲染,全局唯一,TBR(tile-based render) or IMR()
class CU_API Renderer : public Object
{
	DECLARE_RTTI(Renderer, Object, "REND")
public:
	Renderer();
	~Renderer();

	void update();

private:
};

CU_NS_END
