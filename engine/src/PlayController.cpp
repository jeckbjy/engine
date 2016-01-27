#include "PlayController.h"
#include "Playable.h"

CU_NS_BEGIN

PlayController::PlayController()
{
}

PlayController::~PlayController()
{
}

void PlayController::schedule(Playable* playable)
{
	m_animates.append(playable);
}

void PlayController::unschedule(Playable* playable)
{
	m_animates.erase(playable);
}

void PlayController::update(uint32 elapsed)
{
	if (m_animates.empty())
		return;
	AnimateList::iterator itor = m_animates.begin();
	while (itor)
	{
		if (itor->update(elapsed))
			m_animates.erase(itor++);
		else
			++itor;
	}
}

CU_NS_END
