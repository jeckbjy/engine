#pragma once
#include "OAL_API.h"
#include "Cute/AudioClip.h"
#include "Cute/Mutex.h"

CUTE_NS_BEGIN

class CUTE_OAL_API OALAudioClip : public AudioClip
{
public:
private:
	mutable Mutex m_mutex;
	uint32 m_bufferID;
};

CUTE_NS_END
