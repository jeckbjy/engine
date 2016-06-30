#include "Frustum.h"

CU_NS_BEGIN

Frustum::Frustum()
{
	set(Matrix4::IDENTITY);
}

void Frustum::set(const Matrix4& mat)
{
	_near.set(mat.at(3) + mat.at(2), mat.at(7) + mat.at(6), mat.at(11) + mat.at(10), mat.at(15) + mat.at(14));
}

CU_NS_END