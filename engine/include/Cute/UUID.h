#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

/// A UUID is an identifier that is unique across both space and time,
/// with respect to the space of all UUIDs. Since a UUID is a fixed
/// size and contains a time field, it is possible for values to
/// rollover (around A.D. 3400, depending on the specific algorithm
/// used). A UUID can be used for multiple purposes, from tagging
/// objects with an extremely short lifetime, to reliably identifying
/// very persistent objects across a network.
///
/// This class implements a Universal Unique Identifier,
/// as specified in Appendix A of the DCE 1.1 Remote Procedure
/// Call Specification (http://www.opengroup.org/onlinepubs/9629399/),
/// RFC 2518 (WebDAV), section 6.4.1 and the UUIDs and GUIDs internet
/// draft by Leach/Salz from February, 1998 
/// (http://www.ics.uci.edu/~ejw/authoring/uuid-guid/draft-leach-uuids-guids-01.txt)
/// and also http://tools.ietf.org/html/draft-mealling-uuid-urn-05
class CUTE_CORE_API UUID
{
public:
	enum Version
	{
		UUID_TIME_BASED = 0x01,
		UUID_DCE_UID = 0x02,
		UUID_NAME_BASED = 0x03,
		UUID_RANDOM = 0x04,
		UUID_NAME_BASED_SHA1 = 0x05

	};

	static const UUID Null;
	static const UUID DNS;
	static const UUID URI;
	static const UUID OID;
	static const UUID X500;

	UUID();
	UUID(const String& uuid);
	~UUID();

	void swap(UUID& uuid);
	void parse(const String& uuid);
	bool tryParse(const String& uuid);
	String toString() const;

	/// Copies the UUID (16 bytes) from a buffer or byte array.
	/// The UUID fields are expected to be
	/// stored in network byte order.
	/// The buffer need not be aligned.
	void copyFrom(const char* buffer);

	/// Copies the UUID to the buffer. The fields
	/// are in network byte order.
	/// The buffer need not be aligned.
	/// There must have room for at least 16 bytes.
	void copyTo(char* buffer) const;

	Version version() const;

	/// Returns the variant number of the UUID:
	///   - 0 reserved for NCS backward compatibility
	///   - 2 the Leach-Salz variant (used by this class)
	///   - 6 reserved, Microsoft Corporation backward compatibility
	///   - 7 reserved for future definition
	int variant() const;

	bool isNull() const;

	bool operator == (const UUID& uuid) const;
	bool operator != (const UUID& uuid) const;
	bool operator <  (const UUID& uuid) const;
	bool operator <= (const UUID& uuid) const;
	bool operator >  (const UUID& uuid) const;
	bool operator >= (const UUID& uuid) const;

protected:
	UUID(uint32_t timeLow, uint32_t timeMid, uint32_t timeHiAndVersion, uint16_t clockSeq, uint8_t node[]);
	UUID(const char* bytes, Version version);
	int compare(const UUID& uuid) const;
	void fromNetwork();
	void toNetwork();

	static void appendHex(String& str, uint8_t n);
	static void appendHex(String& str, uint16_t n);
	static void appendHex(String& str, uint32_t n);
	static int16_t nibble(char hex);

private:
	uint32_t m_timeLow;
	uint16_t m_timeMid;
	uint16_t m_timeHiAndVersion;
	uint16_t m_clockSeq;
	uint8_t  m_node[6];

	friend class UUIDGenerator;
};

//
// inlines
//
inline bool UUID::operator == (const UUID& uuid) const
{
	return compare(uuid) == 0;
}

inline bool UUID::operator != (const UUID& uuid) const
{
	return compare(uuid) != 0;
}

inline bool UUID::operator < (const UUID& uuid) const
{
	return compare(uuid) < 0;
}

inline bool UUID::operator <= (const UUID& uuid) const
{
	return compare(uuid) <= 0;
}

inline bool UUID::operator > (const UUID& uuid) const
{
	return compare(uuid) > 0;
}

inline bool UUID::operator >= (const UUID& uuid) const
{
	return compare(uuid) >= 0;
}

inline UUID::Version UUID::version() const
{
	return Version(m_timeHiAndVersion >> 12);
}

inline bool UUID::isNull() const
{
	return compare(Null) == 0;
}

inline void swap(UUID& u1, UUID& u2)
{
	u1.swap(u2);
}

CUTE_NS_END
