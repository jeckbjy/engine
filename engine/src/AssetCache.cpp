#include "AssetCache.h"
#include "Util.h"

CU_NS_BEGIN

ImporterMgr::~ImporterMgr()
{
	for (ImporterVec::iterator itor = m_importers.begin(); itor != m_importers.end(); ++itor)
	{
		(*itor)->release();
	}
	m_importers.clear();
	m_mapping.clear();
}

Importer* ImporterMgr::find(const String& ext) const
{
	ImporterMap::const_iterator itor = m_mapping.find(ext);
	if (itor != m_mapping.end())
		return itor->second;
	for (ImporterVec::const_iterator itor = m_importers.begin(); itor != m_importers.end(); ++itor)
	{
		Importer* importer = *itor;
		if (importer->check(ext))
			return importer;
	}

	return NULL;
}

void ImporterMgr::add(Importer* importer)
{
	importer->retain();
	m_importers.push_back(importer);
	if (importer->extension() != 0)
		m_mapping[importer->extension()] = importer;
}

void ImporterMgr::remove(Importer* importer)
{
	ImporterVec::iterator itor = std::find(m_importers.begin(), m_importers.end(), importer);
	if (itor == m_importers.end())
		return;
	m_importers.erase(itor);
	if (importer->extension() != 0)
	{
		ImporterMap::iterator kv_itor = m_mapping.find(importer->extension());
		if (kv_itor != m_mapping.end() && kv_itor->second == importer)
		{
			m_mapping.erase(kv_itor);
		}
	}
	// release
	importer->release();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
AssetCache::AssetCache()
{
	addListener(&AssetCache::onUpdate);
}

AssetCache::~AssetCache()
{
	for (AssetMap::iterator itor = m_caches.begin(); itor != m_caches.end(); ++itor)
	{
		Asset* asset = itor->second;
		asset->release();
	}
	m_caches.clear();

	for (ArchiveVec::iterator itor = m_archives.begin(); itor != m_archives.end(); ++itor)
	{
		(*itor)->release();
	}

	m_archives.clear();
}

void AssetCache::add(Archive* archive, bool front /* = false */)
{
	archive->retain();
	if (front)
	{
		m_archives.insert(m_archives.begin(), archive);
	}
	else
	{
		m_archives.push_back(archive);
	}
}

Asset* AssetCache::load(const String& path)
{
	// 已经加载
	AssetMap::iterator itor = m_caches.find(path);
	if (itor != m_caches.end())
		return itor->second;

	String ext = Util::getExtension(path);
	// 创建，加载
	Importer* importer = ImporterMgr::Instance().find(ext);
	if (!importer)
		return NULL;
	Asset* result = NULL;
	// 查找并加载
	for (ArchiveVec::iterator itor = m_archives.begin(); itor != m_archives.end(); ++itor)
	{
		Archive* archive = *itor;
		Stream* stream = archive->open(path);
		if (!stream)
			continue;
		result = importer->load(stream);
		if (result)
		{
			result->retain();
			m_caches[path] = result;
		}
		break;
	}

	return result;
}

void AssetCache::load(const String& path, LoadCallback fun, void* udata)
{
	// 主线程中调用
	// 先尝试缓存中加载
	AssetMap::iterator itor = m_caches.find(path);
	if (itor != m_caches.end())
	{
		fun(itor->second, udata);
		return;
	}
	// 放入线程中加载
	//WorkQueue::Instance().execute();
}

void AssetCache::onUpdate(UpdateEvent* ev)
{
	// 调用函数
}

void AssetCache::loop()
{
	// 线程
}

CU_NS_END