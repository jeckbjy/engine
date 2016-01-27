#pragma once
#include "List.h"
#include "Bits.h"
#include "Object.h"

CU_NS_BEGIN

// �ɲ��ŵ�,��Ƶor����
class CU_API Playable : public Object, public ListNode<Playable>
{
	enum State
	{
		S_IDLE,
		S_PLAY,
		S_PAUSE,
		S_STOP,
	};
	// λ��ʶ
	enum
	{
		F_SCHEDULED = 0x01,	// �Ƿ��ڵ�����
		F_RUNNING = 0x02,	// �Ƿ��Ѿ���ʼ����
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
	Mask8	m_flags;	// ���λ
	uint8	m_state;	// ��ǰ״̬
	uint8	m_counts;	// ��ǰ���Ŵ���
	uint8	m_repeat;	// ��Ҫ���Ŵ���,0��ʾ����
	uint32	m_time;		// ��ǰʱ��
	uint32	m_duration;	// ��ʱ��
	uint32	m_speed;	// �����ٶ�
};

CU_NS_END
