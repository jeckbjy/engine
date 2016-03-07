#pragma once
#include "API.h"

CU_NS_BEGIN

enum EulerAngleOrder
{
	EAO_XYZ,
	EAO_XZY,
	EAO_YXZ,
	EAO_YZX,
	EAO_ZXY,
	EAO_ZYX
};

/// Intersection test result.
enum Intersection
{
	OUTSIDE,
	INTERSECTS,
	INSIDE
};

class CU_API Math
{
public:
	static const float POS_INFINITY;
	static const float NEG_INFINITY;
	static const float PI;
	static const float TWO_PI;
	static const float HALF_PI;
	static const float DEG2RAD;
	static const float RAD2DEG;
	static const float LOG2;

	static float toDegree(float radian) { return MATH_RAD_TO_DEG(radian);}
	static float toRadian(float degree) { return MATH_DEG_TO_RAD(degree); }

	static float fovToZoom(float fov) { return 1.0f / tan(fov * 0.5f); }
	static float zoomToFov(float zoom) { return 2.0f * atan(1.0f / zoom); }

	static void  sincos(float v, float& s, float& c);

	static float acos(float val);
	static float asin(float val);
	static float atan(float val) { return std::atan(val); }
	static float atan2(float y, float x) { return std::atan2(y, x); }

	static float cos(float val) { return (float)std::cos(val); }
	static float sin(float val) { return (float)std::sin(val); }
	static float tan(float val) { return (float)std::tan(val); }

	static float sqrt(float val) { return (float)std::sqrt(val); }
	static float invsqrt(float val);	// 1/sqrt(val);

	static float pow(float base, float exponent){ return (float)std::pow(base, exponent); }
	static float exp(float val) { return (float)std::exp(val); }

	static float log(float val)	{ return (float)std::log(val); }
	static float log2(float val){ return (float)(std::log(val) / MATH_LOG2E); }
	static float logN(float base, float val) { return (float)(std::log(val) / std::log(base)); }

	static float sign(float val);
	static float abs(float val) { return (float)std::fabs(val); }
	static float ceil(float val){ return (float)std::ceil(val); }
	static float floor(float val){ return (float)std::floor(val); }
	static float round(float val){ return (float)std::floor(val + 0.5f); }

	static int ceilToInt(float val) { return (int)std::ceil(val); }
	static int floorToInt(float val){ return (int)std::floor(val); }
	static int roundToInt(float val){ return (int)std::floor(val + 0.5f); }
	// Not a Num，只对于浮点数出现
	static bool isNaN(float f) { return f != f; }
	// 是否近似相等
	static bool equals(float a, float b, float tolerance = 0.000001f){ return fabs(a - b) <= tolerance; }
	static bool isZero(float a, float tolerance = 0.000001f) { return fabs(a) <= tolerance; }

	template<typename T>
	static T min(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }

	template<typename T>
	static T max(T lhs, T rhs) { return lhs > rhs ? lhs : rhs; }

	template<typename T>
	static T sign(T v) { return v < (T)0 ? (T)-1 : (T)1; }

	template<typename T>
	static T squared(T v) { return v*v; }

	template<typename T>
	static T signedSquared(T v) { return v < (T)0 ? -v*v : v*v; }

	// Checks is the specified value a power of two. Only works on integer values.
	template <typename T>
	static bool isPow2(T v)
	{
		//return (v != 0) && !(v & (v - 1));
		return (v & (v - 1)) == 0;
	}

	template <typename T>
	static T clamp(T val, T minval, T maxval)
	{
		assert(minval <= maxval && "Invalid clamp range");
		return std::max(std::min(val, maxval), minval);
	}

	// Clamp a value within an inclusive range [0..1].
	template <typename T>
	static T clamp01(T val)
	{
		return std::max(std::min(val, (T)1), (T)0);
	}

	// 平方和
	template<typename T>
	static T sumsq(T t1, T t2){ return t1*t1 + t2*t2; }

	template<typename T>
	static T sumsq(T t1, T t2, T t3) { return t1*t1 + t2*t2 + t3*t3; }

	template<typename T>
	static T sumsq(T t1, T t2, T t3, T t4) { return t1*t1 + t2*t2 + t3*t3 + t4*t4; }

	//////////////////////////////////////////////////////////////////////////
	// Evaluates trigonometric functions using polynomial approximations.
	// Trig Approximations 
	// xxx0:更快些，xxx1:更好些，都是弧度计算 fast sin cos tan asin acos atan
	//////////////////////////////////////////////////////////////////////////
	static float fsin0(float val);
	static float fsin1(float val);
	static float fcos0(float val);
	static float fcos1(float val);
	static float ftan0(float val);
	static float ftan1(float val);

	static float fasin0(float val);
	static float fasin1(float val);
	static float facos0(float val);
	static float facos1(float val);
	static float fatan0(float val);
	static float fatan1(float val);
};

CU_NS_END