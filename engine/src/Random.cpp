#include "Cute/Random.h"
#include <ctime>

#ifdef CUTE_OS_FAMILY_WINDOWS
#	include <wincrypt.h>
#elif defined(CUTE_OS_FAMILY_UNIX)
#	include <fcntl.h>
#	include <unistd.h>
#endif

CUTE_NS_BEGIN

/*
* random.c:
*
* An improved random number generation package.  In addition to the standard
* rand()/srand() like interface, this package also has a special state info
* interface.  The initstate() routine is called with a seed, an array of
* bytes, and a count of how many bytes are being passed in; this array is
* then initialized to contain information for random number generation with
* that much state information.  Good sizes for the amount of state
* information are 32, 64, 128, and 256 bytes.  The state can be switched by
* calling the setstate() routine with the same array as was initiallized
* with initstate().  By default, the package runs with 128 bytes of state
* information and generates far better random numbers than a linear
* congruential generator.  If the amount of state information is less than
* 32 bytes, a simple linear congruential R.N.G. is used.
*
* Internally, the state information is treated as an array of uint32_t's; the
* zeroeth element of the array is the type of R.N.G. being used (small
* integer); the remainder of the array is the state information for the
* R.N.G.  Thus, 32 bytes of state information will give 7 ints worth of
* state information, which will allow a degree seven polynomial.  (Note:
* the zeroeth word of state information also has some other information
* stored in it -- see setstate() for details).
*
* The random number generation technique is a linear feedback shift register
* approach, employing trinomials (since there are fewer terms to sum up that
* way).  In this approach, the least significant bit of all the numbers in
* the state table will act as a linear feedback shift register, and will
* have period 2^deg - 1 (where deg is the degree of the polynomial being
* used, assuming that the polynomial is irreducible and primitive).  The
* higher order bits will have longer periods, since their values are also
* influenced by pseudo-random carries out of the lower bits.  The total
* period of the generator is approximately deg*(2**deg - 1); thus doubling
* the amount of state information has a vast influence on the period of the
* generator.  Note: the deg*(2**deg - 1) is an approximation only good for
* large deg, when the period of the shift is the dominant factor.
* With deg equal to seven, the period is actually much longer than the
* 7*(2**7 - 1) predicted by this formula.
*
* Modified 28 December 1994 by Jacob S. Rosenberg.
* The following changes have been made:
* All references to the type u_int have been changed to unsigned long.
* All references to type int have been changed to type long.  Other
* cleanups have been made as well.  A warning for both initstate and
* setstate has been inserted to the effect that on Sparc platforms
* the 'arg_state' variable must be forced to begin on word boundaries.
* This can be easily done by casting a long integer array to char *.
* The overall logic has been left STRICTLY alone.  This software was
* tested on both a VAX and Sun SpacsStation with exactly the same
* results.  The new version and the original give IDENTICAL results.
* The new version is somewhat faster than the original.  As the
* documentation says:  "By default, the package runs with 128 bytes of
* state information and generates far better random numbers than a linear
* congruential generator.  If the amount of state information is less than
* 32 bytes, a simple linear congruential R.N.G. is used."  For a buffer of
* 128 bytes, this new version runs about 19 percent faster and for a 16
* byte buffer it is about 5 percent faster.
*/


/*
* For each of the currently supported random number generators, we have a
* break value on the amount of state information (you need at least this
* many bytes of state info to support this random number generator), a degree
* for the polynomial (actually a trinomial) that the R.N.G. is based on, and
* the separation between the two lower order coefficients of the trinomial.
*/
#define	TYPE_0		0		/* linear congruential */
#define	BREAK_0		8
#define	DEG_0		0
#define	SEP_0		0

#define	TYPE_1		1		/* x**7 + x**3 + 1 */
#define	BREAK_1		32
#define	DEG_1		7
#define	SEP_1		3

#define	TYPE_2		2		/* x**15 + x + 1 */
#define	BREAK_2		64
#define	DEG_2		15
#define	SEP_2		1

#define	TYPE_3		3		/* x**31 + x**3 + 1 */
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

#define	TYPE_4		4		/* x**63 + x + 1 */
#define	BREAK_4		256
#define	DEG_4		63
#define	SEP_4		1

Random::Random(int stateSize)
{
	cute_assert(BREAK_0 <= stateSize && stateSize <= BREAK_4);

	m_buffer = new char[stateSize];

	initState((uint32_t)std::time(NULL), m_buffer, stateSize);
}

Random::~Random()
{
	delete[] m_buffer;
}

/*
* Compute x = (7^5 * x) mod (2^31 - 1)
* wihout overflowing 31 bits:
*      (2^31 - 1) = 127773 * (7^5) + 2836
* From "Random number generators: good ones are hard to find",
* Park and Miller, Communications of the ACM, vol. 31, no. 10,
* October 1988, p. 1195.
*/
inline uint32_t Random::goodRand(int32_t x)
{
	int32_t hi, lo;

	if (x == 0) x = 123459876;
	hi = x / 127773;
	lo = x % 127773;
	x = 16807 * lo - 2836 * hi;
	if (x < 0) x += 0x7FFFFFFF;

	return x;
}

/*
* Initialize the random number generator based on the given seed.  If the
* type is the trivial no-state-information type, just remember the seed.
* Otherwise, initializes state[] based on the given "seed" via a linear
* congruential generator.  Then, the pointers are set to known locations
* that are exactly rand_sep places apart.  Lastly, it cycles the state
* information a given number of times to get rid of any initial dependencies
* introduced by the L.C.R.N.G.  Note that the initialization of randtbl[]
* for default usage relies on values produced by this routine.
*/
void Random::seed(uint32_t x)
{
	int i, lim;

	m_state[0] = x;
	if (m_randType == TYPE_0)
		lim = NSHUFF;
	else
	{
		for (i = 1; i < m_randDeg; i++)
			m_state[i] = goodRand(m_state[i - 1]);
		m_fptr = &m_state[m_randSep];
		m_rptr = &m_state[0];
		lim = 10 * m_randDeg;
	}
	for (i = 0; i < lim; i++)
		next();
}

/*
* Many programs choose the seed value in a totally predictable manner.
* This often causes problems.  We seed the generator using the much more
* secure random(4) interface.  Note that this particular seeding
* procedure can generate states which are impossible to reproduce by
* calling srandom() with any value, since the succeeding terms in the
* state buffer are no longer derived from the LC algorithm applied to
* a fixed seed.
*/
void Random::seed()
{
	size_t len;

	if (m_randType == TYPE_0)
		len = sizeof(m_state[0]);
	else
		len = m_randDeg * sizeof(m_state[0]);

	int n = 0;

	char* buffer = (char*)m_state;
	size_t length = len;

#if defined(CUTE_OS_FAMILY_WINDOWS)
	HCRYPTPROV hProvider = 0;
	CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	CryptGenRandom(hProvider, (DWORD)length, (BYTE*)buffer);
	CryptReleaseContext(hProvider, 0);
#else
	int n = 0;
#if defined(CUTE_OS_FAMILY_UNIX)
	int fd = open("/dev/urandom", O_RDONLY, 0);
	if (fd >= 0)
	{
		n = read(fd, buffer, length);
		close(fd);
	}
#endif
	if (n <= 0)
	{
		// x is here as a source of randomness, so it does not make
		// much sense to protect it with a Mutex.

		/*static uint32_t x = 0;
		Random rnd1(256);
		Random rnd2(64);
		x += rnd1.next();

		n = 0;
		SHA1Engine engine;
		uint32_t t = (uint32_t)std::time(NULL);
		engine.update(&t, sizeof(t));
		void* p = this;
		engine.update(&p, sizeof(p));
		engine.update(buffer, length);
		uint32_t junk[32];
		engine.update(junk, sizeof(junk));
		while (n < length)
		{
			for (int i = 0; i < 100; ++i)
			{
				uint32_t r = rnd2.next();
				engine.update(&r, sizeof(r));
				engine.update(&x, sizeof(x));
				x += rnd1.next();
			}
			DigestEngine::Digest d = engine.digest();
			for (DigestEngine::Digest::const_iterator it = d.begin(); it != d.end() && n < length; ++it, ++n)
			{
				engine.update(*it);
				*buffer++ = *it++;
			}
		}*/
	}
#endif
}

/*
* Initialize the state information in the given array of n bytes for future
* random number generation.  Based on the number of bytes we are given, and
* the break values for the different R.N.G.'s, we choose the best (largest)
* one we can and set things up for it.  srandom() is then called to
* initialize the state information.
*
* Note that on return from srandom(), we set state[-1] to be the type
* multiplexed with the current value of the rear pointer; this is so
* successive calls to initstate() won't lose this information and will be
* able to restart with setstate().
*
* Note: the first thing we do is save the current state, if any, just like
* setstate() so that it doesn't matter when initstate is called.
*
* Returns a pointer to the old state.
*
* Note: The Sparc platform requires that arg_state begin on an int
* word boundary; otherwise a bus error will occur. Even so, lint will
* complain about mis-alignment, but you should disregard these messages.
*/
void Random::initState(uint32_t s, char* argState, int32_t n)
{
	uint32_t* intArgState = (uint32_t*)argState;

	if (n < BREAK_0)
	{
		cute_bugcheck_msg("not enough state");
		return;
	}
	if (n < BREAK_1)
	{
		m_randType = TYPE_0;
		m_randDeg = DEG_0;
		m_randSep = SEP_0;
	}
	else if (n < BREAK_2)
	{
		m_randType = TYPE_1;
		m_randDeg = DEG_1;
		m_randSep = SEP_1;
	}
	else if (n < BREAK_3)
	{
		m_randType = TYPE_2;
		m_randDeg = DEG_2;
		m_randSep = SEP_2;
	}
	else if (n < BREAK_4)
	{
		m_randType = TYPE_3;
		m_randDeg = DEG_3;
		m_randSep = SEP_3;
	}
	else
	{
		m_randType = TYPE_4;
		m_randDeg = DEG_4;
		m_randSep = SEP_4;
	}
	m_state = intArgState + 1; /* first location */
	m_endPtr = &m_state[m_randDeg];	/* must set end_ptr before seed */
	seed(s);
	if (m_randType == TYPE_0)
		intArgState[0] = m_randType;
	else
		intArgState[0] = MAX_TYPES * (int)(m_rptr - m_state) + m_randType;
}

/*
* Next:
*
* If we are using the trivial TYPE_0 R.N.G., just do the old linear
* congruential bit.  Otherwise, we do our fancy trinomial stuff, which is
* the same in all the other cases due to all the global variables that have
* been set up.  The basic operation is to add the number at the rear pointer
* into the one at the front pointer.  Then both pointers are advanced to
* the next location cyclically in the table.  The value returned is the sum
* generated, reduced to 31 bits by throwing away the "least random" low bit.
*
* Note: the code takes advantage of the fact that both the front and
* rear pointers can't wrap on the same call by not testing the rear
* pointer if the front one has wrapped.
*
* Returns a 31-bit random number.
*/
uint32_t Random::next()
{
	uint32_t i;
	uint32_t *f, *r;

	if (m_randType == TYPE_0)
	{
		i = m_state[0];
		m_state[0] = i = goodRand(i) & 0x7FFFFFFF;
	}
	else
	{
		/*
		* Use local variables rather than static variables for speed.
		*/
		f = m_fptr; r = m_rptr;
		*f += *r;
		i = (*f >> 1) & 0x7FFFFFFF;	/* chucking least random bit */
		if (++f >= m_endPtr) {
			f = m_state;
			++r;
		}
		else if (++r >= m_endPtr) {
			r = m_state;
		}

		m_fptr = f; m_rptr = r;
	}
	return i;
}

CUTE_NS_END