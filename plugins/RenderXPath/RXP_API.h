#pragma once

// define CUTE_RXP_API
#if defined(CUTE_USE_LIB_RXPATH)
// build or use lib
#	define CUTE_RXP_API
#elif defined(CUTE_BUILD_RXPATH)
// build dynamic dll
#	define CUTE_RXP_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_RXP_API CUTE_IMPORT
#endif

// Automatically link Foundation library.
//#if defined(_MSC_VER) && !defined(CUTE_BUILD_RXPATH) && !defined(CUTE_UNAUTOLINK_RXPATH)
//#	pragma comment(lib, "plugin_valkan.lib")
//#endif

#include "Cute/API.h"
#include "Cute/Foundation.h"



