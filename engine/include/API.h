#pragma once
#include "Platform.h"
#include "Types.h"
#include "Macro.h"
#include "Ref.h"

// inlineѧϰ:http://blog.csdn.net/nanhaizhixin/article/details/7564084
// msvc:dll��lib��������ͬλ�����ã�
// http://www.cnblogs.com/lidabo/archive/2013/08/16/3261305.html
// ��Ҫ�ڹ��캯��/���������е����麯��

// auto link �Զ�����
#if defined(_MSC_VER) && !defined(CU_UN_AUTO_LINK) && !defined(CU_BUILD_LIB) && !defined(CU_BUILD_DLL)
#	pragma comment(lib, "engine.lib")
#endif

#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif

