#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11Mapping
{
public:
	static LPCSTR getSemantic(VertexUsage usage, UINT& index);
};

CU_NS_END