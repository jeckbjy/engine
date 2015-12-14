#include "Color.h"
#include <limits>

CU_NS_BEGIN

const Color Color::Zero = Color(0.0, 0.0, 0.0, 0.0);
const Color Color::Black = Color(0.0, 0.0, 0.0);
const Color Color::White = Color(1.0, 1.0, 1.0);
const Color Color::Red = Color(1.0, 0.0, 0.0);
const Color Color::Green = Color(0.0, 1.0, 0.0);
const Color Color::Blue = Color(0.0, 0.0, 1.0);

const Color Color::Default = Color(143.0f / 255.0f, 111.0f / 255.0f, 0);

Color::Color(float red /* = 1.0f */, float green /* = 1.0f */, float blue /* = 1.0f */, float alpha /* = 1.0f */)
	:r(red), g(green), b(blue), a(alpha)
{
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
	float vMin = std::min(r, std::min(g, b));
	float vMax = std::max(r, std::max(g, b));
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

CU_NS_END