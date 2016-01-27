#pragma once
#include "List.h"
#include "Bits.h"
#include "Object.h"

CU_NS_BEGIN

// 可播放的,音频or动画
class CU_API Playable : public Object, public ListNode<Playable>
{
	enum State
	{
		S_IDLE,
		S_PLAY,
		S_PAUSE,
		S_STOP,
	};
	// 位标识
	enum
	{
		F_SCHEDULED = 0x01,	// 是否在调度中
		F_RUNNING = 0x02,	// 是否已经开始播放
	};
public:
	static const uint REPEAT_INDEFINITE = 0;

public:
	Playable();
	virtual ~Playable(){}
	virtual void play();
	virtual void stop();
	virtual void pause();
	virtual void resume();
	virtual bool update(uint64 elapsed);

	inline bool isPlaying() const { return m_state == S_PLAY; }
	inline bool isPause() const { return m_state == S_PAUSE; }
	inline bool isScheduled() const { return m_flags[F_SCHEDULED]; }

protected:
	virtual void process(uint64 elapsed) = 0;

protected:
	Mask8	m_flags;	// 标记位
	uint8	m_state;	// 当前状态
	uint8	m_counts;	// 当前播放次数
	uint8	m_repeat;	// 需要播放次数,0表示无限
	uint32	m_time;		// 当前时间
	uint32	m_duration;	// 总时间
	uint32	m_speed;	// 播放速度
};

CU_NS_END
