#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class Ray;
class CUTE_CORE_API LineSegment3
{
public:
	LineSegment3();
	LineSegment3(const Vector3& start, const Vector3& end);

	/**
	* Find the nearest point on the line segment and the provided ray.
	*
	* @return	Set of nearest points and distance from the points. First nearest point is a point along the ray,
	*			while the second is along the line segment.
	*
	* @note	If segment and ray are parallel the set of points at the segment origin are returned.
	*/
	float getNearestPoint(const Ray& ray, Vector3 nearestPoints[2]) const;

	const Vector3& getStart() const { return mStart; }
	const Vector3& getEnd() const { return mEnd; }

	Vector3 getCenter() const { return mStart + (mEnd - mStart) * 0.5f; }
	float	getLength() const { return mStart.distance(mEnd); }

private:
	Vector3 mStart, mEnd;
};

CUTE_NS_END
