//! Math
#include "Cute/Sphere.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

Sphere::Sphere()
	: m_center(Vector3::ZERO)
	, m_radius(1.0f)
{
}
Sphere::Sphere(const Vector3& center, float radius)
	: m_center(center)
	, m_radius(radius)
{
}

void Sphere::merge(const Sphere& rhs)
{
	Vector3 newCenter = (m_center + rhs.m_center) * 0.5f;

	float newRadiusA = newCenter.distance(m_center) + getRadius();
	float newRadiusB = newCenter.distance(rhs.m_center) + rhs.getRadius();

	m_center = newCenter;
	m_radius = std::max(newRadiusA, newRadiusB);
}

void Sphere::merge(const Vector3& point)
{
	float dist = point.distance(m_center);
	m_radius = std::max(m_radius, dist);
}

void Sphere::transform(const Matrix4& matrix)
{
	Vector3 edge = m_center + Vector3::UNIT_X * m_radius;
	m_center = matrix.multiplyAffine(m_center);
	edge = matrix.multiplyAffine(edge);

	m_radius = m_center.distance(edge);
}

bool Sphere::intersects(const Sphere& s) const
{
	return (s.m_center - m_center).squared() <= Math::squared(s.m_radius + m_radius);
}

bool Sphere::contains(const Vector3& v) const
{
	return (v - m_center).squared() < m_radius * m_radius;
}

CUTE_NS_END
