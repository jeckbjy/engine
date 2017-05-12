#pragma once
#include "D3D12_API.h"

CUTE_NS_BEGIN

class CUTE_D3D12_API D3D12_FrameBuffer : public IFrameBuffer
{
public:
	D3D12_FrameBuffer();
	~D3D12_FrameBuffer();

	void bind(void* param);

private:
	void update();

private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_dsv;
	ID3D12DescriptorHeap*		m_rtv;
};

CUTE_NS_END
