#pragma once
#include "BaseMgr.h"

class Player
{
public:
	Player();
	~Player();

	void onTick();
	void onSecond();
	void onHour();

	void onCreate();
	void onLogin();
	void onLogout();
	void onNewDay();

public:


private:
	template<class T>
	T* getMgr(int mgrID)
	{
		return (T*)m_managers[mgrID];
	}

private:
	BaseMgr* m_managers[MGR_ID_MAX];
};