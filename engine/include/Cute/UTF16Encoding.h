#pragma once
#include "Cute/TextEncoding.h"

CUTE_NS_BEGIN

/// UTF-16 text encoding, as defined in RFC 2781.
///
/// When converting from UTF-16 to Unicode, surrogates are
/// reported as they are - in other words, surrogate pairs
/// are not combined into one Unicode character. 
/// When converting from Unicode to UTF-16, however, characters
/// outside the 16-bit range are converted into a low and
/// high surrogate.
class CUTE_CORE_API UTF16Encoding : public TextEncoding
{
public:
	enum ByteOrderType
	{
		BIG_ENDIAN_BYTE_ORDER,
		LITTLE_ENDIAN_BYTE_ORDER,
		NATIVE_BYTE_ORDER
	};

	UTF16Encoding(ByteOrderType byteOrder = NATIVE_BYTE_ORDER);
	UTF16Encoding(int byteOrderMark);
	~UTF16Encoding();

	ByteOrderType getByteOrder() const;
	void setByteOrder(ByteOrderType byteOrder);
	void setByteOrder(int byteOrderMark);

	const char* canonicalName() const;
	bool isA(const std::string& encodingName) const;
	const CharacterMap& characterMap() const;
	int convert(const unsigned char* bytes) const;
	int convert(int ch, unsigned char* bytes, int length) const;
	int queryConvert(const unsigned char* bytes, int length) const;
	int sequenceLength(const unsigned char* bytes, int length) const;

private:
	bool m_flipBytes;
	static const char* m_names[];
	static const CharacterMap m_charMap;
};

CUTE_NS_END