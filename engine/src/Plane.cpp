#include "Plane.h"

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

float Plane::distance(const Vector3& point) const
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