//! Text
#include "Cute/Strings.h"
#include "Cute/TextIterator.h"
#include "Cute/TextConverter.h"
#include "Cute/UTF8Encoding.h"
#include "Cute/UTF16Encoding.h"
#include "Cute/UTF32Encoding.h"

CUTE_NS_BEGIN

void Strings::toUTF8(const UTF16String& from, UTF8String& to)
{
    to.clear();
    UTF16Encoding encodingIn;
    UTF8Encoding  encodingOut;
    TextConverter converter(encodingIn, encodingOut);
    converter.convert(from.data(), (int)from.length() * sizeof(UTF16Char), to);
}

void Strings::toUTF8(const UTF32String& from, UTF8String& to)
{
    to.clear();
    UTF32Encoding encodingIn;
    UTF8Encoding  encodingOut;
    TextConverter converter(encodingIn, encodingOut);
    converter.convert(from.data(), (int)from.length() * sizeof(UTF32Char), to);
}

void Strings::toUTF16(const UTF8String& from, UTF16String& to)
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

void Strings::toUTF32(const UTF8String& from, UTF32String& to)
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

CUTE_NS_END
