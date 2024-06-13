//
//  AppDelegate.m
//  engine-oc macOS
//
//  Created by Gustav Almström on 2024-01-16.
//

#import "AppDelegate.h"
//#import "pointer-state.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)keyDown:(NSEvent *)event {
	printf("keydown delegate!\n");
}

@end
