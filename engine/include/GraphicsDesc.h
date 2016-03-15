#pragma once
#include "Color.h"
#include "PixelFormat.h"
#include "GraphicsDefs.h"

CU_NS_BEGIN

struct CU_API BufferDesc
{
	BufferUsage	usage;	// ��;
	uint32_t	stride;	// ������
	uint32_t	counts;	// �������
	RES_FLAG	flags;	// ��ʶ��Ϣ
	const void*	data;	// ����
	BufferDesc(BufferUsage usage, uint32_t stride, uint32_t counts, RES_FLAG flags, const void* data)
		:usage(usage), stride(stride), counts(counts), flags(flags), data(data)
	{
	}
};

struct CU_API TextureDesc
{
	TexType		type;
	TextureUsage usage;
	PixelFormat format;
	uint32_t	width;
	uint32_t	height;
	uint32_t	depth;
	uint32_t	mipLevels;
	uint32_t	arrayLayers;
	uint32_t	samples;
	uint32_t	quality;
	uint32_t	tiling;
	void*		data;
};

struct CU_API RasterizerDesc
{
	PolygonMode	polygonMode;
	CullMode	cullMode;
	FrontFace	frontFace;
	bool		discardEnable;
	bool		depthClampEnable;
	bool		depthBiasEnable;
	float		depthBiasConstantFactor;
	float		depthBiasClamp;
	float		depthBiasSlopeFactor;
	float		lineWidth;
};

// ���ز���
// alphaToCoverage:http://www.zwqxin.com/archives/opengl/talk-about-alpha-to-coverage.html
struct CU_API MultisampleDesc
{
	uint32_t	rasterizationSamples;
	bool		sampleShadingEnable;
	float		minSampleShading;
	uint32_t	sampleMask;
	bool		alphaToCoverageEnable;
	bool		alphaToOneEnable;
};

// DepthStencilState
struct CU_API StencilOpState
{
	StencilOp	failOp;
	StencilOp	passOp;
	StencilOp	depthFailOp;
	CompareOp	compareOp;
	uint32_t	compareMask;
	uint32_t	writeMask;
	uint32_t	reference;
};

struct CU_API DepthStencilDesc
{
	bool			depthTestEnable;
	bool			depthWriteEnable;
	bool			depthBoundsTestEnable;
	CompareOp		depthCompareOp;
	bool			stencilTestEnable;
	StencilOpState	front;
	StencilOpState	back;
	float			minDepthBounds;
	float			maxDepthBounds;
};

// BlendState
struct BlendTargetDesc
{
	bool			blendEnable;
	BlendFactor		srcColorFactor;
	BlendFactor		dstColorFactor;
	BlendOp			colorOp;
	BlendFactor		srcAlphaFactor;
	BlendFactor		dstAlphaFactor;
	BlendOp			alphaOp;
	uint8_t			colorWriteMask;
};

struct CU_API BlendDesc
{
	bool			logicOpEnable;
	LogicOp			logicOp;
	BlendTargetDesc	targets[8];
	float			blendConstants[4];
};

// ����shader
struct CU_API ProgramDesc
{
	String code;
	String file;		// �����ļ�
	String entry;		// ��ڵ�
	String macros;
	String profile;
	uint32_t flags;
};

// shader������Ϣ
struct CU_API ShaderStageDesc
{
	uint32_t flags;
	Program* prog;
	String	 entry;
};

// ��Ⱦ����
struct CU_API GraphicsPipelineDesc
{
	uint32_t					flags;
	uint32_t					stageCount;
	const ShaderStageDesc*		stages;
	// ����״̬��Ϣ
	RasterizerDesc				rasterizer;
	DepthStencilDesc			depthStencil;
	BlendDesc					blend;
	MultisampleDesc				multisample;
	Topology					topology;
};

// �������
struct CU_API ComputePipelineDesc
{
	uint32_t			flags;
	ShaderStageDesc		stage;
};

//
// Shader����
struct CU_API VariableDesc
{
	String  name;
	int8_t  type;
	int32_t block;	// ��Ӧblock����
	int32_t size;	// ���ݴ�С
};

struct CU_API UniformDesc
{
	enum Type
	{
		T_BUFFER,	// ��ͨ�ڴ� 
		T_BLOCK,	// block buffer
		T_TEXTURE,
		T_SAMPLER,
	};
	String	name;
	int8_t	type;
	int32_t size;
	int8_t	shareable : 1;
	UniformDesc() :type(0), size(0), shareable(0){}
};

struct CU_API ParamDesc
{
	typedef std::vector<VariableDesc> VariableDescVec;
	typedef std::vector<UniformDesc> UniformDescVec;
	UniformDescVec uniforms;
	VariableDescVec variables;
};

CU_NS_END
