#include "OGG_Decoder.h"
#include "Cute/Stream.h"

CUTE_NS_BEGIN

size_t ogg_read(void* ptr, size_t size, size_t nmemb, void* data)
{
	OGGDecoder* decoder = static_cast<OGGDecoder*>(data);
	Stream* stream = decoder->getStream();
	return (size_t)stream->read(ptr, size * nmemb);
}

int ogg_seek(void* data, ogg_int64_t offset, int whence)
{
	OGGDecoder* decoder = static_cast<OGGDecoder*>(data);
	Stream* stream = decoder->getStream();

	stream->seek((long)offset, whence);
	return (int)(stream->position());
}

long ogg_tell(void* data)
{
	OGGDecoder* decoder = static_cast<OGGDecoder*>(data);
	Stream* stream = decoder->getStream();
	return (long)stream->position();
}

OGGDecoder::OGGDecoder()
{
	memset(&m_file, 0, sizeof(m_file));

	m_file.callbacks = { &ogg_read, &ogg_seek, NULL, &ogg_tell };
}

OGGDecoder::~OGGDecoder()
{
	if (m_file.datasource != NULL)
	{
		ov_clear(&m_file);
	}
}

bool OGGDecoder::open(Stream* stream)
{
	ov_callbacks callbacks = {};
	int status = ov_open_callbacks(this, &m_file, NULL, 0, m_file.callbacks);
	if (status < 0)
	{
		return false;
	}

	vorbis_info* info = ov_info(&m_file, -1);
	m_info.samples = (uint)ov_pcm_total(&m_file, -1);
	m_info.channels = info->channels;
	m_info.rate = info->rate;
	m_info.bits = 16;
	return true;
}

uint OGGDecoder::read(uint8* samples, uint count)
{
	uint numReadSamples = 0;
	while (numReadSamples < count)
	{
		int bytesToRead = (count - numReadSamples) * sizeof(int16);
		uint bytesRead = ov_read(&m_file, (char*)samples, bytesToRead, 0, 2, 1, NULL);
		if(bytesToRead <= 0)
			break;

		uint32 samplesRead = bytesRead / sizeof(int16);
		numReadSamples += samplesRead;
		samples += samplesRead * sizeof(int16);
	}

	return numReadSamples;
}

void OGGDecoder::seek(uint offset)
{
	ov_pcm_seek(&m_file, offset / m_info.channels);
}

CUTE_NS_END
