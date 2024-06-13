#ifdef __APPLE__

#include "stdio.h"
#include "sys/stat.h"
#include "sys/types.h"

#include "pointer-state.h"

#include "engine/mt3d.h"
#include "engine/engine.h"

String Engine_localPath;
String Engine_resourcePath;
String Engine_dataPath;
AssetManager *Engine_assetManager_p = NULL;

int Engine_clientHeight = 800;
int Engine_clientWidth = 400;
int Engine_statusBarOffset = 0;
Engine_Pointer Engine_pointer;
bool Engine_isFullscreen = false;

bool Engine_backButtonPressed = false;
Engine_Controller Engine_controller;

Engine_Key Engine_keys[ENGINE_KEYS_LENGTH];
int Engine_elapsedFrames = 0;
float engine_frameUpdateTime = 0.0;
float engine_frameDrawTime = 0.0;
float engine_frameTime = 0.0;

bool Engine_fpsModeOn = false;
bool internal_programShouldQuit = false;

std::vector<char> Engine_textInput;

MTK::View *mt_view_p;
MTL::Device *mt_device_p;
MTL::CommandQueue *mt_commandQueue_p;
MTL::CommandBuffer *mt_drawCommandBuffer_p;
dispatch_semaphore_t mt_semaphore;

int currentFrame = 0;
int mt_frameIndex = 0;

void apple_init(void *viewPointer, const char *localPath, const char *dataPath){

	Engine_localPath = string(localPath) / "/";
	Engine_resourcePath = string(localPath) / "/";
	Engine_dataPath = string(dataPath) / "/";
	
	mt_view_p = (MTK::View *)viewPointer;
	mt_device_p = mt_view_p->device();

	mt_view_p->setDepthStencilPixelFormat(MTL::PixelFormatDepth32Float);
	mt_view_p->setColorPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);
	mt_view_p->setSampleCount(1);

	mt_view_p->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0));

	mt_commandQueue_p = mt_device_p->newCommandQueue();

	//for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		//drawCommandBufferPointers[i] = mt_commandQueue_p->commandBuffer();
	//}

	mt_semaphore = dispatch_semaphore_create(MAX_FRAMES_IN_FLIGHT);
	//mt_semaphore = dispatch_semaphore_create(3);
    
	Engine_initDrawing();
    Engine_start();

}

void apple_update(){

	Engine_clientWidth = (int)apple_windowWidth;
	Engine_clientHeight = (int)apple_windowHeight;

	Engine_statusBarOffset = (int)apple_statusBarOffset;

	Engine_pointer.downed = false;
	Engine_pointer.upped = false;

	if(apple_pointerDown){
		if(!Engine_pointer.down){
			Engine_pointer.downed = true;
		}
		Engine_pointer.down = true;
	}else{
		if(Engine_pointer.down){
			Engine_pointer.upped = true;
		}
		Engine_pointer.down = false;
	}

	Engine_pointer.pos.x = apple_pointerX;
	Engine_pointer.pos.y = apple_pointerY;
    
    Engine_update();

	NS::AutoreleasePool *pool_p = NS::AutoreleasePool::alloc()->init();

	mt_drawCommandBuffer_p = mt_commandQueue_p->commandBuffer();

	dispatch_semaphore_wait(mt_semaphore, DISPATCH_TIME_FOREVER);

	mt_drawCommandBuffer_p->addCompletedHandler(^void(MTL::CommandBuffer *commandBuffer_p){
		dispatch_semaphore_signal(mt_semaphore);
	});

	mt_frameIndex = currentFrame % MAX_FRAMES_IN_FLIGHT;
    
    Engine_draw();

	mt_drawCommandBuffer_p->presentDrawable(mt_view_p->currentDrawable());
	mt_drawCommandBuffer_p->commit();

	pool_p->release();

	currentFrame++;

}

void Engine_quit(){
	//does nothing
}

void Engine_setWindowTitle(char *title){
	//does nothing
}

void Engine_setWindowSize(int width, int height){
	//does nothing
}

void Engine_centerWindow(){
	//does nothing
}

void Engine_toggleFullscreen(){
	//does nothing
}

void Engine_setFPSMode(bool mode){
	//does nothing
}

extern "C" {

    #include "apple-glue.h"

	void c_glue_init(void *viewPointer, const char *localPath, const char *dataPath){
		apple_init(viewPointer, localPath, dataPath);
	}

	void c_glue_update(void){
		apple_update();
	}

}

#endif
