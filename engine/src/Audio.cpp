//! Audio
#include "Cute/Audio.h"

CUTE_NS_BEGIN

Audio::Audio()
{
}

Audio::~Audio()
{
}

void Audio::play(AudioClip* clip, const Vector3& position /* = Vector3::ZERO */, float volume /* = 1.0f */)
{

}

void Audio::update()
{

}

void Audio::addDecoder(const String& ext, AudioDecoder* decoder)
{
	if (decoder == NULL)
		return;

	m_decoders[ext] = decoder;
}

void Audio::delDecoder(const String& ext)
{
	m_decoders.erase(ext);
}

AudioDecoder* Audio::getDecoder(const String& ext)
{
	DecoderMap::iterator itor = m_decoders.find(ext);
	if (itor != m_decoders.end())
		return itor->second;

	return NULL;
}

void Audio::addEncoder(const String& ext, AudioEncoder* encoder)
{
	if (encoder == NULL)
		return;

	m_encoders[ext] = encoder;
}

void Audio::delEncoder(const String& ext)
{
	m_encoders.erase(ext);
}

AudioEncoder* Audio::getEncoder(const String& ext)
{
	EncoderMap::iterator itor = m_encoders.find(ext);
	if (itor != m_encoders.end())
		return itor->second;

	return NULL;
}

CUTE_NS_END
