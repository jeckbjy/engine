//! Math
#include "Cute/Matrix3x4.h"

CUTE_NS_BEGIN

const Matrix3x4 Matrix3x4::ZERO(
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f);

const Matrix3x4 Matrix3x4::IDENTITY;

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
{
	Matrix3 mat;
	rotation.toRotationMatrix(mat);
	mat.scaled(scale);
	setTranslation(translation);
}

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale)
{
	Matrix3 mat;
	rotation.toRotationMatrix(mat);
	mat *= scale;
	setTranslation(translation);
}

Matrix3 Matrix3x4::getRotationMatrix() const
{
	Vector3 invScale(
		1.0f / sqrtf(m00 * m00 + m10 * m10 + m20 * m20),
		1.0f / sqrtf(m01 * m01 + m11 * m11 + m21 * m21),
		1.0f / sqrtf(m02 * m02 + m12 * m12 + m22 * m22)
		);
	return toMatrix3().scaled(invScale);
}

Vector3 Matrix3x4::getScale() const
{
	return Vector3(
		sqrtf(m00 * m00 + m10 * m10 + m20 * m20),
		sqrtf(m01 * m01 + m11 * m11 + m21 * m21),
		sqrtf(m02 * m02 + m12 * m12 + m22 * m22)
		);
}

Matrix3x4 Matrix3x4::inverse() const
{
	float det =
		m00 * m11 * m22 +
		m10 * m21 * m02 +
		m20 * m01 * m12 -
		m20 * m11 * m02 -
		m10 * m01 * m22 -
		m00 * m21 * m12;

	float invDet = 1.0f / det;
	Matrix3x4 ret;

	ret.m00 = (m11 * m22 - m21 * m12) * invDet;
	ret.m01 = -(m01 * m22 - m21 * m02) * invDet;
	ret.m02 = (m01 * m12 - m11 * m02) * invDet;
	ret.m03 = -(m03 * ret.m00 + m13 * ret.m01 + m23 * ret.m02);
	ret.m10 = -(m10 * m22 - m20 * m12) * invDet;
	ret.m11 = (m00 * m22 - m20 * m02) * invDet;
	ret.m12 = -(m00 * m12 - m10 * m02) * invDet;
	ret.m13 = -(m03 * ret.m10 + m13 * ret.m11 + m23 * ret.m12);
	ret.m20 = (m10 * m21 - m20 * m11) * invDet;
	ret.m21 = -(m00 * m21 - m20 * m01) * invDet;
	ret.m22 = (m00 * m11 - m10 * m01) * invDet;
	ret.m23 = -(m03 * ret.m20 + m13 * ret.m21 + m23 * ret.m22);

	return ret;
}

void Matrix3x4::decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
{
	translation.x = m03;
	translation.y = m13;
	translation.z = m23;

	scale.x = sqrtf(m00 * m00 + m10 * m10 + m20 * m20);
	scale.y = sqrtf(m01 * m01 + m11 * m11 + m21 * m21);
	scale.z = sqrtf(m02 * m02 + m12 * m12 + m22 * m22);

	Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
	rotation = Quaternion(toMatrix3().scaled(invScale));
}

String Matrix3x4::toString() const
{
	char buf[256];
	snprintf(buf, sizeof(buf), "%g %g %g %g %g %g %g %g %g %g %g %g",
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23);
	return String(buf);
}

Vector3 Matrix3x4::operator *(const Vector3& rhs) const
{
	return Vector3(
		m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03,
		m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13,
		m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23);
}

Vector3 Matrix3x4::operator *(const Vector4& rhs) const
{
	return Vector3(
		m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03 * rhs.w,
		m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13 * rhs.w,
		m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23 * rhs.w);
}

Matrix4 Matrix3x4::operator *(const Matrix4& rhs) const
{
	return Matrix4(
		m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
		m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
		m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
		m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,
		m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
		m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
		m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
		m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,
		m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
		m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
		m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
		m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,
		rhs.m30,
		rhs.m31,
		rhs.m32,
		rhs.m33
		);
}

Matrix3x4 Matrix3x4::operator *(const Matrix3x4& rhs) const
{
	return Matrix3x4(
		m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
		m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
		m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
		m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03,
		m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
		m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
		m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
		m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13,
		m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
		m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
		m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22,
		m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23
		);
}

Matrix3x4& Matrix3x4::operator +=(const Matrix3x4& rhs)
{
	m00 += rhs.m00;
	m01 += rhs.m01;
	m02 += rhs.m02;
	m03 += rhs.m03;
	m10 += rhs.m10;
	m11 += rhs.m11;
	m12 += rhs.m12;
	m13 += rhs.m13;
	m20 += rhs.m20;
	m21 += rhs.m21;
	m22 += rhs.m22;
	m23 += rhs.m23;

	return *this;
}

Matrix3x4& Matrix3x4::operator -=(const Matrix3x4& rhs)
{
	m00 -= rhs.m00;
	m01 -= rhs.m01;
	m02 -= rhs.m02;
	m03 -= rhs.m03;
	m10 -= rhs.m10;
	m11 -= rhs.m11;
	m12 -= rhs.m12;
	m13 -= rhs.m13;
	m20 -= rhs.m20;
	m21 -= rhs.m21;
	m22 -= rhs.m22;
	m23 -= rhs.m23;

	return *this;
}

Matrix3x4& Matrix3x4::operator *=(float rhs)
{
	m00 *= rhs;
	m01 *= rhs;
	m02 *= rhs;
	m03 *= rhs;
	m10 *= rhs;
	m11 *= rhs;
	m12 *= rhs;
	m13 *= rhs;
	m20 *= rhs;
	m21 *= rhs;
	m22 *= rhs;
	m23 *= rhs;

	return *this;
}

CUTE_NS_END
