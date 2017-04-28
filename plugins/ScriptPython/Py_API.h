#pragma once

// define CUTE_PY_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_PYTHON)
// build or use lib
#	define CUTE_PY_API
#elif defined(CUTE_BUILD_PYTHON)
// build dynamic dll
#	define CUTE_PY_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_PY_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_PYTHON) && !defined(CUTE_UNAUTOLINK_PYTHON)
#	pragma comment(lib, "plugin_py.lib")
#endif

#define PY_PLUGIN_NAME "python"
