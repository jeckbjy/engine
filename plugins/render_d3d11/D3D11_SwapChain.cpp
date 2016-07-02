#include "D3D11_SwapChain.h"
#include "D3D11_Texture.h"
#include "D3D11_Mapping.h"

CU_NS_BEGIN

D3D11_SwapChain::D3D11_SwapChain(const SwapChainDesc& info, IDXGIFactoryN* factory, ID3D11DeviceN* device)
	: m_wnd(info.wnd)
	, m_chain(NULL)
	, m_buffer(NULL)
	, m_depthstencil(NULL)
	, m_rtv(NULL)
	, m_dsv(NULL)
	, m_dsFormat(info.depthStencilFormat)
	, m_dsFlag(0)
	, m_syncInterval(info.syncInterval)
{
	if (info.readOnlyDepth)
		m_dsFlag |= D3D11_DSV_READ_ONLY_DEPTH;
	if (info.readOnlyStencil)
		m_dsFlag |= D3D11_DSV_READ_ONLY_STENCIL;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.SwapEffect = D3D11_Mapping::getSwapEffect(info.swapMode);
	desc.OutputWindow = m_wnd->getHandle();
	desc.BufferCount = info.bufferCount;
	desc.BufferDesc.Width = m_wnd->getWidth();
	desc.BufferDesc.Height = m_wnd->getHeight();
	desc.BufferDesc.Format = D3D11_Mapping::getFormat(info.colorFormat);
	desc.BufferDesc.ScanlineOrdering = D3D11_Mapping::getScanlineOrder(info.scanlineOrdering);
	desc.BufferDesc.Scaling = D3D11_Mapping::getScaling(info.scaling);
	if (m_syncInterval > 0)
	{
		desc.BufferDesc.RefreshRate.Numerator = (UINT)info.refreshRateNumerator;
		desc.BufferDesc.RefreshRate.Denominator = (UINT)info.refreshRateDenominator;
	}
	else
	{
		desc.BufferDesc.RefreshRate.Numerator = 0;
		desc.BufferDesc.RefreshRate.Denominator = 1;
	}
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = (UINT)info.sampleCount;
	desc.SampleDesc.Quality = (UINT)info.sampleQuailty;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = m_wnd->isFullscreen() ? FALSE : TRUE;

	HRESULT hr = factory->CreateSwapChain(device, &desc, &m_chain);
	if (FAILED(hr))
	{
		// Try a second time, may fail the first time due to back buffer count,
		// which will be corrected by the runtime
		hr = factory->CreateSwapChain(device, &desc, &m_chain);
	}
	D3D11_CHECK(hr, "Unable to Get Back Buffer for swap chain");

	create(device, &desc);
}

D3D11_SwapChain::~D3D11_SwapChain()
{
	D3D11_RELEASE(m_chain);
}

void D3D11_SwapChain::present()
{
	m_chain->Present(m_syncInterval, 0);
}

void D3D11_SwapChain::create(ID3D11DeviceN* device, const DXGI_SWAP_CHAIN_DESC* chainDesc)
{
	D3D11_RELEASE(m_buffer);
	D3D11_RELEASE(m_depthstencil);
	D3D11_RELEASE(m_rtv);
	D3D11_RELEASE(m_dsv);

	HRESULT hr;
	hr = m_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_buffer);
	D3D11_CHECK(hr, "Unable to Get Back Buffer for swap chain");

	D3D11_TEXTURE2D_DESC texDesc;
	m_buffer->GetDesc(&texDesc);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = chainDesc->SampleDesc.Count > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(m_buffer, &rtvDesc, &m_rtv);
	D3D11_CHECK(hr, "create rtv failed.");

	// ´´½¨ds
	if (m_dsFormat != PF_UNKNOWN)
	{
		D3D11_TEXTURE2D_DESC dsDesc;
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.Width = texDesc.Width;
		dsDesc.Height = texDesc.Height;
		dsDesc.ArraySize = 1;
		dsDesc.Format = D3D11_Mapping::getFormat(m_dsFormat);
		dsDesc.SampleDesc = texDesc.SampleDesc;
		dsDesc.MipLevels = 1;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;

		if (m_dsFlag == 0)
			dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		else
			dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		hr = device->CreateTexture2D(&dsDesc, NULL, &m_depthstencil);
		D3D11_CHECK(hr, "create depth stencil texture failed.");

		// create view
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(dsvDesc));
		dsvDesc.Format = dsDesc.Format;
		if (dsDesc.SampleDesc.Count > 1)
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;
		}
		dsvDesc.Flags |= m_dsFlag;
		hr = device->CreateDepthStencilView(m_depthstencil, &dsvDesc, &m_dsv);
		D3D11_CHECK(hr, "create depth stencil view failed.");
	}
}

void D3D11_SwapChain::bind(void* param)
{
	ID3D11ContextN* context = (ID3D11ContextN*)param;
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