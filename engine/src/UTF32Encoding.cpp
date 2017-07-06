// module Text
#include "Cute/UTF32Encoding.h"
#include "Cute/ByteOrder.h"
#include "Cute/String.h"

CUTE_NS_BEGIN

const char* UTF32Encoding::m_names[] =
{
	"UTF-32",
	"UTF32",
	NULL
};

const TextEncoding::CharacterMap UTF32Encoding::m_charMap =
{
	/* 00 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 10 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 20 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 30 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 40 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 50 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 60 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 70 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 80 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* 90 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* a0 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* b0 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* c0 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* d0 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* e0 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	/* f0 */	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
};

UTF32Encoding::UTF32Encoding(ByteOrderType byteOrder)
{
	setByteOrder(byteOrder);
}

/// Creates and initializes the encoding for the byte-order
/// indicated by the given byte-order mark, which is the Unicode
/// character 0xFEFF.
UTF32Encoding::UTF32Encoding(int byteOrderMark)
{
	setByteOrder(byteOrderMark);
}

UTF32Encoding::~UTF32Encoding()
{
}

UTF32Encoding::ByteOrderType UTF32Encoding::getByteOrder() const
{
#if (CUTE_BYTE_ORDER == CUTE_BIG_ENDIAN)
	return m_flipBytes ? LITTLE_ENDIAN_BYTE_ORDER : BIG_ENDIAN_BYTE_ORDER;
#else
	return m_flipBytes ? BIG_ENDIAN_BYTE_ORDER : LITTLE_ENDIAN_BYTE_ORDER;
#endif
}

void UTF32Encoding::setByteOrder(ByteOrderType byteOrder)
{
#if (CUTE_BYTE_ORDER == CUTE_BIG_ENDIAN)
	m_flipBytes = byteOrder == LITTLE_ENDIAN_BYTE_ORDER;
#else
	m_flipBytes = byteOrder == BIG_ENDIAN_BYTE_ORDER;;
#endif
}

void UTF32Encoding::setByteOrder(int byteOrderMark)
{
	m_flipBytes = byteOrderMark != 0xFEFF;
}

const char* UTF32Encoding::canonicalName() const
{
	return m_names[0];
}

bool UTF32Encoding::isA(const String& encodingName) const
{
	for (const char** name = m_names; *name; ++name)
	{
		if (encodingName.iequals(*name))
			return true;
	}
	return false;
}

const TextEncoding::CharacterMap& UTF32Encoding::characterMap() const
{
	return m_charMap;
}

int UTF32Encoding::convert(const unsigned char* bytes) const
{
	uint32_t uc;
	unsigned char* p = (unsigned char*)&uc;
	*p++ = *bytes++;
	*p++ = *bytes++;
	*p++ = *bytes++;
	*p++ = *bytes++;

	if (m_flipBytes)
	{
		ByteOrder::flip(uc);
	}

	return uc;
}

int UTF32Encoding::convert(int ch, unsigned char* bytes, int length) const
{
	if (bytes && length >= 4)
	{
		uint32_t ch1 = m_flipBytes ? ByteOrder::flip((uint32_t)ch) : (uint32_t)ch;
		unsigned char* p = (unsigned char*)&ch1;
		*bytes++ = *p++;
		*bytes++ = *p++;
		*bytes++ = *p++;
		*bytes++ = *p++;
	}
	return 4;
}

int UTF32Encoding::queryConvert(const unsigned char* bytes, int length) const
{
	int ret = -4;

	if (length >= 4)
	{
		uint32_t uc;
		unsigned char* p = (unsigned char*)&uc;
		*p++ = *bytes++;
		*p++ = *bytes++;
		*p++ = *bytes++;
		*p++ = *bytes++;
		if (m_flipBytes)
			ByteOrder::flip(uc);
		return uc;
	}

	return ret;
}

int UTF32Encoding::sequenceLength(const unsigned char* bytes, int length) const
{
	return 4;
}

CUTE_NS_END
