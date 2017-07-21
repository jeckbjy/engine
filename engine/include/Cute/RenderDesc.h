//! Render
#pragma once
#include "Cute/RenderDefs.h"

CUTE_NS_BEGIN

class IBuffer;
class ITexture;
class ITextureView;
class IDescriptorSetLayout;
class IFrameBuffer;
class IProgram;

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

enum VertexUsage
{
    VU_POSITION,
    VU_BLENDWEIGHTS,
    VU_BLENDINDICES,
    VU_NORMAL,
    VU_COLOR,
    VU_TEXCOORD,
    VU_BITANGENT,
    VU_TANGENT,
    VU_POSITIONT,       // position transformed
    VU_PSIZE,           // point size

    VERTEX_USAGE_COUNT,
};

enum VertexFormat
{
    VF_FLOAT1       = 0,
    VF_FLOAT2       = 1,
    VF_FLOAT3       = 2,
    VF_FLOAT4       = 3,
    VF_SHORT1       = 4,
    VF_SHORT2       = 5,
    VF_SHORT4       = 6,
    VF_INT1         = 7,
    VF_INT2         = 8,
    VF_INT3         = 9,
    VF_INT4         =10,
    VF_USHORT1      =11,
    VF_USHORT2      =12,
    VF_USHORT4      =13,
    VF_UINT1        =14,
    VF_UINT2        =15,
    VF_UINT3        =16,
    VF_UINT4        =17,
    VF_UBYTE4       =18,
    VF_UBYTE4_NORM  =19,
    VF_COLOR        =20,
    VF_COLOR_ARGB   =21,
    VF_COLOR_ABGR   =22,
    
    VERTEX_FORMAT_COUNT,
};

struct CUTE_CORE_API VertexElement
{
    uint8 stream;
    uint8 offset;
    uint8 type;
    uint8 usage;
    uint8 index;            // usage index
    uint8 instanceStepRate;
    //    uint semantic;
//    uint type;
//    uint slot;
//    uint offset;
//    uint stride;
//    VertexElement(){}
//    VertexElement(){}
};

struct CUTE_CORE_API VertexDeclaration
{
    
};

CUTE_NS_END
