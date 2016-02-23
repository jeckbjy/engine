#pragma once
#include "Matrix4.h"

CU_NS_BEGIN

class CU_API Matrix3x4
{
public:
	static const Matrix3x4 ZERO;
	static const Matrix3x4 IDENTITY;

	Matrix3x4()
		: m00(1.0f), m01(0.0f), m02(0.0f), m03(0.0f)
		, m10(0.0f), m11(1.0f), m12(0.0f), m13(0.0f)
		, m20(0.0f), m21(0.0f), m22(1.0f), m23(0.0f)
	{}
	Matrix3x4(const Matrix3& mat)
		: m00(mat.m00), m01(mat.m01), m02(mat.m02), m03(0.0f)
		, m10(mat.m10), m11(mat.m11), m12(mat.m12), m13(0.0f)
		, m20(mat.m20), m21(mat.m21), m22(mat.m22), m23(0.0f)
	{}
	Matrix3x4(const Matrix4& mat)
		: m00(mat.m00), m01(mat.m01), m02(mat.m02), m03(mat.m03)
		, m10(mat.m10), m11(mat.m11), m12(mat.m12), m13(mat.m13)
		, m20(mat.m20), m21(mat.m21), m22(mat.m22), m23(mat.m23)
	{}

	Matrix3x4(
		float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23)
		: m00(v00), m01(v01), m02(v02), m03(v03)
		, m10(v10), m11(v11), m12(v12), m13(v13)
		, m20(v20), m21(v21), m22(v22), m23(v23)
	{}
	Matrix3x4(const float* data)
		: m00(data[0]), m01(data[1]), m02(data[2]), m03(data[3])
		, m10(data[4]), m11(data[5]), m12(data[6]), m13(data[7])
		, m20(data[8]), m21(data[9]), m22(data[10]), m23(data[11])
	{}

	Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale);
	Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale);

	void setTranslation(const Vector3& translation)
	{
		m03 = translation.x;
		m13 = translation.y;
		m23 = translation.z;
	}

	void setRotation(const Matrix3& rotation)
	{
		m00 = rotation.m00;
		m01 = rotation.m01;
		m02 = rotation.m02;
		m10 = rotation.m10;
		m11 = rotation.m11;
		m12 = rotation.m12;
		m20 = rotation.m20;
		m21 = rotation.m21;
		m22 = rotation.m22;
	}

	void setScale(const Vector3& scale)
	{
		m00 = scale.x;
		m11 = scale.y;
		m22 = scale.z;
	}

	void setScale(float scale)
	{
		m00 = scale;
		m11 = scale;
		m22 = scale;
	}

	Matrix3 toMatrix3() const 
	{
		return Matrix3(
			m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22); 
	}
	Matrix4 toMatrix4() const
	{
		return Matrix4(
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix3 getRotationMatrix() const;
	Vector3 getScale() const;
	Vector3	getTranslation() const { return Vector3(m03, m13, m23); }
	Matrix3x4 inverse() const;

	void decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const;

	Vector3 operator*(const Vector3& rhs) const;
	Vector3 operator*(const Vector4& rhs) const;
	Matrix4	operator *(const Matrix4& rhs) const;
	Matrix3x4 operator*(const Matrix3x4& rhs) const;

	Matrix3x4& operator+=(const Matrix3x4& rhs);
	Matrix3x4& operator-=(const Matrix3x4& rhs);
	Matrix3x4& operator*=(float rhs);
	
	friend Matrix3x4 operator+(Matrix3x4 lhs, const Matrix3x4& rhs) { return lhs += rhs; }
	friend Matrix3x4 operator-(Matrix3x4 lhs, const Matrix3x4& rhs) { return lhs -= rhs; }
	friend Matrix3x4 operator*(Matrix3x4 lhs, const float& rhs) { return lhs *= rhs; }

	friend bool operator==(const Matrix3x4& lhs, const Matrix3x4& rhs) { return memcmp(&lhs, &rhs, sizeof(lhs)) == 0; }
	friend bool operator!=(const Matrix3x4& lhs, const Matrix3x4& rhs) { return memcmp(&lhs, &rhs, sizeof(lhs)) != 0; }

public:
	union
	{
		float m[3][4];
		float _m[12];
		struct
		{
#ifndef COL_MAJOR
			// �д洢
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
#else
			// �д洢
			float m00, m10, m20;
			float m01, m11, m21;
			float m02, m12, m22;
			float m03, m13, m23;
#endif
		};
	};
};

CU_NS_END
