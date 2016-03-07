#pragma once
#include "Drawable.h"
#include "Ray.h"
#include "AABox.h"
#include "Sphere.h"
#include "Frustum.h"

CU_NS_BEGIN

// °Ë²æÊ÷²éÑ¯
class CU_API OctreeQuery
{
public:
	typedef Vector<Drawable*> DrawableVec;
	virtual Intersection intersects(const AABox& box, bool inside) = 0;
	virtual void test(Drawable* drawable) = 0;

	DrawableVec& getResult() { return m_result; }

protected:
	DrawableVec m_result;
};

template<typename T>
class TOctreeQuery : public OctreeQuery
{
public:
	Intersection intersects(const AABox& box, bool inside)
	{
		//if (inside)
		//	return INSIDE;
		//else
		//	return m_box.intersects(box);
		return INSIDE;
	}

	void test(Drawable* drawable)
	{

	}

protected:
	T m_box;
};

typedef TOctreeQuery<Vector3>	PointOctreeQuery;
typedef TOctreeQuery<Sphere>	SphereOctreeQuery;
typedef TOctreeQuery<AABox>		BoxOctreeQuery;
typedef TOctreeQuery<Frustum>	FrustumOctreeQuery;

// ÕÚµ²°åÌÞ³ý
class CU_API OccludedOctreeQuery : public FrustumOctreeQuery
{
public:

};

CU_NS_END
