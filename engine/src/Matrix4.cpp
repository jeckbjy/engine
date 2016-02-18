#include "Matrix4.h"
#include "CMath.h"

CU_NS_BEGIN

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

inline float radians(const float& degrees)
{
	const float degree_2_radian = 3.1415926535897932384626433832795f / 180.0f;
	return degrees * degree_2_radian;
}

Matrix4 Matrix4::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Matrix4 result;
	return result;
}

Matrix4 Matrix4::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float range = tan(radians(fovy / 2)) * zNear;
	float left = -range * aspect;
	float right = range * aspect;
	float bottom = -range;
	float top = range;

	Matrix4 result(0.0f);
	result[0][0] = (2 * zNear) / (right - left);
	result[1][1] = (2 * zNear) / (top - bottom);
	result[2][2] = -(zFar + zNear) / (zFar - zNear);
	result[2][3] = -1;
	result[3][2] = -(2 * zFar*zNear) / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::perspectiveFov(float fov, float width, float height, float zNear, float zFar)
{
	float rad = radians(fov);
	float h = cos(0.5f * rad) / sin(0.5f * rad);
	float w = h * height / width; ///todo max(width , Height) / min(width , Height)?

	Matrix4 result(0.0f);
	result[0][0] = w;
	result[1][1] = h;
	result[2][2] = -(zFar + zNear) / (zFar - zNear);
	result[2][3] = -(1);
	result[3][2] = -((2) * zFar * zNear) / (zFar - zNear);
	return result;
}

Matrix4 Matrix4::lookAt(Vector3 const& eye, Vector3 const& center, Vector3 const& up)
{
	Vector3 f = Vector3::normalize(center - eye);
	Vector3 u = Vector3::normalize(up);
	Vector3 s = Vector3::normalize(Vector3::cross(f, u));
	u = Vector3::cross(s, f);

	return Matrix4(
		s.x, u.x, -f.x, 0,
		s.y, u.y, -f.y, 0,
		s.z, u.z, -f.z, 0,
		-Vector3::dot(s, eye), -Vector3::dot(u, eye), Vector3::dot(f, eye), 1);
}

Matrix4 Matrix4::lookAtLH(Vector3 const& eye, Vector3 const& center, Vector3 const& up)
{
	Vector3 z = Vector3::normalize(center - eye);
	Vector3 x = Vector3::normalize(Vector3::cross(up, z));
	Vector3 y = Vector3::cross(z, x);

	return Matrix4(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		-Vector3::dot(x, eye), -Vector3::dot(y, eye), -Vector3::dot(z, eye), 1);
}

Matrix4 Matrix4::lookAtRH(Vector3 const& eye, Vector3 const& center, Vector3 const& up)
{
	Vector3 z = Vector3::normalize(eye - center);
	Vector3 x = Vector3::normalize(Vector3::cross(up, z));
	Vector3 y = Vector3::cross(z, x);

	return Matrix4(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		-Vector3::dot(x, eye), -Vector3::dot(y, eye), -Vector3::dot(z, eye), 1);
}

Matrix4 Matrix4::createTranslation(const Vector3& t)
{
	Matrix4 r;
	r.m30 = t.x;
	r.m31 = t.y;
	r.m32 = t.z;
	return r;
}

Matrix4 Matrix4::createScale(const Vector3& s)
{
	Matrix4 r;
	r.m00 = s.x;
	r.m11 = s.y;
	r.m22 = s.z;
	return r;
}

Matrix4 Matrix4::createRotation(const Quaternion& q)
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

	Matrix4 r;
	r.m00 = 1.0f - (yy + zz);
	r.m01 = xy + wz;
	r.m02 = xz - wy;

	r.m10 = xy - wz;
	r.m11 = 1.0f - (xx + zz);
	r.m12 = yz + wx;

	r.m20 = xz + wy;
	r.m21 = yz - wx;
	r.m22 = 1.0f - (xx + yy);

	r.m03 = r.m13 = r.m23 = 0;
	r.m30 = r.m31 = r.m32 = 0;
	r.m33 = 1;

	return r;
}

Matrix4 Matrix4::createRotation(const Vector3& axis, float angle)
{
	Matrix4 r(0.0f);
	return r;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
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
	m[0][0] = mat3[0][0]; m[0][1] = mat3[0][1]; m[0][2] = mat3[0][2]; m[0][3] = 0.0f;
	m[1][0] = mat3[1][0]; m[1][1] = mat3[1][1]; m[1][2] = mat3[1][2]; m[1][3] = 0.0f;
	m[2][0] = mat3[2][0]; m[2][1] = mat3[2][1]; m[2][2] = mat3[2][2]; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
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

Matrix4& Matrix4::scale(const Vector3& s)
{
	Matrix4 m = createScale(s);
	(*this) *= m;
	return *this;
}

Matrix4& Matrix4::translate(const Vector3& t)
{
	Matrix4 m = createTranslation(t);
	(*this) *= m;
	return *this;
}

Matrix4& Matrix4::rotate(const Quaternion& q)
{
	Matrix4 m = createRotation(q);
	(*this) *= m;
	return *this;
}

Matrix4& Matrix4::rotate(const Vector3& axis, float angle)
{
	Matrix4 m = createRotation(axis, angle);
	(*this) *= m;
	return *this;
}

void Matrix4::negate()
{
	_m[0] = -_m[0];
	_m[1] = -_m[1];
	_m[2] = -_m[2];
	_m[3] = -_m[3];
	_m[4] = -_m[4];
	_m[5] = -_m[5];
	_m[6] = -_m[6];
	_m[7] = -_m[7];
	_m[8] = -_m[8];
	_m[9] = -_m[9];
	_m[10] = -_m[10];
	_m[11] = -_m[11];
	_m[12] = -_m[12];
	_m[13] = -_m[13];
	_m[14] = -_m[14];
	_m[15] = -_m[15];
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
	if (scaleX < MATH_TOLERANCE || scaleY < MATH_TOLERANCE || scaleZ < MATH_TOLERANCE)
		return;
	xaxis /= scaleX;
	yaxis /= scaleY;
	zaxis /= scaleZ;

	// Now calculate the rotation from the resulting matrix (axes).
	float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

	if (trace > MATH_EPSILON)
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

void Matrix4::setTransform(const Vector3& t, const Vector3& s, const Quaternion& r)
{
	setTranslation(t);
	rotate(r);
	scale(s);
}

void Matrix4::setInverseTransform(const Vector3& t, const Vector3& s, const Quaternion& r)
{

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

Quaternion Matrix4::getRotation() const
{
	Quaternion q;
	q.w = 0.5 * sqrt((std::max)(0.0, 1.0 + m00 + m11 + m22));
	q.x = 0.5 * sqrt((std::max)(0.0, 1.0 + m00 - m11 - m22));
	q.y = 0.5 * sqrt((std::max)(0.0, 1.0 - m00 + m11 - m22));
	q.z = 0.5 * sqrt((std::max)(0.0, 1.0 - m00 - m11 + m22));

	q.x = q.x * Math::sign(m12 - m21);
	q.y = q.y * Math::sign(m20 - m02);
	q.z = q.z * Math::sign(m01 - m10);
	return q;
}

void Matrix4::toMatrix3(Matrix3& mat3) const
{
	mat3[0][0] = m[0][0];
	mat3[0][1] = m[0][1];
	mat3[0][2] = m[0][2];
	mat3[1][0] = m[1][0];
	mat3[1][1] = m[1][1];
	mat3[1][2] = m[1][2];
	mat3[2][0] = m[2][0];
	mat3[2][1] = m[2][1];
	mat3[2][2] = m[2][2];
}

void Matrix4::setPerspective(float fovy, float aspectRatio, float zNear, float zFar)
{

}

bool Matrix4::getPerspective(float& fovy, float& aspectRatio, float& zNear, float& zFar) const
{
	return true;
}

void Matrix4::setOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m00 = 2.0 / (right - left);
	m11 = 2.0 / (top - bottom);
	m22 = -2.0 / (zFar - zNear);

	m30 = -(right + left) / (right - left);
	m31 = -(top + bottom) / (top - bottom);
	m32 = -(zFar + zNear) / (zFar - zNear);

	m01 = m02 = m03 = 0;
	m10 = m12 = m13 = 0;
	m20 = m21 = m23 = 0;
	m33 = 1;
}

bool Matrix4::getOrtho(float& left, float& right, float& bottom, float& top, float& zNear, float& zFar) const
{
	if (!isAffine())
		return false;
	zNear = (m32 + 1.0) / m22;
	zFar = (m32 - 1.0) / m22;

	left = -(1.0 + m30) / m00;
	right = (1.0 + m30) / m00;

	bottom = -(1.0 + m31) / m11;
	top = (1.0 - m31) / m11;

	return true;
}

void Matrix4::setFrustum(float left, float right, float bottom, float top, float zNear, float zFar)
{

}

bool Matrix4::getFrustum(float& left, float& right, float& bottom, float& top, float& zNear, float& zFar) const
{
	return true;
}

void Matrix4::setLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{

}

void Matrix4::getLookAt(Vector3& eye, Vector3& center, Vector3& up, float lookDistance) const
{

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

CU_NS_END