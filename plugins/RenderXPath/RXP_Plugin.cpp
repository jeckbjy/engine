#include "RXP_Plugin.h"
#include "RXP_API.h"

CUTE_NS_BEGIN

#define RENDER_XPATH_NAME "RenderXPath"

DEF_PLUGIN(CUTE_RXP_API, RXP_Plugin)

RXP_Plugin::RXP_Plugin()
    : Plugin(RENDER_XPATH_NAME)
{
}

void RXP_Plugin::install()
{

}

void RXP_Plugin::uninstall()
{

}

CUTE_NS_END
