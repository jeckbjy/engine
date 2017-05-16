#pragma once
#include "Cute/Foundation.h"
#include "Cute/InputDefine.h"
#include "Cute/InputHandler.h"
#include "Cute/Singleton.h"
#include "Cute/DynamicArray.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

/*
1:支持键盘，鼠标，按下和抬起事件
2:摇杆可以支持多个,支持抬起按下事件，各种move事件，axisMoved,sliderMoved,povMove,vector3Move
3:TODO:移动平台需要支持多指触控
4:TODO:陀螺仪，加速器支持
*/
class CUTE_CORE_API Input : public Singleton<Input>
{
public:
	typedef std::vector<Touch> TouchList;

	Input();
	~Input();

	void setHandler(InputHandler* handler);
	// 更新每一帧状态
	void update();

	size_t			getTouchCount();
	TouchList		getTouches();
	const Touch&	getTouch(int index);

	bool isGyroEnable();

	bool isButtonHeld(ButtonCode code) const;
	bool isButtonDown(ButtonCode code) const;
	bool isButtonUp(ButtonCode code) const;

private:
	void onButtonDown(ButtonCode code, size_t deviceIdx, uint64 timestamp);
	void onButtonUp(ButtonCode code, size_t deviceIdx, uint64 timestamp);
	void onDirty(void* data);

private:
	friend class InputHandler;
	enum State
	{
		STATE_NONE,		// 空闲状态
		STATE_DOWN,		// 此帧按下
		STATE_HELD,		// 持续按下
		STATE_UP,		// 此帧抬起
	};

	struct ButtonData
	{
		State	state;	// 当前状态
		bool	dirty;	// 此帧发生过变化
	};

	typedef DynamicArray<ButtonData>	ButtonArray;
	typedef std::vector<ButtonData*>	DirtyArray;
	typedef RefPtr<InputHandler>		HandlerPtr;

	HandlerPtr	m_handler;
	TouchList	m_touches;
	ButtonArray	m_buttons;
	DirtyArray	m_dirties;
};

CUTE_NS_END
