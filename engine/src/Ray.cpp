#include "Ray.h"

CU_NS_BEGIN

Ray::Ray()
:m_direction(0, 0, 1)
{

}

Ray::Ray(const Vector3& origin, const Vector3& direction)
	: m_origin(origin), m_direction(direction)
{
	m_direction.normalize();
}

void Ray::setDirection(const Vector3& dir)
{
	m_direction = dir;
	m_direction.normalize();
}

void Ray::setDirection(float x, float y, float z)
{
	m_direction.set(x, y, z);
	m_direction.normalize();
}

void Ray::set(const Vector3& origin, const Vector3& dir)
{
	m_origin = origin;
	m_direction = dir;
	m_direction.normalize();
}

CU_NS_END