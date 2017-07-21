#include "FreeType_Font.h"

CUTE_NS_BEGIN

FreeTypeFont::FreeTypeFont()
    : m_library(NULL)
{
    
}

bool FreeTypeFont::init(const void* buffer, unsigned length, float pointSize)
{
    FT_Error error = FT_Init_FreeType(&m_library);
    if(error)
        return false;
    
//    error = FT_NewM(m_library, );
    
    return true;
}

void FreeTypeFont::term()
{
    if(m_freetype != NULL)
    {
        FT_Done_FreeType(m_library);
        m_library = NULL;
    }
}

CUTE_NS_END
