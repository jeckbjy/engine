// module DateTime
#include "Cute/Timespan.h"

CUTE_NS_BEGIN

const int64_t Timespan::MILLISECONDS = 1000;
const int64_t Timespan::SECONDS = 1000 * Timespan::MILLISECONDS;
const int64_t Timespan::MINUTES = 60 * Timespan::SECONDS;
const int64_t Timespan::HOURS = 60 * Timespan::MINUTES;
const int64_t Timespan::DAYS = 24 * Timespan::HOURS;

Timespan::Timespan() 
	: m_span(0)
{
}

Timespan::Timespan(int64_t microSeconds) 
	: m_span(microSeconds)
{
}

Timespan::Timespan(long otherSeconds, long otherMicroSeconds) 
	: m_span(int64_t(otherSeconds)*SECONDS + otherMicroSeconds)
{
}

Timespan::Timespan(int otherDays, int otherHours, int otherMinutes, int otherSeconds, int otherMicroSeconds)
	: m_span(int64_t(otherMicroSeconds) + int64_t(otherSeconds)*SECONDS + int64_t(otherMinutes)*MINUTES + int64_t(otherHours)*HOURS + int64_t(otherDays)*DAYS)
{
}

Timespan::Timespan(const Timespan& timespan)
	: m_span(timespan.m_span)
{
}

Timespan::~Timespan()
{
}

Timespan& Timespan::operator = (const Timespan& timespan)
{
	m_span = timespan.m_span;
	return *this;
}

Timespan& Timespan::operator = (int64_t microSeconds)
{
	m_span = microSeconds;
	return *this;
}

Timespan& Timespan::assign(int otherDays, int otherHours, int otherMinutes, int otherSeconds, int otherMicroSeconds)
{
	m_span = int64_t(otherMicroSeconds) + int64_t(otherSeconds)*SECONDS + int64_t(otherMinutes)*MINUTES + int64_t(otherHours)*HOURS + int64_t(otherDays)*DAYS;
	return *this;
}

Timespan& Timespan::assign(long otherSeconds, long otherMicroSeconds)
{
	m_span = int64_t(otherSeconds)*SECONDS + int64_t(otherMicroSeconds);
	return *this;
}

void Timespan::swap(Timespan& timespan)
{
	std::swap(m_span, timespan.m_span);
}

Timespan Timespan::operator + (const Timespan& d) const
{
	return Timespan(m_span + d.m_span);
}

Timespan Timespan::operator - (const Timespan& d) const
{
	return Timespan(m_span - d.m_span);
}

Timespan& Timespan::operator += (const Timespan& d)
{
	m_span += d.m_span;
	return *this;
}

Timespan& Timespan::operator -= (const Timespan& d)
{
	m_span -= d.m_span;
	return *this;
}

Timespan Timespan::operator + (int64_t microSeconds) const
{
	return Timespan(m_span + microSeconds);
}

Timespan Timespan::operator - (int64_t microSeconds) const
{
	return Timespan(m_span - microSeconds);
}

Timespan& Timespan::operator += (int64_t microSeconds)
{
	m_span += microSeconds;
	return *this;
}

Timespan& Timespan::operator -= (int64_t microSeconds)
{
	m_span -= microSeconds;
	return *this;
}

CUTE_NS_END