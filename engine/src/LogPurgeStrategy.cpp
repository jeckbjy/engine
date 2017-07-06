//! Logging
#include "Cute/LogPurgeStrategy.h"
#include "Cute/DirectoryIterator.h"
#include "Cute/Paths.h"
#include "Cute/Files.h"
#include "Cute/Timestamp.h"

CUTE_NS_BEGIN

//
// PurgeStrategy
//
PurgeStrategy::PurgeStrategy()
{
}

PurgeStrategy::~PurgeStrategy()
{
}

void PurgeStrategy::list(const std::string& path, std::vector<String>& files)
{
//	Path p(path);
//	p.makeAbsolute();
//	Path parent = p.parent();
//	std::string baseName = p.getFileName();
//	baseName.append(".");
    
    String parent = Paths::getDirectory(path);
    String baseName = Paths::getBaseFileName(path);
    baseName.append(".");

    DirectoryIterator it(parent, false);
    while(it.next())
    {
        if (it.toString().compare(0, baseName.size(), baseName) == 0)
        {
        	files.push_back(*it);
        }
    }
}

//
// PurgeByAgeStrategy
//
PurgeByAgeStrategy::PurgeByAgeStrategy(const Timespan& age) : m_age(age)
{
}

PurgeByAgeStrategy::~PurgeByAgeStrategy()
{
}

void PurgeByAgeStrategy::purge(const std::string& path)
{
	FileList files;
	list(path, files);
	for (FileList::iterator it = files.begin(); it != files.end(); ++it)
	{
		Timestamp ts = Files::getLastModificationTime(*it);
		if (ts.isElapsed(m_age.totalMicroseconds()))
		{
			Files::remove(*it);
		}
	}
}

//
// PurgeByCountStrategy
//
PurgeByCountStrategy::PurgeByCountStrategy(int count) : m_count(count)
{
	cute_assert(count > 0);
}

PurgeByCountStrategy::~PurgeByCountStrategy()
{
}

void PurgeByCountStrategy::purge(const std::string& path)
{
	FileList files;
	list(path, files);
	while (files.size() > m_count)
	{
		FileList::iterator it = files.begin();
		FileList::iterator purgeIt = it;
		Timestamp purgeTS = Files::getLastModificationTime(*purgeIt);
		++it;
		while (it != files.end())
		{
			Timestamp md(Files::getLastModificationTime(*it));
			if (md <= purgeTS)
			{
				purgeTS = md;
				purgeIt = it;
			}
			++it;
		}
		Files::remove(*purgeIt);
		files.erase(purgeIt);
	}
}

CUTE_NS_END
