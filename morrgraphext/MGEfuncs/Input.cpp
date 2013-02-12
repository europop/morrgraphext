#define INITGUID
#include <dinput.h>

typedef HRESULT (_stdcall *DInputProc)(HINSTANCE,DWORD,REFIID,LPVOID,LPUNKNOWN); 

IDirectInputDevice8* InputDevice=NULL;
IDirectInput8A* InputI=NULL;
HMODULE InputDLL=NULL;

void _stdcall EndRemapper() {
    if(InputDevice) InputDevice->Unacquire();
    if(InputDevice) InputDevice->Release();
    if(InputI) InputI->Release();
    if(InputDLL) FreeLibrary(InputDLL);

	InputDevice=0;
	InputI=0;
	InputDLL=0;
}

BYTE _stdcall InitRemapper(HWND hwin) {
    //OutputDebugString("Init called");
    HRESULT hr;
    DInputProc func=NULL;
    HINSTANCE hinst=(HINSTANCE)GetModuleHandle("dx8s.dll");
    char Path[256];
    GetSystemDirectoryA(Path,256);
    strcat_s(Path,256,"\\dinput8.dll");
    InputDLL=LoadLibraryA(Path);
    if(InputDLL==NULL) {
        //OutputDebugString("LoadLibrary failed");
        return 1;
    }
    func=(DInputProc)GetProcAddress(InputDLL,"DirectInput8Create");
    if(func==NULL) {
        OutputDebugString("GetProcAddress failed");
        EndRemapper();
        return 1;
    }
    hr=(func)(hinst,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&InputI,NULL);
    if(hr!=DI_OK) {
        //OutputDebugString("DirectInput8Create() failed");
        EndRemapper();
        return 1;
    }
    hr=InputI->CreateDevice(GUID_SysKeyboard,&InputDevice,NULL);
    if(hr!=DI_OK) {
        //OutputDebugString("DirectInput8->CreateDevice() failed");
        EndRemapper();
        return 1;
    }
    hr=InputDevice->SetDataFormat(&c_dfDIKeyboard);
    if(hr!=DI_OK) {
        //OutputDebugString("DirectInputDevice8->SetDataFormat failed");
        EndRemapper();
        return 1;
    }
    hr=InputDevice->SetCooperativeLevel(hwin,DISCL_BACKGROUND|DISCL_NONEXCLUSIVE); 
    if(hr!=DI_OK) {
        //OutputDebugString("DirectInputDevice8->SetCooperativeLevel failed");
        EndRemapper();
        return 1;
    }
    //OutputDebugStringA("Input device created");
    InputDevice->Acquire();

    return 0;
}

BYTE _stdcall GetKeyPressed() {
    if(!InputDevice) return 1;
    BYTE buffer[256];
	ZeroMemory(&buffer, sizeof(buffer));
    HRESULT hr=InputDevice->GetDeviceState(256,&buffer);
    if(hr==DIERR_INPUTLOST) {
        //OutputDebugStringA("Unaquired");
        InputDevice->Acquire();
        return 0;
    }
    for(int i=0;i<256;i++) {
        if(buffer[i]) return i;
    }
    //OutputDebugStringA("No keys pressed");

	return 0;
}
