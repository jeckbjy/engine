#pragma once
#include "D3D11_API.h"
#include "SmartPtr.h"

CU_NS_BEGIN

class D3D11_CommandBuffer;
class CU_D3D11_API D3D11_FrameBuffer : public FrameBuffer
{
public:
	D3D11_FrameBuffer();
	~D3D11_FrameBuffer();

	void bind(D3D11_CommandBuffer* cmds);

private:
	void update();

private:
	typedef Vector<ID3D11RenderTargetView*> RenderTargetViewVec;

	ID3D11DepthStencilView* m_dsv;
	RenderTargetViewVec		m_rtv;
};

CU_NS_END