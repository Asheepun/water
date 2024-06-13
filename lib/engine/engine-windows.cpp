#ifdef _WIN32

#include "engine/engine.h"

#include "glad/gl.h"
#include "glad/wgl.h"

#include "stdio.h"
#include "time.h"
#include <limits.h>
#include <winnt.h>

#include "windows.h"
#include "winuser.h"

HWND hwnd;

static unsigned int OS_KEY_IDENTIFIERS[] = {

	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,

	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4D,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,

	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,

	VK_SPACE,
	VK_ESCAPE,
	VK_RETURN,

	VK_SHIFT,
	VK_CONTROL,

};

AssetManager *Engine_assetManager_p;
//String Engine_localPath;
String Engine_resourcePath;
String Engine_dataPath;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	//setup window
	const char CLASS_NAME[] = "Untitled Engine Program";
	
	WNDCLASS wc = {};
	
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	
	RegisterClass(&wc);
	
	hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		"Untitled Engine Program",
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if(hwnd == NULL){
		printf("Could not create Window");
		return 0;
	}
	
	HDC hdc = GetDC(hwnd);
	
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	
	int pf = ChoosePixelFormat(hdc, &pfd);
	
	if(pf == 0){
		printf("Could not choose pixel format\n");
		return 0;
	}
	
	SetPixelFormat(hdc, pf, &pfd);
	
	DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	
	HGLRC hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	gladLoaderLoadWGL(hdc);
	//gladLoadWGL(hdc);
	gladLoaderLoadGL();
	//gladLoadWGL(hdc, (GLADloadfunc)wglGetProcAddress);
	//gladLoadGL((GLADloadfunc)glGetProcAddress);
	//gladLoadGL((GLADloadfunc)wglGetProcAddress);
	//gladLoadGL((GLADloadfunc)GetProcAddress);

	wglSwapIntervalEXT(1);

	printf("%s\n", glGetString(GL_VERSION));
	//printf("%s\n", glGetString(GL_EXTENSIONS));
	//printf("%s\n", wglGetExtensionsStringARB());

	//wglSwapIntervalEXT(0);
	//init keycodes
	for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
		Engine_keys[i].OSIdentifier = OS_KEY_IDENTIFIERS[i];
	}

	//common inits
	//initPixelDrawing();
	internal_initKeys();
	internal_initPointer();

	Engine_resourcePath = string("");
	Engine_dataPath = string("");
	
	Engine_initDrawing();
	Engine_start();
	
	ShowWindow(hwnd, nCmdShow);

	LARGE_INTEGER liFrequency = {0};
	LARGE_INTEGER liStart = {0};
	LARGE_INTEGER liStop = {0};

	float deltaTime;

	float accumilatedTime = 0;

	bool cursorHidden = false;
	bool lastFocused = false;
	bool focused = false;

	//game loop
	while(!internal_programShouldQuit){

		QueryPerformanceFrequency(&liFrequency);

		QueryPerformanceCounter(&liStart);

		//check window focus
		{
			lastFocused = focused;
			focused = false;

			HWND focusedWindow = GetFocus();

			if(focusedWindow == hwnd){
				focused = true;
			}
		
		}
	
		//handle events
		MSG msg = {};
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		
		}

		//do fps magic
		if(Engine_fpsModeOn
		&& focused){

			RECT windowRect;
			RECT clientRect;
			GetWindowRect(hwnd, &windowRect);
			GetClientRect(hwnd, &clientRect);

			int smallMargin = (windowRect.right - windowRect.left - clientRect.right) / 2;
			int largeMargin = windowRect.bottom - windowRect.top - clientRect.bottom - smallMargin;

			SetCursorPos(windowRect.left + smallMargin + Engine_clientWidth / 2, windowRect.top + largeMargin + Engine_clientHeight / 2);

		}

		//handle cursor visibility
		if(focused
		&& Engine_fpsModeOn){
			while(ShowCursor(false) >= 0){}
		}else{
			while(ShowCursor(true) <= 0){}
		}

		//update
			
		while(accumilatedTime > 1000 / 60){

			Engine_update();

			accumilatedTime -= 1000 / 60;

			internal_resetKeys();
			internal_resetPointer();

			//printf("hello from here %f\n", accumilatedTime);
		
		}
		
		//draw
		
		Engine_draw();
		
		SwapBuffers(hdc);
		
		glFinish();
		
		//glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, screenPixels);

		Engine_elapsedFrames++;

		QueryPerformanceCounter(&liStop);

		deltaTime = (float)((liStop.QuadPart - liStart.QuadPart) * 1000000 / liFrequency.QuadPart) / 1000;

		accumilatedTime += deltaTime;
		
		//printf("%f\n", deltaTime);

	}

	Engine_finnish();
	
	return 0;
	
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(uMsg == WM_CLOSE
	|| uMsg == WM_DESTROY
	|| uMsg == WM_QUIT){
		PostQuitMessage(0);
		internal_programShouldQuit = true;
		return 0;
	}

	if(uMsg == WM_CHAR){
		char character = (char)wParam;
		Engine_textInput.push_back(character);
		//printf("%c\n", character);
	}

	if(uMsg == WM_KEYDOWN){
		
		for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
			if(wParam == Engine_keys[i].OSIdentifier){
				if(!Engine_keys[i].down){
					Engine_keys[i].downed = true;
				}
				Engine_keys[i].down = true;
			}
		}

	}

	if(uMsg == WM_KEYUP){
		
		for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
			if(wParam == Engine_keys[i].OSIdentifier){
				if(Engine_keys[i].down){
					Engine_keys[i].upped = true;
				}
				Engine_keys[i].down = false;
			}
		}

		
	}

	if(uMsg == WM_LBUTTONDOWN){
		Engine_pointer.down = true;
		Engine_pointer.downed = true;
		Engine_pointer.lastDownedPos = Engine_pointer.pos;
	}

	if(uMsg == WM_LBUTTONUP){
		Engine_pointer.down = false;
		Engine_pointer.upped = true;
		Engine_pointer.lastUppedPos = Engine_pointer.pos;
	}

	if(uMsg == WM_MOUSEMOVE){

		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		Engine_pointer.pos.x = x;
		Engine_pointer.pos.y = y;

		Engine_pointer.movement.x = x - Engine_clientWidth / 2;
		Engine_pointer.movement.y = y - Engine_clientHeight / 2;

	}

	if(uMsg == WM_MOUSEWHEEL){

		short delta = GET_WHEEL_DELTA_WPARAM(wParam);

		Engine_pointer.scroll = delta / 120;

	}

	if(uMsg == WM_SIZE){
		Engine_clientWidth = LOWORD(lParam);
		Engine_clientHeight = HIWORD(lParam);
	}

    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

void Engine_setWindowTitle(char *title){
	
	SetWindowTextA(hwnd, (LPCSTR)title);
	
}

void Engine_setWindowSize(int width, int height){

	Engine_clientWidth = width;
	Engine_clientHeight = height;

	RECT rect;
	rect.left = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	rect.top = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

}

void Engine_toggleFullscreen(){

	if(!Engine_isFullscreen){
		SetWindowLongPtrA(hwnd, -16, WS_VISIBLE);

		SetWindowPos(hwnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);

		Engine_isFullscreen = true;
	}else{
		SetWindowLongPtrA(hwnd, -16, WS_OVERLAPPEDWINDOW);
		Engine_setWindowSize(480 * 2, 270 * 2);

		Engine_isFullscreen = false;
	}

}

void Engine_centerWindow(){
	//do nothing
}

#endif
