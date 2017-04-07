//! SharedLibrary
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

Plugin::Plugin(const String& name)
	: m_name(name)
{
}

Plugin::~Plugin()
{

}

void Plugin::release()
{
	delete this;
}

CUTE_NS_END