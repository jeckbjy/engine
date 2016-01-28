#include "D3D11_RenderWindow.h"
#include "D3D11_Texture.h"

CU_NS_BEGIN

D3D11RenderWindow::D3D11RenderWindow(Window* wnd, IDXGIFactoryN* factory, ID3D11DeviceN* device)
:m_wnd(wnd), m_chain(NULL)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.OutputWindow = wnd->handle();
	desc.BufferDesc.Width = wnd->width();
	desc.BufferDesc.Height = wnd->height();
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// д������

	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Windowed = TRUE;

	HRESULT hr = factory->CreateSwapChain(device, &desc, &m_chain);
	if (FAILED(hr))
	{
		// Try a second time, may fail the first time due to back buffer count,
		// which will be corrected by the runtime
		hr = factory->CreateSwapChain(device, &desc, &m_chain);
	}
	D3D11_CHECK(hr, "Unable to Get Back Buffer for swap chain");
	// create view
	create(device);
}

D3D11RenderWindow::~D3D11RenderWindow()
{

}

void D3D11RenderWindow::bind(ID3D11DeviceContextN* context)
{
	context->OMSetRenderTargets(1, &m_rtv, m_ds ? m_ds->getDSV() : NULL);
}

void D3D11RenderWindow::create(ID3D11DeviceN* device)
{
	D3D11_RELEASE(m_buffer);
	HRESULT hr = m_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_buffer);
	D3D11_CHECK(hr, "Unable to Get Back Buffer for swap chain");
	D3D11_TEXTURE2D_DESC tex_desc;
	m_buffer->GetDesc(&tex_desc);
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	ZeroMemory(&rtv_desc, sizeof(rtv_desc));
	rtv_desc.Format = tex_desc.Format;
	rtv_desc.ViewDimension = m_wnd->isMultisample() ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_buffer, &rtv_desc, &m_rtv);
	if (FAILED(hr))
	{
		throw std::runtime_error("Unable to create rendertagert view");
	}
	// ����DS
	
}

void D3D11RenderWindow::resize(uint32_t width, uint32_t height)
{
	DXGI_SWAP_CHAIN_DESC desc;
	m_chain->GetDesc(&desc);

	UINT flags = m_wnd->isFullScreen() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;
	HRESULT hr = m_chain->ResizeBuffers(desc.BufferCount, width, height, desc.BufferDesc.Format, flags);
	D3D11_CHECK(hr, "Call to ResizeBuffers failed.");
}

CU_NS_END