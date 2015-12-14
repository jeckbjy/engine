#pragma once
#include "Plane.h"
#include "Matrix4.h"

CU_NS_BEGIN

class CU_API Frustum
{
public:
	Frustum();

	void set(const Matrix4& matrix);

	const Plane& getNear() const { return _near; }
	const Plane& getFar() const { return _far; }
	const Plane& getLeft() const { return _left; }
	const Plane& getRight() const { return _right; }
	const Plane& getBottom() const { return _bottom; }
	const Plane& getTop() const { return _top; }

private:
	Plane	_near;
	Plane	_far;
	Plane	_bottom;
	Plane	_top;
	Plane	_left;
	Plane	_right;
};

CU_NS_END