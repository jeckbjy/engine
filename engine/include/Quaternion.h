#pragma once
#include "Vector3.h"

CU_NS_BEGIN

// 四元数 q=w+xi+yj+zk() 或者q = [w,v]
// 资料 http://blog.csdn.net/hany3000/article/details/9248081#
// http://www.cnblogs.com/dragon2012/p/3924929.html
// http://zh.wikipedia.org/wiki/%E5%9B%9B%E5%85%83%E6%95%B8
class CU_API Quaternion
{
public:
	static const Quaternion Zero;
	static const Quaternion Identity;
	static void lerp(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, float t);
	static void slerp(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, float t);
	static void squad(Quaternion* dst, const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t);
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(float* arrays);

	Quaternion inverse() const;		// 逆
	Quaternion conjugate() const;	// 共轭

	float normalize();
	float dot(const Quaternion& other) const;
	void  multiply(const Quaternion& q);

	void set(float x, float y, float z, float w){ this->x = x; this->y = y; this->z = z; this->w = w; }
	
	Vector3 xAxis() const;
	Vector3 yAxis() const;
	Vector3 zAxis() const;

public:
	float* data() { return &x; }
	float operator[](const size_t i) const { return *(&x + i); }
	float& operator[](const size_t i) { return *(&x + i); }

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
	float x, y, z, w;
};

CU_NS_END