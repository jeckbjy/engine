#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Math
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
	static const float EPSILON;

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

	static void  sincos(float v, float& s, float& c);

	static float asin(float value);
	static float acos(float value);
	static float atan(float value) { return std::atan(value); }
	static float atan2(float y, float x) { return std::atan2(y, x); }

	static float sin(float radian) { return std::sin(radian); }
	static float cos(float radian) { return std::cos(radian); }
	static float tan(float radian) { return std::tan(radian); }

	static float toDegree(float radian) { return radian * 57.29577951f; }
	static float toRadian(float degree) { return degree * 0.0174532925f; }

	static float fovToZoom(float fov) { return 1.0f / tan(fov * 0.5f); }
	static float zoomToFov(float zoom) { return 2.0f * atan(1.0f / zoom); }

	static float sqrt(float value) { return std::sqrt(value); }
	static float invSqrt(float value);
	static float pow(float base, float exponent) { return (float)std::pow(base, exponent); }
	static float exp(float value) { return (float)std::exp(value); }
	static float log(float value) { return (float)std::log(value); }
	static float log2(float value) { return (float)std::log2(value); }
	static float logN(float base, float value) { return (float)(std::log(value) / std::log(base)); }

    static double pow10(int exponent);
    
	static int   ceil(float value)  { return (int)std::ceil(value); }
	static int	 floor(float value) { return (int)std::floor(value); }
	static int	 round(float value) { return (int)std::floor(value + 0.5f); }

	/** Checks if the value is a valid number. */
	static bool isNaN(float f);
	static bool isNaN(double value);

	static bool isInfinite(float value);
	static bool isInfinite(double value);

	static bool isZero(float a, float tolerance = EPSILON) { return fabs(a) <= tolerance; }
	static bool isZero(double a, double tolerance = std::numeric_limits<double>::epsilon()) { return fabs(a) <= tolerance; }

	static bool equals(float a, float b, float tolerance = EPSILON) { return fabs(a - b) <= tolerance; }
	static bool equals(double a, double b, double tolerance = std::numeric_limits<double>::epsilon()) { return fabs(a - b) <= tolerance; }

	template<typename T>
	static T min(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }

	template<typename T>
	static T max(T lhs, T rhs) { return lhs > rhs ? lhs : rhs; }

	template<typename T>
	static T min(T t1, T t2, T t3) { return std::min(std::min(t1, t2), t3); }

	template<typename T>
	static T max(T t1, T t2, T t3) { return std::max(std::max(t1, t2), t3); }

	template<typename T>
	static T sign(T v) { return v < (T)0 ? (T)-1 : (T)1; }

	template<typename T>
	static T abs(T v) { return (T)std::abs(v); }

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

	/** Clamp a value within an inclusive range. */
	template <typename T>
	static T clamp(T val, T minval, T maxval)
	{
		assert(minval <= maxval && "Invalid clamp range");
		return std::max(std::min(val, maxval), minval);
	}

	/** Clamp a value within an inclusive range [0..1]. */
	template <typename T>
	static T clamp01(T val)
	{
		return std::max(std::min(val, (T)1), (T)0);
	}

	/**
	* Interpolates between min and max. Returned value is in [0, 1] range where min = 0, max = 1 and 0.5 is
	* the average of min and max.
	*/
	template <typename T>
	static float lerp01(T val, T min, T max)
	{
		return clamp01((val - min) / std::max(max - min, 0.0001F));
	}

	// 平方和
	template<typename T>
	static T sumsq(T t1, T t2){ return t1*t1 + t2*t2; }

	template<typename T>
	static T sumsq(T t1, T t2, T t3) { return t1*t1 + t2*t2 + t3*t3; }

	template<typename T>
	static T sumsq(T t1, T t2, T t3, T t4) { return t1*t1 + t2*t2 + t3*t3 + t4*t4; }

	/**
	* Solves the linear equation with the parameters A, B. Returns number of roots found and the roots themselves will
	* be output in the @p roots array.
	*
	* @param[in]	A		First variable.
	* @param[in]	B		Second variable.
	* @param[out]	roots	Must be at least size of 1.
	*
	* @note	Only returns real roots.
	*/
	template <typename T>
	static uint32 solveLinear(T A, T B, T* roots);

	/**
	* Solves the quadratic equation with the parameters A, B, C. Returns number of roots found and the roots themselves
	* will be output in the @p roots array.
	*
	* @param[in]	A		First variable.
	* @param[in]	B		Second variable.
	* @param[in]	C		Third variable.
	* @param[out]	roots	Must be at least size of 2.
	*
	* @note	Only returns real roots.
	*/
	template <typename T>
	static uint32 solveQuadratic(T A, T B, T C, T* roots);

	/**
	* Solves the cubic equation with the parameters A, B, C, D. Returns number of roots found and the roots themselves
	* will be output in the @p roots array.
	*
	* @param[in]	A		First variable.
	* @param[in]	B		Second variable.
	* @param[in]	C		Third variable.
	* @param[in]	D		Fourth variable.
	* @param[out]	roots	Must be at least size of 3.
	*
	* @note	Only returns real roots.
	*/
	template <typename T>
	static uint32 solveCubic(T A, T B, T C, T D, T* roots);

	/**
	* Solves the quartic equation with the parameters A, B, C, D, E. Returns number of roots found and the roots
	* themselves will be output in the @p roots array.
	*
	* @param[in]	A		First variable.
	* @param[in]	B		Second variable.
	* @param[in]	C		Third variable.
	* @param[in]	D		Fourth variable.
	* @param[in]	E		Fifth variable.
	* @param[out]	roots	Must be at least size of 4.
	*
	* @note	Only returns real roots.
	*/
	template <typename T>
	static uint32 solveQuartic(T A, T B, T C, T D, T E, T* roots);

	/**
	* Evaluates a cubic Hermite curve at a specific point.
	*
	* @param[in]	t			Parameter that at which to evaluate the curve, in range [0, 1].
	* @param[in]	pointA		Starting point (at t=0).
	* @param[in]	pointB		Ending point (at t=1).
	* @param[in]	tangentA	Starting tangent (at t=0).
	* @param[in]	tangentB	Ending tangent (at t = 1).
	* @return					Evaluated value at @p t.
	*/
	template<class T>
	static T cubicHermite(float t, const T& pointA, const T& pointB, const T& tangentA, const T& tangentB);

	/**
	* Evaluates the first derivative of a cubic Hermite curve at a specific point.
	*
	* @param[in]	t			Parameter that at which to evaluate the curve, in range [0, 1].
	* @param[in]	pointA		Starting point (at t=0).
	* @param[in]	pointB		Ending point (at t=1).
	* @param[in]	tangentA	Starting tangent (at t=0).
	* @param[in]	tangentB	Ending tangent (at t = 1).
	* @return					Evaluated value at @p t.
	*/
	template<class T>
	static T cubicHermiteD1(float t, const T& pointA, const T& pointB, const T& tangentA, const T& tangentB);

	/**
	* Calculates coefficients needed for evaluating a cubic curve in Hermite form. Assumes @p t has been normalized is
	* in range [0, 1]. Tangents must be scaled by the length of the curve (length is the maximum value of @p t before
	* it was normalized).
	*
	* @param[in]	pointA			Starting point (at t=0).
	* @param[in]	pointB			Ending point (at t=1).
	* @param[in]	tangentA		Starting tangent (at t=0).
	* @param[in]	tangentB		Ending tangent (at t = 1).
	* @param[out]	coefficients	Four coefficients for the cubic curve, in order [t^3, t^2, t, 1].
	*/
	template<class T>
	static void cubicHermiteCoefficients(const T& pointA, const T& pointB, const T& tangentA, const T& tangentB, T(&coefficients)[4]);

	/**
	* Calculates coefficients needed for evaluating a cubic curve in Hermite form. Assumes @p t is in range
	* [0, @p length]. Tangents must not be scaled by @p length.
	*
	* @param[in]	pointA			Starting point (at t=0).
	* @param[in]	pointB			Ending point (at t=length).
	* @param[in]	tangentA		Starting tangent (at t=0).
	* @param[in]	tangentB		Ending tangent (at t=length).
	* @param[in]	length			Maximum value the curve will be evaluated at.
	* @param[out]	coefficients	Four coefficients for the cubic curve, in order [t^3, t^2, t, 1].
	*/
	template<class T>
	static void cubicHermiteCoefficients(const T& pointA, const T& pointB, const T& tangentA, const T& tangentB, float length, T(&coefficients)[4]);
};

template<typename T>
uint32 Math::solveLinear(T A, T B, T* roots)
{
	if (!equals(A, (T)0))
	{
		roots[0] = -B / A;
		return 1;
	}

	roots[0] = 0.0f;
	return 1;
}

template<typename T>
uint32 Math::solveQuadratic(T A, T B, T C, T* roots)
{
	if (!equals(A, (T)0))
	{
		T p = B / (2 * A);
		T q = C / A;
		T D = p * p - q;

		if (!equals(D, (T)0))
		{
			if (D < (T)0)
				return 0;

			T sqrtD = sqrt(D);
			roots[0] = sqrtD - p;
			roots[1] = -sqrtD - p;

			return 2;
		}
		else
		{
			roots[0] = -p;
			roots[1] = -p;

			return 1;
		}
	}
	else
	{
		return solveLinear(B, C, roots);
	}
}

template<typename T>
uint32 Math::solveCubic(T A, T B, T C, T D, T* roots)
{
	static const T THIRD = (1 / (T)3);

	T invA = 1 / A;
	A = B * invA;
	B = C * invA;
	C = D * invA;

	T sqA = A * A;
	T p = THIRD * (-THIRD * sqA + B);
	T q = ((T)0.5) * ((2 / (T)27) * A * sqA - THIRD * A * B + C);

	T cbp = p * p * p;
	D = q * q + cbp;

	uint32 numRoots = 0;
	if (!equals(D, (T)0))
	{
		if (D < 0.0)
		{
			T phi = THIRD * ::acos(-q / sqrt(-cbp));
			T t = 2 * sqrt(-p);

			roots[0] = t * cos(phi);
			roots[1] = -t * cos(phi + PI * THIRD);
			roots[2] = -t * cos(phi - PI * THIRD);

			numRoots = 3;
		}
		else
		{
			T sqrtD = sqrt(D);
			T u = cbrt(sqrtD + fabs(q));

			if (q > (T)0)
				roots[0] = -u + p / u;
			else
				roots[0] = u - p / u;

			numRoots = 1;
		}
	}
	else
	{
		if (!equals(q, (T)0))
		{
			T u = cbrt(-q);
			roots[0] = 2 * u;
			roots[1] = -u;

			numRoots = 2;
		}
		else
		{
			roots[0] = 0.0f;
			numRoots = 1;
		}
	}

	T sub = THIRD * A;
	for (uint32 i = 0; i < numRoots; i++)
		roots[i] -= sub;

	return numRoots;
}

template<typename T>
uint32 Math::solveQuartic(T A, T B, T C, T D, T E, T* roots)
{
	T invA = 1 / A;
	A = B * invA;
	B = C * invA;
	C = D * invA;
	D = E * invA;

	T sqA = A*A;
	T p = -(3 / (T)8) * sqA + B;
	T q = (1 / (T)8) * sqA * A - (T)0.5 * A * B + C;
	T r = -(3 / (T)256) * sqA * sqA + (1 / (T)16) * sqA * B - (1 / (T)4) * A * C + D;

	uint32 numRoots = 0;
	if (!equals(r, (T)0))
	{
		T cubicA = 1;
		T cubicB = -(T)0.5 * p;
		T cubicC = -r;
		T cubicD = (T)0.5 * r * p - (1 / (T)8) * q * q;

		solveCubic(cubicA, cubicB, cubicC, cubicD, roots);
		T z = roots[0];

		T u = z * z - r;
		T v = 2 * z - p;

		if (equals(u, T(0)))
			u = 0;
		else if (u > 0)
			u = sqrt(u);
		else
			return 0;

		if (equals(v, T(0)))
			v = 0;
		else if (v > 0)
			v = sqrt(v);
		else
			return 0;

		T quadraticA = 1;
		T quadraticB = q < 0 ? -v : v;
		T quadraticC = z - u;

		numRoots = solveQuadratic(quadraticA, quadraticB, quadraticC, roots);

		quadraticA = 1;
		quadraticB = q < 0 ? v : -v;
		quadraticC = z + u;

		numRoots += solveQuadratic(quadraticA, quadraticB, quadraticC, roots + numRoots);
	}
	else
	{
		numRoots = solveCubic(q, p, (T)0, (T)1, roots);
		roots[numRoots++] = 0;
	}

	T sub = (1 / (T)4) * A;
	for (uint32 i = 0; i < numRoots; i++)
		roots[i] -= sub;

	return numRoots;
}

template<typename T>
T Math::cubicHermite(float t, const T& pointA, const T& pointB, const T& tangentA, const T& tangentB)
{
	float t2 = t * t;
	float t3 = t2 * t;

	float a = 2 * t3 - 3 * t2 + 1;
	float b = t3 - 2 * t2 + t;
	float c = -2 * t3 + 3 * t2;
	float d = t3 - t2;

	return a * pointA + b * tangentA + c * pointB + d * tangentB;
}

template<typename T>
T Math::cubicHermiteD1(float t, const T& pointA, const T& pointB, const T& tangentA, const T& tangentB)
{
	float t2 = t * t;

	float a = 6 * t2 - 6 * t;
	float b = 3 * t2 - 4 * t + 1;
	float c = -6 * t2 + 6 * t;
	float d = 3 * t2 - 2 * t;

	return a * pointA + b * tangentA + c * pointB + d * tangentB;
}

template<typename T>
void Math::cubicHermiteCoefficients(const T& pointA, const T& pointB, const T& tangentA, const T& tangentB, T(&coefficients)[4])
{
	T diff = pointA - pointB;

	coefficients[0] = 2 * diff + tangentA + tangentB;
	coefficients[1] = -3 * diff - 2 * tangentA - tangentB;
	coefficients[2] = tangentA;
	coefficients[3] = pointA;
}

template<typename T>
void Math::cubicHermiteCoefficients(const T& pointA, const T& pointB, const T& tangentA, const T& tangentB, float length, T(&coefficients)[4])
{
	float length2 = length * length;
	float invLength2 = 1.0f / length2;
	float invLength3 = 1.0f / (length2 * length);

	T scaledTangentA = tangentA * length;
	T scaledTangentB = tangentB * length;

	T diff = pointA - pointB;

	coefficients[0] = (2 * diff + scaledTangentA + scaledTangentB) * invLength3;
	coefficients[1] = (-3 * diff - 2 * scaledTangentA - scaledTangentB) * invLength2;
	coefficients[2] = tangentA;
	coefficients[3] = pointA;
}

CUTE_NS_END
