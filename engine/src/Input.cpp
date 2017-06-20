//! Input
#include "Cute/Input.h"

CUTE_NS_BEGIN

Input::Input()
{
	m_buttons.resize(BC_COUNT);
}

Input::~Input()
{
}

void Input::setHandler(InputHandler* handler)
{
	m_handler = handler;
}

void Input::update()
{
	// 更新状态
	for (size_t i = 0; i < m_dirties.size(); ++i)
	{
		ButtonData* data = m_dirties[i];
		data->dirty = false;

		// 重置状态
		if (data->state == STATE_DOWN)
			data->state = STATE_HELD;
		else if (data->state == STATE_UP)
			data->state = STATE_NONE;
	}

	m_dirties.clear();

	// 设置
	if (m_handler)
		m_handler->update();
}

void Input::onButtonDown(ButtonCode code, size_t deviceIdx, uint64 timestamp)
{
	// todo:deviceIdx for joystick
	if (code >= BC_COUNT)
		return;

	ButtonData* data = &m_buttons[code];
	if (data->state != STATE_DOWN && data->state != STATE_HELD)
	{
		data->state = STATE_DOWN;
		onDirty(data);
	}
}

void Input::onButtonUp(ButtonCode code, size_t deviceIdx, uint64 timestamp)
{
	if (code >= BC_COUNT)
		return;

	ButtonData* data = &m_buttons[code];
	if (data->state != STATE_UP)
	{
		data->state = STATE_UP;
		onDirty(data);
	}
}

void Input::onDirty(void* buttonData)
{
	ButtonData* data = (ButtonData*)buttonData;
	if (data->dirty == false)
	{
		data->dirty = true;
		m_dirties.push_back(data);
	}
}

bool Input::isButtonHeld(ButtonCode code) const
{
	const ButtonData* data = &m_buttons[code];
	return (data->state == STATE_HELD || data->state == STATE_DOWN);
}

bool Input::isButtonDown(ButtonCode code) const
{
	const ButtonData* data = &m_buttons[code];
	return data->state == STATE_DOWN;
}

bool Input::isButtonUp(ButtonCode code) const
{
	const ButtonData* data = &m_buttons[code];
	return data->state == STATE_UP;
}

CUTE_NS_END
