#import <simd/simd.h>
#import <ModelIO/ModelIO.h>

#import "Renderer.h"
#import "pointer-state.h"

#import "apple-glue.h"
#import "pointer-state.h"

float apple_windowWidth;
float apple_windowHeight;

static const NSUInteger MaxBuffersInFlight = 3;

@implementation Renderer
{
}

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
{
    
    self = [super init];
    if(self)
    {

		NSString *filePath = [[NSBundle mainBundle] resourcePath];

		NSArray *dataFilePaths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
		NSString *dataFilePath = dataFilePaths[0];

        c_glue_init((__bridge void *)view, [filePath UTF8String], [dataFilePath UTF8String]);

    }

    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
	c_glue_update();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{	
	apple_windowWidth = size.width;
	apple_windowHeight = size.height;
}

@end
