#pragma once
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class Matrix4;

/** Axis aligned box represented by minimum and maximum point. */
class CUTE_CORE_API AABox
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
		FAR_LEFT_BOTTOM		= 0,
		FAR_LEFT_TOP		= 1,
		FAR_RIGHT_TOP		= 2,
		FAR_RIGHT_BOTTOM	= 3,
		NEAR_RIGHT_TOP		= 4,
		NEAR_LEFT_TOP		= 5,
		NEAR_LEFT_BOTTOM	= 6,
		NEAR_RIGHT_BOTTOM	= 7
	};

	static const AABox EMPTY;

	AABox();
	AABox(const Vector3& min, const Vector3& max);
	AABox(float min, float max);
	~AABox(){}

	void scale(const Vector3& s);
	void merge(const AABox& rhs);
	void merge(const Vector3& point);

	void transform(const Matrix4& matrix);
	void transformAffine(const Matrix4& matrix);

	void setMin(const Vector3& vec) { m_min = vec; }
	void setMax(const Vector3& vec) { m_max = vec; }
	void setExtents(const Vector3& min, const Vector3& max) { m_min = min; m_max = max; }

	const Vector3& getMin() const { return m_min; }
	const Vector3& getMax() const { return m_max; }
	Vector3 getCenter() const;
	Vector3 getSize() const;
	Vector3 getHalfSize() const;
	Vector3 getCorner(CornerEnum corner) const;
	float	getRadius() const;
	float	getVolume() const;

	bool	contains(const Vector3& v) const;
	bool	contains(const AABox& other) const;
	bool	intersects(const AABox& b2) const;

	bool operator== (const AABox& rhs) const { return m_min == rhs.m_min && m_max == rhs.m_max; }
	bool operator!= (const AABox& rhs) const { return !(*this == rhs); }

protected:
	Vector3 m_min;
	Vector3 m_max;
};

CUTE_NS_END
