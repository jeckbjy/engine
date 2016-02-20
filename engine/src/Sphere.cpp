#include "Sphere.h"
#include "Matrix4.h"
#include "Plane.h"
#include "AABox.h"
#include "Ray.h"
#include "CMath.h"

CU_NS_BEGIN

void Sphere::merge(const Sphere& rhs)
{

}

void Sphere::merge(const Vector3& point)
{

}

void Sphere::transform(const Matrix4& matrix)
{

}

bool Sphere::intersects(const Sphere& s) const
{
	return (s.m_center - m_center).squared() <= Math::squared(s.m_radius + m_radius);
}

bool Sphere::intersects(const AABox& box) const
{
	return box.intersects(*this);
}

bool Sphere::intersects(const Plane& plane) const
{
	return Math::abs(plane.getDistance(m_center)) <= m_radius;
}

bool Sphere::intersects(const Ray& ray, float* dist) const
{
	const Vector3& raydir = ray.getDirection();
	const Vector3& rayorig = ray.getOrigin() - getCenter();
	float radius = getRadius();

	// Check origin inside first
	if (rayorig.squared() <= radius * radius && dist == NULL)
		return true;

	// t = (-b +/- sqrt(b*b + 4ac)) / 2a
	float a = raydir.dot(raydir);
	float b = 2 * rayorig.dot(raydir);
	float c = rayorig.dot(rayorig) - radius*radius;

	// Determinant
	float d = (b*b) - (4 * a * c);
	if (d < 0)
	{
		// No intersection
		return false;
	}
	else
	{
		// If d == 0 there is one intersection, if d > 0 there are 2.
		// We only return the first one.

		float t = (-b - Math::sqrt(d)) / (2 * a);
		if (t < 0)
			t = (-b + Math::sqrt(d)) / (2 * a);

		if (dist)
			*dist = t;
		return true;
	}
}

CU_NS_END
