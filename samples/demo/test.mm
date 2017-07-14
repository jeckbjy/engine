//
//  test.m
//  demo
//
//  Created by jinyuan.bian on 2017/7/13.
//
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#include "test.h"

//-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender{
//    return YES;
//}

//-(void) closeApplication {
//    [[NSApplication sharedApplication] terminate:nil];
//}

@interface AppDelegate : NSObject {
    
}
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication;
@end

@implementation AppDelegate
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}

@end

int runApp()
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
//    NSApplication* app = [NSApplication sharedApplication];
    //Create the main window
    NSRect rc = NSMakeRect(0, 0, 800, 600);
    NSUInteger uiStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
    NSBackingStoreType backingStoreStyle = NSBackingStoreBuffered;
    NSWindow* win = [[NSWindow alloc] initWithContentRect:rc styleMask:uiStyle backing:backingStoreStyle defer:NO];
//    NSWindow* win = [[NSWindow alloc] init];
    [win setTitle:@"HelloWin Test"];
    [win makeKeyAndOrderFront:win];
    [win makeMainWindow];
    [win center];
//    [win setReleasedWhenClosed:YES];
    
//    NSButton *closeButton = [win standardWindowButton:NSWindowCloseButton];
//    [closeButton setTarget:win];
//    [closeButton setAction:@selector(closeApplication)];
    
    //Start the event loop by calling NSApp run
    AppDelegate* appDelgate = [[AppDelegate alloc] init];
    [NSApp setDelegate: appDelgate];
    [NSApp run];
    [pool drain];
    [appDelgate release];
    return 0;
}
