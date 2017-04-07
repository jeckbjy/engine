#pragma once
#include "Cute/Vector3.h"
#include "Cute/AABox.h"
#include "Cute/Sphere.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Bounds
{
public:
	Bounds();
	Bounds(const AABox& box, const Sphere& sphere);
	~Bounds();

	const AABox&  getBox() const { return m_box; }
	const Sphere& getSphere() const { return m_sphere; }

	void setBounds(const AABox& box, const Sphere& sphere);

	void merge(const Bounds& rhs);
	void merge(const Vector3& point);

	void transform(const Matrix4& matrix);
	void transformAffine(const Matrix4& matrix);

private:
	AABox	m_box;
	Sphere	m_sphere;
};

CUTE_NS_END
