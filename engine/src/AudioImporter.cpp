#include "Cute/AudioImporter.h"
#include "Cute/AudioDecoder.h"
#include "Cute/Audio.h"

CUTE_NS_BEGIN

AudioImporter::AudioImporter()
	: ImporterSpecific()
{
}

AudioImporter::~AudioImporter()
{
}

Resource* AudioImporter::import(const String& path, const ImportOptions* options /* = NULL */)
{
	return NULL;
}

bool AudioImporter::isExtensionSupported(const String& ext) const
{
	AudioDecoder* decoder = Audio::instance().getDecoder(ext);
	return decoder != NULL;
}

CUTE_NS_END
