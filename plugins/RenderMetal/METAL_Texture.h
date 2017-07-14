#pragma once
#include "METAL_API.h"

CUTE_NS_BEGIN

class CUTE_METAL_API METAL_Texture : public ITexture
{
public:
    METAL_Texture();
    ~METAL_Texture();
    
private:
    id<MTLTexture> m_handle;
};

CUTE_NS_END
