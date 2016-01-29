#include "AssetCache.h"

CU_NS_BEGIN

AssetCache::AssetCache()
{

}

AssetCache::~AssetCache()
{

}

Asset* AssetCache::load(const String& name)
{
	AssetMap::iterator itor = m_assets.find(name);
	if (itor != m_assets.end())
		return itor->second;
	// ����������
	return NULL;
}

CU_NS_END