#include "OGL_Mapping.h"

CUTE_NS_BEGIN

//
#define GL_R32		GL_R32I
#define GL_RG32		GL_RG32I
#define GL_RGB32	GL_RGB32I
#define GL_RGBA32	GL_RGBA32I

struct GLPixelInfo
{
	GLenum gtype;
	GLenum gformat;
	GLint  ginternal;
};

static GLPixelInfo gl_infos[] = {
};

//static GLPixelInfo gl_infos[];

//inline void set_info(PixelFormat fmt, GLenum gl_type, GLenum gl_format, GLint gl_internal)
//{
//	GLPixelInfo& info = gl_infos[fmt];
//	info.gtype = gl_type;
//	info.gformat = gl_format;
//	info.ginternal = gl_internal;
//}
//
//CU_C_CTOR(pixel_init_fun)
////void pixel_init_fun()
//{
//	set_info(PF_UNKNOWN, 0, 0, 0);
//	set_info(PF_UINT_R8,		GL_UNSIGNED_INT, GL_RED, GL_R8UI);
//	set_info(PF_UINT_RG8,		GL_UNSIGNED_INT, GL_RG, GL_RG8UI);
//	set_info(PF_UINT_RGB8,		GL_UNSIGNED_INT, GL_RGB, GL_RGB8UI);
//	set_info(PF_UINT_RGBA8,		GL_UNSIGNED_INT, GL_RGBA, GL_RGBA8UI);
//	set_info(PF_UINT_R16,		GL_UNSIGNED_INT, GL_RED, GL_R16UI);
//	set_info(PF_UINT_RG16,		GL_UNSIGNED_INT, GL_RG, GL_RG16UI);
//	set_info(PF_UINT_RGB16,		GL_UNSIGNED_INT, GL_RGB, GL_RGB16UI);
//	set_info(PF_UINT_RGBA16,	GL_UNSIGNED_INT, GL_RGBA, GL_RGBA16UI);
//	set_info(PF_UINT_R32,		GL_UNSIGNED_INT, GL_RED, GL_R32UI);
//	set_info(PF_UINT_RG32,		GL_UNSIGNED_INT, GL_RG, GL_RG32UI);
//	set_info(PF_UINT_RGB32,		GL_UNSIGNED_INT, GL_RGB, GL_RGB32UI);
//	set_info(PF_UINT_RGBA32,	GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//
//	set_info(PF_SINT_R8,		GL_INT, GL_RED, GL_R8I);
//	set_info(PF_SINT_RG8,		GL_INT, GL_RG, GL_RG8I);
//	set_info(PF_SINT_RGB8,		GL_INT, GL_RGB, GL_RGB8I);
//	set_info(PF_SINT_RGBA8,		GL_INT, GL_RGBA, GL_RGBA8I);
//	set_info(PF_SINT_R16,		GL_INT, GL_RED, GL_R16I);
//	set_info(PF_SINT_RG16,		GL_INT, GL_RG, GL_RG16I);
//	set_info(PF_SINT_RGB16,		GL_INT, GL_RGB, GL_RGB16I);
//	set_info(PF_SINT_RGBA16,	GL_INT, GL_RGBA, GL_RGBA16I);
//	set_info(PF_SINT_R32,		GL_INT, GL_RED, GL_R32I);
//	set_info(PF_SINT_RG32,		GL_INT, GL_RG, GL_RG32I);
//	set_info(PF_SINT_RGB32,		GL_INT, GL_RGB, GL_RGB32I);
//	set_info(PF_SINT_RGBA32,	GL_INT, GL_RGBA, GL_RGBA32I);
//
//	set_info(PF_SNORM_R8,		GL_INT, GL_RED, GL_R8_SNORM);
//	set_info(PF_SNORM_RG8,		GL_INT, GL_RG, GL_RG8_SNORM);
//	set_info(PF_SNORM_RGB8,		GL_INT, GL_RGB, GL_RGB8_SNORM);
//	set_info(PF_SNORM_RGBA8,	GL_INT, GL_RGBA, GL_RGBA8_SNORM);
//	set_info(PF_SNORM_R16,		GL_INT, GL_RED, GL_R16_SNORM);
//	set_info(PF_SNORM_RG16,		GL_INT, GL_RG, GL_RG16_SNORM);
//	set_info(PF_SNORM_RGB16,	GL_INT, GL_RGB, GL_RGB16_SNORM);
//	set_info(PF_SNORM_RGBA16,	GL_INT, GL_RGBA, GL_RGBA16_SNORM);
//
//	set_info(PF_FLOAT_R16,		GL_HALF_FLOAT, GL_RED, GL_R16F);
//	set_info(PF_FLOAT_GR16,		GL_HALF_FLOAT, GL_RG, GL_RG16F);
//	set_info(PF_FLOAT_RGB16,	GL_HALF_FLOAT, GL_RGB, GL_RGB16F);
//	set_info(PF_FLOAT_RGBA16,	GL_HALF_FLOAT, GL_RGBA, GL_RGBA16F);
//	set_info(PF_FLOAT_R32,		GL_FLOAT, GL_RED, GL_R32F);
//	set_info(PF_FLOAT_GR32,		GL_FLOAT, GL_RG, GL_RG32F);
//	set_info(PF_FLOAT_RGB32,	GL_FLOAT, GL_RGB, GL_RGB32F);
//	set_info(PF_FLOAT_RGBA32,	GL_FLOAT, GL_RGBA, GL_RGBA32F);
//	set_info(PF_FLOAT_R11G11B10,GL_UNSIGNED_INT_10F_11F_11F_REV, GL_RGB, GL_R11F_G11F_B10F);
//
//#if CU_ENDIAN == CU_ENDIAN_BIG
//	set_info(PF_R8G8B8,		GL_UNSIGNED_BYTE, GL_RGB, GL_RGB8);
//	set_info(PF_B8G8R8,		GL_UNSIGNED_BYTE, GL_BGR, GL_RGB8);
//	set_info(PF_R8G8B8A8,	GL_UNSIGNED_BYTE, GL_BGRA, GL_RGBA8);
//	set_info(PF_B8G8R8A8,	GL_UNSIGNED_BYTE, GL_RGBA, GL_RGBA8);
//	set_info(PF_A8R8G8B8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, GL_RGBA8);
//	set_info(PF_A8B8G8R8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, GL_RGBA8);
//	set_info(PF_X8R8G8B8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, GL_RGB8);
//	set_info(PF_X8B8G8R8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, GL_RGB8);
//
//	set_info(PF_R8G8B8_SRGB,	GL_UNSIGNED_BYTE, GL_RGB, GL_SRGB8);
//	set_info(PF_B8G8R8_SRGB,	GL_UNSIGNED_BYTE, GL_BGR, GL_SRGB8);
//	set_info(PF_R8G8B8A8_SRGB,	GL_UNSIGNED_BYTE, GL_BGRA, GL_SRGB8_ALPHA8);
//	set_info(PF_B8G8R8A8_SRGB,	GL_UNSIGNED_BYTE, GL_RGBA, GL_SRGB8_ALPHA8);
//	set_info(PF_A8R8G8B8_SRGB,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, GL_SRGB8_ALPHA8);
//	set_info(PF_A8B8G8R8_SRGB,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, GL_SRGB8_ALPHA8);
//	set_info(PF_X8R8G8B8_SRGB,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, GL_SRGB8);
//	set_info(PF_X8B8G8R8_SRGB,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, GL_SRGB8);
//#else
//	set_info(PF_R8G8B8,		GL_UNSIGNED_BYTE, GL_BGR, GL_RGB8);
//	set_info(PF_B8G8R8,		GL_UNSIGNED_BYTE, GL_RGB, GL_RGB8);
//	set_info(PF_R8G8B8A8,	GL_UNSIGNED_BYTE, GL_BGRA, GL_RGBA8);
//	set_info(PF_B8G8R8A8,	GL_UNSIGNED_BYTE, GL_RGBA, GL_RGBA8);
//	set_info(PF_A8R8G8B8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, GL_RGBA8);
//	set_info(PF_A8B8G8R8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, GL_RGBA8);
//	set_info(PF_X8R8G8B8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, GL_RGB8);
//	set_info(PF_X8B8G8R8,	GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, GL_RGB8);
//
//	set_info(PF_R8G8B8_SRGB,	GL_UNSIGNED_BYTE, GL_BGR, GL_SRGB8);
//	set_info(PF_B8G8R8_SRGB,	GL_UNSIGNED_BYTE, GL_RGB, GL_SRGB8);
//	set_info(PF_R8G8B8A8_SRGB, GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, GL_SRGB8_ALPHA8);
//	set_info(PF_B8G8R8A8_SRGB, GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, GL_SRGB8_ALPHA8);
//	set_info(PF_A8R8G8B8_SRGB, GL_UNSIGNED_BYTE, GL_BGRA, GL_SRGB8_ALPHA8);
//	set_info(PF_A8B8G8R8_SRGB, GL_UNSIGNED_BYTE, GL_RGBA, GL_SRGB8_ALPHA8);
//	set_info(PF_X8R8G8B8_SRGB, GL_UNSIGNED_BYTE, GL_BGRA, GL_SRGB8);
//	set_info(PF_X8B8G8R8_SRGB, GL_UNSIGNED_BYTE, GL_RGBA, GL_SRGB8);
//#endif
//
//	set_info(PF_D32_FLOAT_S8X24_UINT, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 0, GL_DEPTH32F_STENCIL8);
//	set_info(PF_D24_UNORM_S8_UINT, GL_UNSIGNED_INT_24_8, 0, GL_DEPTH24_STENCIL8);
//	set_info(PF_D32_FLOAT, GL_FLOAT, 0, GL_DEPTH_COMPONENT32F);
//	set_info(PF_D16_UNORM, GL_UNSIGNED_SHORT, 0, GL_DEPTH_COMPONENT16);
//	// bc
//	set_info(PF_BC1_UNORM, GL_UNSIGNED_INT, GL_RGBA, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT);
//	set_info(PF_BC2_UNORM, GL_UNSIGNED_INT, GL_RGBA, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
//	set_info(PF_BC3_UNORM, GL_UNSIGNED_INT, GL_RGBA, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
//	set_info(PF_BC4_UNORM, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC4_SNORM, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC5_UNORM, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC5_SNORM, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC6H_UF16, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC6H_SF16, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC7_UNORM, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//
//	set_info(PF_BC1_SRGB, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC2_SRGB, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC3_SRGB, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC4_SRGB, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC5_SRGB, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_BC7_SRGB, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//
//	//
//	set_info(PF_ETC1_RGB8, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_ETC2_RGB8, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_ETC2_RGBA8, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_ETC2_RGB8A1, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_ATC_RGB, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_ATC_RGBA_EXPLICIT_ALPHA, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_ATC_RGBA_INTERPOLATED_ALPHA, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//
//	set_info(PF_PVRTC_RGB2, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_PVRTC_RGBA2, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_PVRTC_RGB4, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_PVRTC_RGBA4, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_PVRTC_2BPP, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//	set_info(PF_PVRTC_4BPP, GL_UNSIGNED_INT, GL_RGBA, GL_RGBA32UI);
//
//}

void OGL_Mapping::getFormat(RESOURCE_FORMAT fmt, GLint& glinternal, GLenum& glformat, GLenum& gltype)
{
	const GLPixelInfo& info = gl_infos[fmt];
	glinternal = info.ginternal;
	glformat = info.gformat;
	gltype = info.gtype;
}

GLenum OGL_Mapping::getGLInternal(RESOURCE_FORMAT fmt, bool gamma /* = false */)
{
	return gl_infos[fmt].ginternal;
}

GLenum OGL_Mapping::getGLFormat(RESOURCE_FORMAT fmt)
{
	return gl_infos[fmt].gformat;
}

GLenum OGL_Mapping::getGLType(RESOURCE_FORMAT fmt)
{
	return gl_infos[fmt].gtype;
}

GLenum OGL_Mapping::getUsage(RESOURCE_USAGE usage)
{
	switch (usage)
    {
        case RESOURCE_USAGE_COLOR_TARGET:
            return 0;
        case RESOURCE_USAGE_DEPTH_TARGET:
            return 0;
        case RESOURCE_USAGE_UNORDERD_ACCESS:
            return 0;
        case RESOURCE_USAGE_VERTEX_BUFFER:
            return GL_ARRAY_BUFFER;
        case RESOURCE_USAGE_INDEX_BUFFER:
            return GL_ELEMENT_ARRAY_BUFFER;
        case RESOURCE_USAGE_CONSTANT_BUFFER:
            return GL_UNIFORM_BUFFER;
        case RESOURCE_USAGE_INDIRECT_BUFFER:
            return 0;
        case RESOURCE_USAGE_SHADER_RESOURCE:
            return 0;
        case RESOURCE_USAGE_COPY_SRC:
            return 0;
        case RESOURCE_USAGE_COPY_DST:
            return 0;
        case RESOURCE_USAGE_QUERY_BUFFER:
            return 0;
        default:
            assert(false);
            break;
	}
	return 0;
}

//GLbitfield OGL_Mapping::getAccess(MAP_FLAG type)
//{
//	if (type == MAP_READ_ONLY)
//		return GL_MAP_READ_BIT;
//	else if (type == MAP_WRITE_ONLY)
//		return GL_MAP_WRITE_BIT;
//	//
//	GLbitfield access = GL_MAP_WRITE_BIT;
//	if (type == MAP_READ_WRITE)
//		access |= GL_MAP_READ_BIT;
//	else if (type == MAP_WRITE_ONLY_DISCARD)
//		access |= GL_MAP_INVALIDATE_BUFFER_BIT;
//	else if (type == MAP_WRITE_ONLY_ON_OVERWRITE)
//		access |= GL_MAP_UNSYNCHRONIZED_BIT;
//	return access;
//}
//
//GLint OGL_Mapping::getPrimitiveMode(Topology type)
//{
//	static const GLint gl_topology[] = 
//	{
//		0, GL_POINTS, 
//		GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, 
//		GL_LINES_ADJACENCY, GL_LINE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY, GL_TRIANGLE_STRIP_ADJACENCY
//	};
//
//	if (type < PT_CTRL_PATCH_LIST1)
//		return gl_topology[type];
//
//	return GL_PATCHES;
//}
//
//UniformType OGL_Mapping::getUniformType(GLenum type)
//{
//	switch (type)
//	{
//	case GL_BOOL:
//		return UT_BOOL1;
//	case GL_BOOL_VEC2:
//		return UT_BOOL2;
//	case GL_BOOL_VEC3:
//		return UT_BOOL3;
//	case GL_BOOL_VEC4:
//		return UT_BOOL4;
//	case GL_FLOAT:
//		return UT_FLOAT1;
//	case GL_FLOAT_VEC2:
//		return UT_FLOAT2;
//	case GL_FLOAT_VEC3:
//		return UT_FLOAT3;
//	case GL_FLOAT_VEC4:
//		return UT_FLOAT4;
//	case GL_INT:
//		return UT_INT1;
//	case GL_INT_VEC2:
//		return UT_INT2;
//	case GL_INT_VEC3:
//		return UT_INT3;
//	case GL_INT_VEC4:
//		return UT_INT4;
//	case GL_UNSIGNED_INT:
//		return UT_UINT1;
//	case GL_UNSIGNED_INT_VEC2:
//		return UT_UINT2;
//	case GL_UNSIGNED_INT_VEC3:
//		return UT_UINT3;
//	case GL_UNSIGNED_INT_VEC4:
//		return UT_UINT4;
//	case GL_FLOAT_MAT2:
//		return UT_MATRIX_2X2;
//	case GL_FLOAT_MAT2x3:
//		return UT_MATRIX_2X3;
//	case GL_FLOAT_MAT2x4:
//		return UT_MATRIX_2X4;
//	case GL_FLOAT_MAT3x2:
//		return UT_MATRIX_3X2;
//	case GL_FLOAT_MAT3:
//		return UT_MATRIX_3X3;
//	case GL_FLOAT_MAT3x4:
//		return UT_MATRIX_3X4;
//	case GL_FLOAT_MAT4x2:
//		return UT_MATRIX_4X2;
//	case GL_FLOAT_MAT4x3:
//		return UT_MATRIX_4X3;
//	case GL_FLOAT_MAT4:
//		return UT_MATRIX_4X4;
//	case GL_SAMPLER_1D:
//		return UT_SAMPLER1D;
//	case GL_SAMPLER_2D:
//		return UT_SAMPLER2D;
//	case GL_SAMPLER_3D:
//		return UT_SAMPLER3D;
//	case GL_SAMPLER_CUBE:
//		return UT_SAMPLERCUBE;
//	case GL_SAMPLER_2D_MULTISAMPLE:
//		return UT_SAMPLER2DMS;
//	case GL_IMAGE_1D:
//		return UT_RWTEXTURE1D;
//	case GL_IMAGE_2D:
//		return UT_RWTEXTURE2D;
//	case GL_IMAGE_3D:
//		return UT_RWTEXTURE3D;
//	case GL_IMAGE_2D_MULTISAMPLE:
//		return UT_RWTEXTURE2DMS;
//	default:
//		return UT_UNKNOWN;
//	}
//}

CUTE_NS_END
