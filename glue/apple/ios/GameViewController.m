//
//  GameViewController.m
//  engine-oc iOS
//
//  Created by Gustav Almström on 2024-01-16.
//

#import "GameViewController.h"
#import "Renderer.h"
#import "pointer-state.h"

bool apple_pointerDown = false;
float apple_pointerX = 0.0;
float apple_pointerY = 0.0;

float screenScale = 1.0;

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
    _view.backgroundColor = UIColor.blackColor;

    if(!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }

    _renderer = [[Renderer alloc] initWithMetalKitView:_view];

    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];

    _view.delegate = _renderer;

	screenScale = [[UIScreen mainScreen] scale];

}

- (void)touchesBegan:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
    CGPoint location = [[touches allObjects][0] locationInView:_view];
    apple_pointerX = location.x * screenScale;
    apple_pointerY = location.y * screenScale;
    apple_pointerDown = true;
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
    CGPoint location = [[touches allObjects][0] locationInView:_view];
    apple_pointerX = location.x * screenScale;
    apple_pointerY = location.y * screenScale;
}


- (void)touchesEnded:(NSSet<UITouch *> *)touches
           withEvent:(UIEvent *)event {
    CGPoint location = [[touches allObjects][0] locationInView:_view];
    apple_pointerX = location.x * screenScale;
    apple_pointerY = location.y * screenScale;
    apple_pointerDown = false;
}

@end
