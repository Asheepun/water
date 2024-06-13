enum Engine_KeyIdentifiers{

    ENGINE_KEY_0,
    ENGINE_KEY_1,
    ENGINE_KEY_2,
    ENGINE_KEY_3,
    ENGINE_KEY_4,
    ENGINE_KEY_5,
    ENGINE_KEY_6,
    ENGINE_KEY_7,
    ENGINE_KEY_8,
    ENGINE_KEY_9,

    ENGINE_KEY_A,
    ENGINE_KEY_B,
    ENGINE_KEY_C,
    ENGINE_KEY_D,
    ENGINE_KEY_E,
    ENGINE_KEY_F,
    ENGINE_KEY_G,
    ENGINE_KEY_H,
    ENGINE_KEY_I,
    ENGINE_KEY_J,
    ENGINE_KEY_K,
    ENGINE_KEY_L,
    ENGINE_KEY_M,
    ENGINE_KEY_N,
    ENGINE_KEY_O,
    ENGINE_KEY_P,
    ENGINE_KEY_Q,
    ENGINE_KEY_R,
    ENGINE_KEY_S,
    ENGINE_KEY_T,
    ENGINE_KEY_U,
    ENGINE_KEY_V,
    ENGINE_KEY_W,
    ENGINE_KEY_X,
    ENGINE_KEY_Y,
    ENGINE_KEY_Z,

    ENGINE_KEY_UP,
    ENGINE_KEY_DOWN,
    ENGINE_KEY_LEFT,
    ENGINE_KEY_RIGHT,

    ENGINE_KEY_SPACE,
    ENGINE_KEY_ESCAPE,
    ENGINE_KEY_ENTER,

    ENGINE_KEY_SHIFT,
    ENGINE_KEY_CONTROL,

    ENGINE_KEYS_LENGTH,

};

enum Engine_ControllerButtonIdentifiers{

	ENGINE_CONTROLLER_BUTTON_Y,
	ENGINE_CONTROLLER_BUTTON_A,
	ENGINE_CONTROLLER_BUTTON_X,
	ENGINE_CONTROLLER_BUTTON_B,

	ENGINE_CONTROLLER_BUTTON_MIDDLE_LEFT,
	ENGINE_CONTROLLER_BUTTON_MIDDLE_RIGHT,

	ENGINE_CONTROLLER_BUTTON_LEFT_BUTTON,
	ENGINE_CONTROLLER_BUTTON_RIGHT_BUTTON,

	ENGINE_CONTROLLER_LINUX_BUTTONS_LENGTH,

	ENGINE_CONTROLLER_BUTTON_UP,
	ENGINE_CONTROLLER_BUTTON_DOWN,
	ENGINE_CONTROLLER_BUTTON_LEFT,
	ENGINE_CONTROLLER_BUTTON_RIGHT,

	ENGINE_CONTROLLER_BUTTON_LEFT_STICK_UP,
	ENGINE_CONTROLLER_BUTTON_LEFT_STICK_DOWN,
	ENGINE_CONTROLLER_BUTTON_LEFT_STICK_LEFT,
	ENGINE_CONTROLLER_BUTTON_LEFT_STICK_RIGHT,

	ENGINE_CONTROLLER_BUTTON_RIGHT_STICK_UP,
	ENGINE_CONTROLLER_BUTTON_RIGHT_STICK_DOWN,
	ENGINE_CONTROLLER_BUTTON_RIGHT_STICK_LEFT,
	ENGINE_CONTROLLER_BUTTON_RIGHT_STICK_RIGHT,

	ENGINE_CONTROLLER_BUTTON_LEFT_TRIGGER,
	ENGINE_CONTROLLER_BUTTON_RIGHT_TRIGGER,

	ENGINE_CONTROLLER_BUTTONS_LENGTH,

};

typedef struct Engine_Key{
    unsigned int OSIdentifier;
    char upperCaseString;
    char lowerCaseString;
    bool down;
    bool downed;
    bool upped;
}Engine_Key;

typedef struct Engine_Pointer{
    Vec2f pos;
    Vec2f movement;
    Vec2f lastDownedPos;
    Vec2f lastUppedPos;
    bool down;
    bool downed;
    bool upped;
    int scroll;
}Engine_Pointer;

typedef struct Engine_Controller{
	Engine_Key buttons[ENGINE_CONTROLLER_BUTTONS_LENGTH];
	Vec2f leftStick;
	Vec2f lastLeftStick;
	Vec2f rightStick;
	Vec2f lastRightStick;
	float leftTrigger;
	float lastLeftTrigger;
	float rightTrigger;
	float lastRightTrigger;
}Engine_Controller;

//GLOBAL VARIABLE DECLARTIONS
extern Engine_Key Engine_keys[ENGINE_KEYS_LENGTH];

extern Engine_Pointer Engine_pointer;

extern Engine_Controller Engine_controller;
extern bool Engine_controllerIsConnected;

//extern std::vector<char> Engine_textInput;
extern Array<char> Engine_textInput;

extern bool Engine_backButtonPressed;

extern int Engine_clientWidth;
extern int Engine_clientHeight;
extern bool Engine_isFullscreen;

extern bool Engine_windowSizeChanged;

extern int Engine_statusBarOffset;

extern int Engine_elapsedFrames;
extern float Engine_frameUpdateTime;
extern float Engine_frameDrawTime;
extern float Engine_frameTime;

extern float Engine_deltaTime;

extern bool Engine_fpsModeOn;

//extern String Engine_localPath;
extern String Engine_resourcePath;
extern String Engine_dataPath;

extern AssetManager *Engine_assetManager_p;

static const char *ENGINE_KEY_NAMES[] = {

	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",

	"UP",
	"DOWN",
	"LEFT",
	"RIGHT",

	"SPACE",
	"ESCAPE",
	"ENTER",

	"SHIFT",
	"CONTROL",

};

static const char *ENGINE_CONTROLLER_BUTTON_NAMES[] = {

	"Y",
	"A",
	"X",
	"B",

	"Middle Left",
	"Middle Right",

	"LB",
	"RB",

	"---",

	"Up",
	"Down",
	"Left",
	"Right",

	"LS Up",
	"LS Down",
	"LS Left",
	"LS Right",

	"RS Up",
	"RS Down",
	"RS Left",
	"RS Right",

	"LT",
	"RT",

};

//INTERNAL VARIABLE DECLARTAIONS
extern bool internal_programShouldQuit;

//FUNCTIONS DEFINED BY GAME
void Engine_start();

void Engine_update();

void Engine_initDrawing();

void Engine_freeDrawing();

void Engine_draw();

void Engine_finish();

void Engine_handleStop();

void Engine_pause();

void Engine_resume();

void Engine_saveState(void **, size_t *);

void Engine_handleSavedState(void *, size_t);
