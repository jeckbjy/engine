#pragma once
#include "Cute/TextEncoding.h"

CUTE_NS_BEGIN

/// UTF-32 text encoding, as defined in RFC 2781.
///
/// When converting from UTF-32 to Unicode, surrogates are
/// reported as they are - in other words, surrogate pairs
/// are not combined into one Unicode character. 
class CUTE_CORE_API UTF32Encoding : public TextEncoding
{
public:
	UTF32Encoding(ByteOrderType byteOrder = NATIVE_BYTE_ORDER);
	UTF32Encoding(int byteOrderMark);
	~UTF32Encoding();

	ByteOrderType getByteOrder() const;
	void setByteOrder(ByteOrderType byteOrder);
	void setByteOrder(int byteOrderMark);

	const char* canonicalName() const;
	bool isA(const String& encodingName) const;
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
