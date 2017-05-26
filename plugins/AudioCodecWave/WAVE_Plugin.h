#pragma once
#include "WAVE_API.h"
#include "Cute/Plugin.h"

CUTE_NS_BEGIN

#define WAVE_NAME "WAVE"

class WaveDecoder;
class CUTE_WAVE_API WavePlugin : public Plugin
{
public:
	WavePlugin();
	~WavePlugin();

	void install();
	void uninstall();

private:
	String		 m_ext;
	WaveDecoder* m_decoder;
};

CUTE_NS_END
