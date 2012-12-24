#pragma once

#include "joint.h"
#include <dinput.h>
#include "MME function defs.h"

#define KEYDOWN(name, key) (name[key] & 0x80)

#ifdef _DInputWrapper
#define EX
#else
#define EX extern
#endif

#define DeviceType_KEYBOARD 1
#define DeviceType_MOUSE    2

#define MAXMACROS 266
#define MAXTRIGGERS 4

#define State_NONE   0  //This is used when the mouse button is unpressed
#define State_SLASH  1
#define State_PIERCE 2
#define State_HACK   3 
#define State_NONE2  4  //This is used to stop movement when mouse button is pressed, but no axis movement

#define SENSITIVITY 18  //how many pixels the mouse must move to register an attack
#define MAXGAP 15       //If the difference between x and y movement is greater than this then dont use a hacking attack

enum FakeKeyType : DWORD {
    FKT_Unused=0,       //This key isn't assigned to anything
    //FD_Console
    FKT_Console1=1,     //Enter console command, exit console
    FKT_Console2=2,     //Dont open or close console
    //FD_Press
    FKT_Hammer1=3,      //Hammer a key while pressed
    FKT_Hammer2=4,      //Start hammering a key
    FKT_Unhammer=5,     //Stop hammering a key
    FKT_AHammer1=6,     //Does the same as hammer, but hits it in alternate frames
    FKT_AHammer2=7,
    FKT_AUnhammer=8,
    FKT_Press1=9,       //Other keys pressed while this is pressed
    FKT_Press2=10,      //Press keys when this is pressed, but dont depress
    FKT_Unpress=11,     //Depress any pressed keys
    //FD_Timer
    FKT_BeginTimer=12,  //Start a timer trigger
    FKT_EndTimer=13,    //End a timer trigger
    //FD_Graphics
    FKT_Graphics=14     //Perform a graphics function
};

enum GraphicsFuncs : DWORD {
    GF_Screenshot=0,    //Take a screenshot
    GF_Fog=1,           //Toggle fogging
    GF_ScaleFilter=2,   //Cycle the scale filter
    GF_MipFilter=3,     //Cycle the mip filter
    GF_FogMode=4,       //Cycle the fog mode
    GF_AA=5,            //Toggle antialiasing
    GF_CycleAniso=6,    //Cycles anisotropic filtering level
    GF_ToggleZoom=7,   //Toggles zoom on and off
    GF_IncreaseZoom=8, //Increases zoom
    GF_DecreaseZoom=9, //Deceases zoom
    GF_RotateScreen=10, //Rotates the screen
    GF_ToggleText=11,   //toggles status text and fps counter on/off
    GF_ShowLastText=12, //Shows the last bit of text again
    GF_HWShader=13,     //Toggles hardware shaders
    GF_ToggleFps=14,    //Toggle the fps counter
    GF_IncreaseFog=15,  //Increase fogging
    GF_DecreaseFog=16,  //Decrease fogging
    MF_ToggleAltCombat=17,//toggles alternate combat controls
    MF_DelQuickSave=18,   //deletes the quicksave file
    MF_DelAutoSave=19,    //deletes the autosave file
    GF_ToggleAAFix=20,  //Toggle the AA colour fix
    GF_IncreaseView=21, //increase the view distance
    GF_DecreaseView=22, //Decrease the view distance
    GF_IncreaseAI=23,   //Increase the ai distance
    GF_DecreaseAI=24,   //Decrease the ai distance
    GF_IncreaseLOD=25,  //Decreases (yes, I know. lower bias increase LOD) the mipmap bias
    GF_DecreaseLOD=26,  //Increases the mipmap bias
    GF_ToggleCrosshair=27,  //Toggles the crosshair
    GF_NextTrack=28,        //Starts the next music track playing
    GF_DisableMusic=29,     //Sets the music really quiet
    GF_HaggleMore1=30,      //Increases barter amout by 1
    GF_HaggleMore100=31,    //Increases barter amout by 100
    GF_HaggleMore10000=32,  //Increases barter amout by 10000
    GF_HaggleLess1=33,      //Decreases barter amout by 1
    GF_HaggleLess100=34,    //Decreases barter amout by 100
    GF_HaggleLess10000=35,  //Decreases barter amout by 10000 
	GF_ToggleDL=36,
    GF_ToggleDS=37,	//Toggle distant statics
    GF_ToggleGrass=38,	//Toggle grass
    GF_ToggleMwMgeBlending=39,//Toggle MW/MGE blending
    GF_CycleWater=40,//Toggle MW/MGE blending
    GF_IncreaseFOV=41,
    GF_DecreaseFOV=42,
    GF_HaggleMore10=43,    //Increases barter amout by 10
    GF_HaggleMore1000=44,    //Increases barter amout by 1000
    GF_HaggleLess10=45,    //Decreases barter amout by 10
    GF_HaggleLess1000=46,    //Decreases barter amout by 1000
    GF_ToggleSkyReflection=47,    //toggle sky reflection in water
	GF_MoveForward3PC = 48,
	GF_MoveBack3PC = 49,
	GF_MoveLeft3PC = 50,
	GF_MoveRight3PC = 51,
	GF_MoveDown3PC = 52,
	GF_MoveUp3PC = 53,
	GRAPHICSFUNCS       //Handy way of getting the number of elements in an enum
};

struct FD_Console {
    BYTE Length;               //The size of the string
    BYTE KeyCodes[MAXMACROS];  //A list of keycodes (This can be a byte because there's never more than 256)
    BYTE KeyStates[MAXMACROS]; //A list of keystates
};
struct FD_Press {
    BYTE KeyStates[MAXMACROS];    //Last 10 for mouse
};
struct FD_Timer {
    BYTE TimerID;           //The timer to activate/deactivate
};
struct FD_Graphics {
    BYTE function; //The GraphicsFuncs to call in the fake d3d dll
};

struct sFakeKey {
    BYTE type;
    union {
        FD_Console Console;
        FD_Press Press;
        FD_Timer Timer;
        FD_Graphics Graphics;
    };
};

struct sFakeTrigger {
    DWORD TimeInterval;
    BYTE Active;
    FD_Press Data;
    BYTE Unused;        //Need this extra byte to avoid packing issues
};


typedef HRESULT (_stdcall *DInputProc)(HINSTANCE,DWORD,REFIID,LPVOID,LPUNKNOWN); 
typedef void (*FakeFunc)();

EX FakeFunc FakeFuncs[GRAPHICSFUNCS];      //See GraphicsFuncs enum
EX bool SkipIntro;        //Non zero to skip intros
EX bool DisableConsole;  //Switch to true to disable the console
EX bool GlobalHammer;     //Used to hammer keys
EX sFakeKey FakeKeys[MAXMACROS];     //Last 8 reserved for mouse
EX sFakeTrigger Triggers[MAXTRIGGERS];   //Up to 4 time lagged triggers
EX BYTE RemappedKeys[256];
EX bool UseAltCombatWrapper;//true to use the alternate combat wrapper

//Fake input device variables
EX BYTE LastBytes[MAXMACROS];     //Stores which keys were pressed last GetData call
EX BYTE FakeStates[MAXMACROS];    //Stores which keys are currently permenently down
EX BYTE HammerStates[MAXMACROS];  //Stores which keys are currently being hammered
EX BYTE AHammerStates[MAXMACROS]; //Stores the keys that are currently being ahammered
EX BYTE TapStates[MAXMACROS];     //Stores the keys that need to be tapped next frame
EX BYTE DisallowStates[MAXMACROS];//Stores which keys are disallowed
EX DIDEVICEOBJECTDATA FakeBuffer[256]; //Stores the list of fake keypresses to send to console
EX DWORD FakeBufferStart;      //The index of the next character to write from FakeBuffer[]
EX DWORD FakeBufferEnd;        //The index of the last character contained in FakeBuffer[]
EX DWORD TriggerFireTimes[MAXTRIGGERS];
EX bool FinishedFake;    //true to shut down the console
EX bool CloseConsole;    //true to shut the console after performing a command
EX BYTE MouseIn[10];      //Used to transfer keypresses to the mouse
EX BYTE MouseOut[10];     //Used to transfer keypresses back from the mouse
//MWSE mouse control
EX WORD MouseXMov;
EX WORD MouseYMov;
EX float MouseXSpeed;
EX float MouseYSpeed;
EX float MouseXLeft;
EX float MouseYLeft;
EX bool MouseDisable;
//Used by the alternate combat wrapper
EX bool UseAltCombat;    //True when using the alternate combat mode
EX bool MadeAttack;      //True when the player makes an attack (Because the keyboard must be used before the mouse)
EX bool MadeAttackLast;  //Used in power attacks
EX DWORD AttackType;         //Used to store the state of the mouse between frames
EX DWORD LastAttack;         //You cant use the same attack twice in a row

//Misc functions
void FakeKeyPress(BYTE index,BYTE key,BYTE data);
void FakeString(BYTE chars[],BYTE data[],BYTE length);
