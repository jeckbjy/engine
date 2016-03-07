#pragma once
#include "API.h"
#include "List.h"
#include "Singleton.h"

CU_NS_BEGIN

class CU_API ProfilerBlock : public ListNode<ProfilerBlock>
{
public:
	typedef List<ProfilerBlock> ChildList;
	ProfilerBlock(ProfilerBlock* parent, const char* name);
	~ProfilerBlock();

	void begin();
	void end();

	// һ֡����
	void endFrame();
	
	ProfilerBlock* getChild(const char* name);

private:
	friend class Profiler;
	ProfilerBlock*	m_parent;
	ChildList		m_children;
	String	m_name;
	// ��ǰ֡��Ϣ
	uint64	m_timestamp;
	uint64	m_time;		// һ֡��ʱ��
	size_t	m_count;	// ���ô���
	// �ۼ���Ϣ
	size_t	m_totalTime;
	size_t	m_totalCount;
};

class CU_API Profiler : public Singleton<Profiler>
{
public:
	Profiler();
	virtual ~Profiler();

	void beginBlock(const char* name);
	void endBlock();

	void beginFrame();
	void endFrame();

	String getInfo(bool unused = false, bool total = false, size_t maxDepth = UINT32_MAX);

private:
	ProfilerBlock*	m_root;
	ProfilerBlock*	m_curr;
	size_t			m_frames;	// ��֡��
};

class CU_API AutoProfileBlock
{
public:
	AutoProfileBlock(Profiler& profiler, const char* name)
		:m_profiler(&profiler)
	{
		m_profiler->beginBlock(name);
	}
	~AutoProfileBlock()
	{
		m_profiler->endBlock();
	}

private:
	Profiler* m_profiler;
};

CU_API Profiler& gProfiler() { return Profiler::Instance(); }

#ifdef CU_PROFILING
#define PROFILE(name) profile_ ## name (gProfiler(), #name)
#else
#define PROFILE(name)
#endif

CU_NS_END
