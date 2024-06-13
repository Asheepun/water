//
//  GameViewController.m
//  engine-oc macOS
//
//  Created by Gustav Almström on 2024-01-16.
//

#import "GameViewController.h"
#import "Renderer.h"
#import "pointer-state.h"

bool apple_pointerDown = false;
float apple_pointerX = 0.0;
float apple_pointerY = 0.0;

@implementation GameViewController
{
    MTKView *_view;

    Renderer *_renderer;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    _view = (MTKView *)self.view;

    _view.device = MTLCreateSystemDefaultDevice();

	NSSize size;
	size.width = 400;
	size.height = 800;
	[_view setFrameSize:size];

    if(!_view.device)
    {
		//frame does not work for some reason (gustav's comment)
		CGRect frame;
		frame.origin.x = 100;
		frame.origin.y = 100;
		frame.size.height = 100;
		frame.size.width = 100;
        NSLog(@"Metal is not supported on this device");
        self.view = [[NSView alloc] initWithFrame:frame];
        return;
    }

    _renderer = [[Renderer alloc] initWithMetalKitView:_view];

    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];

	[[_view window] makeFirstResponder:self];

    _view.delegate = _renderer;
}

//2.0 multiplikationen funkar på Olivias laptop, vet ej vad som är korrekt egentligen.
- (void)mouseDown:(NSEvent *)event {
	apple_pointerDown = true;
	apple_pointerX = 2.0 * event.locationInWindow.x;
	apple_pointerY = apple_windowHeight - 2.0 * event.locationInWindow.y;
}

- (void)mouseUp:(NSEvent *)event {
	apple_pointerDown = false;
	apple_pointerX = 2.0 * event.locationInWindow.x;
	apple_pointerY = apple_windowHeight - 2.0 * event.locationInWindow.y;
}

- (void)mouseDragged:(NSEvent *)event {
	apple_pointerX = event.locationInWindow.x;
	apple_pointerY = apple_windowHeight - 2.0 * event.locationInWindow.y;
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)keyDown:(NSEvent *)event {
	printf("keydown!\n");
}

- (void)keyUp:(NSEvent *)event {
	printf("keyup!\n");
}



@end
