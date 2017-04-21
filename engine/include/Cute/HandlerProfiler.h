#pragma once
#include "Cute/Foundation.h"

CUTE_NS_BEGIN

// ���ܼ��
class CUTE_CORE_API HandlerProfiler
{
public:
	struct Timer
	{
		Timer() :timestamp(0){}
		int64  timestamp;
	};

	static HandlerProfiler& instance();

	HandlerProfiler();
	~HandlerProfiler();

	void start(Timer& timer);
	void hit(Timer& timer, int cmdid);

	void clear();
	void report();

	void open();
	void close();
	void setOpen(bool flag);

private:
	struct Item
	{
		int		cmdid;
		uint64 elapsedAll;	// ��ֵ
		uint64 elapsedMax;	// ���ֵ
		uint64 elapsedAvg;	// ƽ��ֵ
		uint64 hitNum;		// ����
		Item() :cmdid(0),elapsedAll(0), elapsedMax(0), hitNum(0){}
	};

	typedef std::map<int, Item> ItemMap;
	bool	m_open;
	ItemMap m_events;
};

CUTE_NS_END
