//! Render
#include "Cute/Color.h"
#include "Cute/Math.h"

CUTE_NS_BEGIN

const Color Color::ZERO	 = Color(0.0, 0.0, 0.0, 0.0);
const Color Color::Black = Color(0.0, 0.0, 0.0);
const Color Color::White = Color(1.0, 1.0, 1.0);
const Color Color::Red   = Color(1.0, 0.0, 0.0);
const Color Color::Green = Color(0.0, 1.0, 0.0);
const Color Color::Blue  = Color(0.0, 0.0, 1.0);

Color::Color(float red /* = 1.0f */, float green /* = 1.0f */, float blue /* = 1.0f */, float alpha /* = 1.0f */)
	: r(red), g(green), b(blue), a(alpha)
{
}

void Color::set(float red, float green, float blue, float alpha)
{
    this->r = red;
    this->g = green;
    this->b = blue;
    this->a = alpha;
}

ABGR Color::getABGR() const
{
	uint8 val8;
	uint32 val32 = 0;

	// Convert to 32bit pattern
	// (RGBA = 8888)

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 16;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 += val8;

	return val32;
}

BGRA Color::getBGRA() const
{
	uint8 val8;
	uint32 val32 = 0;

	// Convert to 32bit pattern
	// (ARGB = 8888)

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 = val8 << 24;

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 += val8 << 16;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 8;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8;

	return val32;
}

ARGB Color::getARGB() const
{
	uint8 val8;
	uint32 val32 = 0;

	// Convert to 32bit pattern
	// (ARGB = 8888)

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 16;

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 += val8;


	return val32;
}

RGBA Color::getRGBA() const
{
	uint8 val8;
	uint32 val32 = 0;

	// Convert to 32bit pattern
	// (ABRG = 8888)

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 = val8 << 24;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8 << 16;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 8;

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 += val8;


	return val32;
}

void Color::setABGR(const ABGR val)
{
	uint32 val32 = val;

	// Convert from 32bit pattern
	// (RGBA = 8888)

	// Red
	r = ((val32 >> 24) & 0xFF) / 255.0f;

	// Green
	g = ((val32 >> 16) & 0xFF) / 255.0f;

	// Blue
	b = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	a = (val32 & 0xFF) / 255.0f;
}

void Color::setBGRA(const BGRA val)
{
	uint32 val32 = val;

	// Convert from 32bit pattern
	// (ARGB = 8888)

	// Alpha
	a = ((val32 >> 24) & 0xFF) / 255.0f;

	// Red
	r = ((val32 >> 16) & 0xFF) / 255.0f;

	// Green
	g = ((val32 >> 8) & 0xFF) / 255.0f;

	// Blue
	b = (val32 & 0xFF) / 255.0f;
}

void Color::setARGB(const ARGB val)
{
	uint32 val32 = val;

	// Convert from 32bit pattern
	// (ARGB = 8888)

	// Blue
	b = ((val32 >> 24) & 0xFF) / 255.0f;

	// Green
	g = ((val32 >> 16) & 0xFF) / 255.0f;

	// Red
	r = ((val32 >> 8) & 0xFF) / 255.0f;

	// Alpha
	a = (val32 & 0xFF) / 255.0f;
}

void Color::setRGBA(const RGBA val)
{
	uint32 val32 = val;

	// Convert from 32bit pattern
	// (ABGR = 8888)

	// Alpha
	a = ((val32 >> 24) & 0xFF) / 255.0f;

	// Blue
	b = ((val32 >> 16) & 0xFF) / 255.0f;

	// Green
	g = ((val32 >> 8) & 0xFF) / 255.0f;

	// Red
	r = (val32 & 0xFF) / 255.0f;
}

void Color::saturate()
{
	r = Math::clamp01(r);
	g = Math::clamp01(g);
	b = Math::clamp01(b);
	a = Math::clamp01(a);
}

void Color::setHSB(float hue, float saturation, float brightness)
{
	// wrap hue
	if (hue > 1.0f)
	{
		hue -= (int)hue;
	}
	else if (hue < 0.0f)
	{
		hue += (int)hue + 1;
	}
	// clamp saturation / brightness
	saturation = std::min(saturation, (float)1.0);
	saturation = std::max(saturation, (float)0.0);
	brightness = std::min(brightness, (float)1.0);
	brightness = std::max(brightness, (float)0.0);

	if (brightness == 0.0f)
	{
		// early exit, this has to be black
		r = g = b = 0.0f;
		return;
	}

	if (saturation == 0.0f)
	{
		// early exit, this has to be grey

		r = g = b = brightness;
		return;
	}

	float hueDomain = hue * 6.0f;
	if (hueDomain >= 6.0f)
	{
		// wrap around, and allow mathematical errors
		hueDomain = 0.0f;
	}
	unsigned short domain = (unsigned short)hueDomain;
	float f1 = brightness * (1 - saturation);
	float f2 = brightness * (1 - saturation * (hueDomain - domain));
	float f3 = brightness * (1 - saturation * (1 - (hueDomain - domain)));

	switch (domain)
	{
	case 0:
		// red domain; green ascends
		r = brightness;
		g = f3;
		b = f1;
		break;
	case 1:
		// yellow domain; red descends
		r = f2;
		g = brightness;
		b = f1;
		break;
	case 2:
		// green domain; blue ascends
		r = f1;
		g = brightness;
		b = f3;
		break;
	case 3:
		// cyan domain; green descends
		r = f1;
		g = f2;
		b = brightness;
		break;
	case 4:
		// blue domain; red ascends
		r = f3;
		g = f1;
		b = brightness;
		break;
	case 5:
		// magenta domain; blue descends
		r = brightness;
		g = f1;
		b = f2;
		break;
	}
}

void Color::getHSB(float* hue, float* saturation, float* brightness) const
{
	float vMin = std::min(r, std::min(g, b));
	float vMax = std::max(r, std::max(g, b));
	float delta = vMax - vMin;

	*brightness = vMax;

	if (Math::equals(delta, 0.0f, 1e-6f))
	{
		// grey
		*hue = 0;
		*saturation = 0;
	}
	else
	{
		// a colour
		*saturation = delta / vMax;

		float deltaR = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
		float deltaG = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
		float deltaB = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;

		if (Math::equals(r, vMax))
			*hue = deltaB - deltaG;
		else if (Math::equals(g, vMax))
			*hue = 0.3333333f + deltaR - deltaB;
		else if (Math::equals(b, vMax))
			*hue = 0.6666667f + deltaG - deltaR;

		if (*hue < 0.0f)
			*hue += 1.0f;
		if (*hue > 1.0f)
			*hue -= 1.0f;
	}
}

CUTE_NS_END
