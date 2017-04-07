//! Math
#include "Cute/Bounds.h"

CUTE_NS_BEGIN

Bounds::Bounds()
{
}

Bounds::Bounds(const AABox& box, const Sphere& sphere)
	: m_box(box)
	, m_sphere(sphere)
{
}

Bounds::~Bounds()
{
}

void Bounds::setBounds(const AABox& box, const Sphere& sphere)
{
	m_box = box;
	m_sphere = sphere;
}

void Bounds::merge(const Bounds& rhs)
{
	m_box.merge(rhs.m_box);
	m_sphere.merge(rhs.m_sphere);
}

void Bounds::merge(const Vector3& point)
{
	m_box.merge(point);
	m_sphere.merge(point);
}

void Bounds::transform(const Matrix4& matrix)
{
	m_box.transform(matrix);
	m_sphere.transform(matrix);
}

void Bounds::transformAffine(const Matrix4& matrix)
{
	m_box.transformAffine(matrix);
	m_sphere.transform(matrix);//?right
}

CUTE_NS_END
