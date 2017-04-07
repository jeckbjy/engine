#pragma once
#include "Cute/Object.h"
using namespace Cute;

enum MGR_ID
{
	MGR_ID_TEST,
	MGR_ID_MAX,
};

class Player;
class BaseMgr : public Object
{
public:
	BaseMgr();
	virtual ~BaseMgr();

protected:
	Player* m_player;
};