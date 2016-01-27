#pragma once
#include "Color.h"
#include "PixelFormat.h"
#include "GraphicsDefs.h"

CU_NS_BEGIN

struct CU_API BUFFER_DESC
{
	BIND_FLAG type;
	uint32_t  counts;	// 顶点个数
	uint32_t  stride;	// 顶点跨度
	RES_FLAG  flags;	// 标识信息
	const void*	data;	// 数据
};

struct CU_API TEXTURE_DESC
{
	TexType		type;
	RES_FLAG		usage;
	PixelFormat format;
	uint32_t	width;
	uint32_t	height;
	uint32_t	depth;	// or array size
	uint32_t	mipmaps;
	uint32_t	samples;
	uint32_t	quality;
	void*		data;
};

struct CU_API RASTERIZER_DESC
{
	FillMode	fill_mode;
	CullMode	cull_mode;
	ShadeMode	shade_mode;
	float		polygon_offset_factor;
	float		polygon_offset_units;
	uint8_t		front_face_ccw : 1;
	uint8_t		depth_clip_enable : 1;
	uint8_t		scissor_enable : 1;
	uint8_t		mutisample_enable : 1;
};

struct CU_API SAMPLER_DESC
{
	Color		border;
	TexAddrMode	addr_u;
	TexAddrMode	addr_v;
	TexAddrMode	addr_w;
	TexFilterOp	filter;
	uint8_t		max_anisotropy;	// 各向异性
	float		min_lod;
	float		max_lod;
	float		mip_map_lod_bias;
	CmpFun		cmp_fun;
};

struct CU_API DEPTH_STENCILOP_DESC
{
	StencilOp	stencil_fail;
	StencilOp	stencil_depth_fail;
	StencilOp	stencil_pass;
	CmpFun		stencil_fun;
};

struct CU_API DEPTH_STENCIL_DESC
{
	uint8_t		stencil_enable : 1;
	uint8_t		depth_enable : 1;
	uint8_t		depth_write_mask : 1;
	CmpFun		depth_func;

	uint8_t		stencil_read_mask;
	uint8_t		stencil_write_mask;

	DEPTH_STENCILOP_DESC front;
	DEPTH_STENCILOP_DESC back;
};

struct CU_API RENDER_TARGET_BLEND_DESC
{
	uint8_t			blend_enable : 1;
	uint8_t			logic_enable : 1;

	BlendOperation	blend_op;
	BlendFactor		blend_src;
	BlendFactor		blend_des;
	BlendOperation	blend_op_alpha;
	BlendFactor		blend_src_alpha;
	BlendFactor		blend_des_alpha;

	LogicOperation	logic_op;
	uint8_t			color_write_mask;
};

struct CU_API BLEND_DESC
{
	uint8_t	alpha_to_converage_enable : 1;
	uint8_t	independent_blend_enable : 1;
	RENDER_TARGET_BLEND_DESC targets[8];
};

//
struct CU_API PIPELINE_DESC
{

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
