#pragma once
#include "METAL_API.h"

CUTE_NS_BEGIN

class CUTE_METAL_API METAL_Buffer : public IBuffer
{
public:
    METAL_Buffer();
    ~METAL_Buffer();
    
    void* map();
    void unmap();
    
private:
    id<MTLBuffer> m_handle;
};

CUTE_NS_END
