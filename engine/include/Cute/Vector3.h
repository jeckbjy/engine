//! Math
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector2.h"

CUTE_NS_BEGIN

template<typename T>
class TVector3
{
public:
	static const TVector3 ZERO;
	static const TVector3 ONE;
	static const TVector3 INF;
    
    static const TVector3 LEFT;
    static const TVector3 RIGHT;
    static const TVector3 UP;
    static const TVector3 DOWN;
    static const TVector3 FORWARD;
    static const TVector3 BACK;
    
	static const TVector3 UNIT_X;
	static const TVector3 UNIT_Y;
	static const TVector3 UNIT_Z;

	static float	angleBetween(const TVector3& v1, const TVector3& v2);
	static T		dot(const TVector3& a, const TVector3& b);
	static T		distance(const TVector3& v1, const TVector3& v2);
	static T		distanceSquared(const TVector3& v1, const TVector3& v2);

	static TVector3 normalize(const TVector3& val);
	static TVector3 cross(const TVector3& a, const TVector3& b);
	static TVector3 reflect(const TVector3& vec, const TVector3& normal);
	static TVector3 lerp(float t, const TVector3& a, const TVector3& b);
	static TVector3 min(const TVector3& a, const TVector3& b);
	static TVector3 max(const TVector3& a, const TVector3& b);

	static void		orthonormalize(TVector3& v0, TVector3& v1, TVector3& v2);

public:
	TVector3(){}
	TVector3(T v) : x(v), y(v), z(v){}
	TVector3(T x, T y, T z) :x(x), y(y), z(z){}
	TVector3(const TVector2<T>& vec2, float z = 0) :x(vec2.x), y(vec2.y), z(z){}
	TVector3(const TVector3<T>& other) :x(other.x), y(other.y), z(other.z){}

	void set(T x, T y, T z);

	void swap(TVector3& other);
	void floor(const TVector3& cmp);
	void ceil(const TVector3& cmp);
	bool isZeroLength() const;
	bool isNaN() const;
	void orthogonalComplement(TVector3& a, TVector3& b);

	T length() const;
	T squared() const;
	T distance(const TVector3& vec) const;
	T dot(const TVector3& vec) const;
	T normalize();

	TVector3 cross(const TVector3& other) const;
	TVector3 perpendicular() const;

	T*		 data();
	const T* data() const;

	T& operator[](size_t i);
	T  operator[](size_t i) const;

	bool operator==(const TVector3& rhs) const;
	bool operator!=(const TVector3& rhs) const;

	const TVector3& operator+() const;
	TVector3 operator-() const;

	TVector3& operator=(const TVector3& rhs);
	TVector3& operator=(T rhs);

	TVector3& operator+=(const TVector3& v);
	TVector3& operator-=(const TVector3& v);
	TVector3& operator*=(const TVector3& v);
	TVector3& operator/=(const TVector3& v);
	TVector3& operator+=(const T& t);
	TVector3& operator-=(const T& t);
	TVector3& operator*=(const T& t);
	TVector3& operator/=(const T& t);

	TVector3 operator+(const TVector3& v) const;
	TVector3 operator-(const TVector3& v) const;
	TVector3 operator*(const TVector3& v) const;
	TVector3 operator/(const TVector3& v) const;
	TVector3 operator+(T t) const;
	TVector3 operator-(T t) const;
	TVector3 operator*(T t) const;
	TVector3 operator/(T t) const;

	friend TVector3 operator+ (T lhs, const TVector3& rhs)
	{
		return TVector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
	}
	friend TVector3 operator- (T lhs, const TVector3& rhs)
	{
		return TVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
	}
	friend TVector3 operator* (T lhs, const TVector3& rhs)
	{
		return TVector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}
	friend TVector3 operator/ (T lhs, const TVector3& rhs)
	{
		return TVector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
	}

public:
	T x, y, z;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
typedef TVector3<int>	Vector3I;
typedef TVector3<float>	Vector3F;
typedef TVector3<float>	Vector3;

//////////////////////////////////////////////////////////////////////////
// static
//////////////////////////////////////////////////////////////////////////
template<typename T> const TVector3<T> TVector3<T>::ZERO(0);
template<typename T> const TVector3<T> TVector3<T>::ONE(1);
template<typename T> const TVector3<T> TVector3<T>::INF(std::numeric_limits<T>::infinity());

template<typename T> const TVector3<T> TVector3<T>::LEFT(-1, 0, 0);
template<typename T> const TVector3<T> TVector3<T>::RIGHT(1, 0, 0);

template<typename T> const TVector3<T> TVector3<T>::UP(0, 1, 0);
template<typename T> const TVector3<T> TVector3<T>::DOWN(0, -1, 0);
template<typename T> const TVector3<T> TVector3<T>::FORWARD(0, 0, 1);
template<typename T> const TVector3<T> TVector3<T>::BACK(0, 0, -1);

template<typename T> const TVector3<T> TVector3<T>::UNIT_X(1, 0, 0);
template<typename T> const TVector3<T> TVector3<T>::UNIT_Y(0, 1, 0);
template<typename T> const TVector3<T> TVector3<T>::UNIT_Z(0, 0, 1);

template<typename T>
float TVector3<T>::angleBetween(const TVector3& v1, const TVector3& v2)
{
	float lenProduct = v1.length() * v2.length();

	// Divide by zero check
	if (lenProduct < 1e-6f)
		lenProduct = 1e-6f;

	float f = dot(v1, v2) / lenProduct;

	f = Math::clamp(f, -1.0f, 1.0f);
	return Math::acos(f);
}

template<typename T>
inline T TVector3<T>::dot(const TVector3& a, const TVector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
inline T TVector3<T>::distance(const TVector3& v1, const TVector3& v2)
{
	return (v1 - v2).length();
}

template<typename T>
inline T TVector3<T>::distanceSquared(const TVector3& v1, const TVector3& v2)
{
	return (v1 - v2).squared();
}

template<typename T>
inline void TVector3<T>::orthonormalize(TVector3& vec0, TVector3& vec1, TVector3& vec2)
{
	float dot0, dot1;
	vec0.normalize();

	dot0 = dot(vec0, vec1);
	vec1 -= dot0*vec0;
	vec1.normalize();

	dot1 = dot(vec1, vec2);
	dot0 = dot(vec0, vec2);
	vec2 -= dot0*vec0 + dot1*vec1;
	vec2.normalize();
}

template<typename T>
inline TVector3<T> TVector3<T>::normalize(const TVector3& val)
{
	float len = std::sqrt(val.x * val.x + val.y * val.y + val.z * val.z);

	// Will also work for zero-sized vectors, but will change nothing
	if (len > 1e-08)
	{
		float invLen = 1.0f / len;

		Vector3 normalizedVec;
		normalizedVec.x = val.x * invLen;
		normalizedVec.y = val.y * invLen;
		normalizedVec.z = val.z * invLen;

		return normalizedVec;
	}
	else
		return val;
}

template<typename T>
inline TVector3<T> TVector3<T>::cross(const TVector3& a, const TVector3& b)
{
	return TVector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

template<typename T>
inline TVector3<T> TVector3<T>::reflect(const TVector3& vec, const TVector3& normal)
{
	return Vector3(vec - (2 * dot(vec, normal) * normal));
}

template<typename T>
inline TVector3<T> TVector3<T>::lerp(float t, const TVector3& a, const TVector3& b)
{
	return (1.0f - t) * a + t * b;
}

template<typename T>
inline TVector3<T> TVector3<T>::min(const TVector3& a, const TVector3& b)
{
	return TVector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

template<typename T>
inline TVector3<T> TVector3<T>::max(const TVector3& a, const TVector3& b)
{
	return TVector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

template<typename T>
inline void TVector3<T>::set(T x, T y, T z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template<typename T>
inline void TVector3<T>::swap(TVector3& other)
{
	std::swap(x, other.x);
	std::swap(y, other.y);
	std::swap(z, other.z);
}

template<typename T>
inline T TVector3<T>::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

template<typename T>
inline T TVector3<T>::squared() const
{
	return x * x + y * y + z * z;
}

template<typename T>
inline T TVector3<T>::distance(const TVector3& vec) const
{
	return (*this - vec).length();
}

template<typename T>
inline T TVector3<T>::dot(const TVector3& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

template<typename T>
inline TVector3<T> TVector3<T>::cross(const TVector3& other) const
{
	return TVector3(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x);
}

template<typename T>
inline TVector3<T> TVector3<T>::perpendicular() const
{
	static const float squareZero = (float)(1e-06 * 1e-06);

	TVector3 perp = this->cross(TVector3::UNIT_X);

	if (perp.squared() < squareZero)
		perp = this->cross(TVector3::UNIT_Y);

	perp.normalize();
	return perp;
}

template<typename T>
inline T TVector3<T>::normalize()
{
	float len = length();

	// Will also work for zero-sized vectors, but will change nothing
	if (len > 1e-08)
	{
		float invLen = 1.0f / len;
		x = (T)(x * invLen);
		y = (T)(y * invLen);
		z = (T)(z * invLen);
	}

	return len;
}

template<typename T>
inline void TVector3<T>::floor(const TVector3& cmp)
{
	if (cmp.x < x) x = cmp.x;
	if (cmp.y < y) y = cmp.y;
	if (cmp.z < z) z = cmp.z;
}

template<typename T>
inline void TVector3<T>::ceil(const TVector3& cmp)
{
	if (cmp.x > x) x = cmp.x;
	if (cmp.y > y) y = cmp.y;
	if (cmp.z > z) z = cmp.z;
}

template<typename T>
inline bool TVector3<T>::isZeroLength() const
{
	double sqlen = (x * x) + (y * y) + (z * z);
	return (sqlen < (1e-06 * 1e-06));
}

template<typename T>
inline bool TVector3<T>::isNaN() const
{
	return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z);
}

template<typename T>
inline void TVector3<T>::orthogonalComplement(TVector3& a, TVector3& b)
{
	if (fabs(x) > fabs(y))
		a = TVector3(-z, 0, x);
	else
		a = TVector3(0, z, -y);

	b = cross(a);

	orthonormalize(*this, a, b);
}

template<typename T>
inline T* TVector3<T>::data()
{
	return &x;
}

template<typename T>
inline const T* TVector3<T>::data() const
{
	return &x;
}

template<typename T>
inline T& TVector3<T>::operator[](size_t i)
{
	assert(i < 2);
	return *(&x + i);
}

template<typename T>
inline T TVector3<T>::operator[](size_t i) const
{
	assert(i < 2);
	return *(&x + i);
}

template<typename T>
inline bool TVector3<T>::operator==(const TVector3& rhs) const
{
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

template<typename T>
inline bool TVector3<T>::operator!=(const TVector3& rhs) const
{
	return (x != rhs.x || y != rhs.y || z != rhs.z);
}

template<typename T>
inline const TVector3<T>& TVector3<T>::operator+() const
{
	return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::operator-() const
{
	return TVector3(-x, -y, -z);
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator=(const TVector3& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator=(T rhs)
{
	this->x = rhs;
	this->y = rhs;
	this->z = rhs;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator+=(const TVector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator-=(const TVector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator*=(const TVector3& v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator/=(const TVector3& v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator+=(const T& t)
{
	this->x += t;
	this->y += t;
	this->z += t;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator-=(const T& t)
{
	this->x -= t;
	this->y -= t;
	this->z -= t;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator*=(const T& t)
{
	this->x *= t;
	this->y *= t;
	this->z *= t;
	return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::operator/=(const T& t)
{
	this->x /= t;
	this->y /= t;
	this->z /= t;
	return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::operator+(const TVector3& v) const
{
	return TVector3(x + v.x, y + v.y, z + v.z);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator-(const TVector3& v) const
{
	return TVector3(x - v.x, y - v.y, z - v.z);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator*(const TVector3& v) const
{
	return TVector3(x * v.x, y * v.y, z * v.z);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator/(const TVector3& v) const
{
	return TVector3(x / v.x, y / v.y, z / v.z);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator+(T t) const
{
	return TVector3(x + t, y + t, z + t);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator-(T t) const
{
	return TVector3(x - t, y - t, z - t);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator*(T t) const
{
	return TVector3(x * t, y * t, z * t);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator/(T t) const
{
	return TVector3(x / t, y / t, z / t);
}

//template<typename T>
//inline TVector3<T> operator+ (T lhs, const TVector3<T>& rhs)
//{
//	return TVector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
//}
//
//template<typename T>
//inline TVector3<T> operator- (T lhs, const TVector3<T>& rhs)
//{
//	return TVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
//}
//template<typename T>
//inline TVector3<T> operator* (T lhs, const TVector3<T>& rhs)
//{
//	return TVector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
//}
//
//template<typename T>
//inline TVector3<T> operator/ (T lhs, const TVector3<T>& rhs)
//{
//	return TVector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
//}

CUTE_NS_END
