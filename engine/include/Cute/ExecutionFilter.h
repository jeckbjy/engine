#pragma once
#include "Cute/SessionFilter.h"

CUTE_NS_BEGIN

// Dispatch, Process, Executor
class CUTE_CORE_API ExecutionFilter : public SessionFilter
{
public:
    void onRead(SessionContext* ctx) OVERRIDE;
};

CUTE_NS_END
