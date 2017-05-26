#include "WAVE_Decoder.h"
#include "Cute/Stream.h"

CUTE_NS_BEGIN

WaveDecoder::WaveDecoder()
{
}

WaveDecoder::~WaveDecoder()
{
}

bool WaveDecoder::check(Stream* stream)
{
	char header[MAIN_CHUNK_SIZE];
	if (!stream->read(header, sizeof(header)))
		return false;

	return (header[0] == 'R') && (header[1] == 'I') && (header[2] == 'F') && (header[3] == 'F') &&
		   (header[8] == 'W') && (header[9] == 'A') && (header[10] == 'V') && (header[11] == 'E');
}

bool WaveDecoder::open(Stream* stream)
{
	m_stream = stream;

	if (!check(stream))
		return false;

	if (!parseHeader())
		return false;

	return true;
}

uint WaveDecoder::read(uint8* buffer, uint count)
{
	uint numRead = m_stream->read(buffer, count * m_bytes);
	// 8-bit samples are stored as unsigned, but engine convention is to store all bit depths as signed
	if (m_bytes == 1)
	{
		for (uint i = 0; i < numRead; ++i)
		{
			int8 val = buffer[i] - 128;
			buffer[i] = *((uint8*)&val);
		}
	}

	return numRead;
}

void WaveDecoder::seek(uint offset)
{
	m_stream->seek(offset, SEEK_SET);
}

bool WaveDecoder::parseHeader()
{
	return true;
}

CUTE_NS_END
