//! Animation
#include "Cute/AnimationCurve.h"
#include "Cute/Vector3.h"
#include "Cute/Quaternion.h"

CUTE_NS_BEGIN

//
//
//
template<typename T>
TAnimationCurve<T>::TAnimationCurve()
    : m_start(0.0f), m_end(0.0f), m_length(0.0f)
{
    
}

template<typename T>
TAnimationCurve<T>::TAnimationCurve(const KeyFrameArray& keyframes)
    : m_keyframes(keyframes)
{
    m_end = keyframes.empty() ? 0.0f : keyframes.back().time;
    m_start = 0.0f;
    m_length = m_end;
}

template<typename T>
T TAnimationCurve<T>::evaluate(float time, bool loop) const
{
    if(m_keyframes.empty())
        return T();
    
    return T();
}

template<typename T>
T TAnimationCurve<T>::evaluate(float time, const CurveCache &cache, bool loop) const
{
    return T();
}

//
// instance
//
template class TAnimationCurve<Vector3>;
template class TAnimationCurve<Quaternion>;
template class TAnimationCurve<float>;

CUTE_NS_END
