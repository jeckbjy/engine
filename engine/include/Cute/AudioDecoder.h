//! Audio
#pragma once
#include "Cute/Foundation.h"
#include "Cute/AudioCommon.h"

CUTE_NS_BEGIN

class Stream;
// ��Ƶ������,ע��read��seek���������sample�ģ�������byte
class AudioDecoder : public Object
{
	DECLARE_RTTI(AudioDecoder, Object, OID_ANY)
public:
	virtual ~AudioDecoder(){}

	virtual bool open(Stream* stream) = 0;
	virtual uint read(uint8* samples, uint counts) = 0;
	virtual void seek(uint offset) = 0;

protected:
	AudioInfo m_info;
};

CUTE_NS_END
