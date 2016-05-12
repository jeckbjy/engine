#pragma once
#include "D3D11_API.h"

CU_NS_BEGIN

class CU_D3D11_API D3D11Device : public Device
{
public:
	D3D11Device();
	~D3D11Device();

	GpuBuffer*		newBuffer(const BufferDesc& desc);
	Texture*		newTexture(const TextureDesc& desc);
	RenderTarget*	newRenderWindow(Window* hwnd);
	RenderTarget*	newRenderTexture(Texture* rtv, Texture* dsv /* = NULL */);
	InputLayout*	newInputLayout(const InputElement* elements, size_t count);
	Program*		newProgram();
	Pipeline*		newPipeline(const GraphicsPipelineDesc& desc);
	Pipeline*		newPipeline(const ComputePipelineDesc& desc);
	DescriptorSet*	newDescriptorSet(Program* prog);
	CommandBuffer*	newCommandBuffer();
	CommandQueue*	newCommandQueue();

	ID3D11DeviceN*	getDevice() { return m_device; }
	ID3D11ContextN*	getContext() { return m_context; }
	ID3D11ClassLinkage*	getLinkage() { return m_linkage; }

private:
	ID3D11DeviceN*		m_device;
	IDXGIFactoryN*		m_factory;
	ID3D11ContextN*		m_context;
	ID3D11ClassLinkage* m_linkage;
};

extern CU_D3D11_API D3D11Device		gD3D11Device();
extern CU_D3D11_API ID3D11DeviceN*	gD3D11NativeDevice();
extern CU_D3D11_API ID3D11ContextN* gD3D11NativeContext();

CU_NS_END