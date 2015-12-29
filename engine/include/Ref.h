#pragma once
#include "Types.h"

CU_NS_BEGIN

class RefCounted;
// �����ü�����,����ʶ��ָ���Ƿ���Ч,ֻ����Ref����
struct RefCount
{
	int  refs_count;
	int  weak_count;
	RefCounted* ptr;
	void retain()  { ++weak_count; }
	void release() { if (--weak_count < 0) delete this; }
private:
	friend class RefCounted;
	RefCount(RefCounted* ptr):refs_count(0),weak_count(1), ptr(ptr){}
};

// ����ʽ���ü���
class CU_API RefCounted
{
public:
	RefCounted();
	virtual ~RefCounted();

	void retain();
	void release();
	void unlink() { --m_ref->refs_count; }
	int refs() const { return m_ref->refs_count; }

	RefCount* getRefCount() { return m_ref; }

private:
	RefCounted(const RefCounted&);
	RefCounted& operator=(const RefCounted&);

	RefCount* m_ref;
};

// �򵥵ļ���������ʹ�������ã����ǽ�ʡ�ڴ�
class CU_API Ref
{
public:
	Ref() :m_refs(0){}
	virtual ~Ref(){}
	void retain() 
	{
		++m_refs; 
	}
	void release()
	{
		if (--m_refs <= 0) 
			delete this;
	}
	void unlink() 
	{ 
		--m_refs; 
	}
	int  refs() const { return m_refs; }
private:
	int m_refs;
};

CU_NS_END
