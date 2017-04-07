#pragma once
#include "Cute/Foundation.h"
#include "Cute/Timespan.h"
#include "Cute/Timestamp.h"
#include "Cute/DateTime.h"

CUTE_NS_BEGIN

class LogFile;
class CUTE_CORE_API RotateStrategy
{
public:
	RotateStrategy();
	virtual ~RotateStrategy();

	virtual bool mustRotate(LogFile* pFile) = 0;

private:
	RotateStrategy(const RotateStrategy&);
	RotateStrategy& operator = (const RotateStrategy&);
};

/// The file is rotated at specified [day,][hour]:minute
class CUTE_CORE_API RotateAtTimeStrategy : public RotateStrategy
{
public:
	RotateAtTimeStrategy(const String& time, bool isLocal);
	~RotateAtTimeStrategy();

	bool mustRotate(LogFile* pFile);

private:
	void getNextRollover();

private:
	DateTime m_threshold;
	int		 m_day;
	int		 m_hour;
	int		 m_minute;
};

/// The file is rotated when the log file 
/// exceeds a given age.
///
/// For this to work reliably across all platforms and file systems
/// (there are severe issues on most platforms finding out the real
/// creation date of a file), the creation date of the file is
/// written into the log file as the first entry.
class CUTE_CORE_API RotateByIntervalStrategy : public RotateStrategy
{
public:
	RotateByIntervalStrategy(const Timespan& span);
	~RotateByIntervalStrategy();
	bool mustRotate(LogFile* pFile);

private:
	Timespan  m_span;
	Timestamp m_lastRotate;
	static const String ROTATE_TEXT;
};

/// The file is rotated when the log file
/// exceeds a given size.
class CUTE_CORE_API RotateBySizeStrategy : public RotateStrategy
{
public:
	RotateBySizeStrategy(uint64_t size);
	~RotateBySizeStrategy();
	bool mustRotate(LogFile* pFile);

private:
	uint64_t m_size;
};

CUTE_NS_END