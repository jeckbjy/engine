#pragma once
#include "Cute/Foundation.h"
#include "Cute/Ref.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

// EXAMPLE:
//
//  class Controller {
//   public:
//    Controller() : weak_factory_(this) {}
//    void SpawnWorker() { Worker::StartNew(weak_factory_.GetWeakPtr()); }
//    void WorkComplete(const Result& result) { ... }
//   private:
//    // Member variables should appear before the WeakPtrFactory, to ensure
//    // that any WeakPtrs to Controller are invalidated before its members
//    // variable's destructors are executed, rendering them invalid.
//    WeakPtrFactory<Controller> weak_factory_;
//  };
//
//  class Worker {
//   public:
//    static void StartNew(const WeakPtr<Controller>& controller) {
//      Worker* worker = new Worker(controller);
//      // Kick off asynchronous processing...
//    }
//   private:
//    Worker(const WeakPtr<Controller>& controller)
//        : controller_(controller) {}
//    void DidCompleteAsynchronousProcessing(const Result& result) {
//      if (controller_)
//        controller_->WorkComplete(result);
//    }
//    WeakPtr<Controller> controller_;
//  };
//
// With this implementation a caller may use SpawnWorker() to dispatch multiple
// Workers and subsequently delete the Controller, without waiting for all
// Workers to have completed.

// ------------------------- IMPORTANT: Thread-safety -------------------------

// Weak pointers may be passed safely between threads, but must always be
// dereferenced and invalidated on the same thread otherwise checking the
// pointer would be racey.
//
// To ensure correct use, the first time a WeakPtr issued by a WeakPtrFactory
// is dereferenced, the factory and its WeakPtrs become bound to the calling
// thread, and cannot be dereferenced or invalidated on any other thread. Bound
// WeakPtrs can still be handed off to other threads, e.g. to use to post tasks
// back to object on the bound thread.
//
// If all WeakPtr objects are destroyed or invalidated then the factory is
// unbound from the SequencedTaskRunner/Thread. The WeakPtrFactory may then be
// destroyed, or new WeakPtr objects may be used, from a different sequence.
//
// Thus, at least one WeakPtr object must exist and have been dereferenced on
// the correct thread to enforce that other WeakPtr objects will enforce they
// are used on the desired thread.

template <typename T> class SupportsWeakPtr;
template <typename T> class WeakPtr;
template <typename T> class WeakPtrFactory;

// 可以优化成Flag中含有指针,WeakPtr无须单独持有指针，更加节省内存
class CUTE_CORE_API WeakRef
{
	class Flag;
public:
	WeakRef();
	WeakRef(const Flag* flag);
	~WeakRef() {}

	bool isValid() const;
	void reset();

private:
	friend class WeakRefOwner;
	class Flag : public SafeRef<Flag>
	{
	public:
		Flag() :m_valid(true){}
		void invalidate() { m_valid = false; }
		bool isValid() const { return m_valid; }
	private:
		bool m_valid;
	};

	typedef RefPtr<const Flag> FlagPtr;

	FlagPtr m_flag;
};

// 用于创建Flag
class CUTE_CORE_API WeakRefOwner
{
public:
	WeakRefOwner();
	~WeakRefOwner();

	WeakRef getRef() const;
	bool hasRefs() const;

	void invalidate();

private:
	mutable RefPtr<WeakRef::Flag> m_flag;
};

class CUTE_CORE_API WeakPtrBase
{
protected:
	WeakPtrBase();
	WeakPtrBase(const WeakRef& ref);
	~WeakPtrBase();

protected:
	WeakRef m_ref;
};

class CUTE_CORE_API SupportsWeakPtrBase
{
public:
	template<typename Derived>
	static WeakPtr<Derived> asWeakPtr(Derived* t)
	{
		return asWeakPtrImpl<Derived>(t, *t);
	}

private:
	// This template function uses type inference to find a Base of Derived
	// which is an instance of SupportsWeakPtr<Base>. We can then safely
	// static_cast the Base* to a Derived*.
	template <typename Derived, typename Base>
	static WeakPtr<Derived> asWeakPtrImpl(Derived* t, const SupportsWeakPtr<Base>&) {
		WeakPtr<Base> ptr = t->Base::getWeakPtr();
		return WeakPtr<Derived>(ptr.m_ref, static_cast<Derived*>(ptr.m_ptr));
	}
};

template<typename T>
class WeakPtr : public WeakPtrBase
{
public:
	WeakPtr() 
		: m_ptr(NULL)
	{
	}

	template<typename U>
	WeakPtr(const WeakPtr<U> other)
		: WeakPtrBase(other.m_ref), m_ptr(other.m_ptr)
	{
	}

	void reset()
	{
		m_ref.reset();
		m_ptr = NULL;
	}

	bool expired() const
	{
		return get() == NULL;
	}

	bool isValid() const
	{
		return get() != NULL;
	}

	T* get() const
	{
		return m_ref.isValid() ? m_ptr : NULL;
	}

	T* operator -> () const
	{
		return get();
	}

	T& operator *  () const
	{
		return *get();
	}

private:
	WeakPtr(const WeakRef& ref, T* ptr)
		: WeakPtrBase(ref)
		, m_ptr(ptr)
	{
	}

	// Explicitly declare comparison operators as required by the bool
	// trick, but keep them private.
	template <class U> bool operator==(WeakPtr<U> const&) const;
	template <class U> bool operator!=(WeakPtr<U> const&) const;

	template <class U> friend class WeakPtr;

	friend class SupportsWeakPtr<T>;
	friend class WeakPtrFactory<T>;
	friend class SupportsWeakPtrBase;

	T* m_ptr;
};

// A class may be composed of a WeakPtrFactory and thereby
// control how it exposes weak pointers to itself.  This is helpful if you only
// need weak pointers within the implementation of a class.  This class is also
// useful when working with primitive types.  For example, you could have a
// WeakPtrFactory<bool> that is used to pass around a weak reference to a bool.
template<class T>
class WeakPtrFactory
{
public:
	WeakPtrFactory(T* ptr)
		: m_ptr(ptr)
	{
	}

	~WeakPtrFactory()
	{
		m_ptr = NULL;
	}

	WeakPtr<T> getWeakPtr()
	{
		return WeakPtr<T>(m_owner.getRef(), m_ptr);
	}

	void invalidate()
	{
		m_owner.invalidate();
	}

	bool hasWeakPtrs() const
	{
		return m_owner.hasRefs();
	}

private:
	WeakRefOwner m_owner;
	T* m_ptr;
};

// A class may extend from SupportsWeakPtr to let others take weak pointers to
// it. This avoids the class itself implementing boilerplate to dispense weak
// pointers.  However, since SupportsWeakPtr's destructor won't invalidate
// weak pointers to the class until after the derived class' members have been
// destroyed, its use can lead to subtle use-after-destroy issues.
template<class T>
class SupportsWeakPtr : public SupportsWeakPtrBase
{
public:
	SupportsWeakPtr()
	{
	}

	WeakPtr<T> asWeakPtr()
	{
		return WeakPtr<T>(m_owner.getRef(), static_cast<T*>(this));
	}

protected:
	~SupportsWeakPtr()
	{
	}

private:
	WeakRefOwner m_owner;
};

// Helper function that uses type deduction to safely return a WeakPtr<Derived>
// when Derived doesn't directly extend SupportsWeakPtr<Derived>, instead it
// extends a Base that extends SupportsWeakPtr<Base>.
//
// EXAMPLE:
//   class Base : public base::SupportsWeakPtr<Producer> {};
//   class Derived : public Base {};
//
//   Derived derived;
//   base::WeakPtr<Derived> ptr = base::AsWeakPtr(&derived);
//
// Note that the following doesn't work (invalid type conversion) since
// Derived::AsWeakPtr() is WeakPtr<Base> SupportsWeakPtr<Base>::AsWeakPtr(),
// and there's no way to safely cast WeakPtr<Base> to WeakPtr<Derived> at
// the caller.
//
//   base::WeakPtr<Derived> ptr = derived.AsWeakPtr();  // Fails.
template <typename Derived>
WeakPtr<Derived> AsWeakPtr(Derived* t)
{
	return SupportsWeakPtrBase::asWeakPtr<Derived>(t);
}

CUTE_NS_END