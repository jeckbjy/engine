#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// a3d
//http://qiita.com/ProjectAsura/items/8f03960246c105f0545a

enum INDIRECT_ARGUMENT_TYPE
{
	INDIRECT_ARGUMENT_TYPE_DRAW			= 0,
	INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED = 1,
	INDIRECT_ARGUMENT_TYPE_DISPATCH		= 2,
};

enum RESOURCE_KIND
{
	RESOURCE_KIND_BUFFER	= 0,
	RESOURCE_KIND_TEXTURE	= 1,
};

enum RESOURCE_DIMENSION
{
	RESOURCE_DIMENSION_UNKNOWN	 = 0, 
	RESOURCE_DIMENSION_BUFFER	 = 1, 
	RESOURCE_DIMENSION_TEXTURE1D = 2, 
	RESOURCE_DIMENSION_TEXTURE2D = 3, 
	RESOURCE_DIMENSION_TEXTURE3D = 4, 
	RESOURCE_DIMENSION_CUBEMAP	 = 5, 
};

enum VIEW_DIMENSION
{
	VIEW_DIMENSION_BUFFER			= 0,
	VIEW_DIMENSION_TEXTURE1D		= 1,
	VIEW_DIMENSION_TEXTURE1D_ARRAY	= 2,
	VIEW_DIMENSION_TEXTURE2D		= 3,
	VIEW_DIMENSION_TEXTURE2D_ARRAY	= 4,
	VIEW_DIMENSION_CUBEMAP			= 5,
	VIEW_DIMENSION_CUBEMAP_ARRAY	= 6,
	VIEW_DIMENSION_TEXTURE3D		= 7,
};

enum RESOURCE_LAYOUT
{
	RESOURCE_LAYOUT_LINEAR, 
	RESOURCE_LAYOUT_OPTIMAL,
};

enum RESOURCE_USAGE
{
	RESOURCE_USAGE_COLOR_TARGET		= 0x001,
	RESOURCE_USAGE_DEPTH_TARGET		= 0x002,
	RESOURCE_USAGE_UNORDERD_ACCESS	= 0x004,
	RESOURCE_USAGE_INDEX_BUFFER		= 0x008,
	RESOURCE_USAGE_VERTEX_BUFFER	= 0x010,
	RESOURCE_USAGE_CONSTANT_BUFFER	= 0x020,
	RESOURCE_USAGE_INDIRECT_BUFFER	= 0x040,
	RESOURCE_USAGE_SHADER_RESOURCE	= 0x080,
	RESOURCE_USAGE_COPY_SRC			= 0x100,
	RESOURCE_USAGE_COPY_DST			= 0x200,
	RESOURCE_USAGE_QUERY_BUFFER		= 0x300,
};

enum RESOURCE_FORMAT
{
    RESOURCE_FORMAT_UNKNOWN                 = 0, 
    RESOURCE_FORMAT_R32G32B32A32_FLOAT      = 1, 
    RESOURCE_FORMAT_R32G32B32A32_UINT       = 2, 
    RESOURCE_FORMAT_R32G32B32A32_SINT       = 3, 
    RESOURCE_FORMAT_R16G16B16A16_FLOAT      = 4, 
    RESOURCE_FORMAT_R16G16B16A16_UINT       = 5, 
    RESOURCE_FORMAT_R16G16B16A16_SINT       = 6, 
    RESOURCE_FORMAT_R32G32B32_FLOAT         = 7, 
    RESOURCE_FORMAT_R32G32B32_UINT          = 8, 
    RESOURCE_FORMAT_R32G32B32_SINT          = 9, 
    RESOURCE_FORMAT_R32G32_FLOAT            = 10,
    RESOURCE_FORMAT_R32G32_UINT             = 11,
    RESOURCE_FORMAT_R32G32_SINT             = 12,
    RESOURCE_FORMAT_R16G16_FLOAT            = 13,
    RESOURCE_FORMAT_R16G16_UINT             = 14,
    RESOURCE_FORMAT_R16G16_SINT             = 15,
    RESOURCE_FORMAT_R32_FLOAT               = 16,
    RESOURCE_FORMAT_R32_UINT                = 17,
    RESOURCE_FORMAT_R32_SINT                = 18,
    RESOURCE_FORMAT_R16_FLOAT               = 19,
    RESOURCE_FORMAT_R16_UINT                = 20,
    RESOURCE_FORMAT_R16_SINT                = 21,
    RESOURCE_FORMAT_R8G8B8A8_UNORM_SRGB     = 22,
    RESOURCE_FORMAT_R8G8B8A8_UNORM          = 23,
    RESOURCE_FORMAT_B8G8R8A8_UNORM_SRGB     = 24,
    RESOURCE_FORMAT_B8G8R8A8_UNORM          = 25,
    RESOURCE_FORMAT_R8G8_UNORM              = 26,
    RESOURCE_FORMAT_R8_UNORM                = 27,
    RESOURCE_FORMAT_D32_FLOAT               = 28,
    RESOURCE_FORMAT_D24_UNORM_S8_UINT       = 29,
    RESOURCE_FORMAT_D16_UNORM               = 30,
    RESOURCE_FORMAT_BC1_UNORM_SRGB          = 31,
    RESOURCE_FORMAT_BC1_UNORM               = 32,
    RESOURCE_FORMAT_BC2_UNORM_SRGB          = 33,
    RESOURCE_FORMAT_BC2_UNORM               = 34,
    RESOURCE_FORMAT_BC3_UNORM_SRGB          = 35,
    RESOURCE_FORMAT_BC3_UNORM               = 36,
    RESOURCE_FORMAT_BC4_UNORM               = 37,
    RESOURCE_FORMAT_BC4_SNORM               = 38,
    RESOURCE_FORMAT_BC5_UNORM               = 39,
    RESOURCE_FORMAT_BC5_SNORM               = 40,
    RESOURCE_FORMAT_BC6H_UF16               = 41,
    RESOURCE_FORMAT_BC6H_SF16               = 42,
    RESOURCE_FORMAT_BC7_UNORM_SRGB          = 43,
    RESOURCE_FORMAT_BC7_UNORN               = 44,
    RESOURCE_FORMAT_ASTC_4X4_UNORM_SRGB     = 45,
    RESOURCE_FORMAT_ASTC_4X4_UNORM          = 46,
    RESOURCE_FORMAT_ASTC_5X4_UNORM_SRGB     = 47,
    RESOURCE_FORMAT_ASTC_5X4_UNORM          = 48,
    RESOURCE_FORMAT_ASTC_5X5_UNORM_SRGB     = 49,
    RESOURCE_FORMAT_ASTC_5X5_UNORM          = 50,
    RESOURCE_FORMAT_ASTC_6X5_UNORM_SRGB     = 51,
    RESOURCE_FORMAT_ASTC_6X5_UNORM          = 52,
    RESOURCE_FORMAT_ASTC_6X6_UNORM_SRGB     = 53,
    RESOURCE_FORMAT_ASTC_6X6_UNORM          = 54,
    RESOURCE_FORMAT_ASTC_8X5_UNORM_SRGB     = 55,
    RESOURCE_FORMAT_ASTC_8X5_UNORM          = 56,
    RESOURCE_FORMAT_ASTC_8X6_UNORM_SRGB     = 57,
    RESOURCE_FORMAT_ASTC_8X6_UNORM          = 58,
    RESOURCE_FORMAT_ASTC_8X8_UNORM_SRGB     = 59,
    RESOURCE_FORMAT_ASTC_8X8_UNORM          = 60,
    RESOURCE_FORMAT_ASTC_10X5_UNORM_SRGB    = 61,
    RESOURCE_FORMAT_ASTC_10X5_UNORM         = 62,
    RESOURCE_FORMAT_ASTC_10X6_UNORM_SRGB    = 63,
    RESOURCE_FORMAT_ASTC_10X6_UNORM         = 64,
    RESOURCE_FORMAT_ASTC_10X8_UNORM_SRGB    = 65,
    RESOURCE_FORMAT_ASTC_10X8_UNORM         = 66,
    RESOURCE_FORMAT_ASTC_10X10_UNORM_SRGB   = 67,
    RESOURCE_FORMAT_ASTC_10X10_UNORM        = 68,
    RESOURCE_FORMAT_ASTC_12X10_UNORM_SRGB   = 69,
    RESOURCE_FORMAT_ASTC_12X10_UNORM        = 70,
    RESOURCE_FORMAT_ASTC_12X12_UNORM_SRGB   = 71,
    RESOURCE_FORMAT_ASTC_12X12_UNORM        = 72,
    RESOURCE_FORMAT_R10G10B10A2_UNORM       = 73,
    RESOURCE_FORMAT_R10G10B10A2_UINT        = 74,
    RESOURCE_FORMAT_R11G11B10_FLOAT         = 75,
};

enum RESOURCE_STATE
{
    RESOURCE_STATE_UNKNOWN              = 0, 
    RESOURCE_STATE_GENERAL              = 1, 
    RESOURCE_STATE_VERTEX_BUFFER        = 2, 
    RESOURCE_STATE_INDEX_BUFFER         = 3, 
    RESOURCE_STATE_CONSTANT_BUFFER      = 4, 
    RESOURCE_STATE_COLOR_WRITE          = 5, 
    RESOURCE_STATE_COLOR_READ           = 6, 
    RESOURCE_STATE_UNORDERED_ACCESS     = 7, 
    RESOURCE_STATE_DEPTH_WRITE          = 8, 
    RESOURCE_STATE_DEPTH_READ           = 9, 
    RESOURCE_STATE_SHADER_READ          = 10,
    RESOURCE_STATE_STREAM_OUT           = 11,
    RESOURCE_STATE_INDIRECT_ARGUMENT    = 12,
    RESOURCE_STATE_COPY_DST             = 13,
    RESOURCE_STATE_COPY_SRC             = 14,
    RESOURCE_STATE_RESOLVE_DST          = 15,
    RESOURCE_STATE_RESOLVE_SRC          = 16,
    RESOURCE_STATE_PRESENT              = 17,
    RESOURCE_STATE_PREDICATION          = 18,
};

enum TEXTURE_SWIZZLE
{
    TEXTURE_SWIZZLE_R       = 0, 
    TEXTURE_SWIZZLE_G       = 1, 
    TEXTURE_SWIZZLE_B       = 2, 
    TEXTURE_SWIZZLE_A       = 3, 
    TEXTURE_SWIZZLE_ZERO    = 4, 
    TEXTURE_SWIZZLE_ONE     = 5, 
};

enum HEAP_TYPE
{
    HEAP_TYPE_DEFAULT   = 0, 
    HEAP_TYPE_UPLOAD    = 1, 
    HEAP_TYPE_READBACK  = 2, 
};

enum CPU_PAGE_PROPERTY
{
    CPU_PAGE_PROPERTY_DEFAULT       = 0,
    CPU_PAGE_PROPERTY_NOT_AVAILABLE = 1,
    CPU_PAGE_PROPERTY_WRITE_COMBINE = 2,
    CPU_PAGE_PROPERTY_WRITE_BACK    = 3,
};

enum COMMANDLIST_TYPE
{
    COMMANDLIST_TYPE_DIRECT     = 0,
    COMMANDLIST_TYPE_BUNDLE     = 1,
	COMMANDLIST_TYPE_COMPUTE	= 2,
    COMMANDLIST_TYPE_COPY       = 3,
};

enum PRIMITIVE_TOPOLOGY
{
    PRIMITIVE_TOPOLOGY_POINTLIST            = 0,
    PRIMITIVE_TOPOLOGY_LINELIST             = 1,
    PRIMITIVE_TOPOLOGY_LINESTRIP            = 2,
    PRIMITIVE_TOPOLOGY_TRIANGLELIST         = 3,
    PRIMITVIE_TOPOLOGY_TRIANGLESTRIP        = 4,
    PRIMITIVE_TOPOLOGY_LINELIST_ADJ         = 5,
    PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ        = 6,
    PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ     = 7,
    PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ    = 8,
    PRIMITIVE_TOPOLOGY_PATCHLIST            = 9,
};

enum SHADER_MASK
{
    SHADER_MASK_VERTEX   = 0x1, 
    SHADER_MASK_DOMAIN   = 0x2, 
    SHADER_MASK_HULL     = 0x4, 
    SHADER_MASK_GEOMETRY = 0x8, 
    SHADER_MASK_PIXEL    = 0x10,
    SHADER_MASK_COMPUTE  = 0x20,
};

enum DESCRIPTOR_TYPE
{
	DESCRIPTOR_TYPE_CBV = 0,
	DESCRIPTOR_TYPE_SRV = 1,
	DESCRIPTOR_TYPE_UAV = 2,
	DESCRIPTOR_TYPE_SMP = 3,
	DESCRIPTOR_TYPE_RTV = 4,
	DESCRIPTOR_TYPE_DSV = 5,
};

enum POLYGON_MODE
{
    POLYGON_MODE_SOLID      = 0, 
    POLYGON_MODE_WIREFRAME  = 1, 
};

enum CULL_MODE
{
    CULL_MODE_NONE  = 0,
    CULL_MODE_FRONT = 1,
    CULL_MODE_BACK  = 2,
};

enum COMPARE_OP
{
    COMPARE_OP_NEVER    = 0,
    COMPARE_OP_LESS     = 1,
    COMPARE_OP_EQUAL    = 2,
    COMPARE_OP_LEQUAL   = 3,
    COMPARE_OP_GREATER  = 4,
    COMPARE_OP_GEQUAL   = 5,
    COMPARE_OP_ALWAYS   = 6,
};

enum STENCIL_OP
{
    STENCIL_OP_KEEP     = 0, 
    STENCIL_OP_ZERO     = 1, 
    STENCIL_OP_REPLACE  = 2, 
    STENCIL_OP_INCR_SAT = 3, 
    STENCIL_OP_DECR_SAT = 4, 
    STENCIL_OP_INVERT   = 5, 
    STENCIL_OP_INCR     = 6, 
    STENCIL_OP_DECR     = 7, 
};

enum BLEND_FACTOR
{
    BLEND_FACTOR_ZERO           = 0, 
    BLEND_FACTOR_ONE            = 1, 
    BLEND_FACTOR_SRC_COLOR      = 2, 
    BLEND_FACTOR_INV_SRC_COLOR  = 3, 
    BLEND_FACTOR_SRC_ALPHA      = 4, 
    BLEND_FACTOR_INV_SRC_ALPHA  = 5, 
    BLEND_FACTOR_DST_ALPHA      = 6, 
    BLEND_FACTOR_INV_DST_ALPHA  = 7, 
    BLEND_FACTOR_DST_COLOR      = 8, 
    BLEND_FACTOR_INV_DST_COLOR  = 9, 
    BLEND_FACTOR_SRC_ALPHA_SAT  = 10,
    BLEND_FACTOR_SRC1_COLOR     = 11,
    BLEND_FACTOR_INV_SRC1_COLOR = 12,
    BLEND_FACTOR_SRC1_ALPHA     = 13,
    BLEND_FACTOR_INV_SRC1_ALPHA = 14,
};

enum BLEND_OP
{
    BLEND_OP_ADD        = 0,
    BLEND_OP_SUB        = 1,
    BLEND_OP_REV_SUB    = 2,
    BLEND_OP_MIN        = 3,
    BLEND_OP_MAX        = 4,
};

enum LOGIC_OP
{
    LOGIC_OP_CLEAR      = 0, 
    LOGIC_OP_SET        = 1, 
    LOGIC_OP_COPY       = 2, 
    LOGIC_OP_COPY_INV   = 3, 
    LOGIC_OP_NOOP       = 4, 
    LOGIC_OP_INV        = 5, 
    LOGIC_OP_AND        = 6, 
    LOGIC_OP_NAND       = 7, 
    LOGIC_OP_OR         = 8, 
    LOGIC_OP_NOR        = 9, 
    LOGIC_OP_XOR        = 10,
    LOGIC_OP_EQUIV      = 11,
    LOGIC_OP_AND_REV    = 12,
    LOGIC_OP_AND_INV    = 13,
    LOGIC_OP_OR_REV     = 14,
    LOGIC_OP_OR_INV     = 15,
};

enum FILTER_MODE
{
    FILTER_MODE_NEAREST = 0,
    FILTER_MODE_LINEAR  = 1,
};

enum MIPMAP_MODE
{
    MIPMAP_MODE_NEAREST = 0, 
    MIPMAP_MODE_LINEAR  = 1, 
    MIPMAP_MODE_NONE    = 2, 
};

enum TEXTURE_ADDRESS_MODE
{
    TEXTURE_ADDRESS_MODE_REPEAT         = 0,
    TEXTURE_ADDRESS_MODE_MIRROR         = 1,
    TEXTURE_ADDRESS_MODE_CLAMP          = 2,
    TEXTURE_ADDRESS_MODE_BORDER         = 3,
    TEXTURE_ADDRESS_MODE_MIRROR_ONCE    = 4,
};

enum BORDER_COLOR
{
    BORDER_COLOR_TRANSPARENT_BLACK  = 0,    //!< (0, 0, 0, 0)
    BORDER_COLOR_OPAQUE_BLACK       = 1,    //!< (0, 0, 0, 1)
    BORDER_COLOR_OPAQUE_WHITE       = 2,    //!< (1, 1, 1, 1)
};

enum QUERY_TYPE
{
    QUERY_TYPE_OCCLUSION            = 0,
    QUERY_TYPE_TIMESTAMP            = 1,
    QUERY_TYPE_PIPELINE_STATISTICS  = 2,
};

enum INPUT_CLASSIFICATION
{
    INPUT_CLASSIFICATION_PER_VERTEX   = 0, 
    INPUT_CLASSIFICATION_PER_INSTANCE = 1  
};

enum TEXTURE_ASPECT
{
    TEXTURE_ASPECT_COLOR    = 0,
    TEXTURE_ASPECT_DEPTH    = 1,
    TEXTURE_ASPECT_STENCIL  = 2,
};

enum META_DATA_TYPE
{
    META_DATA_NONE          = 0, 
    META_DATA_HDR10         = 1, 
};

enum COLOR_SPACE_TYPE
{
	COLOR_SPACE_UNKNOWN = 0,        //!< δ֪
	COLOR_SPACE_SRGB,               //!< ColorSpace:RGB,   Range:0-255, Gamma:2.2,  Primaries:BT.709
	COLOR_SPACE_SCRGB,              //!< ColorSpace:RGB,   Range:0-255, Gamma:1.0,  Primaries:BT.709
	COLOR_SPACE_RGB_BT2020,         //!< ColorSpace:RGB,   Range:0-255, Gamma:2.2,  Primaries:BT.2020
	COLOR_SPACE_RGB_BT2020_PQ,      //!< ColorSpace:RGB,   Range:0-255, Gamma:2084, Primaries:BT.2020
};

//////////////////////////////////////////////////////////////////////////
// declare
//////////////////////////////////////////////////////////////////////////
class IBuffer;
class ITexture;
class ITextureView;
class IDescriptorSetLayout;
class IFrameBuffer;
class IProgram;

//////////////////////////////////////////////////////////////////////////
// structure
//////////////////////////////////////////////////////////////////////////
struct Offset2D
{
	int x, y;
};

struct Offset3D
{
	int x, y, z;
};

struct Extent2D
{
	uint32 width;
	uint32 height;
};

struct Extent3D
{
	uint32 width;
	uint32 height;
	uint32 depth;
};

struct Rect
{
	int32  x;
	int32  y;
	uint32 width;
	uint32 height;
};

struct SubresourceLayout
{
	uint64 offset;
	uint64 size;
	uint64 rowCount;
	uint64 rowPitch;
	uint64 slicePitch;
};

struct ComponentMapping
{
	TEXTURE_SWIZZLE r;
	TEXTURE_SWIZZLE g;
	TEXTURE_SWIZZLE b;
	TEXTURE_SWIZZLE a;
};

struct HeapProperty
{
	HEAP_TYPE           type;           
	CPU_PAGE_PROPERTY   cpuPageProperty;
};

struct Viewport
{
	float x;
	float y;
	float width;
	float height;
	float minDepth;
	float maxDepth;
};

union ClearColorValue
{
	float       fvalue[4];
	int         svalue[4]; 
	uint32_t    uvalue[4]; 
};

struct ClearDepthStencilValue
{
	float   depth;             
	uint8_t stencil;           
	bool    enableClearDepth;  
	bool    enableClearStencil;
};

//////////////////////////////////////////////////////////////////////////
// struct description
//////////////////////////////////////////////////////////////////////////
struct CUTE_CORE_API BufferDesc
{
	uint64			size;
	uint32			stride;
	RESOURCE_USAGE	usage;
	RESOURCE_STATE	state;
	HeapProperty	heapProperty;
};

struct CUTE_CORE_API BufferViewDesc
{
	uint64			offset;
	uint64			range;
};

struct CUTE_CORE_API TextureDesc
{
	RESOURCE_DIMENSION	dimension;
	uint32				width;
	uint32				height;
	uint32				depthOrArraySize;
	RESOURCE_FORMAT		format;
	uint32				mipLevels;
	uint32				sampleCount;
	RESOURCE_LAYOUT		layout;
	RESOURCE_USAGE		usage;
	RESOURCE_STATE		initState;
	HeapProperty		heapProperty;
};

struct CUTE_CORE_API TextureViewDesc
{
	VIEW_DIMENSION		dimension;
	RESOURCE_FORMAT		format;
	TEXTURE_ASPECT		aspect;
	uint32				mipSlice;
	uint32				mipLevels;
	uint32				firstArraySlice;
	uint32				arraySize;
	ComponentMapping	components;
};

struct CUTE_CORE_API FrameBufferDesc
{
	uint32_t			colorCount;
	ITextureView*		colorTargets[8];
	ITextureView*		depthTarget;    
};

struct CUTE_CORE_API SamplerDesc
{
	FILTER_MODE				minFilter;
	FILTER_MODE				magFilter;
	MIPMAP_MODE				mipmapMode;
	TEXTURE_ADDRESS_MODE	addressU;
	TEXTURE_ADDRESS_MODE	addressV;
	TEXTURE_ADDRESS_MODE	addressW;
	float					mipLodBias;
	bool                    anisotropyEnable;
	uint32_t                maxAnisotropy;
	bool                    compareEnable;
	COMPARE_OP              compareOp;
	float                   minLod;
	float                   maxLod;
	BORDER_COLOR            borderColor;
};

struct CUTE_CORE_API RasterizerDesc
{
	POLYGON_MODE			polygonMode;
	CULL_MODE				cullMode;
	bool					frontCounterClockWise;
	int						depthBias;
	float					depthBiasClamp;
	float					slopeScaledDepthBias;
	bool					depthClipEnable;
	bool					enableConservativeRaster;
};

struct CUTE_CORE_API StencilDesc
{
	STENCIL_OP	stencilFailOp;
	STENCIL_OP	stencilDepthFailOp;
	STENCIL_OP	stencilPassOp;
	COMPARE_OP	stencilCompareOp;
};

struct CUTE_CORE_API DepthStencilDesc
{
	bool            depthTestEnable;
	bool            depthWriteEnable;
	COMPARE_OP      depthCompareOp;
	bool            stencilTestEnable;
	uint8_t         stencllReadMask;
	uint8_t         stencilWriteMask;
	StencilDesc		frontFace;
	StencilDesc		backFace;
};

struct CUTE_CORE_API ColorBlendDesc
{
	bool			blendEnable;
	BLEND_FACTOR	srcBlend;
	BLEND_FACTOR	dstBlend;
	BLEND_OP        blendOp;
	BLEND_FACTOR    srcBlendAlpha;
	BLEND_FACTOR    dstBlendAlpha;
	BLEND_OP        blendOpAlpha;
	bool            enableWriteR;
	bool            enableWriteG;
	bool            enableWriteB;
	bool            enableWriteA;
};

struct CUTE_CORE_API BlendDesc
{
	bool		  	independentBlendEnable;
	bool		  	logicOpEnable;
	LOGIC_OP	  	logicOp;
	ColorBlendDesc	colors[8];
};

struct CUTE_CORE_API MultiSampleDesc
{
	bool            enableAlphaToCoverage;
	bool            enableMultiSample;
	uint32	        sampleCount;
};

struct CUTE_CORE_API TessellationDesc
{
	uint32			patchControlCount;
};

struct CUTE_CORE_API InputElementDesc
{
	const char*				semanticName;
	uint32					semanticIndex;
	uint32					bindLocation;
	RESOURCE_FORMAT			format;
	uint32					offsetInBytes;
};

struct CUTE_CORE_API InputStreamDesc
{
	uint32					elementCount;
	InputElementDesc*		elements;
	uint32					streamIndex;
	uint32					strideInBytes;
	INPUT_CLASSIFICATION	inputClass;
};

struct CUTE_CORE_API InputLayoutDesc
{
	uint32					streamCount;
	InputStreamDesc*		streams;
};

struct CUTE_CORE_API DescriptorEntry
{
	uint32					shaderRegister;
	uint32					shaderMask;
	uint32					bindLocation;
	DESCRIPTOR_TYPE			type;
};

struct CUTE_CORE_API DescriptorSetLayoutDesc
{
	uint32					maxCount;
	uint32					entryCount;
	DescriptorEntry			entries[64];
};

struct CUTE_CORE_API ShaderDesc
{
	SHADER_MASK				flags;
	size_t					codeSize;
	const void*				code;
};

struct CUTE_CORE_API GraphicsPipelineDesc
{
	IDescriptorSetLayout*	layout;
	IProgram*				vertex;
	IProgram*				pixel;
	IProgram*				domain;
	IProgram*				hull;
	IProgram*				geometry;
	BlendDesc				blend;
	RasterizerDesc			rasterizer;
	MultiSampleDesc			multiSample;
	DepthStencilDesc		depthStencil;
	TessellationDesc		tessellation;
	InputLayoutDesc         inputLayout;
	PRIMITIVE_TOPOLOGY      primitiveTopology;
	IFrameBuffer*           framebuffer;
};

struct CUTE_CORE_API ComputePipelineDesc
{
	IDescriptorSetLayout*	layout;
	IProgram*				compute;
};

struct CUTE_CORE_API SwapChainDesc
{
	Extent2D				extent;
	RESOURCE_FORMAT			format;
	uint32					mipLevels;
	uint32					sampleCount;
	uint32					bufferCount;
	uint32					syncInterval;
	void*					instanceHandle;
	void*					windowHandle;
	bool					fullscreen;
	COLOR_SPACE_TYPE		colorSpace;
};

struct CUTE_CORE_API MetaDataHDR10
{
	float primaryR[2];
	float primaryG[2];
	float primaryB[2];
	float whitePoint[2];
	float maxMasteringLuminance;
	float minMasteringLuminance;
	float maxContentLightLevel;
	float maxFrameAverageLightLevel;
};

struct CUTE_CORE_API DeviceInfo
{
	uint32 constantBufferMemoryAlignment;
	uint32 maxTargetWidth;                
	uint32 maxTargetHeight;               
	uint32 maxTargetArraySize;            
	uint32 maxColorSampleCount;           
	uint32 maxDepthSampleCount;           
	uint32 maxStencilSampleCount;
};

struct CUTE_CORE_API DeviceDesc
{
	uint32 maxShaderResourceCount;
	uint32 maxSamplerCount;
	uint32 maxColorTargetCount;
	uint32 maxDepthTargetCount;
	uint32 maxGraphicsQueueSubmitCount;
	uint32 maxComputeQueueSubmitCount;
	uint32 maxCopyQueueSubmitCount;
	bool   enableDebug;
};

//////////////////////////////////////////////////////////////////////////
// graphics interface
//////////////////////////////////////////////////////////////////////////
class CUTE_CORE_API ISampler : public Object
{
	DECLARE_RTTI(ISampler, Object, OID_ANY)
public:
	virtual ~ISampler(){}
};

class CUTE_CORE_API IResource : public Object
{
	DECLARE_RTTI(IResource, Object, OID_ANY)
public:
	virtual ~IResource(){}

	virtual void* map() = 0;
	virtual void  unmap() = 0;

	virtual RESOURCE_STATE getState() const = 0;
	virtual RESOURCE_KIND  getKind() const = 0;
};

class CUTE_CORE_API IBuffer : public IResource
{
	DECLARE_RTTI(IBuffer, IResource, OID_ANY)
public:
	virtual ~IBuffer(){}

	RESOURCE_KIND getKind() const { return RESOURCE_KIND_BUFFER; }
};

class CUTE_CORE_API ITexture : public IResource
{
	DECLARE_RTTI(ITexture, IResource, OID_ANY)
public:
	virtual ~ITexture(){}

	RESOURCE_KIND getKind() const { return RESOURCE_KIND_TEXTURE; }

	virtual bool getSubresourceLayout(SubresourceLayout& layout, uint32 subresource) const = 0;
};

//class CUTE_CORE_API ITextureView : public Object
//{
//	DECLARE_RTTI(ITextureView, Object, OID_ANY)
//public:
//	virtual ~ITextureView(){}
//};

class CUTE_CORE_API IFrameBuffer : public Object
{
	DECLARE_RTTI(IFrameBuffer, Object, OID_ANY)
public:
	virtual ~IFrameBuffer(){}
};

class CUTE_CORE_API ISwapChain : public Object
{
	DECLARE_RTTI(ISwapChain, Object, OID_ANY)
public:
	virtual ~ISwapChain(){}

	virtual void present() = 0;
	virtual bool resize(uint32 width, uint32 height) = 0;
	virtual bool getBuffer(uint32 index, ITexture* texture) = 0;
	virtual bool setMetaData(META_DATA_TYPE type, void* data) = 0;
	virtual void setFullScreen(bool enable) = 0;
	virtual bool isFullScreen() const = 0;
	virtual bool checkColorSpaceSupport(COLOR_SPACE_TYPE type, uint32* flags) = 0;
};

class CUTE_CORE_API IDescriptorSet : public Object
{
	DECLARE_RTTI(IDescriptorSet, Object, OID_ANY)
public:
	virtual ~IDescriptorSet(){}

	virtual void setBuffer(size_t index, IBuffer* buffer) = 0;
	virtual void setTexture(size_t index, ITextureView* texture) = 0;
	virtual void setSampler(size_t index, ISampler* sampler) = 0;

	virtual void update() {}
};

class CUTE_CORE_API IDescriptorSetLayout : public Object
{
	DECLARE_RTTI(IDescriptorSetLayout, Object, OID_ANY)
public:
	virtual ~IDescriptorSetLayout(){}
};

// gpu program
class CUTE_CORE_API IProgram : public Object
{
	DECLARE_RTTI(IProgram, Object, OID_ANY)
public:
	virtual ~IProgram(){}
};

class CUTE_CORE_API IShaderModule : public Object
{
	DECLARE_RTTI(IShaderModule, Object, OID_ANY)
public:
	virtual ~IShaderModule(){}

	virtual bool compile(const ShaderDesc& desc) = 0;
};

class CUTE_CORE_API IPipeline : public Object
{
	DECLARE_RTTI(IPipeline, Object, OID_ANY)
public:
	virtual ~IPipeline(){}
};

class CUTE_CORE_API IFence : public Object
{
	DECLARE_RTTI(IFence, Object, OID_ANY)
public:
	virtual ~IFence(){}

	virtual bool isSignaled() const = 0;
	virtual bool wait(uint32 msec) = 0;
};

class CUTE_CORE_API IQuery : public Object
{
	DECLARE_RTTI(IQuery, Object, OID_ANY)
public:
	virtual ~IQuery(){}
};

class CUTE_CORE_API ICommandSet : public Object
{
	DECLARE_RTTI(ICommandSet, Object, OID_ANY)
public:
	virtual ~ICommandSet(){}
};

// CommandBuffer
class CUTE_CORE_API ICommandList : public Object
{
	DECLARE_RTTI(ICommandList, Object, OID_ANY)
public:
	virtual ~ICommandList(){}

	virtual void begin() = 0;
	virtual void end() = 0;

	virtual void setBlendConstant(const float values[4]) = 0;
	virtual void setStencilReference(uint32 stencilRef) = 0;
	virtual void setViewports(uint32 count, Viewport* viewports) = 0;
	virtual void setScissors(uint32 count, Rect* scissors) = 0;
	virtual void setPipeline(IPipeline* pipeline) = 0;
	virtual void setDescriptorSet(IDescriptorSet* descriptors) = 0;
	virtual void setVertexBuffers(uint32 startSlot, uint32 count, IBuffer* buffers, uint64 offsets) = 0;
	virtual void setIndexBuffer(IBuffer* buffer, uint64 offset) = 0;

	virtual void setBarrier(IResource* resource, RESOURCE_STATE state) = 0;

	virtual void drawInstanced(uint32 vertexCount, uint32 instanceCount, uint32 firstVertex, uint32 firstInstance) = 0;
	virtual void drawIndexedInstanced(uint32 indexCount, uint32 instanceCount, uint32 firstIndex, int vertexOffset, uint32 firstInstance) = 0;
	virtual void dispatch(uint32 x, uint32 y, uint32 z) = 0;
	virtual void executeIndirect(ICommandSet* cmdSet, uint32 maxCount, IBuffer* argBuffer, uint64 argOffset, IBuffer* counterBuffer, uint64 counterOffset) = 0;
	virtual void executeBundle(ICommandList* cmdList) = 0;

	virtual void updateConstantBuffer(IBuffer* buffer, size_t offset, size_t size, const void* data) = 0;

	virtual void beginQuery(IQuery* query) = 0;
	virtual void endQuery(IQuery* query) = 0;
	virtual void resolveQuery() = 0;

	virtual void copyTexture(ITexture* dst, ITexture* src) = 0;
	virtual void copyBuffer(IBuffer* dst, IBuffer* src) = 0;
	virtual void copyTextureRegion(
		ITexture*		dstResource,
		uint32			dstSubresource,
		const Offset3D& dstOffset,
		ITexture*		srcResource,
		uint32			srcSubresource,
		const Offset3D& srcOffset,
		const Extent3D& srcExtent) = 0;

	virtual void copyBufferRegion(
		IBuffer*		dstBuffer,
		uint64			dstOffset,
		IBuffer*		srcBuffer,
		uint64			srcOffset,
		uint64			byteCount) = 0;

	virtual void copyBufferToTexture(
		ITexture*		dstTexture,
		uint32			dstSubresource,
		const Offset3D&	dstOffset,
		IBuffer*		srcBuffer,
		uint64			srcOffset) = 0;

	virtual void copyTextureToBuffer(
		IBuffer*		dstBuffer,
		uint64			dstOffset,
		ITexture*		srcTexture,
		uint32			srcSubresource,
		const Offset3D& srcOffset,
		const Extent3D& srcExtent) = 0;

	virtual void resolveSubresource(
		ITexture*		dstResource,
		uint32			dstSubresource,
		ITexture*		srcResource,
		uint32			srcSubresource) = 0;

	virtual void pushMarker(const char* tag) = 0;
	virtual void popMarker() = 0;
};

class CUTE_CORE_API ICommandQueue : public Object
{
	DECLARE_RTTI(ICommandQueue, Object, OID_ANY)
public:
	virtual ~ICommandQueue(){}

	virtual bool submit(ICommandList* cmdList) = 0;
	virtual void execute(IFence* fence) = 0;
	virtual void waitIdle() = 0;
};

class CUTE_CORE_API IDevice : public Object
{
	DECLARE_RTTI(IDevice, Object, OID_ANY)
public:
	virtual ~IDevice(){}

	virtual bool getGraphicsQueue(ICommandQueue** ppQueue) = 0;
	virtual bool getComputeQueue(ICommandQueue** ppQueue) = 0;
	virtual bool getCopyQueue(ICommandQueue** ppQueue) = 0;

	virtual bool newBuffer(const BufferDesc* desc, IBuffer** buffer) = 0;
	virtual bool newTexture(const TextureDesc* desc, ITexture** texture) = 0;
	virtual bool newCommandBuffer(COMMANDLIST_TYPE type, ICommandList** cmdBuffer) = 0;
	virtual bool newFrameBuffer() = 0;
	virtual bool newSwapChain(ISwapChain** swapchain) = 0;
};

class CUTE_CORE_API IDeviceFactory : public Object
{
public:
	virtual ~IDeviceFactory(){}

	virtual bool getDevice(const DeviceDesc& desc, IDevice** device) = 0;
};

CUTE_NS_END
