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

// 成员变量
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

// 成员函数
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

// 全局
template<typename T>
class TSetter : public Setter {};

// 成员变量指针
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

// 成员函数指针
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

// 需要处理 enum 成员变量 get set方法
// 属性信息,Property
struct Attribute
{
	String	name;
	Getter* getter;
	Setter* setter;
	Variant value;		// 默认值
	String	annotation;	// 注释??tips
	Attribute(const String& name, Getter* getter, Setter* setter, const Variant& value)
		:name(name), getter(getter), setter(setter), value(value){}
};

typedef Vector<Attribute> AttributeList;

#define DEF_ATTRIBUTE(TYPE,GET,SET,DEFAULT) new Attribute(#TYPE, new TGetter(GET), new TSetter(SET), DEFAULT)

CU_NS_END