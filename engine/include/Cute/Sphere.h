#pragma once
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class Matrix4;
class CUTE_CORE_API Sphere
{
public:
	Sphere();
	Sphere(const Vector3& center, float radius);

	const Vector3& getCenter(void) const { return m_center; }
	float getRadius(void) const { return m_radius; }

	void setCenter(const Vector3& center) { m_center = center; }
	void setRadius(float radius) { m_radius = radius; }

	void merge(const Sphere& rhs);
	void merge(const Vector3& point);

	void transform(const Matrix4& matrix);
	bool contains(const Vector3& v) const;
	bool intersects(const Sphere& s) const;

protected:
	Vector3 m_center;
	float	m_radius;
};

CUTE_NS_END
