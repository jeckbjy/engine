#pragma once
#include "Vector3.h"

CU_NS_BEGIN

class Matrix4;
class Plane;
class Sphere;
class Ray;
/** Axis aligned box represented by minimum and maximum point. */
class CU_API AABox
{
public:
	/** Different corners of a box. */
	/*
	1-----2
	/|    /|
	/ |   / |
	5-----4  |
	|  0--|--3
	| /   | /
	|/    |/
	6-----7
	*/
	enum CornerEnum
	{
		FAR_LEFT_BOTTOM = 0,
		FAR_LEFT_TOP = 1,
		FAR_RIGHT_TOP = 2,
		FAR_RIGHT_BOTTOM = 3,
		NEAR_RIGHT_BOTTOM = 7,
		NEAR_LEFT_BOTTOM = 6,
		NEAR_LEFT_TOP = 5,
		NEAR_RIGHT_TOP = 4
	};
	static const AABox EMPTY;

	AABox();
	AABox(const Vector3& min, const Vector3& max);
	AABox(float min, float max);
	~AABox(){}

	void scale(const Vector3& s);
	void merge(const AABox& rhs);
	void merge(const Vector3& point);

	/**
	* Transforms the bounding box by the given matrix.
	*
	* @note
	* As the resulting box will no longer be axis aligned, an axis align box
	* is instead created by encompassing the transformed oriented bounding box.
	* Retrieving the value as an actual OBB would provide a tighter fit.
	*/
	void transform(const Matrix4& matrix);
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
	void transformAffine(const Matrix4& matrix);

	bool intersects(const AABox& b2) const;
	bool intersects(const Sphere& sphere) const;
	bool intersects(const Plane& plane) const;
	bool intersects(const Ray& ray) const;
	bool intersects(const Ray& ray, float& d1) const;
	bool intersects(const Ray& ray, float& d1, float& d2) const;

	void setMin(const Vector3& vec) { m_min = vec; }
	void setMax(const Vector3& vec) { m_max = vec; }
	void setExtents(const Vector3& min, const Vector3& max) { m_min = min; m_max = max; }

	const Vector3& getMin() const { return m_min; }
	const Vector3& getMax() const { return m_max; }
	Vector3 getCenter() const;
	Vector3 getSize() const;
	Vector3 getHalfSize() const;
	Vector3 getCorner(CornerEnum corner) const;
	float getRadius() const;
	float getVolume() const;

	bool contains(const Vector3& v) const;
	bool contains(const AABox& other) const;

	bool operator== (const AABox& rhs) const { return m_min == rhs.m_min && m_max == rhs.m_max; }
	bool operator!= (const AABox& rhs) const { return !(*this == rhs); }

protected:
	Vector3 m_min;
	Vector3 m_max;
};

CU_NS_END
