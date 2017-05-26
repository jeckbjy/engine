#pragma once
#include "OGG_API.h"
#include "Cute/AudioDecoder.h"
#include "vorbis/vorbisfile.h"

CUTE_NS_BEGIN

class CUTE_OGG_API OGGDecoder : public AudioDecoder
{
public:
	OGGDecoder();
	~OGGDecoder();

	bool open(Stream* stream) OVERRIDE;
	uint read(uint8* buffer, uint length) OVERRIDE;
	void seek(uint offset) OVERRIDE;

	Stream* getStream() { return m_stream; }
	size_t	getOffset() { return m_offset; }

private:
	Stream*			m_stream;
	OggVorbis_File	m_file;
	size_t			m_channels;
	size_t			m_offset;
};

CUTE_NS_END
