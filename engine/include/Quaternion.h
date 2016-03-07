#pragma once
#include "CMath.h"
#include "Vector3.h"

CU_NS_BEGIN

// 四元数 q=w+xi+yj+zk() 或者q = [w,v]
// 资料 http://blog.csdn.net/hany3000/article/details/9248081#
// http://www.cnblogs.com/dragon2012/p/3924929.html
// http://zh.wikipedia.org/wiki/%E5%9B%9B%E5%85%83%E6%95%B8
class CU_API Quaternion
{
public:
	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static void lerp(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, float t);
	static void slerp(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, float t);
	static void squad(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t);
	static Quaternion getRotationFromTo(const Vector3& from, const Vector3& dest, const Vector3& fallbackAxis = Vector3::ZERO);

public:
	Quaternion();
	Quaternion(float w, float x, float y, float z);
	Quaternion(const Matrix3& rot)
	{
		fromRotationMatrix(rot);
	}
	Quaternion(const Vector3& axis, float angle)
	{
		fromAxisAngle(axis, angle);
	}
	Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
	{
		fromAxes(xaxis, yaxis, zaxis);
	}
	Quaternion(float xAngle, float yAngle, float zAngle)
	{
		fromEulerAngles(xAngle, yAngle, zAngle);
	}
	Quaternion(float xAngle, float yAngle, float zAngle, EulerAngleOrder order)
	{
		fromEulerAngles(xAngle, yAngle, zAngle, order);
	}

	void swap(Quaternion& other);

	Quaternion inverse() const;		// 逆
	Quaternion conjugate() const;	// 共轭
	
	float normalize();
	float dot(const Quaternion& other) const;
	void  multiply(const Quaternion& q);

	Vector3 rotate(const Vector3& vec) const;
	void lookRotation(const Vector3& forward);
	void lookRotation(const Vector3& forward, const Vector3& up);

	Vector3 xAxis() const;
	Vector3 yAxis() const;
	Vector3 zAxis() const;

	void fromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
	void fromAxisAngle(const Vector3& axis, float angle);
	void fromEulerAngles(float xAngle, float yAngle, float zAngle);
	void fromEulerAngles(float xAngle, float yAngle, float zAngle, EulerAngleOrder order);
	void fromRotationMatrix(const Matrix3& mat);

	void toAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;
	void toAxisAngle(Vector3& axis, float& angle) const;
	bool toEulerAngles(float& xAngle, float& yAngle, float& zAngle) const;
	void toRotationMatrix(Matrix3& mat) const;

	String toString() const;

public:
	void set(float w, float x, float y, float z){ this->w = w; this->x = x; this->y = y; this->z = z; }

	float* data() { return &w; }
	float operator[](const size_t i) const { return *(&w + i); }
	float& operator[](const size_t i) { return *(&w + i); }

	bool operator==(const Quaternion& rhs) const { return memcmp(&x, &rhs.x, sizeof(Quaternion)) == 0; }
	bool operator!=(const Quaternion& rhs) const { return memcmp(&x, &rhs.x, sizeof(Quaternion)) != 0; }

	Quaternion operator-() const { return Quaternion(-x, -y, -z, -w); }

	Quaternion& operator+=(const Quaternion& rhs);
	Quaternion& operator-=(const Quaternion& rhs);
	Quaternion& operator*=(const Quaternion& rhs);
	Quaternion& operator*=(const float& rhs);

	friend Quaternion operator+(Quaternion lhs, const Quaternion& rhs) { return lhs += rhs; }
	friend Quaternion operator-(Quaternion lhs, const Quaternion& rhs) { return lhs -= rhs; }
	friend Quaternion operator*(Quaternion lhs, const Quaternion& rhs) { return lhs *= rhs; }
	friend Quaternion operator*(Quaternion lhs, const float& rhs) { return lhs *= rhs; }

public:
	float w, x, y, z;
};

CU_NS_END