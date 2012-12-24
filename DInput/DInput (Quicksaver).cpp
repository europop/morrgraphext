#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//0x007C67DC master 
//0x00D1E0CC gamma ( [0x007C6CDC]+0x3C )

#define KEYDOWN(name, key) (name[key] & 0x80)

#define DeviceType_UNSET    0
#define DeviceType_KEYBOARD 1
#define DeviceType_MOUSE    2

#define atForward 1
#define atSide 2
#define atDiag 3

typedef HRESULT (*MYPROC)(HINSTANCE,DWORD,REFIID,LPVOID,LPUNKNOWN); 

HMODULE dinput8dll = NULL;
MYPROC func=NULL;
DWORD sequence=0;
DWORD Ticks=0;

class FakeDevice : public IDirectInputDevice8W {
public:
    LPDIRECTINPUTDEVICE8W RealDevice;
    DWORD DeviceType;
    /*** Constructor and misc functions ***/
    FakeDevice() {
        RealDevice=NULL;
        DeviceType=DeviceType_UNSET;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (REFIID riid, LPVOID * ppvObj) {
        return RealDevice->QueryInterface(riid,ppvObj);
    }
    ULONG _stdcall AddRef(THIS) {
        return RealDevice->AddRef();
    }
    ULONG _stdcall Release(THIS) {
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
    HRESULT _stdcall Acquire(THIS) {
        return RealDevice->Acquire();
    }
    HRESULT _stdcall Unacquire(THIS) {
        return RealDevice->Unacquire();
    }
    HRESULT _stdcall GetDeviceState(DWORD a,LPVOID b) {
        //if(DeviceType==DeviceType_KEYBOARD) {
            //If this is a keyboard
            BYTE bytes[256];
            HRESULT hr=RealDevice->GetDeviceState(sizeof(bytes),bytes);
            DWORD d=GetTickCount();
            if(d > Ticks+120000) {
                Ticks=d;
                bytes[0x3f]=0x80;
            }
            ::CopyMemory(b,bytes,a);
            return hr;
        /*} else {    //This is the mouse
            /*if(AttackType) {
                DIMOUSESTATE2 di;
                HRESULT hr=RealDevice->GetDeviceState(sizeof(di),&di);
                di.rgbButtons[0]=0x80*(HammerMouse2=!HammerMouse2);
                *((DIMOUSESTATE2*)b)=di;
                return hr;
            } else {
                return RealDevice->GetDeviceState(a,b);
            }*/
        //}
    }
    HRESULT _stdcall GetDeviceData(DWORD a,LPDIDEVICEOBJECTDATA b,LPDWORD c,DWORD d) {
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
    HRESULT _stdcall Poll(THIS) {
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
    LPDIRECTINPUT8W RealInput;
    /*** Constructor ***/
    FakeDirectInput() {
        RealInput=NULL;
    }
    /*** IUnknown methods ***/
    HRESULT _stdcall QueryInterface (REFIID riid, LPVOID * ppvObj)
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
    HRESULT _stdcall CreateDevice(REFGUID r,LPDIRECTINPUTDEVICE8W *device,LPUNKNOWN unused)
    {
        //Returns a fake direct input device for keyboard, or a real one for anything else
        if(r!=GUID_SysKeyboard/*&&r!=GUID_SysMouse*/) {
            return RealInput->CreateDevice(r,device,unused);
        } else {
            FakeDevice *fd=new FakeDevice();
            if(r==GUID_SysKeyboard) {
                fd->DeviceType=DeviceType_KEYBOARD;
                Ticks=::GetTickCount();
            } else {
                fd->DeviceType=DeviceType_MOUSE;
            }
            HRESULT hr=RealInput->CreateDevice(r,&(fd->RealDevice),unused);
            if(hr!=DI_OK) return hr;
            *device=fd;
            return DI_OK;
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


HRESULT _stdcall FakeDirectInputCreate( 
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID *ppvOut,
    LPUNKNOWN punkOuter
    ) {
        //Need to replace with GetSystemDirectory()
        dinput8dll=::LoadLibrary(TEXT("c:\\windows\\system32\\dinput8.dll"));
        func=(MYPROC)GetProcAddress(dinput8dll,"DirectInput8Create");
        FakeDirectInput *fdi=new FakeDirectInput();
        HRESULT hr=(func)(hinst,dwVersion,riidltf,(LPVOID*)&(fdi->RealInput),punkOuter);
        *ppvOut=(LPVOID*)fdi;
        if(hr != DI_OK) {
            return hr;
        } else {
            return DI_OK;
        }
}