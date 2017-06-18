#include "WAVE_Decoder.h"
#include "Cute/Stream.h"

CUTE_NS_BEGIN

#define WAVE_FORMAT_PCM			0x0001
#define WAVE_FORMAT_IEEE_FLOAT	0x0003
#define WAVE_FORMAT_ALAW		0x0006
#define WAVE_FORMAT_MULAW		0x0007
#define WAVE_FORMAT_EXTENSIBLE	0xFFFE

struct WaveChunkFormat
{
	uint16 format;
	uint16 channels;		// Number of channels: 1-5
	uint32 samplesPerSec;	// In Hz
	uint32 argBytesPerSec;	// For estimating RAM allocation
	uint16 blockAlign;		// Sample frame size in bytes
	uint32 bitsPerSample;	// Bits per sample
};

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

	return memcmp(header, "RIFFWAVE", 8) == 0;
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

uint WaveDecoder::read(uint8* samples, uint count)
{
	uint numRead = m_stream->read(samples, count * m_bytes);
	// 8-bit samples are stored as unsigned, but engine convention is to store all bit depths as signed
	if (m_bytes == 1)
	{
		for (uint i = 0; i < numRead; ++i)
		{
			int8 val = samples[i] - 128;
			samples[i] = *((uint8*)&val);
		}
	}

	return numRead;
}

void WaveDecoder::seek(uint offset)
{
	m_stream->seek(m_dataOffset + m_bytes * offset, SEEK_SET);
}

bool WaveDecoder::parseHeader()
{
	bool foundData = false;
	while(!foundData)
	{
		if (m_stream->eof())
			return false;

		char   chunkId[4];
		uint32 chunkSize;

		if (!m_stream->read(chunkId, 4))
			return false;

		if (!m_stream->read(chunkSize))
			return false;

		if (memcmp(chunkId, "fmt ", 4) == 0)
		{
			// read format
			WaveChunkFormat fmt;
			if (!m_stream->read(fmt))
				return false;

			if (fmt.format != WAVE_FORMAT_PCM && fmt.format != WAVE_FORMAT_EXTENSIBLE)
			{
				return false;
			}

			if (fmt.format == WAVE_FORMAT_EXTENSIBLE)
			{
				uint16 extensionSize;
				if (!m_stream->read(extensionSize))
					return false;

				if (extensionSize != 22)
					return false;

				uint16 validBitDepth = 0;
				if (!m_stream->read(validBitDepth))
					return false;

				uint32 channelMask;
				if (!m_stream->read(channelMask))
					return false;

				uint8 subFormat[16];
				if (!m_stream->read(subFormat))
					return false;

				memcpy(&fmt.format, subFormat, sizeof(fmt.format));
				if (fmt.format != WAVE_FORMAT_PCM)
				{
					//LOGWRN("Wave file doesn't contain raw PCM data. Not supported.");
					return false;
				}
			}
			
			m_info.channels = fmt.channels;
			m_info.rate = fmt.samplesPerSec;
			m_info.bits = fmt.bitsPerSample;

			m_bytes = m_info.bits / 8;

		}
		else if (memcmp(chunkId, "data", 4) == 0)
		{
			m_info.samples = chunkSize / (m_info.bits / 8);
			foundData = true;
		}
		else
		{
			m_stream->skip(chunkSize);
		}
	}

	return true;
}

CUTE_NS_END
