#pragma once
#include "FourCC.h"

CUTE_NS_BEGIN

// RTTI:Run-Time Type Identification
// rename MetaClass??
class CUTE_CORE_API RTTI
{
public:
	// 默认构造函数
	typedef void*(*Creator)();

	RTTI(const RTTI* parent, const char* name, FourCC type, Creator fun);

	bool isKindOf(const RTTI* other) const;
	bool isKindOf(const String& name) const;
	bool isKindOf(uint32_t type) const;

	void* create();

public:
	uint32_t		getType() const { return m_type; }
	uint32_t		getDepth() const { return m_depth; }
	const RTTI*		getParent() const { return m_parent; }
	const String&	getName() const { return m_name; }

private:
	const RTTI*	m_parent;
	String		m_name;
	uint32_t	m_type;
	uint32_t	m_depth;
	Creator		m_creator;
};

// Root根节点
struct RTTIRoot
{
	static RTTI* getStaticRTTI() { return NULL; }
};

// 萃取默认构造函数
template<typename T, typename Enable = void>
struct ConstructTraits
{
	static void* create() 
	{
		return NULL; 
	}
};

template<typename T>
struct ConstructTraits<T, typename std::enable_if<!std::is_abstract<T>::value && std::is_default_constructible<T>::value>::type >
{
	static void* create() 
	{
		return new T(); 
	}
};

// 如果没有基类，Base使用RootRtti
#define __DECLARE_RTTI(CLS, BASE, FOUR_CC)						\
public:															\
	static RTTI* getStaticRTTI(){								\
		static Cute::RTTI rtti(BASE::getStaticRTTI(), #CLS, FOUR_CC, &ConstructTraits<CLS>::create); \
		return &rtti;											\
	};															\
	virtual RTTI* getRTTI() const { return getStaticRTTI(); }	\
private:

#define DECLARE_RTTI(CLS, BASE, FOUR_CC) __DECLARE_RTTI(CLS, BASE, FOUR_CC)

CUTE_NS_END
