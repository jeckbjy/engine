#include "RenderPath.h"
#include "PixelFormat.h"
#include "XMLFile.h"
#include "Util.h"

CU_NS_BEGIN

bool RenderPath::load(XMLFile* file)
{
	targets.clear();
	commands.clear();
	return append(file);
}

bool RenderPath::append(XMLFile* file)
{
	XMLNode root = file->getRoot();
	if (!root)
		return false;
	XMLNode node = root.getChild("rendertarget");
	while (node)
	{
		loadTarget(node);
		node = node.getNext("rendertarget");
	}

	node = root.getChild("command");
	while (node)
	{
		loadCommand(node);
		node = node.getNext("command");
	}
	return true;
}

void RenderPath::loadTarget(const XMLNode& node)
{
	RenderTargetInfo info;
	info.name = node.getAttribute("name");
	if (info.name.empty())
		return;
	info.tag = node.getAttribute("tag");
	String format = node.getAttribute("format");
	info.format = PixelUtil::getFormat(format);
	if (node.hasAttribute("filter"))
		info.filtered = node.getAttribute<bool>("filter");

	if (node.hasAttribute("size"))
	{
		info.mode = SIZE_MODE_ABSOLUTE;
		node.getAttribute("size", info.size);
	}
	else if (node.hasAttribute("divisor"))
	{
		info.mode = SIZE_MODE_DIVISOR;
		node.getAttribute("divisor", info.size);
	}
	else if (node.hasAttribute("multiplier"))
	{
		info.mode = SIZE_MODE_MULTIPLIER;
		node.getAttribute("multiplier", info.size);
	}
}

void RenderPath::loadCommand(const XMLNode& node)
{
	static const char* STR_CMD_TYPE[] = 
	{
		"none",
		"clear",
		"scenepass",
		"quad",
		"forwardlights",
		"lightvolumes",
		"renderui",
		0
	};
	
	static const char* STR_SORT_MODE[] = 
	{
		"fronttoback",
		"backtofront",
		0
	};

	RenderCommand cmd;
	cmd.type = (CommandType)Util::indexOf(node.getAttribute("type"), STR_CMD_TYPE, CMD_CLEAR);
	if (cmd.type == CMD_CLEAR)
		return;
	cmd.tag = node.getAttribute("tag");
	if (node.hasAttribute("enable"))
		cmd.enabled = node.getAttribute<bool>("enabled");
	if (node.hasAttribute("meta"))
		cmd.meta = node.getAttribute("meta");
	switch (cmd.type)
	{
	case CMD_CLEAR:
	{
		if (node.hasAttribute("color"))
		{

		}
	}
	break;
	case CMD_SCENE_PASS:
	{
	}
	break;
	}
}

CU_NS_END
