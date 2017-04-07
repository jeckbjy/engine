// module Text
#include "Cute/Unicode.h"
#include "Cute/TextIterator.h"
#include "Cute/TextConverter.h"
#include "Cute/UTF8Encoding.h"
#include "Cute/UTF16Encoding.h"
#include "Cute/UTF32Encoding.h"

CUTE_NS_BEGIN

void Unicode::properties(int ch, CharacterProperties& props)
{
	//if (ch > UCP_MAX_CODEPOINT) ch = 0;
	//const ucd_record* ucd = GET_UCD(ch);
	//props.category = static_cast<CharacterCategory>(_pcre_ucp_gentype[ucd->chartype]);
	//props.type = static_cast<CharacterType>(ucd->chartype);
	//props.script = static_cast<Script>(ucd->script);
}


int Unicode::toLower(int ch)
{
	//if (isUpper(ch))
	//	return static_cast<int>(UCD_OTHERCASE(static_cast<unsigned>(ch)));
	//else
		return ch;
}


int Unicode::toUpper(int ch)
{
	//if (isLower(ch))
	//	return static_cast<int>(UCD_OTHERCASE(static_cast<unsigned>(ch)));
	//else
		return ch;
}

void Unicode::convert(const UTF8String& from, UTF16String& to)
{
	to.clear();
	to.reserve(from.size());
	UTF8Encoding encoding;
	TextIterator itor(from, encoding);
	while (itor)
	{
		int cc = *itor++;
		if (cc <= 0xffff)
		{
			to += (UTF16Char)cc;
		}
		else
		{
			cc -= 0x10000;
			to += (UTF16Char)((cc >> 10) & 0x3ff) | 0xd800;
			to += (UTF16Char)(cc & 0x3ff) | 0xdc00;
		}
	}
}

void Unicode::convert(const UTF8String& from, UTF32String& to)
{
	to.clear();
	to.reserve(from.size());
	UTF8Encoding encoding;
	TextIterator itor(from, encoding);
	while (itor)
	{
		int cc = *itor++;
		to += (UTF32Char)cc;
	}
}

void Unicode::convert(const UTF16String& from, UTF8String& to)
{
	to.clear();
	UTF16Encoding encodingIn;
	UTF8Encoding  encodingOut;
	TextConverter converter(encodingIn, encodingOut);
	converter.convert(from.data(), (int)from.length() * sizeof(UTF16Char), to);
}

void Unicode::convert(const UTF32String& from, UTF8String& to)
{
	to.clear();
	UTF32Encoding encodingIn;
	UTF8Encoding  encodingOut;
	TextConverter converter(encodingIn, encodingOut);
	converter.convert(from.data(), (int)from.length() * sizeof(UTF32Char), to);
}

CUTE_NS_END
