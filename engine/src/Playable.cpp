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
	// ��������
	m_counts = 0;
	m_time = 0;
	m_flags.reset(F_RUNNING);
	// ����
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
	// ����ʱ��
	if (!m_flags[F_RUNNING])
	{
		m_time = m_speed > 0 ? 0 : m_duration;
		m_flags.set(F_RUNNING);
	}
	else
	{// �ۻ�ֵ
		m_time += elapsed * m_speed;
		MATH_CLAMP(m_time, 0, m_duration);
		// �ж��Ƿ����
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

	// �����ֵ
	process(elapsed);

	// �ж��Ƿ����
	if (finished)
	{
		// ���²���
		m_flags.reset(F_RUNNING);
		if (m_repeat != REPEAT_INDEFINITE)
		{
			++m_counts;
			if (m_counts >= m_repeat)
			{// ����
				return true;
			}
		}
	}
	return false;
}

CU_NS_END
