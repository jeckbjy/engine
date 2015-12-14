#pragma once
#include "API.h"

CU_NS_BEGIN

typedef uint32_t RGBA;
typedef uint32_t ARGB;
typedef uint32_t ABGR;
typedef uint32_t BGRA;

class CU_API Color
{
public:
	static const Color Zero;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Default;

public:
	float r, g, b, a;

public:
	Color() = default;
	Color(float red, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);

	void saturate();	// 裁剪 clamps[0,1]
	// hsb都映射到[0,1]:色相(0-360)，饱和度(0-100)，亮度模型(0-100)
	void setHSB(float hue, float saturation, float brightness);
	void getHSB(float& hue, float& saturation, float& brightness) const;

	// 转换成int模式
	inline RGBA getRGBA() const { return convertTo(r, g, b, a); }
	inline ARGB getARGB() const { return convertTo(a, r, g, b); }
	inline BGRA getBGRA() const { return convertTo(b, g, r, a); }
	inline ABGR getABGR() const { return convertTo(a, b, g, r); }

	inline void setRGBA(RGBA val){ convertFrom(val, r, g, b, a); }
	inline void setARGB(ARGB val){ convertFrom(val, a, r, g, b); }
	inline void setBGRA(BGRA val){ convertFrom(val, b, g, r, a); }
	inline void setABGR(ABGR val){ convertFrom(val, a, b, g, r); }

	inline float operator[](const size_t i) const{ return *(&r + i); }
	inline float& operator[](const size_t i) { return *(&r + i); }

	inline bool operator ==(const Color& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }
	inline bool operator !=(const Color& rhs) const { return !(*this == rhs); }

	inline Color& operator +=(const Color& rhs);
	inline Color& operator -=(const Color& rhs);
	inline Color& operator *=(const Color& rhs);
	inline Color& operator /=(const Color& rhs);
	inline Color& operator *=(const float scalar);
	inline Color& operator /=(const float scalar);

	inline Color operator+(const Color& rhs) const { Color tmp = *this; tmp += rhs; return tmp; }
	inline Color operator-(const Color& rhs) const { Color tmp = *this; tmp -= rhs; return tmp; }
	inline Color operator*(const Color& rhs) const { Color tmp = *this; tmp *= rhs; return tmp; }
	inline Color operator/(const Color& rhs) const { Color tmp = *this; tmp /= rhs; return tmp; }
	inline Color operator*(const float scalar) const { Color tmp = *this; tmp *= scalar; return tmp; }
	inline Color operator/(const float scalar) const { Color tmp = *this; tmp /= scalar; return tmp; }

	inline friend Color operator*(const float scalar, const Color& color) { Color tmp = color; tmp *= scalar; return tmp; }

private:
	uint32_t convertTo(float c0, float c1, float c2, float c3) const;
	void convertFrom(uint32_t val, float& c0, float& c1, float& c2, float& c3) const;
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