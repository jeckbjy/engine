//! Math
#include "Cute/Ray.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

Ray::Ray()
	: m_origin(Vector3::ZERO)
	, m_direction(Vector3::UNIT_Z)
{
}

Ray::Ray(const Vector3& origin, const Vector3& direction)
	: m_origin(origin)
	, m_direction(direction)
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

void Ray::transform(const Matrix4& matrix)
{
	Vector3 end = getPoint(1.0f);

	m_origin = matrix.multiply(m_origin);
	end = matrix.multiply(end);

	m_direction = Vector3::normalize(end - m_direction);
}

void Ray::transformAffine(const Matrix4& matrix)
{
	Vector3 end = getPoint(1.0f);

	m_origin = matrix.multiplyAffine(m_origin);
	end = matrix.multiplyAffine(end);

	m_direction = Vector3::normalize(end - m_direction);
}

CUTE_NS_END
