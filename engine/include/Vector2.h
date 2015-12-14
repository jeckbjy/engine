#pragma once
#include "API.h"

CU_NS_BEGIN

template<typename T>
class TVector2
{
public:
	typedef typename T type_t;
	static const TVector2 Zero;
	static const TVector2 One;
	static const TVector2 UnitX;
	static const TVector2 UnitY;

public:
	TVector2():x(0),y(0){}
	TVector2(T v) :x(v), y(v){}
	TVector2(T x, T y) :x(x), y(y){}

	inline void set(T x, T y){ this->x = x; this->y = y; }

	inline T length() const { return sqrt(x*x + y*y); }
	inline T squared() const { return x*x + y*y; }
	inline void normalize() { *this /= length(); }
	inline TVector2 normal() const { return TVector2(-y, x); }

	inline size_t size() const { return sizeof(*this); }
	inline T* data() { return m; }
	inline const T* data() const { return m; }

	inline TVector2 operator-() const { return TVector2(-x, -y); }

	inline T  operator[](size_t i) const { return m[i]; }
	inline T& operator[](size_t i) { return m[i]; }

	TVector2& operator+=(const TVector2& v){ x += v.x; y += v.y; return *this; }
	TVector2& operator-=(const TVector2& v){ x -= v.x; y -= v.y; return *this; }
	TVector2& operator*=(const TVector2& v){ x *= v.x; y *= v.y; return *this; }
	TVector2& operator/=(const TVector2& v){ x /= v.x; y /= v.y; return *this; }
	TVector2& operator+=(const T& t) { x += t; y += t; return *this; }
	TVector2& operator-=(const T& t) { x -= t; y -= t; return *this; }
	TVector2& operator*=(const T& t) { x *= t; y *= t; return *this; }
	TVector2& operator/=(const T& t) { x /= t; y /= t; return *this; }

	friend TVector2 operator+(TVector2 lhs, const TVector2& rhs) { return lhs += rhs; }
	friend TVector2 operator-(TVector2 lhs, const TVector2& rhs) { return lhs -= rhs; }
	friend TVector2 operator*(TVector2 lhs, const TVector2& rhs) { return lhs *= rhs; }
	friend TVector2 operator/(TVector2 lhs, const TVector2& rhs) { return lhs /= rhs; }
	friend TVector2 operator+(TVector2 lhs, const T& rhs) { return lhs += rhs; }
	friend TVector2 operator-(TVector2 lhs, const T& rhs) { return lhs -= rhs; }
	friend TVector2 operator*(TVector2 lhs, const T& rhs) { return lhs *= rhs; }
	friend TVector2 operator/(TVector2 lhs, const T& rhs) { return lhs /= rhs; }

	friend bool operator==(const TVector2& lhs, const TVector2& rhs) { return memcmp(lhs.m, rhs.m, sizeof(lhs.m)) == 0; }
	friend bool operator!=(const TVector2& lhs, const TVector2& rhs) { return memcmp(lhs.m, rhs.m, sizeof(lhs.m)) != 0; }

public:
	union
	{
		struct { T x, y; };
		T m[2];
	};
};

template<typename T> const TVector2<T> TVector2<T>::Zero(0, 0);
template<typename T> const TVector2<T> TVector2<T>::One(1, 1);
template<typename T> const TVector2<T> TVector2<T>::UnitX(1, 0);
template<typename T> const TVector2<T> TVector2<T>::UnitY(0, 1);

template<typename T>
inline TVector2<T> normalize(const TVector2<T>& v)
{
	int len = v.length();
	return len > 0 ? (v / len) : v;
}

template<typename T>
inline T length(const TVector2<T>& v)
{
	return v.length();
}

template<typename T>
inline T squared(const TVector2<T>& v)
{
	return v.squared();
}


template<typename T>
inline T distance(const TVector2<T>& p0, const TVector2<T>& p1)
{
	return length(p1 - p0);
}

// 点乘:v1*v2 = |v1|*|v2|*cos<v1,v2>
template<typename T>
inline T dot(const TVector2<T>& v1, const TVector2<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

// 可用于计算点在直线左右
template<typename T>
inline T cross(const TVector2<T>& v1, const TVector2<T>& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

// 计算点p在直线p1p2的左右
template<typename T>
inline T cross(const TVector2<T>& p, const TVector2<T>& p1, const TVector2<T>& p2)
{
	TVector2<T> v1 = p2 - p1;
	TVector2<T> v2 = p - p1;
	return cross(v1, v2);
}

template<typename T>
TVector2<T> reflect(const TVector2<T>& v, const TVector2<T>& n)
{
	return v - 2 * dot(v, n) * n;
}

typedef TVector2<int>	Vector2i;
typedef TVector2<float>	Vector2f;
typedef TVector2<float>	Vector2;

CU_NS_END