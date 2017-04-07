//! Behavior
#include "Cute/BehaviorNode.h"

CUTE_NS_BEGIN

BehaviorNode::BehaviorNode()
{
}

BehaviorNode::~BehaviorNode()
{
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BTComposite::BTComposite()
{
}

BTComposite::~BTComposite()
{
}

void BTComposite::addChild(BehaviorNode* node)
{
	m_children.push_back(node);
}

void BTComposite::removeChild(BehaviorNode* node)
{
	Container::iterator itor = std::find(m_children.begin(), m_children.end(), node);
	if (itor != m_children.end())
	{
		m_children.erase(itor);
	}
}

void BTComposite::clear()
{
	m_children.clear();
}

CUTE_NS_END
