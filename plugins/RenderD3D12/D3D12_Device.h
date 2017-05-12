#pragma once
#include "D3D12_API.h"
#include "Cute/BitMask.h"
#include "Cute/Array.h"

CUTE_NS_BEGIN

class D3D12_DescriptorHeap;
class CUTE_D3D12_API D3D12_Device : public IDevice
{
public:
	enum EXTENSION
	{
		EXT_TEARING = 0x01,
	};

	D3D12_Device();
	~D3D12_Device();

	bool init(const DeviceDesc& desc);
	void term();

	D3D12_DescriptorHeap*	getDescriptorHeap(size_t index);
	ID3D12Device*			getDevice() const { return m_device;}
	IDXGIFactory3*			getFactory() const { return m_factory; }

	bool isSupportTearing() const { return m_features[EXT_TEARING]; }

private:
	typedef Array<D3D12_DescriptorHeap*, 4> HeapArray;
	IDXGIFactory3*		m_factory;
	IDXGIAdapter2*		m_adapter;
	IDXGIOutput1*		m_output;
	ID3D12Device*		m_device;
	DeviceInfo			m_info;
	BitMask8			m_features;
	HeapArray			m_heaps;
};

//class CUTE_D3D12_API D3D12_Device : public Device
//{
//public:
//	D3D12_Device();
//	~D3D12_Device();
//
//	/*GpuBuffer*		newBuffer(const BufferDesc& desc);
//	Texture*		newTexture(const TextureDesc& desc);
//	VertexLayout*	newVertexLayout(const VertexElement* elements, size_t count);
//	ShaderStage*	newShader();
//	ShaderProgram*	newProgram();
//	Pipeline*		newPipeline(const PipelineDesc& desc);
//	DescriptorSet*	newDescriptorSet(ShaderStage* prog);
//	CommandBuffer*	newCommandBuffer();
//	CommandQueue*	newCommandQueue();
//	FrameBuffer*	newFrameBuffer();
//	SwapChain*		newSwapChain(const SwapChainDesc& desc);*/
//
//	ID3D12Device* getDevice() { return m_device; }
//	ID3D12CommandAllocator* getAllocator() { return m_allocator; }
//
//	operator ID3D12Device*() const { return m_device; }
//
//private:
//	ID3D12Device*			m_device;
//	ID3D12CommandAllocator* m_allocator;
//	ID3D12Debug*			m_debuger;
//};
//
//extern CUTE_D3D12_API D3D12_Device*	gD3D12Device();
//extern CUTE_D3D12_API ID3D12Device*	gD3D12NativeDevice();

CUTE_NS_END
