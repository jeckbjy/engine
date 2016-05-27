#include "D3D11_Graphics.h"

CU_NS_BEGIN

D3D11Graphics::D3D11Graphics()
{
	m_device = new D3D11_Device();
}

D3D11Graphics::~D3D11Graphics()
{
	delete m_device;
}

CU_NS_END