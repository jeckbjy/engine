#include "WAVE_Plugin.h"
#include "WAVE_Decoder.h"
#include "Cute/Audio.h"

CUTE_NS_BEGIN

DEF_PLUGIN(CUTE_WAVE_API, WavePlugin)

WavePlugin::WavePlugin()
	: Plugin(WAVE_NAME)
	, m_ext("wav")
	, m_decoder(NULL)
{
}

WavePlugin::~WavePlugin()
{
	uninstall();
}

void WavePlugin::install()
{
	if (m_decoder == NULL)
		m_decoder = new WaveDecoder();

	Audio::instance().addDecoder(m_ext, m_decoder);
}

void WavePlugin::uninstall()
{
	if (m_decoder != NULL)
	{
		Audio::instance().delDecoder(m_ext);
		CUTE_DELETE(m_decoder);
	}
}

CUTE_NS_END
