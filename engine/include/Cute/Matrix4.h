#pragma once
#include "Cute/Foundation.h"
#include "Cute/Matrix3.h"
#include "Cute/Vector4.h"
#include "Cute/Quaternion.h"

CUTE_NS_BEGIN

// 注意：Matrix默认构造函数并没有初始化数据,如果想初始化为零需要用,Matrix(0.0f)初始化，或者set(0.0f)
// dx左手坐标系，gl右手坐标系
// unity:左手坐标系, ogre:右手坐标系
// projection推导 http://www.nshen.net/article/2014-10-16/stage3d-projection-matrix/
class CUTE_CORE_API Matrix4
{
public:
	static Matrix4 ortho(float width, float height, float zNear, float zFar);
	static Matrix4 orthoOffCenter(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 perspective(float width, float height, float zNear, float zFar);
	static Matrix4 perspectiveFov(float fovy, float aspect, float zNear, float zFar);
	static Matrix4 perspectiveOffCenter(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 lookAt(const Vector3& eye, const Vector3& at, const Vector3& up = Vector3::UNIT_Y);

	static Matrix4 orthoRH(float width, float height, float zNear, float zFar);
	static Matrix4 orthoOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 perspectiveRH(float width, float height, float zNear, float zFar);
	static Matrix4 perspectiveFovRH(float fovy, float aspect, float zNear, float zFar);
	static Matrix4 perspectiveOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 lookAtRH(const Vector3& eye, const Vector3& at, const Vector3& up = Vector3::UNIT_Y);

	static Matrix4 orthoLH(float width, float height, float zNear, float zFar);
	static Matrix4 orthoOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 perspectiveLH(float width, float height, float zNear, float zFar);
	static Matrix4 perspectiveFovLH(float fovy, float aspect, float zNear, float zFar);
	static Matrix4 perspectiveOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar);
	static Matrix4 lookAtLH(const Vector3& eye, const Vector3& at, const Vector3& up = Vector3::UNIT_Y);

public:
	enum { ARRAY_SIZE = 16 };
	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;

	Matrix4();
	Matrix4(float t);
	Matrix4(const Matrix3& mat3);
	Matrix4(const Quaternion& q);
	Matrix4(const Vector3& axis, float angle);
	Matrix4(const Vector3& t, const Vector3& s, const Quaternion& r);
	Matrix4(
		float t00, float t01, float t02, float t03,
		float t10, float t11, float t12, float t13,
		float t20, float t21, float t22, float t23,
		float t30, float t31, float t32, float t33);

	void set(float t);
	void set(const Matrix3& mat3);
	void set(const Quaternion& q);
	void set(const Vector3& axis, float angle);
	void set(
		float t00, float t01, float t02, float t03,
		float t10, float t11, float t12, float t13,
		float t20, float t21, float t22, float t23,
		float t30, float t31, float t32, float t33);

	void swap(Matrix4& other);
	void flipHand();

	void getColumn(size_t col, Vector4& vec) const;
	void setColumn(size_t col, const Vector4& vec);
	void getRow(size_t row, Vector4& v) const;
	void setRow(size_t row, const Vector4& v);
	void getMatrix3(Matrix3& mat) const;

	void	negate();				// 取反
	float   determinant() const;	// 行列式
	Matrix4 inverse() const;		// 逆矩阵
	Matrix4 adjoint() const;		// 共轭矩阵，伴随矩阵
	Matrix4 transpose() const;		// 转置矩阵

	Vector3 multiply(const Vector3& v) const;
	Vector4 multiply(const Vector4& v) const;
	Vector3 multiplyAffine(const Vector3& v) const;
	Vector4 multiplyAffine(const Vector4& v) const;
	Vector3 multiplyDirection(const Vector3& v) const;

	void decompose(Quaternion* rotation, Vector3* translation, Vector3* scale);

	void setScale(const Vector3& s);
	void setTranslation(const Vector3& t);
	void setRotation(const Quaternion& q);

	void getScale(Vector3& s) const;
	void getTranslation(Vector3& t) const;
	void getRotation(Quaternion& q) const;

	void setTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);
	void setInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);

	void setOrthoRH(float width, float height, float zNear, float zFar);
	void setOrthoLH(float width, float height, float zNear, float zFar);
	void setOrthoOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar);
	void setOrthoOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar);
	void setPerspectiveRH(float width, float height, float zNear, float zFar);
	void setPerspectiveLH(float width, float height, float zNear, float zFar);
	void setPerspectiveFovRH(float fovy, float aspect, float zNear, float zFar);
	void setPerspectiveFovLH(float fovy, float aspect, float zNear, float zFar);
	void setPerspectiveOffCenterRH(float left, float right, float bottom, float top, float zNear, float zFar);
	void setPerspectiveOffCenterLH(float left, float right, float bottom, float top, float zNear, float zFar);
	void setLookAtRH(const Vector3& eye, const Vector3& at, const Vector3& up = Vector3::UNIT_Y);
	void setLookAtLH(const Vector3& eye, const Vector3& at, const Vector3& up = Vector3::UNIT_Y);

	bool isAffine() const { return (m30 == 0) && (m31 == 0) && (m32 == 0) && (m33 == 1); }

	String toString() const;

	inline size_t size() const { return sizeof(*this); }
	inline const float* data() const { return (const float*)m; }
	inline float* data() { return (float*)m; }
	inline float at(size_t index) const { return ((float*)m)[index]; }

	inline const float* operator[](size_t index) const { return m[index]; }
	inline float* operator[](size_t index) { return m[index]; }

	Matrix4& operator=(const float* t) { memcpy(m, t, sizeof(m)); return *this; }

	Matrix4 operator-() const;
	Matrix4& operator+=(const Matrix4& rhs);
	Matrix4& operator-=(const Matrix4& rhs);

	Matrix4& operator*=(const Matrix4& rhs);
	Matrix4  operator* (const Matrix4& rhs) const;

	Matrix4& operator+=(float rhs);
	Matrix4& operator-=(float rhs);
	Matrix4& operator*=(float rhs);
	Matrix4& operator/=(float rhs);

	friend bool operator==(const Matrix4& lhs, const Matrix4& rhs) { return memcmp(&lhs, &rhs, sizeof(lhs)) == 0; }
	friend bool operator!=(const Matrix4& lhs, const Matrix4& rhs) { return memcmp(&lhs, &rhs, sizeof(lhs)) != 0; }

	friend Matrix4 operator+(Matrix4 lhs, const Matrix4& rhs) { return lhs += rhs; }
	friend Matrix4 operator-(Matrix4 lhs, const Matrix4& rhs) { return lhs -= rhs; }

	friend Matrix4 operator+(Matrix4 lhs, const float& rhs) { return lhs += rhs; }
	friend Matrix4 operator-(Matrix4 lhs, const float& rhs) { return lhs -= rhs; }
	friend Matrix4 operator*(Matrix4 lhs, const float& rhs) { return lhs *= rhs; }
	friend Matrix4 operator/(Matrix4 lhs, const float& rhs) { return lhs /= rhs; }
public:
	union
	{
		float m[4][4];
		float mm[16];
		struct
		{
#if (MATRIX_MAJOR == MATRIX_MAJOR_ROW)
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
#else
			float m00, m10, m20, m30;
			float m01, m11, m21, m31;
			float m02, m12, m22, m32;
			float m03, m13, m23, m33;
#endif
		};
	};
};

CUTE_NS_END
