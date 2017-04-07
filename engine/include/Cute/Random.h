#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A better random number generator.
/// Random implements a pseudo random number generator
/// (PRNG). The PRNG is a nonlinear additive
/// feedback random number generator using 256 bytes
/// of state information and a period of up to 2^69.
class CUTE_CORE_API Random
{
public:
	enum Type
	{
		RND_STATE_0 = 8,  /// linear congruential
		RND_STATE_32 = 32,  /// x**7 + x**3 + 1
		RND_STATE_64 = 64,  /// x**15 + x + 1
		RND_STATE_128 = 128,  /// x**31 + x**3 + 1
		RND_STATE_256 = 256   /// x**63 + x + 1
	};

	Random(int stateSize = 256);
	~Random();

	void seed(uint32_t randSeed);
	void seed();

	uint32_t next();
	uint32_t next(uint32_t n);

	char	nextChar();
	bool	nextBool();
	float	nextFloat();
	double	nextDouble();

protected:
	void initState(uint32_t seed, char* arg_state, int32_t n);
	static uint32_t goodRand(int32_t x);

private:
	enum
	{
		MAX_TYPES = 5,
		NSHUFF = 50
	};

	uint32_t*	m_fptr;
	uint32_t*	m_rptr;
	uint32_t*	m_state;
	uint32_t*	m_endPtr;
	int			m_randType;
	int			m_randDeg;
	int			m_randSep;
	char*		m_buffer;
};

//
// inlines
//
inline uint32_t Random::next(uint32_t n)
{
	return next() % n;
}

inline char Random::nextChar()
{
	return char((next() >> 3) & 0xFF);
}

inline bool Random::nextBool()
{
	return (next() & 0x1000) != 0;
}

inline float Random::nextFloat()
{
	return float(next()) / 0x7FFFFFFF;
}

inline double Random::nextDouble()
{
	return double(next()) / 0x7FFFFFFF;
}

CUTE_NS_END
