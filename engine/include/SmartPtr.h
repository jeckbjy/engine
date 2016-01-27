#pragma once
#include "Ref.h"

CU_NS_BEGIN

// 侵入式智能指针
template<class T>
class SharedPtr
{
public:
	SharedPtr() :m_ptr(0){}
	SharedPtr(const SharedPtr<T>& rhs)
		:m_ptr(rhs.m_ptr)
	{ 
		retain(); 
	}
	explicit SharedPtr(T* ptr) 
		:m_ptr(ptr)
	{
		retain();
	}
	~SharedPtr()
	{
		release(); 
	}

	void reset(){ release(); }

	// Detach without destroying the object even if the refcount goes zero. To be used for scripting language interoperation.
	void detach()
	{
		if (m_ptr)
			m_ptr->unlink();
	}
	// 强制类型转换
	template<class U> SharedPtr<U> cast() { return SharedPtr<U>(static_cast<U*>(m_ptr)); }

	T* get() const { return m_ptr; }
	int refs() const { return m_ptr ? m_ptr->refs() : 0; }
	bool empty() const { return m_ptr == 0; }

	SharedPtr<T>& operator=(const SharedPtr<T>& rhs)
	{
		if (m_ptr != rhs.m_ptr)
		{
			release();
			m_ptr = rhs.m_ptr;
			retain();
		}
		return *this;
	}
	SharedPtr<T>& operator=(T* ptr)
	{
		if (m_ptr != ptr)
		{
			release();
			m_ptr = ptr;
			retain();
		}
		return *this;
	}
	
	bool operator  <(const SharedPtr<T>& rhs) const { return m_ptr < rhs->m_ptr; }
	bool operator ==(const SharedPtr<T>& rhs) const { return m_ptr == rhs->m_ptr; }
	bool operator !=(const SharedPtr<T>& rhs) const { return m_ptr != rhs->m_ptr; }

	T* operator->() const { return m_ptr; }
	T& operator *() const { return *m_ptr; }
	operator T*() const   { return m_ptr; }

private:
	// Prevent direct assignment from a shared pointer of another type.
	template<class U> SharedPtr<T>& operator=(const SharedPtr<U>&);

	inline void retain()
	{
		if (m_ptr)
			m_ptr->retain(); 
	}
	inline void release() 
	{
		if (m_ptr)
		{
			m_ptr->release();
			m_ptr = 0;
		}
	}
	T* m_ptr;
};

// 弱引用,用于防止野指针
template<class T>
class WeakPtr
{
	RefCount* m_ref;
public:
	WeakPtr() :m_ref(0){}
	WeakPtr(const WeakPtr<T>& rhs) :m_ref(0){ set(m_ref); }
	WeakPtr(const SharedPtr<T>& rhs) : m_ref(0) { set(rhs.get()); }
	explicit WeakPtr(T* ptr) :m_ref(0){ set(ptr); }
	~WeakPtr() { release(); }

	T* get() const { return expired() ? 0 : (T*)m_ref->ptr; }
	void reset() { release(); }
	bool expired() const { return !m_ref || m_ref->refs_count <= 0; }
	SharedPtr<T> lock() const { return SharedPtr<T>(get()); }

	WeakPtr<T>& operator=(const WeakPtr<T>& rhs) { set(rhs.m_ref); return *this; }
	WeakPtr<T>& operator=(const SharedPtr<T>& rhs) { set(rhs.get()); return *this; }
	WeakPtr<T>& operator=(T* ptr) { set(ptr); return *this; }

	bool operator == (const WeakPtr<T>& rhs) const { return m_ref == rhs.m_ref; }
	bool operator != (const WeakPtr<T>& rhs) const { return m_ref != rhs.m_ref; }
	bool operator  < (const WeakPtr<T>& rhs) const { return get() < rhs.get(); }

	T* operator ->() const { return get(); }
	T& operator  *() const { return *get(); }
	operator T*() const { return get(); }

private:
	inline void set(RefCount* ref)
	{
		if (ref != m_ref)
		{
			if (m_ref)
				m_ref->release();
			m_ref = ref;
			if (m_ref)
				m_ref->retain();
		}
	}

	inline void set(T* ptr)
	{
		set(ptr ? ptr->getRefCount() : 0);
	}

	inline void release()
	{
		if (m_ref)
		{
			m_ref->release();
			m_ref = 0;
		}
	}
	// Prevent direct assignment from a weak pointer of different type.
	template <class U> WeakPtr<T>& operator =(const WeakPtr<U>& rhs);
};

struct IUnknown;
// just for windows Com
template<typename T>
class ComPtr
{
public:
	typedef T type_t;

	ComPtr() :m_ptr(0){}
	ComPtr(const ComPtr& other) : m_ptr(other.m_ptr)
	{
		addRef();
	}
	template<class U>
	ComPtr(U *other) : m_ptr(other)
	{
		addRef();
	}
	template<class U>
	ComPtr(const ComPtr<U>& other)
		:m_ptr(other.m_ptr)
	{
		addRef();
	}

	~ComPtr()
	{
		release();
	}

	ComPtr& operator=(T* other)
	{
		if (m_ptr != other)
		{
			ComPtr(other).swap(*this);
		}
		return *this;
	}

	ComPtr& operator=(const ComPtr& other)
	{
		if (m_ptr != other.m_ptr)
		{
			ComPtr(other).swap(*this);
		}
		return *this;
	}

	template<class U>
	ComPtr& operator=(U* other)
	{
		ComPtr(other).swap(*this);
		return *this;
	}

	template<class U>
	ComPtr& operator=(const ComPtr<U>& other)
	{
		ComPtr(other).swap(*this);
		return *this;
	}

	typename T* operator->() const
	{
		return m_ptr;
	}

	operator bool() const
	{
		return m_ptr != 0;
	}

	void reset()
	{
		release();
	}

	T* detach() const
	{
		T* ptr = m_ptr;
		m_ptr = 0;
		return ptr;
	}

	T* get() const
	{
		return m_ptr;
	}

	T* const* getAddressOf() const
	{
		return &m_ptr;
	}

	T** getAddressOf()
	{
		return &m_ptr;
	}

	void swap(ComPtr& r)
	{
		T* tmp = m_ptr;
		m_ptr = r.m_ptr;
		r.m_ptr = tmp;
	}

#ifdef CU_OS_WIN
	template<typename U>
	HRESULT as(ComPtr<U>* p) const
	{
		return m_ptr->QueryInterface(__uuidof(U), reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
	}

	HRESULT asIID(REFIID riid, ComPtr<IUnknown>* p) const
	{
		return m_ptr->QueryInterface(__uuidof(U), reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
	}

#endif

#ifdef CU_CPP11
	ComPtr(ComPtr&& other)
		: m_ptr(0)
	{
		this->swap(other);
	}
	template<class U>
	ComPtr(ComPtr<U>&& other)
		: m_ptr(other.m_ptr)
	{
		other.m_ptr = 0;
	}

	ComPtr& operator=(ComPtr&& other)
	{
		ComPtr(static_cast<ComPtr&&>(other)).swap(*this);
		return *this;
	}

	template<class U>
	ComPtr& operator=(ComPtr<U>&& other)
	{
		ComPtr(static_cast<ComPtr<U>&&>(other)).swap(*this);
		return *this;
	}

	void swap(ComPtr&& r)
	{
		T* tmp = m_ptr;
		m_ptr = r.m_ptr;
		r.m_ptr = tmp;
	}
#endif

protected:
	void addRef() const throw()
	{
		if (m_ptr != 0)
			m_ptr->AddRef();
	}

	unsigned long release() throw()
	{
		unsigned long ref = 0;
		if (m_ptr != 0)
		{
			ref = m_ptr->Release();
			m_ptr = 0;
		}
		return ref;
	}
protected:
	template<class U> friend class ComPtr;
	T* m_ptr;
};

// 局部作用域
template<class T>
class ScopedPtr
{
	T* m_ptr;
public:
	ScopedPtr(T* ptr):m_ptr(ptr){}
	~ScopedPtr(){ reset(); }

	void reset(T* p){}
	T& operator *() const { return *m_ptr; }
	T* operator->() const { return  m_ptr; }
	T* get() const { return m_ptr; }
	void swap(ScopedPtr& rhs) { std::swap(m_ptr, rhs->m_ptr); }
};

CU_NS_END