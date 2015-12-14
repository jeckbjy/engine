#pragma once
#include "SmartPtr.h"
#include "Variant.h"

CU_NS_BEGIN

struct Getter
{
	virtual void get(void* owner, Variant& value) const = 0;
};

struct Setter
{
	virtual void set(void* owner, const Variant& value) = 0;
};

template<typename T>
class TGetter : public Getter {};

// ��Ա����
template<typename T, typename U>
class TGetter<U T::*> : public Getter
{
	typedef U T::* mem_ptr;
	mem_ptr m_ptr;
public:
	TGetter(mem_ptr ptr) :m_ptr(ptr){}
	void get(void* owner, Variant& value) const
	{
		value.set<U>((T*)owner->*m_ptr);
	}
};

// ��Ա����
template<typename T, typename U>
class TGetter<U(T::*)() const> : public Getter
{
	typedef U(T::*fun_t)();
	fun_t m_fun;
public:
	TGetter(fun_t fun) :m_fun(fun){}
	void get(void* owner, Variant& value) const
	{
		value.set(m_fun());
	}
};

// ȫ��
template<typename T>
class TSetter : public Setter {};

// ��Ա����ָ��
template<typename T, typename U>
class TSetter<U T::*> : public Setter
{
	typedef U T::* mem_ptr;
	mem_ptr m_ptr;
public:
	TSetter(mem_ptr ptr) :m_ptr(ptr){}
	void set(void* owner, const Variant& value)
	{
		(T*)owner->*m_ptr = value.get<U>();
	}
};

// ��Ա����ָ��
template<typename T, typename U>
class TSetter<void(T::*)(U)> : public Getter
{
	typedef void(T::*fun_t)(U);
	fun_t m_fun;
public:
	TSetter(fun_t fun) :m_fun(fun){}
	void set(void* owner, const Variant& value)
	{
		m_fun(value.get<U>());
	}
};

// ��Ҫ���� enum ��Ա���� get set����
// ������Ϣ,Property
struct Attribute
{
	String	name;
	Getter* getter;
	Setter* setter;
	Variant value;		// Ĭ��ֵ
	String	annotation;	// ע��??tips
	Attribute(const String& name, Getter* getter, Setter* setter, const Variant& value)
		:name(name), getter(getter), setter(setter), value(value){}
};

typedef Vector<Attribute> AttributeList;

#define DEF_ATTRIBUTE(TYPE,GET,SET,DEFAULT) new Attribute(#TYPE, new TGetter(GET), new TSetter(SET), DEFAULT)

CU_NS_END