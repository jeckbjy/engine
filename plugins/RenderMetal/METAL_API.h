#pragma once

// define CUTE_METAL_API
#if defined(CUTE_USE_LIB_METAL)
// build or use lib
#	define CUTE_METAL_API
#elif defined(CUTE_BUILD_METAL)
// build dynamic dll
#	define CUTE_METAL_API CUTE_EXPORT
#else
// use static lib
#	define CUTE_METAL_API CUTE_IMPORT
#endif

#include "Cute/Graphics.h"
#include <Metal/Metal.h>
