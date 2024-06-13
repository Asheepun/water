#include "engine/engine.h"

bool ENGINE_PRINT_FRAME_TIME = true;

//COMMON GLOBAL VARIABLE DEFINITIONS
int Engine_clientWidth = 800;
int Engine_clientHeight = 450;
int Engine_lastClientWidth = 800;
int Engine_lastClientHeight = 450;
int Engine_statusBarOffset = 0;
bool Engine_isFullscreen = false;
bool Engine_windowSizeChanged = false;

float Engine_frameUpdateTime = 0.0;
float Engine_frameDrawTime = 0.0;
float Engine_frameTime = 0.0;

int Engine_elapsedFrames = 0;

bool Engine_fpsModeOn = false;

Engine_Key Engine_keys[ENGINE_KEYS_LENGTH];

Engine_Pointer Engine_pointer;

Engine_Controller Engine_controller;

Array<char> Engine_textInput;

bool internal_programShouldQuit = false;

//COMMON INTERNAL FUNCTIONS INITS
void internal_initKeys(){

	for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){

		Engine_keys[i].down = false;
		Engine_keys[i].downed = false;
		Engine_keys[i].upped = false;
	
	}

	Engine_controller.leftStick = getVec2f(0, 0);
	Engine_controller.rightStick = getVec2f(0, 0);
	Engine_controller.lastLeftStick = getVec2f(0, 0);
	Engine_controller.lastRightStick = getVec2f(0, 0);
	Engine_controller.leftTrigger = 0;
	Engine_controller.rightTrigger = 0;
	Engine_controller.lastLeftTrigger = 0;
	Engine_controller.lastRightTrigger = 0;

	for(int i = 0; i < ENGINE_CONTROLLER_BUTTONS_LENGTH; i++){
		Engine_controller.buttons[i].down = false;
		Engine_controller.buttons[i].downed = false;
		Engine_controller.buttons[i].upped = false;
	}
	
}

void internal_resetKeys(){

	for(int i = 0; i < ENGINE_KEYS_LENGTH; i++){
		Engine_keys[i].downed = false;
		Engine_keys[i].upped = false;
	}

	//Array_clear<char>(&Engine_textInput);
	Engine_textInput.clear();

	for(int i = 0; i < ENGINE_CONTROLLER_BUTTONS_LENGTH; i++){
		Engine_controller.buttons[i].downed = false;
		Engine_controller.buttons[i].upped = false;
	}

	Engine_controller.lastLeftStick = Engine_controller.leftStick;
	Engine_controller.lastRightStick = Engine_controller.rightStick;
	Engine_controller.lastLeftTrigger = Engine_controller.leftTrigger;
	Engine_controller.lastRightTrigger = Engine_controller.rightTrigger;

}

void internal_initPointer(){
	Engine_pointer.pos = getVec2f(0, 0);
	Engine_pointer.down = false;
	Engine_pointer.downed = false;
	Engine_pointer.upped = false;
	Engine_pointer.scroll = 0;
}

void internal_resetPointer(){
	Engine_pointer.downed = false;
	Engine_pointer.upped = false;
	Engine_pointer.scroll = 0;
}

void internal_checkWindowDimensionChange(){
	if(Engine_clientWidth != Engine_lastClientWidth){
		Engine_windowSizeChanged = true;
		Engine_lastClientWidth = Engine_clientWidth;
	}
	if(Engine_clientHeight != Engine_lastClientHeight){
		Engine_windowSizeChanged = true;
		Engine_lastClientHeight = Engine_clientHeight;
	}
}

//COMMON PUBLIC FUNCTIONS
void Engine_quit(){
	internal_programShouldQuit = true;
}
