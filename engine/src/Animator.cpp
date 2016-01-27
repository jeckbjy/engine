#include "Animator.h"

CU_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
AnimState::AnimState(Animator* owner, AnimClip* clip)
: m_owner(owner)
, m_clip(clip)
, m_weight(0)
, m_layer(0)
, m_enable(true)
{

}

AnimState::~AnimState()
{

}

void AnimState::process(uint64 elapsed)
{
	// 查找目标

	// 插值处理数据
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Animator::Animator()
{

}

Animator::~Animator()
{

}

void Animator::play(const String& name)
{

}

void Animator::stop(const String& name)
{

}

AnimState* Animator::find(const String& name)
{
	return NULL;
}

CU_NS_END
