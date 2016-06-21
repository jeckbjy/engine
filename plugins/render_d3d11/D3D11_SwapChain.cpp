#include "D3D11_SwapChain.h"
#include "D3D11_Texture.h"
#include "Window.h"

CU_NS_BEGIN

D3D11_SwapChain::D3D11_SwapChain(Window* wnd, IDXGIFactoryN* factory, ID3D11DeviceN* device)
	: m_wnd(wnd)
	, m_chain(NULL)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.OutputWindow = wnd->handle();
	desc.BufferDesc.Width = wnd->getWidth();
	desc.BufferDesc.Height = wnd->getHeight();
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// Ð´ËÀ£¿£¿

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
}

D3D11_SwapChain::~D3D11_SwapChain()
{

}

void D3D11_SwapChain::present()
{
	m_chain->Present(0, 0);
}

void D3D11_SwapChain::create(ID3D11DeviceN* device)
{
	D3D11_RELEASE(m_buffer);
	HRESULT hr = m_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_buffer);
	D3D11_CHECK(hr, "Unable to Get Back Buffer for swap chain");
	D3D11_TEXTURE2D_DESC tex_desc;
	m_buffer->GetDesc(&tex_desc);
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	ZeroMemory(&rtv_desc, sizeof(rtv_desc));
	rtv_desc.Format = tex_desc.Format;
	rtv_desc.ViewDimension = m_wnd->isMultiSample() ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_buffer, &rtv_desc, &m_rtv);
	if (FAILED(hr))
	{
		throw std::runtime_error("Unable to create rendertagert view");
	}
	// ´´½¨DS
}

void D3D11_SwapChain::bind(ID3D11ContextN* context)
{
	ID3D11RenderTargetView* views[1] = { m_rtv };
	context->OMSetRenderTargets(1, views, m_dsv);
}

void D3D11_SwapChain::clearRTV(ID3D11ContextN* context, const Color& color)
{
	if (m_rtv)
		context->ClearRenderTargetView(m_rtv, color.data());
}

void D3D11_SwapChain::clearDSV(ID3D11ContextN* context, UINT32 flags, float depth, UINT8 stencil)
{
	if (m_dsv)
		context->ClearDepthStencilView(m_dsv, flags, depth, stencil);
}

CU_NS_END