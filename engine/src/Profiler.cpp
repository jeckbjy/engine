#include "Profiler.h"
#include "Util.h"
#include "Thread.h"

CU_NS_BEGIN

ProfilerBlock::ProfilerBlock(ProfilerBlock* parent, const char* name)
: m_parent(parent), m_name(name), m_timestamp(0), m_time(0), m_count(0)
, m_totalTime(0), m_totalCount(0)
{

}

ProfilerBlock::~ProfilerBlock()
{
	m_children.free();
}

void ProfilerBlock::begin()
{
	m_timestamp = Util::time();
	++m_count;
}

void ProfilerBlock::end()
{
	uint64 now = Util::time();
	m_time += (now - m_timestamp);
}

void ProfilerBlock::endFrame()
{
	m_totalTime += m_time;
	m_totalCount += m_count;

	m_time = 0;
	m_count = 0;

	for (ChildList::iterator itor = m_children.begin(); itor != m_children.end(); ++itor)
	{
		itor->endFrame();
	}
}

ProfilerBlock* ProfilerBlock::getChild(const char* name)
{
	for (ChildList::iterator itor = m_children.begin(); itor != m_children.end(); ++itor)
	{
		if (strcmp(itor->m_name.c_str(), name) == 0)
			return *itor;
	}

	ProfilerBlock* block = new ProfilerBlock(this, name);
	m_children.push(block);
	return block;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Profiler::Profiler()
{
	m_root = new ProfilerBlock(0, "Root");
	m_curr = m_root;
}

Profiler::~Profiler()
{
	delete m_root;
}

void Profiler::beginBlock(const char* name)
{
	if (!Thread::isMainThread())
		return;
	m_curr = m_curr->getChild(name);
	m_curr->begin();
}

void Profiler::endBlock()
{
	if (!Thread::isMainThread())
		return;
	if (m_curr != m_root)
	{
		m_curr->end();
		m_curr = m_curr->m_parent;
	}
}

void Profiler::beginFrame()
{
	endFrame();
	beginBlock("runFrame");
}

void Profiler::endFrame()
{
	if (m_curr == m_root)
		return;
	endBlock();
	m_root->endFrame();
	m_curr = m_root;
}

String Profiler::getInfo(bool unused /* = false */, bool total /* = false */, size_t maxDepth /* = UINT64_MAX */)
{
	String output;
	if (!total)
	{
		output += "Block                            Cnt     Avg      Max     Frame     Total\n\n";
	}
	else
	{
		output += "Block                                       Last frame                       Whole execution time\n\n";
		output += "                                 Cnt     Avg      Max      Total      Cnt      Avg       Max        Total\n\n";
	}
	if (maxDepth == 0)
		maxDepth = 1;

	char line[256];
	typedef std::stack<ProfilerBlock*> BlockStack;
	BlockStack blocks;
	blocks.push(m_root);
	while (!blocks.empty())
	{
		ProfilerBlock* block = blocks.top();
		blocks.pop();
		ProfilerBlock::ChildList& children = block->m_children;
		for (ProfilerBlock::ChildList::iterator itor = children.begin(); itor != children.end(); ++itor)
		{
			blocks.push(*itor);
		}
		if (block == m_root)
			continue;
		const char* name = block->m_name.c_str();
		if (!total)
		{
			uint  count = 0;
			float avg = 0;
			float max = 0;
			float frame = 0;
			float all = 0;
			sprintf(line, "%s %5u %8.3f %8.3f %8.3f %9.3f\n", 
				name, count, avg, max, frame, all);
		}
		else
		{
			uint count = 0;
			float avg = 0;
			float max = 0;
			float all = 0;

			uint totalCount = 0;
			float totalAvg = 0;
			float totalMax = 0;
			float totalAll = 0;
			sprintf(line, "%s %5u %8.3f %8.3f %9.3f  %7u %9.3f %9.3f %11.3f\n",
				name, count, avg, max, all, totalCount, totalAvg, totalMax, totalAll);
		}
		output += line;
	}
	return output;
}

CU_NS_END
