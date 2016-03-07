#pragma once
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

	bool		enabled;
	CommandType type;
	String		tag;
	String		meta;
	OutputList	outputs;
	Clear		clear;
};

enum SizeMode
{
	SIZE_MODE_ABSOLUTE,		// ����ֵ
	SIZE_MODE_DIVISOR,		// ������
	SIZE_MODE_MULTIPLIER	// ������
};

// ��ȾĿ�꣬Ĭ��ֻ��һ��viewport
struct RenderTargetInfo
{
	String	name;
	String	tag;
	uint	format;
	Vector2	size;
	SizeMode mode;
	bool	cubemap;
	bool	filtered;
	bool	sRGB;
	bool	persistent;
};

// ��Ⱦ·��
class CU_API RenderPath
{
public:
	typedef Vector<RenderTargetInfo> TargetList;
	typedef Vector<RenderCommand>	CommandList;
	TargetList	targets;
	CommandList commands;

	bool load(XMLFile* file);
	bool append(XMLFile* file);

private:
	void loadTarget(const XMLNode& node);
	void loadCommand(const XMLNode& node);
};

CU_NS_END