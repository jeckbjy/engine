#pragma once
#include "Object.h"
#include "Graphics.h"

CU_NS_BEGIN

// �߲���Ⱦ,ȫ��Ψһ,TBR(tile-based render) or IMR()
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
