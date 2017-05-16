#pragma once
#include "Cute/Foundation.h"
#include "Cute/InputDefine.h"

CUTE_NS_BEGIN

class CUTE_CORE_API InputHandler : public Ref<InputHandler>
{
public:
	virtual ~InputHandler();

	virtual void update() = 0;

	void onButtonDown(ButtonCode code, size_t deviceIdx, uint64 timestamp);
	void onButtonUp(ButtonCode code, size_t deviceIdx, uint64 timestamp);
};

CUTE_NS_END
