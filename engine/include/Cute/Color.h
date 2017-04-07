#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

typedef uint32 RGBA;
typedef uint32 ARGB;
typedef uint32 ABGR;
typedef uint32 BGRA;

class CUTE_CORE_API Color
{
public:
	static const Color ZERO;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;

	explicit Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);

	RGBA getRGBA(void) const;
	ARGB getARGB(void) const;
	BGRA getBGRA(void) const;
	ABGR getABGR(void) const;

	void setRGBA(const RGBA val);
	void setARGB(const ARGB val);
	void setBGRA(const BGRA val);
	void setABGR(const ABGR val);

	/** Clamps colour value to the range [0, 1]. */
	void saturate();

	/**
	* Set a colour value from Hue, Saturation and Brightness.
	*
	* @param[in] hue			Hue value, scaled to the [0,1] range.
	* @param[in] saturation	Saturation level, [0,1].
	* @param[in] brightness	Brightness level, [0,1].
	*/
	void setHSB(float hue, float saturation, float brightness);

	/**
	* Convert the current color to Hue, Saturation and Brightness values.
	*
	* @param[in] hue			Output hue value, scaled to the [0,1] range.
	* @param[in] saturation	Output saturation level, [0,1].
	* @param[in] brightness	Output brightness level, [0,1].
	*/
	void getHSB(float* hue, float* saturation, float* brightness) const;

	float* data();
	const float* data() const;

	float  operator[] (const size_t i) const;
	float& operator[] (const size_t i);

	bool operator==(const Color& rhs) const;
	bool operator!=(const Color& rhs) const;

	Color& operator+= (const Color& rhs);
	Color& operator-= (const Color& rhs);
	Color& operator*= (const Color& rhs);
	Color& operator/= (const Color& rhs);

	Color& operator+= (const float rhs);
	Color& operator-= (const float rhs);
	Color& operator*= (const float rhs);
	Color& operator/= (const float rhs);

	Color operator+ (const Color& rhs) const;
	Color operator- (const Color& rhs) const;
	Color operator* (const Color& rhs) const;
	Color operator/ (const Color& rhs) const;

	Color operator+ (const float rhs) const;
	Color operator- (const float rhs) const;
	Color operator* (const float rhs) const;
	Color operator/ (const float rhs) const;

	friend Color operator+ (const float lhs, const Color& rhs);
	friend Color operator* (const float lhs, const Color& rhs);

public:
	float r, g, b, a;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
float* Color::data()
{
	return &r;
}

const float* Color::data() const
{
	return &r;
}

float Color::operator[] (const size_t i) const
{
	assert(i < 4);

	return *(&r + i);
}

float& Color::operator[] (const size_t i)
{
	assert(i < 4);

	return *(&r + i);
}

bool Color::operator==(const Color& rhs) const
{
	return
		r == rhs.r &&
		g == rhs.g &&
		b == rhs.b &&
		a == rhs.a;
}

bool Color::operator!=(const Color& rhs) const
{
	return !(*this == rhs);
}

Color& Color::operator+= (const Color& rhs)
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	a += rhs.a;

	return *this;
}

Color& Color::operator-= (const Color& rhs)
{
	r -= rhs.r;
	g -= rhs.g;
	b -= rhs.b;
	a -= rhs.a;

	return *this;
}

Color& Color::operator*= (const Color& rhs)
{
	r *= rhs.r;
	g *= rhs.g;
	b *= rhs.b;
	a *= rhs.a;

	return *this;
}

Color& Color::operator/= (const Color& rhs)
{
	r /= rhs.r;
	g /= rhs.g;
	b /= rhs.b;
	a /= rhs.a;

	return *this;
}

Color& Color::operator+= (const float rhs)
{
	r += rhs;
	g += rhs;
	b += rhs;
	a += rhs;

	return *this;
}

Color& Color::operator-= (const float rhs)
{
	r -= rhs;
	g -= rhs;
	b -= rhs;
	a -= rhs;

	return *this;
}

Color& Color::operator*= (const float rhs)
{
	r *= rhs;
	g *= rhs;
	b *= rhs;
	a *= rhs;

	return *this;
}

Color& Color::operator/= (const float rhs)
{
	r /= rhs;
	g /= rhs;
	b /= rhs;
	a /= rhs;

	return *this;
}

Color Color::operator+ (const Color& rhs) const
{
	return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
}

Color Color::operator- (const Color& rhs) const
{
	return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
}

Color Color::operator* (const Color& rhs) const
{
	return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
}

Color Color::operator/ (const Color& rhs) const
{
	return Color(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
}

Color Color::operator+ (float rhs) const
{
	return Color(r + rhs, g + rhs, b + rhs, a + rhs);
}

Color Color::operator- (float rhs) const
{
	return Color(r - rhs, g - rhs, b - rhs, a - rhs);
}

Color Color::operator* (float rhs) const
{
	return Color(r * rhs, g * rhs, b * rhs, a * rhs);
}

Color Color::operator/ (float rhs) const
{
	assert(rhs != 0);
	return Color(r / rhs, g / rhs, b / rhs, a / rhs);
}

Color operator+ (const float lhs, const Color& rhs)
{
	return Color(lhs + rhs.r, lhs + rhs.g, lhs + rhs.b, lhs + rhs.a);
}

Color operator* (const float lhs, const Color& rhs)
{
	return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
}

CUTE_NS_END