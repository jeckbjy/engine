#pragma once
#include "List.h"
#include "Singleton.h"

CU_NS_BEGIN

// ¿ØÖÆÆ÷
class Playable;
class CU_API PlayController : public Singleton<PlayController>
{
public:
	PlayController();
	~PlayController();

	void schedule(Playable* playable);
	void unschedule(Playable* playable);

	void update(uint32 elapsed);

private:
	typedef List<Playable> AnimateList;
	AnimateList m_animates;
};

CU_NS_END