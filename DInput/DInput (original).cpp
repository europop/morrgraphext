#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define KEYDOWN(name, key) (name[key] & 0x80)

#define DeviceType_UNSET    0
#define DeviceType_KEYBOARD 1
#define DeviceType_MOUSE    2

struct sFakeKey {
    DWORD KeyCode;
    DWORD chars[256];
    DWORD data[256];
    DWORD size;
};

typedef HRESULT (*MYPROC)(HINSTANCE,DWORD,REFIID,LPVOID,LPUNKNOWN); 

HMODULE dinput8dll = NULL;
MYPROC func=NULL;
DWORD sequence=0;
BOOL SkipIntro=true;
BOOL SkipIntro2=true;
sFakeKey FakeKeys[32];

class FakeDevice : public IDirectInputDevice8W {
public:
    LPDIRECTINPUTDEVICE8W RealDevice;
    DWORD DeviceType;
    BYTE LastBytes[256];
    DIDEVICEOBJECTDATA FakeBuffer[256];  
    DWORD UseFakeBuffer;
    BOOL FinishedFake;
    /*** Constructor and misc functions ***/
    FakeDevice() {
        RealDevice=NULL;
        DeviceType=DeviceType_UNSET;
        UseFakeBuffer=0;
        FinishedFake=false;
    }
    void _stdcall FakeKeyPress(DWORD index,DWORD key,DWORD data) {
        FakeBuffer[index].dwOfs=key;
        FakeBuffer[index].dwData=data;
        FakeBuffer[index].dwTimeStamp=::GetTickCount();
        FakeBuffer[index].dwSequence=sequence++;
        FakeBuffer[index].uAppData=0xffffffff;
    }
    void _stdcall FakeString(DWORD chars[],DWORD data[],DWORD length) {
        UseFakeBuffer=length+2;
        DWORD count=0;
        FakeKeyPress(count++,0x1c,0x00);
        FakeKeyPress(count++,0x1c,0x80);
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
        //This needs to be modified to use b instead of bytes - it would save one copymemory
        BYTE bytes[256];
        HRESULT hr=RealDevice->GetDeviceState(sizeof(bytes),bytes);
        //push escape to skip the intro
        if(SkipIntro) {
            if(SkipIntro2) {
                bytes[0x01]=0x80;
                SkipIntro2=false;
            } else {
                SkipIntro2=true;
            }
        } else if(FinishedFake) {
            FinishedFake=false;
            bytes[0x29]=0x80;
        } else if(UseFakeBuffer==0) {
            //Only want to respond to keypresses if the buffer is empty
            for(BYTE b=0;b<32;b++) {
                if(FakeKeys[b].KeyCode==0) break;
                if(KEYDOWN(bytes,FakeKeys[b].KeyCode)&&!KEYDOWN(LastBytes,FakeKeys[b].KeyCode)) {
                    bytes[0x29]=0x80;
                    bytes[FakeKeys[b].KeyCode]=0x00;
                    FakeString(FakeKeys[b].chars,FakeKeys[b].data,FakeKeys[b].size);
                }
            }
        }
        ::CopyMemory(b,bytes,a);
        ::CopyMemory(LastBytes,bytes,a);
        return hr;
    }
    HRESULT _stdcall GetDeviceData(THIS_ DWORD a,LPDIDEVICEOBJECTDATA b,LPDWORD c,DWORD d) {
        //Switch not necessary because only keyboard is faked
        //TODO: When adding mouse support remember to check sequence
        if(*c==1&&SkipIntro) {
            //Skip the second intro
            SkipIntro=false;
            FakeBuffer[0].dwOfs=0x01;
            FakeBuffer[0].dwData=0x80;
            FakeBuffer[0].dwSequence=1;
            FakeBuffer[0].dwTimeStamp=::GetTickCount();
            FakeBuffer[0].uAppData=0xffffffff;
            *b=FakeBuffer[0];
            return DI_OK;
        } else if(*c==1&&UseFakeBuffer>0) {
            //Inject a fake keypress
            *b=FakeBuffer[--UseFakeBuffer];
            if(UseFakeBuffer==0) FinishedFake=true;
            return DI_OK;
        } else {
            //Read a real keypress
            HRESULT hr2=RealDevice->GetDeviceData(a,b,c,d);
            if(*c>0&&b) {
                sequence=b[*c-1].dwSequence+1;
            }
            return hr2;
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
        //Returns a fake direct input device for keyboard, or a real one for anything else
        if(r!=GUID_SysKeyboard) {
            return RealInput->CreateDevice(r,device,unused);
        } else {
            FakeDevice *fd=new FakeDevice();
            fd->DeviceType=DeviceType_KEYBOARD;
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
            //If it's a success then load all of the fake keys
            DWORD unused;
            HANDLE keyfile=(HANDLE)::CreateFile(TEXT("FakeKeys.data"),GENERIC_READ,0,0,OPEN_EXISTING,
                0,0);
            if(keyfile==INVALID_HANDLE_VALUE) return DI_OK;
            ReadFile(keyfile,&SkipIntro,1,&unused,NULL);
            ReadFile(keyfile,&FakeKeys,sizeof(FakeKeys),&unused,NULL);
            CloseHandle(keyfile);
            return DI_OK;
        }
}