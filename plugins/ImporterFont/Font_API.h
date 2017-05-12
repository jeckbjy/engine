#pragma once

// define CUTE_FONT_API
// Ĭ��ʹ��DLL�����ʹ��LIB��Ҫ��������
#if defined(CUTE_USE_LIB_FONT)
// build or use lib
#	define CUTE_FONT_API
#elif defined(CUTE_BUILD_FONT)
// build dynamic dll
#	define CUTE_FONT_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_FONT_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
#if defined(_MSC_VER) && !defined(CUTE_BUILD_FONT) && !defined(CUTE_UNAUTOLINK_FONT)
#	pragma comment(lib, "plugin_font.lib")
#endif
