//! Audio
#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// 编码器
class AudioEncoder : public Object
{
	DECLARE_RTTI(AudioEncoder, Object, OID_ANY)
public:
	virtual ~AudioEncoder(){}

	virtual bool open() = 0;
	virtual void close() = 0;
	virtual void flush() = 0;
	virtual void write(const void* samples, size_t length) = 0;
};

CUTE_NS_END
