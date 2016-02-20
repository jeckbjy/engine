#include "Plane.h"
#include "AABox.h"
#include "Sphere.h"
#include "CMath.h"

CU_NS_BEGIN

Plane::Plane()
:m_normal(0, 1, 0), m_distance(0)
{
}

Plane::Plane(const Vector3& normal, float dist)
	: m_normal(normal), m_distance(dist)
{
	normalize();
}

void Plane::normalize()
{
	float len = m_normal.length();
	if (len < 0.000001f)
		return;
	float factor = 1 / len;
	m_normal *= factor;
	m_distance *= factor;
}

Plane::Side Plane::getSide(const Vector3& point) const
{
	float dist = getDistance(point);

	if (dist < 0.0f)
		return Plane::NEGATIVE_SIDE;

	if (dist > 0.0f)
		return Plane::POSITIVE_SIDE;

	return Plane::NO_SIDE;
}

Plane::Side Plane::getSide(const AABox& box) const
{
	// Calculate the distance between box centre and the plane
	float dist = getDistance(box.getCenter());

	// Calculate the maximize allows absolute distance for
	// the distance between box centre and plane
	Vector3 halfSize = box.getHalfSize();
	float maxAbsDist = Math::abs(m_normal.x * halfSize.x) + Math::abs(m_normal.y * halfSize.y) + Math::abs(m_normal.z * halfSize.z);

	if (dist < -maxAbsDist)
		return Plane::NEGATIVE_SIDE;

	if (dist > +maxAbsDist)
		return Plane::POSITIVE_SIDE;

	return Plane::BOTH_SIDE;
}

Plane::Side Plane::getSide(const Sphere& sphere) const
{
	// Calculate the distance between box centre and the plane
	float dist = getDistance(sphere.getCenter());
	float radius = sphere.getRadius();

	if (dist < -radius)
		return Plane::NEGATIVE_SIDE;

	if (dist > +radius)
		return Plane::POSITIVE_SIDE;

	return Plane::BOTH_SIDE;
}

float Plane::getDistance(const Vector3& point) const
{
	return Vector3::dot(m_normal, point) + m_distance;
}

void Plane::setNormal(const Vector3& normal)
{
	m_normal = normal;
	normalize();
}

void Plane::setNormal(float x, float y, float z)
{
	m_normal.set(x, y, z);
	normalize();
}

void Plane::set(const Vector3& normal, float dis)
{
	m_normal = normal;
	m_distance = dis;
	normalize();
}

void Plane::set(float x, float y, float z, float dis)
{
	m_normal.set(x, y, z); 
	m_distance = dis; 
	normalize();
}

CU_NS_END