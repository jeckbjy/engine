#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_SwapChain : public SwapChain
{
public:
	D3D11_SwapChain(Window* wnd, IDXGIFactoryN* factory, ID3D11DeviceN* device);
	~D3D11_SwapChain();

	void present();

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