#include "Animation.h"
#include "Stream.h"

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
AnimTrack::AnimTrack()
{

}

AnimTrack::~AnimTrack()
{

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
AnimClip::AnimClip(AnimTrackSet* tracks, const String& name, uint start, uint end, WrapMode mode)
{

}

AnimClip::~AnimClip()
{

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Animation::Animation()
{

}

Animation::~Animation()
{

}

bool Animation::load(Stream* stream)
{
	return true;
}

void Animation::save(Stream* stream)
{

}

void Animation::addClip(AnimClip* clip)
{

}

AnimClip* Animation::getClip(const String& name)
{
	return NULL;
}

CU_NS_END
