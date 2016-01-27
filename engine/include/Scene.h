#pragma once
#include "API.h"
#include "Transform.h"

CU_NS_BEGIN

// 场景中包含各种物件，地形等
class CU_API Scene :public Transform
{
public:
	Scene();
	~Scene();

	void attach(Component* component);
	void detach(Component* component);
};

CU_NS_END
