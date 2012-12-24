#define _DInputWrapper
#include "DInputHeader.h"
#include <math.h>

class RemapWrapper : public IDirectInputDevice8 {
public:
    LPDIRECTINPUTDEVICE8 RealDevice;
    /*** Constructor ***/
    RemapWrapper(IDirectInputDevice8* device) {
        RealDevice=device;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (REFIID riid,void** ppvObj) {
        return RealDevice->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef() {
        return RealDevice->AddRef();
    }
    ULONG _stdcall Release() {
        return RealDevice->Release();
    }

    /*** IDirectInputDevice8W methods ***/
    HRESULT _stdcall GetCapabilities(LPDIDEVCAPS a) {
        return RealDevice->GetCapabilities(a);
    }
    HRESULT _stdcall EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA a,void* b,DWORD c) {
        return RealDevice->EnumObjects(a,b,c);
    }
    HRESULT _stdcall GetProperty(REFGUID a,DIPROPHEADER* b) {
        return RealDevice->GetProperty(a,b);
    }
    HRESULT _stdcall SetProperty(REFGUID a,const DIPROPHEADER* b) {
        return RealDevice->SetProperty(a,b);
    }
    HRESULT _stdcall Acquire() {
        return RealDevice->Acquire();
    }
    HRESULT _stdcall Unacquire() {
        return RealDevice->Unacquire();
    }
    HRESULT _stdcall GetDeviceState(DWORD a,void* b) {
        BYTE* b2=(BYTE*)b;
        BYTE bytes[256];    //Create an array of 256 bytes to store the keystates
        HRESULT hr=RealDevice->GetDeviceState(256,bytes);
        if(hr!=DI_OK) return hr;    //Error getting device, so return
        //We only want to modify keyboard input in alternate combat mode when the player has the mouse held down
        ZeroMemory(b,256);
        for(int i=0;i<256;i++) {
            if(RemappedKeys[i]) b2[RemappedKeys[i]]=bytes[i]; else b2[i]=bytes[i];
        }
        return DI_OK;
    }
    HRESULT _stdcall GetDeviceData(DWORD a,DIDEVICEOBJECTDATA* b,DWORD* c,DWORD d) {
        if(*c!=1||b==NULL) return RealDevice->GetDeviceData(a,b,c,d);
        HRESULT hr=RealDevice->GetDeviceData(a,b,c,d);
        if(*c!=1||hr!=DI_OK) return hr;
        if(RemappedKeys[b->dwOfs]) {
            b->dwOfs=RemappedKeys[b->dwOfs];
        }
        return hr;
    }
    HRESULT _stdcall SetDataFormat(const DIDATAFORMAT* a) {
        return RealDevice->SetDataFormat(a);
    }
    HRESULT _stdcall SetEventNotification(HANDLE a) {
        return RealDevice->SetEventNotification(a);
    }
    HRESULT _stdcall SetCooperativeLevel(HWND a,DWORD b) {
        return RealDevice->SetCooperativeLevel(a,b);
    }
    HRESULT _stdcall GetObjectInfo(DIDEVICEOBJECTINSTANCE* a,DWORD b,DWORD c) {
        return RealDevice->GetObjectInfo(a,b,c);
    }
    HRESULT _stdcall GetDeviceInfo(DIDEVICEINSTANCE* a) {
        return RealDevice->GetDeviceInfo(a);
    }
    HRESULT _stdcall RunControlPanel(HWND a,DWORD b) {
        return RealDevice->RunControlPanel(a,b);
    }
    HRESULT _stdcall Initialize(HINSTANCE a,DWORD b,REFGUID c) {
        return RealDevice->Initialize(a,b,c);
    }
    HRESULT _stdcall CreateEffect(REFGUID a,const DIEFFECT* b,LPDIRECTINPUTEFFECT* c,IUnknown* d) {
        return RealDevice->CreateEffect(a,b,c,d);
    }
    HRESULT _stdcall EnumEffects(LPDIENUMEFFECTSCALLBACK a,void* b,DWORD c) {
        return RealDevice->EnumEffects(a,b,c);
    }
    HRESULT _stdcall GetEffectInfo(DIEFFECTINFO* a,REFGUID b) {
        return RealDevice->GetEffectInfo(a,b);
    }
    HRESULT _stdcall GetForceFeedbackState(DWORD* a) {
        return RealDevice->GetForceFeedbackState(a);
    }
    HRESULT _stdcall SendForceFeedbackCommand(DWORD a) {
        return RealDevice->SendForceFeedbackCommand(a);
    }
    HRESULT _stdcall EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK a,void* b,DWORD c) {
        return RealDevice->EnumCreatedEffectObjects(a,b,c);
    }
    HRESULT _stdcall Escape(DIEFFESCAPE* a) {
        return RealDevice->Escape(a);
    }
    HRESULT _stdcall Poll() {
        return RealDevice->Poll();
    }
    HRESULT _stdcall SendDeviceData(DWORD a,const DIDEVICEOBJECTDATA* b,DWORD* c,DWORD d) {
        return RealDevice->SendDeviceData(a,b,c,d);
    }
    HRESULT _stdcall EnumEffectsInFile(const char* a,LPDIENUMEFFECTSINFILECALLBACK b,void* c,DWORD d) {
        return RealDevice->EnumEffectsInFile(a,b,c,d);
    }
    HRESULT _stdcall WriteEffectToFile(const char* a,DWORD b,DIFILEEFFECT* c,DWORD d) {
        return RealDevice->WriteEffectToFile(a,b,c,d);
    }
    HRESULT _stdcall BuildActionMap(DIACTIONFORMAT* a,const char* b,DWORD c) {
        return RealDevice->BuildActionMap(a,b,c);
    }
    HRESULT _stdcall SetActionMap(DIACTIONFORMAT* a,const char* b,DWORD c) {
        return RealDevice->SetActionMap(a,b,c);
    }
    HRESULT _stdcall GetImageInfo(DIDEVICEIMAGEINFOHEADER* a) {
        return RealDevice->GetImageInfo(a);
    }
};
class CombatWrapper : public IDirectInputDevice8 {
public:
    LPDIRECTINPUTDEVICE8 RealDevice;
    DWORD DeviceType;
    /*** Constructor ***/
    CombatWrapper(IDirectInputDevice8* device,DWORD DevType) {
        RealDevice=device;
        DeviceType=DevType;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (REFIID riid,void** ppvObj) {
        return RealDevice->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef() {
        return RealDevice->AddRef();
    }
    ULONG _stdcall Release() {
        return RealDevice->Release();
    }

    /*** IDirectInputDevice8W methods ***/
    HRESULT _stdcall GetCapabilities(LPDIDEVCAPS a) {
        return RealDevice->GetCapabilities(a);
    }
    HRESULT _stdcall EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA a,void* b,DWORD c) {
        return RealDevice->EnumObjects(a,b,c);
    }
    HRESULT _stdcall GetProperty(REFGUID a,DIPROPHEADER* b) {
        return RealDevice->GetProperty(a,b);
    }
    HRESULT _stdcall SetProperty(REFGUID a,const DIPROPHEADER* b) {
        return RealDevice->SetProperty(a,b);
    }
    HRESULT _stdcall Acquire() {
        return RealDevice->Acquire();
    }
    HRESULT _stdcall Unacquire() {
        return RealDevice->Unacquire();
    }
    HRESULT _stdcall GetDeviceState(DWORD a,void* b) {
        if(DeviceType==DeviceType_KEYBOARD) {
            //This is the keyboard
            BYTE bytes[256];    //Create an array of 256 bytes to store the keystates
            HRESULT hr=RealDevice->GetDeviceState(256,bytes);
            if(hr!=DI_OK) return hr;    //Error getting device, so return
            //We only want to modify keyboard input in alternate combat mode when the player has the mouse held down
            if(UseAltCombat&&AttackType) {  
                bytes[0x1e]=bytes[0x1f]=bytes[0x20]=bytes[0x11]=0;  //Set all movement keys to 0
                //Then set appropriate keys to 0x80 depending on what type of attack is being made
                if(GlobalHammer) {
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
            ::CopyMemory(b,bytes,a);        //Copy the bytes array both to morrowind and to the LastBytes array
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
    HRESULT _stdcall GetDeviceData(DWORD a,DIDEVICEOBJECTDATA* b,DWORD* c,DWORD d) {
        //This only gets called for keyboards, and can be ignored because we don't want to change buffered data
        return RealDevice->GetDeviceData(a,b,c,d);
    }
    HRESULT _stdcall SetDataFormat(const DIDATAFORMAT* a) {
        return RealDevice->SetDataFormat(a);
    }
    HRESULT _stdcall SetEventNotification(HANDLE a) {
        return RealDevice->SetEventNotification(a);
    }
    HRESULT _stdcall SetCooperativeLevel(HWND a,DWORD b) {
        return RealDevice->SetCooperativeLevel(a,b);
    }
    HRESULT _stdcall GetObjectInfo(DIDEVICEOBJECTINSTANCE* a,DWORD b,DWORD c) {
        return RealDevice->GetObjectInfo(a,b,c);
    }
    HRESULT _stdcall GetDeviceInfo(DIDEVICEINSTANCE* a) {
        return RealDevice->GetDeviceInfo(a);
    }
    HRESULT _stdcall RunControlPanel(HWND a,DWORD b) {
        return RealDevice->RunControlPanel(a,b);
    }
    HRESULT _stdcall Initialize(HINSTANCE a,DWORD b,REFGUID c) {
        return RealDevice->Initialize(a,b,c);
    }
    HRESULT _stdcall CreateEffect(REFGUID a,const DIEFFECT* b,LPDIRECTINPUTEFFECT* c,IUnknown* d) {
        return RealDevice->CreateEffect(a,b,c,d);
    }
    HRESULT _stdcall EnumEffects(LPDIENUMEFFECTSCALLBACK a,void* b,DWORD c) {
        return RealDevice->EnumEffects(a,b,c);
    }
    HRESULT _stdcall GetEffectInfo(DIEFFECTINFO* a,REFGUID b) {
        return RealDevice->GetEffectInfo(a,b);
    }
    HRESULT _stdcall GetForceFeedbackState(DWORD* a) {
        return RealDevice->GetForceFeedbackState(a);
    }
    HRESULT _stdcall SendForceFeedbackCommand(DWORD a) {
        return RealDevice->SendForceFeedbackCommand(a);
    }
    HRESULT _stdcall EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK a,void* b,DWORD c) {
        return RealDevice->EnumCreatedEffectObjects(a,b,c);
    }
    HRESULT _stdcall Escape(DIEFFESCAPE* a) {
        return RealDevice->Escape(a);
    }
    HRESULT _stdcall Poll() {
        return RealDevice->Poll();
    }
    HRESULT _stdcall SendDeviceData(DWORD a,const DIDEVICEOBJECTDATA* b,DWORD* c,DWORD d) {
        return RealDevice->SendDeviceData(a,b,c,d);
    }
    HRESULT _stdcall EnumEffectsInFile(const char* a,LPDIENUMEFFECTSINFILECALLBACK b,void* c,DWORD d) {
        return RealDevice->EnumEffectsInFile(a,b,c,d);
    }
    HRESULT _stdcall WriteEffectToFile(const char* a,DWORD b,DIFILEEFFECT* c,DWORD d) {
        return RealDevice->WriteEffectToFile(a,b,c,d);
    }
    HRESULT _stdcall BuildActionMap(DIACTIONFORMAT* a,const char* b,DWORD c) {
        return RealDevice->BuildActionMap(a,b,c);
    }
    HRESULT _stdcall SetActionMap(DIACTIONFORMAT* a,const char* b,DWORD c) {
        return RealDevice->SetActionMap(a,b,c);
    }
    HRESULT _stdcall GetImageInfo(DIDEVICEIMAGEINFOHEADER* a) {
        return RealDevice->GetImageInfo(a);
    }
};
class FakeDirectInputDevice : public IDirectInputDevice8 {
private:
	inline bool isRepeatedKey (DWORD key) {
		switch (FakeKeys [key].Graphics.function) {
		case GF_IncreaseZoom:
		case GF_DecreaseZoom:
		case GF_IncreaseView:
		case GF_DecreaseView:
		case GF_IncreaseAI:
		case GF_DecreaseAI:
		case GF_IncreaseFOV:
		case GF_DecreaseFOV:
		case GF_MoveForward3PC:
		case GF_MoveBack3PC:
		case GF_MoveLeft3PC:
		case GF_MoveRight3PC:
		case GF_MoveDown3PC:
		case GF_MoveUp3PC:
			return true;
		}
		return false;
	}
public:
    IDirectInputDevice8* RealDevice;
    DWORD DeviceType;
    /*** Constructor and misc functions ***/
    FakeDirectInputDevice(IDirectInputDevice8* device,DWORD type) {
        RealDevice=device;
        DeviceType=type;
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

    /*** IDirectInputDevice8A methods ***/
    HRESULT _stdcall GetCapabilities(LPDIDEVCAPS a) {
        return RealDevice->GetCapabilities(a);
    }
    HRESULT _stdcall EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA a,LPVOID b,DWORD c) {
        return RealDevice->EnumObjects(a,b,c);
    }
    HRESULT _stdcall GetProperty(REFGUID a,DIPROPHEADER* b) {
        return RealDevice->GetProperty(a,b);
    }
    HRESULT _stdcall SetProperty(REFGUID a,const DIPROPHEADER* b) {
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
            //This is a keyboard, so get a list of bytes (Dont forget the mouse too)
            BYTE bytes[MAXMACROS];
            HRESULT hr=RealDevice->GetDeviceState(256,bytes);
            if(hr!=DI_OK) return hr;
            CopyMemory(&bytes[256],&MouseOut,10);
            //Get any extra key presses
            if(GlobalHammer=!GlobalHammer) {
                for(DWORD byte=0;byte<256;byte++) {
                    bytes[byte]|=FakeStates[byte];
                    bytes[byte]|=HammerStates[byte];
                    bytes[byte]&=DisallowStates[byte];
                    if(TapStates[byte]) {
                        bytes[byte]=0x80;
                        TapStates[byte]=0;
                    }
                }
                for(DWORD byte=256;byte<MAXMACROS-2;byte++) {
                    bytes[byte]|=FakeStates[byte];
                    bytes[byte]|=HammerStates[byte];
                }
            } else {
                for(DWORD byte=0;byte<256;byte++) {
                    bytes[byte]|=FakeStates[byte];
                    bytes[byte]|=AHammerStates[byte];
                    bytes[byte]&=DisallowStates[byte];
                    if(TapStates[byte]) {
                        bytes[byte]=0x80;
                        TapStates[byte]=0;
                    }
                }
                for(DWORD byte=256;byte<MAXMACROS-2;byte++) {
                    bytes[byte]|=FakeStates[byte];
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
                    if(FakeKeys[key].type!=FKT_Unused&&KEYDOWN(bytes,key)) {
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
                                if (!KEYDOWN (LastBytes,key) || isRepeatedKey (key)) {
                                    switch(FakeKeys[key].Graphics.function) {
                                        case MF_ToggleAltCombat:
                                            UseAltCombat=!UseAltCombat;
                                            break;
                                        case MF_DelQuickSave:
                                            DeleteFile("saves\\quiksave.ess");
                                            break;
                                        case MF_DelAutoSave:
                                            DeleteFile("saves\\autosave.ess");
                                            break;
                                        default:
                                            (FakeFuncs[FakeKeys[key].Graphics.function])();
                                            break;
                                    }
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
            if(MouseDisable) {
                MouseState->lX=0;
                MouseState->lY=0;
            }
            if(MouseXMov) {
                MouseState->lX+=MouseXMov;
                MouseXMov=0;
            }
            if(MouseYMov) {
                MouseState->lY+=MouseYMov;
                MouseYMov=0;
            }
            if(MouseXSpeed) {
                float move=MouseXSpeed*JointInfo.FrameTime;
                MouseState->lX+=(long)move;
                if(MouseXSpeed>0) {
                    MouseXLeft+=fmodf(move,1.0f);
                    if(MouseXLeft>1) {
                        MouseState->lX+=1;
                        MouseXLeft-=1;
                    }
                } else {
                    MouseXLeft-=fmodf(-move,1.0f);
                    if(MouseXLeft<-1) {
                        MouseState->lX-=1;
                        MouseXLeft+=1;
                    }
                }
            }
            if(MouseYSpeed) {
                float move=MouseYSpeed*JointInfo.FrameTime;
                MouseState->lY+=(long)move;
                if(MouseYSpeed>0) {
                    MouseYLeft+=fmodf(move,1.0f);
                    if(MouseYLeft>1) {
                        MouseState->lY+=1;
                        MouseYLeft-=1;
                    }
                } else {
                    MouseYLeft-=fmodf(-move,1.0f);
                    if(MouseYLeft<-1) {
                        MouseState->lY-=1;
                        MouseYLeft+=1;
                    }
                }
            }
            for(DWORD i=0;i<8;i++) {
                MouseOut[i]=MouseState->rgbButtons[i];
                MouseState->rgbButtons[i]|=MouseIn[i];
                MouseState->rgbButtons[i]&=DisallowStates[i+256];
                if(TapStates[i+256]) {
                    MouseState->rgbButtons[i]=0x80;
                    TapStates[i+256]=0x00;
                }
            }
            return DI_OK;
        }
    }
    HRESULT _stdcall GetDeviceData(DWORD a,DIDEVICEOBJECTDATA* b,DWORD* c,DWORD d) {
        //This only gets called for keyboards
        if(*c==1&&SkipIntro) {
            //Skip the second intro (Beats me why it wants buffered data)
            SkipIntro=false;
            FakeBuffer[0].dwOfs=0x01;
            FakeBuffer[0].dwData=0x80;
            *b=FakeBuffer[0];
            return DI_OK;
        } else if(*c==1&&FakeBufferEnd>FakeBufferStart) {
            //Inject a fake keypress
            *b=FakeBuffer[FakeBufferStart++];
            if(FakeBufferStart==FakeBufferEnd) {
                if(CloseConsole) {
                    FinishedFake=true;
                    CloseConsole=false;
                }
                FakeBufferStart=0;
                FakeBufferEnd=0;
            }
            return DI_OK;
        } else {
            //Read a real keypress
            if(*c>1&&!CloseConsole) {
                FakeBufferStart=0;
                FakeBufferEnd=0;
            }
            return RealDevice->GetDeviceData(a,b,c,d);
        }
    }
    HRESULT _stdcall SetDataFormat(const DIDATAFORMAT* a) {
        return RealDevice->SetDataFormat(a);
    }
    HRESULT _stdcall SetEventNotification(HANDLE a) {
        return RealDevice->SetEventNotification(a);
    }
    HRESULT _stdcall SetCooperativeLevel(HWND a,DWORD b) {
        return RealDevice->SetCooperativeLevel(a,b);
    }
    HRESULT _stdcall GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA a,DWORD b,DWORD c) {
        return RealDevice->GetObjectInfo(a,b,c);
    }
    HRESULT _stdcall GetDeviceInfo(LPDIDEVICEINSTANCEA a) {
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
    HRESULT _stdcall EnumEffects(LPDIENUMEFFECTSCALLBACKA a,LPVOID b,DWORD c) {
        return RealDevice->EnumEffects(a,b,c);
    }
    HRESULT _stdcall GetEffectInfo(LPDIEFFECTINFOA a,REFGUID b) {
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
    HRESULT _stdcall EnumEffectsInFile(LPCSTR a,LPDIENUMEFFECTSINFILECALLBACK b,LPVOID c,DWORD d) {
        return RealDevice->EnumEffectsInFile(a,b,c,d);
    }
    HRESULT _stdcall WriteEffectToFile(LPCSTR a,DWORD b,LPDIFILEEFFECT c,DWORD d) {
        return RealDevice->WriteEffectToFile(a,b,c,d);
    }
    HRESULT _stdcall BuildActionMap(LPDIACTIONFORMATA a,LPCSTR b,DWORD c) {
        return RealDevice->BuildActionMap(a,b,c);
    }
    HRESULT _stdcall SetActionMap(LPDIACTIONFORMATA a,LPCSTR b,DWORD c) {
        return RealDevice->SetActionMap(a,b,c);
    }
    HRESULT _stdcall GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA a) {
        return RealDevice->GetImageInfo(a);
    }
};

class FakeDirectInput : public IDirectInput8 {
public:
    IDirectInput8* RealInput;
    /*** Constructor ***/
    FakeDirectInput(IDirectInput8* Real) {
        RealInput=Real;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (REFIID riid, LPVOID* ppvObj) {
        return RealInput->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef(void) {
        return RealInput->AddRef();
    }
    ULONG _stdcall Release(void) {
        return RealInput->Release();
    }

    /*** IDirectInput8W methods ***/
    HRESULT _stdcall CreateDevice(REFGUID r,IDirectInputDevice8A** device,IUnknown* unused) {
        if(r!=GUID_SysKeyboard&&r!=GUID_SysMouse) {
            return RealInput->CreateDevice(r,device,unused);
        } else {
            DWORD d;
            if(r==GUID_SysKeyboard) {
                d=DeviceType_KEYBOARD;
            } else {
                d=DeviceType_MOUSE;
            }
            IDirectInputDevice8A* RealDevice;
            HRESULT hr=RealInput->CreateDevice(r,&RealDevice,unused);
            if(hr!=DI_OK) return hr;
            if(UseAltCombatWrapper) {
                *device=new FakeDirectInputDevice(new CombatWrapper(RealDevice,d),d);
            } else {
                *device=new FakeDirectInputDevice(RealDevice,d);
            }
            if(r==GUID_SysKeyboard) {
                HANDLE RemapperFile=CreateFileA("MGE\\Remap.data",GENERIC_READ,0,0,OPEN_EXISTING,0,0);
                if(RemapperFile!=INVALID_HANDLE_VALUE) {
                    DWORD read;
                    ReadFile(RemapperFile,&RemappedKeys,256,&read,NULL);
                    CloseHandle(RemapperFile);
                    *device=new RemapWrapper(*device);
                }
            }
            return DI_OK;
        }
    }
    HRESULT _stdcall EnumDevices(DWORD a,LPDIENUMDEVICESCALLBACKA b,void* c,DWORD d) {
        return RealInput->EnumDevices(a,b,c,d);
    }
    HRESULT _stdcall GetDeviceStatus(REFGUID r) {
        return RealInput->GetDeviceStatus(r);
    }
    HRESULT _stdcall RunControlPanel(HWND a,DWORD b) {
        return RealInput->RunControlPanel(a,b);
    }
    HRESULT _stdcall Initialize(HINSTANCE a,DWORD b) {
        return RealInput->Initialize(a,b);
    }
    HRESULT _stdcall FindDevice(REFGUID a,LPCSTR b,LPGUID c) {
        return RealInput->FindDevice(a,b,c);
    }
    HRESULT _stdcall EnumDevicesBySemantics(LPCSTR a,LPDIACTIONFORMATA b,LPDIENUMDEVICESBYSEMANTICSCBA c,void* d,DWORD e) {
        return RealInput->EnumDevicesBySemantics(a,b,c,d,e);
    }
    HRESULT _stdcall ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK a,LPDICONFIGUREDEVICESPARAMSA b,DWORD c,void* d) {
        return RealInput->ConfigureDevices(a,b,c,d);
    }
};

void DInputOnProcessStart() {
    SkipIntro=false;         //Non zero to skip intros
    DisableConsole=false;    //Switch to true to disable the console
    GlobalHammer=true;       //Used to hammer keys
    UseAltCombatWrapper=false;

    FakeBufferStart=0;
    FakeBufferEnd=0;
    FinishedFake=false;      //true to shut down the console
    CloseConsole=false;      //true to shut the console after performing a command
    ZeroStruct(RemappedKeys);
    ZeroStruct(LastBytes);
    ZeroStruct(FakeStates);
    ZeroStruct(FakeBuffer);
    ZeroStruct(FakeKeys);
    ZeroStruct(Triggers);
    ZeroStruct(TapStates);
    for(WORD w=0;w<MAXMACROS;w++) DisallowStates[w]=0x80;

    MouseXMov=0;
    MouseYMov=0;
    MouseXSpeed=0;
    MouseYSpeed=0;
    MouseXLeft=0;
    MouseYLeft=0;
    MouseDisable=false;

    UseAltCombat=false;    //True when using the alternate combat mode
    MadeAttack=false;      //True when the player makes an attack (Because the keyboard must be used before the mouse)
    MadeAttackLast=false;  //Used in power attacks
    AttackType=0;          //Used to store the state of the mouse between frames
    LastAttack=0;          //You cant use the same attack twice in a row
}
void* CreateInputWrapper(void* real) {
	IDirectInput8A* Real=(IDirectInput8A*)real;
    //Now load all of the fake keys
    DWORD unused;
    HANDLE keyfile=CreateFileA("MGE3\\DInput.data",GENERIC_READ,0,0,OPEN_EXISTING,0,0);
    if(keyfile!=INVALID_HANDLE_VALUE) {
        BYTE version;
        ReadFile(keyfile,&version,1,&unused,NULL);
        if(version==MGE_SAVE_VERSION) {
            ReadFile(keyfile,&SkipIntro,1,&unused,NULL);
            ReadFile(keyfile,&DisableConsole,1,&unused,NULL);
            ReadFile(keyfile,&UseAltCombatWrapper,1,&unused,NULL);
            ReadFile(keyfile,&FakeKeys,sizeof(FakeKeys),&unused,NULL);
            ReadFile(keyfile,&Triggers,sizeof(Triggers),&unused,NULL);
        } else {
            ERRORMSG("MGE\\dinput.data appears to be out of date.\n"
                "You need to run MGEgui.exe at least once to update the save files.");
        }
        CloseHandle(keyfile);
    } else {
        ERRORMSG("Could not open MGE\\dinput.data for reading.\n"
            "You need to run MGEgui.exe at least once to create the save files.");
    }
	//Then create a fake device and return
	//Set initial trigger times
	for(int i=0;i<MAXTRIGGERS;i++) {
		TriggerFireTimes[i]=::GetTickCount()+Triggers[i].TimeInterval;
	}
	//Create the array of function pointers
	FakeFuncs[GF_Screenshot]=TakeScreenshot;
	FakeFuncs[GF_Fog]=ToggleFog;
	FakeFuncs[GF_ScaleFilter]=CycleScaleFilter;
	FakeFuncs[GF_MipFilter]=CycleMipFilter;
	FakeFuncs[GF_FogMode]=CycleFogMode;
	FakeFuncs[GF_AA]=ToggleAntialias;
	FakeFuncs[GF_CycleAniso]=CycleAnisoLevel;
	FakeFuncs[GF_ToggleZoom]=ToggleZoom;
	FakeFuncs[GF_IncreaseZoom]=IncreaseZoom;
	FakeFuncs[GF_DecreaseZoom]=DecreaseZoom;
	FakeFuncs[GF_RotateScreen]=RotateScreen;
	FakeFuncs[GF_ToggleText]=ToggleStatusText;
	FakeFuncs[GF_ShowLastText]=ShowLastMessage;
	FakeFuncs[GF_HWShader]=ToggleHardwareShader;
	FakeFuncs[GF_ToggleFps]=ToggleFpsCounter;
	FakeFuncs[GF_IncreaseFog]=IncreaseFog;
	FakeFuncs[GF_DecreaseFog]=DecreaseFog;
	FakeFuncs[GF_ToggleAAFix]=ToggleAAFix;
	FakeFuncs[GF_IncreaseView]=IncreaseView;
	FakeFuncs[GF_DecreaseView]=DecreaseView;
	FakeFuncs[GF_IncreaseAI]=IncreaseAI;
	FakeFuncs[GF_DecreaseAI]=DecreaseAI;
	FakeFuncs[GF_IncreaseLOD]=IncreaseLOD;
	FakeFuncs[GF_DecreaseLOD]=DecreaseLOD;
	FakeFuncs[GF_ToggleCrosshair]=ToggleCrosshair;
	FakeFuncs[GF_NextTrack]=NextTrack;
	FakeFuncs[GF_DisableMusic]=DisableMusic;
	FakeFuncs[GF_HaggleMore1]=HaggleMore1;
	FakeFuncs[GF_HaggleMore100]=HaggleMore100;
	FakeFuncs[GF_HaggleMore10000]=HaggleMore10000;
	FakeFuncs[GF_HaggleLess1]=HaggleLess1;
	FakeFuncs[GF_HaggleLess100]=HaggleLess100;
	FakeFuncs[GF_HaggleLess10000]=HaggleLess10000;
	FakeFuncs[GF_ToggleDL]=ToggleDistantLand;
	FakeFuncs[GF_ToggleDS]=ToggleDistantStatics;
	FakeFuncs[GF_ToggleGrass]=ToggleGrass;
	FakeFuncs[GF_ToggleMwMgeBlending]=ToggleMwMgeBlending;
	FakeFuncs[GF_CycleWater]=CycleWater;
	FakeFuncs[GF_IncreaseFOV]=IncreaseFOV;
	FakeFuncs[GF_DecreaseFOV]=DecreaseFOV;
	FakeFuncs[GF_HaggleMore10]=HaggleMore10;
	FakeFuncs[GF_HaggleMore1000]=HaggleMore1000;
	FakeFuncs[GF_HaggleLess10]=HaggleLess10;
	FakeFuncs[GF_HaggleLess1000]=HaggleLess1000;
	FakeFuncs[GF_ToggleSkyReflection]=ToggleSkyReflection;
	FakeFuncs [GF_MoveForward3PC] = MoveForward3PC;
	FakeFuncs [GF_MoveBack3PC] = MoveBack3PC;
	FakeFuncs [GF_MoveLeft3PC] = MoveLeft3PC;
	FakeFuncs [GF_MoveRight3PC] = MoveRight3PC;
	FakeFuncs [GF_MoveDown3PC]= MoveDown3PC;
	FakeFuncs [GF_MoveUp3PC] = MoveUp3PC;
	DEBUGSTRING("MME: The direct input device has been wrapped\n");
	return new FakeDirectInput(Real);
}
