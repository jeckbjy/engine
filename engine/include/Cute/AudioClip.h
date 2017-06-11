#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

//enum AudioFormat
//{
//	AUDIO_FORMAT_PCM,		/**< Pulse code modulation audio ("raw" uncompressed audio). */
//	AUDIO_FORMAT_VORBIS,	/**< Vorbis compressed audio. */
//};

enum AudioReadMode
{
	/** Entire audio clip will be loaded and decompressed. Uses most memory but has lowest CPU impact. */
	AUDIO_READ_DECOMPRESSED,
	/**
	* Entire audio clip will be loaded, but will be decompressed while playing. Uses medium amount of memory and has
	* the highest CPU impact.
	*/
	AUDIO_READ_COMPRESSED,
	/**
	* Audio will be slowly streamed from the disk, and decompressed if needed. Uses very little memory, and has either
	* low or high CPU impact depending if the audio is in a compressed format. Since data is streamed from the disk,
	* read speeds could also be a bottleneck.
	*/
	AUDIO_READ_STREAM
};

struct CUTE_CORE_API AudioClipDesc
{
	AudioReadMode	readMode;
	//AudioFormat		format;
	uint32			frequency;
	uint32			bitDepth;
	uint32			numChannels;
	bool			is3D;
	/**
	* Determines should the audio clip keep the original data in memory after creation. For example if the audio data
	* is normally compressed, but audio clip uncompresses it on load, the original compressed data will be lost unless
	* this is enabled. This will cause extra memory to be used, but can be useful in certain circumstances (for example
	* you might require that data to save the audio clip on disk).
	*
	* When loading audio clip directly from disk, this properly is controlled by the ResourceLoadFlag::KeepSourceData.
	*/
	bool			keepSourceData;

	AudioClipDesc()
		: readMode(AUDIO_READ_DECOMPRESSED)
		, format(AUDIO_FORMAT_PCM)
		, frequency(44100)
		, bitDepth(16)
		, numChannels(2)
		, is3D(true)
		, keepSourceData(true)
	{
	}
};

class Stream;
class AudioDecoder;
// 分成两种方式：立即解码，Stream必须是MemoryStream，和延迟解码
class CUTE_CORE_API AudioClip : public Object
{
	DECLARE_RTTI(AudioClip, Object, OID_ANY)
public:
	~AudioClip();

	//AudioFormat		getFormat() const { return m_desc.format; }
	AudioReadMode	getReadMode() const { return m_desc.readMode; }

	uint32 getBitDepth() const { return m_desc.bitDepth; }
	uint32 getFrequency() const { return m_desc.frequency; }
	uint32 getNumChannels() const { return m_desc.numChannels; }
	uint32 getNumSamples() const { return m_numSamples; }
	float  getLength() const { return m_length; }

	bool   is3D() const { return m_desc.is3D; }

protected:
	AudioClip(Stream* stream, size_t streamSize, size_t numSamples, const AudioClipDesc& desc);

protected:
	AudioClipDesc	m_desc;
	uint32			m_numSamples;
	uint32			m_streamSize;
	uint32			m_streamOffset;
	float			m_length;
	Stream*			m_stream;
	AudioDecoder*	m_decoder;
};

CUTE_NS_END
