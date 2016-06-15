#include "PixelFormat.h"

CU_NS_BEGIN

// norm:用整数标识浮点数
enum PixelType
{
	PT_UNDEFINED,
	PT_UNORM,
	PT_SNORM,
	PT_USCALED,
	PT_SSCALED,
	PT_UINT,
	PT_SINT,
	PT_UFLOAT,
	PT_SFLOAT,
	PT_SRGB,
};

enum PixelChannel
{
	PC_DEFAULT = 0,
	PC_ALPHA = 0x01,
	PC_BLOCK = 0x02,	// COMPRESSED
	// 缩写
	PC_D = PC_DEFAULT,
	PC_A = PC_ALPHA,
	PC_B = PC_BLOCK,
};

struct PixelInfo
{
	const char* name;
	PixelType	type;		// components类型
	uint32_t	channel;
	uint8_t		components;	// 元素个数
	uint8_t		bytes;		// 总字节数
};

#define DEF_FORMAT(fmt, type, channel, components, bytes) {#fmt, type, channel, components, bytes }

static PixelInfo gPixelInfoArray[PIXEL_FORMAT_MAX] = 
{
	DEF_FORMAT(PF_UNDEFINED, PT_UNDEFINED,	PC_D, 0, 0),
	DEF_FORMAT(PF_R4G4_UNORM,			PT_UNORM,	PC_D, 2, 1),
	DEF_FORMAT(PF_R4G4B4A4_UNORM,		PT_UNORM,	PC_A, 4, 2),
	DEF_FORMAT(PF_B4G4R4A4_UNORM,		PT_UNORM,	PC_A, 4, 2), 
	DEF_FORMAT(PF_R5G6B5_UNORM,			PT_UNORM,	PC_D, 3, 2),
	DEF_FORMAT(PF_B5G6R5_UNORM,			PT_UNORM,	PC_D, 3, 2),
	DEF_FORMAT(PF_R5G5B5A1_UNORM,		PT_UNORM,	PC_A, 4, 2),
	DEF_FORMAT(PF_B5G5R5A1_UNORM,		PT_UNORM,	PC_A, 4, 2),
	DEF_FORMAT(PF_A1R5G5B5_UNORM,		PT_UNORM,	PC_A, 4, 2),
	DEF_FORMAT(PF_R8_UNORM,				PT_UNORM,	PC_D, 1, 1),
	DEF_FORMAT(PF_R8_SNORM,				PT_SNORM,	PC_D, 1, 1),
	DEF_FORMAT(PF_R8_USCALED,			PT_USCALED,	PC_D, 1, 1),
	DEF_FORMAT(PF_R8_SSCALED,			PT_SSCALED, PC_D, 1, 1),
	DEF_FORMAT(PF_R8_UINT,				PT_UINT,	PC_D, 1, 1),
	DEF_FORMAT(PF_R8_SINT,				PT_SINT,	PC_D, 1, 1),
	DEF_FORMAT(PF_R8_SRGB,				PT_SRGB,	PC_D, 1, 1),
	DEF_FORMAT(PF_R8G8_UNORM,			PT_UNORM,	PC_D, 2, 2),
	DEF_FORMAT(PF_R8G8_SNORM,			PT_SNORM,	PC_D, 2, 2),
	DEF_FORMAT(PF_R8G8_USCALED,			PT_USCALED, PC_D, 2, 2),
	DEF_FORMAT(PF_R8G8_SSCALED,			PT_USCALED, PC_D, 2, 2),
	DEF_FORMAT(PF_R8G8_UINT,			PT_UINT,	PC_D, 2, 2),
	DEF_FORMAT(PF_R8G8_SINT,			PT_SINT,	PC_D, 2, 2),
	DEF_FORMAT(PF_R8G8_SRGB,			PT_SRGB,	PC_D, 2, 2),
	DEF_FORMAT(PF_R8G8B8_UNORM,			PT_UNORM,	PC_D, 3, 3),
	DEF_FORMAT(PF_R8G8B8_SNORM,			PT_SNORM,	PC_D, 3, 3),
	DEF_FORMAT(PF_R8G8B8_USCALED,		PT_USCALED, PC_D, 3, 3),
	DEF_FORMAT(PF_R8G8B8_SSCALED,		PT_USCALED, PC_D, 3, 3),
	DEF_FORMAT(PF_R8G8B8_UINT,			PT_UINT,	PC_D, 3, 3),
	DEF_FORMAT(PF_R8G8B8_SINT,			PT_SINT,	PC_D, 3, 3),
	DEF_FORMAT(PF_R8G8B8_SRGB,			PT_SRGB,	PC_D, 3, 3),
	DEF_FORMAT(PF_B8G8R8_UNORM,			PT_UNORM,	PC_D, 3, 3),
	DEF_FORMAT(PF_B8G8R8_SNORM,			PT_SNORM,	PC_D, 3, 3),
	DEF_FORMAT(PF_B8G8R8_USCALED,		PT_USCALED, PC_D, 3, 3),
	DEF_FORMAT(PF_B8G8R8_SSCALED,		PT_USCALED, PC_D, 3, 3),
	DEF_FORMAT(PF_B8G8R8_UINT,			PT_UINT,	PC_D, 3, 3),
	DEF_FORMAT(PF_B8G8R8_SINT,			PT_SINT,	PC_D, 3, 3),
	DEF_FORMAT(PF_B8G8R8_SRGB,			PT_SRGB,	PC_D, 3, 3),
	DEF_FORMAT(PF_R8G8B8A8_UNORM,		PT_UNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_R8G8B8A8_SNORM,		PT_SNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_R8G8B8A8_USCALED,		PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_R8G8B8A8_SSCALED,		PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_R8G8B8A8_UINT,		PT_UINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_R8G8B8A8_SINT,		PT_SINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_R8G8B8A8_SRGB,		PT_SRGB,	PC_A, 4, 4),
	DEF_FORMAT(PF_B8G8R8A8_UNORM,		PT_UNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_B8G8R8A8_SNORM,		PT_SNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_B8G8R8A8_USCALED,		PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_B8G8R8A8_SSCALED,		PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_B8G8R8A8_UINT,		PT_UINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_B8G8R8A8_SINT,		PT_SINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_B8G8R8A8_SRGB,		PT_SRGB,	PC_A, 4, 4),
	DEF_FORMAT(PF_A8B8G8R8_UNORM,		PT_UNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_A8B8G8R8_SNORM,		PT_SNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_A8B8G8R8_USCALED,		PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_A8B8G8R8_SSCALED,		PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_A8B8G8R8_UINT,		PT_UINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_A8B8G8R8_SINT,		PT_SINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_A8B8G8R8_SRGB,		PT_SRGB,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2R10G10B10_UNORM,	PT_UNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2R10G10B10_SNORM,	PT_SNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2R10G10B10_USCALED,	PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_A2R10G10B10_SSCALED,	PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_A2R10G10B10_UINT,		PT_UINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2R10G10B10_SINT,		PT_SINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2B10G10R10_UNORM,	PT_UNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2B10G10R10_SNORM,	PT_SNORM,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2B10G10R10_USCALED,	PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_A2B10G10R10_SSCALED,	PT_USCALED, PC_A, 4, 4),
	DEF_FORMAT(PF_A2B10G10R10_UINT,		PT_UINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_A2B10G10R10_SINT,		PT_SINT,	PC_A, 4, 4),
	DEF_FORMAT(PF_R16_UNORM,			PT_UNORM,	PC_D, 1, 2),
	DEF_FORMAT(PF_R16_SNORM,			PT_SNORM,	PC_D, 1, 2),
	DEF_FORMAT(PF_R16_USCALED,			PT_USCALED, PC_D, 1, 2),
	DEF_FORMAT(PF_R16_SSCALED,			PT_SSCALED, PC_D, 1, 2),
	DEF_FORMAT(PF_R16_UINT,				PT_UINT,	PC_D, 1, 2),
	DEF_FORMAT(PF_R16_SINT,				PT_SINT,	PC_D, 1, 2),
	DEF_FORMAT(PF_R16_SFLOAT,			PT_SFLOAT,	PC_D, 1, 2),
	DEF_FORMAT(PF_R16G16_UNORM,			PT_UNORM,	PC_D, 2, 4),
	DEF_FORMAT(PF_R16G16_SNORM,			PT_SNORM,	PC_D, 2, 4),
	DEF_FORMAT(PF_R16G16_USCALED,		PT_USCALED, PC_D, 2, 4),
	DEF_FORMAT(PF_R16G16_SSCALED,		PT_SSCALED, PC_D, 2, 4),
	DEF_FORMAT(PF_R16G16_UINT,			PT_UINT,	PC_D, 2, 4),
	DEF_FORMAT(PF_R16G16_SINT,			PT_SINT,	PC_D, 2, 4),
	DEF_FORMAT(PF_R16G16_SFLOAT,		PT_SFLOAT,	PC_D, 2, 4),
	DEF_FORMAT(PF_R16G16B16_UNORM,		PT_UNORM,	PC_D, 3, 6),
	DEF_FORMAT(PF_R16G16B16_SNORM,		PT_SNORM,	PC_D, 3, 6),
	DEF_FORMAT(PF_R16G16B16_USCALED,	PT_USCALED, PC_D, 3, 6),
	DEF_FORMAT(PF_R16G16B16_SSCALED,	PT_SSCALED, PC_D, 3, 6),
	DEF_FORMAT(PF_R16G16B16_UINT,		PT_UINT,	PC_D, 3, 6),
	DEF_FORMAT(PF_R16G16B16_SINT,		PT_SINT,	PC_D, 3, 6),
	DEF_FORMAT(PF_R16G16B16_SFLOAT,		PT_SFLOAT,	PC_D, 3, 6),
	DEF_FORMAT(PF_R16G16B16A16_UNORM,	PT_UNORM,	PC_A, 4, 8),
	DEF_FORMAT(PF_R16G16B16A16_SNORM,	PT_SNORM,	PC_A, 4, 8),
	DEF_FORMAT(PF_R16G16B16A16_USCALED, PT_USCALED, PC_A, 4, 8),
	DEF_FORMAT(PF_R16G16B16A16_SSCALED, PT_SSCALED, PC_A, 4, 8),
	DEF_FORMAT(PF_R16G16B16A16_UINT,	PT_UINT,	PC_A, 4, 8),
	DEF_FORMAT(PF_R16G16B16A16_SINT,	PT_SINT,	PC_A, 4, 8),
	DEF_FORMAT(PF_R16G16B16A16_SFLOAT,	PT_SFLOAT,	PC_A, 4, 8),
	DEF_FORMAT(PF_R32_UINT,				PT_UINT,	PC_D, 1, 4),
	DEF_FORMAT(PF_R32_SINT,				PT_SINT,	PC_D, 1, 4),
	DEF_FORMAT(PF_R32_SFLOAT,			PT_SFLOAT,	PC_D, 1, 4),
	DEF_FORMAT(PF_R32G32_UINT,			PT_UINT,	PC_D, 2, 8),
	DEF_FORMAT(PF_R32G32_SINT,			PT_SINT,	PC_D, 2, 8),
	DEF_FORMAT(PF_R32G32_SFLOAT,		PT_SFLOAT,	PC_D, 2, 8),
	DEF_FORMAT(PF_R32G32B32_UINT,		PT_UINT,	PC_D, 3, 12),
	DEF_FORMAT(PF_R32G32B32_SINT,		PT_SINT,	PC_D, 3, 12),
	DEF_FORMAT(PF_R32G32B32_SFLOAT,		PT_SFLOAT,	PC_D, 3, 12),
	DEF_FORMAT(PF_R32G32B32A32_UINT,	PT_UINT,	PC_A, 4, 16),
	DEF_FORMAT(PF_R32G32B32A32_SINT,	PT_SINT,	PC_A, 4, 16),
	DEF_FORMAT(PF_R32G32B32A32_SFLOAT,	PT_SFLOAT,	PC_A, 4, 16),
	DEF_FORMAT(PF_R64_UINT,				PT_UINT,	PC_D, 1, 4),
	DEF_FORMAT(PF_R64_SINT,				PT_SINT,	PC_D, 1, 4),
	DEF_FORMAT(PF_R64_SFLOAT,			PT_SFLOAT,	PC_D, 1, 4),
	DEF_FORMAT(PF_R64G64_UINT,			PT_UINT,	PC_D, 2, 8),
	DEF_FORMAT(PF_R64G64_SINT,			PT_SINT,	PC_D, 2, 8),
	DEF_FORMAT(PF_R64G64_SFLOAT,		PT_SFLOAT,	PC_D, 2, 8),
	DEF_FORMAT(PF_R64G64B64_UINT,		PT_UINT,	PC_D, 3, 12),
	DEF_FORMAT(PF_R64G64B64_SINT,		PT_SINT,	PC_D, 3, 12),
	DEF_FORMAT(PF_R64G64B64_SFLOAT,		PT_SFLOAT,	PC_D, 3, 12),
	DEF_FORMAT(PF_R64G64B64A64_UINT,	PT_UINT,	PC_A, 4, 16),
	DEF_FORMAT(PF_R64G64B64A64_SINT,	PT_SINT,	PC_A, 4, 16),
	DEF_FORMAT(PF_R64G64B64A64_SFLOAT,	PT_SFLOAT,	PC_A, 4, 16),
	DEF_FORMAT(PF_B10G11R11_UFLOAT,		PT_UFLOAT,	PC_D, 3, 4),
	DEF_FORMAT(PF_E5B9G9R9_UFLOAT,		PT_UFLOAT,	PC_D, 3, 4),
	DEF_FORMAT(PF_D16_UNORM,			PT_UNORM,	PC_D, 1, 2),
	DEF_FORMAT(PF_X8_D24_UNORM,			PT_UNORM,	PC_D, 1, 4),
	DEF_FORMAT(PF_D32_SFLOAT,			PT_SFLOAT,	PC_D, 1, 4),
	DEF_FORMAT(PF_S8_UINT,				PT_UINT,	PC_D, 1, 1),
	DEF_FORMAT(PF_D16_UNORM_S8_UINT,	PT_UNORM,	PC_D, 2, 3),
	DEF_FORMAT(PF_D24_UNORM_S8_UINT,	PT_UNORM,	PC_D, 2, 4),
	DEF_FORMAT(PF_D32_SFLOAT_S8_UINT,	PT_SFLOAT,	PC_D, 2, 5),
	DEF_FORMAT(PF_BC1_RGB_UNORM,		PT_UNORM,	PC_D, 3, 0),
	DEF_FORMAT(PF_BC1_RGB_SRGB,			PT_SRGB,	PC_D, 3, 0),
	DEF_FORMAT(PF_BC1_RGBA_UNORM,		PT_UNORM,	PC_D, 4, 0),
	DEF_FORMAT(PF_BC1_RGBA_SRGB,		PT_SRGB,	PC_D, 4, 0),
	DEF_FORMAT(PF_BC2_UNORM,			PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC2_SRGB,				PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC3_UNORM,			PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC3_SRGB,				PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC4_UNORM,			PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC4_SNORM,			PT_SNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC5_UNORM,			PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC5_SNORM,			PT_SNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC6H_UFLOAT,			PT_UFLOAT,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC6H_SFLOAT,			PT_SFLOAT,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC7_UNORM,			PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_BC7_SRGB,				PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ETC2_R8G8B8_UNORM,	PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ETC2_R8G8B8_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ETC2_R8G8B8A1_UNORM,	PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ETC2_R8G8B8A1_SRGB,	PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ETC2_R8G8B8A8_UNORM,	PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ETC2_R8G8B8A8_SRGB,	PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_EAC_R11_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_EAC_R11_SNORM,		PT_SNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_EAC_R11G11_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_EAC_R11G11_SNORM,		PT_SNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_4x4_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_4x4_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_5x4_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_5x4_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_5x5_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_5x5_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_6x5_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_6x5_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_6x6_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_6x6_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_8x5_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_8x5_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_8x6_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_8x6_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_8x8_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_8x8_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x5_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x5_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x6_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x6_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x8_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x8_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x10_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_10x10_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_12x10_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_12x10_SRGB,		PT_SRGB,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_12x12_UNORM,		PT_UNORM,	PC_D, 0, 0),
	DEF_FORMAT(PF_ASTC_12x12_SRGB,		PT_SRGB,	PC_D, 0, 0),
};

inline static const PixelInfo& getDesc(PixelFormat fmt)
{
	assert(fmt < PIXEL_FORMAT_MAX);
	return gPixelInfoArray[(int)fmt];
}

uint PixelUtil::getFormat(const String& format)
{
	if (format == "rgb")
		return PF_R32G32B32_SFLOAT;
	else if (format == "rgba")
		return PF_R32G32B32A32_SFLOAT;
	else if (format == "d24s8")
		return PF_D24_UNORM_S8_UINT;

	return PF_UNKNOWN;

	//if (format == "a")
	//	return PF_A8;
	//else if (format == "l")
	//	return PF_L8;
	//else if (format == "la")
	//	return PF_A4L4;		// a8l8?
	//else if (format == "rgb")
	//	return PF_FLOAT_RGB32;
	//else if (format == "rgba")
	//	return PF_FLOAT_RGBA32;
	//else if (format == "d24s8")
	//	return PF_D24_UNORM_S8_UINT;
	//return PF_FLOAT_RGB32;
}

uint PixelUtil::getBytes(PixelFormat fmt)
{
	return gPixelInfoArray[fmt].bytes;
}

uint PixelUtil::getComponents(PixelFormat fmt)
{
	return gPixelInfoArray[fmt].components;
}

uint PixelUtil::getMemorySize(PixelFormat fmt, uint width, uint height, uint depth)
{
	if (isCompressed(fmt))
	{
		//switch (fmt)
		//{
		//	// DXT formats work by dividing the image into 4x4 blocks, then encoding each
		//	// 4x4 block with a certain number of bytes. 
		//case PF_BC1_UNORM:
		//	return ((width + 3) / 4)*((height + 3) / 4) * 8 * depth;
		//case PF_BC2_UNORM:
		//case PF_BC3_UNORM:
		//	return ((width + 3) / 4)*((height + 3) / 4) * 16 * depth;
		//case PF_BC4_SNORM:
		//case PF_BC4_UNORM:
		//	return ((width + 3) / 4)*((height + 3) / 4) * 8 * depth;
		//case PF_BC5_SNORM:
		//case PF_BC5_UNORM:
		//case PF_BC6H_SF16:
		//case PF_BC6H_UF16:
		//case PF_BC7_UNORM:
		//case PF_BC7_SRGB:
		//	return ((width + 3) / 4)*((height + 3) / 4) * 16 * depth;
		//	// Size calculations from the PVRTC OpenGL extension spec
		//	// http://www.khronos.org/registry/gles/extensions/IMG/IMG_texture_compression_pvrtc.txt
		//	// Basically, 32 bytes is the minimum texture size.  Smaller textures are padded up to 32 bytes
		//case PF_PVRTC_RGB2:
		//case PF_PVRTC_RGBA2:
		//case PF_PVRTC_2BPP:
		//	return (std::max((int)width, 16) * std::max((int)height, 8) * 2 + 7) / 8;
		//case PF_PVRTC_RGB4:
		//case PF_PVRTC_RGBA4:
		//case PF_PVRTC_4BPP:
		//	return (std::max((int)width, 8) * std::max((int)height, 8) * 4 + 7) / 8;
		//case PF_ETC1_RGB8:
		//case PF_ETC2_RGB8:
		//case PF_ETC2_RGBA8:
		//case PF_ETC2_RGB8A1:
		//	return ((width * height) >> 1);
		//case PF_ATC_RGB:
		//	return ((width + 3) / 4) * ((height + 3) / 4) * 8;
		//case PF_ATC_RGBA_EXPLICIT_ALPHA:
		//case PF_ATC_RGBA_INTERPOLATED_ALPHA:
		//	return ((width + 3) / 4) * ((height + 3) / 4) * 16;
		//default:
		//	throw std::runtime_error("error format");
		//	return 0;
		//}
		return 0;
	}
	else
	{
		return width*height*depth*getBytes(fmt);
	}
}

uint PixelUtil::getImageSize(PixelFormat fmt, uint width, uint height, uint depth, uint mipmaps, uint faces)
{
	uint size = 0;
	for (uint mip = 0; mip < mipmaps; ++mip)
	{
		size += PixelUtil::getMemorySize(fmt, width, height, depth) * faces;
		if (width != 1) width >>= 1;
		if (height != 1) height >>= 1;
		if (depth != 1) depth >>= 1;
	}
	return size;
}

bool PixelUtil::isCompressed(PixelFormat fmt)
{
	return fmt >= PF_BC1_RGB_UNORM && fmt <= PF_ASTC_12x12_SRGB;
}

bool PixelUtil::isDepth(PixelFormat fmt)
{
	switch (fmt)
	{
	case PF_D16_UNORM:
	case PF_X8_D24_UNORM:
	case PF_D32_SFLOAT:
	case PF_D16_UNORM_S8_UINT:
	case PF_D24_UNORM_S8_UINT:
	case PF_D32_SFLOAT_S8_UINT:
		return true;
	default:
		return false;
	}
}

bool PixelUtil::isStencil(PixelFormat fmt)
{
	switch (fmt)
	{
	case PF_S8_UINT:
	case PF_D16_UNORM_S8_UINT:
	case PF_D24_UNORM_S8_UINT:
	case PF_D32_SFLOAT_S8_UINT:
		return true;
	default:
		return false;
	}
}

bool PixelUtil::isDepthStencil(PixelFormat fmt)
{
	switch (fmt)
	{
	case PF_D16_UNORM:
	case PF_X8_D24_UNORM:
	case PF_D32_SFLOAT:
	case PF_S8_UINT:
	case PF_D16_UNORM_S8_UINT:
	case PF_D24_UNORM_S8_UINT:
	case PF_D32_SFLOAT_S8_UINT:
		return true;
	default:
		return false;
	}
}

static inline bool hasChannel(PixelFormat fmt, uint32_t flag)
{
	return (gPixelInfoArray[fmt].channel & flag) == flag;
}

bool PixelUtil::isAlpha(PixelFormat fmt)
{
	return hasChannel(fmt, PC_A);
}

bool PixelUtil::isFloat(PixelFormat fmt)
{
	PixelType type = gPixelInfoArray[fmt].type;
	return (type == PT_SFLOAT) || (type == PT_UFLOAT);
}

bool PixelUtil::isInteger(PixelFormat fmt)
{
	PixelType type = gPixelInfoArray[fmt].type;
	return (type == PT_SINT) || (type == PT_UINT);
}

bool PixelUtil::isNormalized(PixelFormat fmt)
{
	PixelType type = gPixelInfoArray[fmt].type;
	return (type == PT_SNORM) || (type == PT_UNORM);
}

bool PixelUtil::isScaled(PixelFormat fmt)
{
	PixelType type = gPixelInfoArray[fmt].type;
	return (type == PT_SSCALED) || (type == PT_USCALED);
}

bool PixelUtil::isSRGB(PixelFormat fmt)
{
	PixelType type = gPixelInfoArray[fmt].type;
	return type == PT_SRGB;
}

bool PixelUtil::isSigned(PixelFormat fmt)
{
	PixelType type = gPixelInfoArray[fmt].type;
	switch (type)
	{
		case PT_SNORM:
		case PT_SSCALED:
		case PT_SINT:
		case PT_SFLOAT:
			return true;
		default:
			return false;
	}
}

bool PixelUtil::isUnsigned(PixelFormat fmt)
{
	PixelType type = gPixelInfoArray[fmt].type;
	switch (type)
	{
	case PT_UNORM:
	case PT_USCALED:
	case PT_UINT:
	case PT_UFLOAT:
		return true;
	default:
		return false;
	}
}

//bool PixelUtil::isLuminance(PixelFormat fmt)
//{
//	return hasFlags(fmt, PC_LUMINANCE);
//}

CU_NS_END
