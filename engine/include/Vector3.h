#pragma once
#include "API.h"

CU_NS_BEGIN

template<typename T>
class TVector3
{
public:
	typedef typename T type_t;

	static const TVector3 Zero;
	static const TVector3 One;
	static const TVector3 UnitX;
	static const TVector3 UnitY;
	static const TVector3 UnitZ;

public:
	TVector3() : x(0), y(0), z(0){}
	TVector3(T v) : x(v), y(v), z(v){}
	TVector3(T x, T y, T z) : x(x), y(y), z(z){}

	inline void set(T x, T y, T z){ this->x = x; this->y = y; this->z = z; }

	inline T length() const { return sqrt((float)squared()); }
	inline T squared() const { return x*x + y*y + z*z; }
	inline void normalize() { *this /= length(); }

	inline size_t size() const{ return sizeof(*this); }
	
	inline T* data() { return m; }
	inline const T* data() const { return m; }

	inline T operator[](size_t i) const { return m[i]; }
	inline T& operator[](size_t i) { return m[i]; }
	inline TVector3 operator-() const { return Vector_T(-x, -y, -z); }

	TVector3& operator+=(const TVector3& v){ x += v.x; y += v.y; z += v.z; return *this; }
	TVector3& operator-=(const TVector3& v){ x -= v.x; y -= v.y; z -= v.z; return *this; }
	TVector3& operator*=(const TVector3& v){ x *= v.x; y *= v.y; z *= v.z; return *this; }
	TVector3& operator/=(const TVector3& v){ x /= v.x; y /= v.y; z /= v.z; return *this; }
	TVector3& operator+=(const T& t) { x += t; y += t; z += t; return *this; }
	TVector3& operator-=(const T& t) { x -= t; y -= t; z -= t; return *this; }
	TVector3& operator*=(const T& t) { x *= t; y *= t; z *= t; return *this; }
	TVector3& operator/=(const T& t) { x /= t; y /= t; z /= t; return *this; }

	friend TVector3 operator+(TVector3 lhs, const TVector3& rhs) { return lhs += rhs; }
	friend TVector3 operator-(TVector3 lhs, const TVector3& rhs) { return lhs -= rhs; }
	friend TVector3 operator*(TVector3 lhs, const TVector3& rhs) { return lhs *= rhs; }
	friend TVector3 operator/(TVector3 lhs, const TVector3& rhs) { return lhs /= rhs; }
	friend TVector3 operator+(TVector3 lhs, const T& rhs) { return lhs += rhs; }
	friend TVector3 operator-(TVector3 lhs, const T& rhs) { return lhs -= rhs; }
	friend TVector3 operator*(TVector3 lhs, const T& rhs) { return lhs *= rhs; }
	friend TVector3 operator/(TVector3 lhs, const T& rhs) { return lhs /= rhs; }

	friend bool operator==(const TVector3& lhs, const TVector3& rhs) { return memcmp(lhs.m, rhs.m, sizeof(lhs.m)) == 0; }
	friend bool operator!=(const TVector3& lhs, const TVector3& rhs) { return memcmp(lhs.m, rhs.m, sizeof(lhs.m)) != 0; }

public:
	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		T m[3];
	};
};

template<typename T> const TVector3<T> TVector3<T>::Zero(0, 0, 0);
template<typename T> const TVector3<T> TVector3<T>::One(1, 1, 1);
template<typename T> const TVector3<T> TVector3<T>::UnitX(1, 0, 0);
template<typename T> const TVector3<T> TVector3<T>::UnitY(0, 1, 0);
template<typename T> const TVector3<T> TVector3<T>::UnitZ(0, 0, 1);

template<typename T>
inline TVector3<T> normalize(const TVector3<T>& v)
{
	int len = v.length();
	return len > 0 ? (v / len) : v;
}

template<typename T>
inline T length(const TVector3<T>& v)
{
	return v.length();
}

template<typename T>
inline T squared(const TVector3<T>& v)
{
	return v.squared();
}


template<typename T>
inline T distance(const TVector3<T>& p0, const TVector3<T>& p1)
{
	return length(p1 - p0);
}

template<typename T>
T dot(const TVector3<T>& v1, const TVector3<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T>
TVector3<T> cross(const TVector3<T>& v1, const TVector3<T>& v2)
{
	return TVector3<T>(
		v1.y * v2.z - v2.y * v1.z,
		v1.z * v2.x - v2.z * v1.x,
		v1.x * v2.y - v2.x * v1.y);
}

typedef TVector3<int>	Vector3i;
typedef TVector3<float>	Vector3f;
typedef TVector3<float>	Vector3;

CU_NS_END