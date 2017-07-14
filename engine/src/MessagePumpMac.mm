//! Application
#include "MessagePump.h"

#if (CUTE_OS_MAC)
#import <Cocoa/Cocoa.h>

CUTE_NS_BEGIN

void MessagePump::dispatch()
{
    @autoreleasepool {
        NSEvent* ev =[NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
        if (ev != nil){
            [NSApp sendEvent:ev];
        }
    }
}

void MessagePump::quit()
{
    [NSApp stop:nil];
    [NSEvent startPeriodicEventsAfterDelay:0 withPeriod:0.1];
}

CUTE_NS_END

#endif
