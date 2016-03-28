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
	const void*	data;
	TextureDesc();
	TextureDesc(PixelFormat fmt, uint32_t width, uint32_t height, TextureUsage usage);
};

// ����
struct CU_API SamplerDesc
{
	FilterType	magFilter;
	FilterType	minFilter;
	FilterType	mipFilter;
	AddressMode addrU;
	AddressMode addrV;
	AddressMode addrW;
	bool		anisotropyEnable;
	float		maxAnisotropy;
	float		mipLodBias;
	CompareOp	compareOp;
	float		minLod;
	float		maxLod;
	float		borderColor[4];
	bool		unnormalizedCoordinates;
	SamplerDesc();
};

struct CU_API RasterizerDesc
{
	FillMode	fillMode;
	CullMode	cullMode;
	FrontFace	frontFace;
	bool		discardEnable;
	bool		depthClampEnable;
	bool		depthBiasEnable;
	float		depthBiasConstantFactor;
	float		depthBiasClamp;
	float		depthBiasSlopeFactor;
	float		lineWidth;
	RasterizerDesc();
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

	MultisampleDesc();
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
	StencilOpState();
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
	DepthStencilDesc();
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
	BlendTargetDesc();
};

struct CU_API BlendDesc
{
	bool			logicOpEnable;
	LogicOp			logicOp;
	BlendTargetDesc	targets[8];
	float			blendConstants[4];
	BlendDesc();
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

struct CU_API InputElement
{
	Semantic		semantic;	//
	PixelFormat		format;
	uint8_t			slot;		// Bufferλ��
	uint8_t			offset;		// ƫ��
	InputRate		rate;
	InputElement(){ memset(this, 0, sizeof(InputElement)); }
	InputElement(Semantic sem, PixelFormat format = PF_UNKNOWN, uint8_t slot = 0, InputRate rate = INPUT_RATE_VERTEX);
};

// ����һ��descriptor���ͼ�����
struct CU_API DescriptorRangeDesc
{
	uint32_t		slot;
	DescriptorType	type;
	uint32_t		count;
	ShaderStageFlag	stages;
};

// ����һ��DescriptorSet��Ϣ,�����ж��ٸ�DescriptorRange
struct CU_API DescriptorSetLayoutDesc
{
	uint32_t					count;
	const DescriptorRangeDesc*	descriptors;
};

// ��̬����
struct CU_API ConstantRangeDesc
{
	uint32_t		offset;
	uint32_t		size;
	ShaderStageFlag stages;
};

// �ɺ��ж��DescriptorSet�Լ�Const��Sampler
struct CU_API PipelineLayoutDesc 
{
	uint32_t						layoutCount;
	const DescriptorSetLayoutDesc*	layouts;
	uint32_t						counstantRangeCount;
	const ConstantRangeDesc*		counstantRanges;
	// samplers, single descriptor??
};

CU_NS_END
