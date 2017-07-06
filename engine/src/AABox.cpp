//! Math
#include "Cute/AABox.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

const AABox AABox::EMPTY;

AABox::AABox()
	: m_min(-0.5f, -0.5f, -0.5f)
	, m_max(0.5f, 0.5f, 0.5f)
{
}

AABox::AABox(const Vector3& min, const Vector3& max)
	: m_min(min), m_max(max)
{
}

AABox::AABox(float min, float max)
	: m_min(min, min, min)
	, m_max(max, max, max)
{

}

Vector3 AABox::getCenter() const
{
	return (m_min + m_max) * 0.5f;
}

Vector3 AABox::getSize() const
{
	return m_max - m_min;
}

Vector3 AABox::getHalfSize() const
{
	return (m_max - m_min) * 0.5f;
}

Vector3 AABox::getCorner(CornerEnum corner) const
{
	switch (corner)
	{
	case FAR_LEFT_BOTTOM:
		return m_min;
	case FAR_LEFT_TOP:
		return Vector3(m_min.x, m_max.y, m_min.z);
	case FAR_RIGHT_TOP:
		return Vector3(m_max.x, m_max.y, m_min.z);
	case FAR_RIGHT_BOTTOM:
		return Vector3(m_max.x, m_min.y, m_min.z);
	case NEAR_RIGHT_BOTTOM:
		return Vector3(m_max.x, m_min.y, m_max.z);
	case NEAR_LEFT_BOTTOM:
		return Vector3(m_min.x, m_min.y, m_max.z);
	case NEAR_LEFT_TOP:
		return Vector3(m_min.x, m_max.y, m_max.z);
	case NEAR_RIGHT_TOP:
		return m_max;
	default:
		return Vector3::ZERO;
	}
}

float AABox::getRadius() const
{
	return (m_max - m_min).length();
}

float AABox::getVolume() const
{
	Vector3 diff = m_max - m_min;
	return diff.x * diff.y * diff.z;
}

void AABox::scale(const Vector3& s)
{
	Vector3 center = getCenter();
	m_min = center + (m_min - center) * s;
	m_max = center + (m_max - center) * s;
}

void AABox::merge(const AABox& rhs)
{
	m_max.ceil(rhs.m_max);
	m_min.floor(rhs.m_min);
}

void AABox::merge(const Vector3& point)
{
	m_max.ceil(point);
	m_min.floor(point);
}

/**
* Transforms the bounding box by the given matrix.
*
* @note
* As the resulting box will no longer be axis aligned, an axis align box
* is instead created by encompassing the transformed oriented bounding box.
* Retrieving the value as an actual OBB would provide a tighter fit.
*/
void AABox::transform(const Matrix4& matrix)
{
	Vector3 oldMin, oldMax, currentCorner;

	// Getting the old values so that we can use the existing merge method.
	oldMin = m_min;
	oldMax = m_max;

	// We sequentially compute the corners in the following order :
	// 0, 6, 5, 1, 2, 4, 7, 3
	// This sequence allows us to only change one member at a time to get at all corners.

	// For each one, we transform it using the matrix
	// Which gives the resulting point and merge the resulting point.

	// First corner
	// min min min
	currentCorner = oldMin;
	merge(matrix.multiplyAffine(currentCorner));

	// min,min,max
	currentCorner.z = oldMax.z;
	merge(matrix.multiplyAffine(currentCorner));

	// min max max
	currentCorner.y = oldMax.y;
	merge(matrix.multiplyAffine(currentCorner));

	// min max min
	currentCorner.z = oldMin.z;
	merge(matrix.multiplyAffine(currentCorner));

	// max max min
	currentCorner.x = oldMax.x;
	merge(matrix.multiplyAffine(currentCorner));

	// max max max
	currentCorner.z = oldMax.z;
	merge(matrix.multiplyAffine(currentCorner));

	// max min max
	currentCorner.y = oldMin.y;
	merge(matrix.multiplyAffine(currentCorner));

	// max min min
	currentCorner.z = oldMin.z;
	merge(matrix.multiplyAffine(currentCorner));
}

/**
* Transforms the bounding box by the given matrix.
*
* @note
* As the resulting box will no longer be axis aligned, an axis align box
* is instead created by encompassing the transformed oriented bounding box.
* Retrieving the value as an actual OBB would provide a tighter fit.
*
* @note
* Provided matrix must be affine.
*/
void AABox::transformAffine(const Matrix4& m)
{
	assert(m.isAffine());

	Vector3 centre = getCenter();
	Vector3 halfSize = getHalfSize();

	Vector3 newCentre = m.multiplyAffine(centre);
	Vector3 newHalfSize(
	    Math::abs(m[0][0]) * halfSize.x + Math::abs(m[0][1]) * halfSize.y + Math::abs(m[0][2]) * halfSize.z,
	    Math::abs(m[1][0]) * halfSize.x + Math::abs(m[1][1]) * halfSize.y + Math::abs(m[1][2]) * halfSize.z,
	    Math::abs(m[2][0]) * halfSize.x + Math::abs(m[2][1]) * halfSize.y + Math::abs(m[2][2]) * halfSize.z);

	setExtents(newCentre - newHalfSize, newCentre + newHalfSize);
}

bool AABox::contains(const Vector3& v) const
{
	return
	    m_min.x <= v.x && v.x <= m_max.x &&
	    m_min.y <= v.y && v.y <= m_max.y &&
	    m_min.z <= v.z && v.z <= m_max.z;
}

bool AABox::contains(const AABox& other) const
{
	return
	    m_min.x <= other.m_min.x &&
	    m_min.y <= other.m_min.y &&
	    m_min.z <= other.m_min.z &&
	    m_max.x >= other.m_max.x &&
	    m_max.y >= other.m_max.y &&
	    m_max.z >= other.m_max.z;
}

bool AABox::intersects(const AABox& b2) const
{
	// Use up to 6 separating planes
	if (m_max.x < b2.m_min.x)
		return false;
	if (m_max.y < b2.m_min.y)
		return false;
	if (m_max.z < b2.m_min.z)
		return false;

	if (m_min.x > b2.m_max.x)
		return false;
	if (m_min.y > b2.m_max.y)
		return false;
	if (m_min.z > b2.m_max.z)
		return false;

	// Otherwise, must be intersecting
	return true;
}

CUTE_NS_END
