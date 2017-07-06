#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// This class calculates CRC-32 or Adler-32 checksums for arbitrary data.
/// 
/// A cyclic redundancy check (CRC) is a type of hash function, which is used to produce a 
/// small, fixed-size checksum of a larger block of data, such as a packet of network 
/// traffic or a computer file. CRC-32 is one of the most commonly used CRC algorithms.
///
/// Adler-32 is a checksum algorithm which was invented by Mark Adler. 
/// It is almost as reliable as a 32-bit cyclic redundancy check for protecting against 
/// accidental modification of data, such as distortions occurring during a transmission, 
/// but is significantly faster to calculate in software.
class CUTE_CORE_API Checksum
{
public:
	enum Type
	{
		TYPE_ADLER32 = 0,
		TYPE_CRC32
	};

	Checksum();
	Checksum(Type t);
	~Checksum();

	void update(const char* data, unsigned length);
	void update(const String& data);
	void update(char data);

	uint32 checksum() const;
	Type type() const;

private:
	Type m_type;
	uint32 m_value;
};

inline void Checksum::update(const String& data)
{
	update(data.c_str(), static_cast<unsigned int>(data.size()));
}


inline void Checksum::update(char c)
{
	update(&c, 1);
}


inline uint32_t Checksum::checksum() const
{
	return m_value;
}

inline Checksum::Type Checksum::type() const
{
	return m_type;
}

CUTE_NS_END
