#pragma once
#include "FourCC.h"
#include "Attribute.h"

CU_NS_BEGIN

// ��νӿ�ע��?��̳У�
class CU_API Rtti
{
public:
	static Rtti* find(uint32_t type);
	static Rtti* find(const String& name);
	template<typename T>
	static T* create(uint32_t type)
	{
		Rtti* rtti = find(type);
		if (rtti)
			return (T*)rtti->create();
		return NULL;
	}
	template<typename T>
	static T* create(const String& name)
	{
		Rtti* rtti = find(name);
		if (rtti)
			return (T*)rtti->create();
		return NULL;
	}

public:
	typedef void*(*Creator)();
	Rtti(const Rtti* parent, const char* name, FourCC type, Creator fun);

	// �ж�
	bool isKindOf(const Rtti* other) const;
	bool isKindOf(const String& name) const;
	bool isKindOf(uint32_t type) const;

	void*		create() { return m_creator ? m_creator() : NULL; }
	FourCC		getFourCC() const { return m_type; }
	uint32_t	getType() const { return m_type; }
	uint32_t	getDepth() const { return m_depth; }
	const Rtti* getParent() const { return m_parent; }
	const String& getName() const { return m_name; }

	void addAttribute(const Attribute& attr);
	bool hasAttributes() const { return m_registed; }
	const AttributeList& getAttributes() const { return m_attributes; }

private:
	const Rtti*	m_parent;
	String		m_name;
	FourCC		m_type;
	uint32_t	m_depth;	// tree���
	Creator		m_creator;
	bool		m_registed;
	AttributeList m_attributes;
};

template<typename R, typename T>
inline R* cast(T* ptr)
{
	if (ptr && ptr->getRtti()->isKindOf(T::getStaticRtti()))
		return (R*)ptr;
	return NULL;
}

template<typename T, typename Enable = void>
struct rtti_traits
{
	inline static Rtti::Creator getCreator() { return NULL; }
};

template<typename T>
struct rtti_traits<T, typename std::enable_if<!std::is_abstract<T>::value && std::is_default_constructible<T>::value>::type>
{
	inline static void* create() { return new T(); }
	inline static Rtti::Creator getCreator() { return &create; }
};

// ע���Ŀ¼ʱʹ��
struct RootRtti
{
	static Rtti* getStaticRtti() { return NULL; }
};

// ���û�л��࣬Baseʹ��RootRtti
#define __DeclareRTTI(CLS, BASE, FCC)	\
public:\
	static Rtti* getStaticRtti(){ \
		static cute::Rtti rtti(BASE::getStaticRtti(), #CLS, FCC, rtti_traits<CLS>::getCreator()); \
		return &rtti; \
	};\
	virtual Rtti* getRtti() const { return getStaticRtti(); }

#define DECLARE_RTTI(CLS, BASE, FOUR_CC) __DeclareRTTI(CLS, BASE, FOUR_CC)
#define DECLARE_BASE_RTTI(CLS, FOUR_CC)  __DeclareRTTI(CLS, RootRtti, FOUR_CC)
#define OBJECT_TYPE(OBJ) OBJ::getStaticRtti()->getType()

CU_NS_END