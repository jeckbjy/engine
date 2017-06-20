//! Core
#include "Cute/Bitwise.h"
#if defined(CUTE_OS_FAMILY_WINDOWS)
#   include <intrin.h>   
#endif

CUTE_NS_BEGIN

#define EASTL_BITSET_COUNT_STRING "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4"

/** Converts float in UINT32 format to a a half in UINT16 format. */
static uint16 floatToHalfI(uint32 i)
{
	int s = (i >> 16) & 0x00008000;
	int e = ((i >> 23) & 0x000000ff) - (127 - 15);
	int m = i & 0x007fffff;

	if (e <= 0)
	{
		if (e < -10)
		{
			return 0;
		}
		m = (m | 0x00800000) >> (1 - e);

		return static_cast<uint16>(s | (m >> 13));
	}
	else if (e == 0xff - (127 - 15))
	{
		if (m == 0) // Inf
		{
			return static_cast<uint16>(s | 0x7c00);
		}
		else    // NAN
		{
			m >>= 13;
			return static_cast<uint16>(s | 0x7c00 | m | (m == 0));
		}
	}
	else
	{
		if (e > 30) // Overflow
		{
			return static_cast<uint16>(s | 0x7c00);
		}

		return static_cast<uint16>(s | (e << 10) | (m >> 13));
	}
}

static uint32 halfToFloatI(uint16 y)
{
	// step 1: uint16 to uint32;
	int s = (y >> 15) & 0x00000001;
	int e = (y >> 10) & 0x0000001f;
	int m = y & 0x000003ff;

	if (e == 0)
	{
		if (m == 0) // Plus or minus zero
		{
			return s << 31;
		}
		else // Denormalized number -- renormalize it
		{
			while (!(m & 0x00000400))
			{
				m <<= 1;
				e -= 1;
			}

			e += 1;
			m &= ~0x00000400;
		}
	}
	else if (e == 31)
	{
		if (m == 0) // Inf
		{
			return (s << 31) | 0x7f800000;
		}
		else // NaN
		{
			return (s << 31) | 0x7f800000 | (m << 13);
		}
	}

	e = e + (127 - 15);
	m = m << 13;

	return (s << 31) | (e << 23) | m;
}

uint Bitwise::popcount(uint8 data)
{
#if defined(__GNUC__) || defined(__clang__)
	return (uint)__builtin_popcountl(data);
#elif defined(_MSC_VER)
	return (uint)__popcnt16(data);
#else
#error "popcount not support!"
#endif
}

uint Bitwise::popcount(uint16 data)
{
#if defined(__GNUC__) || defined(__clang__)
	return (uint)__builtin_popcountl(data);
#elif defined(_MSC_VER)
	return (uint)__popcnt16(data);
#else
#error "popcount not support!"
#endif
}

uint Bitwise::popcount(uint32 data)
{
#if defined(__GNUC__) || defined(__clang__)
	return (uint)__builtin_popcountl(data);
#elif defined(_MSC_VER)
	return (uint)__popcnt16(data);
#else
#error "popcount not support!"
#endif
}

uint Bitwise::popcount(uint64 data)
{
#if defined(__GNUC__) || defined(__clang__)
	return (uint)__builtin_popcountll(data);
#elif defined(_MSC_VER) && defined(_WIN64)
	return (uint)__popcnt(data);
#else
	int n = 0;
	for (uint64 w = data; w; w >>= 4)
		n += EASTL_BITSET_COUNT_STRING[w & 0xF];

	return n;
#endif
}

uint32 Bitwise::mostSignificantBitSet(uint value)
{
	uint32 result = 0;
	while (value != 0) {
		++result;
		value >>= 1;
	}
	return result - 1;
}

uint32 Bitwise::nextPow2(uint32 n)
{
	--n;
	n |= n >> 16;
	n |= n >> 8;
	n |= n >> 4;
	n |= n >> 2;
	n |= n >> 1;
	++n;
	return n;
}

uint32 Bitwise::closestPow2(uint32 n)
{
	uint32 next = nextPow2(n);

	uint32 prev = next >> 1;
	if (n - prev < next - n)
		return prev;

	return next;
}

uint Bitwise::fixedToFixed(uint32 value, unsigned int n, unsigned int p)
{
	if (n > p)
	{
		// Less bits required than available; this is easy
		value >>= n - p;
	}
	else if (n < p)
	{
		// More bits required than are there, do the fill
		// Use old fashioned division, probably better than a loop
		if (value == 0)
			value = 0;
		else if (value == (static_cast<unsigned int>(1) << n) - 1)
			value = (1 << p) - 1;
		else    value = value*(1 << p) / ((1 << n) - 1);
	}
	return value;
}

uint Bitwise::floatToFixed(const float value, const unsigned int bits)
{
	if (value <= 0.0f) return 0;
	else if (value >= 1.0f) return (1 << bits) - 1;
	else return (unsigned int)(value * (1 << bits));
}

float Bitwise::fixedToFloat(unsigned value, unsigned int bits)
{
	return (float)value / (float)((1 << bits) - 1);
}

void Bitwise::intWrite(void *dest, const int n, const unsigned int value)
{
	switch (n) {
	case 1:
		((uint8*)dest)[0] = (uint8)value;
		break;
	case 2:
		((uint16*)dest)[0] = (uint16)value;
		break;
	case 3:
#if CUTE_ENDIAN == CUTE_ENDIAN_BIG      
		((uint8*)dest)[0] = (uint8)((value >> 16) & 0xFF);
		((uint8*)dest)[1] = (uint8)((value >> 8) & 0xFF);
		((uint8*)dest)[2] = (uint8)(value & 0xFF);
#else
		((uint8*)dest)[2] = (uint8)((value >> 16) & 0xFF);
		((uint8*)dest)[1] = (uint8)((value >> 8) & 0xFF);
		((uint8*)dest)[0] = (uint8)(value & 0xFF);
#endif
		break;
	case 4:
		((uint32*)dest)[0] = (uint32)value;
		break;
	}
}

uint Bitwise::intRead(const void *src, int n)
{
	switch (n) {
	case 1:
		return ((uint8*)src)[0];
	case 2:
		return ((uint16*)src)[0];
	case 3:
#if CUTE_ENDIAN == CUTE_ENDIAN_BIG      
		return 
			((uint32)((uint8*)src)[0] << 16) |
			((uint32)((uint8*)src)[1] << 8) |
			((uint32)((uint8*)src)[2]);
#else
		return
			((uint32)((uint8*)src)[0]) |
			((uint32)((uint8*)src)[1] << 8) |
			((uint32)((uint8*)src)[2] << 16);
#endif
	case 4:
		return ((uint32*)src)[0];
	}
	return 0; // ?
}

uint16 Bitwise::floatToHalf(float f)
{
	union { float f; uint32 i; } v;
	v.f = f;
	return floatToHalfI(v.i);
}

float Bitwise::halfToFloat(uint16 y)
{
	union { float f; uint32 i; } v;
	v.i = halfToFloatI(y);
	return v.f;
}

CUTE_NS_END
