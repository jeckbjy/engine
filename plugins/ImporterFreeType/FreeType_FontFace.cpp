#include "FreeType_FontFace.h"
//#include "Cute/Stream.h"

CUTE_NS_BEGIN

inline int RoundToPixels(FT_Pos value)
{
    return (int)(value >> 6) + (((value & 0x3f) >= 0x20) ? 1 : 0);
}

class FreeType_Library
{
public:
    static FreeType_Library& instance()
    {
        static FreeType_Library _inst;
        return _inst;
    }
    
    static FT_Library get()
    {
        return instance().native();
    }
    
    FreeType_Library()
        : m_library(NULL)
    {
        FT_Library library;
        FT_Error error = FT_Init_FreeType(&library);
        if(!error)
        {
            m_library = library;
        }
    }
    
    ~FreeType_Library()
    {
        if(m_library != NULL)
        {
            FT_Done_FreeType(m_library);
            m_library = NULL;
        }
    }
    
    FT_Library native()
    {
        return m_library;
    }
    
private:
    FT_Library m_library;
};

//
//
//
FreeType_FontFace::FreeType_FontFace()
    : m_face(NULL)
{
    
}

FreeType_FontFace::~FreeType_FontFace()
{
    term();
}

bool FreeType_FontFace::init(const unsigned char* buffer, unsigned length, float pointSize)
{
    if(pointSize <= 0)
    {
        return false;
    }
    
    FT_Library library;
    FT_Face  face;
    FT_Error error;
    
    library = FreeType_Library::get();
    if(library == NULL)
    {
        return false;
    }
    
    error = FT_New_Memory_Face(library, buffer, length, 0, &face);
    if(error)
    {
        return false;
    }
    
    error = FT_Set_Char_Size(face, 0, pointSize * 64, FONT_DPI, FONT_DPI);
    if(error)
    {
        FT_Done_Face(face);
        return false;
    }
    
    m_face = face;
    
//    unsigned numGlyphs = (unsigned)face->num_glyphs;
    
    // load kerning
    if(FT_HAS_KERNING(face))
    {
        // Read kerning manually to be more efficient and avoid out of memory crash when use large font file, for example there
        // are 29354 glyphs in msyh.ttf
        FT_ULong tagKern = FT_MAKE_TAG('k', 'e', 'r', 'n');
        FT_ULong kerningTableSize = 0;
        FT_Error error = FT_Load_Sfnt_Table(face, tagKern, 0, 0, &kerningTableSize);
        if (error)
        {
            return false;
        }
        
        unsigned char* kerningTable = new unsigned char[kerningTableSize];
        error = FT_Load_Sfnt_Table(face, tagKern, 0, kerningTable, &kerningTableSize);
        if(error)
            return false;
        
        // Convert big endian to little endian：needed??
        for (unsigned i = 0; i < kerningTableSize; i += 2)
            std::swap(kerningTable[i], kerningTable[i + 1]);
        
        // parse table
//        MemoryStream stream();
    }
    
    return true;
}

void FreeType_FontFace::term()
{
    if(m_face)
    {
        FT_Done_Face(m_face);
        m_face = NULL;
    }
}

bool FreeType_FontFace::load(const String &path, float pointSize)
{
    return true;
}

bool FreeType_FontFace::prepare(const String &text)
{
    return false;
}

CUTE_NS_END
