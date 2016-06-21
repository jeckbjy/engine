#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_SwapChain : public SwapChain
{
	DECLARE_RTTI(D3D11_SwapChain, SwapChain, OBJ_ID_D3D11_SWAPCHAIN);
public:
	D3D11_SwapChain(Window* wnd, IDXGIFactoryN* factory, ID3D11DeviceN* device);
	~D3D11_SwapChain();

	void present();

	void bind(ID3D11ContextN* context);
	void clearRTV(ID3D11ContextN* context, const Color& color);
	void clearDSV(ID3D11ContextN* context, UINT32 flags, float depth, UINT8 stencil);

private:
	void create(ID3D11DeviceN* device);

private:
	Window*					m_wnd;
	IDXGISwapChainN*		m_chain;
	ID3D11Texture2D*		m_buffer;
	ID3D11Texture2D*		m_depthstencil;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView*	m_dsv;
};

CU_NS_END