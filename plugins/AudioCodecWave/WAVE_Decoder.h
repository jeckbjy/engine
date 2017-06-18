#pragma once
#include "WAVE_API.h"
#include "Cute/AudioDecoder.h"

CUTE_NS_BEGIN

// http://blog.chinaunix.net/uid-11857489-id-2814493.html
// http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff538799(v=vs.85).aspx
class CUTE_WAVE_API WaveDecoder : public AudioDecoder
{
public:
	WaveDecoder();
	~WaveDecoder();

	bool check(Stream* stream);
	bool open(Stream* stream) OVERRIDE;
	uint read(uint8* samples, uint count) OVERRIDE;
	void seek(uint offset) OVERRIDE;

private:
	bool parseHeader();
	enum
	{
		MAIN_CHUNK_SIZE = 12,
	};

	Stream*		m_stream;
	size_t		m_dataOffset;	// 数据源起始位置
	size_t		m_bytes;		// 每个采样占用的字节数
};

CUTE_NS_END
