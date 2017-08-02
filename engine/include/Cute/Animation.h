#pragma once
#include "Cute/Foundation.h"
#include "Cute/Component.h"
#include "Cute/AnimationClip.h"
#include "Cute/AABox.h"

CUTE_NS_BEGIN

enum PlayMode
{
    PM_STOP_SAME_LAYER,
    PM_STOP_ALL,
};

struct AnimationState
{
    const AnimationClip* clip;
    String      name;
    float       time;
    float       speed;
    float       weight;
    WrapMode    wrapMode;
};

// 模拟动画播放,Animator??? AnimationLayer, AnimationController
class CUTE_CORE_API Animation : public Component
{
public:
	Animation();
	~Animation();
    
    void setWrapMode(WrapMode mode);
    void setSpeed(float speed);
    void setBounds(const AABox& bounds);
    
    void addClip(AnimationClip* clip, const String& name, int firstFrame, int lastFrame);
    
    void play(const String& name, enum PlayMode mode = PM_STOP_SAME_LAYER);
    void crossFade(const String& name);
    void sample();
    void stop();
    void stopAll();
    
    bool isPlaying() const;
    uint getClipCount() const;
    
    const AnimationClip* getClip(const String& name) const;
    AnimationState*      getState(const String& name) const;
    
private:
    struct ClipInfo
    {
        AnimationClip* clip;
        String name;
        int firstFrame;
        int lastFrame;
    };
    typedef Vector<ClipInfo>        ClipList;
    typedef Vector<AnimationState*> StateList;
    uint        m_id;
    bool        m_playAutomatically;
    WrapMode    m_mode;
    float       m_speed;
    AABox       m_bounds;
    ClipList    m_clips;
    StateList   m_states;
};

CUTE_NS_END
