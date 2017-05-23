#pragma once
#include "OAL_API.h"
#include "Cute/AudioListener.h"
#include "Cute/Array.h"

CUTE_NS_BEGIN

class CUTE_OAL_API OALAudioListener : public AudioListener
{
	DECLARE_RTTI(OALAudioListener, AudioListener, OID_ANY)

public:
	OALAudioListener(ALCdevice* device);
	~OALAudioListener();

	void setPosition(const Vector3& pos) OVERRIDE;
	void setDirection(const Vector3& dir) OVERRIDE;
	void setUp(const Vector3& up) OVERRIDE;
	void setVelocity(const Vector3& velocity) OVERRIDE;

private:
	void setCurrentContext();
	void rebuild();

	void updatePosition();
	void updateVelocity();
	void updateOrientation();
	void updateVolume(float volume);

private:
	ALCcontext* m_context;
};

CUTE_NS_END
