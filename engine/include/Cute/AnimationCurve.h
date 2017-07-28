#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

template<typename T>
struct TKeyFrame
{
    T value;
    T inTangent;
    T outTangent;
    float time;
};

template<typename T>
class TAnimationCurve;

template<typename T>
struct TCurveCache
{
private:
    friend class TAnimationCurve<T>;
    mutable uint32  m_key;
    mutable float   m_start;
    mutable float   m_end;
    mutable T       m_coefficients[4];
};

template<typename T>
class CUTE_CORE_API TAnimationCurve
{
public:
    typedef TCurveCache<T>      CurveCache;
    typedef TKeyFrame<T>        KeyFrame;
    typedef Vector<KeyFrame>    KeyFrameArray;
    
    TAnimationCurve();
    TAnimationCurve(const KeyFrameArray& keyframes);
    
    T evaluate(float time, bool loop = true) const;
    T evaluate(float time, const CurveCache& cache, bool loop = true) const;
    
    float   getLength() const { return m_end; }
    uint    getNumKeyFrames() const { return (uint)m_keyframes.size(); }
    const KeyFrame& getKeyFrame(uint index) const { return m_keyframes[index]; }
    
private:
    uint findKey(float time);
    void findKeys(float time, uint32& leftKey, uint32& rightKey) const;
    void findKeys(float time, const CurveCache& cache, uint32& leftKey, uint32& rightKey) const;
    
private:
    KeyFrameArray   m_keyframes;
    float           m_start;
    float           m_end;
    float           m_length;
};

CUTE_NS_END
