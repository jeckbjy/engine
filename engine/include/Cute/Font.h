#pragma once
#include "Cute/FontFace.h"

CUTE_NS_BEGIN

static const int FONT_DPI = 96;

class ITexture;
// 异步加载Font资源?
// 动态绘制Font？
class CUTE_CORE_API Font : public Resource
{
public:
//    typedef RefPtr<Font> Ptr;
    enum Style
    {
        BOLD,
        ITALIC,
        UNDERLINE,
    };
    
    Font();
    Font(const String& path);
    ~Font();
    
private:
    String      m_path;
    FontFace*   m_face;
};

CUTE_NS_END

