#include "OIS_Plugin.h"
#include "OIS_InputHandler.h"
#include "Cute/Input.h"

CUTE_NS_BEGIN

DEF_PLUGIN(CUTE_OIS_API, OISPlugin)

void OISPlugin::install()
{
	Input::instance().setHandler(new OISInputHandler());
}

void OISPlugin::uninstall()
{
	Input::instance().setHandler(NULL);
}

CUTE_NS_END
