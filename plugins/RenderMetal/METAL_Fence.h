#pragma once
#include "METAL_API.h"

CUTE_NS_BEGIN

class CUTE_METAL_API METAL_Fence : public IFence
{
public:
private:
    id<MTLFence> m_handle;
};

CUTE_NS_END
