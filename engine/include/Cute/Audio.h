#pragma once
#include "Cute/Foundation.h"
#include "Cute/Module.h"
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

class Stream;
class AudioClip;
class AudioSource;
class AudioListener;
class CUTE_CORE_API Audio : public Module<Audio>
{
public:
	typedef std::vector<String> DeviceList;

	Audio();
	virtual ~Audio();

	void play(AudioClip* clip, const Vector3& position = Vector3::ZERO, float volume = 1.0f);

	/** Called once per frame. Queues streaming audio requests. */
	virtual void update();

	virtual void setVolume(float volume) = 0;
	virtual void setPaused(bool paused) = 0;
	virtual void setActiveDevice(const String& name) = 0;

	virtual float getVolume() const = 0;
	virtual bool  isPaused() const = 0;

	virtual String getActiveDevice() const = 0;
	virtual String getDefaultDevice() const = 0;
	virtual const DeviceList& getAllDevices() const = 0;

protected:
	virtual AudioClip*   createClip(Stream* samples, uint32 streamSize, uint32 sampleNum) = 0;
	virtual AudioSource* createSource() = 0;
	virtual AudioSource* createListener() = 0;

private:
	friend class AudioClip;
	friend class AudioSource;
	friend class AudioListener;
	typedef std::vector<AudioSource*> SourceList;

	SourceList m_manuals;
	SourceList m_temps;
};

CUTE_NS_END
