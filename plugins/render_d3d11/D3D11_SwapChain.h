#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11_SwapChain : public SwapChain
{
public:
	D3D11_SwapChain();
	~D3D11_SwapChain();

	void present();
	FrameBuffer* getFrameBuffer() { return NULL; }

private:
	Window*					m_wnd;
	IDXGISwapChainN*		m_chain;
};

CU_NS_END