#pragma once
#include "Vector3.h"

CU_NS_BEGIN

class CU_API Ray
{
public:
	Ray();
	Ray(const Vector3& origin, const Vector3& direction);

	const Vector3& getOrigin() const { return m_origin; }
	const Vector3& getDirection() const { return m_direction; }

	void setOrigin(const Vector3& origin) { m_origin = origin; }
	void setOrigin(float x, float y, float z) { m_origin.set(x, y, z); }
	void setDirection(const Vector3& dir);
	void setDirection(float x, float y, float z);
	void set(const Vector3& origin, const Vector3& dir);

private:
	Vector3 m_origin;
	Vector3 m_direction;
};

CU_NS_END