#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_FrameBuffer : public FrameBuffer
{
public:
	D3D12_FrameBuffer();
	~D3D12_FrameBuffer();

	void bind();
	void attach(size_t att, Texture* attachment);
	void detach(size_t att);
private:

};

CU_NS_END