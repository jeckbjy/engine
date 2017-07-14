#pragma once
#include "METAL_API.h"

CUTE_NS_BEGIN

class CUTE_METAL_API METAL_SwapChain : public ISwapChain
{
public:
private:
//    CAMetalLayer* m_layer;
    id<MTLDrawable> m_handle;
};

CUTE_NS_END
