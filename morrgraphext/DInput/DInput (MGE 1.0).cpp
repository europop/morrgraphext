#include "input.h"
#include <tchar.h>
//TODO: Get the mouse sorted

FakeFunc FakeFuncs[GRAPHICSFUNCS];      //See GraphicsFuncs enum
bool SkipIntro=true;        //Non zero to skip intos
bool DisableConsole=false;  //Switch to true to disable the console
bool GlobalHammer=true;     //Used to hammer keys
sFakeKey FakeKeys[MAXMACROS];     //Last 8 reserved for mouse
sFakeTrigger Triggers[MAXTRIGGERS];   //Up to 4 time lagged triggers
bool DeviceWrapped=false;   //true if the graphics device got wrapped by the fake d3d dll

/* Only used by FakeDevice */
BYTE LastBytes[MAXMACROS];    //Stores which keys were pressed last GetData call
BYTE FakeStates[MAXMACROS];   //Stores which keys are currently permenently down
BYTE HammerStates[MAXMACROS]; //Stores which keys are currently being hammered
BYTE AHammerStates[MAXMACROS];//Stores the keys that are currently being ahammered
DIDEVICEOBJECTDATA FakeBuffer[256]; //Stores the list of fake keypresses to send to console
DWORD UseFakeBuffer=0;      //The number of fake characters left to write to the fake buffer
DWORD TriggerFireTimes[MAXTRIGGERS];
bool FinishedFake=false;    //true to shut down the console
bool CloseConsole=false;    //true to shut the console after performing a command
bool WrapDevice=false;      //true to pass morrowind a fake dinput device
BYTE MouseIn[10];      //Used to transfer keypresses to the mouse
BYTE MouseOut[10];     //Used to transfer keypresses back from the mouse

class FakeDevice : public IDirectInputDevice8 {
public:
    LPDIRECTINPUTDEVICE8W RealDevice;
    DWORD DeviceType;
    /*** Constructor and misc functions ***/
    FakeDevice() {
        RealDevice=NULL;
        DeviceType=DeviceType_UNSET;
    }
    void _stdcall FakeKeyPress(BYTE index,BYTE key,BYTE data) {
        FakeBuffer[index].dwOfs=key;
        FakeBuffer[index].dwData=data;
    }
    void _stdcall FakeString(BYTE chars[],BYTE data[],BYTE length) {
        if(DisableConsole) return;
        UseFakeBuffer=length;
        BYTE count=0;
        for(int i=length-1;i>=0;i--) {
            FakeKeyPress((count++),chars[i],data[i]);
        }

    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (THIS_ REFIID riid, LPVOID * ppvObj) {
        return RealDevice->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef(THIS) {
        return RealDevice->AddRef();
    }
    ULONG _stdcall Release(THIS) {
        return RealDevice->Release();
    }

    /*** IDirectInputDevice8W methods ***/
    HRESULT _stdcall GetCapabilities(THIS_ LPDIDEVCAPS a) {
        return RealDevice->GetCapabilities(a);
    }
    HRESULT _stdcall EnumObjects(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKW a,LPVOID b,DWORD c) {
        return RealDevice->EnumObjects(a,b,c);
    }
    HRESULT _stdcall GetProperty(THIS_ REFGUID a,LPDIPROPHEADER b) {
        return RealDevice->GetProperty(a,b);
    }
    HRESULT _stdcall SetProperty(THIS_ REFGUID a,LPCDIPROPHEADER b) {
        return RealDevice->SetProperty(a,b);
    }
    HRESULT _stdcall Acquire(THIS) {
        return RealDevice->Acquire();
    }
    HRESULT _stdcall Unacquire(THIS) {
        return RealDevice->Unacquire();
    }
    HRESULT _stdcall GetDeviceState(THIS_ DWORD a,LPVOID b) {
        if(DeviceType==DeviceType_KEYBOARD) {
            //This is a keyboard, so get a list of bytes (Dont forget the mouse too)
            BYTE bytes[MAXMACROS];
            HRESULT hr=RealDevice->GetDeviceState(256,bytes);
            CopyMemory(&bytes[256],&MouseOut,10);
            if(hr!=DI_OK) return hr;
            //Get any extra key presses
            for(DWORD byte=0;byte<MAXMACROS;byte++) {
                bytes[byte]|=FakeStates[byte];
            }
            if(GlobalHammer=!GlobalHammer) {
                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                    bytes[byte]|=HammerStates[byte];
                }
            } else {
                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                    bytes[byte]|=AHammerStates[byte];
                }
            }
            if(SkipIntro) {
                //push escape to skip the intro
                if(GlobalHammer) bytes[0x01]=0x80;
            } else if(FinishedFake) {
                //Close the console after faking a command (If using console 1 style)
                FinishedFake=false;
                bytes[0x29]=0x80;
            } else {
                //Process triggers
                DWORD time=::GetTickCount();
                for(DWORD trigger=0;trigger<MAXTRIGGERS;trigger++) {
                    if(Triggers[trigger].Active&&Triggers[trigger].TimeInterval>0&&TriggerFireTimes[trigger]<time) {
                        for(int i=0;i<MAXMACROS;i++) {
                            bytes[i]|=Triggers[trigger].Data.KeyStates[i];
                        }
                        TriggerFireTimes[trigger]=time+Triggers[trigger].TimeInterval;
                    }
                }
                //Process each key for keypresses
                for(DWORD key=0;key<MAXMACROS;key++) {
                    if(KEYDOWN(bytes,key)&&FakeKeys[key].type!=FKT_Unused) {
                        switch(FakeKeys[key].type) {
                            case FKT_Console1:
                                if(!KEYDOWN(LastBytes,key)) {
                                    bytes[0x29]=0x80;
                                    FakeString(FakeKeys[key].Console.KeyCodes,FakeKeys[key].Console.KeyStates,FakeKeys[key].Console.Length);
                                    CloseConsole=true;
                                }
                                break;
                            case FKT_Console2:
                                if(!KEYDOWN(LastBytes,key)) {
                                    FakeString(FakeKeys[key].Console.KeyCodes,FakeKeys[key].Console.KeyStates,FakeKeys[key].Console.Length);
                                    CloseConsole=false;
                                }
                                break;
                            case FKT_Hammer1:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte]&&GlobalHammer)
                                        bytes[byte]=0x80;
                                }
                                break;
                            case FKT_Hammer2:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte])
                                        HammerStates[byte]=0x80;
                                }
                                break;
                            case FKT_Unhammer:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte])
                                        HammerStates[byte]=0x00;
                                }
                                break;
                            case FKT_AHammer1:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte]&&!GlobalHammer)
                                        bytes[byte]=0x80;
                                }
                                break;
                            case FKT_AHammer2:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte])
                                        AHammerStates[byte]=0x80;
                                }
                                break;
                            case FKT_AUnhammer:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte])
                                        AHammerStates[byte]=0x00;
                                }
                                break;
                            case FKT_Press1:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte])
                                        bytes[byte]=0x80;
                                }
                                break;
                            case FKT_Press2:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte])
                                        FakeStates[byte]=0x80;
                                }
                                break;
                            case FKT_Unpress:
                                for(DWORD byte=0;byte<MAXMACROS;byte++) {
                                    if(FakeKeys[key].Press.KeyStates[byte])
                                        FakeStates[byte]=0x00;
                                }
                                break;
                            case FKT_BeginTimer:
                                if(!KEYDOWN(LastBytes,key))
                                    Triggers[FakeKeys[key].Timer.TimerID].Active=true;
                                break;
                            case FKT_EndTimer:
                                if(!KEYDOWN(LastBytes,key))
                                    Triggers[FakeKeys[key].Timer.TimerID].Active=false;

                                break;
                            case FKT_Graphics:
                                //Want increase/decrease zoom to work at all times
                                if((!KEYDOWN(LastBytes,key))||(FakeKeys[key].Graphics.function==GF_IncreaseZoom||
                                    FakeKeys[key].Graphics.function==GF_DecreaseZoom)) {
                                    if(DeviceWrapped)
                                       (FakeFuncs[FakeKeys[key].Graphics.function])();
                                }
                                break;
                        }
                    }
                }
            }
            ::CopyMemory(b,bytes,a);
            ::CopyMemory(LastBytes,bytes,MAXMACROS);
            ::CopyMemory(MouseIn,&bytes[256],10);
            if(DisableConsole) {
                ((BYTE*)b)[0x29]=0;
            }
            return DI_OK;
        } else {
            //This is a mouse
            DIMOUSESTATE2* MouseState=(DIMOUSESTATE2*)b;
            HRESULT hr=RealDevice->GetDeviceState(sizeof(DIMOUSESTATE2),MouseState);
            if(hr!=DI_OK) return hr;
            if(MouseState->lZ>0) {
                MouseOut[8]=0x80;
                MouseOut[9]=0;
            } else if(MouseState->lZ<0) {
                MouseOut[8]=0;
                MouseOut[9]=0x80;
            } else {
                MouseOut[8]=0;
                MouseOut[9]=0;
            }
            for(DWORD i=0;i<8;i++) {
                if(MouseState->rgbButtons[i]!=0) {
                    int zz=1;
                }
                MouseOut[i]=MouseState->rgbButtons[i];
                MouseState->rgbButtons[i]|=MouseIn[i];
            }
            return DI_OK;
        }
    }
    HRESULT _stdcall GetDeviceData(THIS_ DWORD a,LPDIDEVICEOBJECTDATA b,LPDWORD c,DWORD d) {
        //This only gets called for keyboards
        if(*c==1&&SkipIntro) {
            //Skip the second intro (Beats me why it wants buffered data)
            SkipIntro=false;
            FakeBuffer[0].dwOfs=0x01;
            FakeBuffer[0].dwData=0x80;
            *b=FakeBuffer[0];
            return DI_OK;
        } else if(*c==1&&UseFakeBuffer>0) {
            //Inject a fake keypress
            *b=FakeBuffer[--UseFakeBuffer];
            if(UseFakeBuffer==0&&CloseConsole) FinishedFake=true;
            return DI_OK;
        } else {
            //Read a real keypress
            return RealDevice->GetDeviceData(a,b,c,d);
        }
    }
    HRESULT _stdcall SetDataFormat(THIS_ LPCDIDATAFORMAT a) {
        return RealDevice->SetDataFormat(a);
    }
    HRESULT _stdcall SetEventNotification(THIS_ HANDLE a) {
        return RealDevice->SetEventNotification(a);
    }
    HRESULT _stdcall SetCooperativeLevel(THIS_ HWND a,DWORD b) {
        return RealDevice->SetCooperativeLevel(a,b);
    }
    HRESULT _stdcall GetObjectInfo(THIS_ LPDIDEVICEOBJECTINSTANCEW a,DWORD b,DWORD c) {
        return RealDevice->GetObjectInfo(a,b,c);
    }
    HRESULT _stdcall GetDeviceInfo(THIS_ LPDIDEVICEINSTANCEW a) {
        return RealDevice->GetDeviceInfo(a);
    }
    HRESULT _stdcall RunControlPanel(THIS_ HWND a,DWORD b) {
        return RealDevice->RunControlPanel(a,b);
    }
    HRESULT _stdcall Initialize(THIS_ HINSTANCE a,DWORD b,REFGUID c) {
        return RealDevice->Initialize(a,b,c);
    }
    HRESULT _stdcall CreateEffect(THIS_ REFGUID a,LPCDIEFFECT b,LPDIRECTINPUTEFFECT *c,LPUNKNOWN d) {
        return RealDevice->CreateEffect(a,b,c,d);
    }
    HRESULT _stdcall EnumEffects(THIS_ LPDIENUMEFFECTSCALLBACKW a,LPVOID b,DWORD c) {
        return RealDevice->EnumEffects(a,b,c);
    }
    HRESULT _stdcall GetEffectInfo(THIS_ LPDIEFFECTINFOW a,REFGUID b) {
        return RealDevice->GetEffectInfo(a,b);
    }
    HRESULT _stdcall GetForceFeedbackState(THIS_ LPDWORD a) {
        return RealDevice->GetForceFeedbackState(a);
    }
    HRESULT _stdcall SendForceFeedbackCommand(THIS_ DWORD a) {
        return RealDevice->SendForceFeedbackCommand(a);
    }
    HRESULT _stdcall EnumCreatedEffectObjects(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK a,LPVOID b,DWORD c) {
        return RealDevice->EnumCreatedEffectObjects(a,b,c);
    }
    HRESULT _stdcall Escape(THIS_ LPDIEFFESCAPE a) {
        return RealDevice->Escape(a);
    }
    HRESULT _stdcall Poll(THIS) {
        return RealDevice->Poll();
    }
    HRESULT _stdcall SendDeviceData(THIS_ DWORD a,LPCDIDEVICEOBJECTDATA b,LPDWORD c,DWORD d) {
        return RealDevice->SendDeviceData(a,b,c,d);
    }
    HRESULT _stdcall EnumEffectsInFile(THIS_ LPCWSTR a,LPDIENUMEFFECTSINFILECALLBACK b,LPVOID c,DWORD d) {
        return RealDevice->EnumEffectsInFile(a,b,c,d);
    }
    HRESULT _stdcall WriteEffectToFile(THIS_ LPCWSTR a,DWORD b,LPDIFILEEFFECT c,DWORD d) {
        return RealDevice->WriteEffectToFile(a,b,c,d);
    }
    HRESULT _stdcall BuildActionMap(THIS_ LPDIACTIONFORMATW a,LPCWSTR b,DWORD c) {
        return RealDevice->BuildActionMap(a,b,c);
    }
    HRESULT _stdcall SetActionMap(THIS_ LPDIACTIONFORMATW a,LPCWSTR b,DWORD c) {
        return RealDevice->SetActionMap(a,b,c);
    }
    HRESULT _stdcall GetImageInfo(THIS_ LPDIDEVICEIMAGEINFOHEADERW a) {
        return RealDevice->GetImageInfo(a);
    }
};

class FakeDirectInput : public IDirectInput8 {
public:
    LPDIRECTINPUT8W RealInput;
    /*** Constructor ***/
    FakeDirectInput() {
        RealInput=NULL;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (THIS_ REFIID riid, LPVOID * ppvObj)
    {
        return RealInput->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef(THIS)
    {
        return RealInput->AddRef();
    }
    ULONG _stdcall Release(THIS)
    {
        return RealInput->Release();
    }

    /*** IDirectInput8W methods ***/
    HRESULT _stdcall CreateDevice(THIS_ REFGUID r,LPDIRECTINPUTDEVICE8W *device,LPUNKNOWN unused)
    {
        if(r!=GUID_SysKeyboard&&r!=GUID_SysMouse) {
            return RealInput->CreateDevice(r,device,unused);
        } else {
            FakeDevice *fd=new FakeDevice();
            if(r==GUID_SysKeyboard) {
                fd->DeviceType=DeviceType_KEYBOARD;
            } else {
                fd->DeviceType=DeviceType_MOUSE;
            }
            HRESULT hr=RealInput->CreateDevice(r,&(fd->RealDevice),unused);
            if(hr!=DI_OK) return hr;
            *device=fd;
            return DI_OK;
        }
    }
    HRESULT _stdcall EnumDevices(THIS_ DWORD a,LPDIENUMDEVICESCALLBACKW b,LPVOID c,DWORD d)
    {
        return RealInput->EnumDevices(a,b,c,d);
    }
    HRESULT _stdcall GetDeviceStatus(THIS_ REFGUID r)
    {
        return RealInput->GetDeviceStatus(r);
    }
    HRESULT _stdcall RunControlPanel(THIS_ HWND a,DWORD b)
    {
        return RealInput->RunControlPanel(a,b);
    }
    HRESULT _stdcall Initialize(THIS_ HINSTANCE a,DWORD b)
    {
        return RealInput->Initialize(a,b);
    }
    HRESULT _stdcall FindDevice(THIS_ REFGUID a,LPCWSTR b,LPGUID c)
    {
        return RealInput->FindDevice(a,b,c);
    }
    HRESULT _stdcall EnumDevicesBySemantics(THIS_ LPCWSTR a,LPDIACTIONFORMATW b,LPDIENUMDEVICESBYSEMANTICSCBW c,LPVOID d,DWORD e)
    {
        return RealInput->EnumDevicesBySemantics(a,b,c,d,e);
    }
    HRESULT _stdcall ConfigureDevices(THIS_ LPDICONFIGUREDEVICESCALLBACK a,LPDICONFIGUREDEVICESPARAMSW b,DWORD c,LPVOID d)
    {
        return RealInput->ConfigureDevices(a,b,c,d);
    }
};

HRESULT _stdcall FakeDirectInputCreate(HINSTANCE a,DWORD b,REFIID c,LPVOID d,LPUNKNOWN e) {
    //Some local handles (Never need to bother freeing them, so they can be local)
    HMODULE dinput8dll = NULL;  //Handle to the real dinput dll
    HMODULE FakeD3Ddll = NULL;  //Handle to the fake D3D dll
    MYPROC func=NULL;           //Handle to the real create dinput8 function
    //Load the real dinput dll
    TCHAR Path[MAX_PATH];
    GetSystemDirectory((LPWSTR)&Path,MAX_PATH);
    _tcscat_s(Path,TEXT("\\dinput8.dll"));
    dinput8dll=::LoadLibrary(Path);
    func=(MYPROC)GetProcAddress(dinput8dll,"DirectInput8Create");
    if(func==NULL) {
        ::OutputDebugString(TEXT("Unable to load real direct input dll"));
        return NULL;
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
        (NameBuffer[LastSlash+1]!='m'&&NameBuffer[LastSlash+1]!='M')    ||
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
            //Process isn't morrowind, so return
            ::OutputDebugString(TEXT("Proccess was not morrowind"));
            ::OutputDebugString(NameBuffer);
            return (func)(a,b,c,d,e);
    }
    //Zero some memory that needs zeroing
    ::ZeroMemory(&LastBytes,sizeof(LastBytes));
    ::ZeroMemory(&FakeStates,sizeof(FakeStates));
    ::ZeroMemory(&FakeBuffer,sizeof(FakeBuffer));
    ::ZeroMemory(&FakeKeys,sizeof(FakeKeys));
    ::ZeroMemory(&Triggers,sizeof(Triggers));
    //Load the fake d3d dll and create function pointers
    FakeD3Ddll=::LoadLibrary(TEXT("d3d8.dll"));
    if(FakeD3Ddll!=NULL) {
        FakeFuncRet TestFunc=(FakeFuncRet)GetProcAddress(FakeD3Ddll,"FakeD3D_DeviceWrapped");
        if(TestFunc!=NULL&&(TestFunc)()==MGE_VERSION) {
            FakeFuncs[GF_Screenshot]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_TakeScreenshot");
            FakeFuncs[GF_Fog]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleFog");
            FakeFuncs[GF_Dither]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleDither");
            FakeFuncs[GF_ScaleFilter]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_CycleScaleFilter");
            FakeFuncs[GF_MipFilter]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_CycleMipFilter");
            FakeFuncs[GF_FogMode]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_CycleFogMode");
            FakeFuncs[GF_FillMode]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_CycleFillMode");
            FakeFuncs[GF_Shading]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleShading");
            FakeFuncs[GF_AA]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleAntialias");
            FakeFuncs[GF_CycleAniso]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_CycleAnisoLevel");
            FakeFuncs[GF_ToggleZoom]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleZoom");
            FakeFuncs[GF_IncreaseZoom]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_IncreaseZoom");
            FakeFuncs[GF_DecreaseZoom]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_DecreaseZoom");
            FakeFuncs[GF_RotateScreen]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_RotateScreen");
            FakeFuncs[GF_ToggleText]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleStatusText");
            FakeFuncs[GF_ShowLastText]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ShowLastMessage");
            FakeFuncs[GF_HWShader]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleHardwareShader");
            FakeFuncs[GF_SWShader]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleSoftwareShader");
            FakeFuncs[GF_Specular]=(FakeFunc)GetProcAddress(FakeD3Ddll,"FakeD3D_ToggleSpecular");
            DeviceWrapped=true;
        }
    }
    //Call the real direct input dll
    HRESULT hr=(func)(a,b,c,d,e);
    if(hr!=DI_OK) return hr;
    //Now load all of the fake keys
    DWORD unused;
    HANDLE keyfile=(HANDLE)::CreateFile(TEXT("DInput.data"),GENERIC_READ,0,0,OPEN_EXISTING,0,0);
    if(keyfile!=INVALID_HANDLE_VALUE) {
        BYTE version;
        ReadFile(keyfile,&version,1,&unused,NULL);
        if(version==MGE_VERSION) {
            ReadFile(keyfile,&SkipIntro,1,&unused,NULL);
            ReadFile(keyfile,&DisableConsole,1,&unused,NULL);
            ReadFile(keyfile,&WrapDevice,1,&unused,NULL);
            ReadFile(keyfile,&FakeKeys,sizeof(FakeKeys),&unused,NULL);
            ReadFile(keyfile,&Triggers,sizeof(Triggers),&unused,NULL);
        } else {
            OutputDebugString(TEXT("dinput8.data appears to be out of date"));
        }
        CloseHandle(keyfile);
    }
#ifndef RELEASE
    //FakeKeys[0x2c].type=FKT_Graphics;
    //FakeKeys[0x2c].Graphics.function=GF_Effects;
#endif
    //Then create a fake device and return
    if(WrapDevice) {
        //Set initial trigger times
        for(int i=0;i<MAXTRIGGERS;i++) {
            TriggerFireTimes[i]=::GetTickCount()+Triggers[i].TimeInterval;
        }
        FakeDirectInput *fdi=new FakeDirectInput();
        fdi->RealInput=(IDirectInput8*)*((IDirectInput8**)d);
        *((IDirectInput8**)d)=fdi;
    }
    return DI_OK;
}