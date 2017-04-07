#pragma once
#include "Cute/BehaviorNode.h"

CUTE_NS_BEGIN

class CUTE_CORE_API BTSelector : public BTComposite
{
public:
	virtual Status exec(BehaviorAgent* agent);
};

class CUTE_CORE_API BTSequence : public BTComposite
{
public:
	virtual Status exec(BehaviorAgent* agent);
};


CUTE_NS_END
