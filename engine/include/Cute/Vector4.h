//! Math
#pragma once
#include "Cute/Foundation.h"
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

template<typename T>
class TVector4
{
public:
	static const TVector4 ZERO;
	static const TVector4 ONE;
	static const TVector4 UNIT_X;
	static const TVector4 UNIT_Y;
	static const TVector4 UNIT_Z;
	static const TVector4 UNIT_W;

	static T		dot(const TVector4& a, const TVector4& b);
	static T		distance(const TVector4& a, const TVector4& b);
	static T		distanceSquared(const TVector4& a, const TVector4& b);

	static TVector4 min(const TVector4& a, const TVector4& b);
	static TVector4 max(const TVector4& a, const TVector4& b);

public:
	TVector4(){}
	TVector4(T v) : x(v), y(v), z(v), w(v){}
	TVector4(T x, T y, T z, T w) :x(x), y(y), z(z), w(w){}
	TVector4(const TVector3<T>& vec3, float w = 0) :x(vec3.x), y(vec3.y), z(vec3.z), w(w){}
	TVector4(const TVector4<T>& other) :x(other.x), y(other.y), z(other.z), w(other.w){}

	void set(T x, T y, T z, T w);
	void swap(TVector4& other);
	void floor(const TVector4& cmp);
	void ceil(const TVector4& cmp);
	bool isZeroLength() const;
	bool isNaN() const;

	T normalize();
	T length() const;
	T squared() const;
	T dot(const TVector4& vec) const;

	T*		 data();
	const T* data() const;

	T& operator[](size_t i);
	T  operator[](size_t i) const;

	bool operator==(const TVector4& rhs) const;
	bool operator!=(const TVector4& rhs) const;

	const TVector4& operator+() const;
	TVector4 operator-() const;

	TVector4& operator=(const TVector4& rhs);
	TVector4& operator=(T rhs);

	TVector4& operator+=(const TVector4& v);
	TVector4& operator-=(const TVector4& v);
	TVector4& operator*=(const TVector4& v);
	TVector4& operator/=(const TVector4& v);
	TVector4& operator+=(const T& t);
	TVector4& operator-=(const T& t);
	TVector4& operator*=(const T& t);
	TVector4& operator/=(const T& t);

	TVector4 operator+(const TVector4& v) const;
	TVector4 operator-(const TVector4& v) const;
	TVector4 operator*(const TVector4& v) const;
	TVector4 operator/(const TVector4& v) const;
	TVector4 operator+(T t) const;
	TVector4 operator-(T t) const;
	TVector4 operator*(T t) const;
	TVector4 operator/(T t) const;

	friend TVector4 operator+ (T lhs, const TVector4& rhs);
	friend TVector4 operator- (T lhs, const TVector4& rhs);
	friend TVector4 operator* (T lhs, const TVector4& rhs);
	friend TVector4 operator/ (T lhs, const TVector4& rhs);

public:
	T x, y, z, w;
};

//////////////////////////////////////////////////////////////////////////
// define
//////////////////////////////////////////////////////////////////////////
typedef TVector4<int>	Vector4I;
typedef TVector4<float>	Vector4F;
typedef TVector4<float>	Vector4;

//////////////////////////////////////////////////////////////////////////
// static inline
//////////////////////////////////////////////////////////////////////////
template<typename T> const TVector4<T> TVector4<T>::ZERO(0);
template<typename T> const TVector4<T> TVector4<T>::ONE(1);
template<typename T> const TVector4<T> TVector4<T>::UNIT_X(1, 0, 0, 0);
template<typename T> const TVector4<T> TVector4<T>::UNIT_Y(0, 1, 0, 0);
template<typename T> const TVector4<T> TVector4<T>::UNIT_Z(0, 0, 1, 0);
template<typename T> const TVector4<T> TVector4<T>::UNIT_W(0, 0, 0, 1);

template<typename T>
inline T TVector4<T>::dot(const TVector4& a, const TVector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template<typename T>
inline T TVector4<T>::distance(const TVector4& a, const TVector4& b)
{
	return (a - b).length();
}

template<typename T>
inline T TVector4<T>::distanceSquared(const TVector4& a, const TVector4& b)
{
	return (a - b).squared();
}

template<typename T>
inline TVector4<T> TVector4<T>::min(const TVector4& a, const TVector4& b)
{
	return TVector4(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
}

template<typename T>
inline TVector4<T> TVector4<T>::max(const TVector4& a, const TVector4& b)
{
	return TVector4(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}

//////////////////////////////////////////////////////////////////////////
// method inline
//////////////////////////////////////////////////////////////////////////
template<typename T>
inline void TVector4<T>::set(T x, T y, T z, T w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

template<typename T>
inline void TVector4<T>::swap(TVector4& other)
{
	std::swap(x, other.x);
	std::swap(y, other.y);
	std::swap(z, other.z);
	std::swap(w, other.w);
}

template<typename T>
inline T TVector4<T>::length() const
{
	return std::sqrt(x * x + y * y + z * z + w * w);
}

template<typename T>
inline T TVector4<T>::squared() const
{
	return x * x + y * y + z * z + w * w;
}

template<typename T>
inline T TVector4<T>::dot(const TVector4& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
}

template<typename T>
inline T TVector4<T>::normalize()
{
	float len = length();

	// Will also work for zero-sized vectors, but will change nothing
	if (len > 1e-08)
	{
		float invLen = 1.0f / len;
		x = (T)(x * invLen);
		y = (T)(y * invLen);
		z = (T)(z * invLen);
		w = (T)(w * invLen);
	}

	return len;
}

template<typename T>
inline void TVector4<T>::floor(const TVector4& cmp)
{
	if (cmp.x < x) x = cmp.x;
	if (cmp.y < y) y = cmp.y;
	if (cmp.z < z) z = cmp.z;
	if (cmp.w < w) w = cmp.w;
}

template<typename T>
inline void TVector4<T>::ceil(const TVector4& cmp)
{
	if (cmp.x > x) x = cmp.x;
	if (cmp.y > y) y = cmp.y;
	if (cmp.z > z) z = cmp.z;
	if (cmp.w > w) w = cmp.w;
}

template<typename T>
inline bool TVector4<T>::isZeroLength() const
{
	double sqlen = squared();
	return (sqlen < (1e-06 * 1e-06));
}

template<typename T>
inline bool TVector4<T>::isNaN() const
{
	return Math::isNaN(x) || Math::isNaN(y) || Math::isNaN(z) || Math::isNaN(w);
}

template<typename T>
inline T* TVector4<T>::data()
{
	return &x;
}

template<typename T>
inline const T* TVector4<T>::data() const
{
	return &x;
}

template<typename T>
inline T& TVector4<T>::operator[](size_t i)
{
	assert(i < 2);
	return *(&x + i);
}

template<typename T>
inline T TVector4<T>::operator[](size_t i) const
{
	assert(i < 2);
	return *(&x + i);
}

template<typename T>
inline bool TVector4<T>::operator==(const TVector4& rhs) const
{
	return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
}

template<typename T>
inline bool TVector4<T>::operator!=(const TVector4& rhs) const
{
	return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
}

template<typename T>
inline const TVector4<T>& TVector4<T>::operator+() const
{
	return *this;
}

template<typename T>
inline TVector4<T> TVector4<T>::operator-() const
{
	return TVector4(-x, -y, -z, -w);
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator=(const TVector4& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	this->z = rhs.z;
	this->w = rhs.w;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator=(T rhs)
{
	this->x = rhs;
	this->y = rhs;
	this->z = rhs;
	this->w = rhs;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator+=(const TVector4& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator-=(const TVector4& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator*=(const TVector4& v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	this->w *= v.w;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator/=(const TVector4& v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	this->w /= v.w;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator+=(const T& t)
{
	this->x += t;
	this->y += t;
	this->z += t;
	this->w += t;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator-=(const T& t)
{
	this->x -= t;
	this->y -= t;
	this->z -= t;
	this->w -= t;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator*=(const T& t)
{
	this->x *= t;
	this->y *= t;
	this->z *= t;
	this->w *= t;
	return *this;
}

template<typename T>
inline TVector4<T>& TVector4<T>::operator/=(const T& t)
{
	this->x /= t;
	this->y /= t;
	this->z /= t;
	this->w /= t;
	return *this;
}

template<typename T>
inline TVector4<T> TVector4<T>::operator+(const TVector4& v) const
{
	return TVector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

template<typename T>
inline TVector4<T> TVector4<T>::operator-(const TVector4& v) const
{
	return TVector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

template<typename T>
inline TVector4<T> TVector4<T>::operator*(const TVector4& v) const
{
	return TVector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

template<typename T>
inline TVector4<T> TVector4<T>::operator/(const TVector4& v) const
{
	return TVector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

template<typename T>
inline TVector4<T> TVector4<T>::operator+(T t) const
{
	return TVector4(x + t, y + t, z + t, w + t);
}

template<typename T>
inline TVector4<T> TVector4<T>::operator-(T t) const
{
	return TVector4(x - t, y - t, z - t, w - t);
}

template<typename T>
inline TVector4<T> TVector4<T>::operator*(T t) const
{
	return TVector4(x * t, y * t, z * t, w * t);
}

template<typename T>
inline TVector4<T> TVector4<T>::operator/(T t) const
{
	return TVector4(x / t, y / t, z / t, w / t);
}

template<typename T>
inline TVector4<T> operator+ (T lhs, const TVector4<T>& rhs)
{
	return TVector4<T>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
}

template<typename T>
inline TVector4<T> operator- (T lhs, const TVector4<T>& rhs)
{
	return TVector4<T>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
}
template<typename T>
inline TVector4<T> operator* (T lhs, const TVector4<T>& rhs)
{
	return TVector4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}

template<typename T>
inline TVector4<T> operator/ (T lhs, const TVector4<T>& rhs)
{
	return TVector4<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
}

CUTE_NS_END
