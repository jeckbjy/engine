//! Input
#include "Cute/InputHandler.h"
#include "Cute/Input.h"

CUTE_NS_BEGIN

InputHandler::~InputHandler()
{
}

void InputHandler::onButtonDown(ButtonCode code, size_t deviceIdx, uint64 timestamp)
{
	Input::instance().onButtonDown(code, deviceIdx, timestamp);
}

void InputHandler::onButtonUp(ButtonCode code, size_t deviceIdx, uint64 timestamp)
{
	Input::instance().onButtonUp(code, deviceIdx, timestamp);
}

CUTE_NS_END
