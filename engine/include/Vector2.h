#pragma once
#include "API.h"

CU_NS_BEGIN

template<typename T>
class TVector2
{
public:
	typedef typename T type_t;
	static const TVector2 ZERO;
	static const TVector2 ONE;
	static const TVector2 UNIT_X;
	static const TVector2 UNIT_Y;

	static TVector2 normalize(const TVector2& v)
	{
		T len = v.length();
		return len > 0 ? (v / len) : v;
	}

	static T length(const TVector2<T>& v)
	{
		return v.length();
	}

	static T squared(const TVector2& v)
	{
		return v.squared();
	}

	static T distance(const TVector2& p0, const TVector2& p1)
	{
		return length(p1 - p0);
	}

	// 点乘:v1*v2 = |v1|*|v2|*cos<v1,v2>
	static T dot(const TVector2& v1, const TVector2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	// 可用于计算点在直线左右
	static T cross(const TVector2& v1, const TVector2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}

	// 计算点p在直线p1p2的左右
	static T cross(const TVector2& p, const TVector2& p1, const TVector2& p2)
	{
		TVector2<T> v1 = p2 - p1;
		TVector2<T> v2 = p - p1;
		return cross(v1, v2);
	}

	static TVector2 reflect(const TVector2& v, const TVector2& n)
	{
		return v - 2 * dot(v, n) * n;
	}

public:
	TVector2():x(0),y(0){}
	TVector2(T v) :x(v), y(v){}
	TVector2(T x, T y) :x(x), y(y){}

	void set(T x, T y){ this->x = x; this->y = y; }

	T length() const { return sqrt(x*x + y*y); }
	T squared() const { return x*x + y*y; }
	void normalize() { *this /= length(); }
	TVector2 normal() const { return TVector2(-y, x); }

	size_t size() const { return sizeof(*this); }
	T* data() { return m; }
	const T* data() const { return m; }

	String toString() const 
	{
		std::stringstream ss; 
		ss << x << " " << y; 
		return ss.str();
	}

	TVector2 operator-() const { return TVector2(-x, -y); }

	T  operator[](size_t i) const { return m[i]; }
	T& operator[](size_t i) { return m[i]; }

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
		struct { T width, height; };
		T m[2];
	};
};

template<typename T> const TVector2<T> TVector2<T>::ZERO(0, 0);
template<typename T> const TVector2<T> TVector2<T>::ONE(1, 1);
template<typename T> const TVector2<T> TVector2<T>::UNIT_X(1, 0);
template<typename T> const TVector2<T> TVector2<T>::UNIT_Y(0, 1);

typedef TVector2<int>	Vector2i;
typedef TVector2<float>	Vector2f;
typedef TVector2<float>	Vector2;

CU_NS_END
