#include "D3D11_Mapping.h"

CU_NS_BEGIN

void D3D11Mapping::getSemantic(Semantic usage, LPCSTR& str, UINT& index)
{
	static const LPCSTR sematic_names[] = { "POSITION", "NORMAL", "COLOR", "TANGENT", "BINORMAL", "BLENDWEIGHTS", "BLENDINDICES" };
	if (usage >= SEMANTIC_TEXCOORD0)
	{
		str = "TEXCOORD";
		index = usage - SEMANTIC_TEXCOORD0;
	}
	else
	{
		str = sematic_names[usage];
		index = 0;
	}
}

DXGI_FORMAT D3D11Mapping::getFormat(PixelFormat fmt)
{
	return DXGI_FORMAT_UNKNOWN;
}

CU_NS_END