#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11Mapping
{
public:
	static void getSemantic(Semantic usage, LPCSTR& str, UINT& index);
	static DXGI_FORMAT getFormat(PixelFormat fmt);
};

CU_NS_END