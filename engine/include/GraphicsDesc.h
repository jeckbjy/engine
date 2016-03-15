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

//
// Shader反射
struct CU_API VariableDesc
{
	String  name;
	int8_t  type;
	int32_t block;	// 对应block索引
	int32_t size;	// 数据大小
};

struct CU_API UniformDesc
{
	enum Type
	{
		T_BUFFER,	// 普通内存 
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
