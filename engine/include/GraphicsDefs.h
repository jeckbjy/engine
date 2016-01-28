#pragma once
#include "Object.h"

CU_NS_BEGIN

#define CU_MAX_MULTIPLE_RENDER_TARGETS 8

// ���ģʽ,gl:polygon
enum FillMode CU_ENUM(uint8_t)
{
	FILL_POINT,		// �����
	FILL_LINE,		// �߿����
	FILL_SOLID,		// ʵ�����,SOLID

	FILL_MODE_COUNT
};

// ��ɫģʽ,�ƹ���ɫģʽ,Ĭ��SHADE_GOURAUD
enum ShadeMode CU_ENUM(uint8_t)
{
	SHADE_FLAT,		// ƽ����ɫģʽ
	SHADE_GOURAUD,	// ���͵���ɫģʽ
	SHADE_PHONG		//
};

// �޳�ģʽ
enum CullMode CU_ENUM(uint8_t)
{
	CULL_NONE,		// ���޳�
	CULL_FRONT,		// �޳�ǰ��,CLOCKWISE
	CULL_BACK,		// �޳�����,COUNTERCLOCKWISE

	CULL_MODE_COUNT
};

// ����
enum FrontFace CU_ENUM(uint8_t)
{
	FRONT_FACE_CW,	// ˳ʱ��
	FRONT_FACE_CCW,	// ��ʱ��
};

// ����
enum FogMode
{
	FOG_NONE,
	FOG_EXP,		// fog = 1/e^(distance * density)
	FOG_EXP2,		// fog = 1/e^(distance * density)^2
	FOG_LINEAR		// linearly between the start and end distances
};

enum BlendFactor
{
	BLEND_ZERO,				//F(0,0,0),color(R,G,B,A)
	BLEND_ONE,				//F(1,1,1)

	BLEND_SRC_COLOR,		//(Rs,Gs,Bs)
	BLEND_SRC_ALPHA,		//(As,As,As)
	BLEND_DST_COLOR,		//(Rd,Gd,Bd)
	BLEND_DST_ALPHA,		//(Ad,Ad,Ad)

	BLEND_INV_SRC_COLOR,	//(1-Rs,1-Cs,1-Bs)
	BLEND_INV_SRC_ALPHA,	//(1-As,1-As,1-As)
	BLEND_INV_DST_COLOR,	//(1-Rd,1-Gd,1-Bd)
	BLEND_INV_DEST_ALPHA,	//(1-Ad,1-Ad,1-Ad)

	BLEND_FACTOR,			//����ָ��
	//BLEND_INV_FACTOR,		//ָ��ȡ��1-color

	BLEND_FACTOR_COUNT
};

enum BlendOperation
{
	BLEND_OP_ADD,			// c = Csrc * Fsrc + Cdst * Fdst;
	BLEND_OP_SUB,			// c = Cdst * Fdst - Csrc * Fsrc;
	BLEND_OP_REV_SUB,		// c = Csrc * Fsrc - Cdst * Fdst;
	BLEND_OP_MIN,			// c = min(Cdst, Csrc)
	BLEND_OP_MAX,	 		// c = max(Cdst, Csrc)

	BLEND_OP_COUNT
};

enum LogicOperation
{
	LOP_CLEAR,
	LOP_SET,
	LOP_COPY,
	LOP_COPY_INVERTED,
	LOP_NOOP,
	LOP_INVERT,
	LOP_AND,
	LOP_NAND,
	LOP_OR,
	LOP_NOR,
	LOP_XOR,
	LOP_EQUIV,
	LOP_AND_REVERSE,
	LOP_AND_INVERTED,
	LOP_OR_REVERSE,
	LOP_OR_INVERTED,

	LOGIC_OP_COUNT
};

/**
* Defines the supported stencil operations to perform.
*
* Stencil operations determine what should happen to the pixel if the
* stencil test fails, passes, or passes but fails the depth test.
*
* The initial stencil operation is STENCIL_OP_KEEP.
*/
enum StencilOp
{
	STENCIL_OP_KEEP,
	STENCIL_OP_ZERO,
	STENCIL_OP_REPLACE,
	STENCIL_OP_INVERT,		// ȡ��,��0xff��Ϊ0x00
	STENCIL_OP_INCR,		// ������Χ��ü������
	STENCIL_OP_DECR,
	STENCIL_OP_INCR_WRAP,	// ������Χ�󷵻ص���
	STENCIL_OP_DECR_WRAP,
	// ����
	STENCIL_OP_COUNT
};

// �ȽϺ�����Stencil��Depthʹ��,Comparison
enum CmpFun
{
	CMP_ALWAYS_FAIL,
	CMP_ALWAYS_PASS,
	CMP_LESS,
	CMP_LESS_EQUAL,
	CMP_EQUAL,
	CMP_NOT_EQUAL,
	CMP_GREAER_EQUAL,
	CMP_GREATER,

	COMPARISON_FUN_COUNT
};

// ����Ѱַģʽ
enum TexAddrMode
{
	TEX_ADDR_WRAP,
	TEX_ADDR_MIRROR,		// ����
	TEX_ADDR_CLAMP,			// 
	TEX_ADDR_BORDER,
	//TEX_ADDR_MIRROR_ONCE,

	TEX_ADDR_COUNT
};

enum TexFilterOp
{
	BIT_POINT = 0x00,
	BIT_MIP_LINEAR = 0x01,
	BIT_MAG_LINEAR = 0x02,
	BIT_MIN_LINEAR = 0x04,
	BIT_ANISOTROPIC = 0x08,
	BIT_COMPARISON = 0x10,

	// ���
	TEX_MIN_MAG_MIP_POINT = BIT_POINT,
	TEX_MIN_MAG_POINT_MIP_LINEAR = BIT_MIP_LINEAR,
	TEX_MIN_POINT_MAG_LINEAR_MIP_POINT = BIT_MAG_LINEAR,
	TEX_MIN_POINT_MAG_MIP_LINEAR = BIT_MAG_LINEAR | BIT_MIP_LINEAR,
	TEX_MIN_LINEAR_MAG_MIP_POINT = BIT_MIN_LINEAR,
	TEX_MIN_LINEAR_MAG_POINT_MIP_LINEAR = BIT_MIN_LINEAR | BIT_MIP_LINEAR,
	TEX_MIN_MAG_LINEAR_MIP_POINT = BIT_MIN_LINEAR | BIT_MAG_LINEAR,
	TEX_MIN_MAG_MIP_LINEAR = BIT_MIN_LINEAR | BIT_MAG_LINEAR | BIT_MIP_LINEAR,
	TEX_ANISOTROPIC = BIT_ANISOTROPIC,
	// CMP
	TEX_CMP_MIN_MAG_MIP_POINT = BIT_COMPARISON | TEX_MIN_MAG_MIP_POINT,
	TEX_CMP_MIN_MAG_POINT_MIP_LINEAR = BIT_COMPARISON | TEX_MIN_MAG_POINT_MIP_LINEAR,
	TEX_CMP_MIN_POINT_MAG_LINEAR_MIP_POINT = BIT_COMPARISON | TEX_MIN_POINT_MAG_LINEAR_MIP_POINT,
	TEX_CMP_MIN_POINT_MAG_MIP_LINEAR = BIT_COMPARISON | TEX_MIN_POINT_MAG_MIP_LINEAR,
	TEX_CMP_MIN_LINEAR_MAG_MIP_POINT = BIT_COMPARISON | TEX_MIN_LINEAR_MAG_MIP_POINT,
	TEX_CMP_MIN_LINEAR_MAG_POINT_MIP_LINEAR = BIT_COMPARISON | TEX_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	TEX_CMP_MIN_MAG_LINEAR_MIP_POINT = BIT_COMPARISON | TEX_MIN_MAG_LINEAR_MIP_POINT,
	TEX_CMP_MIN_MAG_MIP_LINEAR = BIT_COMPARISON | TEX_MIN_MAG_MIP_LINEAR,
	TEX_CMP_ANISOTROPIC = BIT_COMPARISON | TEX_ANISOTROPIC
};

// ��ɫ����
enum ColorMask
{
	COLOR_MASK_RED = 0x01,
	COLOR_MASK_GREEN = 0x02,
	COLOR_MASK_BLUE = 0x04,
	COLOR_MASK_ALPHA = 0x08,
	COLOR_MASK_ALL = 0x0f
};

// ֡��������
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

// �������
enum GpuLanguage
{
	LANG_HLSL,
	LANG_GLSL,
	LANG_CG
};

enum TexType
{
	TEX_1D,
	TEX_2D,
	TEX_3D,
	TEX_CUBE,
};

// �󶨱�ʶ
enum BIND_FLAG
{
	BIND_VERTEX_BUFFER = 0x01,
	BIND_INDEX_BUFFER = 0x02,
	BIND_UNIFORM_BUFFER = 0x04,
	BIND_SHADER_RESOURCE = 0x08,
	BIND_STREAM_OUTPUT = 0x10,
	BIND_RENDER_TARGET = 0x20,
	BIND_DEPTH_STENCIL = 0x40,
	BIND_UNORDERED_ACCESS = 0x80,
	//BIND_DECODER = 0x200L,
	//BIND_VIDEO_ENCODER = 0x400L,
};

enum MAP_FLAG
{
	MAP_READ_WRITE,
	MAP_READ_ONLY,
	MAP_WRITE_ONLY,
	MAP_WRITE_ONLY_DISCARD,
	MAP_WRITE_ONLY_ON_OVERWRITE,
};

// ��ʶ
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
	RES_CACHE = 0x20000,	// ����һ��			
	RES_DIRTY = 0x40000,	// ���ʶ

	RES_DEFAULT = 0,		// Ĭ��ֵ
};

// ��֧��Index8
enum IndexType
{
	INDEX16,
	INDEX32,
};

// PrimitiveTopology ���˽ṹ,��ϸ˵����http://shiba.hpe.sh.cn/jiaoyanzu/wuli/ShowArticle.aspx?articleId=1518&classId=4
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
	// ���Ƶ�
	PT_CTRL_PT_PATCH_LIST1,
	PT_CTRL_PT_PATCH_LIST2,
	PT_CTRL_PT_PATCH_LIST3,
	PT_CTRL_PT_PATCH_LIST4,
	PT_CTRL_PT_PATCH_LIST5,
	PT_CTRL_PT_PATCH_LIST6,
	PT_CTRL_PT_PATCH_LIST7,
	PT_CTRL_PT_PATCH_LIST8,
	PT_CTRL_PT_PATCH_LIST9,
	PT_CTRL_PT_PATCH_LIST10,
	PT_CTRL_PT_PATCH_LIST11,
	PT_CTRL_PT_PATCH_LIST12,
	PT_CTRL_PT_PATCH_LIST13,
	PT_CTRL_PT_PATCH_LIST14,
	PT_CTRL_PT_PATCH_LIST15,
	PT_CTRL_PT_PATCH_LIST16,
	PT_CTRL_PT_PATCH_LIST17,
	PT_CTRL_PT_PATCH_LIST18,
	PT_CTRL_PT_PATCH_LIST19,
	PT_CTRL_PT_PATCH_LIST20,
	PT_CTRL_PT_PATCH_LIST21,
	PT_CTRL_PT_PATCH_LIST22,
	PT_CTRL_PT_PATCH_LIST23,
	PT_CTRL_PT_PATCH_LIST24,
	PT_CTRL_PT_PATCH_LIST25,
	PT_CTRL_PT_PATCH_LIST26,
	PT_CTRL_PT_PATCH_LIST27,
	PT_CTRL_PT_PATCH_LIST28,
	PT_CTRL_PT_PATCH_LIST29,
	PT_CTRL_PT_PATCH_LIST30,
	PT_CTRL_PT_PATCH_LIST31,
	PT_CTRL_PT_PATCH_LIST32,
};

// shader����
enum ShaderType
{
	ST_VERTEX,
	ST_PIXEL,	// ST_FRAGMENT
	ST_GEOMETRY,
	ST_HULL,
	ST_DOMAIN,
	ST_COMPUTE,

	SHADER_TYPE_MAX,
	ST_UNKNOWN,
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
enum VertexUsage
{
	VU_POSITION,
	VU_NORMAL,
	VU_COLOR,
	VU_TANGENT,
	VU_BINORMAL,
	VU_BLENDWEIGHTS,
	VU_BLENDINDICES,
	VU_TEXCOORD0,
	VU_TEXCOORD1,
	VU_TEXCOORD2,
	VU_TEXCOORD3,
	VU_TEXCOORD4,
	VU_TEXCOORD5,
	VU_TEXCOORD6,
	VU_TEXCOORD7,

	VERTEX_USAGE_MAX,
	VU_TEXCOORD_MAX = 8,
};

CU_NS_END