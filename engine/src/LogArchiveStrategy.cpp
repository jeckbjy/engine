//! Logging
#include "Cute/LogArchiveStrategy.h"
#include "Cute/File.h"
#include "Cute/Path.h"

CUTE_NS_BEGIN

//
// ArchiveStrategy
//
ArchiveStrategy::ArchiveStrategy()
	: _compress(false)
	//, _pCompressor(0)
{
}


ArchiveStrategy::~ArchiveStrategy()
{
	//delete _pCompressor;
}


void ArchiveStrategy::compress(bool flag)
{
	_compress = flag;
}

void ArchiveStrategy::moveFile(const std::string& oldPath, const std::string& newPath)
{
	bool compressed = false;
	Path p(oldPath);
	String f(oldPath);
	if (!File::exists(oldPath))
	{
		f = oldPath + ".gz";
		compressed = true;
	}
	std::string mvPath(newPath);
	if (_compress || compressed)
		mvPath.append(".gz");
	if (!_compress || compressed)
	{
		File::rename(f, mvPath);
	}
	else
	{
		File::rename(f, newPath);
		//if (!_pCompressor) _pCompressor = new ArchiveCompressor;
		//_pCompressor->compress(newPath);
	}
}

bool ArchiveStrategy::exists(const std::string& name)
{
	if (File::exists(name))
	{
		return true;
	}
	else if (_compress)
	{
		std::string gzName(name);
		gzName.append(".gz");
		return File::exists(gzName);
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
ArchiveByNumberStrategy::ArchiveByNumberStrategy()
{

}

ArchiveByNumberStrategy::~ArchiveByNumberStrategy()
{

}

LogFile* ArchiveByNumberStrategy::archive(LogFile* pFile)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
ArchiveByTimestampStrategy::ArchiveByTimestampStrategy()
{

}

ArchiveByTimestampStrategy::~ArchiveByTimestampStrategy()
{

}

LogFile* ArchiveByTimestampStrategy::archive(LogFile* pFile)
{
	return NULL;
}

void ArchiveByTimestampStrategy::archiveByNumber(const std::string& basePath)
{

}

CUTE_NS_END