#pragma once
#include "D3D11_API.h"
#include "SmartPtr.h"

CUTE_NS_BEGIN

class D3D11_CommandBuffer;
class CUTE_D3D11_API D3D11_FrameBuffer : public FrameBuffer
{
	DECLARE_RTTI(D3D11_FrameBuffer, FrameBuffer, OBJ_ID_D3D11_FRAMEBUFFER);
public:
	D3D11_FrameBuffer();
	~D3D11_FrameBuffer();

	void bind(void* param);
	void clearRTV(ID3D11ContextN* context, const Color& color, uint8_t targetMask);
	void clearDSV(ID3D11ContextN* context, UINT32 flags, float depth, UINT8 stencil);

private:
	void update();

private:
	typedef Vector<ID3D11RenderTargetView*> RenderTargetViewVec;

	ID3D11DepthStencilView* m_dsv;
	RenderTargetViewVec		m_rtv;
};

CUTE_NS_END
