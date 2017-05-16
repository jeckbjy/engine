#pragma once
#include "Cute/Foundation.h"
#include "Cute/InputDefine.h"
#include "Cute/InputHandler.h"
#include "Cute/Singleton.h"
#include "Cute/DynamicArray.h"
#include "Cute/RefPtr.h"

CUTE_NS_BEGIN

/*
1:֧�ּ��̣���꣬���º�̧���¼�
2:ҡ�˿���֧�ֶ��,֧��̧�����¼�������move�¼���axisMoved,sliderMoved,povMove,vector3Move
3:TODO:�ƶ�ƽ̨��Ҫ֧�ֶ�ָ����
4:TODO:�����ǣ�������֧��
*/
class CUTE_CORE_API Input : public Singleton<Input>
{
public:
	typedef std::vector<Touch> TouchList;

	Input();
	~Input();

	void setHandler(InputHandler* handler);
	// ����ÿһ֡״̬
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
		STATE_NONE,		// ����״̬
		STATE_DOWN,		// ��֡����
		STATE_HELD,		// ��������
		STATE_UP,		// ��̧֡��
	};

	struct ButtonData
	{
		State	state;	// ��ǰ״̬
		bool	dirty;	// ��֡�������仯
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
