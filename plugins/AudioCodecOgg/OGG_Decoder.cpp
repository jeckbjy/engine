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
	
	return true;
}

uint OGGDecoder::read(uint8* buffer, uint length)
{
	//uint 
	//uint numReadSamples = 0;
	//while (numReadSamples < length)
	//{
	//	int byteToRead = (int)(length - numReadSamples) * sizeof(int16_t);
	//	long bytesRead = ov_read(&m_file, (char*)buffer, byteToRead, 0, 2, 1, NULL);
	//	if (bytesRead <= 0)
	//		break;

	//	numReadSamples += 1;
	//	samples += 
	//}

	return 0;
}

void OGGDecoder::seek(uint offset)
{
	ov_pcm_seek(&m_file, offset / m_channels);
}

CUTE_NS_END
