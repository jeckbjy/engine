#pragma once
#include "API.h"

CU_NS_BEGIN

// http://blog.csdn.net/liminled/article/details/45149259
enum PixelFormat
{
	PF_UNKNOWN = 0,

	PF_UINT_R8,
	PF_UINT_RG8,
	PF_UINT_RGB8,
	PF_UINT_RGBA8,
	PF_UINT_R16,
	PF_UINT_RG16,
	PF_UINT_RGB16,
	PF_UINT_RGBA16,
	PF_UINT_R32,
	PF_UINT_RG32,
	PF_UINT_RGB32,
	PF_UINT_RGBA32,

	PF_SINT_R8,
	PF_SINT_RG8,
	PF_SINT_RGB8,
	PF_SINT_RGBA8,
	PF_SINT_R16,
	PF_SINT_RG16,
	PF_SINT_RGB16,
	PF_SINT_RGBA16,
	PF_SINT_R32,
	PF_SINT_RG32,
	PF_SINT_RGB32,
	PF_SINT_RGBA32,

	PF_SNORM_R8,
	PF_SNORM_RG8,
	PF_SNORM_RGB8,
	PF_SNORM_RGBA8,
	PF_SNORM_R16,
	PF_SNORM_RG16,
	PF_SNORM_RGB16,
	PF_SNORM_RGBA16,

	//PF_SNORM_R32,
	//PF_SNORM_RG32,
	//PF_SNORM_RGB32,
	//PF_SNORM_RGBA32,
	//PF_UNORM_R8,
	//PF_UNORM_RG8,
	//PF_UNORM_RGB8,
	//PF_UNORM_RGBA8,
	//PF_UNORM_R16,
	//PF_UNORM_RG16,
	//PF_UNORM_RGB16,
	//PF_UNORM_RGBA16,
	//PF_UNORM_R32,
	//PF_UNORM_RG32,
	//PF_UNORM_RGB32,
	//PF_UNORM_RGBA32,

	PF_FLOAT_R16,
	PF_FLOAT_GR16,
	PF_FLOAT_RGB16,
	PF_FLOAT_RGBA16,
	PF_FLOAT_R32,
	PF_FLOAT_GR32,
	PF_FLOAT_RGB32,
	PF_FLOAT_RGBA32,
	PF_FLOAT_R11G11B10,

	// unorm
	PF_R8G8B8,
	PF_B8G8R8,
	PF_R8G8B8A8,
	PF_B8G8R8A8,
	PF_A8R8G8B8,
	PF_A8B8G8R8,
	PF_X8R8G8B8,
	PF_X8B8G8R8,
	// srgb
	PF_R8G8B8_SRGB,
	PF_B8G8R8_SRGB,
	PF_R8G8B8A8_SRGB,
	PF_B8G8R8A8_SRGB,
	PF_A8R8G8B8_SRGB,
	PF_A8B8G8R8_SRGB,
	PF_X8R8G8B8_SRGB,
	PF_X8B8G8R8_SRGB,

	PF_D32_FLOAT_S8X24_UINT,
	PF_D24_UNORM_S8_UINT,
	PF_D32_FLOAT,
	PF_D16_UNORM,

	PF_L8,
	PF_L16,
	PF_A8,
	PF_A4L4,
	PF_R5G6B5,
	PF_B5G6R5,
	PF_R3G3B2,
	PF_A4R4G4B4,
	PF_A1R5G5B5,
	PF_A2R10G10B10,
	PF_A2B10G10R10,
	PF_R9G9B9E5,		//PF_R9G9B9E5_SHAREDEXP

	// dds, S3TC_DXT1, S3TC_DXT3, S3TC_DXT5
	PF_BC1_UNORM,
	PF_BC2_UNORM,
	PF_BC3_UNORM,
	PF_BC4_UNORM,
	PF_BC4_SNORM,
	PF_BC5_UNORM,
	PF_BC5_SNORM,
	PF_BC6H_UF16,
	PF_BC6H_SF16,
	PF_BC7_UNORM,

	PF_BC1_SRGB,
	PF_BC2_SRGB,
	PF_BC3_SRGB,
	PF_BC4_SRGB,
	PF_BC5_SRGB,
	PF_BC7_SRGB,

	// opengl es:etc1,etc2,eac,astc
	PF_ETC1_RGB8,
	PF_ETC2_RGB8,
	PF_ETC2_RGBA8,
	PF_ETC2_RGB8A1,
	PF_ATC_RGB,							// ATC (AMD_compressed_ATC_texture)
	PF_ATC_RGBA_EXPLICIT_ALPHA,
	PF_ATC_RGBA_INTERPOLATED_ALPHA,

	// PVRTC(PowerVR) RGB 2 bpp
	PF_PVRTC_RGB2,
	PF_PVRTC_RGBA2,
	PF_PVRTC_RGB4,
	PF_PVRTC_RGBA4,
	PF_PVRTC_2BPP,	// PVRTC (PowerVR) Version 2, 2 bpp
	PF_PVRTC_4BPP,

	PIXEL_FORMAT_MAX,
	PF_FLOAT1 = PF_FLOAT_R32,
	PF_FLOAT2 = PF_FLOAT_GR32,
	PF_FLOAT3 = PF_FLOAT_RGB32,
	PF_FLOAT4 = PF_FLOAT_RGBA32,
	PF_SBYTE1 = PF_SINT_R8,
	PF_SBYTE2 = PF_SINT_RG8,
	PF_SBYTE3 = PF_SINT_RGB8,
	PF_SBYTE4 = PF_SINT_RGBA8,
	PF_UBYTE1 = PF_UINT_R8,
	PF_UBYTE2 = PF_UINT_RG8,
	PF_UBYTE3 = PF_UINT_RGB8,
	PF_UBYTE4 = PF_UINT_RGBA8,
	PF_SHORT1 = PF_SINT_R16,
	PF_SHORT2 = PF_SINT_RG16,
	PF_SHORT3 = PF_SINT_RGB16,
	PF_SHORT4 = PF_SINT_RGBA16,
	PF_USHORT1 = PF_UINT_R16,
	PF_USHORT2 = PF_UINT_RG16,
	PF_USHORT3 = PF_UINT_RGB16,
	PF_USHORT4 = PF_UINT_RGBA16,
	PF_SINT1 = PF_SINT_R32,
	PF_SINT2 = PF_SINT_RG32,
	PF_SINT3 = PF_SINT_RGB32,
	PF_SINT4 = PF_SINT_RGBA32,
	PF_UINT1 = PF_UINT_R32,
	PF_UINT2 = PF_UINT_RG32,
	PF_UINT3 = PF_UINT_RGB32,
	PF_UINT4 = PF_UINT_RGBA32,
	// dxt1,2硬件相同，3,4硬件上相同
	PF_DXT1 = PF_BC1_UNORM,
	PF_DXT2 = PF_BC1_UNORM,
	PF_DXT3 = PF_BC2_UNORM,
	PF_DXT4 = PF_BC2_UNORM,
	PF_DXT5 = PF_BC3_UNORM,

#if CU_ENDIAN == CU_ENDIAN_BIG
	PF_BYTE_RGB = PF_R8G8B8,
	PF_BYTE_BGR = PF_B8G8R8,
	PF_BYTE_RGBA = PF_R8G8B8A8,
	PF_BYTE_BGRA = PF_B8G8R8A8,
#else
	PF_BYTE_RGB = PF_B8G8R8,
	PF_BYTE_BGR = PF_R8G8B8,
	PF_BYTE_RGBA = PF_B8G8R8A8,
	PF_BYTE_BGRA = PF_R8G8B8A8,
#endif
};

struct CU_API PixelUtil
{
	static uint getFormat(const String& format);
	static uint getBytes(PixelFormat fmt);			// 总共占有多少字节
	static uint getElementCount(PixelFormat fmt);	// 返回有多少个元素

	static uint getMemorySize(PixelFormat fmt, uint width, uint height, uint depth);
	static uint getImageSize(PixelFormat fmt, uint width, uint height, uint depth, uint mipmaps, uint faces);

	static bool hasFlags(PixelFormat fmt, uint32_t flags);
	static bool isCompressed(PixelFormat fmt);
	static bool isDepth(PixelFormat fmt);
	static bool isStencil(PixelFormat fmt);
	static bool isDepthStencil(PixelFormat fmt);
	static bool isAlpha(PixelFormat fmt);
	static bool isFloat(PixelFormat fmt);
	static bool isNormalized(PixelFormat fmt);
	static bool isLuminance(PixelFormat fmt);
};

struct PixelData
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	PixelFormat format;
	char* data;
	PixelData(){ memset(this, 0, sizeof(PixelData)); }
	PixelData(uint32_t width, uint32_t height, char* data)
		:x(0), y(0), z(0), width(width), height(height), data(data){}
};

CU_NS_END
