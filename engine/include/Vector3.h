#pragma once
#include "API.h"

CU_NS_BEGIN

template<typename T>
class TVector3
{
public:
	typedef typename T type_t;

	static const TVector3 ZERO;
	static const TVector3 ONE;
	static const TVector3 UNIT_X;
	static const TVector3 UNIT_Y;
	static const TVector3 UNIT_Z;

	static T length(const TVector3& v)
	{
		return v.length(); 
	}

	static T squared(const TVector3& v) 
	{
		return v.squared(); 
	}

	static T distance(const TVector3& p0, const TVector3& p1)
	{
		return length(p1 - p0); 
	}

	static T dot(const TVector3<T>& v1, const TVector3<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static TVector3 normalize(const TVector3& v)
	{
		T len = v.length();
		return len > 0 ? (v / len) : v;
	}

	static TVector3 cross(const TVector3& v1, const TVector3& v2)
	{
		return TVector3(
			v1.y * v2.z - v2.y * v1.z,
			v1.z * v2.x - v2.z * v1.x,
			v1.x * v2.y - v2.x * v1.y);
	}

public:
	TVector3() : x(0), y(0), z(0){}
	TVector3(T v) : x(v), y(v), z(v){}
	TVector3(T x, T y, T z) : x(x), y(y), z(z){}

	inline void set(T x, T y, T z){ this->x = x; this->y = y; this->z = z; }

	inline T length() const { return sqrt((float)squared()); }
	inline T squared() const { return x*x + y*y + z*z; }
	inline void normalize() { *this /= length(); }

	inline bool isZeroLength() const { return squared() < (1e-06 * 1e-06); }

	inline size_t size() const{ return sizeof(*this); }
	
	inline T* data() { return m; }
	inline const T* data() const { return m; }

	inline T operator[](size_t i) const { return m[i]; }
	inline T& operator[](size_t i) { return m[i]; }
	inline TVector3 operator-() const { return TVector3(-x, -y, -z); }

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

	friend TVector3 operator+(const T& lhs, const TVector3& rhs) { return rhs += lhs; }
	friend TVector3 operator*(const T& lhs, const TVector3& rhs) { return rhs *= lhs; }
	friend TVector3 operator-(const T& lhs, const TVector3& rhs) { return TVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }
	friend TVector3 operator/(const T& lhs, const TVector3& rhs)
	{
		T rx = rhs.x != 0 ? lhs / rhs.x : rhs.x;
		T ry = rhs.x != 0 ? lhs / rhs.y : rhs.y;
		T rz = rhs.x != 0 ? lhs / rhs.z : rhs.z;
		return TVector3(rx, ry, rz);
	}

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

template<typename T> const TVector3<T> TVector3<T>::ZERO(0, 0, 0);
template<typename T> const TVector3<T> TVector3<T>::ONE(1, 1, 1);
template<typename T> const TVector3<T> TVector3<T>::UNIT_X(1, 0, 0);
template<typename T> const TVector3<T> TVector3<T>::UNIT_Y(0, 1, 0);
template<typename T> const TVector3<T> TVector3<T>::UNIT_Z(0, 0, 1);

typedef TVector3<int>	Vector3i;
typedef TVector3<float>	Vector3f;
typedef TVector3<float>	Vector3;

CU_NS_END