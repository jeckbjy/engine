//! Render
#include "Cute/RenderDesc.h"

CUTE_NS_BEGIN

enum PixelType
{
    PT_NONE,
    PT_BYTE,
    PT_SHORT,
    PT_SINT,
    PT_UINT,
    PT_FLOAT16,
    PT_FLOAT32,
};

enum PixelChannel
{
    PC_ALPHA		= 0x01,
    PC_FLOAT		= 0x02,
    PC_INTEGER		= 0x04,
    PC_SIGNED		= 0x08,
    PC_DEPTH		= 0x10,
    PC_STENCIL		= 0x20,
    PC_LUMINANCE	= 0x40,
    PC_COMPRESSED	= 0x80,
    PC_NORM			= 0x100,
    PC_ENDIAN		= 0x200,
    //
    PC_A    = PC_ALPHA,
    PC_F    = PC_FLOAT,
    PC_I    = PC_INTEGER,
    PC_S    = PC_SIGNED,
    PC_D    = PC_DEPTH,
    PC_L    = PC_LUMINANCE,
    PC_C    = PC_COMPRESSED,
    PC_N    = PC_NORM,
    PC_E    = PC_ENDIAN,
    PC_DS   = PC_D  | PC_STENCIL,
    PC_IE   = PC_I  | PC_E,
    PC_IEA  = PC_IE | PC_A,
    PC_SE   = PC_S  | PC_E,
    PC_SEA  = PC_SE | PC_A,
    PC_FA   = PC_F  | PC_A,
    PC_FE   = PC_F  | PC_E,
    PC_EA   = PC_E  | PC_A,
    PC_LE   = PC_L  | PC_E,
    PC_LEA  = PC_LE | PC_A,
    PC_CA   = PC_C  | PC_A,
    PC_CS   = PC_C  | PC_S,
};

struct PixelInfo
{
    const char* name;
    uint32      flags;
    PixelType	type;
    uint8       components;
    uint8		bytes;
};

static PixelInfo g_infos[RESOURCE_FORMAT_MAX];

inline void set_fmt(RESOURCE_FORMAT fmt, const char* name, uint32_t flags, PixelType type, uint8_t components, uint8_t bytes)
{
    PixelInfo& info = g_infos[fmt];
    info.name = name;
    info.flags = flags;
    info.type = type;
    info.components = components;
    info.bytes = bytes;
}

//CU_C_CTOR(init_pixel_format)
void init_pixel_format()
{
//    set_fmt(PF_UNKNOWN, "PF_UNKNOWN", 0, PT_BYTE, 0, 0);
//    
//    set_fmt(PF_UINT_R8, "PF_UINT_R8", PC_IE, PT_UINT, 1, 1);
//    set_fmt(PF_UINT_RG8, "PF_UINT_RG8", PC_IE, PT_UINT, 2, 2 );
//    set_fmt(PF_UINT_RGB8, "PF_UINT_RGB8", PC_IE, PT_UINT, 3, 3 );
//    set_fmt(PF_UINT_RGBA8, "PF_UINT_RGBA8", PC_IEA, PT_UINT, 4, 4 );
//    set_fmt(PF_UINT_R16, "PF_UINT_R16", PC_IE, PT_UINT, 1, 2 );
//    set_fmt(PF_UINT_RG16, "PF_UINT_RG16", PC_IE, PT_UINT, 2, 4 );
//    set_fmt(PF_UINT_RGB16, "PF_UINT_RGB16", PC_IE, PT_UINT, 3, 6 );
//    set_fmt(PF_UINT_RGBA16, "PF_UINT_RGBA16", PC_IEA, PT_UINT, 4, 8 );
//    set_fmt(PF_UINT_R32, "PF_UINT_R32", PC_IE, PT_UINT, 1, 4 );
//    set_fmt(PF_UINT_RG32, "PF_UINT_RG32", PC_IE, PT_UINT, 2, 8 );
//    set_fmt(PF_UINT_RGB32, "PF_UINT_RGB32", PC_IE, PT_UINT, 3, 12 );
//    set_fmt(PF_UINT_RGBA32, "PF_UINT_RGBA32", PC_IEA, PT_UINT, 4, 16 );
//    
//    set_fmt(PF_SINT_R8, "PF_SINT_R8", PC_IE, PT_SINT, 1, 1);
//    set_fmt(PF_SINT_RG8, "PF_SINT_RG8", PC_IE, PT_SINT, 2, 2);
//    set_fmt(PF_SINT_RGB8, "PF_SINT_RGB8", PC_IE, PT_SINT, 3, 3);
//    set_fmt(PF_SINT_RGBA8, "PF_SINT_RGBA8", PC_IEA, PT_SINT, 4, 4);
//    set_fmt(PF_SINT_R16, "PF_SINT_R16", PC_IE, PT_SINT, 1, 2);
//    set_fmt(PF_SINT_RG16, "PF_SINT_RG16", PC_IE, PT_SINT, 2, 4);
//    set_fmt(PF_SINT_RGB16, "PF_SINT_RGB16", PC_IE, PT_SINT, 3, 6);
//    set_fmt(PF_SINT_RGBA16, "PF_SINT_RGBA16", PC_IEA, PT_SINT, 4, 8);
//    set_fmt(PF_SINT_R32, "PF_SINT_R32", PC_IE, PT_SINT, 1, 4);
//    set_fmt(PF_SINT_RG32, "PF_SINT_RG32", PC_IE, PT_SINT, 2, 8);
//    set_fmt(PF_SINT_RGB32, "PF_SINT_RGB32", PC_IE, PT_SINT, 3, 12);
//    set_fmt(PF_SINT_RGBA32, "PF_SINT_RGBA32", PC_IEA, PT_SINT, 4, 16);
//    
//    set_fmt(PF_SNORM_R8, "PF_SNORM_R8", PC_SE, PT_BYTE, 1, 1);
//    set_fmt(PF_SNORM_RG8, "PF_SNORM_RG8", PC_SE, PT_BYTE, 2, 2);
//    set_fmt(PF_SNORM_RGB8, "PF_SNORM_RGB8", PC_SE, PT_BYTE, 3, 3);
//    set_fmt(PF_SNORM_RGBA8, "PF_SNORM_RGBA8", PC_SEA, PT_BYTE, 4, 4);
//    set_fmt(PF_SNORM_R16, "PF_SNORM_R16", PC_SE, PT_BYTE, 1, 2);
//    set_fmt(PF_SNORM_RG16, "PF_SNORM_RG16", PC_SE, PT_BYTE, 2, 4);
//    set_fmt(PF_SNORM_RGB16, "PF_SNORM_RGB16", PC_SE, PT_BYTE, 3, 6);
//    set_fmt(PF_SNORM_RGBA16, "PF_SNORM_RGBA16", PC_SEA, PT_BYTE, 4, 8);
//    
//    set_fmt(PF_FLOAT_R16, "PF_FLOAT_R16", PC_F, PT_FLOAT16, 1, 2);
//    set_fmt(PF_FLOAT_GR16, "PF_FLOAT_GR16", PC_F, PT_FLOAT16, 2, 4);
//    set_fmt(PF_FLOAT_RGB16, "PF_FLOAT_RGB16", PC_F, PT_FLOAT16, 3, 6);
//    set_fmt(PF_FLOAT_RGBA16, "PF_FLOAT_RGBA16", PC_FA, PT_FLOAT16, 4, 8);
//    
//    set_fmt(PF_FLOAT_R32, "PF_FLOAT_R32", PC_F, PT_FLOAT32, 1, 4);
//    set_fmt(PF_FLOAT_GR32, "PF_FLOAT_GR32", PC_F, PT_FLOAT32, 2, 8);
//    set_fmt(PF_FLOAT_RGB32, "PF_FLOAT_RGB32", PC_F, PT_FLOAT32, 3, 12);
//    set_fmt(PF_FLOAT_RGBA32, "PF_FLOAT_RGBA32", PC_FA, PT_FLOAT32, 4, 16);
//    
//    set_fmt(PF_FLOAT_R11G11B10, "PF_FLOAT_R11G11B10", PC_FE, PT_FLOAT32, 1, 4);
//    
//    set_fmt(PF_R8G8B8, "PF_R8G8B8", PC_E, PT_BYTE, 3, 3);
//    set_fmt(PF_B8G8R8, "PF_B8G8R8", PC_E, PT_BYTE, 3, 3);
//    set_fmt(PF_R8G8B8A8, "PF_R8G8B8A8", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_B8G8R8A8, "PF_B8G8R8A8", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_A8R8G8B8, "PF_A8R8G8B8", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_A8B8G8R8, "PF_A8B8G8R8", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_X8R8G8B8, "PF_X8R8G8B8", PC_E, PT_BYTE, 4, 4);
//    set_fmt(PF_X8B8G8R8, "PF_X8B8G8R8", PC_E, PT_BYTE, 4, 4);
//    // srgb
//    set_fmt(PF_R8G8B8_SRGB, "PF_R8G8B8_SRGB", PC_E, PT_BYTE, 3, 3);
//    set_fmt(PF_B8G8R8_SRGB, "PF_B8G8R8_SRGB", PC_E, PT_BYTE, 3, 3);
//    set_fmt(PF_R8G8B8A8_SRGB, "PF_R8G8B8A8_SRGB", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_B8G8R8A8_SRGB, "PF_B8G8R8A8_SRGB", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_A8R8G8B8_SRGB, "PF_A8R8G8B8_SRGB", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_A8B8G8R8_SRGB, "PF_A8B8G8R8_SRGB", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_X8R8G8B8_SRGB, "PF_X8R8G8B8_SRGB", PC_E, PT_BYTE, 4, 4);
//    set_fmt(PF_X8B8G8R8_SRGB, "PF_X8B8G8R8_SRGB", PC_E, PT_BYTE, 4, 4);
//    
//    set_fmt(PF_D32_FLOAT_S8X24_UINT, "PF_D32_FLOAT_S8X24_UINT", PC_DS, PT_UINT, 2, 8);
//    set_fmt(PF_D24_UNORM_S8_UINT, "PF_D24_UNORM_S8_UINT", PC_DS, PT_UINT, 2, 4);
//    set_fmt(PF_D32_FLOAT, "PF_D32_FLOAT", PC_D, PT_FLOAT32, 1, 4);
//    set_fmt(PF_D16_UNORM, "PF_D16_UNORM", PC_D, PT_SHORT, 1, 2);
//    
//    set_fmt(PF_L8, "PF_L8", PC_LE, PT_BYTE, 1, 1);
//    set_fmt(PF_L16, "PF_L16", PC_LE, PT_BYTE, 1, 2);
//    set_fmt(PF_A8, "PF_A8", PC_EA, PT_BYTE, 1, 1);
//    set_fmt(PF_A4L4, "PF_A4L4", PC_LEA, PT_BYTE, 2, 1);
//    set_fmt(PF_R5G6B5, "PF_R5G6B5", PC_E, PT_BYTE, 3, 2);
//    set_fmt(PF_B5G6R5, "PF_B5G6R5", PC_E, PT_BYTE, 3, 2);
//    set_fmt(PF_R3G3B2, "PF_R3G3B2", PC_E, PT_BYTE, 3, 1);
//    set_fmt(PF_A4R4G4B4, "PF_A4R4G4B4", PC_EA, PT_BYTE, 4, 2);
//    set_fmt(PF_A1R5G5B5, "PF_A1R5G5B5", PC_EA, PT_BYTE, 4, 2);
//    set_fmt(PF_A2R10G10B10, "PF_A2R10G10B10", PC_EA, PT_BYTE, 4, 4);
//    set_fmt(PF_A2B10G10R10, "PF_A2B10G10R10", PC_E, PT_BYTE, 4, 4);
//    set_fmt(PF_R9G9B9E5, "PF_R9G9B9E5", PC_E, PT_BYTE, 4, 4);
//    
//    set_fmt(PF_BC1_UNORM, "PF_BC1_UNORM", PC_C,  PT_BYTE, 3, 8);
//    set_fmt(PF_BC2_UNORM, "PF_BC2_UNORM", PC_CA, PT_BYTE, 4, 16);
//    set_fmt(PF_BC3_UNORM, "PF_BC3_UNORM", PC_CA, PT_BYTE, 4, 16);
//    set_fmt(PF_BC4_UNORM, "PF_BC4_UNORM", PC_C,  PT_BYTE, 1, 8);
//    set_fmt(PF_BC4_SNORM, "PF_BC4_SNORM", PC_CS, PT_BYTE, 1, 8);
//    set_fmt(PF_BC5_UNORM, "PF_BC5_UNORM", PC_C,  PT_BYTE, 2, 16);
//    set_fmt(PF_BC5_SNORM, "PF_BC5_SNORM", PC_CS, PT_BYTE, 2, 16);
//    set_fmt(PF_BC6H_UF16, "PF_BC6H_UF16", PC_C,  PT_BYTE, 3, 16);
//    set_fmt(PF_BC6H_SF16, "PF_BC6H_SF16", PC_C,  PT_BYTE, 3, 16);
//    set_fmt(PF_BC7_UNORM, "PF_BC7_UNORM", PC_CA, PT_BYTE, 4, 16);
//    
//    set_fmt(PF_BC1_SRGB, "PF_BC1_SRGB", PC_CA, PT_BYTE, 4, 8);
//    set_fmt(PF_BC2_SRGB, "PF_BC2_SRGB", PC_CA, PT_BYTE, 4, 16);
//    set_fmt(PF_BC3_SRGB, "PF_BC3_SRGB", PC_CA, PT_BYTE, 4, 16);
//    set_fmt(PF_BC4_SRGB, "PF_BC4_SRGB", PC_CA, PT_BYTE, 4, 8);
//    set_fmt(PF_BC5_SRGB, "PF_BC5_SRGB", PC_CA, PT_BYTE, 4, 16);
//    set_fmt(PF_BC7_SRGB, "PF_BC7_SRGB", PC_CA, PT_BYTE, 4, 16);
//    
//    set_fmt(PF_ETC1_RGB8, "PF_ETC1_RGB8", PC_C, PT_BYTE, 3, 0);
//    set_fmt(PF_ETC2_RGB8, "PF_ETC2_RGB8", PC_C, PT_BYTE, 3, 0);
//    set_fmt(PF_ETC2_RGBA8, "PF_ETC2_RGBA8", PC_CA, PT_BYTE, 4, 0);
//    set_fmt(PF_ETC2_RGB8A1, "PF_ETC2_RGB8A1", PC_CA, PT_BYTE, 4, 0);
//    set_fmt(PF_ATC_RGB, "PF_ATC_RGB", PC_C, PT_BYTE, 3, 0);
//    set_fmt(PF_ATC_RGBA_EXPLICIT_ALPHA, "PF_ATC_RGBA_EXPLICIT_ALPHA", PC_CA, PT_BYTE, 4, 0);
//    set_fmt(PF_ATC_RGBA_INTERPOLATED_ALPHA, "PF_ATC_RGBA_INTERPOLATED_ALPHA", PC_CA, PT_BYTE, 4, 0);
//    
//    set_fmt(PF_PVRTC_RGB2, "PF_PVRTC_RGB2", PC_C, PT_BYTE, 3, 0);
//    set_fmt(PF_PVRTC_RGBA2, "PF_PVRTC_RGBA2", PC_CA, PT_BYTE, 4, 0);
//    set_fmt(PF_PVRTC_RGB4, "PF_PVRTC_RGB4", PC_C, PT_BYTE, 3, 0);
//    set_fmt(PF_PVRTC_RGBA4, "PF_PVRTC_RGBA4", PC_CA, PT_BYTE, 4, 0);
//    set_fmt(PF_PVRTC_2BPP, "PF_PVRTC_2BPP", PC_CA, PT_BYTE, 4, 0);
//    set_fmt(PF_PVRTC_4BPP, "PF_PVRTC_4BPP", PC_CA, PT_BYTE, 4, 0);
}

//static struct init_helper { init_helper(){ init_pixel_format(); } } g_helper;

inline static const PixelInfo& getDesc(RESOURCE_FORMAT fmt)
{
//    assert(fmt < PIXEL_FORMAT_MAX);
    return g_infos[(int)fmt];
}

uint PixelUtil::getBytes(RESOURCE_FORMAT fmt)
{
    return g_infos[fmt].bytes;
}

uint PixelUtil::getComponents(RESOURCE_FORMAT fmt)
{
    return g_infos[fmt].components;
}

uint PixelUtil::getMemorySize(RESOURCE_FORMAT fmt, uint width, uint height, uint depth)
{
    // DXT:
    // DXT formats work by dividing the image into 4x4 blocks, then encoding each
    // 4x4 block with a certain number of bytes.
    // PVRTC:
    // Size calculations from the PVRTC OpenGL extension spec
    // http://www.khronos.org/registry/gles/extensions/IMG/IMG_texture_compression_pvrtc.txt
    // Basically, 32 bytes is the minimum texture size.  Smaller textures are padded up to 32 bytes
    if (isCompressed(fmt))
    {
        switch (fmt)
        {
            case RESOURCE_FORMAT_BC1_UNORM:
                return ((width + 3) / 4)*((height + 3) / 4) * 8 * depth;
            case RESOURCE_FORMAT_BC2_UNORM:
            case RESOURCE_FORMAT_BC3_UNORM:
                return ((width + 3) / 4)*((height + 3) / 4) * 16 * depth;
            case RESOURCE_FORMAT_BC4_SNORM:
            case RESOURCE_FORMAT_BC4_UNORM:
                return ((width + 3) / 4)*((height + 3) / 4) * 8 * depth;
            case RESOURCE_FORMAT_BC5_SNORM:
            case RESOURCE_FORMAT_BC5_UNORM:
            case RESOURCE_FORMAT_BC6H_SF16:
            case RESOURCE_FORMAT_BC6H_UF16:
            case RESOURCE_FORMAT_BC7_UNORM:
            case RESOURCE_FORMAT_BC7_UNORM_SRGB:
                return ((width + 3) / 4)*((height + 3) / 4) * 16 * depth;
            case RESOURCE_FORMAT_PVRTC_RGB2:
            case RESOURCE_FORMAT_PVRTC_RGBA2:
            case RESOURCE_FORMAT_PVRTC_2BPP:
                return (std::max((int)width, 16) * std::max((int)height, 8) * 2 + 7) / 8;
            case RESOURCE_FORMAT_PVRTC_RGB4:
            case RESOURCE_FORMAT_PVRTC_RGBA4:
            case RESOURCE_FORMAT_PVRTC_4BPP:
                return (std::max((int)width, 8) * std::max((int)height, 8) * 4 + 7) / 8;
            case RESOURCE_FORMAT_ETC1_RGB8:
            case RESOURCE_FORMAT_ETC2_RGB8:
            case RESOURCE_FORMAT_ETC2_RGBA8:
            case RESOURCE_FORMAT_ETC2_RGB8A1:
                return ((width * height) >> 1);
            case RESOURCE_FORMAT_ATC_RGB:
                return ((width + 3) / 4) * ((height + 3) / 4) * 8;
            case RESOURCE_FORMAT_ATC_RGBA_EXPLICIT_ALPHA:
            case RESOURCE_FORMAT_ATC_RGBA_INTERPOLATED_ALPHA:
                return ((width + 3) / 4) * ((height + 3) / 4) * 16;
            default:
                throw std::runtime_error("error format");
                return 0;
        }
    }
    else
    {
        return width*height*depth*getBytes(fmt);
    }
}

uint PixelUtil::getImageSize(RESOURCE_FORMAT fmt, uint width, uint height, uint depth, uint mipmaps, uint faces)
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

bool PixelUtil::hasFlags(RESOURCE_FORMAT fmt, uint32_t flags)
{
    return (getDesc(fmt).flags & flags) == flags;
}

bool PixelUtil::isAlpha(RESOURCE_FORMAT fmt)
{
    return hasFlags(fmt, PC_A);
}

bool PixelUtil::isFloat(RESOURCE_FORMAT fmt)
{
    return hasFlags(fmt, PC_FLOAT);
}

bool PixelUtil::isDepth(RESOURCE_FORMAT fmt)
{
    return hasFlags(fmt, PC_D);
}

bool PixelUtil::isStencil(RESOURCE_FORMAT fmt)
{
    return hasFlags(fmt, PC_S);
}

bool PixelUtil::isLuminance(RESOURCE_FORMAT fmt)
{
    return hasFlags(fmt, PC_LUMINANCE);
}

bool PixelUtil::isNormalized(RESOURCE_FORMAT fmt)
{
    return hasFlags(fmt, PC_NORM);
}

bool PixelUtil::isCompressed(RESOURCE_FORMAT fmt)
{
    return hasFlags(fmt, PC_COMPRESSED);
}

CUTE_NS_END
