#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12_Graphics : public Graphics
{
public:
	D3D12_Graphics();
	~D3D12_Graphics();

private:
	ID3D12Debug* m_debuger;
};

CU_NS_END
