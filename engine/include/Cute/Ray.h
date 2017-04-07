#pragma once
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class Matrix4;
// direction must be unit length!!
class CUTE_CORE_API Ray
{
public:
	Ray();
	Ray(const Vector3& origin, const Vector3& direction);

	const Vector3& getOrigin() const { return m_origin; }
	const Vector3& getDirection() const { return m_direction; }

	void setOrigin(const Vector3& origin) { m_origin = origin; }
	void setOrigin(float x, float y, float z) { m_origin.set(x, y, z); }
	void setDirection(const Vector3& dir);
	void setDirection(float x, float y, float z);

	void set(const Vector3& origin, const Vector3& dir);

	void transform(const Matrix4& matrix);
	void transformAffine(const Matrix4& matrix);

	Vector3 getPoint(float t) const { return m_origin + m_direction * t; }
	Vector3 operator*(float t) const { return getPoint(t); }

private:
	Vector3 m_origin;
	Vector3 m_direction;
};

CUTE_NS_END
