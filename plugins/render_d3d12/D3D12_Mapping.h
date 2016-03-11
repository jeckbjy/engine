#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Mapping
{
public:
	static D3D12_RESOURCE_DIMENSION getTextureType(TexType type);
	static DXGI_FORMAT getFormat(PixelFormat format);
};

CU_NS_END
