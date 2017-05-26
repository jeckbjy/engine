#pragma once
#include "WAVE_API.h"
#include "Cute/AudioDecoder.h"

CUTE_NS_BEGIN

class CUTE_WAVE_API WaveDecoder : public AudioDecoder
{
public:
	WaveDecoder();
	~WaveDecoder();

	bool check(Stream* stream) OVERRIDE;
	bool open(Stream* stream) OVERRIDE;
	uint read(uint8* buffer, uint count) OVERRIDE;
	void seek(uint offset) OVERRIDE;

private:
	bool parseHeader();
	enum
	{
		MAIN_CHUNK_SIZE = 12,
	};

	Stream* m_stream;
	size_t	m_bytes;	// ÿ������ռ�õ��ֽ���
};

CUTE_NS_END
