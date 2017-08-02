//! Components
#include "Cute/Animation.h"

CUTE_NS_BEGIN

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::setWrapMode(WrapMode mode)
{
    m_mode = mode;
}

void Animation::setSpeed(float speed)
{
    m_speed = speed;
}

void Animation::setBounds(const AABox &bounds)
{
    m_bounds = bounds;
}

void Animation::addClip(AnimationClip *clip, const String &name, int firstFrame, int lastFrame)
{
    ClipInfo info;
    info.clip = clip;
    info.name = name;
    info.firstFrame= firstFrame;
    info.lastFrame = lastFrame;
    m_clips.push_back(info);
}

AnimationState* Animation::getState(const String &name) const
{
    foreach(AnimationState* state, m_states)
    {
        if(state->name == name)
            return state;
    }
    
    return NULL;
}

CUTE_NS_END
