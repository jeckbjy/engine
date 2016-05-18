#pragma once
#include "D3D11_API.h"
#include "D3D11_StateMap.h"

CU_NS_BEGIN

class D3D11BlendState;
class D3D11SamplerState;
class D3D11RasterizerState;
class D3D11DepthStencilState;

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

	D3D11BlendState*			getBlendState(const BlendDesc& desc);
	D3D11SamplerState*			getSamplerState(const SamplerDesc& desc);
	D3D11RasterizerState*		getRasterizerState(const RasterizerDesc& desc);
	D3D11DepthStencilState*		getDepthStencilState(const DepthStencilDesc& desc);

private:
	typedef StateMap<D3D11BlendState, BlendDesc>				BlendMap;
	typedef StateMap<D3D11SamplerState, SamplerDesc>			SamplerMap;
	typedef StateMap<D3D11RasterizerState, RasterizerDesc>		RasterizerMap;
	typedef StateMap<D3D11DepthStencilState, DepthStencilDesc>	DepthStencilMap;

	ID3D11DeviceN*		m_device;
	IDXGIFactoryN*		m_factory;
	ID3D11ContextN*		m_context;
	ID3D11ClassLinkage* m_linkage;

	BlendMap			m_blends;
	SamplerMap			m_samplers;
	RasterizerMap		m_rasterizers;
	DepthStencilMap		m_depthStencils;
};

extern CU_D3D11_API D3D11Device*	gD3D11Device();
extern CU_D3D11_API ID3D11DeviceN*	gD3D11NativeDevice();
extern CU_D3D11_API ID3D11ContextN* gD3D11NativeContext();

CU_NS_END