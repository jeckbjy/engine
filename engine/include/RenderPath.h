#include "API.h"
#include "Color.h"
#include "Vector2.h"

CU_NS_BEGIN

enum CommandType
{
	CMD_NONE = 0,
	CMD_CLEAR,
	CMD_SCENE_PASS,
	CMD_QUAD,			// post process ??
	CMD_FORWARD_LIGHTS,	// cluster tiled
	CMD_DEFER_LIGHTS,	// CMD_LIGHT_VOLUMES
	CMD_RENDERUI
};

enum SortMode
{
	SORT_FRONT_TO_BACK,
	SORT_BACK_TO_FRONT,
};

struct RenderCommand
{
	struct Clear
	{
		uint	flags;
		Color	color;
		float	depth;
		uint	stencil;
	};

	struct Output
	{
		String	target;
		uint8_t	face;
		uint8_t	index;	// 索引外部填充
	};

	typedef Vector<Output> OutputList;
	CommandType type;
	String		tag;
	OutputList	outputs;
	// 参数union？？
	Clear		clear;
	// 其他 pass light
};

enum SizeMode
{
	SIZE_ABSOLUTE,		// 绝对值
	SIZE_DIVISOR,		// 除因子
	SIZE_MULTIPLIER		// 乘因子
};

// 渲染目标，默认只有一个viewport
struct RenderTargetInfo
{
	String	name;
	String	tag;
	size_t	format;
	Vector2	size;
	SizeMode mode;
	char	enable : 1;
	char	cubemap : 1;
	char	filtered : 1;
	char	sRGB : 1;
	char	persistent : 1;
};

// 渲染路径
struct CU_API RenderPath
{
	typedef Vector<RenderTargetInfo> TargetList;
	typedef Vector<RenderCommand>	CommandList;
	TargetList	targets;
	CommandList commands;

	bool load();
};

CU_NS_END