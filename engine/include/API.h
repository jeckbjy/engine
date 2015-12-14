#pragma once
#include "Platform.h"
#include "Types.h"
#include "Macro.h"
#include "Ref.h"

// inline学习:http://blog.csdn.net/nanhaizhixin/article/details/7564084
// msvc:dll和lib导出到不同位置配置：
// http://www.cnblogs.com/lidabo/archive/2013/08/16/3261305.html
// 不要在构造函数/析构函数中调用虚函数

// auto link 自动连接
#if defined(_MSC_VER) && !defined(CU_UN_AUTO_LINK) && !defined(CU_BUILD_LIB) && !defined(CU_BUILD_DLL)
#	pragma comment(lib, "engine.lib")
#endif

#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#endif

