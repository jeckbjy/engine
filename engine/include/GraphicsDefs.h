#pragma once

CU_NS_BEGIN

#define CU_MAX_MULTIPLE_RENDER_TARGETS	8
#define CU_MAX_BOUND_VERTEX_BUFFERS		32


// 着色模式,灯光着色模式,默认SHADE_GOURAUD
enum ShadeMode CU_ENUM(uint8_t)
{
	SHADE_FLAT,		// 平面着色模式
	SHADE_GOURAUD,	// 格劳德着色模式
	SHADE_PHONG		//
};

// 填充模式,gl:polygon
enum FillMode CU_ENUM(uint8_t)
{
	FILL_MODE_SOLID,		// 实体填充,SOLID
	FILL_MODE_LINE,			// 线框填充
	FILL_MODE_POINT,		// 点填充

	FILL_MODE_COUNT
};

// 剔除模式
enum CullMode CU_ENUM(uint8_t)
{
	CULL_MODE_NONE	= 0x00,		// 不剔除
	CULL_MODE_FRONT	= 0x01,		// 剔除前边,CLOCKWISE
	CULL_MODE_BACK	= 0x02,		// 剔除背面,COUNTERCLOCKWISE
	CULL_MODE_BOTH	= 0x03,		// 全部剔除
};

// 朝向
enum FrontFace CU_ENUM(uint8_t)
{
	FRONT_FACE_COUNTER_CLOCKWISE,	// 逆时针
	FRONT_FACE_CLOCKWISE,			// 顺时针
};

// 雾化
enum FogMode
{
	FOG_NONE,
	FOG_EXP,		// fog = 1/e^(distance * density)
	FOG_EXP2,		// fog = 1/e^(distance * density)^2
	FOG_LINEAR		// linearly between the start and end distances
};

enum BlendFactor
{
	BLEND_FACTOR_ZERO,					//F(0,0,0),color(R,G,B,A)
	BLEND_FACTOR_ONE,					//F(1,1,1)
		 
	BLEND_FACTOR_SRC_COLOR,				//(Rs,Gs,Bs)
	BLEND_FACTOR_ONE_MINUS_SRC_COLOR,	//(1-Rs,1-Cs,1-Bs)
	BLEND_FACTOR_SRC_ALPHA,				//(As,As,As)
	BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,	//(1-As,1-As,1-As)

	BLEND_FACTOR_DST_COLOR,				//(Rd,Gd,Bd)
	BLEND_FACTOR_ONE_MINUS_DST_COLOR,	//(1-Rd,1-Gd,1-Bd)
	BLEND_FACTOR_DST_ALPHA,				//(Ad,Ad,Ad)
	BLEND_FACTOR_ONE_MINUS_DST_ALPHA,	//(1-Ad,1-Ad,1-Ad)

	BLEND_FACTOR_CONST_COLOR,		
	BLEND_FACTOR_ONE_MINUS_CONST_COLOR,
	BLEND_FACTOR_CONST_ALPHA,
	BLEND_FACTOR_ONE_MINUS_CONST_ALPHA,

	BLEND_FACTOR_SRC_ALPHA_SATURATE,

	BLEND_FACTOR_SRC1_COLOR,
	BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
	BLEND_FACTOR_SRC1_ALPHA,
	BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA,

	BLEND_FACTOR_COUNT
};

enum BlendOp
{
	BLEND_OP_ADD,			// c = Csrc * Fsrc + Cdst * Fdst;
	BLEND_OP_SUB,			// c = Cdst * Fdst - Csrc * Fsrc;
	BLEND_OP_REV_SUB,		// c = Csrc * Fsrc - Cdst * Fdst;
	BLEND_OP_MIN,			// c = min(Cdst, Csrc)
	BLEND_OP_MAX,	 		// c = max(Cdst, Csrc)

	BLEND_OP_COUNT
};

enum LogicOp
{
	LOGIC_OP_CLEAR = 0,
	LOGIC_OP_AND = 1,
	LOGIC_OP_AND_REVERSE = 2,
	LOGIC_OP_COPY = 3,
	LOGIC_OP_AND_INVERTED = 4,
	LOGIC_OP_NO_OP = 5,
	LOGIC_OP_XOR = 6,
	LOGIC_OP_OR = 7,
	LOGIC_OP_NOR = 8,
	LOGIC_OP_EQUIV = 9,
	LOGIC_OP_INVERT = 10,
	LOGIC_OP_OR_REVERSE = 11,
	LOGIC_OP_COPY_INVERTED = 12,
	LOGIC_OP_OR_INVERTED = 13,
	LOGIC_OP_NAND = 14,
	LOGIC_OP_SET = 15,

	LOGIC_OP_COUNT
};

enum StencilOp
{
	STENCIL_OP_KEEP,
	STENCIL_OP_ZERO,
	STENCIL_OP_REPLACE,
	STENCIL_OP_INCR_CLAMP,		// INCREMENT_AND_CLAMP:超过范围后裁剪到最大
	STENCIL_OP_DECR_CLAMP,		// DECREMENT_AND_CLAMP 
	STENCIL_OP_INVERT,			// 取反,即0xff变为0x00
	STENCIL_OP_INCR_WRAP,		// 超出范围后返回到零
	STENCIL_OP_DECR_WRAP,
	// 个数
	STENCIL_OP_COUNT
};

enum StencilFaceFlags
{
	STENCIL_FACE_FRONT	= 0x01,
	STENCIL_FACE_BACK	= 0x02,
	STENCIL_FACE_BOTH	= 0x03,
};

// 比较函数，Stencil，Depth使用,Comparison
enum CompareOp
{
	CMP_OP_NEVER,
	CMP_OP_LESS,
	CMP_OP_EQUAL,
	CMP_OP_LESS_EQUAL,
	CMP_OP_GREATER,
	CMP_OP_NOT_EQUAL,
	CMP_OP_GREAER_EQUAL,
	CMP_OP_ALWAYS,

	CMP_OP_COUNT
};

enum AddressMode
{
	ADDRESS_MODE_WRAP = 1,
	ADDRESS_MODE_MIRROR,
	ADDRESS_MODE_CLAMP,
	ADDRESS_MODE_BORDER,
	ADDRESS_MODE_MIRROR_ONCE,
};

enum FilterType
{
	FILTER_TYPE_POINT = 0,
	FILTER_TYPE_LINEAR = 1,
};

enum FilterReducitionType
{
	FILTER_REDUCTION_TYPE_STANDARD = 0,
	FILTER_REDUCTION_TYPE_COMPARISON,
	FILTER_REDUCTION_TYPE_MINIMUM,
	FILTER_REDUCTION_TYPE_MAXIMUM,
};

// 颜色掩码
enum ColorMask
{
	COLOR_MASK_RED		= 0x01,
	COLOR_MASK_GREEN	= 0x02,
	COLOR_MASK_BLUE		= 0x04,
	COLOR_MASK_ALPHA	= 0x08,
	COLOR_MASK_ALL		= 0x0f
};

// 帧缓存类型
enum FrameBufferType
{
	FBT_COLOR = 0x01,
	FBT_DEPTH = 0x02,
	FBT_STENCIL = 0x04
};

enum ClearMask
{
	CLEAR_COLOR = 0x01,
	CLEAR_DEPTH = 0x02,
	CLEAR_STENCIL = 0x04,
	CLEAR_ALL = 0x07
};

enum AttachmentType
{
	ATT_DepthStencil,
	ATT_Color,
};

// 编程语言
enum GpuLanguage
{
	LANG_HLSL,
	LANG_GLSL,
	LANG_CG
};

enum PipelineType
{
	PIPELINE_GRAPHICS = 0,
	PIPELINE_COMPUTE,
};

enum TexType
{
	TEX_1D,
	TEX_2D,
	TEX_3D,
	TEX_CUBE,
};

enum BufferUsage
{
	BU_TRANSFER_SRC		= 0x0001,
	BU_TRANSFER_SDT		= 0x0002,
	BU_UNIFORM_TEXEL	= 0x0004,
	BU_STORAGE_TEXEL	= 0x0008,
	BU_UNIFORM			= 0x0010,
	BU_STORAGE			= 0x0020,
	BU_INDEX			= 0x0040,
	BU_VERTEX			= 0x0080,
	BU_INDIRECT			= 0x0100,
};

enum TextureUsage
{
	TU_TRANSFER_SRC		= 0x0001,
	TU_TRANSFER_DST		= 0x0002,
	TU_SAMPLED			= 0x0004,
	TU_STORAGE			= 0x0008,
	TU_COLOR_ATTACHMENT	= 0x0010,
	TU_DEPTH_STENCIL	= 0x0020,
	TU_TRANSIENT		= 0x0040,
	TU_INPUT_ATTACHMENT	= 0x0080,
};

enum MAP_FLAG
{
	MAP_READ_WRITE,
	MAP_READ_ONLY,
	MAP_WRITE_ONLY,
	MAP_WRITE_ONLY_DISCARD,
	MAP_WRITE_ONLY_ON_OVERWRITE,
};

// 标识
enum RES_FLAG
{
	RES_CPU_READ = 0x01,
	RES_CPU_WRITE = 0x02,
	RES_GPU_READ = 0x04,
	RES_GPU_WRITE = 0x08,
	RES_GPU_UNORDERED = 0x10,
	RES_GPU_STRUCTURED = 0x20,
	RES_GENERATE_MIPS = 0x40,
	RES_IMMUTABLE = 0x80,
	RES_RAW = 0x100,
	RES_APPEND = 0x200,
	RES_COUNTER = 0x400,

	RES_DYNAMIC = 0x1000,
	RES_SYSTEM = 0x2000,
	RES_RENDER_TARGET = 0x4000,	// for texture
	RES_DEPTH_STENCIL = 0x8000,

	RES_INDEX16 = 0x10000,	// for index buffer
	RES_CACHE = 0x20000,	// 缓存一份			
	RES_DIRTY = 0x40000,	// 脏标识

	RES_DEFAULT = 0,		// 默认值
};

// 不支持Index8
enum IndexType
{
	INDEX16,
	INDEX32,
};

// PrimitiveTopology 拓扑结构,详细说明：http://shiba.hpe.sh.cn/jiaoyanzu/wuli/ShowArticle.aspx?articleId=1518&classId=4
enum Topology
{
	PT_UNDEFINED = 0,
	PT_POINT_LIST,
	PT_LINE_LIST,
	PT_LINE_STRIP,
	PT_TRIANGLE_LIST,
	PT_TRIANGLE_STRIP,
	PT_LINE_LIST_ADJ,
	PT_LINE_STRIP_ADJ,
	PT_TRIANGLE_LIST_ADJ,
	PT_TRIANGLE_STRIP_ADJ,
	// 控制点
	PT_CTRL_PATCH_LIST1,
	PT_CTRL_PATCH_LIST2,
	PT_CTRL_PATCH_LIST3,
	PT_CTRL_PATCH_LIST4,
	PT_CTRL_PATCH_LIST5,
	PT_CTRL_PATCH_LIST6,
	PT_CTRL_PATCH_LIST7,
	PT_CTRL_PATCH_LIST8,
	PT_CTRL_PATCH_LIST9,
	PT_CTRL_PATCH_LIST10,
	PT_CTRL_PATCH_LIST11,
	PT_CTRL_PATCH_LIST12,
	PT_CTRL_PATCH_LIST13,
	PT_CTRL_PATCH_LIST14,
	PT_CTRL_PATCH_LIST15,
	PT_CTRL_PATCH_LIST16,
	PT_CTRL_PATCH_LIST17,
	PT_CTRL_PATCH_LIST18,
	PT_CTRL_PATCH_LIST19,
	PT_CTRL_PATCH_LIST20,
	PT_CTRL_PATCH_LIST21,
	PT_CTRL_PATCH_LIST22,
	PT_CTRL_PATCH_LIST23,
	PT_CTRL_PATCH_LIST24,
	PT_CTRL_PATCH_LIST25,
	PT_CTRL_PATCH_LIST26,
	PT_CTRL_PATCH_LIST27,
	PT_CTRL_PATCH_LIST28,
	PT_CTRL_PATCH_LIST29,
	PT_CTRL_PATCH_LIST30,
	PT_CTRL_PATCH_LIST31,
	PT_CTRL_PATCH_LIST32,
};

// shader类型
enum ShaderType
{
	SHADER_VERTEX,
	SHADER_HULL,		// TESSELLATION_CONTROL
	SHADER_DOMAIN,		// TESSELLATION_EVALUATION
	SHADER_GEOMETRY,
	SHADER_PIXEL,		// fragment
	SHADER_COMPUTE,
	SHADER_COUNT,
};

enum ShaderTypeBit
{
	SHADER_BIT_VERTEX	= 0x01,
	SHADER_BIT_HULL		= 0x02,	// TESSELLATION_CONTROL
	SHADER_BIT_DOMAIN	= 0x04,	// TESSELLATION_EVALUATION
	SHADER_BIT_GEOMETRY = 0x08,
	SHADER_BIT_PIXEL	= 0x10,	// fragment
	SHADER_BIT_COMPUTE	= 0x20,
	SHADER_BIT_ALL		= 0x1F,
};

enum ShaderProfile
{
	SP_NONE,
	SP_PS_1_1,
	SP_PS_1_2,
	SP_PS_1_3,
	SP_PS_1_4,
	SP_PS_2_0,
	SP_PS_2_x,
	SP_PS_2_a,
	SP_PS_2_b,
	SP_PS_3_0,
	SP_PS_3_x,
	SP_PS_4_0,
	SP_PS_4_1,
	SP_PS_5_0,
	SP_VS_1_1,
	SP_VS_2_0,
	SP_VS_2_x,
	SP_VS_2_a,
	SP_VS_3_0,
	SP_VS_4_0,
	SP_VS_4_1,
	SP_VS_5_0,
	SP_GS_4_0,
	SP_GS_4_1,
	SP_GS_5_0,
	SP_HS_5_0,
	SP_DS_5_0,
	SP_CS_5_0
};

// Semantic
enum Semantic
{
	SEMANTIC_POSITION,
	SEMANTIC_NORMAL,
	SEMANTIC_COLOR,
	SEMANTIC_TANGENT,
	SEMANTIC_BINORMAL,
	SEMANTIC_BLENDWEIGHTS,
	SEMANTIC_BLENDINDICES,
	SEMANTIC_TEXCOORD0,
	SEMANTIC_TEXCOORD1,
	SEMANTIC_TEXCOORD2,
	SEMANTIC_TEXCOORD3,
	SEMANTIC_TEXCOORD4,
	SEMANTIC_TEXCOORD5,
	SEMANTIC_TEXCOORD6,
	SEMANTIC_TEXCOORD7,

	SEMANTIC_MAX,
	TEXCOORD_MAX = 8
};

enum InputRate
{
	INPUT_RATE_VERTEX = 0,
	INPUT_RATE_INSTANCE = 1,
};

enum UniformType
{
	UT_UNKNOWN,
	UT_COLOR,		// Same as FLOAT4, but can be used to better deduce usage. 
	UT_BOOL,
	UT_INT1,
	UT_INT2,
	UT_INT3,
	UT_INT4,
	UT_HALF1,
	UT_HALF2,
	UT_HALF3,
	UT_HALF4,
	UT_FLOAT1,
	UT_FLOAT2,
	UT_FLOAT3,
	UT_FLOAT4,
	UT_MATRIX_2X2,
	UT_MATRIX_2X3,
	UT_MATRIX_2X4,
	UT_MATRIX_3X2,
	UT_MATRIX_3X3,
	UT_MATRIX_3X4,
	UT_MATRIX_4X2,
	UT_MATRIX_4X3,
	UT_MATRIX_4X4,
	UT_STRUCT,
	UT_CBUFFER,		// ConstantBuffer
	UT_SAMPLER1D,
	UT_SAMPLER2D,
	UT_SAMPLER3D,
	UT_SAMPLERCUBE,
	UT_SAMPLER2DMS,
	UT_TEXTURE1D,
	UT_TEXTURE2D,
	UT_TEXTURE3D,
	UT_TEXTURECUBE,
	UT_TEXTURE2DMS,
	UT_BYTE_BUFFER,
	UT_STRUCTURED_BUFFER,
	UT_RWTYPED_BUFFER,
	UT_RWTEXTURE1D,
	UT_RWTEXTURE2D,
	UT_RWTEXTURE3D,
	UT_RWTEXTURE2DMS,
	UT_RWBYTE_BUFFER,
	UT_RWSTRUCTURED_BUFFER,
	UT_RWSTRUCTURED_BUFFER_WITH_COUNTER,
	UT_RWAPPEND_BUFFER,
	UT_RWCONSUME_BUFFER,
};

CU_NS_END
