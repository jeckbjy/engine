#pragma once
#include "RXP_API.h"
#include "Cute/Vector2.h"

CUTE_NS_BEGIN

enum RenderCommandMode
{
    RENDER_CMD_NONE = 0,
    RENDER_CMD_CLEAR,
    RENDER_CMD_SCENE_PASS,
    RENDER_CMD_QUAD,
    RENDER_CMD_FORWARD_LIGHTS,
    RENDER_CMD_LIGHT_VOLUMES,
    RENDER_CMD_UI,
    RENDER_CMD_SEND_EVENT
};

enum RenderSortMode
{
    RENDER_SORT_FRONTTOBACK,
    RENDER_SORT_BACKTOFRONT
};

enum RenderSizeMode
{
    RENDER_SIZE_ABSOLUTE = 0,
    RENDER_SIZE_VIEWPORTDIVISOR,
    RENDER_SIZE_VIEWPORTMULTIPLIER
};

struct RenderTarget
{
    String  name;
    String  tag;
    uint    format;
    Vector2 size;
    int     sizeMode;
    /// Multisampling level (1 = no multisampling).
    int     multSample;
    /// Multisampling autoresolve flag.
    bool    autoResolve;
    bool    cubemap;
    bool    filtered;
    bool    sRGB;
    bool    persistent;
    bool    enabled;
};

struct RenderCommand
{
    RenderCommandMode   type;
    RenderSortMode      sort;
    String              tag;
    String              pass;
    uint                passIndex;
    String              metadata;
};

class CUTE_RXP_API RXP_RenderPath
{
public:
    typedef Vector<RenderTarget>    TargetArray;
    typedef Vector<RenderCommand>   CommandArray;
    
    void load();
    
public:
    TargetArray  targets;
    CommandArray commands;
};

CUTE_NS_END
