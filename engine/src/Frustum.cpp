//! Math
#include "Cute/Frustum.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

Frustum::Frustum()
{
	update(Matrix4::IDENTITY);
}

Frustum::Frustum(const Matrix4& projectionMatrix)
{
	update(projectionMatrix);
}

Frustum::~Frustum()
{
}

void Frustum::update(const Matrix4& proj)
{
	m_left.set(proj[3][0] + proj[0][0], proj[3][1] + proj[0][1], proj[3][2] + proj[0][2], proj[3][3] + proj[0][3]);
	m_right.set(proj[3][0] - proj[0][0], proj[3][1] - proj[0][1], proj[3][2] - proj[0][2], proj[3][3] - proj[0][3]);

	m_top.set(proj[3][0] - proj[1][0], proj[3][1] - proj[1][1], proj[3][2] - proj[1][2], proj[3][3] - proj[1][3]);
	m_bottom.set(proj[3][0] + proj[1][0], proj[3][1] + proj[1][1], proj[3][2] + proj[1][2], proj[3][3] + proj[1][3]);

	m_near.set(proj[3][0] + proj[2][0], proj[3][1] + proj[2][1], proj[3][2] + proj[2][2], proj[3][3] + proj[2][3]);
	m_far.set(proj[3][0] - proj[2][0], proj[3][1] - proj[2][1], proj[3][2] - proj[2][2], proj[3][3] - proj[2][3]);

	for (int i = 0; i < 6; ++i)
	{
		Plane& plane = *(&m_near + i);
		float length = plane.normalize();
		plane.setDistance(plane.getDistance() / length);
	}
}

CUTE_NS_END
