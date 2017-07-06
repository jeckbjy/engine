#pragma once
#include "Cute/Foundation.h"
#include "Cute/Timespan.h"

CUTE_NS_BEGIN

/// The PurgeStrategy is used by FileChannel
/// to purge archived log files.
class CUTE_CORE_API PurgeStrategy
{
protected:
	typedef std::vector<String> FileList;

public:
	PurgeStrategy();
	virtual ~PurgeStrategy();

	/// Purges archived log files. The path to the
	/// current "hot" log file is given.
	/// To find archived log files, look for files
	/// with a name consisting of the given path 
	/// plus any suffix (e.g., .1, .20050929081500, .1.gz).
	/// A list of archived files can be obtained by calling
	/// the list() method.
	virtual void purge(const std::string& path) = 0;

protected:
	/// Fills the given vector with a list of archived log
	/// files. The path of the current "hot" log file is
	/// given in path.
	///
	/// All files with the same name as the one given in path,
	/// plus some suffix (e.g., .1, .20050929081500, .1.gz) are
	/// considered archived files.
	void list(const std::string& path, FileList& files);

private:
	PurgeStrategy(const PurgeStrategy&);
	PurgeStrategy& operator = (const PurgeStrategy&);
};

/// This purge strategy purges all files that have
/// exceeded a given age (given in seconds).
class CUTE_CORE_API PurgeByAgeStrategy : public PurgeStrategy
{
public:
	PurgeByAgeStrategy(const Timespan& age);
	~PurgeByAgeStrategy();

	void purge(const std::string& path);

private:
	Timespan m_age;
};

/// This purge strategy ensures that a maximum number
/// of archived files is not exceeded. Files are deleted
/// based on their age, with oldest files deleted first.
class CUTE_CORE_API PurgeByCountStrategy : public PurgeStrategy
{
public:
	PurgeByCountStrategy(int count);
	~PurgeByCountStrategy();

	void purge(const std::string& path);

private:
	int m_count;
};

CUTE_NS_END
