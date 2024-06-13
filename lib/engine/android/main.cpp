#include <jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <memory>
//#include <vector>
#include "assert.h"

#include "engine/AndroidOut.h"

//#include "engine/engine.h"
//#include "engine/files.h"
//#include "engine/time.h"

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

AssetManager *Engine_assetManager_p;

auto display = EGL_NO_DISPLAY;
auto surface = EGL_NO_SURFACE;
auto context = EGL_NO_CONTEXT;

bool initedWindow = false;
bool windowExists = false;

bool Engine_backButtonPressed = false;

float Engine_deltaTime = 1.0;

long long lastFrameTime_us = 0;

long long Engine_time_us = 0;
long long Engine_time_ms = 0;

//String Engine_localPath;
String Engine_resourcePath;
String Engine_dataPath;

int testVar = 10;

extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.c>

#define PRINT_GL_STRING(s) {aout << #s": "<< glGetString(s) << std::endl;}
#define PRINT_GL_STRING_AS_LIST(s) { \
std::istringstream extensionStream((const char *) glGetString(s));\
std::vector<std::string> extensionList(\
        std::istream_iterator<std::string>{extensionStream},\
        std::istream_iterator<std::string>());\
aout << #s":\n";\
for (auto& extension: extensionList) {\
    aout << extension << "\n";\
}\
aout << std::endl;\
}

/*!
 * Handles commands sent to this Android application
 * @param pApp the app the commands are coming from
 * @param cmd the command to handle
 */
void handle_cmd(android_app *pApp, int32_t cmd){

    if(cmd == APP_CMD_PAUSE){
		Engine_pause();
        //Engine_finnish();
    }
    if(cmd == APP_CMD_RESUME){
		Engine_resume();
        //Engine_finnish();
    }

    if(cmd == APP_CMD_TERM_WINDOW){

		Engine_freeDrawing();
		windowExists = false;

		if(display != EGL_NO_DISPLAY){
			eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if(context != EGL_NO_CONTEXT){
				eglDestroyContext(display, context);
				context = EGL_NO_CONTEXT;
			}
			if(surface != EGL_NO_SURFACE){
				eglDestroySurface(display, surface);
				surface = EGL_NO_SURFACE;
			}
			eglTerminate(display);
			display = EGL_NO_DISPLAY;
		}

    }

    if(cmd == APP_CMD_INIT_WINDOW){

        int x = 0;
        testVar++;

        //init window and gl context
        constexpr
            EGLint attribs[] = {
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                    EGL_BLUE_SIZE, 8,
                    EGL_GREEN_SIZE, 8,
                    EGL_RED_SIZE, 8,
                    EGL_DEPTH_SIZE, 24,
                    EGL_NONE
            };

        // The default display is probably what you want on Android
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(display, nullptr, nullptr);

        // figure out how many configs there are
        EGLint numConfigs;
        eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);

        // get the list of configurations
        std::unique_ptr < EGLConfig[] > supportedConfigs(new EGLConfig[numConfigs]);
        eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);

        // Find a config we like.
        // Could likely just grab the first if we don't care about anything else in the config.
        // Otherwise hook in your own heuristic
        EGLConfig config = NULL;

        for(int i = 0; i < numConfigs; i++) {

            EGLConfig checkConfig = supportedConfigs[i];
            EGLint red, green, blue, depth;

            if (eglGetConfigAttrib(display, checkConfig, EGL_RED_SIZE, &red)
            && eglGetConfigAttrib(display, checkConfig, EGL_GREEN_SIZE, &green)
            && eglGetConfigAttrib(display, checkConfig, EGL_BLUE_SIZE, &blue)
            && eglGetConfigAttrib(display, checkConfig, EGL_DEPTH_SIZE, &depth)
            && red == 8 && green == 8 && blue == 8 && depth == 24
            || i == numConfigs - 1) {
                config = checkConfig;

                break;
            }

        }

        aout << "Found " << numConfigs << " configs" << std::endl;
        aout << "Chose " << config << std::endl;

        // create the proper window surface
        EGLint format;
        eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
        surface = eglCreateWindowSurface(display, config, pApp->window, nullptr);

        // Create a GLES 3 context
        EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
        context = eglCreateContext(display, config, nullptr, contextAttribs);

        // get some window metrics
        auto madeCurrent = eglMakeCurrent(display, surface, surface, context);
        assert(madeCurrent);

		eglSwapInterval(display, 1);

        // make width and height invalid so it gets updated the first frame in @a updateRenderArea()
        Engine_clientWidth = -1;
        Engine_clientHeight = -1;

        PRINT_GL_STRING(GL_VENDOR);
        PRINT_GL_STRING(GL_RENDERER);
        PRINT_GL_STRING(GL_VERSION);
        PRINT_GL_STRING_AS_LIST(GL_EXTENSIONS);

        // enable alpha globally for now, you probably don't want to do this in a game
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Engine_assetManager_p = pApp->activity->assetManager;

		Engine_initDrawing();

        if(!initedWindow){
            Engine_start();
        }

        initedWindow = true;
        windowExists = true;

    }
}

/*!
 * Enable the motion events you want to handle; not handled events are
 * passed back to OS for further processing. For this example case,
 * only pointer and joystick devices are enabled.
 *
 * @param motionEvent the newly arrived GameActivityMotionEvent.
 * @return true if the event is from a pointer or joystick device,
 *         false for all other input devices.
 */
bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent) {
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
            sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

/*!
 * This the main entry point for a native activity
 */
void android_main(struct android_app *pApp) {

    // Can be removed, useful to ensure your code is running
    aout << "Welcome to android_main" << std::endl;

    Engine_resourcePath = string("");
    Engine_dataPath = string(pApp->activity->internalDataPath);

    int x = 0;

    //clear instance data
    //String tmp = string("");
    //writeDataToFile(Engine_localPath / "instanceSaveData.txt", tmp, 0);

    // Register an event handler for Android events
    pApp->onAppCmd = handle_cmd;

    // Set input event filters (set it to NULL if the app wants to process all inputs).
    // Note that for key inputs, this example uses the default default_key_filter()
    // implemented in android_native_app_glue.c.
    android_app_set_motion_event_filter(pApp, motion_event_filter_func);


    // This sets up a typical game/event loop. It will run until the app is destroyed.
    int events;
    android_poll_source *pSource;
    do {

        // Process all pending events before running game logic.
        if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
            if (pSource) {
                pSource->process(pApp, pSource);
            }
        }

        if(initedWindow) {

            //reset pointer state
            Engine_pointer.downed = false;
            Engine_pointer.upped = false;
            Engine_backButtonPressed = false;

            //check for pointer input
            auto *inputBuffer = android_app_swap_input_buffers(pApp);

            if(inputBuffer) {

				for(auto i = 0; i < inputBuffer->keyEventsCount; i++){
                    auto &keyEvent = inputBuffer->keyEvents[i];
					if(keyEvent.keyCode == AKEYCODE_BACK
                    && keyEvent.action == AKEY_EVENT_ACTION_DOWN){
						Engine_backButtonPressed = true;
					}
				}

                for (auto i = 0; i < inputBuffer->motionEventsCount; i++) {
                    auto &motionEvent = inputBuffer->motionEvents[i];
                    auto action = motionEvent.action;

                    // Find the pointer index, mask and bitshift to turn it into a readable value.
                    auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                            >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

                    // get the x and y position of this event if it is not ACTION_MOVE.
                    auto &pointer = motionEvent.pointers[pointerIndex];
                    auto x = GameActivityPointerAxes_getX(&pointer);
                    auto y = GameActivityPointerAxes_getY(&pointer);

                    // determine the action type and process the event accordingly.
                    switch (action & AMOTION_EVENT_ACTION_MASK) {
                        case AMOTION_EVENT_ACTION_DOWN:
                        case AMOTION_EVENT_ACTION_POINTER_DOWN:
                            Engine_pointer.downed = true;
                            Engine_pointer.down = true;

                            pointer = motionEvent.pointers[0];
                            Engine_pointer.pos.x = GameActivityPointerAxes_getX(&pointer);
                            Engine_pointer.pos.y = GameActivityPointerAxes_getY(&pointer);
                            break;

                        case AMOTION_EVENT_ACTION_CANCEL:
                            // treat the CANCEL as an UP event: doing nothing in the app, except
                            // removing the pointer from the cache if pointers are locally saved.
                            // code pass through on purpose.
                        case AMOTION_EVENT_ACTION_UP:
                        case AMOTION_EVENT_ACTION_POINTER_UP:

                            Engine_pointer.upped = true;
                            Engine_pointer.down = false;

                            pointer = motionEvent.pointers[0];
                            Engine_pointer.pos.x = GameActivityPointerAxes_getX(&pointer);
                            Engine_pointer.pos.y = GameActivityPointerAxes_getY(&pointer);
                            break;

                        case AMOTION_EVENT_ACTION_MOVE:
                            pointer = motionEvent.pointers[0];
                            Engine_pointer.pos.x = GameActivityPointerAxes_getX(&pointer);
                            Engine_pointer.pos.y = GameActivityPointerAxes_getY(&pointer);
                            break;
                        default:
                            aout << "Unknown MotionEvent Action: " << action;
                    }
                    aout << std::endl;
                }

                android_app_clear_motion_events(inputBuffer);
                android_app_clear_key_events(inputBuffer);

            }

            if(windowExists) {

                //update window size
                EGLint width;
                eglQuerySurface(display, surface, EGL_WIDTH, &width);

                EGLint height;
                eglQuerySurface(display, surface, EGL_HEIGHT, &height);

                Engine_clientWidth = width;
                Engine_clientHeight = height;

                Engine_assetManager_p = pApp->activity->assetManager;

                Engine_update();

                Engine_draw();

                // Present the rendered image. This is an implicit glFlush.
                auto swapResult = eglSwapBuffers(display, surface);
                assert(swapResult == EGL_TRUE);

				glFinish();

				long long frameTime_us = getTime_us();

				long long deltaFrameTime_us = frameTime_us - lastFrameTime_us;
				if(lastFrameTime_us == 0){
					deltaFrameTime_us = 16666;
				}

				lastFrameTime_us = frameTime_us;

                //aout << deltaFrameTime << std::endl;

				Engine_deltaTime = (float)deltaFrameTime_us / (1000.0 * 1000.0 / 60.0);

				Engine_time_us += deltaFrameTime_us;
				Engine_time_ms = Engine_time_us / 1000;

            }

        }

        if(internal_programShouldQuit) {
            GameActivity_finish(pApp->activity);
            internal_programShouldQuit = false;
        }

    } while (!pApp->destroyRequested);
}
}

void Engine_setWindowSize(int x, int y){
	//do nothing
}

void Engine_toggleFullscreen(){
	//do nothing
}

void Engine_centerWindow(){
	//do nothing
}
