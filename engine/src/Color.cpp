#include "Color.h"
#include "CMath.h"
#include <limits>

CU_NS_BEGIN

const Color Color::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::BLACK(0.0f, 0.0f, 0.0f);
const Color Color::WHITE(1.0f, 1.0f, 1.0f);
const Color Color::RED(1.0f, 0.0f, 0.0f);
const Color Color::GREEN(0.0f, 1.0f, 0.0f);
const Color Color::BLUE(0.0f, 0.0f, 1.0f);
const Color Color::GRAY(0.5f, 0.5f, 0.5f);
const Color Color::YELLOW(1.0f, 1.0f, 0.0f);
const Color Color::CYAN(0.0f, 1.0f, 1.0f);
const Color Color::MAGENTA(1.0f, 0.0f, 1.0f);
const Color Color::TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);

const Color Color::DEFAULT = Color(143.0f / 255.0f, 111.0f / 255.0f, 0);

Color::Color()
	:r(1.0f), g(1.0f), b(1.0f), a(1.0f)
{

}

Color::Color(float red, float green, float blue, float alpha /* = 1.0f */)
	:r(red), g(green), b(blue), a(alpha)
{
}

Color::Color(const float* data)
	: r(data[0])
	, g(data[1])
	, b(data[2])
	, a(data[3])
{

}

float Color::chroma() const
{
	float min, max;
	bounds(min, max, true);
	return max - min;
}

float Color::hue() const
{
	float min, max;
	bounds(min, max, true);
	return calcHue(min, max);

}

float Color::lightness() const
{
	float min, max;
	bounds(min, max, true);

	return (max + min) * 0.5f;
}

float Color::saturationHSL() const
{
	float min, max;
	bounds(min, max, true);
	return calcSaturationHSL(min, max);
}

float Color::saturationHSV() const
{
	float min, max;
	bounds(min, max, true);
	return calcSaturationHSV(min, max);
}


float Color::calcHue(float min, float max) const
{
	float chroma = max - min;

	// If chroma equals zero, hue is undefined
	if (chroma <= MATH_EPSILON)
		return 0.0f;

	// Calculate and return hue
	if (Math::equals(g, max))
		return (b + 2.0f * chroma - r) / (6.0f * chroma);
	else if (Math::equals(b, max))
		return (4.0f * chroma - g + r) / (6.0f * chroma);
	else
	{
		float tr = (g - b) / (6.0f * chroma);
		return (tr < 0.0f) ? 1.0f + tr : ((tr >= 1.0f) ? tr - 1.0f : tr);
	}
}

float Color::calcSaturationHSL(float min, float max) const
{
	// Avoid div-by-zero: result undefined
	if (max <= MATH_EPSILON || min >= 1.0f - MATH_EPSILON)
		return 0.0f;

	// Chroma = max - min, lightness = (max + min) * 0.5
	float hl = (max + min);
	if (hl <= 1.0f)
		return (max - min) / hl;
	else
		return (min - max) / (hl - 2.0f);
}

float Color::calcSaturationHSV(float min, float max) const
{
	// Avoid div-by-zero: result undefined
	if (max <= MATH_EPSILON)
		return 0.0f;

	// Saturation equals chroma:value ratio
	return 1.0f - (min / max);
}

void Color::bounds(float& min, float& max, bool clipped /* = false */) const
{
	if (r > g)
	{
		if (g > b)
		{
			max = r;
			min = b;
		}
		else
		{
			max = r > b ? r : b;
			min = g;
		}
	}
	else
	{
		if (b > g)
		{
			max = b;
			min = r;
		}
		else
		{
			max = g;
			min = r < b ? r : b;
		}
	}

	if (clipped)
	{
		max = max > 1.0f ? 1.0f : (max < 0.0f ? 0.0f : max);
		min = min > 1.0f ? 1.0f : (min < 0.0f ? 0.0f : min);
	}
}

void Color::saturate()
{
	if (r < 0)
		r = 0;
	else if (r > 1)
		r = 1;

	if (g < 0)
		g = 0;
	else if (g > 1)
		g = 1;

	if (b < 0)
		b = 0;
	else if (b > 1)
		b = 1;

	if (a < 0)
		a = 0;
	else if (a > 1)
		a = 1;
}

uint32_t Color::convertTo(float c0, float c1, float c2, float c3) const
{
	// Convert to 32bit pattern
	// (RGBA = 8888)

	uint8_t val8;
	uint32_t val32 = 0;

#ifdef CU_LITTLE_ENDIAN
	// Red
	val8 = static_cast<uint8_t>(c0 * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<uint8_t>(c1 * 255);
	val32 += val8 << 16;

	// Blue
	val8 = static_cast<uint8_t>(c2 * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<uint8_t>(c3 * 255);
	val32 += val8;
#else
	// Red
	val8 = static_cast<uint8_t>(c3 * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<uint8_t>(c2 * 255);
	val32 += val8 << 16;

	// Blue
	val8 = static_cast<uint8_t>(c1 * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<uint8_t>(c0 * 255);
	val32 += val8;
#endif

	return val32;
}

void Color::convertFrom(uint32_t val, float& c0, float& c1, float& c2, float& c3) const
{
	uint32_t val32 = val;
#ifdef CU_LITTLE_ENDIAN
	// Red
	c0 = ((val32 >> 24) & 0xFF) / 255.0f;
	// Green
	c1 = ((val32 >> 16) & 0xFF) / 255.0f;
	// Blue
	c2 = ((val32 >> 8) & 0xFF) / 255.0f;
	// Alpha
	c3 = (val32 & 0xFF) / 255.0f;
#else
	// Red
	c3 = ((val32 >> 24) & 0xFF) / 255.0f;
	// Green
	c2 = ((val32 >> 16) & 0xFF) / 255.0f;
	// Blue
	c1 = ((val32 >> 8) & 0xFF) / 255.0f;
	// Alpha
	c0 = (val32 & 0xFF) / 255.0f;
#endif
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
	saturation = (std::min)(saturation, (float)1.0);
	saturation = (std::max)(saturation, (float)0.0);
	brightness = (std::min)(brightness, (float)1.0);
	brightness = (std::max)(brightness, (float)0.0);

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

void Color::getHSB(float& hue, float& saturation, float& brightness) const
{
	float vMin, vMax;
	bounds(vMin, vMax);
	float delta = vMax - vMin;

	brightness = vMax;

	//if (Math::RealEqual(delta, 0.0f, 1e-6))
	if (delta < 1e-6)
	{
		// grey
		hue = 0;
		saturation = 0;
	}
	else
	{
		// a colour
		saturation = delta / vMax;

		float deltaR = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
		float deltaG = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
		float deltaB = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;

		if (vMax - r < std::numeric_limits<float>::epsilon())
			hue = deltaB - deltaG;
		else if (vMax - g < std::numeric_limits<float>::epsilon())
			hue = 0.3333333f + deltaR - deltaB;
		else if (vMax - b < std::numeric_limits<float>::epsilon())
			hue = 0.6666667f + deltaG - deltaR;

		if (hue < 0.0f)
			hue += 1.0f;
		if (hue > 1.0f)
			hue -= 1.0f;
	}
}

String Color::toString() const
{
	char buf[128];
	sprintf(buf, "%g %g %g %g", r, g, b, a);
	return String(buf);
}

CU_NS_END