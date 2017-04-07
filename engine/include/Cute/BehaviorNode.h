#pragma once
#include "Cute/Foundation.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

class CUTE_CORE_API BehaviorAgent
{

};

class CUTE_CORE_API BehaviorNode : public Object
{
	DECLARE_RTTI(BehaviorNode, Object, OID_ANY)
public:
	enum Status
	{
		SUCCESS,
		FAILURE,
		RUNNING,
	};

	enum Kind
	{
		NONE,
		COMPOSITE,
		DECORATOR,
		CONDITION,
		ACTION,
	};

	typedef RefPtr<BehaviorNode> Ptr;

	BehaviorNode();
 	virtual ~BehaviorNode();

	virtual Kind   kind() const = 0;
	virtual Status exec(BehaviorAgent* agent) = 0;
};

// ���ֻ�������
// ���Ͻڵ�
class CUTE_CORE_API BTComposite : public BehaviorNode
{
public:
	BTComposite();
	virtual ~BTComposite();

	Kind kind() const { return BehaviorNode::COMPOSITE; }

	void addChild(BehaviorNode* node);
	void removeChild(BehaviorNode* node);
	void clear();

protected:
	typedef std::vector<BehaviorNode*> Container;
	Container m_children;
};

// װ�νڵ�
class CUTE_CORE_API BTDecorator : public BehaviorNode
{
	DECLARE_RTTI(BTDecorator, BehaviorNode, OID_ANY)
public:
	virtual ~BTDecorator(){}

	void setChild(BehaviorNode* node);

protected:
	Ptr m_node;
};

// ����
class CUTE_CORE_API BTCondition : public BehaviorNode
{
	DECLARE_RTTI(BTCondition, BehaviorNode, OID_ANY)
public:
	virtual ~BTCondition(){}
	Kind kind() const { return BehaviorNode::CONDITION; }
};

// �¼�
class CUTE_CORE_API BTAction : public BehaviorNode
{
	DECLARE_RTTI(BTAction, BehaviorNode, OID_ANY)
public:
	virtual ~BTAction(){}
	Kind kind() const { return BehaviorNode::ACTION; }
};

CUTE_NS_END
