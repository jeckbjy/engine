#pragma once
#include "Vector3.h"

CU_NS_BEGIN

class AABox;
class Sphere;
class Plane
{
public:
	/**
	* The "positive side" of the plane is the half space to which the plane normal points. The "negative side" is the
	* other half space. The flag "no side" indicates the plane itself.
	*/
	enum Side
	{
		NO_SIDE,
		POSITIVE_SIDE,
		NEGATIVE_SIDE,
		BOTH_SIDE
	};

public:
	Plane();
	Plane(const Vector3& normal, float dist);

	Side getSide(const Vector3& point) const;
	Side getSide(const AABox& box) const;
	Side getSide(const Sphere& sphere) const;

	float getDistance(const Vector3& point) const;
	float getDistance() const { return m_distance; }
	const Vector3& getNormal() const { return m_normal; }

	void setNormal(const Vector3& normal);
	void setNormal(float x, float y, float z);
	void setDistance(float dist) { m_distance = dist; }
	void set(const Vector3& normal, float dis);
	void set(float x, float y, float z, float dis);
private:
	void normalize();
private:
	Vector3 m_normal;
	float	m_distance;
};

CU_NS_END
