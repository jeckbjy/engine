#include "D3D12_Pipeline.h"
#include "D3D12_Mapping.h"
#include "D3D12_Device.h"
//#include "D3D12_Program.h"
#include "D3D12_CommandList.h"
#include "D3D12_DescriptorSetLayout.h"

CUTE_NS_BEGIN

D3D12_Pipeline::D3D12_Pipeline()
	: m_pipeline(NULL)
	, m_primitive(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
	, m_layout(NULL)
	, m_isGraphics(false)
{
}

D3D12_Pipeline::~D3D12_Pipeline()
{
	term();
}

bool D3D12_Pipeline::init(D3D12_Device* device, const GraphicsPipelineDesc& desc)
{
	ID3D12Device* native = device->getDevice();
	D3D12_DescriptorSetLayout* layout = static_cast<D3D12_DescriptorSetLayout*>(desc.layout);
	D3D12_FrameBuffer* pWrapFrameBuffer = (D3D12_FrameBuffer*)(desc.framebuffer);

	D3D12_INPUT_ELEMENT_DESC elements[D3D12_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT] = {};

	// convert
	D3D12_GRAPHICS_PIPELINE_STATE_DESC info = {};
	info.pRootSignature = layout->getRootSignature();

	info.BlendState.AlphaToCoverageEnable = desc.multiSample.enableAlphaToCoverage ? TRUE : FALSE;

	HRESULT hr = native->CreateGraphicsPipelineState(&info, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr))
		return false;

	m_primitive = D3D12_Mapping::getPrimitive(desc.primitiveTopology, desc.tessellation.patchControlCount);
	m_isGraphics = true;
	return true;
}

bool D3D12_Pipeline::init(D3D12_Device* device, const ComputePipelineDesc& desc)
{
	ID3D12Device* native = device->getDevice();
	D3D12_DescriptorSetLayout* layout = static_cast<D3D12_DescriptorSetLayout*>(desc.layout);

	m_isGraphics = false;

	D3D12_COMPUTE_PIPELINE_STATE_DESC info = {};
	info.pRootSignature = layout->getRootSignature();
	
	HRESULT hr = native->CreateComputePipelineState(&info, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr))
		return false;

	return true;
}

void D3D12_Pipeline::term()
{
	D3D12_RELEASE(m_pipeline);
}

void D3D12_Pipeline::issue(ID3D12GraphicsCommandList* cmdList)
{
	if (m_isGraphics)
	{
		cmdList->SetGraphicsRootSignature(m_layout->getRootSignature());
		cmdList->SetPipelineState(m_pipeline);
		cmdList->IASetPrimitiveTopology(m_primitive);
	}
	else
	{
		cmdList->SetComputeRootSignature(m_layout->getRootSignature());
		cmdList->SetPipelineState(m_pipeline);
	}
}

//D3D12_GraphicsPipeline::D3D12_GraphicsPipeline(ID3D12Device* device, const PipelineDesc& desc)
//	: m_rootSignature(NULL)
//{
//	const RenderStateDesc* states = desc.states;
//	// 根据shader创建
//	ID3D10Blob *sig, *err;
//	D3D12_ROOT_SIGNATURE_DESC sigDesc;
//	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//	D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sig, &err);
//	device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
//	
//	D3D12_RELEASE(sig);
//	//D3D12_RELEASE(err);
//
//	// 填充固定的
//	m_desc.PrimitiveTopologyType = D3D12_Mapping::getTopologyType(states->topology);
//	m_desc.pRootSignature = m_rootSignature;
//	
//	//D3D12Mapping::fillShader(m_desc.VS, desc.vs);
//	//D3D12Mapping::fillShader(m_desc.HS, desc.hs);
//	//D3D12Mapping::fillShader(m_desc.DS, desc.ds);
//	//D3D12Mapping::fillShader(m_desc.GS, desc.gs);
//	//D3D12Mapping::fillShader(m_desc.PS, desc.ps);
//
//	D3D12_Mapping::fillRasterizerState(m_desc.RasterizerState, states->rasterizer);
//	D3D12_Mapping::fillDepthStencilState(m_desc.DepthStencilState, states->depthStencil);
//	D3D12_Mapping::fillBlendState(m_desc.BlendState, states->blend);
//}
//
//D3D12_GraphicsPipeline::~D3D12_GraphicsPipeline()
//{
//	D3D12_RELEASE(m_rootSignature);
//}
//
//void D3D12_GraphicsPipeline::bind(D3D12_CommandList* cmdBuffer)
//{
//	D3D12_Device* device = gD3D12Device();
//	// 查找
//	D3D12_DescriptorSetLayout* layout = cmdBuffer->getLayout();
//	if (!layout)
//		return;
//
//	ID3D12PipelineState* pso = NULL;
//	PipelineMap::iterator itor = m_pipelines.find(layout->getID());
//	if (itor == m_pipelines.end())
//	{
//		// 创建
//		layout->fillLayout(m_desc.InputLayout);
//		HRESULT hr = device->getDevice()->CreateGraphicsPipelineState(&m_desc, IID_PPV_ARGS(&pso));
//		if (FAILED(hr))
//			return;
//
//		m_pipelines[layout->getID()] = pso;
//	}
//	else
//	{
//		pso = itor->second;
//	}
//
//	cmdBuffer->native()->SetPipelineState(pso);
//}
//
////////////////////////////////////////////////////////////////////////////
////
////////////////////////////////////////////////////////////////////////////
//D3D12_ComputePipeline::D3D12_ComputePipeline(ID3D12Device* device, const PipelineDesc& desc)
//{
//	D3D12_COMPUTE_PIPELINE_STATE_DESC pso = {};
//	pso.pRootSignature = NULL;
//
//	//D3D12Mapping::fillShader(pso.CS, desc.cs);
//
//	device->CreateComputePipelineState(&pso, IID_PPV_ARGS(&m_pipeline));
//}
//
//D3D12_ComputePipeline::~D3D12_ComputePipeline()
//{
//	D3D12_RELEASE(m_pipeline);
//}
//
//void D3D12_ComputePipeline::bind(D3D12_CommandList* cmdBuffer)
//{
//	if (!m_pipeline)
//		return;
//
//	cmdBuffer->native()->SetPipelineState(m_pipeline);
//}

CUTE_NS_END
