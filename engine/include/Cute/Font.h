#pragma once
#include "Cute/Foundation.h"
#include "Cute/Resource.h"

CUTE_NS_BEGIN

class ITexture;
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
};

//class CUTE_CORE_API FontFace
//{
//public:
//private:
//};

class CUTE_CORE_API Font : public Resource
{
public:
	Font();
	virtual ~Font();
    
    virtual bool  isMutableGlyphs() const = 0;
    virtual const FontGlyph* getGlyph(int ch, int flags) = 0;
    virtual ITexture* getTexture(int page) const = 0;
//    virtual short getKerning(int c, int d) const;
    
private:
};

CUTE_NS_END

