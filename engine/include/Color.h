#pragma once
#include "API.h"
#include "Vector3.h"
#include "Vector4.h"

// Defined by Windows headers
#ifdef TRANSPARENT
#undef TRANSPARENT
#endif

CU_NS_BEGIN

typedef uint32_t RGBA;
typedef uint32_t ARGB;
typedef uint32_t ABGR;
typedef uint32_t BGRA;

class CU_API Color
{
public:
	static const Color ZERO;
	static const Color BLACK;
	static const Color WHITE;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color GRAY;
	static const Color YELLOW;
	static const Color CYAN;
	static const Color MAGENTA;
	static const Color TRANSPARENT;
	static const Color DEFAULT;

public:
	float r, g, b, a;

public:
	Color();
	Color(float red, float green, float blue, float alpha = 1.0f);
	Color(const float* data);

	float sum() const { return r + g + b; }
	float average() const { return (r + g + b) / 3.0f; }

	/// Return the 'grayscale' representation of RGB values, as used by JPEG and PAL/NTSC among others.
	float luma() const { return r *0.299f + g*0.587f + b*0.114f; }
	/// Return the colorfulness relative to the brightness of a similarly illuminated white.
	float chroma() const;
	float hue() const;
	/// Return lightness as defined for HSL: average of the largest and smallest values of the RGB components.
	float lightness() const;
	float saturationHSL() const;
	float saturationHSV() const;

	void bounds(float& min, float& max, bool clipped = false) const;
	void saturate();	// 裁剪 clamps[0,1]
	// hsb都映射到[0,1]:色相(0-360)，饱和度(0-100)，亮度模型(0-100)
	void setHSB(float hue, float saturation, float brightness);
	void getHSB(float& hue, float& saturation, float& brightness) const;

	// 转换成int模式
	RGBA getRGBA() const { return convertTo(r, g, b, a); }
	ARGB getARGB() const { return convertTo(a, r, g, b); }
	BGRA getBGRA() const { return convertTo(b, g, r, a); }
	ABGR getABGR() const { return convertTo(a, b, g, r); }

	void setRGBA(RGBA val){ convertFrom(val, r, g, b, a); }
	void setARGB(ARGB val){ convertFrom(val, a, r, g, b); }
	void setBGRA(BGRA val){ convertFrom(val, b, g, r, a); }
	void setABGR(ABGR val){ convertFrom(val, a, b, g, r); }

	Vector3 toVector3() const { return Vector3(r, g, b); }
	Vector4 toVector4() const { return Vector4(r, g, b, a); }
	String  toString() const;

	const float* data() const { return &r; }
	float* data() { return &r; }

	float operator[](const size_t i) const{ return *(&r + i); }
	float& operator[](const size_t i) { return *(&r + i); }

	bool operator ==(const Color& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }
	bool operator !=(const Color& rhs) const { return !(*this == rhs); }

	Color& operator +=(const Color& rhs);
	Color& operator -=(const Color& rhs);
	Color& operator *=(const Color& rhs);
	Color& operator /=(const Color& rhs);
	Color& operator *=(const float scalar);
	Color& operator /=(const float scalar);

	Color operator+(const Color& rhs) const { Color tmp = *this; tmp += rhs; return tmp; }
	Color operator-(const Color& rhs) const { Color tmp = *this; tmp -= rhs; return tmp; }
	Color operator*(const Color& rhs) const { Color tmp = *this; tmp *= rhs; return tmp; }
	Color operator/(const Color& rhs) const { Color tmp = *this; tmp /= rhs; return tmp; }
	Color operator*(const float scalar) const { Color tmp = *this; tmp *= scalar; return tmp; }
	Color operator/(const float scalar) const { Color tmp = *this; tmp /= scalar; return tmp; }

	friend Color operator*(const float scalar, const Color& color) { return color * scalar; }

private:
	uint32_t convertTo(float c0, float c1, float c2, float c3) const;
	void convertFrom(uint32_t val, float& c0, float& c1, float& c2, float& c3) const;
	float calcHue(float min, float max) const;
	float calcSaturationHSL(float min, float max) const;
	float calcSaturationHSV(float min, float max) const;
};

//inline std::ostream& operator <<(std::ostream& os, const Color& c)
//{
//	os << c.r << "," << c.g << "," << c.a;
//	return os;
//}
//
//inline std::istream& operator>>(std::istream& is, Color& c)
//{
//	char ch;
//	is >> c.r >> ch >> c.g >> ch >> c.a;
//	return is;
//}

inline Color& Color::operator +=(const Color& rhs)
{
	r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a;
	return *this;
}

inline Color& Color::operator -=(const Color& rhs)
{
	r -= rhs.r; g -= rhs.g; b -= rhs.b; a -= rhs.a;
	return *this;
}

inline Color& Color::operator *=(const Color& rhs)
{
	r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a;
	return *this;
}

inline Color& Color::operator /=(const Color& rhs)
{
	r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a;
	return *this;
}

inline Color& Color::operator *=(const float scalar)
{
	r *= scalar; g *= scalar; b *= scalar; a *= scalar;
	return *this;
}

inline Color& Color::operator /=(const float scalar)
{
	if (scalar == 0.0f)
		return *this;
	r /= scalar; g /= scalar; b /= scalar; a /= scalar;
	return *this;
}

CU_NS_END