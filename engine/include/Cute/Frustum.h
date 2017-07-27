#pragma once
#include "Cute/Foundation.h"
#include "Cute/Plane.h"
#include "Cute/Matrix4.h"

CUTE_NS_BEGIN

enum FrustumPlane
{
	FRUSTUM_PLANE_NEAR	= 0,
	FRUSTUM_PLANE_FAR	= 1,
	FRUSTUM_PLANE_LEFT	= 2,
	FRUSTUM_PLANE_RIGHT = 3,
	FRUSTUM_PLANE_TOP	= 4,
	FRUSTUM_PLANE_BOTTOM= 5
};

class Matrix4;
class CUTE_CORE_API Frustum
{
public:
	Frustum();
	Frustum(const Matrix4& projectionMatrix);
	~Frustum();

	const Plane& getNear() const { return m_near; }
	const Plane& getFar() const { return m_far; }
	const Plane& getLeft() const { return m_left; }
	const Plane& getRight() const { return m_right; }
	const Plane& getTop() const { return m_top; }
	const Plane& getBottom() const { return m_bottom; }
	const Plane& getPlane(FrustumPlane index) const { return *(&m_near + index); }

	void update(const Matrix4& proj);
    void updateOrtho(float orthoSize, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix4& transform = Matrix4::IDENTITY);

private:
	Plane m_near;
	Plane m_far;
	Plane m_left;
	Plane m_right;
	Plane m_top;
	Plane m_bottom;
};

CUTE_NS_END
