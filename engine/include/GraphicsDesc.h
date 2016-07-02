#pragma once
#include "Color.h"
#include "Window.h"
#include "PixelFormat.h"
#include "GraphicsDefs.h"

CU_NS_BEGIN

// ����
struct CU_API SamplerDesc
{
	FilterType	magFilter;
	FilterType	minFilter;
	FilterType	mipFilter;
	AddressMode addrU;
	AddressMode addrV;
	AddressMode addrW;
	bool		anisotropyEnable;	// for check filter
	float		maxAnisotropy;		// uint?
	float		mipLodBias;
	CompareOp	compareOp;
	float		minLod;
	float		maxLod;
	float		borderColor[4];
	bool		unnormalizedCoordinates;

	SamplerDesc();
	bool operator==(const SamplerDesc& other) const;
	uint32_t getHashCode() const;
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
	bool operator==(const RasterizerDesc& other) const;
	uint32_t getHashCode() const;
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
	bool operator==(const MultisampleDesc& other) const;
};

// DepthStencilState
struct CU_API StencilOpDesc
{
	StencilOp	failOp;
	StencilOp	passOp;
	StencilOp	depthFailOp;
	CompareOp	compareOp;
	uint32_t	compareMask;
	uint32_t	writeMask;
	uint32_t	reference;
	
	StencilOpDesc();

	bool operator==(const StencilOpDesc& other) const;
};

struct CU_API DepthStencilDesc
{
	bool			depthTestEnable;
	bool			depthWriteEnable;
	bool			depthBoundsTestEnable;
	CompareOp		depthCompareOp;
	bool			stencilTestEnable;
	StencilOpDesc	front;
	StencilOpDesc	back;
	float			minDepthBounds;
	float			maxDepthBounds;

	DepthStencilDesc();
	bool operator==(const DepthStencilDesc& other) const;
	uint32_t getHashCode() const;
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

	bool operator == (const BlendTargetDesc& other) const;
	bool operator != (const BlendTargetDesc& other) const { return !(*this == other); }
};

#define MAX_BLEND_TARGET 8

struct CU_API BlendDesc
{
	bool			logicOpEnable;
	LogicOp			logicOp;
	BlendTargetDesc	targets[MAX_BLEND_TARGET];
	float			blendConstants[4];
	BlendDesc();

	bool operator == (const BlendDesc& other) const;
	uint32_t getHashCode() const;
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
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
	uint32_t	depthOrArraySize;
	uint32_t	mipLevels;
	uint32_t	samples;
	uint32_t	quality;
	uint32_t	tiling;
	const void*	data;
	TextureDesc();
	TextureDesc(PixelFormat fmt, uint32_t width, uint32_t height, TextureUsage usage);
};

// ����shader
struct CU_API ShaderDesc
{
	ShaderType	stage;
	String		code;
	String		file;		// �����ļ�
	String		entry;		// ��ڵ�
	String		macros;
	String		profile;
};

struct CU_API RenderStateDesc
{
	RasterizerDesc		rasterizer;
	DepthStencilDesc	depthStencil;
	BlendDesc			blend;
	MultisampleDesc		multisample;
	Topology			topology;
};

// ����
class ShaderProgram;
struct CU_API PipelineDesc
{
	ShaderProgram*			program;
	const RenderStateDesc*	states;
	PipelineDesc() :program(NULL), states(NULL){}
};

//
struct CU_API SwapChainDesc
{
	Window*		wnd;
	PixelFormat colorFormat;
	PixelFormat	depthStencilFormat;	// ��Ȼ�����
	size_t		bufferCount;		// ��̨����������
	size_t		sampleCount;
	size_t		sampleQuailty;
	size_t		refreshRateNumerator;
	size_t		refreshRateDenominator;
	size_t		syncInterval;
	ScanlineOrderMode scanlineOrdering;
	ScalingMode	scaling;
	SwapMode	swapMode;
	bool		readOnlyDepth;
	bool		readOnlyStencil;
	SwapChainDesc(Window* wnd, PixelFormat format = PF_R8G8B8A8_UNORM, PixelFormat dsFormat = PF_D24_UNORM_S8_UINT, size_t bufferCount = 1);
};

struct CU_API InputElement
{
	Semantic		semantic;	//
	PixelFormat		format;
	uint8_t			slot;		// Bufferλ��
	uint8_t			offset;		// ƫ��
	InputRate		rate;
	InputElement() { memset(this, 0, sizeof(InputElement)); }
	InputElement(Semantic sem, PixelFormat format = PF_UNKNOWN, uint8_t slot = 0, InputRate rate = INPUT_RATE_VERTEX);
};

// shader reflection
// StageInfoMap:�����ط������õ�,1:desicroptor:slot,2:variable:index+offset
typedef std::map<uint8_t, uint64_t> StageInfoMap;
struct CU_API UniformDesc
{
	UniformType	type;
	String		name;
	uint32_t	slot;		// desicroptor bind slot,UINT32_MAX�����Ǹ�variable
	uint32_t	arrays;
	uint32_t	bytes;		// variable or uniform buffer �ֽڴ�С
	uint32_t	offset;		// variable ��block��ƫ��
	uint32_t	index;		// variable ��Ӧ��block����
	StageInfoMap stages;
	UniformDesc();
	bool isVariable() const { return slot == UINT32_MAX; }
};

CU_NS_END
