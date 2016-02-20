#pragma once
#include "Vector3.h"

CU_NS_BEGIN

class Matrix4;
class AABox;
class Plane;
class Ray;
class CU_API Sphere
{
public:
	Sphere()
		: m_center(Vector3::ZERO), m_radius(1.0f)
	{
	}
	Sphere(const Vector3& center, float radius) 
		:m_center(center), m_radius(radius)
	{
	}

	void setCenter(const Vector3& center) { m_center = center; }
	void setRadius(float radius) { m_radius = radius; }

	const Vector3& getCenter(void) const { return m_center; }
	float getRadius(void) const { return m_radius; }

	void merge(const Sphere& rhs);
	void merge(const Vector3& point);

	void transform(const Matrix4& matrix);

	bool intersects(const Sphere& s) const;
	bool intersects(const AABox& box) const;
	bool intersects(const Plane& plane) const;
	bool intersects(const Ray& ray, float* dist) const;

	bool contains(const Vector3& v) const
	{
		return (v - m_center).squared() < m_radius * m_radius; 
	}

protected:
	Vector3 m_center;
	float	m_radius;
};

CU_NS_END
