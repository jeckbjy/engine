#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class Ray;

/**
* Represents a rectangle in three dimensional space. It is represented by two axes that extend from the specified
* origin. Axes should be perpendicular to each other and they extend in both positive and negative directions from the
* origin by the amount specified by extents.
*/
class CUTE_CORE_API Rect3
{
public:
	Rect3();
	Rect3(const Vector3& center, const Vector3& axisH, const Vector3& axisV, float extH, float extV);

	void getNearestPoint(const Ray& ray, Vector3 points[2], float& distance) const;
	void getNearestPoint(const Vector3& point, Vector3& nearestPoint, float& distance) const;

	bool intersects(const Ray& ray, float* distance) const;

	const Vector3& getCenter() const { return m_center; }
	const Vector3& getAxisHorz() const { return m_axisHorz; }
	const Vector3& getAxisVert() const { return m_axisVert; }
	const float& getExtentHorz() const { return m_extentHorz; }
	const float& getExtentVertical() const { return m_extentVert; }

private:
	Vector3 m_center;
	Vector3 m_axisHorz;
	Vector3 m_axisVert;
	float	m_extentHorz;
	float	m_extentVert;
};

CUTE_NS_END
