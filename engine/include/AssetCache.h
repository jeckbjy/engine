#pragma once
#include "Asset.h"
#include "Archive.h"
#include "Delegate.h"
#include "Singleton.h"
#include "Mutex.h"
#include "SyncEvent.h"
#include "Thread.h"

CU_NS_BEGIN

// ������
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
	ImporterVec m_importers;	// ��������
	ImporterMap m_mapping;
};

typedef Delegate<void(Asset*, void*)>	LoadCallback;
typedef Delegate<void(void*)>			MultiLoadCallback;
/*
֧��ͬ������,�첽����
�첽����ʱӦע��:
1:���ؽ�������������
2:�ص�ʱ�������߳��е���
3:MultiLoadʱ,�ײ�Ӧ��֧��ͳ����Ϣ(�ļ�����,���ļ���С)���������̻߳ص���ʾ������
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
	//static void WorkThread(void* param);	// ʹ��WorkQueue��Σ�
	typedef std::vector<Archive*>		ArchiveVec;
	typedef std::map<String, Asset*>	AssetMap;
	typedef std::pair<LoadCallback, void*> LoadData;
	typedef std::map<String, LoadData>	LoadMap;

	AssetMap	m_caches;
	ArchiveVec	m_archives;
	// ���߳�
	Mutex		m_mutex;
	LoadMap		m_loading;	// ���ڼ����е�
	LoadMap		m_loaded;	// �Ѿ�������ģ���Ҫ�����߳��лص�
};

CU_NS_END
