#include "D3D12_Graphics.h"

CU_NS_BEGIN

D3D12Graphics::D3D12Graphics()
	: m_debuger(NULL)
{
	D3D12GetDebugInterface(IID_PPV_ARGS(&m_debuger));
}

D3D12Graphics::~D3D12Graphics()
{
}

CU_NS_END