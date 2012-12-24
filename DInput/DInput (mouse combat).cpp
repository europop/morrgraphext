// DInput.cpp
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// Linking this library statically or dynamically with other modules is
// making a combined work based on this library.  Thus, the terms and
// conditions of the GNU General Public License cover the whole
// combination.

/********************************************************************************************************
 * A fake Direct input dll that provides alternate combat controls.                                     *
 * Originally by Timeslip                                                                               *
 *                                                                                                      *
 * This works by passing morrowind a fake IDirectInputDevice8 interface for the keyboard and mouse      *
 * It never frees the created classes, so leaks memory. In general the classes only need to be freed    *
 * when morrowind shuts down, so this doesn't really matter.                                            *
 *                                                                                                      *
 * If you are using the provided solution file, ensure that you change the output directory and         *
 * debug locations before compiling.                                                                    *
 *                                                                                                      *
 * The only overloaded interface functions are IDirectInput->CreateDevice and                           *
 * IDirectInputDevice8->GetDeviceState. All other functions just call their real equivelent.            *
 ********************************************************************************************************/

#define STRICT      //Standard win32 defines
#define WIN32_LEAN_AND_MEAN

#define DIRECTINPUT_VERSION 0x0800  //Using direct input v8 interfaces

#define KEYDOWN(now, then, key) (now[key]&0x80)&&!(then[key]&0x80)   //Used to check for key presses

#define DeviceType_UNSET    0   //Used so that the FakeDevice class knows what it is
#define DeviceType_KEYBOARD 1
#define DeviceType_MOUSE    2

#define State_NONE   0  //This is used when the mouse button is unpressed
#define State_SLASH  1
#define State_PIERCE 2
#define State_HACK   3 
#define State_NONE2  4  //This is used to stop movement when mouse button is pressed, but no axis movement

#define SENSITIVITY 18  //how many pixels the mouse must move to register an attack
#define MAXGAP 15       //If the difference between x and y movement is greater than this then dont use a hacking attack
#define ALTKEY 0x45     //numlock

#include <dinput.h>     //This includes all the windows headers, so no need to include them again
#include <tchar.h>      //Used when loading the real direct input dll. Not really necessary

typedef HRESULT (*MYPROC)(HINSTANCE,DWORD,REFIID,LPVOID,LPUNKNOWN); //This is used to call the real CreateDirectInput8 function

bool Hammer=false;          //Used to alternate direction of movement each frame when attacking
bool UseAltCombat=false;    //True when using the alternate combat mode
bool MadeAttack=false;      //True when the player makes an attack (Because the keyboard must be used before the mouse)
bool MadeAttackLast=false;  //Used in power attacks
BYTE LastBytes[256];        //Used to store the keyboard state from the last frame
DWORD AttackType=0;         //Used to store the state of the mouse between frames
DWORD LastAttack=0;         //You cant use the same attack twice in a row

class FakeDevice : public IDirectInputDevice8 {
public:
    LPDIRECTINPUTDEVICE8 RealDevice;
    DWORD DeviceType;
    /*** Constructor ***/
    FakeDevice(LPDIRECTINPUTDEVICE8 device,DWORD DevType) {
        RealDevice=device;
        DeviceType=DevType;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (REFIID riid, LPVOID * ppvObj) {
        return RealDevice->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef(void) {
        return RealDevice->AddRef();
    }
    ULONG _stdcall Release(void) {
        return RealDevice->Release();
    }

    /*** IDirectInputDevice8W methods ***/
    HRESULT _stdcall GetCapabilities(LPDIDEVCAPS a) {
        return RealDevice->GetCapabilities(a);
    }
    HRESULT _stdcall EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW a,LPVOID b,DWORD c) {
        return RealDevice->EnumObjects(a,b,c);
    }
    HRESULT _stdcall GetProperty(REFGUID a,LPDIPROPHEADER b) {
        return RealDevice->GetProperty(a,b);
    }
    HRESULT _stdcall SetProperty(REFGUID a,LPCDIPROPHEADER b) {
        return RealDevice->SetProperty(a,b);
    }
    HRESULT _stdcall Acquire(void) {
        return RealDevice->Acquire();
    }
    HRESULT _stdcall Unacquire(void) {
        return RealDevice->Unacquire();
    }
    HRESULT _stdcall GetDeviceState(DWORD a,LPVOID b) {
        if(DeviceType==DeviceType_KEYBOARD) {
            //This is the keyboard
            BYTE bytes[256];    //Create an array of 256 bytes to store the keystates
            HRESULT hr=RealDevice->GetDeviceState(256,bytes);
            if(hr!=DI_OK) return hr;    //Error getting device, so return
            //We only want to modify keyboard input in alternate combat mode when the player has the mouse held down
            if(UseAltCombat&&AttackType) {  
                bytes[0x1e]=bytes[0x1f]=bytes[0x20]=bytes[0x11]=0;  //Set all movement keys to 0
                //Then set appropriate keys to 0x80 depending on what type of attack is being made
                if(Hammer=!Hammer) {
                    if(AttackType==State_SLASH) bytes[0x1e]=0x80;
                    if(AttackType==State_PIERCE) bytes[0x11]=0x80;
                } else {
                    if(AttackType==State_SLASH) bytes[0x20]=0x80;
                    if(AttackType==State_PIERCE) bytes[0x1f]=0x80;
                }
                if(!(AttackType&State_NONE2)) {
                    MadeAttack=true;    //Tell the mouse that an attack has been made, so to press the mouse button
                }
            }
            if(KEYDOWN(bytes,LastBytes,ALTKEY)) UseAltCombat=!UseAltCombat; //Change input mode when necessary
            ::CopyMemory(b,bytes,a);        //Copy the bytes array both to morrowind and to the LastBytes array
            ::CopyMemory(LastBytes,bytes,256);
            return DI_OK;
        } else if (UseAltCombat) {
            //This is a mouse, and we're using alternate combat
            DIMOUSESTATE2* MouseState=(DIMOUSESTATE2*)b;
            HRESULT hr=RealDevice->GetDeviceState(sizeof(DIMOUSESTATE2),MouseState);
            if(MouseState->rgbButtons[0]) {
                if(abs(MouseState->lX)>abs(MouseState->lY)+MAXGAP) MouseState->lY=0;
                if(abs(MouseState->lY)>abs(MouseState->lX)+MAXGAP) MouseState->lX=0;
                bool slash = (abs(MouseState->lX)>SENSITIVITY); //Check for mouse movement
                bool pierce = (abs(MouseState->lY)>SENSITIVITY);
                DWORD Attack=0x0000;   //Which direction has the mouse moved
                if(MouseState->lX>SENSITIVITY)  Attack|=0x0001;
                if(MouseState->lX<-SENSITIVITY) Attack|=0x0010;
                if(MouseState->lY>SENSITIVITY)  Attack|=0x0100;
                if(MouseState->lY<-SENSITIVITY) Attack|=0x1000;
                if(MadeAttackLast&&(Attack==LastAttack&&Attack!=0)) {
                    MadeAttack=true;
                }
                if(Attack==LastAttack||Attack==0) {
                    AttackType=State_NONE2; //Cant attack by moving the mouse in the same direction twice
                } else {
                    //Set attacktype appropriately depending on mouse movement
                    if(slash&&pierce) {
                        AttackType=State_HACK;
                    } else if(slash) {
                        AttackType=State_SLASH;
                    } else if(pierce) {
                        AttackType=State_PIERCE;
                    } else {
                        //This differentiates between not having the mouse button down, and having the mouse down but not moving it
                        AttackType=State_NONE2;
                    }
                    LastAttack=Attack;
                }
                MouseState->lX=0;   //Set mouse movement and left button to 0
                MouseState->lY=0;
                MouseState->rgbButtons[0]=0;
                if(MadeAttack) {
                    MouseState->rgbButtons[0]=0x80; //If an attack has been made then press the left mouse button
                    MadeAttack=false;
                    MadeAttackLast=true;
                } else { MadeAttackLast=false; }
            } else { // Mouse button isn't down
                AttackType=State_NONE;
                LastAttack=0;
            }
            return DI_OK;
        }
        return RealDevice->GetDeviceState(a,b);
    }
    HRESULT _stdcall GetDeviceData(DWORD a,LPDIDEVICEOBJECTDATA b,LPDWORD c,DWORD d) {
        //This only gets called for keyboards, and can be ignored because we don't want to change buffered data
        return RealDevice->GetDeviceData(a,b,c,d);
    }
    HRESULT _stdcall SetDataFormat(LPCDIDATAFORMAT a) {
        return RealDevice->SetDataFormat(a);
    }
    HRESULT _stdcall SetEventNotification(HANDLE a) {
        return RealDevice->SetEventNotification(a);
    }
    HRESULT _stdcall SetCooperativeLevel(HWND a,DWORD b) {
        return RealDevice->SetCooperativeLevel(a,b);
    }
    HRESULT _stdcall GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW a,DWORD b,DWORD c) {
        return RealDevice->GetObjectInfo(a,b,c);
    }
    HRESULT _stdcall GetDeviceInfo(LPDIDEVICEINSTANCEW a) {
        return RealDevice->GetDeviceInfo(a);
    }
    HRESULT _stdcall RunControlPanel(HWND a,DWORD b) {
        return RealDevice->RunControlPanel(a,b);
    }
    HRESULT _stdcall Initialize(HINSTANCE a,DWORD b,REFGUID c) {
        return RealDevice->Initialize(a,b,c);
    }
    HRESULT _stdcall CreateEffect(REFGUID a,LPCDIEFFECT b,LPDIRECTINPUTEFFECT *c,LPUNKNOWN d) {
        return RealDevice->CreateEffect(a,b,c,d);
    }
    HRESULT _stdcall EnumEffects(LPDIENUMEFFECTSCALLBACKW a,LPVOID b,DWORD c) {
        return RealDevice->EnumEffects(a,b,c);
    }
    HRESULT _stdcall GetEffectInfo(LPDIEFFECTINFOW a,REFGUID b) {
        return RealDevice->GetEffectInfo(a,b);
    }
    HRESULT _stdcall GetForceFeedbackState(LPDWORD a) {
        return RealDevice->GetForceFeedbackState(a);
    }
    HRESULT _stdcall SendForceFeedbackCommand(DWORD a) {
        return RealDevice->SendForceFeedbackCommand(a);
    }
    HRESULT _stdcall EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK a,LPVOID b,DWORD c) {
        return RealDevice->EnumCreatedEffectObjects(a,b,c);
    }
    HRESULT _stdcall Escape(LPDIEFFESCAPE a) {
        return RealDevice->Escape(a);
    }
    HRESULT _stdcall Poll(void) {
        return RealDevice->Poll();
    }
    HRESULT _stdcall SendDeviceData(DWORD a,LPCDIDEVICEOBJECTDATA b,LPDWORD c,DWORD d) {
        return RealDevice->SendDeviceData(a,b,c,d);
    }
    HRESULT _stdcall EnumEffectsInFile(LPCWSTR a,LPDIENUMEFFECTSINFILECALLBACK b,LPVOID c,DWORD d) {
        return RealDevice->EnumEffectsInFile(a,b,c,d);
    }
    HRESULT _stdcall WriteEffectToFile(LPCWSTR a,DWORD b,LPDIFILEEFFECT c,DWORD d) {
        return RealDevice->WriteEffectToFile(a,b,c,d);
    }
    HRESULT _stdcall BuildActionMap(LPDIACTIONFORMATW a,LPCWSTR b,DWORD c) {
        return RealDevice->BuildActionMap(a,b,c);
    }
    HRESULT _stdcall SetActionMap(LPDIACTIONFORMATW a,LPCWSTR b,DWORD c) {
        return RealDevice->SetActionMap(a,b,c);
    }
    HRESULT _stdcall GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW a) {
        return RealDevice->GetImageInfo(a);
    }
};

class FakeDirectInput : public IDirectInput8 {
public:
    LPDIRECTINPUT8 RealInput;
    /*** Constructor ***/
    FakeDirectInput(LPDIRECTINPUT8 real) {
        RealInput=real;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (REFIID riid, LPVOID * ppvObj)
    {
        return RealInput->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef(void)
    {
        return RealInput->AddRef();
    }
    ULONG _stdcall Release(void)
    {
        return RealInput->Release();
    }

    /*** IDirectInput8W methods ***/
    HRESULT _stdcall CreateDevice(REFGUID r,LPDIRECTINPUTDEVICE8W *device,LPUNKNOWN unused) {
        HRESULT hr;
        if(r==GUID_SysKeyboard||r==GUID_SysMouse) {
            hr=RealInput->CreateDevice(r,device,unused);    //Create a real device
            if(hr!=DI_OK) return hr;       //If there's an erro then return
            //Create a fake device and pass it to morrowind
            if(r==GUID_SysKeyboard) {
                *device=new FakeDevice(*device,DeviceType_KEYBOARD);
            } else {
                *device=new FakeDevice(*device,DeviceType_MOUSE);
            }
            return DI_OK;
        } else {
            //This must be a joystick, so ignore it
            return RealInput->CreateDevice(r,device,unused);
        }
    }
    HRESULT _stdcall EnumDevices(DWORD a,LPDIENUMDEVICESCALLBACKW b,LPVOID c,DWORD d)
    {
        return RealInput->EnumDevices(a,b,c,d);
    }
    HRESULT _stdcall GetDeviceStatus(REFGUID r)
    {
        return RealInput->GetDeviceStatus(r);
    }
    HRESULT _stdcall RunControlPanel(HWND a,DWORD b)
    {
        return RealInput->RunControlPanel(a,b);
    }
    HRESULT _stdcall Initialize(HINSTANCE a,DWORD b)
    {
        return RealInput->Initialize(a,b);
    }
    HRESULT _stdcall FindDevice(REFGUID a,LPCWSTR b,LPGUID c)
    {
        return RealInput->FindDevice(a,b,c);
    }
    HRESULT _stdcall EnumDevicesBySemantics(LPCWSTR a,LPDIACTIONFORMATW b,LPDIENUMDEVICESBYSEMANTICSCBW c,LPVOID d,DWORD e)
    {
        return RealInput->EnumDevicesBySemantics(a,b,c,d,e);
    }
    HRESULT _stdcall ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK a,LPDICONFIGUREDEVICESPARAMSW b,DWORD c,LPVOID d)
    {
        return RealInput->ConfigureDevices(a,b,c,d);
    }
};

HRESULT _stdcall FakeDirectInputCreate(HINSTANCE a,DWORD b,REFIID c,LPVOID d,LPUNKNOWN e) {
    //Some local handles (Never need to bother freeing them, so they can be local)
    HMODULE dinput8dll = NULL;  //Handle to the real dinput dll
    MYPROC func=NULL;           //Handle to the real create dinput8 function
    //Load the real dinput dll
    TCHAR Path[MAX_PATH];   //Path to the real DInput8.dll
    GetSystemDirectory((LPWSTR)&Path,MAX_PATH); //Could probably just use %system%
    _tcscat_s(Path,TEXT("\\dinput8.dll"));      //Add dinput8.dll to the end of the system path
    dinput8dll=::LoadLibrary(Path); //Load the real direct input dll
    func=(MYPROC)GetProcAddress(dinput8dll,"DirectInput8Create");   //Get the address of the real function
    if(func==NULL) {
        ::OutputDebugString(TEXT("Unable to load real direct input dll"));
        return DIERR_GENERIC;
    }
    //Check that the process is morrowind
    TCHAR NameBuffer[MAX_PATH];
    ::GetModuleFileName(NULL,(LPTSTR)&NameBuffer,MAX_PATH);
    int LastSlash;
    int End;
    for(int i=0;i<MAX_PATH;i++) {
        if(NameBuffer[i]=='\\') LastSlash=i;
        if(NameBuffer[i]==0) {
            End=i;
            break;
        }
    }
    if(End-LastSlash!=14                ||
        (NameBuffer[LastSlash+1]!='m'&&NameBuffer[LastSlash+1]!='M')    ||  //The first letter isn't case sensitive
        NameBuffer[LastSlash+2]!='o'    ||
        NameBuffer[LastSlash+3]!='r'    ||
        NameBuffer[LastSlash+4]!='r'    ||
        NameBuffer[LastSlash+5]!='o'    ||
        NameBuffer[LastSlash+6]!='w'    ||
        NameBuffer[LastSlash+7]!='i'    ||
        NameBuffer[LastSlash+8]!='n'    ||
        NameBuffer[LastSlash+9]!='d'    ||
        NameBuffer[LastSlash+10]!='.'   ||
        NameBuffer[LastSlash+11]!='e'   ||
        NameBuffer[LastSlash+12]!='x'   ||
        NameBuffer[LastSlash+13]!='e'
        ) {
            //Process isn't morrowind, so dont create a fake IDirectInput8 interface
            ::OutputDebugString(TEXT("Proccess was not morrowind"));
            ::OutputDebugString(NameBuffer);
            return (func)(a,b,c,d,e);
    }
    //Call the real direct input dll
    HRESULT hr=(func)(a,b,c,d,e);
    if(hr!=DI_OK) return hr;
    //Then create a fake interface and return
    *((IDirectInput8**)d)=new FakeDirectInput((IDirectInput8*)*((IDirectInput8**)d));
    return DI_OK;
}