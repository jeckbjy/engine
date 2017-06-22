//! Math
#include "Cute/Rect3.h"
#include "Cute/Ray.h"
#include "Cute/LineSegment3.h"

CUTE_NS_BEGIN

Rect3::Rect3()
	: m_center(Vector3::ZERO)
	, m_axisHorz(Vector3::ZERO)
	, m_axisVert(Vector3::ZERO)
	, m_extentHorz(0.0f)
	, m_extentVert(0.0f)
{
}

Rect3::Rect3(const Vector3& center, const Vector3& axisH, const Vector3& axisV, float extH, float extV)
	: m_center(center)
	, m_axisHorz(axisH)
	, m_axisVert(axisV)
	, m_extentHorz(extH)
	, m_extentVert(extV)
{
}

void Rect3::getNearestPoint(const Ray& ray, Vector3 nearestPoints[2], float& distance) const
{
	const Vector3& org = ray.getOrigin();
	const Vector3& dir = ray.getDirection();

//	bool foundNearest = false;
	float t = 0.0f;

	bool result = intersects(ray, &t);

	if (result == true)
	{
		nearestPoints[0] = org + dir * t;
		nearestPoints[1] = nearestPoints[0];
	}
	else
	{
		Vector3 scaledAxes[2];
		scaledAxes[0] = m_extentHorz * m_axisHorz;
		scaledAxes[1] = m_extentVert * m_axisVert;;

		distance = std::numeric_limits<float>::max();
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				float sign = (float)(2 * j - 1);
				Vector3 segCenter = m_center + sign * scaledAxes[i];
				Vector3 segStart = segCenter - scaledAxes[1 - i];
				Vector3 segEnd = segCenter + scaledAxes[1 - i];

				LineSegment3 segment(segStart, segEnd);

				Vector3 segPoints[2];
				float segDist = segment.getNearestPoint(ray, segPoints);

				if (segDist < distance)
				{
					std::copy(nearestPoints, nearestPoints + 1, segPoints);
					nearestPoints = segPoints;
					distance = segDist;
				}
			}
		}
	}


	// Front of the ray is nearest, use found points
	if (t >= 0.0f)
	{
		// Do nothing, we already have the points
	}
	else // Rectangle is behind the ray origin, find nearest point to origin
	{
		getNearestPoint(org, nearestPoints[1], distance);
		nearestPoints[0] = org;
	}
}

void Rect3::getNearestPoint(const Vector3& point, Vector3& nearestPoint, float& distance) const
{
	Vector3 diff = m_center - point;
	float b0 = diff.dot(m_axisHorz);
	float b1 = diff.dot(m_axisVert);
	float s0 = -b0, s1 = -b1;
	float sqrDistance = diff.dot(diff);

	if (s0 < -m_extentHorz)
		s0 = -m_extentHorz;
	else if (s0 > m_extentHorz)
		s0 = m_extentHorz;

	sqrDistance += s0*(s0 + 2.0f*b0);

	if (s1 < -m_extentVert)
		s1 = -m_extentVert;
	else if (s1 > m_extentVert)
		s1 = m_extentVert;

	sqrDistance += s1*(s1 + 2.0f*b1);

	if (sqrDistance < 0.0f)
		sqrDistance = 0.0f;

	distance = std::sqrt(sqrDistance);
	nearestPoint = m_center + s0 * m_axisHorz + s1 * m_axisVert;
}

bool Rect3::intersects(const Ray& ray, float* distance /* = NULL */) const
{
	const Vector3& org = ray.getOrigin();
	const Vector3& dir = ray.getDirection();

	Vector3 normal = m_axisHorz.cross(m_axisVert);
	float NdotD = normal.dot(dir);
	if (fabs(NdotD) > 0.0f)
	{
		Vector3 diff = org - m_center;
		Vector3 basis[3];

		basis[0] = dir;
		basis[0].orthogonalComplement(basis[1], basis[2]);

		float UdD0 = basis[1].dot(m_axisHorz);
		float UdD1 = basis[1].dot(m_axisVert);
		float UdPmC = basis[1].dot(diff);
		float VdD0 = basis[2].dot(m_axisHorz);
		float VdD1 = basis[2].dot(m_axisVert);
		float VdPmC = basis[2].dot(diff);
		float invDet = 1.0f / (UdD0*VdD1 - UdD1*VdD0);

		float s0 = (VdD1*UdPmC - UdD1*VdPmC)*invDet;
		float s1 = (UdD0*VdPmC - VdD0*UdPmC)*invDet;

		if (fabs(s0) <= m_extentHorz && fabs(s1) <= m_extentVert)
		{
			float DdD0 = dir.dot(m_axisHorz);
			float DdD1 = dir.dot(m_axisVert);
			float DdDiff = dir.dot(diff);

			float t = s0 * DdD0 + s1 * DdD1 - DdDiff;

			if (distance != NULL)
				*distance = t;

			return true;
		}
	}

	if (distance != NULL)
		*distance = 0.0f;

	return false;
}

CUTE_NS_END
