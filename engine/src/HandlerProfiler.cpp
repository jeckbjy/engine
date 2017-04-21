//! Server
#include "Cute/HandlerProfiler.h"
#include "Cute/Timestamp.h"
#include <fstream>

CUTE_NS_BEGIN

HandlerProfiler& HandlerProfiler::instance()
{
	static HandlerProfiler ins;
	return ins;
}

HandlerProfiler::HandlerProfiler()
	: m_open(false)
{
}

HandlerProfiler::~HandlerProfiler()
{
}

void HandlerProfiler::start(Timer& timer)
{
	if (!m_open)
		return;
	timer.timestamp = Timestamp::now();
}

void HandlerProfiler::hit(Timer& timer, int cmdid)
{
	if (!m_open || timer.timestamp == 0)
		return;
	
	int64 now = Timestamp::now();
	int64 elapsed = now - timer.timestamp;

	Item& item = m_events[cmdid];
	item.cmdid = cmdid;

	if (elapsed > item.elapsedMax)
		item.elapsedMax = elapsed;

	item.elapsedAll += elapsed;
	item.hitNum += 1;
}

void HandlerProfiler::clear()
{
	m_events.clear();
}

void HandlerProfiler::report()
{
	// 格式化输出到文件
	std::ofstream outfile("profile.log", std::ofstream::binary);
	int64 totalTime = 0;	// 总时间
	int64 totalHit = 0;		// 总事件次数
	// 统计最大值
	Item* maxAll = 0;
	Item* maxAvg = 0;

	for (ItemMap::iterator itor = m_events.begin(); itor != m_events.end(); ++itor)
	{
		Item& item = itor->second;
		item.elapsedAvg = (int64)(item.elapsedAll / item.hitNum);
		totalTime += item.elapsedAll;
		totalHit += item.hitNum;
		//
		if (maxAll == 0 || maxAll->elapsedAll < item.elapsedAll)
			maxAll = &item;

		if (maxAvg == 0 || maxAvg->elapsedAvg < item.elapsedAvg)
			maxAvg = &item;
	}

	// 输出
	outfile << "Event\t\tHit\t\tTime\t\tMax\t\tAvg\t\tTime%" << std::endl;
	for (ItemMap::iterator itor = m_events.begin(); itor != m_events.end(); ++itor)
	{
		int cmdid = itor->first;
		Item& item = itor->second;
		outfile << cmdid << "\t\t";
		outfile << item.hitNum << "\t\t";
		outfile << item.elapsedAll << "\t\t";
		outfile << item.elapsedMax << "\t\t";
		outfile << item.elapsedAvg << "\t\t";
		outfile << (100.0 * item.elapsedAll / totalTime) << "%\t\t";
		outfile << std::endl;
	}

	// max
	if (maxAvg != NULL)
		outfile << "MAX_AVG:" << maxAll->cmdid << "," << (maxAvg->elapsedAvg) << std::endl;

	if (maxAll != NULL)
		outfile << "MAX_ALL:" << maxAll->cmdid << "," << maxAll->elapsedAll << std::endl;

	// sum
	outfile << "SUM:" << totalHit << "," << totalTime << "\t\tAvg:" << (100.0*totalTime / totalHit) << std::endl;

	// 平均值
	outfile.close();
}

void HandlerProfiler::open()
{
	setOpen(true);
}

void HandlerProfiler::close()
{
	setOpen(false);
}

void HandlerProfiler::setOpen(bool flag)
{
	if (flag == m_open)
		return;
	m_open = flag;
	if (m_open)
		clear();
}

CUTE_NS_END
