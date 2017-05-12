#pragma once
#include "D3D12_API.h"
#include "Cute/DynamicArray.h"

CUTE_NS_BEGIN

class D3D12_DescriptorSetLayout;
class CUTE_D3D12_API D3D12_DescriptorSet : public IDescriptorSet
{
public:
	D3D12_DescriptorSet();
	~D3D12_DescriptorSet();

	bool init(ID3D12Device* device, size_t entryCount, bool isGraphics);
	void term();

	void setBuffer(size_t index, IBuffer* buffer) OVERRIDE;
	void setTexture(size_t index, ITextureView* texture) OVERRIDE;
	void setSampler(size_t index, ISampler* sampler) OVERRIDE;

	void bind(ICommandList* pCmdList);

private:
	typedef DynamicArray<D3D12_GPU_DESCRIPTOR_HANDLE> HandleArray;
	HandleArray	m_handles;
	bool		m_isGraphics;
};

CUTE_NS_END
