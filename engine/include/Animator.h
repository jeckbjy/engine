#pragma once
#include "Component.h"
#include "Playable.h"
#include "Animatable.h"
#include "Animation.h"

CU_NS_BEGIN

// ��ǰ�������ſ���
class Animator;
class CU_API AnimState : public Playable
{
public:
	AnimState(Animator* owner, AnimClip* clip);
	~AnimState();

	void process(uint64 elapsed);
private:
	typedef Vector<Animatable*> TargetVec;
	TargetVec m_targets;	// ��̬cache??
	Animator* m_owner;
	AnimClip* m_clip;
	float	  m_weight;
	uint	  m_layer;		// ��ϲ㼶
	bool	  m_enable;
};

class Animation;
// ����������
class CU_API Animator : public Component
{
	DECLARE_RTTI(Animator, Component, TYPE_ANIMATOR);
public:
	Animator();
	~Animator();

	void play(const String& name);
	void stop(const String& name);

	AnimState* find(const String& name);

private:
	typedef Vector<AnimState*> StateVec;
	Animation*	m_data;
	StateVec	m_states;
};

CU_NS_END