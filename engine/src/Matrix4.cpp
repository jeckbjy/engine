//! Math
#include "Cute/Matrix4.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

inline static float MINOR(
const Matrix4& m,
size_t r0, size_t r1, size_t r2,
size_t c0, size_t c1, size_t c2)
{
	return
		m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
		m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
		m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
}

const Matrix4 Matrix4::ZERO(0.0f);
const Matrix4 Matrix4::IDENTITY(1.0f);

//////////////////////////////////////////////////////////////////////////
// Default Use Left-Hand
//////////////////////////////////////////////////////////////////////////
Matrix4 Matrix4::ortho(float width, float height, float zNear, float zFar)
{
	Matrix4 m;
	m.setOrthoLH(width, height, zNear, zFar);
	return m;
}

Matrix4 Matrix4::orthoOffCenter(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 m;
	m.setOrthoOffCenterLH(left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspective(float width, float height, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveLH(width, height, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveFov(float fovy, float aspect, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveFovLH(fovy, aspect, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveOffCenter(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveOffCenterLH(left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
{
	Matrix4 m;
	m.setLookAtLH(eye, at, up);
	return m;
}

//////////////////////////////////////////////////////////////////////////
// Right-Hand
//////////////////////////////////////////////////////////////////////////
Matrix4 Matrix4::orthoRH(float width, float height, float zNear, float zFar)
{
	Matrix4 m;
	m.setOrthoRH(width, height, zNear, zFar);
	return m;
}

Matrix4 Matrix4::orthoOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 m;
	m.setOrthoOffCenterRH(left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveRH(float width, float height, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveRH(width, height, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveFovRH(float fovy, float aspect, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveFovRH(fovy, aspect, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveOffCenterRH(left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4 Matrix4::lookAtRH(const Vector3& eye, const Vector3& at, const Vector3& up)
{
	Matrix4 m;
	m.setLookAtRH(eye, at, up);
	return m;
}

//////////////////////////////////////////////////////////////////////////
// Left-Hand
//////////////////////////////////////////////////////////////////////////
Matrix4 Matrix4::orthoLH(float width, float height, float zNear, float zFar)
{
	Matrix4 m;
	m.setOrthoLH(width, height, zNear, zFar);
	return m;
}

Matrix4 Matrix4::orthoOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 m;
	m.setOrthoOffCenterLH(left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveLH(float width, float height, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveLH(width, height, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveFovLH(float fovy, float aspect, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveFovLH(fovy, aspect, zNear, zFar);
	return m;
}

Matrix4 Matrix4::perspectiveOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 m;
	m.setPerspectiveOffCenterLH(left, right, bottom, top, zNear, zFar);
	return m;
}

Matrix4 Matrix4::lookAtLH(const Vector3& eye, const Vector3& at, const Vector3& up)
{
	Matrix4 m;
	m.setLookAtLH(eye, at, up);
	return m;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Matrix4::Matrix4()
{
}

Matrix4::Matrix4(float t)
{
	set(t);
}

Matrix4::Matrix4(
	float t00, float t01, float t02, float t03,
	float t10, float t11, float t12, float t13,
	float t20, float t21, float t22, float t23,
	float t30, float t31, float t32, float t33)
{
	m00 = t00; m01 = t01; m02 = t02; m03 = t03;
	m10 = t10; m11 = t11; m12 = t12; m13 = t13;
	m20 = t20; m21 = t21; m22 = t22; m23 = t23;
	m30 = t30; m31 = t31; m32 = t32; m33 = t33;
}

Matrix4::Matrix4(const Matrix3& mat3)
{
	set(mat3);
}

Matrix4::Matrix4(const Quaternion& q)
{
	set(q);
}

Matrix4::Matrix4(const Vector3& axis, float angle)
{
	set(axis, angle);
}

Matrix4::Matrix4(const Vector3& t, const Vector3& s, const Quaternion& r)
{
	setTransform(t, s, r);
}

void Matrix4::set(float t)
{
	m00 = t; m01 = 0; m02 = 0; m03 = 0;
	m10 = 0; m11 = t; m12 = 0; m13 = 0;
	m20 = 0; m21 = 0; m22 = t; m23 = 0;
	m30 = 0; m31 = 0; m32 = 0; m33 = t;
}

void Matrix4::set(const Matrix3& mat3)
{
	m[0][0] = mat3[0][0]; m[0][1] = mat3[0][1]; m[0][2] = mat3[0][2]; m[0][3] = 0.0f;
	m[1][0] = mat3[1][0]; m[1][1] = mat3[1][1]; m[1][2] = mat3[1][2]; m[1][3] = 0.0f;
	m[2][0] = mat3[2][0]; m[2][1] = mat3[2][1]; m[2][2] = mat3[2][2]; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Matrix4::set(const Quaternion& q)
{
	float x2 = q.x + q.x;
	float y2 = q.y + q.y;
	float z2 = q.z + q.z;

	float wx = q.w * x2;
	float wy = q.w * y2;
	float wz = q.w * z2;

	float xx = q.x * x2;
	float xy = q.x * y2;
	float xz = q.x * z2;

	float yy = q.y * y2;
	float yz = q.y * z2;

	float zz = q.z * z2;

	m00 = 1.0f - (yy + zz);
	m01 = xy + wz;
	m02 = xz - wy;

	m10 = xy - wz;
	m11 = 1.0f - (xx + zz);
	m12 = yz + wx;

	m20 = xz + wy;
	m21 = yz - wx;
	m22 = 1.0f - (xx + yy);

	m03 = m13 = m23 = 0;
	m30 = m31 = m32 = 0;
	m33 = 1;
}

void Matrix4::set(const Vector3& axis, float angle)
{

}

void Matrix4::set(
	float t00, float t01, float t02, float t03,
	float t10, float t11, float t12, float t13,
	float t20, float t21, float t22, float t23,
	float t30, float t31, float t32, float t33)
{
	m00 = t00; m01 = t01; m02 = t02; m03 = t03;
	m10 = t10; m11 = t11; m12 = t12; m13 = t13;
	m20 = t20; m21 = t21; m22 = t22; m23 = t23;
	m30 = t30; m31 = t31; m32 = t32; m33 = t33;
}

void Matrix4::swap(Matrix4& other)
{
	std::swap(m00, other.m00);
	std::swap(m01, other.m01);
	std::swap(m02, other.m02);
	std::swap(m03, other.m03);
	std::swap(m10, other.m10);
	std::swap(m11, other.m11);
	std::swap(m12, other.m12);
	std::swap(m13, other.m13);
	std::swap(m20, other.m20);
	std::swap(m21, other.m21);
	std::swap(m22, other.m22);
	std::swap(m23, other.m23);
	std::swap(m30, other.m30);
	std::swap(m31, other.m31);
	std::swap(m32, other.m32);
	std::swap(m33, other.m33);
}

void Matrix4::getColumn(size_t col, Vector4& vec) const
{
	assert(col < 4);
	vec.set(m[0][col], m[1][col], m[2][col], m[3][col]);
}

void Matrix4::setColumn(size_t col, const Vector4& vec)
{
	assert(col < 4);
	m[0][col] = vec.x;
	m[1][col] = vec.y;
	m[2][col] = vec.z;
	m[3][col] = vec.w;
}

void Matrix4::getRow(size_t row, Vector4& v) const
{
	assert(row < 4);
	v.set(m[row][0], m[row][1], m[row][2], m[row][3]);
}

void Matrix4::setRow(size_t row, const Vector4& v)
{
	assert(row < 4);
	m[row][0] = v.x;
	m[row][1] = v.y;
	m[row][2] = v.z;
	m[row][3] = v.w;
}

void Matrix4::getMatrix3(Matrix3& mat) const
{
	mat.m00 = m00; mat.m01 = m01; mat.m02 = m02;
	mat.m10 = m10; mat.m11 = m11; mat.m12 = m12;
	mat.m20 = m20; mat.m21 = m21; mat.m22 = m22;
}

float Matrix4::determinant() const
{
	return
		m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
		m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
		m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
		m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
}

Matrix4 Matrix4::inverse() const
{
	float v0 = m20 * m31 - m21 * m30;
	float v1 = m20 * m32 - m22 * m30;
	float v2 = m20 * m33 - m23 * m30;
	float v3 = m21 * m32 - m22 * m31;
	float v4 = m21 * m33 - m23 * m31;
	float v5 = m22 * m33 - m23 * m32;

	float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
	float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
	float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
	float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

	float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	float d00 = t00 * invDet;
	float d10 = t10 * invDet;
	float d20 = t20 * invDet;
	float d30 = t30 * invDet;

	float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	return Matrix4(
		d00, d01, d02, d03,
		d10, d11, d12, d13,
		d20, d21, d22, d23,
		d30, d31, d32, d33);
}

Matrix4 Matrix4::adjoint() const
{
	return Matrix4(
		MINOR(*this, 1, 2, 3, 1, 2, 3),
		-MINOR(*this, 0, 2, 3, 1, 2, 3),
		MINOR(*this, 0, 1, 3, 1, 2, 3),
		-MINOR(*this, 0, 1, 2, 1, 2, 3),

		-MINOR(*this, 1, 2, 3, 0, 2, 3),
		MINOR(*this, 0, 2, 3, 0, 2, 3),
		-MINOR(*this, 0, 1, 3, 0, 2, 3),
		MINOR(*this, 0, 1, 2, 0, 2, 3),

		MINOR(*this, 1, 2, 3, 0, 1, 3),
		-MINOR(*this, 0, 2, 3, 0, 1, 3),
		MINOR(*this, 0, 1, 3, 0, 1, 3),
		-MINOR(*this, 0, 1, 2, 0, 1, 3),

		-MINOR(*this, 1, 2, 3, 0, 1, 2),
		MINOR(*this, 0, 2, 3, 0, 1, 2),
		-MINOR(*this, 0, 1, 3, 0, 1, 2),
		MINOR(*this, 0, 1, 2, 0, 1, 2));
}

Matrix4 Matrix4::transpose() const
{
	Matrix4 result(
		m00, m10, m20, m30,
		m01, m11, m21, m31,
		m02, m12, m22, m32,
		m03, m13, m23, m33);
	return result;
}

Vector3 Matrix4::multiplyAffine(const Vector3& v) const
{
	return Vector3(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
}

Vector4 Matrix4::multiplyAffine(const Vector4& v) const
{
	return Vector4(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
		v.w);
}

Vector3 Matrix4::multiplyDirection(const Vector3& v) const
{
	return Vector3(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
}

Vector3 Matrix4::multiply(const Vector3& v) const
{
	Vector3 r;

	float fInvW = 1.0f / (m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]);

	r.x = (m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3]) * fInvW;
	r.y = (m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3]) * fInvW;
	r.z = (m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]) * fInvW;

	return r;
}

/**
* Transform a 4D vector by this matrix.
*
* @note	If your matrix doesn't contain projection components use multiplyAffine() method as it is faster.
*/
Vector4 Matrix4::multiply(const Vector4& v) const
{
	return Vector4(
		m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
		m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
		);
}

void Matrix4::negate()
{
	mm[0] = -mm[0];
	mm[1] = -mm[1];
	mm[2] = -mm[2];
	mm[3] = -mm[3];
	mm[4] = -mm[4];
	mm[5] = -mm[5];
	mm[6] = -mm[6];
	mm[7] = -mm[7];
	mm[8] = -mm[8];
	mm[9] = -mm[9];
	mm[10] = -mm[10];
	mm[11] = -mm[11];
	mm[12] = -mm[12];
	mm[13] = -mm[13];
	mm[14] = -mm[14];
	mm[15] = -mm[15];
}

void Matrix4::decompose(Quaternion* rotation, Vector3* translation, Vector3* scale)
{
	if (translation)
	{
		translation->x = m30;
		translation->y = m31;
		translation->z = m32;
	}
	if (scale == NULL && rotation == NULL)
		return;
	// Extract the scale.
	// This is simply the length of each axis (row/column) in the matrix.
	Vector3 xaxis(m00, m01, m02);
	Vector3 yaxis(m10, m11, m12);
	Vector3 zaxis(m20, m21, m22);
	float scaleX = xaxis.length();
	float scaleY = yaxis.length();
	float scaleZ = zaxis.length();
	// Determine if we have a negative scale (true if determinant is less than zero).
	// In this case, we simply negate a single axis of the scale.
	float det = determinant();
	if (det < 0)
		scaleZ = -scaleZ;
	if (scale)
	{
		scale->set(scaleX, scaleY, scaleZ);
	}
	if (rotation == NULL)
		return;
	if (scaleX < Math::EPSILON || scaleY < Math::EPSILON || scaleZ < Math::EPSILON)
		return;
	xaxis /= scaleX;
	yaxis /= scaleY;
	zaxis /= scaleZ;

	// Now calculate the rotation from the resulting matrix (axes).
	float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;
	
	if (trace > Math::EPSILON)
	{
		float s = 0.5f / sqrt(trace);
		rotation->w = 0.25f / s;
		rotation->x = (yaxis.z - zaxis.y) * s;
		rotation->y = (zaxis.x - xaxis.z) * s;
		rotation->z = (xaxis.y - yaxis.x) * s;
	}
	else
	{
		// Note: since xaxis, yaxis, and zaxis are normalized, 
		// we will never divide by zero in the code below.
		if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
		{
			float s = 0.5f / sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
			rotation->w = (yaxis.z - zaxis.y) * s;
			rotation->x = 0.25f / s;
			rotation->y = (yaxis.x + xaxis.y) * s;
			rotation->z = (zaxis.x + xaxis.z) * s;
		}
		else if (yaxis.y > zaxis.z)
		{
			float s = 0.5f / sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
			rotation->w = (zaxis.x - xaxis.z) * s;
			rotation->x = (yaxis.x + xaxis.y) * s;
			rotation->y = 0.25f / s;
			rotation->z = (zaxis.y + yaxis.z) * s;
		}
		else
		{
			float s = 0.5f / sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y);
			rotation->w = (xaxis.y - yaxis.x) * s;
			rotation->x = (zaxis.x + xaxis.z) * s;
			rotation->y = (zaxis.y + yaxis.z) * s;
			rotation->z = 0.25f / s;
		}
	}
}

void Matrix4::setTranslation(const Vector3& t)
{
	m30 = t.x;
	m31 = t.y;
	m32 = t.z;
}

void Matrix4::setScale(const Vector3& s)
{
	m00 = s.x;
	m11 = s.y;
	m22 = s.z;
}

void Matrix4::setRotation(const Quaternion& q)
{
	float x2 = q.x + q.x;
	float y2 = q.y + q.y;
	float z2 = q.z + q.z;

	float wx = q.w * x2;
	float wy = q.w * y2;
	float wz = q.w * z2;

	float xx = q.x * x2;
	float xy = q.x * y2;
	float xz = q.x * z2;

	float yy = q.y * y2;
	float yz = q.y * z2;

	float zz = q.z * z2;

	m00 = 1.0f - (yy + zz);
	m01 = xy + wz;
	m02 = xz - wy;

	m10 = xy - wz;
	m11 = 1.0f - (xx + zz);
	m12 = yz + wx;

	m20 = xz + wy;
	m21 = yz - wx;
	m22 = 1.0f - (xx + yy);

	m03 = m13 = m23 = 0;
	m30 = m31 = m32 = 0;
	m33 = 1;
}

void Matrix4::getScale(Vector3& s) const
{
	s.set(m00, m11, m22);
}

void Matrix4::getTranslation(Vector3& t) const
{
	t.set(m30, m31, m32);
}

void Matrix4::getRotation(Quaternion& q) const
{
	q.w = 0.5f * (float)sqrt((std::max)(0.0, 1.0 + m00 + m11 + m22));
	q.x = 0.5f * (float)sqrt((std::max)(0.0, 1.0 + m00 - m11 - m22));
	q.y = 0.5f * (float)sqrt((std::max)(0.0, 1.0 - m00 + m11 - m22));
	q.z = 0.5f * (float)sqrt((std::max)(0.0, 1.0 - m00 - m11 + m22));

	q.x = q.x * Math::sign(m12 - m21);
	q.y = q.y * Math::sign(m20 - m02);
	q.z = q.z * Math::sign(m01 - m10);
}

String Matrix4::toString() const
{
	char buf[256];
	snprintf(buf, sizeof(buf), "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g",
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33);
	return String(buf);
}

/** Building a Matrix4 from orientation / scale / position.
@remarks
Transform is performed in the order scale, rotate, translation, i.e. translation is independent
of orientation axes, scale does not affect size of translation, rotation and scaling are always
centered on the origin.
*/
void Matrix4::setTransform(const Vector3& p, const Vector3& s, const Quaternion& q)
{
	// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate

	Matrix3 r;
	q.toRotationMatrix(r);

	// Set up final matrix with scale, rotation and translation
	// No projection term
	m00 = s.x * r.m00; m01 = s.y*r.m01; m02 = s.z*r.m02; m03 = p.x;
	m10 = s.x * r.m10; m11 = s.y*r.m11; m12 = s.z*r.m12; m13 = p.x;
	m20 = s.x * r.m20; m21 = s.y*r.m21; m22 = s.z*r.m22; m23 = p.x;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
}

/** Building an inverse Matrix4 from orientation / scale / position.
@remarks
As makeTransform except it build the inverse given the same data as makeTransform, so
performing -translation, -rotate, 1/scale in that order.
*/
void Matrix4::setInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
{
	// Invert the parameters
	// Because we're inverting, order is translation, rotation, scale
	// So make translation relative to scale & rotation
	Quaternion	invR = orientation.inverse();
	Vector3		s(1 / scale.x, 1 / scale.y, 1 / scale.z);
	Vector3		p = invR.rotate(-position);
	p *= s;

	Matrix3 r;
	invR.toRotationMatrix(r);

	// Set up final matrix with scale, rotation and translation
	// No projection term
	m00 = s.x * r.m00; m01 = s.x * r.m01; m02 = s.x * r.m02; m03 = p.x;
	m10 = s.y * r.m10; m11 = s.y * r.m11; m12 = s.y * r.m12; m13 = p.x;
	m20 = s.z * r.m20; m21 = s.z * r.m21; m22 = s.z * r.m22; m23 = p.x;
	m30 = 0; m31 = 0; m32 = 0; m33 = 1;
}

void Matrix4::setOrthoRH(float width, float height, float zNear, float zFar)
{
	float q = 1 / (zFar - zNear);
	this->set(
		2.0f / width, 0.0, 0.0, 0,
		0.0, 2.0f / height, 0.0, 0,
		0.0, 0.0, -q, 0,
		0.0, 0.0, zNear * q, 1.0f);
}

void Matrix4::setOrthoLH(float width, float height, float zNear, float zFar)
{
	float q = 1 / (zFar - zNear);

	this->set(
		2.0f / width, 0.0, 0, 0,
		0.0, 2.0f / height, 0, 0,
		0.0, 0.0, q, 0,
		0.0, 0.0, -zNear*q, 1.0f);
}

void Matrix4::setOrthoOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float q = 1 / (zFar - zNear);
	float invW = 1 / (right - left);
	float invH = 1 / (top - bottom);
	float x = invW + invW;
	float y = invH + invH;

	this->set(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, -q, 0,
		-(left + right)*invW, -(top + bottom)*invH, -zNear*q, 1);
}

void Matrix4::setOrthoOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float q = 1 / (zFar - zNear);
	float invW = 1 / (right - left);
	float invH = 1 / (top - bottom);
	float x = invW + invW;
	float y = invH + invH;

	this->set(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, q, 0,
		-(left + right)*invW, -(top + bottom)*invH, -zNear*q, 1);
}

void Matrix4::setPerspectiveRH(float width, float height, float zNear, float zFar)
{
	float q = zFar / (zFar - zNear);
	float n = zNear + zNear;
	this->set(
		n / width, 0, 0, 0,
		0, n / height, 0, 0,
		0, 0, -q, -1,
		0, 0, -zNear*q, 0);
}

void Matrix4::setPerspectiveLH(float width, float height, float zNear, float zFar)
{
	float q = zFar / (zFar - zNear);
	float n = zNear + zNear;
	this->set(
		n / width, 0, 0, 0,
		0, n / height, 0, 0,
		0, 0, q, 1,
		0, 0, -zNear*q, 0);
}

void Matrix4::setPerspectiveFovRH(float fovy, float aspect, float zNear, float zFar)
{
	float h = 1 / Math::tan(fovy / 2.0f);
	float w = h / aspect;
	float q = zFar / (zFar - zNear);

	this->set(
		w, 0, 0, 0,
		0, h, 0, 0,
		0, 0, -q, -1,
		0, 0, -zNear * q, 0);
}

void Matrix4::setPerspectiveFovLH(float fovy, float aspect, float zNear, float zFar)
{
	float h = 1 / Math::tan(fovy / 2.0f);
	float w = h / aspect;
	float q = zFar / (zFar - zNear);

	this->set(
		w, 0, 0, 0,
		0, h, 0, 0,
		0, 0, q, 1,
		0, 0, -zNear * q, 0);
}

void Matrix4::setPerspectiveOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float q = zFar / (zFar - zNear);
	float n = zNear + zNear;
	float invW = 1 / (right - left);
	float invH = 1 / (top - bottom);

	float w = right + left;
	float h = top + bottom;

	this->set(
		n * invW, 0, 0, 0,
		0, n * invH, 0, 0,
		w * invW, h * invH, -q, -1,
		0, 0, -zNear*q, 0);
}

void Matrix4::setPerspectiveOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float q = zFar / (zFar - zNear);
	float n = zNear + zNear;
	float invW = 1 / (right - left);
	float invH = 1 / (top - bottom);

	float w = right + left;
	float h = top + bottom;

	this->set(
		n * invW, 0, 0, 0,
		0, n * invH, 0, 0,
		-w * invW, -h * invH, q, 1,
		0, 0, -zNear*q, 0);
}

void Matrix4::setLookAtRH(const Vector3& eye, const Vector3& at, const Vector3& up)
{
	Vector3 zAxis = Vector3::normalize(eye - at);
	Vector3 xAxis = Vector3::normalize(Vector3::cross(up, zAxis));
	Vector3 yAxis = Vector3::cross(zAxis, xAxis);

	float dotX = -Vector3::dot(xAxis, eye);
	float dotY = -Vector3::dot(yAxis, eye);
	float dotZ = -Vector3::dot(zAxis, eye);

	this->set(
		xAxis.x, yAxis.x, zAxis.x, 0,
		xAxis.y, yAxis.y, zAxis.y, 0,
		xAxis.z, yAxis.z, zAxis.z, 0,
		dotX, dotY, dotZ, 1);
}

void Matrix4::setLookAtLH(const Vector3& eye, const Vector3& at, const Vector3& up)
{
	//diffrent: left-hand(at - eye) vs right-hand(eye -at)
	Vector3 zAxis = Vector3::normalize(at - eye);
	Vector3 xAxis = Vector3::normalize(Vector3::cross(up, zAxis));
	Vector3 yAxis = Vector3::cross(zAxis, xAxis);

	float dotX = -Vector3::dot(xAxis, eye);
	float dotY = -Vector3::dot(yAxis, eye);
	float dotZ = -Vector3::dot(zAxis, eye);

	this->set(
		xAxis.x, yAxis.x, zAxis.x, 0,
		xAxis.y, yAxis.y, zAxis.y, 0,
		xAxis.z, yAxis.z, zAxis.z, 0,
		dotX, dotY, dotZ, 1);
}

Matrix4 Matrix4::operator -() const
{
	Matrix4 r = *this;
	r.negate();
	return r;
}

Matrix4& Matrix4::operator +=(const Matrix4& rhs)
{
	m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
	m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m03 += rhs.m13;
	m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
	m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
	return *this;
}

Matrix4& Matrix4::operator -=(const Matrix4& rhs)
{
	m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
	m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m03 -= rhs.m13;
	m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
	m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const
{
	Matrix4 r;
	r.m00 = m00*rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30;
	r.m01 = m00*rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31;
	r.m02 = m00*rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32;
	r.m03 = m00*rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33;

	r.m10 = m10*rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30;
	r.m11 = m10*rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31;
	r.m12 = m10*rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32;
	r.m13 = m10*rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33;


	r.m20 = m20*rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30;
	r.m21 = m20*rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31;
	r.m22 = m20*rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32;
	r.m23 = m20*rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33;

	r.m30 = m30*rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30;
	r.m31 = m30*rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31;
	r.m32 = m30*rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32;
	r.m33 = m30*rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33;
	return r;
}

Matrix4& Matrix4::operator *=(const Matrix4& rhs)
{
	*this = (*this) * rhs;
	return *this;
}

Matrix4& Matrix4::operator +=(float rhs)
{
	m00 += rhs; m01 += rhs; m02 += rhs; m03 += rhs;
	m10 += rhs; m11 += rhs; m12 += rhs; m03 += rhs;
	m20 += rhs; m21 += rhs; m22 += rhs; m23 += rhs;
	m30 += rhs; m31 += rhs; m32 += rhs; m33 += rhs;
	return *this;
}

Matrix4& Matrix4::operator -=(float rhs)
{
	*this += (-rhs);
	return *this;
}

Matrix4& Matrix4::operator *=(float rhs)
{
	if (rhs == 1)
		return *this;
	m00 *= rhs; m01 *= rhs; m02 *= rhs; m03 *= rhs;
	m10 *= rhs; m11 *= rhs; m12 *= rhs; m03 *= rhs;
	m20 *= rhs; m21 *= rhs; m22 *= rhs; m23 *= rhs;
	m30 *= rhs; m31 *= rhs; m32 *= rhs; m33 *= rhs;
	return *this;
}

Matrix4& Matrix4::operator /=(float rhs)
{
	if (rhs == 0)
		return *this;
	*this *= (1 / rhs);
	return *this;
}

CUTE_NS_END
