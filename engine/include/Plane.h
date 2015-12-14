#pragma once
#include "Vector3.h"

CU_NS_BEGIN

class Plane
{
public:
	Plane();
	Plane(const Vector3& normal, float dist);

	float distance(const Vector3& point) const;

	const Vector3& getNormal() const { return m_normal; }
	float getDistance() const { return m_distance; }

	void setNormal(const Vector3& normal);
	void setNormal(float x, float y, float z);
	void setDistance(float dist) { m_distance = dist; }
	void set(const Vector3& normal, float dis);
	void set(float x, float y, float z, float dis);
private:
	void normalize();
private:
	Vector3 m_normal;
	float	m_distance;
};

CU_NS_END
