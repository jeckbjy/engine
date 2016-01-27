#include "Playable.h"
#include "PlayController.h"

CU_NS_BEGIN

Playable::Playable()
: m_state(S_IDLE)
, m_flags(0)
, m_counts(0)
, m_repeat(1)
, m_time(0)
, m_duration(1)
, m_speed(1.0)
{

}

void Playable::play()
{
	m_state = S_PLAY;
	// 重置数据
	m_counts = 0;
	m_time = 0;
	m_flags.reset(F_RUNNING);
	// 调度
	if (!m_flags[F_SCHEDULED])
	{
		m_flags.set(F_SCHEDULED);
		PlayController::Instance().schedule(this);
	}
}

void Playable::stop()
{
	if (m_flags[F_SCHEDULED])
		m_state = S_STOP;
}

void Playable::pause()
{
	if (m_state == S_PLAY)
		m_state = S_STOP;
}

void Playable::resume()
{
	if (m_state == S_PAUSE)
		m_state = S_PLAY;
}

bool Playable::update(uint64 elapsed)
{
	if (m_state == S_STOP)
		return true;
	if (m_state == S_PAUSE)
		return false;

	bool finished = false;
	// 计算时间
	if (!m_flags[F_RUNNING])
	{
		m_time = m_speed > 0 ? 0 : m_duration;
		m_flags.set(F_RUNNING);
	}
	else
	{// 累积值
		m_time += elapsed * m_speed;
		MATH_CLAMP(m_time, 0, m_duration);
		// 判断是否结束
		if (m_time <= 0)
		{
			m_time = 0;
			finished = true;
		}
		else if (m_time >= m_duration)
		{
			m_time = m_duration;
			finished = true;
		}
	}

	// 处理插值
	process(elapsed);

	// 判断是否结束
	if (finished)
	{
		// 重新播放
		m_flags.reset(F_RUNNING);
		if (m_repeat != REPEAT_INDEFINITE)
		{
			++m_counts;
			if (m_counts >= m_repeat)
			{// 结束
				return true;
			}
		}
	}
	return false;
}

CU_NS_END
