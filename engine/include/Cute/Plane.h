#pragma once
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class AABox;
class Sphere;
class CUTE_CORE_API Plane
{
public:
	/**
	* The "positive side" of the plane is the half space to which the plane normal points. The "negative side" is the
	* other half space. The flag "no side" indicates the plane itself.
	*/
	enum Side
	{
		SIDE_NONE,
		SIDE_POSITIVE,
		SIDE_NEGATIVE,
		SIDE_BOTH,
	};

public:
	Plane();
	Plane(const Vector3& normal, float dist);
	Plane(const Vector3& normal, const Vector3& point);
	Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);

	Side getSide(const Vector3& point) const;
	Side getSide(const AABox& box) const;
	Side getSide(const Sphere& sphere) const;

	const Vector3& getNormal() const { return m_normal; }
	float getDistance(const Vector3& point) const;
	float getDistance() const { return m_distance; }

	void setNormal(const Vector3& normal);
	void setNormal(float x, float y, float z);
	void setDistance(float dist) { m_distance = dist; }

	void set(const Vector3& normal, float dis);
	void set(float x, float y, float z, float dis);

	float normalize();

	Vector3 projectVector(const Vector3& point) const;

private:
	Vector3 m_normal;
	float	m_distance;
};

CUTE_NS_END
