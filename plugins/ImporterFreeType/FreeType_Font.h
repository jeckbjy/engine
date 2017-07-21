#pragma once
#include "FreeType_API.h"
#include "Cute/Font.h"

#include "ft2build.h"
//#include FT_FREETYPE_H
//#include FT_TRUETYPE_TABLES_H

CUTE_NS_BEGIN

class CUTE_FREETYPE_API FreeTypeFont : public Font
{
public:
    FreeTypeFont();
    ~FreeTypeFont();
    
    bool init(const void* buffer, unsigned length, float pointSize);
    void term();
    
    const FontGlyph* getGlyph(int ch, size_t pointSize, int flags) const;
    
private:
    FT_Library  m_library;
    FT_Face     m_face;
};

CUTE_NS_END
