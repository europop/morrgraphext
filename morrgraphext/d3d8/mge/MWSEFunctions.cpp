#include "Configuration.h"
#include "d3d8header.h"
#include "MME function defs.h"
#include "MWBridge.h"
#include "MWSEFunctions.h"
#include "DInputHeader.h"
#include "DistantLand.h"

#define ISKEYCODEVALID(a) if(a>=MAXMACROS-2) return
#define KEYBOARDVALID(a) if(a>=256) return

#define bSize 4096

static BYTE PipeBuffer[bSize];  //Just allocate it now to avoid having to repeatedly allocate/deallocate it each frame
static BYTE PipeOutBuffer[bSize];
static char ActiveModel[256];
static bool UsingModel = false;

static char ActiveHUDbit[33];
static bool UsingHUDbit = false;

extern HANDLE MWSEpipe2;
void MWSE_Update2(HANDLE);

//-----------------------------------------------------------------------------

int MWSE::HUDIndex(char* p, char* file) {
	if (strlen(p) > 32) return -1;
	BYTE FirstFree = 0xFF;
	for (int i = 0; i < MAXMWSEOBJECTS; i++) {
		if (HUDbits[i].texture) { if (!strcmp(p, HUDbits[i].Tag)) return i; }
		else if (FirstFree == 0xFF) FirstFree = i;
	}
	if (file != NULL) {
		if (FirstFree != 0xFF) {
			int index;
			if ((index = MWSE::TextureIndex(file, true)) == -1) {
				DEBUGSTRING("MGE: Could not create HUD element because the texture could not be loaded\n");
				return -1;
			}
			HUDbits[FirstFree].texture = LoadedTextures[index].texture;
			HUDbits[FirstFree].texture->AddRef();
			strcpy_s(HUDbits[FirstFree].Tag, 33, p);
			HUDbits[FirstFree].scale.x = 1.0f;
			HUDbits[FirstFree].scale.y = 1.0f;
			HUDbits[FirstFree].position.x = 0;
			HUDbits[FirstFree].position.y = 0;
			DEBUGSTRING("MGE: HUD element loaded successfully\n");
			return FirstFree;
		} else {
			DEBUGSTRING("MGE: Failed to load HUD element because there is no more room\n");
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------

void MWSE::FreeHUD(int index) {
	if (HUDbits[index].texture != NULL) HUDbits[index].texture->Release();
	else return;
	if (HUDbits[index].effect != NULL) HUDbits[index].effect->Release();
	HUDbits[index].Enabled = false;
	HUDbits[index].texture = NULL;
	HUDbits[index].effect = NULL;
	HUDbits[index].Tag[0] = '\0';
	DEBUGSTRING("MGE: HUD element release successful\n");
}

//-----------------------------------------------------------------------------

int MWSE::ObjectIndex(char* p, char* file) {
	if (strlen(p) > 32) return -1;
	BYTE FirstFree = 0xFF;
	for (int i = 0; i < MAXMWSEOBJECTS; i++) {
		if (Objects[i].Created) {
			if (!strcmp(p, Objects[0].Tag)) return i;
		} else if (FirstFree == 0xFF) FirstFree = i;
	}
	if (file != NULL && FirstFree != 0xFF && MWSE::IsSafeString(file)) {
		char buffer[257]; buffer[0] = '\0';
		strcat_s(buffer, 256, "Data Files\\MGE meshes\\");
		strcat_s(buffer, 256, file);
		DEBUGSTRING("MGE: Trying to load mesh\n");
		DEBUGSTRING(buffer);
		ID3DXMesh* TempMesh = NULL;
		ID3DXBuffer* Materials = NULL;
		DWORD subsets;
		ID3DXBuffer* Effects = NULL;
		HRESULT hr = D3DXLoadMeshFromXA(buffer, D3DXMESH_MANAGED, RealDevice, NULL, &Materials,
			&Effects, &subsets, &TempMesh);
		if (Effects) Effects->Release();
		if (hr != D3D_OK) {
			DEBUGSTRING("MGE: Failed to load mesh\n");
		} else if (subsets > MAXMWSESUBSETS) {
			DEBUGSTRING("MGE: Could not load mesh because it contained to many subsets\n");
			if (TempMesh != NULL) TempMesh->Release();
			if (Materials != NULL) Materials->Release();
		} else {
			ZeroMemory(&Objects[FirstFree], sizeof(MWSEobject));
			strcpy_s(Objects[FirstFree].Tag, 33, p);
			Objects[FirstFree].Created = true;
			Objects[FirstFree].IsMesh = true;
			D3DXMATERIAL* Mat = (D3DXMATERIAL*)Materials->GetBufferPointer();
			if (Materials != NULL) {
				for (UINT i = 0; i < subsets; i++) {
					Objects[FirstFree].materials[i] = Mat[i].MatD3D;
					if (Mat[i].pTextureFilename != NULL && Mat[i].pTextureFilename[0] != '\0') {
						int index = MWSE::TextureIndex(Mat[i].pTextureFilename, true);
						if (index != -1) {
							Objects[FirstFree].textures[i] = LoadedTextures[index].texture;
							LoadedTextures[index].texture->AddRef();
						}
					}
				}
				Materials->Release();
			}
			Objects[FirstFree].mesh = TempMesh;
			Objects[FirstFree].subsets = subsets;
			D3DXMatrixRotationYawPitchRoll(&Objects[FirstFree].RotMatrix, 0, 0, 0);
			D3DXMatrixScaling(&Objects[FirstFree].ScaleMatrix, 1, 1, 1);
			DEBUGSTRING("MGE: Mesh loaded successfully\n");
			UsedMWSEobjects++;
			return FirstFree;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------

void MWSE::FreeModel(int index) {
	if (Objects[index].Created) UsedMWSEobjects--;
	Objects[index].Enabled = false;
	Objects[index].Created = false;
	if (Objects[index].vBuffer != NULL) Objects[index].vBuffer->Release();
	if (Objects[index].iBuffer != NULL) Objects[index].iBuffer->Release();
	for (BYTE i = 0; i < MAXMWSESUBSETS; i++) if (Objects[index].textures[i] != NULL) Objects[index].textures[i]->Release();
	if (Objects[index].effect != NULL) Objects[index].effect->Release();
	Objects[index].Tag[0] = '\0';
	Objects[index].mesh->Release();
	DEBUGSTRING("MGE: Mesh release successful\n");
}

//-----------------------------------------------------------------------------

void MWSE::UpdateObjectMatrix(int index, BYTE type) { //0 - pos, 1 - rot, 2 - scale
	switch (type) {
		case 0:
			D3DXMatrixTranslation(&Objects[index].PosMatrix, Objects[index].pos.x,
				Objects[index].pos.y, Objects[index].pos.z);
			break;
		case 1:
			D3DXMatrixRotationYawPitchRoll(&Objects[index].RotMatrix, Objects[index].rot.x,
				Objects[index].rot.y, Objects[index].rot.z);
			break;
		case 2:
			D3DXMatrixScaling(&Objects[index].ScaleMatrix, Objects[index].scale.x,
				Objects[index].scale.y, Objects[index].scale.z);
			break;
	} 
	Objects[index].WorldMatrix = Objects[index].ScaleMatrix*Objects[index].RotMatrix*Objects[index].PosMatrix;
}

//-----------------------------------------------------------------------------

int MWSE::ShaderIndex(char* p, bool LoadIfNeeded) {
	BYTE FirstFree = 0xFF;
	for (int i = 0; i < MAXMWSESHADERS; i++) {
		if (ShaderStruct.MWSEeffects[i].effect != NULL) {
			if (!strcmp(p, ShaderStruct.MWSEeffects[i].FilePath)) return i;
		} else if (FirstFree == 0xFF) FirstFree = i;
	}
	if (LoadIfNeeded && FirstFree != 0xFF && MWSE::IsSafeString(p)) {
		char buffer[257]; buffer[0] = '\0';
		strcat_s(buffer, 256, "Data Files\\shaders\\");
		strcat_s(buffer, 256, p);
		DEBUGSTRING("MGE: Trying to load shader\n");
		DEBUGSTRING(buffer);
		HRESULT hr = D3DXCreateEffectFromFileA(RealDevice, buffer, 0, 0, 0, 0,
			&ShaderStruct.MWSEeffects[FirstFree].effect, NULL);
		if (hr == D3D_OK) {
			DEBUGSTRING("MGE: Shader loaded\n");
			strcpy_s(ShaderStruct.MWSEeffects[FirstFree].FilePath, MAXFILEPATHLENGTH+1, p);
			LoadShaderTextures(ShaderStruct.MWSEeffects[FirstFree].effect);
			InitShaderFlags(&ShaderStruct.MWSEeffects[FirstFree]);
			return FirstFree;
		} else {
			DEBUGSTRING("MGE: Error loading shader\n");
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------

void MWSE::FreeEffect(int index) {
	if (ShaderStruct.MWSEeffects[index].effect == NULL) return;
	ShaderStruct.MWSEeffects[index].Active = false;
	ShaderStruct.MWSEeffects[index].effect->Release();
	ShaderStruct.MWSEeffects[index].effect = NULL;
	ShaderStruct.MWSEeffects[index].FilePath[0] = '\0';
	DEBUGSTRING("MGE: Effect freed\n");
}

//-----------------------------------------------------------------------------

int MWSE::TextureIndex(char* p, bool LoadIfNeeded) {
	BYTE FirstFree = 0xFF;
	for (int i = 0; i < MAXMWSESHADERS; i++) {
		if (LoadedTextures[i].texture != NULL) {
			if (!strcmp(p, LoadedTextures[i].FilePath)) return i;
		} else if (FirstFree == 0xFF) FirstFree = i;
	}
	if (LoadIfNeeded && FirstFree != 0xFF && MWSE::IsSafeString(p)) {
		char buffer[256]; buffer[0] = '\0';
		HRESULT hr;
		strcat_s(buffer, 256, "Data Files\\Textures\\");
		strcat_s(buffer, 256, p);
		DEBUGSTRING("MGE: Trying to load texture\n");
		DEBUGSTRING(buffer);
		hr = D3DXCreateTextureFromFileA(RealDevice, buffer, &LoadedTextures[FirstFree].texture);
		if (hr == D3D_OK) {
			DEBUGSTRING("MGE: Texture loaded\n");
			strcpy_s(LoadedTextures[FirstFree].FilePath, MAXFILEPATHLENGTH+1, p);
			return FirstFree;
		} else {
			DEBUGSTRING("MGE: Error loading texture\n");
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------

void MWSE::FreeTexture(int index) {
	if (LoadedTextures[index].texture) LoadedTextures[index].texture->Release();
	LoadedTextures[index].texture = NULL;
	LoadedTextures[index].FilePath[0] = '\0';
	DEBUGSTRING("MGE: texture freed\n");
}

//-----------------------------------------------------------------------------

bool MWSE::IsSafeString(char* str) {
#define NOTSAFESTRING { DEBUGSTRING("MGE: file path failed security validation check\n"); return false; }
	int pointer = 0;
	char c;
	if (str[0] == '.' || str[0] == '\\') NOTSAFESTRING
	bool LastWasDot = true;
	while ((c = str[pointer++]) != '\0') {
		if (!((c >= 'a' && c <= 'z') || (c >= '0' || c <= '9') || c == '.' || c == '_' || c == '\\' || c == ' ' || c == '#')) NOTSAFESTRING
		if (c == '.') {
			if (LastWasDot) NOTSAFESTRING
			else LastWasDot = true;
		} else LastWasDot = false;
	}
	if (pointer > MAXFILEPATHLENGTH) NOTSAFESTRING
	else return true;
#undef NOTSAFESTRING
}

//-----------------------------------------------------------------------------

void MWSE::GetString(char* buffer, DWORD* pPointer) {
	MWSE::GetStringC(buffer, pPointer);
	_strlwr_s(buffer, 256);
}

//-----------------------------------------------------------------------------

void MWSE::GetStringC(char* buffer, DWORD* pPointer) {
	BYTE pos = 0;
	if (!IsBadStringPtr((LPCSTR)pPointer, 256)) {
		DWORD Pointer = *pPointer;
		while (PipeBuffer[Pointer] != '\0' && pos < 255) buffer[pos++] = PipeBuffer[Pointer++];
		*pPointer = Pointer+1;
	}
	buffer[pos] = '\0';
}

//-----------------------------------------------------------------------------

void MWSE::SetStringC(DWORD* pPointer, char* buffer) {
	BYTE pos = 0;
	DWORD Pointer = *pPointer;
	while (buffer[pos] != '\0' && pos < 255) PipeOutBuffer[Pointer++] = buffer[pos++];
	PipeOutBuffer[Pointer] = '\0';
	*pPointer = Pointer+1;
}

//-----------------------------------------------------------------------------

int MWSE_TextureHookIndex(char* str) {
	int index = -1;
	for (int i = 0; i < MAXHOOKEDTEXTURES; i++) {
		if (!strcmp(str, HookedTextures[i].path)) {
			index = i;
			break;
		}
	}
	return index;
}

//-----------------------------------------------------------------------------

void DoMWSEFunction() {
	MWSE_Update2(MWSEpipe2);
}

//-----------------------------------------------------------------------------

void MWSE::Update() {
	MWSE_Update2(MWSEpipe);
}

//-----------------------------------------------------------------------------

void MWSE_Update2(HANDLE MWSEpipe) {
	DWORD size;
	DWORD pointer = 0;
	DWORD BytesLeft;
	DWORD BytesToSend = 0;
	PeekNamedPipe(MWSEpipe, NULL, 0, NULL, &BytesLeft, NULL);
	if (BytesLeft && ReadFile(MWSEpipe, PipeBuffer, BytesLeft, &size, NULL) && size > 0) {
		DEBUGSTRING("MGE: Read MWSE command\n");
		while (pointer < size) {
			DEBUGSTRING("MGE: Parsed command\n");

			DECLARE_MWBRIDGE

			switch (PipeBuffer[pointer++]) {
				#pragma region "d" : Debug
				case 'd':
				case 0: {
					DEBUGSTRING("MGE: Recieved special command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "w" : WipeAll
						case 'w':   //Complete wipe
							if (size > 4080) {
								DEBUGSTRING("MGE: Recieved too many commands in one frame to use '\0w'\n");
							} else {
								PipeBuffer[size] = 'm';
								PipeBuffer[size+1] = 'w';
								PipeBuffer[size+2] = 't';
								PipeBuffer[size+3] = 'w';
								PipeBuffer[size+4] = 'S';
								PipeBuffer[size+5] = 'w';
								PipeBuffer[size+7] = 'H';
								PipeBuffer[size+8] = 'w';
								size += 8;
							}
							break;
						#pragma endregion
						#pragma region "p" : OutputDebugString
						case 'p':   //DebugString
							{
								DEBUGSTRING("MGE: Script is trying to print a debugging comment\n");
								char buffer[256];
								MWSE::GetString(buffer, &pointer);
								DEBUGSTRING(buffer);
								break;
							}
						#pragma endregion
						#pragma region "v" : OutputVersionString
						case 'v':
							DEBUGSTRING(VERSION);
							break;
						#pragma endregion
						#pragma region "V" : GetVersion
						case 'V':
							DEBUGSTRING("MGE: A script has requested MGE's version number\n");
							*((DWORD*)&PipeOutBuffer[BytesToSend]) = MGE_EX_VERSION;
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "x" : GetWidth
						case 'x':
							DEBUGSTRING("MGE: A script has requested the screen width\n");
							*((WORD*)&PipeOutBuffer[BytesToSend]) = JointInfo.GraphicsWidth;
							BytesToSend += 2;
							break;
						#pragma endregion
						#pragma region "y" : GetHeight
						case 'y':
							DEBUGSTRING("MGE: A script has requested the screen height\n");
							*((WORD*)&PipeOutBuffer[BytesToSend]) = JointInfo.GraphicsHeight;
							BytesToSend += 2;
							break;
						#pragma endregion
						#pragma region "f" : GetFPS
						case 'f':
							DEBUGSTRING("MGE: A script has requested the current fps\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = fps;
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "P" : GetEyeVec
						case 'P':
							DEBUGSTRING("MGE: A script has requested the players viewing direction\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = EyeVec.x;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = EyeVec.y;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = EyeVec.z;
							BytesToSend += 12;
							break;
						#pragma endregion
						#pragma region "9" : IsDX9Build
						case '9':
							DEBUGSTRING("MGE: A script has requested if this is a DX9 build of MGE\n");
							*((WORD*)&PipeOutBuffer[BytesToSend]) = 1;
							BytesToSend += 2;
							break;
						#pragma endregion
						#pragma region "m" : IsNoMemBuild
						case 'm':
							DEBUGSTRING("MGE: A script has requested if this is a NoMem build of MGE\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = 0;
							BytesToSend += 2;
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Special command not recognized\n");
							break;
					}
					break;
				}
				#pragma endregion
				#pragma region "D" : Distant Land
				case 'D': {
					DEBUGSTRING("MGE: Recieved a distant land command\n");
					char Command = PipeBuffer[pointer++];
					switch (Command) {
						#pragma region "d" : EnableDistantLand
						case 'd':   //Toggle distant land
							DEBUGSTRING("MGE: A script is trying to disable distant land\n");
							UsingDistantLand = false;
							DL_Deactivate();
							break;
						#pragma endregion
						#pragma region "e" : DisableDistantLand
						case 'e':
							DEBUGSTRING("MGE: A script is trying to enable distant land\n");
							UsingDistantLand = true;
							DL_Activate();
							break;
						#pragma endregion
						#pragma region "s" : DisableDistantStatics
						case 's':   //Disable distant statics
							DEBUGSTRING("MGE: A script is trying to disable distant statics\n");
							UseDistantStatics = false;
							break;
						#pragma endregion
						#pragma region "S" : EnableDistantStatics
						case 'S':   //Enable distant statics
							DEBUGSTRING("MGE: A script is trying to enable distant statics\n");
							UseDistantStatics = true;
							break;
						#pragma endregion
						#pragma region "g" : DisableGrass
						case 'g':   //Disable grass
							DEBUGSTRING("MGE: A script is trying to disable grass\n");
							UseGrass = false;
							break;
						#pragma endregion
						#pragma region "G" : EnableGrass
						case 'G':   //Enable grass
							DEBUGSTRING("MGE: A script is trying to enable grass\n");
							UseGrass = true;
							break;
						#pragma endregion
						#pragma region "w" : SetDLWindFactor
						case 'w': {  //SetDLWindFactor
							int wthr = *(short*)(&PipeBuffer[pointer]);
							if (wthr >= 0 && wthr <= 9) DL_Wind[wthr] = *(float*)(&PipeBuffer[pointer+2]);
							pointer += 6;
							break;
						}
						#pragma endregion
						#pragma region "W" : GetDLWindFactor
						case 'W': {  //GetDLWindFactor
							int wthr = *(short*)(&PipeBuffer[pointer]);
							float factor = 0;
							if (wthr >= 0 && wthr <= 9) factor = DL_Wind[wthr];
							*((float*)&PipeOutBuffer[BytesToSend]) = factor;
							pointer += 2;
							BytesToSend += 4;
							break;
						}
						#pragma endregion
						#pragma region "f" : SetDLFogFactor
						case 'f': {  //SetDLFogFactor
							int wthr = *(short*)(&PipeBuffer[pointer]);
							if (wthr >= 0 && wthr <= 9) DL_FogD[wthr] = *(float*)(&PipeBuffer[pointer+2]);
							pointer += 6;
							break;
						}
						#pragma endregion
						#pragma region "F" : GetDLFogFactor
						case 'F': {  //GetDLFogFactor
							int wthr = *(short*)(&PipeBuffer[pointer]);
							float factor = 0;
							if (wthr >= 0 && wthr <= 9) factor = DL_FogD[wthr];
							*((float*)&PipeOutBuffer[BytesToSend]) = factor;
							pointer += 2;
							BytesToSend += 4;
							break;
						}
						#pragma endregion
					}
				}
				#pragma endregion
				#pragma region "H" : HUD
				case 'H': {
					char CharPointer[256];
					int index;
					DEBUGSTRING("MGE: Recieved a HUD element command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "w" : WipeHUDElements
						case 'w':  //wipe all loaded HUD elements
							DEBUGSTRING("MGE: Unloading all loaded HUD elements\n");
							for (UINT i = 0; i < MAXMWSEOBJECTS; i++)
								if (HUDbits[i].texture) MWSE::FreeHUD(i);
							break;
						#pragma endregion
						#pragma region "W" : WithHUD
						case 'W':  //with HUD element do
							MWSE::GetString(CharPointer, &pointer);
							DEBUGSTRING("MGE: All following HUD commands will refer to object\n");
							DEBUGSTRING(CharPointer);
							if (strlen(CharPointer) > 33) {
								DEBUGSTRING("MGE: 'HW' failed because object id was more than 32 characters\n");
							} else {
								strcpy_s(ActiveHUDbit, 33, CharPointer);
								UsingHUDbit = true;
							}
							break;
						#pragma endregion
						#pragma region "c" : CancelWithHUD
						case 'c': //continue
							DEBUGSTRING("MGE: All following model commands must specify their own object\n");
							UsingHUDbit = false;
							break;
						#pragma endregion
						#pragma region "l" : LoadHUD
						case 'l':  //Assign and load HUD
						{
							char FileName[256];
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							MWSE::GetString(FileName, &pointer);
							DEBUGSTRING("MGE: Trying to create a new HUD element\n");
							DEBUGSTRING(CharPointer);
							DEBUGSTRING(FileName);
							MWSE::HUDIndex(CharPointer, FileName);
							break;
						}
						#pragma endregion
						#pragma region "p" : PositionHUD
						case 'p': //Position
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to position HUD element\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Position failed because HUD id does not exist\n");
							} else {
								HUDbits[index].position.x = *(float*)(&PipeBuffer[pointer]);
								HUDbits[index].position.y = *(float*)(&PipeBuffer[pointer+4]);
								DEBUGSTRING("MGE: Position successful\n");
							}
							pointer += 8;
							break;
						#pragma endregion
						#pragma region "s" : ScaleHUD
						case 's': //Scale
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to scale HUD element\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Scale failed because HUD id does not exist\n");
							} else {
								HUDbits[index].scale.x = *(float*)(&PipeBuffer[pointer]);
								HUDbits[index].scale.y = *(float*)(&PipeBuffer[pointer+4]);
								DEBUGSTRING("MGE: Scale successful\n");
							}
							pointer += 8;
							break;
						#pragma endregion
						#pragma region "F" : FullscreenHUD
						case 'F': //Make fullscreen
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to make HUD element full screen\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Scale failed because HUD id does not exist\n");
							} else {
								D3DSURFACE_DESC desc;
								HUDbits[index].texture->GetLevelDesc(0, &desc);
								HUDbits[index].scale.x = (float)JointInfo.GraphicsWidth/(float)desc.Width;
								HUDbits[index].scale.y = (float)JointInfo.GraphicsHeight/(float)desc.Height;
								HUDbits[index].position.x = 0;
								HUDbits[index].position.y = 0;
							}
							break;
						#pragma endregion
						#pragma region "e" : EnableHUD
						case 'e': //enable
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to enable HUD element\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Enable failed because HUD id does not exist\n");
							} else {
								if (HUDbits[index].Enabled) {
									DEBUGSTRING("MGE: Object already enabled\n");
								} else {
									UsedHUDbits++;
								}
								HUDbits[index].Enabled = true;
								DEBUGSTRING("MGE: Enable successful\n");
							}
							break;
						#pragma endregion
						#pragma region "d" : DisableHUD
						case 'd': //disable
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to disable HUD element\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Disable failed because HUD id does not exist\n");
							} else {
								if (!HUDbits[index].Enabled) {
									DEBUGSTRING("MGE: Object already disabled\n");
								} else {
									UsedHUDbits--;
								}
								HUDbits[index].Enabled = false;
								DEBUGSTRING("MGE: Disable successful\n");
							}
							break;
						#pragma endregion
						#pragma region "t" : ChangeHUDTexture
						case 't': //SetTexture
						{
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to texture HUD element\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Set texture failed because HUD id does not exist\n");
							}
							MWSE::GetString(CharPointer, &pointer);
							DEBUGSTRING("MGE: Texture name\n");
							DEBUGSTRING(CharPointer);
							if (index != -1) {
								int index2;
								if (CharPointer[0] == '\0') {
									DEBUGSTRING("MGE: Set texture failed. HUD elements must have a texture applied at all times.\n");
								} else if ((index2 = MWSE::TextureIndex(CharPointer, true)) == -1) {
									DEBUGSTRING("MGE: Set texture failed because texture could not be loaded\n");
								} else {
									if (HUDbits[index].texture != NULL) HUDbits[index].texture->Release();
									HUDbits[index].texture = LoadedTextures[index2].texture;
									HUDbits[index].texture->AddRef();
									DEBUGSTRING("MGE: Set texture successful\n");
								}
							}
							break;
						}
						#pragma endregion
						#pragma region "E" : ChangeHUDEffect
						case 'E': //SetEffect
						{
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to bind effect to object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Set effect failed because HUD id does not exist\n");
							}
							MWSE::GetString(CharPointer, &pointer);
							DEBUGSTRING("MGE: Effect name");
							DEBUGSTRING(CharPointer);
							if (index != -1) {
								int index2;
								if (CharPointer[0] != '\0' && (index2 = MWSE::ShaderIndex(CharPointer, true)) == -1) {
									DEBUGSTRING("MGE: Set effect failed because effect could not be loaded\n");
								} else {
									if (HUDbits[index].effect != NULL) HUDbits[index].effect->Release();
									if (CharPointer[0] == '\0') {
										HUDbits[index].effect = NULL;
									} else {
										HUDbits[index].effect = ShaderStruct.MWSEeffects[index2].effect;
										HUDbits[index].effect->AddRef();
									}
									DEBUGSTRING("MGE: Set effect successful\n");
								}
							}
							break;
						}
						#pragma endregion
						#pragma region "f" : FreeHUD
						case 'f': //Free sprite
							if (!UsingHUDbit) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveHUDbit);
							DEBUGSTRING("MGE: Trying to free HUD element\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::HUDIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("Free object failed because object id does not exist\n");
							} else {
								MWSE::FreeHUD(index);
							}
							break;
						#pragma endregion
						#pragma region "x" : DisableCrosshair
						case 'x': //Disable crosshair
							mwBridge->SetCrosshairEnabled(false);
							DEBUGSTRING("MGE: Crosshair disabled by a script\n");
							break;
						#pragma endregion
						#pragma region "X" : EnableCrosshair
						case 'X': //Enable crosshair
							mwBridge->SetCrosshairEnabled(true);
							DEBUGSTRING("MGE: Crosshair enabled by a script\n");
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: HUD command not recognised\n");
							break;
					}
					break;
				}
				#pragma endregion
				#pragma region "w" : Weather and lighting
				case 'w': {
					DEBUGSTRING("MGE: Recieved a weather control command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "m" : ManualLighting
						case 'm':
							SunManual = true;
							break;
						#pragma endregion
						#pragma region "M" : EndManualLighting
						case 'M':
							SunManual = false;
							break;
						#pragma endregion
						#pragma region "c" : SetSunColour
						case 'c':
							DEBUGSTRING("MGE: A script is manually setting the suns colour\n");
							SunManColour = true;
							SunColour.r = *(float*)(&PipeBuffer[pointer]);
							SunColour.g = *(float*)(&PipeBuffer[pointer+4]);
							SunColour.b = *(float*)(&PipeBuffer[pointer+8]);
							SunColour.a = 1;
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "C" : DefaultSunColour
						case 'C':
							SunManColour = false;
							break;
						#pragma endregion
						#pragma region "g" : GetSunColour
						case 'g':
							DEBUGSTRING("MGE: A script has requested the suns colour\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = SunColour.r;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = SunColour.g;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = SunColour.b;
							BytesToSend += 12;
							break;
						#pragma endregion
						#pragma region "d" : SetSunDirection
						case 'd':
							DEBUGSTRING("MGE: A script is manually setting the suns position\n");
							SunManDir = true;
							SunDirection.x = *(float*)(&PipeBuffer[pointer]);
							SunDirection.y = *(float*)(&PipeBuffer[pointer+4]);
							SunDirection.z = *(float*)(&PipeBuffer[pointer+8]);
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "D" : DefaultSunDirection
						case 'D':
							SunManDir = false;
							break;
						#pragma endregion
						#pragma region "G" : GetSunDir
						case 'G':
							DEBUGSTRING("MGE: A script has requested the suns direction\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = SunDirection.x;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = SunDirection.y;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = SunDirection.z;
							BytesToSend += 12;
							break;
						#pragma endregion
						#pragma region "t" : SetSunTexture
						case 't': {
							char FilePointer[256]; FilePointer[0] = '\0';
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Assigning a new texture to the sun\n");
							DEBUGSTRING(FilePointer);
							int index;
							if ((index = MWSE::TextureIndex(FilePointer, true)) != -1) {
								if (SunTexture) SunTexture->Release();
								SunTexture = LoadedTextures[index].texture;
								SunTexture->AddRef();
							} else {
								DEBUGSTRING("MGE: Could not use texture because it could not be loaded\n");
							}
							break;
						}
						#pragma endregion
						#pragma region "T" : DefaultSunTexture
						case 'T':
							SunTexture->Release();
							SunTexture = NULL;
							break;
						#pragma endregion
						#pragma region "o" : SetAddAmbientColour
						case 'o': {
							DEBUGSTRING("MGE: A script is manually setting the ambient colour\n");
							AmbientManual = true;
							D3DXCOLOR NewColour;
							NewColour.r = *(float*)(&PipeBuffer[pointer]);
							NewColour.g = *(float*)(&PipeBuffer[pointer+4]);
							NewColour.b = *(float*)(&PipeBuffer[pointer+8]);
							NewColour.a = 1;
							AmbientColour = NewColour;
							pointer += 12;
							RealDevice->SetRenderState(D3DRS_AMBIENT, AmbientColour);
							break;
						}
						#pragma endregion
						#pragma region "O" : DefaultAddAmbientColour
						case 'O':
							AmbientManual = false;
							break;
						#pragma endregion
						#pragma region "0" : GetAddAmbientColour
						case '0': {
							DEBUGSTRING("MGE: A script has requested the ambient colour\n");
							D3DXCOLOR NewColour = AmbientColour;
							*((float*)&PipeOutBuffer[BytesToSend]) = NewColour.r;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = NewColour.g;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = NewColour.b;
							BytesToSend += 12;
							break;
						}
						#pragma endregion
						#pragma region "b" : SetClsColour
						case 'b': {
							DEBUGSTRING("MGE: A script is manually setting the cls colour\n");
							ClsManual = true;
							D3DXCOLOR NewColour;
							NewColour.r = *(float*)(&PipeBuffer[pointer]);
							NewColour.g = *(float*)(&PipeBuffer[pointer+4]);
							NewColour.b = *(float*)(&PipeBuffer[pointer+8]);
							NewColour.a = 1;
							ClsColour = NewColour;
							pointer += 12;
							break;
						}
						#pragma endregion
						#pragma region "B" : DefaultClsColour
						case 'B':
							ClsManual = false;
							break;
						#pragma endregion
						#pragma region "L" : GetClsColour
						case 'L': {
							DEBUGSTRING("MGE: A script has requested the cls colour\n");
							D3DXCOLOR NewColour = ClsColour;
							*((float*)&PipeOutBuffer[BytesToSend]) = NewColour.r;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = NewColour.g;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = NewColour.b;
							BytesToSend += 12;
							break;
						}
						#pragma endregion
						#pragma region "s" : SetSkyColour
						case 's': {
							DEBUGSTRING("MGE: A script is manually setting the sky colour\n");
							SkyManual = true;
							D3DXCOLOR NewColour;
							NewColour.r = *(float*)(&PipeBuffer[pointer]);
							NewColour.g = *(float*)(&PipeBuffer[pointer+4]);
							NewColour.b = *(float*)(&PipeBuffer[pointer+8]);
							NewColour.a = 1;
							SkyColour = NewColour;
							pointer += 12;
							break;
						}
						#pragma endregion
						#pragma region "S" : DefaultSkyColour
						case 'S':
							SkyManual = false;
							break;
						#pragma endregion
						#pragma region "$" : GetSkyColour
						case '$': {
							DEBUGSTRING("MGE: A script has requested the sky colour\n");
							D3DXCOLOR NewColour = SkyColour;
							*((float*)&PipeOutBuffer[BytesToSend]) = NewColour.r;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = NewColour.g;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = NewColour.b;
							BytesToSend += 12;
							break;
						}
						#pragma endregion
						#pragma region "h" : SetHUDColour
						case 'h': {
							DEBUGSTRING("MGE: A script is manually setting the HUD colour\n");
							HUDManual = true;
							D3DXCOLOR NewColour;
							NewColour.r = *(float*)(&PipeBuffer[pointer]);
							NewColour.g = *(float*)(&PipeBuffer[pointer+4]);
							NewColour.b = *(float*)(&PipeBuffer[pointer+8]);
							NewColour.a = 1;
							HUDColour = NewColour;
							pointer += 12;
							break;
						}
						#pragma endregion
						#pragma region "H" : DefaultHUDColour
						case 'H':
							HUDManual = false;
							break;
						#pragma endregion
						#pragma region "#" : GetHUDColour
						case '#': {
							DEBUGSTRING("MGE: A script has requested the HUD colour\n");
							D3DXCOLOR NewColour = HUDColour;
							*((float*)&PipeOutBuffer[BytesToSend]) = NewColour.r;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = NewColour.g;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = NewColour.b;
							BytesToSend += 12;
							break;
						}
						#pragma endregion
						#pragma region "a" : SetAmbientColour
						case 'a': //SetAmbientLight
							DEBUGSTRING("MGE: A script is manually setting the ambient light colour\n");
							SunAmbManual = true;
							SunAmbColour.r = *(float*)(&PipeBuffer[pointer]);
							SunAmbColour.g = *(float*)(&PipeBuffer[pointer+4]);
							SunAmbColour.b = *(float*)(&PipeBuffer[pointer+8]);
							SunAmbColour.a = 1;
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "A" : DefaultAmbientColour
						case 'A': //DefaultAmbientLight
							SunAmbManual = false;
							break;
						#pragma endregion
						#pragma region "l" : GetAmbientColour
						case 'l': //GetAmbientLight
							DEBUGSTRING("MGE: A script has requested the ambient light colour\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = SunAmbColour.r;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = SunAmbColour.g;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = SunAmbColour.b;
							BytesToSend += 12;
							break;
						#pragma endregion
						#pragma region "i" : SetSunriseHour
						case 'i': //SetSunriseHour
							DEBUGSTRING("MGE: A script changes sunrise hour\n");
							mwBridge->SetSunriseHour( *(float*)(&PipeBuffer[pointer]) );
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "I" : GetSunriseHour
						case 'I': //GetSunriseHour
							DEBUGSTRING("MGE: A script requested sunrise hour\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = mwBridge->GetSunriseHour();
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "v" : SetSunsetHour
						case 'v': //SetSunsetHour
							DEBUGSTRING("MGE: A script changes sunset hour\n");
							mwBridge->SetSunsetHour( *(float*)(&PipeBuffer[pointer]) );
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "V" : GetSunsetHour
						case 'V': //GetSunsetHour
							DEBUGSTRING("MGE: A script requested sunset hour\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = mwBridge->GetSunsetHour();
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "j" : SetSunriseDuration
						case 'j': //SetSunriseDuration
							DEBUGSTRING("MGE: A script changes sunrise duration");
							mwBridge->SetSunriseDuration( *(float*)(&PipeBuffer[pointer]));
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "J" : GetSunriseDuration
						case 'J': //GetSunriseDuration
							DEBUGSTRING("MGE: A script requested sunrise duration\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = mwBridge->GetSunriseDuration();
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "u" : SetSunsetDuration
						case 'u': //SetSunsetDuration
							DEBUGSTRING("MGE: A script changes sunset duration\n");
							mwBridge->SetSunsetDuration( *(float*)(&PipeBuffer[pointer]));
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "U" : GetSunsetDuration
						case 'U': //GetSunsetDuration
							DEBUGSTRING("MGE: A script requested sunset duration\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = mwBridge->GetSunsetDuration();
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "e" : CellHasWeather
						case 'e': //CellHasWeather
							DEBUGSTRING("MGE: A script checks if cell has weather\n");
							*((WORD*)&PipeOutBuffer[BytesToSend]) = mwBridge->CellHasWeather();
							break;
						#pragma endregion
						#pragma region "n" : GetNewWeather
						case 'n': //GetNewWeather
							DEBUGSTRING("MGE: A script requested new weather type\n");
							*((WORD*)&PipeOutBuffer[BytesToSend]) = (WORD)mwBridge->GetNextWeather();
							BytesToSend += 2;
							break;
						#pragma endregion
						#pragma region "r" : GetNewWeatherRatio
						case 'r': //GetNewWeatherRatio
							DEBUGSTRING("MGE: A script requested new weather ratio\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = mwBridge->GetWeatherRatio();
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "q" : SetCloudTexture
						case 'q': {
							DEBUGSTRING("MGE: A script changes cloud texture\n");
							char TexName[256];
							int wthr = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::GetStringC(TexName, &pointer);
							mwBridge->SetWthrString(wthr, 0x104, TexName);
							break;
						}
						#pragma endregion
						#pragma region "Q" : GetCloudTexture
						case 'Q': //GetCloudTexture
							DEBUGSTRING("MGE: A script requested cloud texture name\n");
							BytesToSend += mwBridge->GetWthrString(*(WORD*)(&PipeBuffer[pointer]), 0x104, (char*)(&PipeOutBuffer[BytesToSend]));
							pointer += 2;
							break;
						#pragma endregion
						#pragma region "w" : SetWeatherData
						case 'w': { //SetWeatherData
							int wthr = *(short*)(&PipeBuffer[pointer]);
							DWORD addr = mwBridge->WthrStruc(wthr);
							int what = *(short*)(&PipeBuffer[pointer+2]);
							pointer += 4;
							if (what >= 0 && what <= 3) {
								//SetColors
								int when = *(short*)(&PipeBuffer[pointer]);
								RGBVECTOR col = *(RGBVECTOR*)(&PipeBuffer[pointer + 2]);
								pointer += 14;
								if (addr == 0 || when < 0 || (what < 3 && when == 4) || when > 4) break;
								RGBVECTOR* rgb = (RGBVECTOR*)(addr + 3 * ((what << 4) + when) + 0x14);
								*rgb = col;
								break;
							}
							if (what >= -7 && what < 0) {
								//SetFloatParams
								what = 1 - (what < -2 ? what - 56 : what);
								float* var = (float*)(addr + (what << 2));
								*var = *(float*)(&PipeBuffer[pointer]);
								pointer += 4;
								break;
							}
							if (what != 4) break;
							//SetAmbientSoundID
							char SndAmb[256];
							MWSE::GetStringC(SndAmb, &pointer);
							mwBridge->SetWthrString(wthr, 0x20E, SndAmb);
							break;
						}
						#pragma endregion
						#pragma region "W" : GetWeatherData
						case 'W': { //GetWeatherData
							int wthr = *(short*)(&PipeBuffer[pointer]);
							DWORD addr = mwBridge->WthrStruc(wthr);
							int what = *(short*)(&PipeBuffer[pointer+2]);
							pointer += 4;
							if (what >= 0 && what <= 3) {
								//GetColors
								int when = *(short*)(&PipeBuffer[pointer]);
								pointer += 2;
								RGBVECTOR* col = (RGBVECTOR*)(&PipeOutBuffer[BytesToSend]);
								BytesToSend += 12;
								*col = RGBVECTOR(-1, -1, -1);
								if (addr == 0 || when < 0 || (what < 3 && when == 4) || when > 4) break;
								*col = *(RGBVECTOR*)(addr + 3 * ((what << 4) + when) + 0x14);
								break;
							}
							if (what >= -7 && what < 0) {
								//GetFloatParams
								what = 1 - (what < -2 ? what - 56 : what);
								*(float*)(&PipeOutBuffer[BytesToSend]) = *(float*)(addr + (what << 2));
								BytesToSend += 4;
								break;
							}
							if (what != 4) break;
							//SetAmbientSoundID
							BytesToSend += mwBridge->GetWthrString(wthr, 0x20E, (char*)(&PipeOutBuffer[BytesToSend]));
							break;
						}
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Unrecognised weather command\n");
							break;
					}
					break;
				}
				#pragma endregion
				#pragma region "k" : Input (key)
				case 'k': {
					WORD code;
					DEBUGSTRING("MGE: Recieved a direct input control command\n");
					float f;
					switch (PipeBuffer[pointer++]) {
						#pragma region "t" : TapKey
						case 't':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::TapKey(code);
							break;
						#pragma endregion
						#pragma region "p" : PushKey
						case 'p':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::HoldKey(code);
							break;
						#pragma endregion
						#pragma region "r" : ReleaseKey
						case 'r':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::UnholdKey(code);
							break;
						#pragma endregion
						#pragma region "h" : HammerKey
						case 'h':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::HammerKey(code);
							break;
						#pragma endregion
						#pragma region "u" : UnhammerKey
						case 'u':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::UnhammerKey(code);
							break;
						#pragma endregion
						#pragma region "d" : DisallowKey
						case 'd':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::DisallowKey(code);
							break;
						#pragma endregion
						#pragma region "a" : AllowKey
						case 'a':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::AllowKey(code);
							break;
						#pragma endregion
						#pragma region "H" : AHammerKey
						case 'H':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::AHammerKey(code);
							break;
						#pragma endregion
						#pragma region "U" : AUnhammerKey
						case 'U':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::AUnhammerKey(code);
							break;
						#pragma endregion
						#pragma region "x" : MoveMouseRight
						case 'x':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::MoveMouseX(code);
							break;
						#pragma endregion
						#pragma region "X" : MoveMouseLeft
						case 'X':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::MoveMouseX(-code);
							break;
						#pragma endregion
						#pragma region "Y" : MoveMouseUp
						case 'y':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::MoveMouseY(code);
							break;
						#pragma endregion
						#pragma region "Y" : MoveMouseDown
						case 'Y':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::MoveMouseY(-code);
							break;
						#pragma endregion
						#pragma region "b" : BufferedKeyDown
						case 'b':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::BufferedDown(code);
							break;
						#pragma endregion
						#pragma region "B" : BufferedKeyUp
						case 'B':
							code = *(WORD*)(&PipeBuffer[pointer]);
							pointer += 2;
							MWSE::BufferedUp(code);
							break;
						#pragma endregion
						#pragma region "v" : SetMouseSpeedRight
						case 'v':
							f = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							MWSE::MouseSpeedX(f);
							break;
						#pragma endregion
						#pragma region "V" : SetMouseSpeedLeft
						case 'V':
							f = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							MWSE::MouseSpeedX(-f);
							break;
						#pragma endregion
						#pragma region "w" : SetMouseSpeedUp
						case 'w':
							f = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							MWSE::MouseSpeedY(f);
							break;
						#pragma endregion
						#pragma region "W" : SetMouseSpeedDown
						case 'W':
							f = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							MWSE::MouseSpeedY(-f);
							break;
						#pragma endregion
						#pragma region "m" : DisableMouse
						case 'm':
							MWSE::DisableMouse();
							break;
						#pragma endregion
						#pragma region "M" : EnableMouse
						case 'M':
							MWSE::EnableMouse();
							break;
						#pragma endregion
						#pragma region "c" : GetMousePos
						case 'c': {
							DEBUGSTRING("MGE: A script requested mouse cursor coordinates\n");
							POINT coord;
							GetCursorPos(&coord);
							*((short*)&PipeOutBuffer[BytesToSend]) = (short)coord.x;
							*((short*)&PipeOutBuffer[BytesToSend+4]) = (short)coord.y;
							BytesToSend += 4;
							break;
						}
						#pragma endregion
						default:
							DEBUGSTRING("MGE: recieved unknown input control command\n");
							break;
					}
					break;
				}
				#pragma endregion
				#pragma region "f" : Fog
				case 'f':   //Fogging
					DEBUGSTRING("MGE: Recieved fogging command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "d" : DisableFog
						case 'd':
							if (MGEFlags & FOG_ENABLED) ToggleFog();
							break;
						#pragma endregion
						#pragma region "e" : EnableFog
						case 'e':
							if (!(MGEFlags & FOG_ENABLED)) ToggleFog();
							break;
						#pragma endregion
						#pragma region "t" : ToggleFog
						case 't':
							ToggleFog();
							break;
						#pragma endregion
						#pragma region "i" : IncreaseFog
						case 'i':
							IncreaseFog();
							break;
						#pragma endregion
						#pragma region "o" : DecreaseFog
						case 'o':
							DecreaseFog();
							break;
						#pragma endregion
						#pragma region "I" : IncreaseFogBy
						case 'I':
							//SatDown(&FogRange, MINFOG, *(float*)(&PipeBuffer[pointer]));
							FogRange = CLAMP(FogRange-*(float*)(&PipeBuffer[pointer]), MINFOG, MAXFOG);
							UpdateFogRange();
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "O" : DecreaseFogBy
						case 'O':
							//SatUp(&FogRange, MAXFOG, *(float*)(&PipeBuffer[pointer]));
							FogRange = CLAMP(FogRange+*(float*)(&PipeBuffer[pointer]), MINFOG, MAXFOG);
							UpdateFogRange();
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "s" : SetFog
						case 's':
							FogRange = CLAMP(*(float*)(&PipeBuffer[pointer]), MINFOG, MAXFOG);
							UpdateFogRange();
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "z" : FogZoom
						case 'z':
							DEBUGSTRING("MGE: Begining fog zoom\n");
							FogZooming = true;
							FogZoomRate = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "Z" : StopFogZoom
						case 'Z':
							DEBUGSTRING("MGE: Ending fog zoom\n");
							FogZooming = false;
							break;
						#pragma endregion
						#pragma region "g" : GetFog
						case 'g':
							DEBUGSTRING("MGE: A script has requested the current fog\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = FogRange;
							BytesToSend += 4;
							break;
						#pragma endregion
						#pragma region "c" : SetFogColour
						case 'c': {
							DEBUGSTRING("MGE: A script is manually setting the fog colour\n");
							FogManual = true;
							D3DXCOLOR NewColour;
							NewColour.r = *(float*)(&PipeBuffer[pointer]);
							NewColour.g = *(float*)(&PipeBuffer[pointer+4]);
							NewColour.b = *(float*)(&PipeBuffer[pointer+8]);
							NewColour.a = 1;
							FogColour = NewColour;
							pointer += 12;
							RealDevice->SetRenderState(D3DRS_FOGCOLOR, FogColour);
							break;
						}
						#pragma endregion
						#pragma region "C" : DefaultFogColour
						case 'C':
							FogManual = false;
							break;
						#pragma endregion
						#pragma region "G" : GetFogColour
						case 'G': {
							DEBUGSTRING("MGE: A script has requested the current fog colour\n");
							D3DXCOLOR NewColour = FogColour;
							*((float*)&PipeOutBuffer[BytesToSend]) = NewColour.r;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = NewColour.g;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = NewColour.b;
							BytesToSend += 12;
							break;
						}
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Unknown fogging command\n");
							break;
					}
					break;
				#pragma endregion
				#pragma region "c" : Camera
				case 'c': //Camera
					DEBUGSTRING("MGE: Recieved a camera shake command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "e" : EnableCameraShake
						case 'e':
							DEBUGSTRING("MGE: Camera shake enabled\n");
							CameraShake = true;
							break;
						#pragma endregion
						#pragma region "d" : DisableCameraShake
						case 'd':
							DEBUGSTRING("MGE: Camera shake disabled\n");
							CameraShake = false;
							break;
						#pragma endregion
						#pragma region "s" : SetCameraShakeMagnitude
						case 's':
							DEBUGSTRING("MGE: Setting camera shake amplitude\n");
							CameraShakeAmp = CLAMP(*(float*)(&PipeBuffer[pointer]), MINSHAKE, MAXSHAKE);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "z" : CameraShakeZoom
						case 'z':
							DEBUGSTRING("MGE: Setting camera shake rate of change of amplitude\n");
							CameraShakeChange = true;
							CameraShakeChangeRate = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "Z" : StopCameraShakeZoom
						case 'Z':
							CameraShakeChange = false;
							break;
						#pragma endregion
						#pragma region "o" : Set3rdPCamOffset
						case 'o': {
							Cam3Offset.x = *(float*)(&PipeBuffer[pointer]);
							Cam3Offset.y = *(float*)(&PipeBuffer[pointer+4]);
							Cam3Offset.z = *(float*)(&PipeBuffer[pointer+8]);
							ForceCam3Offset = true;
							D3DXVECTOR3 * mwCam3Offset = mwBridge->PCam3Offset ();
							if (mwCam3Offset) {
								mwCam3Offset->x = Cam3Offset.x;
								mwCam3Offset->y = Cam3Offset.y;
								mwCam3Offset->z = Cam3Offset.z;
							}
							pointer += 12;
							break;
						}
						#pragma endregion
						#pragma region "O" : Get3rdPCamOffset
						case 'O':
							*((float*)&PipeOutBuffer[BytesToSend]) = Cam3Offset.x;
							*((float*)&PipeOutBuffer[BytesToSend+4]) = Cam3Offset.y;
							*((float*)&PipeOutBuffer[BytesToSend+8]) = Cam3Offset.z;
							BytesToSend += 12;
							break;
						#pragma endregion
						#pragma region "0" : DontForce3rdPCamOffset
						case '0':
							ForceCam3Offset = false;
							break;
						#pragma endregion
						#pragma region "f" : SetHorzFOV
						case 'f':
							ScreenFOV = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "F" : GetHorzFOV
						case 'F':
							*((float*)&PipeOutBuffer[BytesToSend]) = ScreenFOV;
							BytesToSend += 4;
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: recieved unknown camera shake command\n");
							break;
					}
					break;
				#pragma endregion
				#pragma region "s" : Screenshot
				case 's':   //Screenshot
					DEBUGSTRING("MGE: Recieved screenshot command\n");
					switch (PipeBuffer[pointer++]) {
						BYTE b;
						#pragma region "s" : TakeScreenshot
						case 0:
						case 's':
							TakeScreenshot();
							break;
						#pragma endregion
						#pragma region "0" : TakeBMPScreenShot
						case '0':
							b = SSFormat;
							SSFormat = 0;
							TakeScreenshot();
							SSFormat = b;
							break;
						#pragma endregion
						#pragma region "1" : TakeJPEGScreenShot
						case '1':
							b = SSFormat;
							SSFormat = 1;
							TakeScreenshot();
							SSFormat = b;
							break;
						#pragma endregion
						#pragma region "2" : TakeTGAScreenShot
						case '2':
							b = SSFormat;
							SSFormat = 2;
							TakeScreenshot();
							SSFormat = b;
							break;
						#pragma endregion
						#pragma region "4" : TakeDDSScreenshot
						case '4':
							b = SSFormat;
							SSFormat = 4;
							TakeScreenshot();
							SSFormat = b;
							break;
						#pragma endregion
						#pragma region "5" : TakePNGScreenshot
						case '5':
							b = SSFormat;
							SSFormat = 5;
							TakeScreenshot();
							SSFormat = b;
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Unknown screenshot command\n");
							break;
					}
					break;
				#pragma endregion
				#pragma region "F" : FPS counter
				case 'F':   //fps counter
					DEBUGSTRING("MGE: Recived fps counter command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "d" : DisableFPSCounter
						case 'd':
							BitReset(&MGEFlags, FPS_COUNTER_BIT);
							break;
						#pragma endregion
						#pragma region "e" : EnableFPSCounter
						case 'e':
							BitSet(&MGEFlags, FPS_COUNTER_BIT);
							break;
						#pragma endregion
						#pragma region "t" : ToggleFPSCounter
						case 't':
							BitCompliment(&MGEFlags, FPS_COUNTER_BIT);
							break;
						#pragma endregion
						#pragma region "g" : GetFPS
						case 'g':
							DEBUGSTRING("MGE: A script has requested the current fps\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = fps;
							BytesToSend += 4;
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Unknown fps counter command\n");
							break;
					}
					break;
				#pragma endregion
				#pragma region "r" : Screen rotation
				case 'r':
					DEBUGSTRING("MGE: Recived rotation command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "9" : RotateScreen
						case 0:
						case '9':
							RotateScreen();
							break;
						#pragma endregion
						#pragma region "0" : SetScreenRot0
						case '0':
							Rotation = 0;
							UpdateRotation();
							break;
						#pragma endregion
						#pragma region "1" : SetScreenRot90
						case '1':
							Rotation = PI/2;
							UpdateRotation();
							break;
						#pragma endregion
						#pragma region "2" : SetScreenRot180
						case '2':
							Rotation = PI;
							UpdateRotation();
							break;
						#pragma endregion
						#pragma region "3" : SetScreenRot270
						case '3':
							Rotation = (PI*2.0f/3.0f);
							UpdateRotation();
							break;
						#pragma endregion
						#pragma region "r" : RotateScreenBy
						case 'r':
							Rotation += *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							UpdateRotation();
							break;
						#pragma endregion
						#pragma region "R" : SetScreenRotation
						case 'R':
							Rotation = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							UpdateRotation();
							break;
						#pragma endregion
						#pragma region "s" : ScreenSpin
						case 's':
							DEBUGSTRING("MGE: Spinning screen\n");
							RotSpinning = true;
							RotSpinRate = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "S" : StopSpinSpin
						case 'S':
							DEBUGSTRING("MGE: Finished spinning screen\n");
							RotSpinning = false;
							break;
						#pragma endregion
						#pragma region "g" : GetScreenRotation
						case 'g':
							DEBUGSTRING("MGE: A script has requested the current rotation\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = Rotation;
							BytesToSend += 4;
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Unknown rotation command\n");
							break;
					}
					break;
				#pragma endregion
				#pragma region "z" : Zoom
				case 'z':   //zoom
					DEBUGSTRING("MGE: Recived zoom command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "d" : DisableZoom
						case 'd':
							Zoom = false;
							break;
						#pragma endregion
						#pragma region "e" : EnableZoom
						case 'e':
							Zoom = true;
							break;
						#pragma endregion
						#pragma region "t" : ToggleZoom
						case 't':
							Zoom = !Zoom;
							break;
						#pragma endregion
						#pragma region "i" : ZoomIn
						case 'i':
							IncreaseZoom();
							break;
						#pragma endregion
						#pragma region "o" : ZoomOut
						case 'o':
							DecreaseZoom();
							break;
						#pragma endregion
						#pragma region "I" : ZoomInBy
						case 'I':
							FOV = CLAMP(FOV+*(float*)(&PipeBuffer[pointer]), MINZOOM, MAXZOOM);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "O" : ZoomOutBy
						case 'O':
							FOV = CLAMP(FOV-*(float*)(&PipeBuffer[pointer]), MINZOOM, MAXZOOM);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "s" : SetZoom
						case 's':
							FOV = CLAMP(*(float*)(&PipeBuffer[pointer]), MINZOOM, MAXZOOM);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "z" : Zoom
						case 'z':
							DEBUGSTRING("MGE: Begining FOV zoom\n");
							FOVZooming = true;
							FOVZoomRate = *(float*)(&PipeBuffer[pointer]);
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "Z" : StopZoom
						case 'Z':
							DEBUGSTRING("MGE: Ending FOV zoom\n");
							FOVZooming = false;
							break;
						#pragma endregion
						#pragma region "g" : GetZoom
						case 'g':
							DEBUGSTRING("MGE: A script has requested the current zoom\n");
							*((float*)&PipeOutBuffer[BytesToSend]) = FOV;
							BytesToSend += 4;
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Unknown zoom command\n");
							break;
					}
					break;
				#pragma endregion
				#pragma region "t" : Textures
				case 't': {
					char FilePointer[256]; FilePointer[0] = '\0';
					int index;
					DEBUGSTRING("MGE: Recieved texture command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "w" : WipeTextures
						case 'w':
							DEBUGSTRING("MGE: Wiping all loaded textures\n");
							for (UINT i = 0; i < MAXMWSESHADERS; i++)
								if (LoadedTextures[i].texture != NULL) MWSE::FreeTexture(i);
							break;
						#pragma endregion
						#pragma region "l" : LoadTexture
						case 'l':
							DEBUGSTRING("MGE: Trying to load texture\n");
							MWSE::GetString(FilePointer, &pointer);
							MWSE::TextureIndex(FilePointer, true);
							break;
						#pragma endregion
						#pragma region "f" : FreeTexture
						case 'f':
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Trying to free texture\n");
							DEBUGSTRING(FilePointer);
							if ((index = MWSE::TextureIndex(FilePointer, false)) != -1) {
								MWSE::FreeTexture(index);
							} else {
								DEBUGSTRING("MGE: Could not free texture because it was not loaded\n");
							}
							break;
						#pragma endregion
						#pragma region "b" : BeginTexDraw
						case 'b': {
							D3DSURFACE_DESC desc;
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Trying to begin writing to texture");
							DEBUGSTRING(FilePointer);
							if (TexRenderTarget != NULL) {
								DEBUGSTRING("MGE: Write to texture failed because another texture is already being written to.\n");
								break;
							}
							if ((index = MWSE::TextureIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Could not write to texture because it could not be loaded\n");
								break;
							}
							if (LoadedTextures[index].texture->GetLevelDesc(0, &desc) != D3D_OK) {
								DEBUGSTRING("MGE: Could not get description of texture\n");
								break;
							}
							if (desc.Format != D3DFMT_X8R8G8B8) {
								DEBUGSTRING("MGE: Texture is not in the correct format\n");
								break;
							}
							if (RealDevice->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET,
								D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &TexRenderTarget, NULL) != D3D_OK) {
									DEBUGSTRING("MGE: Could not create render target\n");
									break;
							}
							TexRenderTarget->GetSurfaceLevel(0, &SufRenderTarget);
							RealDevice->SetRenderTarget(0, SufRenderTarget);
							TexCopyTo = LoadedTextures[index].texture;
							TexCopyTo->AddRef();
							TexCopyTo->GetSurfaceLevel(0, &SufCopyTo);
							RealDevice->BeginScene();
							ZeroStruct(TexDrawOrigin);
							TexDrawScale.x = 1;
							TexDrawScale.y = 1;
							TexDrawColour = 0xFFFFFFFF;
							RealDevice->StretchRect(SufCopyTo, 0, SufRenderTarget, NULL, D3DTEXF_NONE);
							DEBUGSTRING("MGE: Successfully began writing to texture\n"); 	  
							break;
						}
						#pragma endregion
						#pragma region "e" : EndTexDraw
						case 'e': //end write
							DEBUGSTRING("MGE: Stopping writing to texture\n");
							if (TexRenderTarget == NULL) {
								DEBUGSTRING("MGE: EndWrite failed because no texture was being written to\n");
								break;
							}
							RealDevice->EndScene();
							RealDevice->SetRenderTarget(0, SufRenderTarget);
							RealDevice->StretchRect(SufRenderTarget, NULL, SufCopyTo, NULL, D3DTEXF_NONE);
							SufRenderTarget->Release();
							TexRenderTarget->Release();
							SufCopyTo->Release();
							TexCopyTo->Release();
							TexRenderTarget = TexCopyTo = NULL;
							SufRenderTarget = SufCopyTo = NULL;
							break;
						#pragma endregion
						#pragma region "o" : SetTexDrawOrigin
						case 'o': //Origin
							DEBUGSTRING("MGE: Setting render origin\n");
							TexDrawOrigin.x = *(float*)&PipeBuffer[pointer];
							TexDrawOrigin.y = *(float*)&PipeBuffer[pointer+4];
							pointer += 8;
							break;
						#pragma endregion
						#pragma region "s" : SetTexDrawScale
						case 's': //Scale
							DEBUGSTRING("MGE: Setting render scale\n");
							TexDrawScale.x = *(float*)&PipeBuffer[pointer];
							TexDrawScale.y = *(float*)&PipeBuffer[pointer+4];
							pointer += 8;
							break;
						#pragma endregion
						#pragma region "c" : SetTexDrawColour
						case 'c': //Colour
							DEBUGSTRING("MGE: Setting texture render colour\n");
							TexDrawColour = *(DWORD*)&PipeBuffer[pointer];
							pointer += 4;
							break;
						#pragma endregion
						#pragma region "T" : DrawTextToTexture
						case 'T': {
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Drawing text to texture\n");
							DEBUGSTRING(FilePointer);
							RECT r;
							r.left = (DWORD)TexDrawOrigin.x;
							r.top = (DWORD)TexDrawOrigin.y;
							r.right = r.left+(DWORD)TexDrawScale.x;
							r.bottom = r.top+(DWORD)TexDrawScale.y;
							Font->DrawText(NULL, FilePointer, -1, &r, 0, TexDrawColour);
							break;
						}
						#pragma endregion
						#pragma region "t" : DrawTextureToTexture
						case 't': //Draw texture
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Drawing texture to texture\n");
							DEBUGSTRING(FilePointer);
							if ((index = MWSE::TextureIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Could load texture to be written\n");
								break;
							}
							D3DXMATRIX m;
							D3DXVECTOR3 v;
							v.x = TexDrawOrigin.x;
							v.y = TexDrawOrigin.y;
							v.z = 0;
							HUDsprite->SetTransform(D3DXMatrixScaling(&m, TexDrawScale.x, TexDrawScale.y, 1));
							HUDsprite->Draw(LoadedTextures[index].texture, 0, 0, &v, TexDrawColour);
							break;
						#pragma endregion
					}
					break;
				}
				#pragma endregion
				#pragma region "T" : Hooked textures
				case 'T': {
					char CharPointer[256];
					char Command = PipeBuffer[pointer++];
					MWSE::GetString(CharPointer, &pointer);
					DEBUGSTRING("MGE: Recieved a texture hook command\n");
					DEBUGSTRING(CharPointer);
					int index = MWSE_TextureHookIndex(CharPointer);
					if (index == -1) {
						DEBUGSTRING("MGE: texture hook command failed because video does not exist\n");
						break;
					}
					switch (Command) {
						#pragma region "a" : AutoFreeHook
						case 'a':
							if (HookedTextures[index].AutoClean) {
								DEBUGSTRING("MGE: Texture hook is already set to auto-free\n");
							} else {
								DEBUGSTRING("MGE: Texture hook will freed automatically\n");
								HookedTextures[index].AutoClean = true;
								HookedTextures[index].tHook->Release();
							}
							break;
						#pragma endregion
						#pragma region "d" : DontFreeHook
						case 'd':
							if (!HookedTextures[index].AutoClean) {
								DEBUGSTRING("MGE: Texture hook is already set to don't-free\n");
							} else {
								DEBUGSTRING("MGE: Texture hook will no longer be freed automatically\n");
								HookedTextures[index].AutoClean = false;
								HookedTextures[index].tHook->AddRef();
							}
							break;
						#pragma endregion
					}
				}
				break;
				#pragma endregion
				#pragma region "v" : Video
				case 'v':
					switch (PipeBuffer[pointer++]) {
						#pragma region "r" : ResumeVideo
						case 'v':
							break;
						#pragma endregion
						#pragma region "p" : PauseVideo
						case 'p':
							break;
						#pragma endregion
						#pragma region "s" : StopVideo
						case 's':
							break;
						#pragma endregion
						#pragma region "R" : RewindVideo
						case 'R':
							break;
						#pragma endregion
						#pragma region "S" : SeekVideo
						case 'S':
							break;
						#pragma endregion
					}
					break;
				#pragma endregion
				#pragma region "S" : Shaders
				case 'S': {
					char FilePointer[256]; FilePointer[0] = '\0';
					int index;
					DEBUGSTRING("MGE: Recieved hardware shader command\n");
					switch (PipeBuffer[pointer++]) {
						#pragma region "w" : WipeLoadedEffects
						case 'w':
							DEBUGSTRING("MGE: Wiping all loaded effects\n");
							for (UINT i = 0; i < MAXMWSESHADERS; i++) if (ShaderStruct.MWSEeffects[i].effect != NULL) MWSE::FreeEffect(i);
							break;
						#pragma endregion
						#pragma region "d" : DisableFullscreenShaders
						case 'd':
							BitReset(&MGEFlags, USE_HW_SHADER_BIT);
							break;
						#pragma endregion
						#pragma region "e" : EnableFullscreenShaders
						case 'e':
							BitSet(&MGEFlags, USE_HW_SHADER_BIT);
							break;
						#pragma endregion
						#pragma region "t" : ToggleFullscreenShaders
						case 't':
							BitCompliment(&MGEFlags, USE_HW_SHADER_BIT);
							break;
						#pragma endregion
						#pragma region "E" : UseEffectAsFullscreenShader
						case 'E':
							MWSE::GetString(FilePointer, &pointer);
							if ((index = MWSE::ShaderIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Could not enable shader because it could not be loaded\n");
							} else {
								if (ShaderStruct.MWSEeffects[index].Active) {
									DEBUGSTRING("MGE: Shader was already enabled\n");
								} else {
									ShaderStruct.MWSEeffects[index].Active = true;
									UsedMWSEShaders++;
								}
							}
							break;
						#pragma endregion
						#pragma region "D" : DisableEffectAsFullscreenShader
						case 'D':
							MWSE::GetString(FilePointer, &pointer);
							if ((index = MWSE::ShaderIndex(FilePointer, false)) == -1) {
								DEBUGSTRING("MGE: Could not disable shader because it was not loaded\n");
							} else {
								if (!ShaderStruct.MWSEeffects[index].Active) {
									DEBUGSTRING("MGE: Shader was already disabled\n");
								} else {
									ShaderStruct.MWSEeffects[index].Active = false;
									UsedMWSEShaders--;
								}
							}
							break;
						#pragma endregion
						#pragma region "l" : LoadEffect
						case 'l':
							MWSE::GetString(FilePointer, &pointer);
							MWSE::ShaderIndex(FilePointer, true);
							break;
						#pragma endregion
						#pragma region "s" : SetEffectTechnique
						case 's': {
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Trying to set technique\n");
							DEBUGSTRING(FilePointer);
							if ((index = MWSE::ShaderIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Couldn't set technique because effect could not be loaded\n");
							}
							MWSE::GetString(FilePointer, &pointer);   //Need to pop this string even if index == -1
							DEBUGSTRING("MGE: Trying to set technique\n");
							DEBUGSTRING(FilePointer);
							if (index != -1) {
								ShaderStruct.MWSEeffects[index].effect->SetTechnique(FilePointer);
								DEBUGSTRING("MGE: Technique set\n");
							}
							break;
						}
						#pragma endregion
						#pragma region "f" : FreeEffect
						case 'f': {
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Trying to free effect\n");
							DEBUGSTRING(FilePointer);
							if ((index = MWSE::ShaderIndex(FilePointer, false)) != -1) {
								MWSE::FreeEffect(index);
							} else {
								DEBUGSTRING("MGE: Could not free effect because it was not loaded\n");
							}
							break;
						}
						#pragma endregion
						#pragma region "L" : SetEffectVarLong
						case 'L': {
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Setting effect DWORD\n");
							DEBUGSTRING(FilePointer);
							if (FilePointer[0] == '\0') index = -2;
							else if ((index = MWSE::ShaderIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Could not set DWORD because effect could not be loaded\n");
							}
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Variable name\n");
							DEBUGSTRING(FilePointer);
							if (index >= 0) {
								DWORD d = *(DWORD*)&PipeBuffer[pointer];
								ShaderStruct.MWSEeffects[index].effect->SetInt(FilePointer, d);
							} else if (index == -2 && ShaderStruct.DefEffectCount) {
								DWORD d = *(DWORD*)&PipeBuffer[pointer];
								ShaderStruct.DefaultEffects[0].effect->SetInt(FilePointer, d);
							}
							pointer += 4;
							break;
						}
						#pragma endregion
						#pragma region "F" : SetEffectVarFloat
						case 'F': {
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Setting effect float\n");
							DEBUGSTRING(FilePointer);
							if (FilePointer[0] == '\0') index = -2;
							else if ((index = MWSE::ShaderIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Could not set float because effect could not be loaded\n");
							}
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Variable name");
							DEBUGSTRING(FilePointer);
							if (index >= 0) {
								float d = *(float*)&PipeBuffer[pointer];
								ShaderStruct.MWSEeffects[index].effect->SetFloat(FilePointer, d);
							} else if (index == -2 && ShaderStruct.DefEffectCount) {
								float d = *(float*)&PipeBuffer[pointer];
								ShaderStruct.DefaultEffects[0].effect->SetFloat(FilePointer, d);
							}
							pointer += 4;
							break;
						}
						#pragma endregion
						#pragma region "V" : SetEffectVarVector
						case 'V': {
							MWSE::GetString(FilePointer, &pointer);
							D3DXVECTOR4 vec;
							DEBUGSTRING("MGE: Setting effect vector\n");
							DEBUGSTRING(FilePointer);
							if (FilePointer[0] == '\0') index = -2;
							else if ((index = MWSE::ShaderIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Could not set vector because effect could not be loaded\n");
							}
							MWSE::GetString(FilePointer, &pointer);
							DEBUGSTRING("MGE: Variable name\n");
							DEBUGSTRING(FilePointer);
							if (index >= 0) {
								vec.x = *(float*)&PipeBuffer[pointer];
								vec.y = *(float*)&PipeBuffer[pointer+4];
								vec.z = *(float*)&PipeBuffer[pointer+8];
								vec.w = *(float*)&PipeBuffer[pointer+12];
								ShaderStruct.MWSEeffects[index].effect->SetVector(FilePointer, &vec);
							} else if (index == -2 && ShaderStruct.DefEffectCount) {
								vec.x = *(float*)&PipeBuffer[pointer];
								vec.y = *(float*)&PipeBuffer[pointer+4];
								vec.z = *(float*)&PipeBuffer[pointer+8];
								vec.w = *(float*)&PipeBuffer[pointer+12];
								ShaderStruct.DefaultEffects[0].effect->SetVector(FilePointer, &vec);
							}
							pointer += 16;
							break;
						}
						#pragma endregion
						#pragma region "T" : SetEffectVarTexture
						case 'T': {
							MWSE::GetString(FilePointer, &pointer);
							char texture[256];
							DEBUGSTRING("MGE: Setting effect texture\n");
							DEBUGSTRING(FilePointer);
							if (FilePointer[0] == '\0') index = -2;
							else if ((index = MWSE::ShaderIndex(FilePointer, true)) == -1) {
								DEBUGSTRING("MGE: Could not set texture because effect could not be loaded\n");
							}
							MWSE::GetString(FilePointer, &pointer);
							MWSE::GetString(texture, &pointer);
							DEBUGSTRING("MGE: Variable name\n");
							DEBUGSTRING(FilePointer);
							DEBUGSTRING("MGE: Texture path\n");
							DEBUGSTRING(texture);
							if (index >= 0) {
								DWORD index2;
								if ((index2 = MWSE::TextureIndex(texture, true)) == -1) {
									DEBUGSTRING("MGE: Set texture failed because texture could not be loaded\n");
								} else {
									ShaderStruct.MWSEeffects[index].effect->SetTexture(FilePointer, LoadedTextures[index2].texture);
								}
							} else if (index == -2 && ShaderStruct.DefEffectCount) {
								DWORD index2;
								if ((index2 = MWSE::TextureIndex(texture, true)) == -1) {
									DEBUGSTRING("MGE: Set texture failed because texture could not be loaded\n");
								} else {
									ShaderStruct.DefaultEffects[0].effect->SetTexture(FilePointer, LoadedTextures[index2].texture);
								}
							}
							break;
						}
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Unknown hardware shader command\n");
							break;
					}
					break;
				}
				#pragma endregion
				#pragma region "m" : Model
				case 'm': //Model command
					DEBUGSTRING("MGE: Recieved MGE Object command\n");
					char CharPointer[256]; CharPointer[0] = '\0';
					switch (PipeBuffer[pointer++]) {
						int index;
						#pragma region "w" : WipeLoadedMeshes
						case 'w':  //wipe all loaded models
							DEBUGSTRING("MGE: Unloading all loaded models\n");
							for (UINT i = 0; i < MAXMWSEOBJECTS; i++) if (Objects[i].Created) MWSE::FreeModel(i);
							break;
						#pragma endregion
						#pragma region "W" : WithMesh
						case 'W':  //with model do
							MWSE::GetString(CharPointer, &pointer);
							DEBUGSTRING("MGE: All following model commands will refer to object\n");
							DEBUGSTRING(CharPointer);
							if (strlen(CharPointer) > 32) {
								DEBUGSTRING("MGE: 'mW' failed because object id was longer than 32 characters\n");
							} else {
								strcpy_s(ActiveModel, 33, CharPointer);
								UsingModel = true;
							}
							break;
						#pragma endregion
						#pragma region "c" : CancelWithMesh
						case 'c': //continue
							DEBUGSTRING("MGE: All following model commands must specify their own object\n");
							UsingModel = false;
							break;
						#pragma endregion
						#pragma region "l" : LoadMesh
						case 'l': {
							char FileName[256];
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							MWSE::GetString(FileName, &pointer);
							DEBUGSTRING("MGE: Trying to assign id to file\n");
							DEBUGSTRING(CharPointer);
							DEBUGSTRING(FileName);
							MWSE::ObjectIndex(CharPointer, FileName);
							break;
						}
						#pragma endregion
						#pragma region "e" : EnableMesh
						case 'e': //enable
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to enable object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Enable failed because object id does not exist\n");
							} else {
								if (Objects[index].Enabled) { DEBUGSTRING("MGE: Object already enabled\n"); }
								Objects[index].Enabled = true;
								DEBUGSTRING("MGE: Enable successful\n");
							}
							break;
						#pragma endregion
						#pragma region "d" : DisableMesh
						case 'd': //disable
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to disable object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Disable failed because object id does not exist\n");
							} else {
								if (!Objects[index].Enabled) { DEBUGSTRING("MGE: Object already disabled\n"); }
								Objects[index].Enabled = false;
								DEBUGSTRING("MGE: Disable successful\n");
							}
							break;
						#pragma endregion
						#pragma region "p" : SetMeshPosition
						case 'p': //Set position
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to set position of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: SetPos failed because object id does not exist\n");
							} else {
								Objects[index].pos.x = *(float*)(&PipeBuffer[pointer]);
								Objects[index].pos.y = *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].pos.z = *(float*)(&PipeBuffer[pointer+8]);
								MWSE::UpdateObjectMatrix(index, 0);
								DEBUGSTRING("MGE: SetPos successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "r" : SetMeshRotation
						case 'r': //Set yaw pitch roll
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to set rotation of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: SetRot failed because object id does not exist\n");
							} else {
								Objects[index].rot.x = *(float*)(&PipeBuffer[pointer]);
								Objects[index].rot.y = *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].rot.z = *(float*)(&PipeBuffer[pointer+8]);
								MWSE::UpdateObjectMatrix(index, 1);
								DEBUGSTRING("MGE: SetRot successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "s" : SetMeshScale
						case 's': //Scale
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to scale object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: SetScale failed because object id does not exist\n");
							} else {
								Objects[index].scale.x = *(float*)(&PipeBuffer[pointer]);
								Objects[index].scale.y = *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].scale.z = *(float*)(&PipeBuffer[pointer+8]);
								MWSE::UpdateObjectMatrix(index, 2);
								DEBUGSTRING("MGE: SetScale successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "M" : MoveMesh
						case 'M': //Move
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to move object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Move failed because object id does not exist\n");
							} else {
								Objects[index].pos.x += *(float*)(&PipeBuffer[pointer]);
								Objects[index].pos.y += *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].pos.z += *(float*)(&PipeBuffer[pointer+8]);
								MWSE::UpdateObjectMatrix(index, 0);
								DEBUGSTRING("MGE: Move successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "R" : RotateMesh
						case 'R': //rotate
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to rotate object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Rotate failed because object id does not exist\n");
							} else {
								Objects[index].rot.x += *(float*)(&PipeBuffer[pointer]);
								Objects[index].rot.y += *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].rot.z += *(float*)(&PipeBuffer[pointer+8]);
								MWSE::UpdateObjectMatrix(index, 1);
								DEBUGSTRING("MGE: Rotate successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "S" : ScaleMesh
						case 'S': //scale
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to scale object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Scale failed because object id does not exist\n");
							} else {
								Objects[index].scale.x += *(float*)(&PipeBuffer[pointer]);
								Objects[index].scale.y += *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].scale.z += *(float*)(&PipeBuffer[pointer+8]);
								MWSE::UpdateObjectMatrix(index, 2);
								DEBUGSTRING("MGE: Scale successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "v" : SetMeshVelocity
						case 'v':
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to set velocity of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: SetVelocity failed because object id does not exist\n");
							} else {
								Objects[index].speed.x = *(float*)(&PipeBuffer[pointer]);
								Objects[index].speed.y = *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].speed.z = *(float*)(&PipeBuffer[pointer+8]);
								Objects[index].SpeedFlags |= 0x01;
								DEBUGSTRING("MGE: SetVelocity successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "a" : SetMeshAngularVelocity
						case 'a':
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to set angular velocity of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: SetAngularVelocity failed because object id does not exist\n");
							} else {
								Objects[index].AngSpeed.x = *(float*)(&PipeBuffer[pointer]);
								Objects[index].AngSpeed.y = *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].AngSpeed.z = *(float*)(&PipeBuffer[pointer+8]);
								Objects[index].SpeedFlags |= 0x02;
								DEBUGSTRING("MGE: SetAngularVelocity successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "g" : SetMeshGrowth
						case 'g':
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to set growth of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: SetGrowth failed because object id does not exist\n");
							} else {
								Objects[index].growth.x = *(float*)(&PipeBuffer[pointer]);
								Objects[index].growth.y = *(float*)(&PipeBuffer[pointer+4]);
								Objects[index].growth.z = *(float*)(&PipeBuffer[pointer+8]);
								Objects[index].SpeedFlags |= 0x04;
								DEBUGSTRING("MGE: SetGrowth successful\n");
							}
							pointer += 12;
							break;
						#pragma endregion
						#pragma region "V" : ZeroMeshVelocity
						case 'V':
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to stop movement of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: StopMovement failed because object id does not exist\n");
							} else {
								Objects[index].speed.x = 0;
								Objects[index].speed.y = 0;
								Objects[index].speed.z = 0;
								Objects[index].SpeedFlags &= 0xFFFFFFFE;
								DEBUGSTRING("MGE: StopMovement successful\n");
							}
							break;
						#pragma endregion
						#pragma region "A" : ZeroMeshAngularVelocity
						case 'A':
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to stop rotation of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: StopRotation failed because object id does not exist\n");
							} else {
								Objects[index].AngSpeed.x = 0;
								Objects[index].AngSpeed.y = 0;
								Objects[index].AngSpeed.z = 0;
								Objects[index].SpeedFlags &= 0xFFFFFFFD;
								DEBUGSTRING("MGE: StopRotation successful\n");
							}
							break;
						#pragma endregion
						#pragma region "G" : ZeroMeshGrowth
						case 'G':
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to stop growth of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: StopGrowth failed because object id does not exist\n");
							} else {
								Objects[index].growth.x = 0;
								Objects[index].growth.y = 0;
								Objects[index].growth.z = 0;
								Objects[index].SpeedFlags &= 0xFFFFFFFB;
								DEBUGSTRING("MGE: StopGrowth successful\n");
							}
							break;
						#pragma endregion
						#pragma region "t" : SetMeshTexture
						case 't': {
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to texture object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Set texture failed because object id does not exist\n");
							}
							MWSE::GetString(CharPointer, &pointer);
							DEBUGSTRING("MGE: Texture name");
							DEBUGSTRING(CharPointer);
							if (index != -1) {
								int index2;
								if (CharPointer[0] != '\0' && (index2 = MWSE::TextureIndex(CharPointer, true)) == -1) {
									DEBUGSTRING("MGE: Set texture failed because texture could not be loaded\n");
								} else {
									WORD d = *(WORD*)(&PipeBuffer[pointer]);
									if (d >= MAXMWSESUBSETS) {
										DEBUGSTRING("MGE: Set texture failed because texture stage was out of range\n");
									} else {
										if (Objects[index].textures[d] != NULL) Objects[index].textures[d]->Release();
										if (CharPointer[0] == '\0') {
											Objects[index].textures[d] = NULL;
										} else {
											Objects[index].textures[d] = LoadedTextures[index2].texture;
											Objects[index].textures[d]->AddRef();
										}
										DEBUGSTRING("MGE: Set texture successful\n");
									}
								}
							}
							pointer += 2;
							break;
						}
						#pragma endregion
						#pragma region "E" : SetMeshEffect
						case 'E': {
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to bind effect to object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Set effect failed because object id does not exist\n");
							}
							MWSE::GetString(CharPointer, &pointer);
							DEBUGSTRING("MGE: Effect name\n");
							DEBUGSTRING(CharPointer);
							if (index != -1) {
								int index2;
								if (CharPointer[0] != '\0' && (index2 = MWSE::ShaderIndex(CharPointer, true)) == -1) {
									DEBUGSTRING("MGE: Set effect failed because effect could not be loaded\n");
								} else {
									if (Objects[index].effect != NULL) Objects[index].effect->Release();
									if (CharPointer[0] == '\0') {
										Objects[index].effect = NULL;
									} else {
										Objects[index].effect = ShaderStruct.MWSEeffects[index2].effect;
										Objects[index].effect->AddRef();
									}
									DEBUGSTRING("MGE: Set effect successful\n");
								}
							}
							break;
						}
						#pragma endregion
						#pragma region "D" : ManualMesh
						case 'D': //Destroy mesh
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to destroy mesh of object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("MGE: Destroy mesh failed because object id does not exist\n");
							} else if (!Objects[index].IsMesh) {
								DEBUGSTRING("MGE: Destroy mesh failed because object is not a mesh\n");
							} else {
								Objects[index].FVF = Objects[index].mesh->GetFVF();
								Objects[index].faces = Objects[index].mesh->GetNumFaces();
								Objects[index].stride = D3DXGetFVFVertexSize(Objects[index].FVF);
								Objects[index].vertices = Objects[index].mesh->GetNumVertices();
								Objects[index].mesh->GetVertexBuffer(&Objects[index].vBuffer);
								Objects[index].mesh->GetIndexBuffer(&Objects[index].iBuffer);
								Objects[index].IsMesh = false;
								DEBUGSTRING("MGE: Destroy mesh successful\n");
							}
							break;
						#pragma endregion
						#pragma region "f" : FreeMesh
						case 'f': //Free mesh
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to free object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("Free object failed because object id does not exist\n");
							} else {
								MWSE::FreeModel(index);
							}
							break;
						#pragma endregion
						#pragma region "u" : UpdateMeshLighting
						case 'u': //Update lighting
							if (!UsingModel) MWSE::GetString(CharPointer, &pointer);
							else strcpy_s(CharPointer, 256, ActiveModel);
							DEBUGSTRING("MGE: Trying to update lighting on object\n");
							DEBUGSTRING(CharPointer);
							if ((index = MWSE::ObjectIndex(CharPointer, NULL)) == -1) {
								DEBUGSTRING("Update lights failed because object id does not exist\n");
							} else {
								UpdateLighting(index);
								DEBUGSTRING("MGE: Update lighting sucessful\n");
							}
							break;
						#pragma endregion
						default:
							DEBUGSTRING("MGE: Object command not recognised\n");
							break;
						}
						break;
				#pragma endregion
				default:
					DEBUGSTRING("MGE: Unknown command recieved from MWSE\n");
					break;
			}
		}
		if (BytesToSend) {
			WriteFile(MWSEpipe, PipeOutBuffer, BytesToSend, &size, NULL);
		}
	}
}

//-----------------------------------------------------------------------------
//DirectInput
//-----------------------------------------------------------------------------

void MWSE::TapKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    TapStates[keycode]=0x80;
}

//-----------------------------------------------------------------------------

void MWSE::HoldKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    FakeStates[keycode]=0x80;
}

//-----------------------------------------------------------------------------

void MWSE::UnholdKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    FakeStates[keycode]=0x00;
}
void MWSE::HammerKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    HammerStates[keycode]=0x80;
}

//-----------------------------------------------------------------------------

void MWSE::UnhammerKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    HammerStates[keycode]=0x00;
}
void MWSE::AHammerKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    AHammerStates[keycode]=0x80;
}

//-----------------------------------------------------------------------------

void MWSE::AUnhammerKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    AHammerStates[keycode]=0x00;
}
void MWSE::DisallowKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    DisallowStates[keycode]=0x00;
}

//-----------------------------------------------------------------------------

void MWSE::AllowKey(WORD keycode) {
    ISKEYCODEVALID(keycode);
    DisallowStates[keycode]=0x80;
}

//-----------------------------------------------------------------------------

void MWSE::MoveMouseX(WORD pixels) {
    MouseXMov+=pixels;
}
void MWSE::MoveMouseY(WORD pixels) {
    MouseYMov+=pixels;
}

//-----------------------------------------------------------------------------

void MWSE::MouseSpeedX(float speed) {
    MouseXSpeed=speed;
}
void MWSE::MouseSpeedY(float speed) {
    MouseYSpeed=speed;
}

//-----------------------------------------------------------------------------

void MWSE::DisableMouse() {
    MouseDisable=true;
}

//-----------------------------------------------------------------------------

void MWSE::EnableMouse() {
    MouseDisable=false;
}

//-----------------------------------------------------------------------------

void MWSE::BufferedDown(WORD keycode) {
    KEYBOARDVALID(keycode);
    BYTE data=0x80;
    BYTE chr=(BYTE)keycode;
    FakeString(&chr,&data,1);
}

//-----------------------------------------------------------------------------

void MWSE::BufferedUp(WORD keycode) {
    KEYBOARDVALID(keycode);
    BYTE data=0x00;
    BYTE chr=(BYTE)keycode;
    FakeString(&chr,&data,1);
}

//-----------------------------------------------------------------------------
