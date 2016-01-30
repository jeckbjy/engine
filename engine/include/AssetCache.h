#pragma once
#include "Asset.h"
#include "Archive.h"
#include "Delegate.h"
#include "Singleton.h"
#include "Mutex.h"
#include "SyncEvent.h"
#include "Thread.h"

CU_NS_BEGIN

// 管理器
class CU_API ImporterMgr : public Singleton<ImporterMgr>
{
public:
	ImporterMgr(){}
	~ImporterMgr();

	Importer* find(const String& ext) const;
	void add(Importer* importer);
	void remove(Importer* importer);

private:
	typedef std::vector<Importer*>		ImporterVec;
	typedef std::map<String, Importer*>	ImporterMap;
	ImporterVec m_importers;	// 含有所有
	ImporterMap m_mapping;
};

typedef Delegate<void(Asset*, void*)>	LoadCallback;
typedef Delegate<void(void*)>			MultiLoadCallback;
/*
支持同步加载,异步加载
异步加载时应注意:
1:返回结果可能是无序的
2:回调时会在主线程中调用
3:MultiLoad时,底层应该支持统计信息(文件数量,总文件大小)，方便主线程回调显示进度条
*/
class CU_API AssetCache : public Object, public Singleton<AssetCache>
{
	DECLARE_RTTI(AssetCache, Object, "ATCH");
public:
	template<typename T>
	static T* load(const String& path)
	{
		return (T*)AssetCache::Instance().load(path);
	}

public:
	AssetCache();
	~AssetCache();

	Asset* load(const String& path);
	void load(const String& path, LoadCallback fun, void* udata);
	void load(const StringList& files, MultiLoadCallback fun, void* data);

	void add(Archive* archive, bool front = false);
	void onUpdate(UpdateEvent* ev);

private:
	void loop();
	//static void WorkThread(void* param);	// 使用WorkQueue如何？
	typedef std::vector<Archive*>		ArchiveVec;
	typedef std::map<String, Asset*>	AssetMap;
	typedef std::pair<LoadCallback, void*> LoadData;
	typedef std::map<String, LoadData>	LoadMap;

	AssetMap	m_caches;
	ArchiveVec	m_archives;
	// 多线程
	Mutex		m_mutex;
	LoadMap		m_loading;	// 正在加载中的
	LoadMap		m_loaded;	// 已经加载完的，需要在主线程中回调
};

CU_NS_END
