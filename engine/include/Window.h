#pragma once
#include "API.h"

#if defined(CU_OS_WINNT)
#	include <Windows.h>
#elif defined(CU_OS_WINRT)
#	include <agile.h>
#elif defined(CU_OS_ANDROID)
#	include <android/native_window.h>
#elif defined(CU_HAS_X11)
#	include <X11/X.h>
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#	include <X11/keysym.h>
#endif

CU_NS_BEGIN

// ���岻ͬ���ʹ���ָ��
#if defined(CU_OS_WINNT)
typedef HWND window_t;
#elif defined(CU_OS_WINRT)
typedef Platform::Agile<Windows::UI::Core::CoreWindow> window_t;
#elif defined(CU_OS_ANDROID)
typedef ::ANativeWindow* window_t;
#elif defined(CU_HAS_X11)
typedef ::Window	window_t;
typedef ::Display	display_t;
#endif

// UIWidget ������UI�༭��??
// ���ڴ���ԭ�����ڣ���ӵ�и��ӹ�ϵ,������Widget,��Ҫ������Ϣѭ��,���Թҽ��ⲿ���
class CU_API Window
{
public:
	Window();
	~Window();

protected:
	window_t m_hwnd;
};

CU_NS_END