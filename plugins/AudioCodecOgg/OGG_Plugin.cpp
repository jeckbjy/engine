#include "OGG_Plugin.h"
#include "OGG_Decoder.h"
#include "Cute/Audio.h"

CUTE_NS_BEGIN

DEF_PLUGIN(CUTE_OGG_API, OGGPlugin)

OGGPlugin::OGGPlugin()
	: Plugin(OGG_NAME)
	, m_ext("ogg")
	, m_decoder(NULL)
{
}

OGGPlugin::~OGGPlugin()
{
	uninstall();
}

void OGGPlugin::install()
{
	if (m_decoder == NULL)
		m_decoder = new OGGDecoder();

	Audio::instance().addDecoder(m_ext, m_decoder);
}

void OGGPlugin::uninstall()
{
	if (m_decoder != NULL)
	{
		Audio::instance().delDecoder(m_ext);
		CUTE_DELETE(m_decoder);
	}
}

CUTE_NS_END
