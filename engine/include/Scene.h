#pragma once
#include "API.h"
#include "Transform.h"

CU_NS_BEGIN

// �����а���������������ε�
class CU_API Scene :public Transform
{
public:
	Scene();
	~Scene();

	void attach(Component* component);
	void detach(Component* component);
};

CU_NS_END
