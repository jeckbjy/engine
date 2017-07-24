#pragma once
#include "Cute/Foundation.h"
#include "Cute/Resource.h"

CUTE_NS_BEGIN

class CUTE_CORE_API FontGlyph
{
public:
    short x;
    short y;
    short width;
    short height;
    short offsetX;
    short offsetY;
    short advanceX;
    uint  page;
    bool  used;
//    bool  kerninged;
};

class CUTE_CORE_API FontFace
{
public:
    FontFace();
    virtual ~FontFace();
    
    virtual bool load(const String& path, float pointSize) = 0;
    virtual bool prepare(const String& text);
    
    virtual const FontGlyph* getGlyph(unsigned ch);
    
    short getKerning(unsigned c, unsigned d) const;
    
protected:
    void addKerning(unsigned c, unsigned d, short kerning);
    
private:
    typedef HashMap<unsigned, FontGlyph>    GlyphMap;
    typedef HashMap<unsigned, short>        KerningMap;
    GlyphMap    m_glyphs;
    KerningMap  m_kernings;
    float       m_size;
};

CUTE_NS_END
