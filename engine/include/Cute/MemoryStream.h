#pragma once
#include "Cute/Stream.h"

CUTE_NS_BEGIN

// ChunkedMemoryStream
class CUTE_CORE_API MemoryStream : public Stream
{
public:
	MemoryStream();
	~MemoryStream();

protected:
};

CUTE_NS_END