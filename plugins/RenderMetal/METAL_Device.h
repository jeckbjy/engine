#pragma once
#include "METAL_API.h"

CUTE_NS_BEGIN

class CUTE_METAL_API METAL_Device : public IDevice
{
public:
    METAL_Device();
    ~METAL_Device();

private:
    id<MTLDevice> m_handle;
};

CUTE_NS_END
