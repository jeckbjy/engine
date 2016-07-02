#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_FrameBuffer : public FrameBuffer
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

CU_NS_END