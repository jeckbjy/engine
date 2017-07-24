#pragma once
#include "FreeType_API.h"
#include "Cute/Font.h"

#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/tttables.h"
//#include FT_FREETYPE_H
//#include FT_TRUETYPE_TABLES_H

CUTE_NS_BEGIN

class CUTE_FREETYPE_API FreeType_FontFace : public FontFace
{
public:
    FreeType_FontFace();
    ~FreeType_FontFace();
    
    bool init(const unsigned char* buffer, unsigned length, float pointSize);
    void term();
    
    bool load(const String& path, float pointSize);
    bool prepare(const String& text);
    
private:
    FT_Face     m_face;
};

CUTE_NS_END
