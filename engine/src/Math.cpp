#include "Cute/Math.h"

CUTE_NS_BEGIN

#if defined(_MSC_VER)
#define IS_INF(x)	_finite(x) != 0
#define IS_NAN(x)	_isnan(x) != 0
#elif defined(__MINGW32__)
#define IS_INF(x)	isfinite(x)
#define IS_NAN(x)	isnan(x)
#else
#define IS_INF(x)	isinf(x)
#define IS_NAN(x)	isnan(x)
#endif

const float Math::POS_INFINITY = std::numeric_limits<float>::infinity();
const float Math::NEG_INFINITY = -std::numeric_limits<float>::infinity();
const float Math::PI = (float)4.0f * std::atan(1.0f);
const float Math::TWO_PI = (float)(2.0f * PI);
const float Math::HALF_PI = (float)(0.5f * PI);
const float Math::DEG2RAD = PI / 180.0f;
const float Math::RAD2DEG = 180.0f / PI;
const float Math::LOG2 = std::log(2.0f);
const float Math::EPSILON = std::numeric_limits<float>::epsilon();

float Math::acos(float value)
{
	if (-1.0f < value)
	{
		return value < 1.0f ? std::acos(value) : 0.0f;
	}
	else
	{
		return PI;
	}
}

float Math::asin(float value)
{
	if (-1.0f < value)
	{
		return value < 1.0f ? std::asin(value) : HALF_PI;
	}
	else
	{
		return -HALF_PI;
	}
}

float Math::invSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f375a86 - (i >> 1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x*(1.5f - xhalf*x*x); // Newton step, repeating increases accuracy
	return x;
}

bool Math::isInfinite(float value)
{
	return IS_INF(value);
}

bool Math::isInfinite(double value)
{
	return IS_INF(value);
}

bool Math::isNaN(float value)
{
	return IS_NAN(value);
}

bool Math::isNaN(double value)
{
	return IS_NAN(value);
}

float Math::fsin0(float val)
{
	float angleSqr = val*val;
	float result = 7.61e-03f;
	result *= angleSqr;
	result -= 1.6605e-01f;
	result *= angleSqr;
	result += 1.0f;
	result *= val;
	return result;
}

float Math::fsin1(float val)
{
	float angleSqr = val*val;
	float result = -2.39e-08f;
	result *= angleSqr;
	result += 2.7526e-06f;
	result *= angleSqr;
	result -= 1.98409e-04f;
	result *= angleSqr;
	result += 8.3333315e-03f;
	result *= angleSqr;
	result -= 1.666666664e-01f;
	result *= angleSqr;
	result += 1.0f;
	result *= val;

	return result;
}

float Math::fcos0(float val)
{
	float angleSqr = val*val;
	float result = 3.705e-02f;
	result *= angleSqr;
	result -= 4.967e-01f;
	result *= angleSqr;
	result += 1.0f;

	return result;
}

float Math::fcos1(float val)
{
	float angleSqr = val*val;
	float result = -2.605e-07f;
	result *= angleSqr;
	result += 2.47609e-05f;
	result *= angleSqr;
	result -= 1.3888397e-03f;
	result *= angleSqr;
	result += 4.16666418e-02f;
	result *= angleSqr;
	result -= 4.999999963e-01f;
	result *= angleSqr;
	result += 1.0f;

	return result;
}

float Math::ftan0(float val)
{
	float angleSqr = val*val;
	float result = 2.033e-01f;
	result *= angleSqr;
	result += 3.1755e-01f;
	result *= angleSqr;
	result += 1.0f;
	result *= val;
	return result;
}

float Math::ftan1(float val)
{
	float angleSqr = val*val;
	float result = 9.5168091e-03f;
	result *= angleSqr;
	result += 2.900525e-03f;
	result *= angleSqr;
	result += 2.45650893e-02f;
	result *= angleSqr;
	result += 5.33740603e-02f;
	result *= angleSqr;
	result += 1.333923995e-01f;
	result *= angleSqr;
	result += 3.333314036e-01f;
	result *= angleSqr;
	result += 1.0f;
	result *= val;
	return result;
}

float Math::fasin0(float val)
{
	float root = sqrt(abs(1.0f - val));
	float result = -0.0187293f;
	result *= val;
	result += 0.0742610f;
	result *= val;
	result -= 0.2121144f;
	result *= val;
	result += 1.5707288f;
	result = HALF_PI - root*result;
	return result;
}

float Math::fasin1(float val)
{
	float root = sqrt(abs(1.0f - val));
	float result = -0.0012624911f;
	result *= val;
	result += 0.0066700901f;
	result *= val;
	result -= 0.0170881256f;
	result *= val;
	result += 0.0308918810f;
	result *= val;
	result -= 0.0501743046f;
	result *= val;
	result += 0.0889789874f;
	result *= val;
	result -= 0.2145988016f;
	result *= val;
	result += 1.5707963050f;
	result = HALF_PI - root*result;
	return result;
}

float Math::facos0(float val)
{
	float root = sqrt(abs(1.0f - val));
	float result = -0.0187293f;
	result *= val;
	result += 0.0742610f;
	result *= val;
	result -= 0.2121144f;
	result *= val;
	result += 1.5707288f;
	result *= root;
	return result;
}

float Math::facos1(float val)
{
	float root = sqrt(abs(1.0f - val));
	float result = -0.0012624911f;
	result *= val;
	result += 0.0066700901f;
	result *= val;
	result -= 0.0170881256f;
	result *= val;
	result += 0.0308918810f;
	result *= val;
	result -= 0.0501743046f;
	result *= val;
	result += 0.0889789874f;
	result *= val;
	result -= 0.2145988016f;
	result *= val;
	result += 1.5707963050f;
	result *= root;
	return result;
}

float Math::fatan0(float val)
{
	float valueSqr = val*val;
	float result = 0.0208351f;
	result *= valueSqr;
	result -= 0.085133f;
	result *= valueSqr;
	result += 0.180141f;
	result *= valueSqr;
	result -= 0.3302995f;
	result *= valueSqr;
	result += 0.999866f;
	result *= val;
	return result;
}

float Math::fatan1(float val)
{
	float valueSqr = val*val;
	float result = 0.0028662257f;
	result *= valueSqr;
	result -= 0.0161657367f;
	result *= valueSqr;
	result += 0.0429096138f;
	result *= valueSqr;
	result -= 0.0752896400f;
	result *= valueSqr;
	result += 0.1065626393f;
	result *= valueSqr;
	result -= 0.1420889944f;
	result *= valueSqr;
	result += 0.1999355085f;
	result *= valueSqr;
	result -= 0.3333314528f;
	result *= valueSqr;
	result += 1.0f;
	result *= val;
	return result;
}

CUTE_NS_END
