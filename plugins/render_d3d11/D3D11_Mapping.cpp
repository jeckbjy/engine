#include "D3D11_Mapping.h"

CU_NS_BEGIN

LPCSTR D3D11Mapping::getSemantic(VertexUsage usage, UINT& index)
{
	LPCSTR sematic_names[] = {"POSITION", "NORMAL", "COLOR", "TANGENT", "BINORMAL", "BLENDWEIGHTS", "BLENDINDICES", "TEXCOORD" };
	index = usage < VU_TEXCOORD0 ? 0 : usage - VU_TEXCOORD0;
	return usage < VU_TEXCOORD0 ? sematic_names[usage] : "TEXCOORD";
}

CU_NS_END