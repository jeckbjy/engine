#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11DescriptorSet : public DescriptorSet
{
public:
	D3D11DescriptorSet();
	~D3D11DescriptorSet();

	void bind(const String& name, GpuResource* res);
	void bind(uint32_t index, GpuResource* res);

private:

};

CU_NS_END