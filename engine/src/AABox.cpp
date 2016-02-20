#include "AABox.h"
#include "Ray.h"
#include "Plane.h"
#include "Sphere.h"
#include "Matrix4.h"
#include "CMath.h"

CU_NS_BEGIN

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
		return Vector3();
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

bool AABox::intersects(const Sphere& sphere) const
{
	// Use splitting planes
	const Vector3& center = sphere.getCenter();
	float radius = sphere.getRadius();
	const Vector3& min = getMin();
	const Vector3& max = getMax();

	// Arvo's algorithm
	float s, d = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (center[i] < min[i])
		{
			s = center[i] - min[i];
			d += s * s;
		}
		else if (center[i] > max[i])
		{
			s = center[i] - max[i];
			d += s * s;
		}
	}
	return d <= radius * radius;
}

bool AABox::intersects(const Plane& plane) const
{
	return (plane.getSide(*this) == Plane::BOTH_SIDE);
}

bool AABox::intersects(const Ray& ray) const
{
	float d1;
	return intersects(ray, d1);
}

bool AABox::intersects(const Ray& ray, float& d1) const
{
	d1 = 0.0f;
	float lowt = 0.0f;
	float t;
	bool hit = false;
	Vector3 hitpoint;
	const Vector3& min = getMin();
	const Vector3& max = getMax();
	const Vector3& rayorig = ray.getOrigin();
	const Vector3& raydir = ray.getDirection();

	// Check origin inside first
	if ((rayorig.x > min.x && rayorig.y > min.y && rayorig.z > min.z) && (rayorig.x < max.x && rayorig.y < max.y && rayorig.z < max.z))
	{
		return true;
	}

	// Check each face in turn, only check closest 3
	// Min x
	if (rayorig.x <= min.x && raydir.x > 0)
	{
		t = (min.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max x
	if (rayorig.x >= max.x && raydir.x < 0)
	{
		t = (max.x - rayorig.x) / raydir.x;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min y
	if (rayorig.y <= min.y && raydir.y > 0)
	{
		t = (min.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max y
	if (rayorig.y >= max.y && raydir.y < 0)
	{
		t = (max.y - rayorig.y) / raydir.y;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.z >= min.z && hitpoint.z <= max.z &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Min z
	if (rayorig.z <= min.z && raydir.z > 0)
	{
		t = (min.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}
	// Max z
	if (rayorig.z >= max.z && raydir.z < 0)
	{
		t = (max.z - rayorig.z) / raydir.z;
		if (t >= 0)
		{
			// Substitute t back into ray and check bounds and dist
			hitpoint = rayorig + raydir * t;
			if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
				hitpoint.y >= min.y && hitpoint.y <= max.y &&
				(!hit || t < lowt))
			{
				hit = true;
				lowt = t;
			}
		}
	}

	d1 = lowt;
	return hit;
}

bool AABox::intersects(const Ray& ray, float& d1, float& d2) const
{
	const Vector3& min = getMin();
	const Vector3& max = getMax();
	const Vector3& rayorig = ray.getOrigin();
	const Vector3& raydir = ray.getDirection();

	Vector3 absDir;
	absDir[0] = Math::abs(raydir[0]);
	absDir[1] = Math::abs(raydir[1]);
	absDir[2] = Math::abs(raydir[2]);

	// Sort the axis, ensure check minimise floating error axis first
	int imax = 0, imid = 1, imin = 2;
	if (absDir[0] < absDir[2])
	{
		imax = 2;
		imin = 0;
	}
	if (absDir[1] < absDir[imin])
	{
		imid = imin;
		imin = 1;
	}
	else if (absDir[1] > absDir[imax])
	{
		imid = imax;
		imax = 1;
	}

	float start = 0, end = Math::POS_INFINITY;

#define _CALC_AXIS(i)                                       \
	do {                                                    \
	float denom = 1 / raydir[i];                         \
	float newstart = (min[i] - rayorig[i]) * denom;      \
	float newend = (max[i] - rayorig[i]) * denom;        \
	if (newstart > newend) std::swap(newstart, newend); \
	if (newstart > end || newend < start) return false; \
	if (newstart > start) start = newstart;             \
	if (newend < end) end = newend;                     \
			} while(0)

	// Check each axis in turn

	_CALC_AXIS(imax);

	if (absDir[imid] < std::numeric_limits<float>::epsilon())
	{
		// Parallel with middle and minimise axis, check bounds only
		if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
			rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
			return false;
	}
	else
	{
		_CALC_AXIS(imid);

		if (absDir[imin] < std::numeric_limits<float>::epsilon())
		{
			// Parallel with minimise axis, check bounds only
			if (rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
				return false;
		}
		else
		{
			_CALC_AXIS(imin);
		}
	}
#undef _CALC_AXIS

	d1 = start;
	d2 = end;

	return true;
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

CU_NS_END
