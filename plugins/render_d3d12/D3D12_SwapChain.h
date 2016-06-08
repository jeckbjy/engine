#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_SwapChain : public SwapChain
{
public:
	D3D12_SwapChain();
	~D3D12_SwapChain();

	void present();
	FrameBuffer* getFrameBuffer() { return NULL; }

private:
	Window*				m_wnd;
	IDXGISwapChain*		m_chain;
};

CU_NS_END