//! Audio
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

class Stream;
// “Ù∆µΩ‚¬Î∆˜
class AudioDecoder : public Object
{
	DECLARE_RTTI(AudioDecoder, Object, OID_ANY)
public:
	virtual ~AudioDecoder(){}

	virtual bool open(Stream* stream) = 0;
	virtual uint read(uint8* buffer, uint length) = 0;
	virtual void seek(uint offset) = 0;
};

CUTE_NS_END
