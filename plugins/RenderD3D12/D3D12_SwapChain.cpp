#include "D3D12_SwapChain.h"
#include "D3D12_Device.h"
#include "D3D12_CommandQueue.h"
#include "D3D12_Mapping.h"
#include "D3D12_Texture.h"

CUTE_NS_BEGIN

D3D12_SwapChain::D3D12_SwapChain()
	: m_chain(NULL)
{
}

D3D12_SwapChain::~D3D12_SwapChain()
{
	term();
}

bool D3D12_SwapChain::init(D3D12_Device* device, const SwapChainDesc& desc)
{
	D3D12_CommandQueue* pQueue = NULL;
	if (!device->getGraphicsQueue((ICommandQueue**)&pQueue))
		return false;

	HRESULT hr;
	ID3D12Device* pNativeDevice = device->getDevice();
	IDXGIFactory3* pNativeFactory = device->getFactory();
	ID3D12CommandQueue* pNativeQueue = pQueue->getQueue();

	
	uint32 w = desc.extent.width;
	uint32 h = desc.extent.height;

	if (desc.fullscreen)
	{
		w = GetSystemMetrics(SM_CXSCREEN);
		h = GetSystemMetrics(SM_CYSCREEN);
	}

	bool isSupportTearing = device->isSupportTearing();

	UINT flags = 0;
	//if (device->isSupportTearing())
	//	flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	DXGI_SWAP_CHAIN_DESC1 info = {};
	info.Width				= w;
	info.Height				= h;
	info.Format				= D3D12_Mapping::getFormat(desc.format);
	info.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	info.BufferCount		= desc.bufferCount;
	info.SampleDesc.Count	= desc.sampleCount;
	info.SampleDesc.Quality	= 0;
	info.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;	// DXGI_SWAP_EFFECT_FLIP_DISCARD
	info.Flags				= flags;

	IDXGISwapChain1* pSwapChain = NULL;
	hr = pNativeFactory->CreateSwapChainForHwnd(pNativeQueue, m_wnd, &info, NULL, NULL, &pSwapChain);
	if (FAILED(hr))
		return false;

	if (isSupportTearing)
		pNativeFactory->MakeWindowAssociation(m_wnd, DXGI_MWA_NO_ALT_ENTER);

	hr = pSwapChain->QueryInterface(IID_PPV_ARGS(&m_chain));
	D3D12_RELEASE(pSwapChain);
	if (FAILED(hr))
		return false;

	m_buffers.resize(desc.bufferCount);

	ComponentMapping components;
	components.r = TEXTURE_SWIZZLE_R;
	components.g = TEXTURE_SWIZZLE_G;
	components.b = TEXTURE_SWIZZLE_B;
	components.a = TEXTURE_SWIZZLE_A;

	for (uint32 i = 0; i < desc.bufferCount; ++i)
	{
		ID3D12Resource* pBuffer;
		hr = m_chain->GetBuffer(i, IID_PPV_ARGS(&pBuffer));
		if (SUCCEEDED(hr))
		{
			D3D12_Texture* texture = new D3D12_Texture();
			if (!texture->init(pNativeDevice, pBuffer, RESOURCE_USAGE_COLOR_TARGET, RESOURCE_STATE_PRESENT, components))
			{
				delete texture;
				return false;
			}

			m_buffers[i] = texture;
		}
	}

	if (desc.syncInterval == 0 && isSupportTearing && !desc.fullscreen)
		m_presentFlag = 0; // DXGI_PRESENT_ALLOW_TEARING
	else
		m_presentFlag = 0;


	return true;
}

void D3D12_SwapChain::term()
{
	for (size_t i = 0; i < m_buffers.size(); ++i)
	{
		// ÊÍ·Å safe_release
		delete m_buffers[i];
	}

	if (m_chain != NULL)
	{
		if (isFullScreen())
		{
			setFullScreen(false);
		}
	}

	D3D12_RELEASE(m_chain);
}

void D3D12_SwapChain::present()
{
	m_chain->Present(m_syncInterval, m_presentFlag);
}

CUTE_NS_END
