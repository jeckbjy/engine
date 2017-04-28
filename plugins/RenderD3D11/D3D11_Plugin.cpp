#include "D3D11_API.h"
#include "D3D11_Plugin.h"
#include "D3D11_Device.h"

CUTE_NS_BEGIN

DEF_PLUGIN(CUTE_D3D11_API, D3D11_Plugin)

D3D11_Plugin::D3D11_Plugin()
	: Plugin(D3D11_NAME)
	, m_device(NULL)
{
}

D3D11_Plugin::~D3D11_Plugin()
{
	CU_SAFE_RELEASE(m_device);
}

void D3D11_Plugin::install()
{
	if (!m_device)
	{
		m_device = new D3D11_Device();
		m_device->retain();
	}
	gGraphics.setDevice(m_device);
}

void D3D11_Plugin::uninstall()
{
	if ((D3D11_Device*)gGraphics.getDevice() == m_device)
	{
		gGraphics.setDevice(NULL);
	}
}

CUTE_NS_END
