#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"

CU_NS_BEGIN

class CU_API Transform
{
public:
	Transform();
	~Transform();

protected:
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
};

CU_NS_END