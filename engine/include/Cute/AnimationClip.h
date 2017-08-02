#pragma once
#include "Cute/Foundation.h"
#include "Cute/AnimationCurve.h"
#include "Cute/Vector3.h"

CUTE_NS_BEGIN

struct CUTE_CORE_API AnimationEvent
{
    String  name;
    float   time;
    int     parami;
    String  params;
};

enum WrapMode
{
    WM_ONCE,
    WM_LOOP,
    WM_PINGPONG,
    WM_CLAMP,
    WM_DEFAULT,
};

// resource file
class CUTE_CORE_API AnimationClip : public TObject<AnimationClip>
{
public:
    AnimationClip();
    ~AnimationClip();
    
    void addCurve();
    
    void addEvent(const AnimationEvent& ev);
    void addEvent(const String& name, float time);
    
private:
    typedef TAnimationCurve<Vector3> AnimationCurve;
    typedef Vector<AnimationEvent>  EventList;
    typedef Vector<AnimationCurve*> CurveList;
    EventList m_events;
};

CUTE_NS_END
