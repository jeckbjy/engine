#pragma once
#include "D3D12_API.h"

CU_NS_BEGIN

class CU_D3D12_API D3D12Program : public Program
{
public:
	D3D12Program();
	~D3D12Program();

	bool compile(const ProgramDesc& desc);

	void release();
private:
	ID3DBlob* m_blob;
	ID3DBlob* m_error;
};

CU_NS_END