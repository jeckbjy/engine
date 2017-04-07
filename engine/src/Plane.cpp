//! Math
#include "Cute/Plane.h"
#include "Cute/Matrix3.h"
#include "Cute/AABox.h"
#include "Cute/Sphere.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

Plane::Plane()
	: m_normal(0, 1, 0)
	, m_distance(0)
{
}

Plane::Plane(const Vector3& normal, float dist)
	: m_normal(normal)
	, m_distance(dist)
{
}

Plane::Plane(const Vector3& normal, const Vector3& point)
	: m_normal(normal)
	, m_distance(normal.dot(point))
{
}

Plane::Plane(const Vector3& point0, const Vector3& point1, const Vector3& point2)
{
	Vector3 kEdge1 = point1 - point0;
	Vector3 kEdge2 = point2 - point0;
	m_normal = kEdge1.cross(kEdge2);
	m_normal.normalize();
	m_distance = m_normal.dot(point0);
}

Plane::Side Plane::getSide(const Vector3& point) const
{
	float dist = getDistance(point);

	if (dist < 0.0f)
		return Plane::SIDE_NEGATIVE;

	if (dist > 0.0f)
		return Plane::SIDE_POSITIVE;

	return Plane::SIDE_NONE;
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
		return Plane::SIDE_NEGATIVE;

	if (dist > +maxAbsDist)
		return Plane::SIDE_POSITIVE;

	return Plane::SIDE_BOTH;
}

Plane::Side Plane::getSide(const Sphere& sphere) const
{
	// Calculate the distance between box centre and the plane
	float dist = getDistance(sphere.getCenter());
	float radius = sphere.getRadius();

	if (dist < -radius)
		return Plane::SIDE_NEGATIVE;

	if (dist > +radius)
		return Plane::SIDE_POSITIVE;

	return Plane::SIDE_BOTH;
}

float Plane::getDistance(const Vector3& point) const
{
	return Vector3::dot(m_normal, point) + m_distance;
}

void Plane::setNormal(const Vector3& normal)
{
	m_normal = normal;
}

void Plane::setNormal(float x, float y, float z)
{
	m_normal.set(x, y, z);
}

void Plane::set(const Vector3& normal, float dis)
{
	m_normal = normal;
	m_distance = dis;
}

void Plane::set(float x, float y, float z, float dis)
{
	m_normal.set(x, y, z);
	m_distance = dis;
}

float Plane::normalize()
{
	float len = m_normal.length();

	if (len > Math::EPSILON)
	{
		float factor = 1 / len;
		m_normal *= factor;
		m_distance *= factor;
	}

	return len;
}

Vector3 Plane::projectVector(const Vector3& point) const
{
	// We know plane normal is unit length, so use simple method
	Matrix3 xform;
	xform[0][0] = 1.0f - m_normal.x * m_normal.x;
	xform[0][1] = -m_normal.x * m_normal.y;
	xform[0][2] = -m_normal.x * m_normal.z;
	xform[1][0] = -m_normal.y * m_normal.x;
	xform[1][1] = 1.0f - m_normal.y * m_normal.y;
	xform[1][2] = -m_normal.y * m_normal.z;
	xform[2][0] = -m_normal.z * m_normal.x;
	xform[2][1] = -m_normal.z * m_normal.y;
	xform[2][2] = 1.0f - m_normal.z * m_normal.z;
	return xform.transform(point);
}

CUTE_NS_END
