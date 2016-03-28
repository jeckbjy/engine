#pragma once
#include "Color.h"
#include "PixelFormat.h"
#include "GraphicsDefs.h"

CU_NS_BEGIN

struct CU_API BufferDesc
{
	BufferUsage	usage;	// 用途
	uint32_t	stride;	// 顶点跨度
	uint32_t	counts;	// 顶点个数
	RES_FLAG	flags;	// 标识信息
	const void*	data;	// 数据
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

// 采样
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

// 多重采样
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

// 创建shader
struct CU_API ProgramDesc
{
	String code;
	String file;		// 所在文件
	String entry;		// 入口点
	String macros;
	String profile;
	uint32_t flags;
};

// shader描述信息
struct CU_API ShaderStageDesc
{
	uint32_t flags;
	Program* prog;
	String	 entry;
};

// 渲染管线
struct CU_API GraphicsPipelineDesc
{
	uint32_t					flags;
	uint32_t					stageCount;
	const ShaderStageDesc*		stages;
	// 其他状态信息
	RasterizerDesc				rasterizer;
	DepthStencilDesc			depthStencil;
	BlendDesc					blend;
	MultisampleDesc				multisample;
	Topology					topology;
};

// 计算管线
struct CU_API ComputePipelineDesc
{
	uint32_t			flags;
	ShaderStageDesc		stage;
};

struct CU_API InputElement
{
	Semantic		semantic;	//
	PixelFormat		format;
	uint8_t			slot;		// Buffer位置
	uint8_t			offset;		// 偏移
	InputRate		rate;
	InputElement(){ memset(this, 0, sizeof(InputElement)); }
	InputElement(Semantic sem, PixelFormat format = PF_UNKNOWN, uint8_t slot = 0, InputRate rate = INPUT_RATE_VERTEX);
};

// 描述一个descriptor类型及个数
struct CU_API DescriptorRangeDesc
{
	uint32_t		slot;
	DescriptorType	type;
	uint32_t		count;
	ShaderStageFlag	stages;
};

// 描述一个DescriptorSet信息,即含有多少个DescriptorRange
struct CU_API DescriptorSetLayoutDesc
{
	uint32_t					count;
	const DescriptorRangeDesc*	descriptors;
};

// 静态数据
struct CU_API ConstantRangeDesc
{
	uint32_t		offset;
	uint32_t		size;
	ShaderStageFlag stages;
};

// 可含有多个DescriptorSet以及Const和Sampler
struct CU_API PipelineLayoutDesc 
{
	uint32_t						layoutCount;
	const DescriptorSetLayoutDesc*	layouts;
	uint32_t						counstantRangeCount;
	const ConstantRangeDesc*		counstantRanges;
	// samplers, single descriptor??
};

CU_NS_END
