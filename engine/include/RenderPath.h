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
		uint8_t	index;	// �����ⲿ���
	};

	typedef Vector<Output> OutputList;
	CommandType type;
	String		tag;
	OutputList	outputs;
	// ����union����
	Clear		clear;
	// ���� pass light
};

enum SizeMode
{
	SIZE_ABSOLUTE,		// ����ֵ
	SIZE_DIVISOR,		// ������
	SIZE_MULTIPLIER		// ������
};

// ��ȾĿ�꣬Ĭ��ֻ��һ��viewport
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

// ��Ⱦ·��
struct CU_API RenderPath
{
	typedef Vector<RenderTargetInfo> TargetList;
	typedef Vector<RenderCommand>	CommandList;
	TargetList	targets;
	CommandList commands;

	bool load();
};

CU_NS_END