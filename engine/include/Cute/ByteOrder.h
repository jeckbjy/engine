#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class CUTE_CORE_API ByteOrder
{
public:
	static int8_t	flip(int8_t  value);
	static uint8_t	flip(uint8_t value);
	static int16_t	flip(int16_t value);
	static uint16_t	flip(uint16_t value);
	static int32_t	flip(int32_t value);
	static uint32_t flip(uint32_t value);
	static int64_t	flip(int64_t value);
	static uint64_t	flip(uint64_t value);
	static float	flip(float value);
	static double	flip(double value);

	static int8_t	toLittleEndian(int8_t  value);
	static uint8_t	toLittleEndian(uint8_t value);
	static int16_t	toLittleEndian(int16_t value);
	static uint16_t	toLittleEndian(uint16_t value);
	static int32_t	toLittleEndian(int32_t value);
	static uint32_t toLittleEndian(uint32_t value);
	static int64_t	toLittleEndian(int64_t value);
	static uint64_t	toLittleEndian(uint64_t value);
	static float	toLittleEndian(float value);
	static double	toLittleEndian(double value);

	static int8_t	fromLittleEndian(int8_t  value);
	static uint8_t	fromLittleEndian(uint8_t value);
	static int16_t	fromLittleEndian(int16_t value);
	static uint16_t	fromLittleEndian(uint16_t value);
	static int32_t	fromLittleEndian(int32_t value);
	static uint32_t fromLittleEndian(uint32_t value);
	static int64_t	fromLittleEndian(int64_t value);
	static uint64_t	fromLittleEndian(uint64_t value);
	static float	fromLittleEndian(float value);
	static double	fromLittleEndian(double value);

	static int8_t	toBigEndian(int8_t  value);
	static uint8_t	toBigEndian(uint8_t value);
	static int16_t	toBigEndian(int16_t value);
	static uint16_t	toBigEndian(uint16_t value);
	static int32_t	toBigEndian(int32_t value);
	static uint32_t toBigEndian(uint32_t value);
	static int64_t	toBigEndian(int64_t value);
	static uint64_t	toBigEndian(uint64_t value);
	static float	toBigEndian(float value);
	static double	toBigEndian(double value);

	static int8_t	fromBigEndian(int8_t  value);
	static uint8_t	fromBigEndian(uint8_t value);
	static int16_t	fromBigEndian(int16_t value);
	static uint16_t	fromBigEndian(uint16_t value);
	static int32_t	fromBigEndian(int32_t value);
	static uint32_t fromBigEndian(uint32_t value);
	static int64_t	fromBigEndian(int64_t value);
	static uint64_t	fromBigEndian(uint64_t value);
	static float	fromBigEndian(float value);
	static double	fromBigEndian(double value);

	static int8_t	toNetwork(int8_t  value);
	static uint8_t	toNetwork(uint8_t value);
	static int16_t	toNetwork(int16_t value);
	static uint16_t	toNetwork(uint16_t value);
	static int32_t	toNetwork(int32_t value);
	static uint32_t toNetwork(uint32_t value);
	static int64_t	toNetwork(int64_t value);
	static uint64_t	toNetwork(uint64_t value);
	static float	toNetwork(float value);
	static double	toNetwork(double value);

	static int8_t	fromNetwork(int8_t  value);
	static uint8_t	fromNetwork(uint8_t value);
	static int16_t	fromNetwork(int16_t value);
	static uint16_t	fromNetwork(uint16_t value);
	static int32_t	fromNetwork(int32_t value);
	static uint32_t fromNetwork(uint32_t value);
	static int64_t	fromNetwork(int64_t value);
	static uint64_t	fromNetwork(uint64_t value);
	static float	fromNetwork(float value);
	static double	fromNetwork(double value);
};

#if !defined(CUTE_NO_BYTESWAP_BUILTINS)
#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#define byteswap16(x) _byteswap_ushort(x)
#define byteswap32(x) _byteswap_ulong(x)
#define byteswap64(x) _byteswap_uint64(x)
#elif defined(__clang__) && __has_builtin(__builtin_bswap32)
#define byteswap16(x) __builtin_bswap16(x)
#define byteswap32(x) __builtin_bswap32(x)
#define byteswap64(x) __builtin_bswap64(x)
#elif defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
#define byteswap16(x) __builtin_bswap16(x)
#define byteswap32(x) __builtin_bswap32(x)
#define byteswap64(x) __builtin_bswap64(x)
#endif
#endif // endif

#if !defined(byteswap16)
uint16_t byteswap16(uint16_t value)
{
	return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
}
uint32_t byteswap32(uint32_t value)
{
	return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00)
		| ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
}
uint64_t byteswap64(uint64_t value)
{
	uint32_t hi = uint32_t(value >> 32);
	uint32_t lo = uint32_t(value & 0xFFFFFFFF);
	return uint64_t(byteswap32(hi)) | (uint64_t(byteswap32(lo)) << 32);
}
#endif

// inlines
inline int8_t ByteOrder::flip(int8_t value)
{
	return value;
}

inline uint8_t ByteOrder::flip(uint8_t value)
{
	return value;
}

inline int16_t ByteOrder::flip(int16_t value)
{
	return byteswap16((uint16_t)value);
}

inline uint16_t ByteOrder::flip(uint16_t value)
{
	return byteswap16(value);
}

inline int32_t ByteOrder::flip(int32_t value)
{
	return byteswap32((uint32_t)value);
}

inline uint32_t ByteOrder::flip(uint32_t value)
{
	return byteswap32(value);
}

inline int64_t ByteOrder::flip(int64_t value)
{
	return byteswap64((uint64_t)value);
}

inline uint64_t ByteOrder::flip(uint64_t value)
{
	return byteswap64(value);
}

inline float ByteOrder::flip(float value)
{
	union
	{
		uint32_t i;
		float	 f;
	} v;
	v.f = value;
	return (float)byteswap32(v.i);
}

inline double ByteOrder::flip(double value)
{
	union
	{
		uint64_t i;
		double	 f;
	} v;
	v.f = value;
	return (double)byteswap64(v.i);
}

//
// some macro trickery to automate the method implementation
//
#define CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, type)\
	inline type ByteOrder::op(type value)		\
	{											\
		return value;							\
	}

#define CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, type)\
	inline type ByteOrder::op(type value)		\
	{											\
		return flip(value);						\
	}

#define CUTE_IMPLEMENT_BYTEORDER_NOOP(op)			\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, int8_t)	\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, uint8_t)	\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, int16_t)	\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, uint16_t)\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, int32_t)	\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, uint32_t)\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, int64_t)	\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, uint64_t)\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, float)	\
		CUTE_IMPLEMENT_BYTEORDER_NOOP_(op, double)

#define CUTE_IMPLEMENT_BYTEORDER_FLIP(op)			\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, int8_t)	\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, uint8_t)	\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, int16_t)	\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, uint16_t)\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, int32_t)	\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, uint32_t)\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, int64_t)	\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, uint64_t)\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, float)	\
		CUTE_IMPLEMENT_BYTEORDER_FLIP_(op, double)

#if (CUTE_BYTE_ORDER == CUTE_BIG_ENDIAN)
#define CUTE_IMPLEMENT_BYTEORDER_BIG CUTE_IMPLEMENT_BYTEORDER_NOOP
#define CUTE_IMPLEMENT_BYTEORDER_LIT CUTE_IMPLEMENT_BYTEORDER_FLIP
#else
#define CUTE_IMPLEMENT_BYTEORDER_BIG CUTE_IMPLEMENT_BYTEORDER_FLIP
#define CUTE_IMPLEMENT_BYTEORDER_LIT CUTE_IMPLEMENT_BYTEORDER_NOOP
#endif


CUTE_IMPLEMENT_BYTEORDER_LIT(toLittleEndian)
CUTE_IMPLEMENT_BYTEORDER_LIT(fromLittleEndian)
CUTE_IMPLEMENT_BYTEORDER_BIG(toBigEndian)
CUTE_IMPLEMENT_BYTEORDER_BIG(fromBigEndian)
CUTE_IMPLEMENT_BYTEORDER_BIG(toNetwork)
CUTE_IMPLEMENT_BYTEORDER_BIG(fromNetwork)

CUTE_NS_END
