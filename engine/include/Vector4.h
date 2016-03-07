#pragma once
#include "API.h"

CU_NS_BEGIN

template<typename T>
class TVector4
{
public:
	typedef typename T type_t;
	static const TVector4 ZERO;
	static const TVector4 ONE;
	static const TVector4 UNIT_X;
	static const TVector4 UNIT_Y;
	static const TVector4 UNIT_Z;
	static const TVector4 UNIT_W;

	static T length(const TVector4& v)
	{
		return v.length();
	}

	static T squared(const TVector4& v)
	{
		return v.squared();
	}

	static T distance(const TVector4& p0, const TVector4& p1)
	{
		return length(p1 - p0);
	}

	static T dot(const TVector4& v1, const TVector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	TVector4 normalize(const TVector4& v)
	{
		T len = v.length();
		return len > 0 ? (v / len) : v;
	}

public:
	TVector4() :x(0), y(0), z(0), w(0){}
	TVector4(T v) :x(v), y(v), z(v), w(v){}
	TVector4(T x, T y, T z, T w) :x(x), y(y), z(z), w(w){}

	void set(T x, T y, T z, T w){ this->x = x; this->y = y; this->z = z; this->w = w; }

	T length() const { return sqrt((float)squared()); }
	T squared() const { return x*x + y*y + z*z + w*w; }

	size_t size() const { return sizeof(*this); }
	T* data() { return m; }
	const T* data() const { return m; }

	String toString() const
	{
		std::stringstream ss;
		ss << x << " " << y << " " << z << " " << w;
		return ss.str();
	}

	T operator[](size_t i) const { return m[i]; }
	T& operator[](size_t i) { return m[i]; }

	const TVector4& operator+() const { return *this; }
	TVector4 operator-() const { return TVector4(-x, -y, -z, -w); }

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

template<typename T> const TVector4<T> TVector4<T>::ZERO(0);
template<typename T> const TVector4<T> TVector4<T>::ONE(1);
template<typename T> const TVector4<T> TVector4<T>::UNIT_X(1, 0, 0, 0);
template<typename T> const TVector4<T> TVector4<T>::UNIT_Y(0, 1, 0, 0);
template<typename T> const TVector4<T> TVector4<T>::UNIT_Z(0, 0, 1, 0);
template<typename T> const TVector4<T> TVector4<T>::UNIT_W(0, 0, 0, 1);

typedef TVector4<int>	Vector4i;
typedef TVector4<float>	Vector4f;
typedef TVector4<float>	Vector4;


CU_NS_END