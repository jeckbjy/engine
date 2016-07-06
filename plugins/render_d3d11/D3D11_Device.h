#pragma once
#include "D3D11_API.h"
#include "D3D11_StateMap.h"

CU_NS_BEGIN

class D3D11_Shader;
class D3D11_VertexLayout;

class D3D11_BlendState;
class D3D11_SamplerState;
class D3D11_RasterizerState;
class D3D11_DepthStencilState;

class CU_D3D11_API D3D11_Device : public Device
{
public:
	D3D11_Device();
	~D3D11_Device();

	GpuBuffer*					newBuffer(const BufferDesc& desc);
	Texture*					newTexture(const TextureDesc& desc);
	FrameBuffer*				newFrameBuffer();
	SwapChain*					newSwapChain(const SwapChainDesc& desc);
	VertexLayout*				newVertexLayout(const VertexElement* elements, size_t count);
	ShaderStage*				newShader();
	ShaderProgram*				newProgram();
	Pipeline*					newPipeline(const PipelineDesc& desc);
	DescriptorSet*				newDescriptorSet(Pipeline* pipeline);
	CommandBuffer*				newCommandBuffer();
	CommandQueue*				newCommandQueue();

	ID3D11DeviceN*				getDevice() { return m_device; }
	ID3D11ContextN*				getContext() { return m_context; }
	ID3D11ClassLinkage*			getLinkage() { return m_linkage; }

	D3D11_BlendState*			getBlendState(const BlendDesc& desc);
	D3D11_SamplerState*			getSamplerState(const SamplerDesc& desc);
	D3D11_RasterizerState*		getRasterizerState(const RasterizerDesc& desc);
	D3D11_DepthStencilState*	getDepthStencilState(const DepthStencilDesc& desc);

	ID3D11InputLayout*			getInputLayout(D3D11_Shader* vs, D3D11_VertexLayout* layout);

private:
	typedef std::map<uint64_t, ID3D11InputLayout*>						LayoutMap;

	typedef D3D11_StateMap<D3D11_BlendState, BlendDesc>					BlendMap;
	typedef D3D11_StateMap<D3D11_SamplerState, SamplerDesc>				SamplerMap;
	typedef D3D11_StateMap<D3D11_RasterizerState, RasterizerDesc>		RasterizerMap;
	typedef D3D11_StateMap<D3D11_DepthStencilState, DepthStencilDesc>	DepthStencilMap;

	ID3D11DeviceN*		m_device;
	ID3D11ContextN*		m_context;

	IDXGIFactoryN*		m_factory;
	IDXGIAdapterN*		m_adapter;
	ID3D11ClassLinkage* m_linkage;

	BlendMap			m_blends;
	SamplerMap			m_samplers;
	RasterizerMap		m_rasterizers;
	DepthStencilMap		m_depthStencils;

	LayoutMap			m_layouts;
	uint32_t			m_layoutMax;
};

extern CU_D3D11_API D3D11_Device*	gD3D11Device();
extern CU_D3D11_API ID3D11DeviceN*	gD3D11NativeDevice();
extern CU_D3D11_API ID3D11ContextN* gD3D11NativeContext();

CU_NS_END