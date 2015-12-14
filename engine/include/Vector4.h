#pragma once
#include "API.h"

CU_NS_BEGIN

template<typename T>
class TVector4
{
public:
	typedef typename T type_t;
	static const TVector4 Zero;
	static const TVector4 One;
	static const TVector4 UnitX;
	static const TVector4 UnitY;
	static const TVector4 UnitZ;
	static const TVector4 UnitW;

	TVector4() :x(0), y(0), z(0), w(0){}
	TVector4(T v) :x(v), y(v), z(v), w(v){}
	TVector4(T x, T y, T z, T w) :x(x), y(y), z(z), w(w){}

	inline void set(T x, T y, T z, T w){ this->x = x; this->y = y; this->z = z; this->w = w; }

	inline T length() const { return sqrt((float)squared()); }
	inline T squared() const { return x*x + y*y + z*z + w*w; }

	inline size_t size() const { return sizeof(*this); }
	inline T* data() { return m; }
	inline const T* data() const { return m; }
	inline T operator[](size_t i) const { return m[i]; }
	inline T& operator[](size_t i) { return m[i]; }

	inline const TVector4& operator+() const { return *this; }
	inline TVector4 operator-() const { return Vector_T(-x, -y, -z, -w); }

	TVector4& operator+=(const TVector4& v){ x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	TVector4& operator-=(const TVector4& v){ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	TVector4& operator*=(const TVector4& v){ x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	TVector4& operator/=(const TVector4& v){ x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	TVector4& operator+=(const T& t) { x += t; y += t; z += t; w += t; return *this; }
	TVector4& operator-=(const T& t) { x -= t; y -= t; z -= t; w -= t; return *this; }
	TVector4& operator*=(const T& t) { x *= t; y *= t; z *= t; w *= t; return *this; }
	TVector4& operator/=(const T& t) { x /= t; y /= t; z /= t; w /= t; return *this; }

	friend TVector4 operator+(TVector4 lhs, const TVector4& rhs) { return lhs += rhs; }
	friend TVector4 operator-(TVector4 lhs, const TVector4& rhs) { return lhs -= rhs; }
	friend TVector4 operator*(TVector4 lhs, const TVector4& rhs) { return lhs *= rhs; }
	friend TVector4 operator/(TVector4 lhs, const TVector4& rhs) { return lhs /= rhs; }
	friend TVector4 operator+(TVector4 lhs, const T& rhs) { return lhs += rhs; }
	friend TVector4 operator-(TVector4 lhs, const T& rhs) { return lhs -= rhs; }
	friend TVector4 operator*(TVector4 lhs, const T& rhs) { return lhs *= rhs; }
	friend TVector4 operator/(TVector4 lhs, const T& rhs) { return lhs /= rhs; }

	friend bool operator==(const TVector4& lhs, const TVector4& rhs) { return memcmp(lhs.m, rhs.m, sizeof(lhs.m)) == 0; }
	friend bool operator!=(const TVector4& lhs, const TVector4& rhs) { return memcmp(lhs.m, rhs.m, sizeof(lhs.m)) != 0; }

public:
	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T m[4];
	};
};

template<typename T> const TVector4<T> TVector4<T>::Zero(0);
template<typename T> const TVector4<T> TVector4<T>::One(1);
template<typename T> const TVector4<T> TVector4<T>::UnitX(1, 0, 0, 0);
template<typename T> const TVector4<T> TVector4<T>::UnitY(0, 1, 0, 0);
template<typename T> const TVector4<T> TVector4<T>::UnitZ(0, 0, 1, 0);
template<typename T> const TVector4<T> TVector4<T>::UnitW(0, 0, 0, 1);

template<typename T>
inline TVector4<T> normalize(const TVector4<T>& v)
{
	int len = v.length();
	return len > 0 ? (v / len) : v;
}

template<typename T>
inline T length(const TVector4<T>& v)
{
	return v.length();
}

template<typename T>
inline T squared(const TVector4<T>& v)
{
	return v.squared();
}

template<typename T>
inline T distance(const TVector4<T>& p0, const TVector4<T>& p1)
{
	return length(p1 - p0);
}

template<typename T>
T dot(const TVector4<T>& v1, const TVector4<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

typedef TVector4<int>	Vector4i;
typedef TVector4<float>	Vector4f;
typedef TVector4<float>	Vector4;


CU_NS_END