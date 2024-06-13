#ifdef __linux__

//#include "engine/engine.h"
//#include "engine/strings.h"
//#include "engine/array.h"
//#include "engine/time.h"

//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
//#include "math.h"
//#include "time.h"
//#include "unistd.h"

//#include <chrono>

#include "glad/gl.h"

#include "fcntl.h"
#include "linux/joystick.h"

#include "X11/Xatom.h"
#include "X11/X.h"
#include "X11/Xlib.h"
#include "X11/extensions/Xfixes.h"
#include "X11/XKBlib.h"
#include <X11/Xutil.h>
#include "GL/glx.h"

//#include "glxext.h"

//#define GLX_GLXEXT_PROTOTYPES

//void glXSwapIntervalEXT(Display *dpy, GLXDrawable drawable, int interval);

Display *dpy;
int screenNumber;
Window root;
GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;
XWindowAttributes gwa;
XEvent xev;
int controllerFD;

float leftStickSensitivity = 0.0;
float triggerSensitivity = 0.0;

//String Engine_localPath;
String Engine_resourcePath;
String Engine_dataPath;

bool Engine_backButtonPressed = false;

bool Engine_controllerIsConnected = false;

float Engine_deltaTime = 1.0;

long long lastFrameTime_us = 0;

long long Engine_time_us = 0;
long long Engine_time_ms = 0;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

unsigned int OS_KEY_IDENTIFIERS[] = {

	XK_0,
	XK_1,
	XK_2,
	XK_3,
	XK_4,
	XK_5,
	XK_6,
	XK_7,
	XK_8,
	XK_9,

	XK_A,
	XK_B,
	XK_C,
	XK_D,
	XK_E,
	XK_F,
	XK_G,
	XK_H,
	XK_I,
	XK_J,
	XK_K,
	XK_L,
	XK_M,
	XK_N,
	XK_O,
	XK_P,
	XK_Q,
	XK_R,
	XK_S,
	XK_T,
	XK_U,
	XK_V,
	XK_W,
	XK_X,
	XK_Y,
	XK_Z,

	XK_Up,
	XK_Down,
	XK_Left,
	XK_Right,

	XK_space,
	XK_Escape,
	XK_Return,

	XK_Shift_L,
	XK_Control_L,

};

static int CONTROLLER_BUTTON_IDENTIFIERS[] = {

	3,
	0,
	2,
	1,

	6,
	7,

	4,
	5,

};

AssetManager *Engine_assetManager_p = NULL;

int main(){

	//setup window
	dpy = XOpenDisplay(NULL);

	if(dpy == NULL){
		printf("Cannot open X display!\n");
		return 0;
	}

	root = DefaultRootWindow(dpy);
	screenNumber = DefaultScreen(dpy);

	static int visual_attribs[] = {
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		GLX_SAMPLE_BUFFERS  , 1,
		GLX_SAMPLES         , 4,
		None
    };

    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), visual_attribs, &num_fbc);

    if (!fbc) {
        printf("glXChooseFBConfig() failed\n");
        exit(1);
    }

	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

	int i;
	for(i = 0; i < num_fbc; ++i){

		XVisualInfo *vi = glXGetVisualFromFBConfig(dpy, fbc[i]);

		if(vi){
			int samp_buf, samples;
			glXGetFBConfigAttrib( dpy, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
			glXGetFBConfigAttrib( dpy, fbc[i], GLX_SAMPLES       , &samples  );

			if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
				best_fbc = i, best_num_samp = samples;
			if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
				worst_fbc = i, worst_num_samp = samples;
		}

		XFree( vi );

	}

	GLXFBConfig bestFbc = fbc[ best_fbc ];
	XFree(fbc);

	XVisualInfo *vi = glXGetVisualFromFBConfig( dpy, bestFbc );

	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	//swa.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | StructureNotifyMask | ButtonReleaseMask | PointerMotionMask;
	swa.event_mask = StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | PropertyChangeMask;

	win = XCreateWindow(dpy, root, 0, 0, Engine_clientWidth, Engine_clientHeight, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XStoreName(dpy, win, "Untitled Engine Program");

	XMapWindow(dpy, win);

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB =
        (glXCreateContextAttribsARBProc)
        glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

	static int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		GLX_CONTEXT_FLAGS_ARB, 		   GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

	glc = glXCreateContextAttribsARB(dpy, bestFbc, NULL, GL_TRUE, context_attribs);

	glXMakeCurrent(dpy, win, glc);

	//gladLoadGL(dpy, screenNumber, (GLADloadfunc)glXGetProcAddress);
	//gladLoaderLoadGLX();
	gladLoaderLoadGL();

	//int screen = DefaultScreen(dpy);

	//glXSwapIntervalEXT(dpy, (GLXDrawable)win, 1);
	//glXSwapIntervalEXT(1);

	Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(dpy, win, &wmDelete, 1);

	int autoRepeatIsAvailable;
	XkbSetDetectableAutoRepeat(dpy, true, &autoRepeatIsAvailable);

	//init keycodes
	for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
		Engine_keys[i].OSIdentifier = OS_KEY_IDENTIFIERS[i];
	}

	//common inits
	//initPixelDrawing();
	internal_initKeys();
	internal_initPointer();

	//make controller not freeze program
	controllerFD = open("/dev/input/js0", O_NONBLOCK);

	Engine_resourcePath = string("");
	Engine_dataPath = string("");

	Engine_initDrawing();
	Engine_start();

	//game loop
	bool cursorHidden = false;
	bool lastFocused = false;
	bool focused = false;

	while(!internal_programShouldQuit){

		//check window focus
		{
			lastFocused = focused;
			focused = false;

			Window focusedWindow;
			int returnInt;
			XGetInputFocus(dpy, &focusedWindow, &returnInt);

			if(focusedWindow == win){
				focused = true;
			}
		}

		//printf("%i\n", focused);

		if(focused){

			//handle window and button press events
			while(XPending(dpy) > 0){

				XNextEvent(dpy, &xev);

				if(xev.type == ClientMessage
				|| xev.type == DestroyNotify){
					internal_programShouldQuit = true;
				}

				if(xev.type == ConfigureNotify){

					XConfigureEvent xce = xev.xconfigure;

					if(xce.width != Engine_clientWidth
					|| xce.height != Engine_clientHeight){
						Engine_clientWidth = xce.width;
						Engine_clientHeight = xce.height;
					}

				}

				if(xev.type == ButtonPress){
					XButtonEvent *buttonEvent_p = (XButtonEvent *)&xev;

					if(buttonEvent_p->button == 1){
						Engine_pointer.down = true;
						Engine_pointer.downed = true;
						Engine_pointer.lastDownedPos = Engine_pointer.pos;
					}

				}
				if(xev.type == ButtonRelease){

					XButtonEvent *buttonEvent_p = (XButtonEvent *)&xev;

					if(buttonEvent_p->button == 1){
						Engine_pointer.down = false;
						Engine_pointer.upped = true;
						Engine_pointer.lastUppedPos = Engine_pointer.pos;
					}

					if(buttonEvent_p->button == 4){
						Engine_pointer.scroll++;
					}
					if(buttonEvent_p->button == 5){
						Engine_pointer.scroll--;
					}

				}

				if(xev.type == KeyPress){

					char buffer[SMALL_STRING_SIZE];
					String_set(buffer, "", SMALL_STRING_SIZE);

					XLookupString((XKeyPressedEvent *)&xev, buffer, STRING_SIZE, NULL, NULL);

					if(!(strcmp(buffer, "") == 0)){
						Engine_textInput.push(*buffer);
					}

				}

			}

			//get keyboard state
			{
				char keys[32];
				XQueryKeymap(dpy, keys);

				for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){

					int keyCode = XKeysymToKeycode(dpy, OS_KEY_IDENTIFIERS[i]);
					int byteIndex = keyCode / 8;
					int bitIndex = keyCode % 8;

					if((keys[byteIndex] >> bitIndex) & 0x01){
						if(!Engine_keys[i].down){
							Engine_keys[i].downed = true;
						}
						Engine_keys[i].down = true;
					}else{
						if(Engine_keys[i].down){
							Engine_keys[i].upped = true;
						}
						Engine_keys[i].down = false;
					}
					
				}
			}

			//get pointer state
			{
				Window returnWindow;
				int returnInt;
				int XPointerX, XPointerY;
				unsigned int buttonMask;
				if(XQueryPointer(dpy, win, &returnWindow, &returnWindow, &returnInt, &returnInt, &XPointerX, &XPointerY, &buttonMask)){

					Engine_pointer.pos.x = XPointerX;
					Engine_pointer.pos.y = XPointerY;

					Engine_pointer.movement.x = Engine_pointer.pos.x - Engine_clientWidth / 2;
					Engine_pointer.movement.y = Engine_pointer.pos.y - Engine_clientHeight / 2;

				}
			}

			//check if controller is connected
			if(access("/dev/input/js0", F_OK) == 0){
				if(controllerFD == -1){
					controllerFD = open("/dev/input/js0", O_NONBLOCK);
				}
			}else{
				controllerFD = -1;
			}

			if(controllerFD == -1){
				Engine_controllerIsConnected = false;
			}else{
				Engine_controllerIsConnected = true;
			}

			//handle controller events
			if(Engine_controllerIsConnected){

				struct js_event jse;

				while(read(controllerFD, &jse, sizeof(jse)) > 0){

					if(jse.type == 1){

						for(int i = 0; i < ENGINE_CONTROLLER_LINUX_BUTTONS_LENGTH; i++){

							if(jse.number == CONTROLLER_BUTTON_IDENTIFIERS[i]){

								if(jse.value == 1){
									if(!Engine_controller.buttons[i].down){
										Engine_controller.buttons[i].downed = true;
									}
									Engine_controller.buttons[i].down = true;
								}

								if(jse.value == 0){
									if(Engine_controller.buttons[i].down){
										Engine_controller.buttons[i].upped = true;
									}
									Engine_controller.buttons[i].down = false;
								}

							}

						}

					}

					if(jse.type == 2){

						if(jse.number == 0){
							Engine_controller.leftStick.x = (float)jse.value / 32767.0;
						}
						if(jse.number == 1){
							Engine_controller.leftStick.y = (float)jse.value / 32767.0;
						}
						if(jse.number == 3){
							Engine_controller.rightStick.x = (float)jse.value / 32767.0;
						}
						if(jse.number == 4){
							Engine_controller.rightStick.y = (float)jse.value / 32767.0;
						}

						if(jse.number == 2){
							Engine_controller.leftTrigger = (float)(jse.value + 32767) / (2.0 * 32767.0);
						}
						if(jse.number == 5){
							Engine_controller.rightTrigger = (float)(jse.value + 32767) / (2.0 * 32767.0);
						}

						if(jse.number == 6){
							if(jse.value < 0){
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT].down = true;
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT].downed = true;
							}
							if(jse.value > 0){
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT].down = true;
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT].downed = true;
							}
							if(jse.value == 0){

								if(Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT].down){
									Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT].upped = true;
								}
								if(Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT].down){
									Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT].upped = true;
								}

								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT].down = false;
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT].down = false;
							}
						}
						if(jse.number == 7){
							if(jse.value < 0){
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_UP].down = true;
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_UP].downed = true;
							}
							if(jse.value > 0){
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_DOWN].down = true;
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_DOWN].downed = true;
							}
							if(jse.value == 0){

								if(Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_UP].down){
									Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_UP].upped = true;
								}
								if(Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_DOWN].down){
									Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_DOWN].upped = true;
								}

								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_UP].down = false;
								Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_DOWN].down = false;
							}
						}

					}

				}

				float Engine_triggerButtonSensitivity = 0.5;
				float Engine_stickButtonSensitivity = 0.5;

				//create discrete buttons out of analog controller input

				//handle button from left trigger
				if(Engine_controller.leftTrigger > Engine_triggerButtonSensitivity){
					if(!Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT_TRIGGER].down){
						Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT_TRIGGER].downed = true;
					}
					Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT_TRIGGER].down = true;
				}else{
					if(Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT_TRIGGER].down){
						Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT_TRIGGER].upped = true;
					}
					Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_LEFT_TRIGGER].down = false;
				}

				//handle button from right trigger
				if(Engine_controller.rightTrigger > Engine_triggerButtonSensitivity){
					if(!Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT_TRIGGER].down){
						Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT_TRIGGER].downed = true;
					}
					Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT_TRIGGER].down = true;
				}else{
					if(Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT_TRIGGER].down){
						Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT_TRIGGER].upped = true;
					}
					Engine_controller.buttons[ENGINE_CONTROLLER_BUTTON_RIGHT_TRIGGER].down = false;
				}

				//handle buttons from left stick
				{
					int maxDotIndex = -1;

					if(length(Engine_controller.leftStick) > Engine_stickButtonSensitivity){

						Vec2f directionVectors[] = {
							{ 0.0, -1.0 },
							{ 0.0, 1.0 },
							{ -1.0, 0.0 },
							{ 1.0, 0.0 },
						};

						float maxDot = 0.0;

						for(int i = 0; i < 4; i++){

							float checkDot = dot(directionVectors[i], Engine_controller.leftStick);

							if(checkDot > maxDot){
								maxDot = checkDot;
								maxDotIndex = i;
							}

						}

					}

					for(int i = 0; i < 4; i++){

						int buttonIndex = ENGINE_CONTROLLER_BUTTON_LEFT_STICK_UP + i;

						if(i == maxDotIndex){
							if(!Engine_controller.buttons[buttonIndex].down){
								Engine_controller.buttons[buttonIndex].downed = true;
							}
							Engine_controller.buttons[buttonIndex].down = true;
						}else{
							if(Engine_controller.buttons[buttonIndex].down){
								Engine_controller.buttons[buttonIndex].upped = true;
							}
							Engine_controller.buttons[buttonIndex].down = false;
						}
						
					}
				}
			
				//handle buttons from right stick
				{
					int maxDotIndex = -1;

					if(length(Engine_controller.rightStick) > Engine_stickButtonSensitivity){

						Vec2f directionVectors[] = {
							{ 0.0, -1.0 },
							{ 0.0, 1.0 },
							{ -1.0, 0.0 },
							{ 1.0, 0.0 },
						};

						float maxDot = 0.0;

						for(int i = 0; i < 4; i++){

							float checkDot = dot(directionVectors[i], Engine_controller.rightStick);

							if(checkDot > maxDot){
								maxDot = checkDot;
								maxDotIndex = i;
							}

						}

					}

					for(int i = 0; i < 4; i++){

						int buttonIndex = ENGINE_CONTROLLER_BUTTON_RIGHT_STICK_UP + i;

						if(i == maxDotIndex){
							if(!Engine_controller.buttons[buttonIndex].down){
								Engine_controller.buttons[buttonIndex].downed = true;
							}
							Engine_controller.buttons[buttonIndex].down = true;
						}else{
							if(Engine_controller.buttons[buttonIndex].down){
								Engine_controller.buttons[buttonIndex].upped = true;
							}
							Engine_controller.buttons[buttonIndex].down = false;
						}
						
					}
				}

			}


			//do fps magic
			if(Engine_fpsModeOn){

				/*
				if(!lastFocused){
					XFixesHideCursor(dpy, root);
					XFlush(dpy);
				}
				*/

				int screenWidth = DisplayWidth(dpy, DefaultScreen(dpy));
				int screenHeight = DisplayHeight(dpy, DefaultScreen(dpy));

				XWarpPointer(dpy, None, root, 0, 0, 0, 0, screenWidth / 2, screenHeight / 2);
				//XWarpPointer(dpy, None, root, 0, 0, 0, 0, Engine_clientWidth / 2, Engine_clientWidth / 2);

			}


		}

		//handle cursor visibility
		if(focused
		&& Engine_fpsModeOn){
			if(!cursorHidden){
				XFixesHideCursor(dpy, root);
				XFlush(dpy);
				cursorHidden = true;
			}
		}else{
			if(cursorHidden){
				XFixesShowCursor(dpy, root);
				XFlush(dpy);
				cursorHidden = false;
			}
		}

		/*
		if(!focused
		&& lastFocused
		&& Engine_fpsModeOn){
			XFixesShowCursor(dpy, root);
			XFlush(dpy);
		}
		*/

		internal_checkWindowDimensionChange();

		long long ust = getTime_us();

		Engine_update();

		internal_resetKeys();
		internal_resetPointer();

		long long uet = getTime_us();
		long long dst = getTime_us();

		Engine_draw();

		glXSwapBuffers(dpy, win);

		glFinish();

		long long det = getTime_us();

		bool printTimings = false;
		//bool printTimings = true;
		if(printTimings){
			print("update time");
			print((int)(uet - ust));
			print("draw time");
			print((int)(det - dst));
			int totalTime = det - ust;
			if(totalTime > 17000){
				print("total time - TO HIGH!");
			}else{
				print("total time");
			}
			print(totalTime);
		}

		Engine_elapsedFrames++;

		long long frameTime_us = getTime_us();

		long long deltaFrameTime_us = frameTime_us - lastFrameTime_us;
		if(lastFrameTime_us == 0){
			deltaFrameTime_us = 16666;
		}

		lastFrameTime_us = frameTime_us;

		Engine_deltaTime = (float)deltaFrameTime_us / (1000.0 * 1000.0 / 60.0);

		Engine_time_us += deltaFrameTime_us;
		Engine_time_ms = Engine_time_us / 1000;
		//Engine_deltaTime = 1.0;
	
	}

	Engine_finish();

	return 0;

}

void Engine_setWindowSize(int width, int height){

	Engine_clientWidth = width;
	Engine_clientHeight = height;

	XResizeWindow(dpy, win, width, height);

}

void Engine_centerWindow(){

	XMoveWindow(dpy, win, DisplayWidth(dpy, screenNumber) / 2 - Engine_clientWidth / 2, DisplayHeight(dpy, screenNumber) / 2 - Engine_clientHeight / 2);

}

void Engine_toggleFullscreen(){

	if(!Engine_isFullscreen){

		Atom wm_state = XInternAtom(dpy, "_NET_WM_STATE", False);
		Atom fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);

		XEvent xev;
		memset(&xev, 0, sizeof(xev));
		xev.type = ClientMessage;
		xev.xclient.window = win;
		xev.xclient.message_type = wm_state;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 1;
		xev.xclient.data.l[1] = fullscreen;
		xev.xclient.data.l[2] = 0;

		XMapWindow(dpy, win);

		XSendEvent (dpy, DefaultRootWindow(dpy), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);

		Engine_isFullscreen = true;

	}else{

		Atom wm_state = XInternAtom(dpy, "_NET_WM_STATE", False);
		Atom fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);

		XEvent xev;
		memset(&xev, 0, sizeof(xev));
		xev.type = ClientMessage;
		xev.xclient.window = win;
		xev.xclient.message_type = wm_state;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 0;
		xev.xclient.data.l[1] = fullscreen;
		xev.xclient.data.l[2] = 0;

		XMapWindow(dpy, win);

		XSendEvent (dpy, DefaultRootWindow(dpy), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);

		Engine_isFullscreen = false;

	}

}

#endif
