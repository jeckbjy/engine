//! Math
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

template<typename T>
class TVector2
{
public:
	static const TVector2 ZERO;
	static const TVector2 ONE;
	static const TVector2 UNIT_X;
	static const TVector2 UNIT_Y;

	static T		manhattanDistance(const TVector2& a, const TVector2& b);
	// 点乘:v1*v2 = |v1|*|v2|*cos<v1,v2>
	static T		dot(const TVector2& v1, const TVector2& v2);
	// 可用于计算点在直线左右
	static T		cross(const TVector2& v1, const TVector2& v2);
	// 计算点p在直线p1p2的左右
	static T		cross(const TVector2& p, const TVector2& p1, const TVector2& p2);
	static T		distance(const TVector2& v1, const TVector2& v2);
	static T		distanceSquared(const TVector2& v1, const TVector2& v2);

	static void		orthonormalize(TVector2& u, TVector2& v);

	static TVector2 reflect(const TVector2& v, const TVector2& n);
	static TVector2 normalize(const TVector2& val);
	static TVector2 min(const TVector2& a, const TVector2& b);
	static TVector2 max(const TVector2& a, const TVector2& b);

public:
	TVector2(){}
	TVector2(T x, T y):x(x),y(y){}
	TVector2(const TVector2& other) :x(other.x), y(other.y){}

	void set(T x, T y);
	void swap(TVector2& other);

	T length() const;
	T squared() const;

	T dot(const TVector2& vec) const;
	T cross(const TVector2& other) const;
	T normalize();

	void floor(const TVector2& cmp);
	void ceil(const TVector2& cmp);
	bool isZeroLength() const;
	bool isNaN() const;

	TVector2 reflect(const TVector2& normal) const;
	TVector2 perpendicular() const;

	T*		 data();
	const T* data() const;

	T& operator[](size_t i);
	T  operator[](size_t i) const;

	bool operator==(const TVector2& rhs) const;
	bool operator!=(const TVector2& rhs) const;

	const TVector2& operator+() const;
	TVector2 operator-() const;

	TVector2& operator=(const TVector2& rhs);
	TVector2& operator=(T rhs);

	TVector2& operator+=(const TVector2& v);
	TVector2& operator-=(const TVector2& v);
	TVector2& operator*=(const TVector2& v);
	TVector2& operator/=(const TVector2& v);
	TVector2& operator+=(const T& t);
	TVector2& operator-=(const T& t);
	TVector2& operator*=(const T& t);
	TVector2& operator/=(const T& t);

	TVector2 operator+(const TVector2& v) const;
	TVector2 operator-(const TVector2& v) const;
	TVector2 operator*(const TVector2& v) const;
	TVector2 operator/(const TVector2& v) const;
	TVector2 operator+(T t) const;
	TVector2 operator-(T t) const;
	TVector2 operator*(T t) const;
	TVector2 operator/(T t) const;

	friend TVector2 operator+ (T lhs, const TVector2& rhs);
	friend TVector2 operator- (T lhs, const TVector2& rhs);
	friend TVector2 operator* (T lhs, const TVector2& rhs);
	friend TVector2 operator/ (T lhs, const TVector2& rhs);

public:
	T x, y;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
typedef TVector2<int>	Vector2I;
typedef TVector2<float>	Vector2F;

typedef TVector2<float>	Vector2;

//////////////////////////////////////////////////////////////////////////
// static
//////////////////////////////////////////////////////////////////////////
template<typename T> const TVector2<T> TVector2<T>::ZERO(0, 0);
template<typename T> const TVector2<T> TVector2<T>::ONE(1, 1);
template<typename T> const TVector2<T> TVector2<T>::UNIT_X(1, 0);
template<typename T> const TVector2<T> TVector2<T>::UNIT_Y(0, 1);

template<typename T>
inline T TVector2<T>::dot(const TVector2& v1, const TVector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

template<typename T>
inline T TVector2<T>::cross(const TVector2& v1, const TVector2& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

template<typename T>
inline T TVector2<T>::cross(const TVector2& p, const TVector2& p1, const TVector2& p2)
{
	TVector2<T> v1 = p2 - p1;
	TVector2<T> v2 = p - p1;
	return cross(v1, v2);
}

template<typename T>
inline T TVector2<T>::distance(const TVector2& v1, const TVector2& v2)
{
	return (v1 - v2).length();
}

template<typename T>
inline T TVector2<T>::distanceSquared(const TVector2& v1, const TVector2& v2)
{
	return (v1 - v2).squared();
}

template<typename T>
inline TVector2<T> TVector2<T>::reflect(const TVector2& v, const TVector2& n)
{
	return v - 2 * dot(v, n) * n;
}

template<typename T>
inline T TVector2<T>::manhattanDistance(const TVector2& a, const TVector2& b)
{
	return Math::abs(a.x - b.x) + Math::abs(a.y - b.OS_MAC_OS_Xy);
}

template<typename T>
inline void TVector2<T>::orthonormalize(TVector2& u, TVector2& v)
{
	u.normalize();
	T dot_value = u.dot(v);
	v -= u * dot_value;
	v.normalize();
}

template<typename T>
inline TVector2<T> TVector2<T>::normalize(const TVector2& val)
{
	float len = std::sqrt(val.x * val.x + val.y * val.y);

	// Will also work for zero-sized vectors, but will change nothing
	TVector2 normalizedVec = val;
	if (len > 1e-08)
	{
		float invLen = 1.0f / len;
		normalizedVec.x = (T)(normalizedVec.x * invLen);
		normalizedVec.y = (T)(normalizedVec.x * invLen);
	}

	return normalizedVec;
}

template<typename T>
inline TVector2<T> TVector2<T>::min(const TVector2& a, const TVector2& b)
{
	return TVector2(std::min(a.x, b.x), std::min(a.y, b.y));
}

template<typename T>
inline TVector2<T> TVector2<T>::max(const TVector2& a, const TVector2& b)
{
	return TVector2(std::max(a.x, b.x), std::max(a.y, b.y));
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
template<typename T>
inline void TVector2<T>::set(T x, T y)
{
	this->x = x;
	this->y = y;
}

template<typename T>
inline void TVector2<T>::swap(TVector2& other)
{
	std::swap(x, other.x);
	std::swap(y, other.y);
}

template<typename T>
inline T TVector2<T>::length() const
{
	return std::sqrt(x * x + y * y);
}

template<typename T>
inline T TVector2<T>::squared() const
{
	return x * x + y * y;
}

template<typename T>
inline T TVector2<T>::dot(const TVector2& vec) const
{
	return x * vec.x + y * vec.y;
}

template<typename T>
inline T TVector2<T>::cross(const TVector2& other) const
{
	return x * other.y - y * other.x;
}

template<typename T>
inline T TVector2<T>::normalize()
{
	float len = std::sqrt(x * x + y * y);

	// Will also work for zero-sized vectors, but will change nothing
	if (len > 1e-08)
	{
		float invLen = 1.0f / len;
		x = (T)(x * invLen);
		y = (T)(y * invLen);
	}

	return len;
}

template<typename T>
inline void TVector2<T>::floor(const TVector2& cmp)
{
	if (cmp.x < x) x = cmp.x;
	if (cmp.y < y) y = cmp.y;
}

template<typename T>
inline void TVector2<T>::ceil(const TVector2& cmp)
{
	if (cmp.x > x) x = cmp.x;
	if (cmp.y > y) y = cmp.y;
}

template<typename T>
inline bool TVector2<T>::isZeroLength() const
{
	double sqlen = (x * x) + (y * y);
	return (sqlen < (1e-06 * 1e-06));
}

template<typename T>
inline bool TVector2<T>::isNaN() const
{
	return Math::isNaN(x) || Math::isNaN(y);
}

template<typename T>
inline TVector2<T> TVector2<T>::reflect(const TVector2& normal) const
{
	return TVector2(*this - (2 * this->dot(normal) * normal));
}

template<typename T>
inline TVector2<T> TVector2<T>::perpendicular() const
{
	return TVector2(-y, x);
}

template<typename T>
inline T* TVector2<T>::data()
{
	return &x;
}

template<typename T>
inline const T* TVector2<T>::data() const
{
	return &x;
}

template<typename T>
inline T& TVector2<T>::operator[](size_t i)
{
	assert(i < 2);
	return *(&x + i);
}

template<typename T>
inline T TVector2<T>::operator[](size_t i) const
{
	assert(i < 2);
	return *(&x + i);
}

template<typename T>
inline bool TVector2<T>::operator==(const TVector2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

template<typename T>
inline bool TVector2<T>::operator!=(const TVector2& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

template<typename T>
inline const TVector2<T>& TVector2<T>::operator+() const
{
	return *this;
}

template<typename T>
inline TVector2<T> TVector2<T>::operator-() const
{
	return TVector2(-x, -y);
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator=(const TVector2& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator=(T rhs)
{
	this->x = rhs;
	this->y = rhs;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator+=(const TVector2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator-=(const TVector2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator*=(const TVector2& v)
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator/=(const TVector2& v)
{
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator+=(const T& t)
{
	this->x += t;
	this->y += t;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator-=(const T& t)
{
	this->x -= t;
	this->y -= t;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator*=(const T& t)
{
	this->x *= t;
	this->y *= t;
	return *this;
}

template<typename T>
inline TVector2<T>& TVector2<T>::operator/=(const T& t)
{
	this->x /= t;
	this->y /= t;
	return *this;
}

template<typename T>
inline TVector2<T> TVector2<T>::operator+(const TVector2& v) const
{
	return TVector2(x + v.x, y + v.y);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator-(const TVector2& v) const
{
	return TVector2(x - v.x, y - v.y);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator*(const TVector2& v) const
{
	return TVector2(x * v.x, y * v.y);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator/(const TVector2& v) const
{
	return TVector2(x / v.x, y / v.y);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator+(T t) const
{
	return TVector2(x + t, y + t);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator-(T t) const
{
	return TVector2(x - t, y - t);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator*(T t) const
{
	return TVector2(x * t, y * t);
}

template<typename T>
inline TVector2<T> TVector2<T>::operator/(T t) const
{
	return TVector2(x / t, y / t);
}

template<typename T>
inline TVector2<T> operator+ (T lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs + rhs.x, lhs + rhs.y);
}

template<typename T>
inline TVector2<T> operator- (T lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs - rhs.x, lhs - rhs.y);
}
template<typename T>
inline TVector2<T> operator* (T lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs * rhs.x, lhs * rhs.y);
}

template<typename T>
inline TVector2<T> operator/ (T lhs, const TVector2<T>& rhs)
{
	return TVector2<T>(lhs / rhs.x, lhs / rhs.y);
}

CUTE_NS_END
