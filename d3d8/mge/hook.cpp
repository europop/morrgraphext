#include "Configuration.h"
#include "d3d8header.h"
#include "VidTex.h"
#include "MWSEFunctions.h"
#include "MWBridge.h"

typedef void* (_cdecl *fopenProc)(char* file,char* args);
static fopenProc Real_fopen;

void HookFile(char* newfile,char** file) {
    //variables
    HANDLE h;
    DWORD BytesRead;
    char buffer[256];
    //open the original file and check if it's an MGE override
    h=CreateFileA(*file,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
    if(h==INVALID_HANDLE_VALUE) return;
    ReadFile(h,buffer,256,&BytesRead,0);
    CloseHandle(h);
    if(BytesRead<5||buffer[0]!='M'||buffer[1]!='G'||buffer[2]!='E') return;
    //This is an mge override. Replace the old file pointer with the default texture, and tell mge to catch it
    strcpy(newfile,"data files\\textures\\_land_default.tga");
    *file=newfile;
    for(BYTE i=0;i<MAXHOOKEDTEXTURES;i++) {
        if(strcmp(HookedTextures[i].path,&buffer[3])==0) {
            HookNextTexture=true;
            NextHookIndex=i;
            return;
        }
    }
    for(BYTE i=0;i<MAXHOOKEDTEXTURES;i++) {
        if(HookedTextures[i].path[0]=='\0') {
			if(!_stricmp(&buffer[strlen(buffer)-4], ".avi")) {
				IDirect3DTexture9* tex;
				void* movie;
				if(!LoadMovie(&buffer[3], &tex, &movie)) {
					DEBUGSTRING("Texture hook failed because substitute movie could not be loaded\n");
					return;
				}
				HookedTextures[i].avi=movie;
				HookedTextures[i].tex=tex;
			} else {
				HookedTextures[i].TextureIndex = MWSE::TextureIndex(&buffer[3],true);
				if(HookedTextures[i].TextureIndex==255) {
					DEBUGSTRING("Texture hook failed because substitute texture could not be loaded\n");
					return;
				}
			}
            HookNextTexture=true;
            NextHookIndex=i;
            strcpy(HookedTextures[i].path,*file);
            if(buffer[4+strlen(&buffer[3])]!='\0') {
				HookedTextures[i].ShaderIndex = MWSE::ShaderIndex(&buffer[4+strlen(&buffer[3])],true);
            } else {
                HookedTextures[i].ShaderIndex = 255;
            }
            HookedTextures[i].AutoClean=true;
            return;
        }
    }
}

void* _cdecl FileOpenHook(char* file,char* args) {
	if((MGEFlags & USE_DISTANT_LAND) && !DlDoneHooking && strlen(file)==37 ) {
		char str[38];
		strcpy(str,file);
		str[31]='\0';
		_strlwr(str);
		if(!strcmp(str,"data files\\textures\\water\\water")) {
			//DEBUGSTRING("MGE: Texturn hook fired: next loaded texture is water\n");
			NextTexIsWater=true;
		}
	}
	if (MGEFlags & USE_TEX_HOOKS) {
		char newfile[256];
		DWORD size=strlen(file);
		if(file[size-3]=='d') {
			if(file[size-2]=='d'&&file[size-1]=='s') HookFile(newfile,&file);
		} else if(file[size-3]=='t') {
			if(file[size-2]=='g'&&file[size-1]=='a') HookFile(newfile,&file);
		} else if(file[size-3]=='b') {
			if(file[size-2]=='m'&&file[size-1]=='p') HookFile(newfile,&file);
		}
	}
	//Restore origional fopen pointer if we're done with it.
	if (!(MGEFlags & USE_TEX_HOOKS) && DlDoneHooking) {
		DECLARE_MWBRIDGE
		DWORD OldProtect;
		VirtualProtect(mwBridge->GetFOpenPnt(),4,PAGE_READWRITE, &OldProtect);
		*(DWORD*)(mwBridge->GetFOpenPnt())=(DWORD)Real_fopen;
		VirtualProtect(mwBridge->GetFOpenPnt(),4,OldProtect, &OldProtect);
	}
    return Real_fopen(file,args);
}

void InitHooks() {
	DECLARE_MWBRIDGE
	//Approx 1 gazillion times simpler than the old hooking method.
	//Just overwrite the fopen function pointer
	DEBUGSTRING("MGE: Hooking fopen function pointer\n");
    Real_fopen = (fopenProc)*(DWORD*)mwBridge->GetFOpenPnt();
	DWORD OldProtect;
	VirtualProtect(mwBridge->GetFOpenPnt(),4,PAGE_READWRITE, &OldProtect);
	*(DWORD*)(mwBridge->GetFOpenPnt())=(DWORD)FileOpenHook;
	VirtualProtect(mwBridge->GetFOpenPnt(),4,OldProtect, &OldProtect);
}