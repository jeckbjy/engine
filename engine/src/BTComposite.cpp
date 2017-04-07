//! Behavior
#include "Cute/BTComposite.h"

CUTE_NS_BEGIN

BehaviorNode::Status BTSelector::exec(BehaviorAgent* agent)
{
	return BehaviorNode::SUCCESS;
}

BehaviorNode::Status BTSequence::exec(BehaviorAgent* agent)
{
	return SUCCESS;
}

CUTE_NS_END
