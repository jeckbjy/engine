#pragma once
#include "OGG_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define OGG_NAME "OGG"

class OGGDecoder;
class CUTE_OGG_API OGGPlugin : public Plugin
{
public:
	OGGPlugin();
	~OGGPlugin();

	void install();
	void uninstall();

private:
	String		m_ext;
	OGGDecoder*	m_decoder;
};

CUTE_NS_END
