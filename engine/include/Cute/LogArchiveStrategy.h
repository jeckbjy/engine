#pragma once
#include "Cute/Foundation.h"
#include "Cute/LogFile.h"

CUTE_NS_BEGIN

/// The ArchiveStrategy is used by FileChannel 
/// to rename a rotated log file for archiving.
///
/// Archived files can be automatically compressed,
/// using the gzip file format.
class CUTE_CORE_API ArchiveStrategy
{
public:
	ArchiveStrategy();
	virtual ~ArchiveStrategy();

	virtual LogFile* archive(LogFile* pFile) = 0;
	/// Renames the given log file for archiving
	/// and creates and returns a new log file.
	/// The given LogFile object is deleted.

	void compress(bool flag = true);
	/// Enables or disables compression of archived files.	
protected:
	void moveFile(const std::string& oldName, const std::string& newName);
	bool exists(const std::string& name);

private:
	ArchiveStrategy(const ArchiveStrategy&);
	ArchiveStrategy& operator = (const ArchiveStrategy&);

	bool _compress;
	//ArchiveCompressor* _pCompressor;
};

/// A monotonic increasing number is appended to the
/// log file name. The most recent archived file
/// always has the number zero.
class CUTE_CORE_API ArchiveByNumberStrategy : public ArchiveStrategy
{
public:
	ArchiveByNumberStrategy();
	~ArchiveByNumberStrategy();
	LogFile* archive(LogFile* pFile);
};

/// A timestamp (YYYYMMDDhhmmssiii) is appended to archived
/// log files.
class CUTE_CORE_API ArchiveByTimestampStrategy : public ArchiveStrategy
{
public:
	ArchiveByTimestampStrategy();
	~ArchiveByTimestampStrategy();

	LogFile* archive(LogFile* pFile);

private:
	void archiveByNumber(const std::string& basePath);
};

CUTE_NS_END