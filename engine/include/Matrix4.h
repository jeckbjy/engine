#pragma once
#include "Matrix3.h"
#include "Quaternion.h"

CU_NS_BEGIN

//#define ROW_MAJOR
////#define COL_MAJOR
//#define MATRIX_SIZE (sizeof(float) * 16)

// 行存储 to 列存储??
class CU_API Matrix4
{
public:
	static const Matrix4& zero() { static Matrix4 mat(0.0f); return mat; }
	static const Matrix4& identity(){ static Matrix4 mat(1.0f);  return mat; }
	// 创建正交矩阵
	static Matrix4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	// 创建透视矩阵
	static Matrix4 perspective(float fovy, float aspect, float zNear, float zFar);
	static Matrix4 perspectiveFov(float fov, float width, float height, float zNear, float zFar);
	static Matrix4 lookAt(Vector3 const& eye, Vector3 const& center, Vector3 const& up);
	static Matrix4 lookAtLH(Vector3 const& eye, Vector3 const& center, Vector3 const& up);
	static Matrix4 lookAtRH(Vector3 const& eye, Vector3 const& center, Vector3 const& up);

	static Matrix4 createTranslation(const Vector3& t);
	static Matrix4 createScale(const Vector3& s);
	static Matrix4 createRotation(const Quaternion& q);
	static Matrix4 createRotation(const Vector3& axis, float angle);
public:
	enum { array_size = 16 };
	Matrix4() {};
	Matrix4(float t);
	Matrix4(
		float t00, float t01, float t02, float t03,
		float t10, float t11, float t12, float t13,
		float t20, float t21, float t22, float t23,
		float t30, float t31, float t32, float t33);
	Matrix4(const Matrix3& mat3);
	Matrix4(const Vector3& t, const Vector3& s, const Quaternion& r);

	void set(float t);
	void set(
		float t00, float t01, float t02, float t03,
		float t10, float t11, float t12, float t13,
		float t20, float t21, float t22, float t23,
		float t30, float t31, float t32, float t33);

	void negate();					// 取反
	float   determinant() const;	// 行列式
	Matrix4 inverse() const;		// 逆矩阵
	Matrix4 adjoint() const;		// 共轭矩阵，伴随矩阵
	Matrix4 transpose() const;		// 转置矩阵

	Matrix4& scale(const Vector3& s);
	Matrix4& translate(const Vector3& t);
	Matrix4& rotate(const Quaternion& q);
	Matrix4& rotate(const Vector3& axis, float angle);

	void decompose(Quaternion* rotation, Vector3* translation, Vector3* scale);

	void setTransform(const Vector3& t, const Vector3& s, const Quaternion& r);
	void setInverseTransform(const Vector3& t, const Vector3& s, const Quaternion& r);

	void setTranslation(const Vector3& t);
	void setScale(const Vector3& s);
	void setRotation(const Quaternion& q);

	Vector3 getTranslation() const { return Vector3(m30, m31, m32); }
	Vector3 getScale() const { return Vector3(m00, m11, m22); }
	Quaternion getRotation() const;

	void toMatrix3(Matrix3& mat3) const;

	void setPerspective(float fovy, float aspectRatio, float zNear, float zFar);
	bool getPerspective(float& fovy, float& aspectRatio, float& zNear, float& zFar) const;

	void setOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	bool getOrtho(float& left, float& right, float& bottom, float& top, float& zNear, float& zFar) const;

	void setFrustum(float left, float right, float bottom, float top, float zNear, float zFar);
	bool getFrustum(float& left, float& right, float& bottom, float& top, float& zNear, float& zFar) const;

	void setLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	void getLookAt(Vector3& eye, Vector3& center, Vector3& up, float lookDistance) const;

	bool isAffine() const { return (m03 == 0) && (m13 == 0) && (m23 == 0) && (m33 == 1); }

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
		float _m[16];
		struct  
		{
#ifdef COL_MAJOR
			float m00, m10, m20, m30;
			float m01, m11, m21, m31;
			float m02, m12, m22, m32;
			float m03, m13, m23, m33;
#else
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
#endif
		};
	};
};

CU_NS_END