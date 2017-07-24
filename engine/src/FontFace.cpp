//! Font
#include "Cute/FontFace.h"

CUTE_NS_BEGIN

FontFace::FontFace()
{
}

FontFace::~FontFace()
{
}

bool FontFace::prepare(const String &text)
{
    return true;
}

const FontGlyph* FontFace::getGlyph(unsigned int ch)
{
    GlyphMap::iterator itor = m_glyphs.find(ch);
    if(itor != m_glyphs.end())
        return &(itor->second);
    else
        return NULL;
}

short FontFace::getKerning(unsigned int c, unsigned int d) const
{
    if(m_kernings.empty())
        return 0;
    
    if(c == '\n' || d == '\n')
        return 0;
    
    if (c > 0xffff || d > 0xffff)
        return 0;
    
    unsigned value = (c << 16) + d;
    KerningMap::const_iterator itor = m_kernings.find(value);
    if(itor != m_kernings.end())
        return itor->second;
    
    return 0;
}

void FontFace::addKerning(unsigned int c, unsigned int d, short kerning)
{
    unsigned value = (c << 16) + d;
    m_kernings[value] = kerning;
}

CUTE_NS_END
