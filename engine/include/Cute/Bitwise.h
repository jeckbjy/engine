#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API Bitwise
{
public:
	static uint popcount(uint8 data);
	static uint popcount(uint16 data);
	static uint popcount(uint32 data);
	static uint popcount(uint64 data);

	/** Returns the most significant bit set in a value. */
	static uint32 mostSignificantBitSet(uint value);

	/** Returns the power-of-two number greater or equal to the provided value. */
	static uint32 nextPow2(uint32 n);

	/** Returns the power-of-two number closest to the provided value. */
	static uint32 closestPow2(uint32 n);

	/**
	* Convert N bit colour channel value to P bits. It fills P bits with the bit pattern repeated.
	* (this is /((1<<n)-1) in fixed point).
	*/
	static uint fixedToFixed(uint32 value, unsigned int n, unsigned int p);

	/**
	* Convert floating point color channel value between 0.0 and 1.0 (otherwise clamped) to integer of a certain
	* number of bits. Works for any value of bits between 0 and 31.
	*/
	static uint floatToFixed(const float value, const unsigned int bits);

	/** Fixed point to float. */
	static float fixedToFloat(unsigned value, unsigned int bits);

	/** Write a n*8 bits integer value to memory in native endian. */
	static void intWrite(void *dest, const int n, const unsigned int value);

	/** Read a n*8 bits integer value to memory in native endian. */
	static uint intRead(const void *src, int n);

	/** Convert a float32 to a float16 (NV_half_float). */
	static uint16 floatToHalf(float f);

	/** Convert a float16 (NV_half_float) to a float32. */
	static float halfToFloat(uint16 y);

	/** Determines whether the number is power-of-two or not. */
	template<typename T>
	static bool isPow2(T n);

	/** Returns the number of bits a pattern must be shifted right by to remove right-hand zeros. */
	template<typename T>
	static uint getBitShift(T mask);

	/** Takes a value with a given src bit mask, and produces another value with a desired bit mask. */
	template<typename SrcT, typename DestT>
	static DestT convertBitPattern(SrcT srcValue, SrcT srcBitMask, DestT destBitMask);
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
template<typename T>
bool Bitwise::isPow2(T n)
{
	return (n & (n - 1)) == 0;
}

template<typename T>
uint Bitwise::getBitShift(T mask)
{
	if (mask == 0)
		return 0;

	uint result = 0;
	while ((mask & 1) == 0) {
		++result;
		mask >>= 1;
	}
	return result;
}

/** Takes a value with a given src bit mask, and produces another value with a desired bit mask. */
template<typename SrcT, typename DestT>
DestT Bitwise::convertBitPattern(SrcT srcValue, SrcT srcBitMask, DestT destBitMask)
{
	// Mask off irrelevant source value bits (if any)
	srcValue = srcValue & srcBitMask;

	// Shift source down to bottom of DWORD
	const unsigned int srcBitShift = getBitShift(srcBitMask);
	srcValue >>= srcBitShift;

	// Get max value possible in source from srcMask
	const SrcT srcMax = srcBitMask >> srcBitShift;

	// Get max available in dest
	const unsigned int destBitShift = getBitShift(destBitMask);
	const DestT destMax = destBitMask >> destBitShift;

	// Scale source value into destination, and shift back
	DestT destValue = (srcValue * destMax) / srcMax;
	return (destValue << destBitShift);
}

CUTE_NS_END
