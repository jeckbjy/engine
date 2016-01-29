#pragma once
#include "Object.h"
#include "Asset.h"
#include "Singleton.h"

CU_NS_BEGIN

// 单独线程加载资源,Importer?Archive?
class CU_API AssetCache : public Singleton<AssetCache>
{
public:
	template<typename T>
	static T* load(const String& name)
	{
		return (T*)AssetCache::Instance().load(name);
	}

public:
	AssetCache();
	~AssetCache();

	Asset* load(const String& name);
private:
	static void WorkThread(void* param);
	typedef std::map<String, Asset*> AssetMap;
	AssetMap m_assets;
};

CU_NS_END
