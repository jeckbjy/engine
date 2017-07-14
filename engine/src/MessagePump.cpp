//! Application
#include "MessagePump.h"

#if !defined(CUTE_OS_FAMILY_APPLE)

CUTE_NS_BEGIN

void MessagePump::dispatch()
{
//#if defined(CUTE_OS_FAMILY_APPLE)
//    // OSX Message Pump
//    EventRef event = NULL;
//    EventTargetRef targetWindow;
//    targetWindow = GetEventDispatcherTarget();
//    
//    // If we are unable to get the target then we no longer care about events.
//    if( !targetWindow ) return;
//    
//    // Grab the next event, process it if it is a window event
//    while( ReceiveNextEvent( 0, NULL, kEventDurationNoWait, true, &event ) == noErr )
//    {
//        // Dispatch the event
//        SendEventToEventTarget( event, targetWindow );
//        ReleaseEvent( event );
//    }
//#endif
}

void MessagePump::quit()
{
    
}

CUTE_NS_END

#endif
