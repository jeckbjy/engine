#include "Cute/UUID.h"
#include "Cute/ByteOrder.h"
#include "Cute/Exception.h"
#include <algorithm>
#include <cstring>

CUTE_NS_BEGIN

const UUID UUID::Null;
const UUID UUID::DNS("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
const UUID UUID::URI("6ba7b811-9dad-11d1-80b4-00c04fd430c8");
const UUID UUID::OID("6ba7b812-9dad-11d1-80b4-00c04fd430c8");
const UUID UUID::X500("6ba7b814-9dad-11d1-80b4-00c04fd430c8");

UUID::UUID() :
	m_timeLow(0),
	m_timeMid(0),
	m_timeHiAndVersion(0),
	m_clockSeq(0)
{
	std::memset(m_node, 0, sizeof(m_node));
}

UUID::UUID(const String& uuid)
{
	parse(uuid);
}

UUID::UUID(uint32_t timeLow, uint32_t timeMid, uint32_t timeHiAndVersion, uint16_t clockSeq, uint8_t node[]) :
	m_timeLow(timeLow),
	m_timeMid(timeMid),
	m_timeHiAndVersion(timeHiAndVersion),
	m_clockSeq(clockSeq)
{
	std::memcpy(m_node, node, sizeof(m_node));
}


UUID::UUID(const char* bytes, Version uuidVersion)
{
	uint32_t i32;
	uint16_t i16;
	std::memcpy(&i32, bytes, sizeof(i32));
	m_timeLow = ByteOrder::fromNetwork(i32);
	bytes += sizeof(i32);
	std::memcpy(&i16, bytes, sizeof(i16));
	m_timeMid = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	std::memcpy(&i16, bytes, sizeof(i16));
	m_timeHiAndVersion = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	std::memcpy(&i16, bytes, sizeof(i16));
	m_clockSeq = ByteOrder::fromNetwork(i16);
	bytes += sizeof(i16);
	std::memcpy(m_node, bytes, sizeof(m_node));

	m_timeHiAndVersion &= 0x0FFF;
	m_timeHiAndVersion |= (uuidVersion << 12);
	m_clockSeq &= 0x3FFF;
	m_clockSeq |= 0x8000;
}


UUID::~UUID()
{
}

void UUID::swap(UUID& uuid)
{
	std::swap(m_timeLow, uuid.m_timeLow);
	std::swap(m_timeMid, uuid.m_timeMid);
	std::swap(m_timeHiAndVersion, uuid.m_timeHiAndVersion);
	std::swap(m_clockSeq, uuid.m_clockSeq);
	std::swap_ranges(m_node, m_node + 6, &uuid.m_node[0]);
}


void UUID::parse(const String& uuid)
{
	if (!tryParse(uuid))
		throw SyntaxException(uuid);
}


bool UUID::tryParse(const String& uuid)
{
	if (uuid.size() < 32)
		return false;

	bool haveHyphens = false;
	if (uuid[8] == '-' && uuid[13] == '-' && uuid[18] == '-' && uuid[23] == '-')
	{
		if (uuid.size() >= 36)
			haveHyphens = true;
		else
			return false;
	}

	UUID newUUID;
	String::const_iterator it = uuid.begin();
	newUUID.m_timeLow = 0;
	for (int i = 0; i < 8; ++i)
	{
		int16_t n = nibble(*it++);
		if (n < 0) return false;
		newUUID.m_timeLow = (newUUID.m_timeLow << 4) | n;
	}
	if (haveHyphens) ++it;
	newUUID.m_timeMid = 0;
	for (int i = 0; i < 4; ++i)
	{
		int16_t n = nibble(*it++);
		if (n < 0) return false;
		newUUID.m_timeMid = (newUUID.m_timeMid << 4) | n;
	}
	if (haveHyphens) ++it;
	newUUID.m_timeHiAndVersion = 0;
	for (int i = 0; i < 4; ++i)
	{
		int16_t n = nibble(*it++);
		if (n < 0) return false;
		newUUID.m_timeHiAndVersion = (newUUID.m_timeHiAndVersion << 4) | n;
	}
	if (haveHyphens) ++it;
	newUUID.m_clockSeq = 0;
	for (int i = 0; i < 4; ++i)
	{
		int16_t n = nibble(*it++);
		if (n < 0) return false;
		newUUID.m_clockSeq = (newUUID.m_clockSeq << 4) | n;
	}
	if (haveHyphens) ++it;
	for (int i = 0; i < 6; ++i)
	{
		int16_t n1 = nibble(*it++);
		if (n1 < 0) return false;
		int16_t n2 = nibble(*it++);
		if (n2 < 0) return false;

		newUUID.m_node[i] = (n1 << 4) | n2;
	}
	swap(newUUID);

	return true;
}


String UUID::toString() const
{
	String result;
	result.reserve(36);
	appendHex(result, m_timeLow);
	result += '-';
	appendHex(result, m_timeMid);
	result += '-';
	appendHex(result, m_timeHiAndVersion);
	result += '-';
	appendHex(result, m_clockSeq);
	result += '-';
	for (int i = 0; i < sizeof(m_node); ++i)
		appendHex(result, m_node[i]);
	return result;
}


void UUID::copyFrom(const char* buffer)
{
	uint32_t i32;
	uint16_t i16;
	std::memcpy(&i32, buffer, sizeof(i32));
	m_timeLow = ByteOrder::fromNetwork(i32);
	buffer += sizeof(i32);
	std::memcpy(&i16, buffer, sizeof(i16));
	m_timeMid = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	std::memcpy(&i16, buffer, sizeof(i16));
	m_timeHiAndVersion = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	std::memcpy(&i16, buffer, sizeof(i16));
	m_clockSeq = ByteOrder::fromNetwork(i16);
	buffer += sizeof(i16);
	std::memcpy(m_node, buffer, sizeof(m_node));
}


void UUID::copyTo(char* buffer) const
{
	uint32_t i32 = ByteOrder::toNetwork(m_timeLow);
	std::memcpy(buffer, &i32, sizeof(i32));
	buffer += sizeof(i32);
	uint16_t i16 = ByteOrder::toNetwork(m_timeMid);
	std::memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	i16 = ByteOrder::toNetwork(m_timeHiAndVersion);
	std::memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	i16 = ByteOrder::toNetwork(m_clockSeq);
	std::memcpy(buffer, &i16, sizeof(i16));
	buffer += sizeof(i16);
	std::memcpy(buffer, m_node, sizeof(m_node));
}


int UUID::variant() const
{
	int v = m_clockSeq >> 13;
	if ((v & 6) == 6)
		return v;
	else if (v & 4)
		return 2;
	else
		return 0;
}


int UUID::compare(const UUID& uuid) const
{
	if (m_timeLow != uuid.m_timeLow) return m_timeLow < uuid.m_timeLow ? -1 : 1;
	if (m_timeMid != uuid.m_timeMid) return m_timeMid < uuid.m_timeMid ? -1 : 1;
	if (m_timeHiAndVersion != uuid.m_timeHiAndVersion) return m_timeHiAndVersion < uuid.m_timeHiAndVersion ? -1 : 1;
	if (m_clockSeq != uuid.m_clockSeq) return m_clockSeq < uuid.m_clockSeq ? -1 : 1;
	for (int i = 0; i < sizeof(m_node); ++i)
	{
		if (m_node[i] < uuid.m_node[i])
			return -1;
		else if (m_node[i] > uuid.m_node[i])
			return 1;
	}
	return 0;
}


void UUID::appendHex(String& str, uint8_t n)
{
	static const char* digits = "0123456789abcdef";
	str += digits[(n >> 4) & 0xF];
	str += digits[n & 0xF];
}


void UUID::appendHex(String& str, uint16_t n)
{
	appendHex(str, uint8_t(n >> 8));
	appendHex(str, uint8_t(n & 0xFF));
}


void UUID::appendHex(String& str, uint32_t n)
{
	appendHex(str, uint16_t(n >> 16));
	appendHex(str, uint16_t(n & 0xFFFF));
}


int16_t UUID::nibble(char hex)
{
	if (hex >= 'a' && hex <= 'f')
		return hex - 'a' + 10;
	else if (hex >= 'A' && hex <= 'F')
		return hex - 'A' + 10;
	else if (hex >= '0' && hex <= '9')
		return hex - '0';
	else
		return -1;
}


void UUID::fromNetwork()
{
	m_timeLow = ByteOrder::fromNetwork(m_timeLow);
	m_timeMid = ByteOrder::fromNetwork(m_timeMid);
	m_timeHiAndVersion = ByteOrder::fromNetwork(m_timeHiAndVersion);
	m_clockSeq = ByteOrder::fromNetwork(m_clockSeq);
}


void UUID::toNetwork()
{
	m_timeLow = ByteOrder::toNetwork(m_timeLow);
	m_timeMid = ByteOrder::toNetwork(m_timeMid);
	m_timeHiAndVersion = ByteOrder::toNetwork(m_timeHiAndVersion);
	m_clockSeq = ByteOrder::toNetwork(m_clockSeq);
}

CUTE_NS_END
