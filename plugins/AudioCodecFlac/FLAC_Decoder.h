#pragma once
#include "FLAC_API.h"
#include "Cute/AudioDecoder.h"
#include "FLAC/stream_decoder.h"

CUTE_NS_BEGIN

class CUTE_FLAC_API FLACDecoder : public AudioDecoder
{
public:
	FLACDecoder();
	~FLACDecoder();

	bool open(const Stream* stream) OVERRIDE;
	uint read(uint8* buffer, uint length) OVERRIDE;
	void seek(uint offset) OVERRIDE;
	void close();

	Stream* getStream() { return m_stream; }

private:
	typedef FLAC__StreamDecoder Decoder;
	Decoder*	m_decoder;
	Stream*		m_stream;
};

CUTE_NS_END
