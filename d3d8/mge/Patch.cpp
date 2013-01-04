#include "ModuleInformation.h"
#include "../log.h"
#include <stdlib.h>
#define _PATCH
#include "Patch.h"

//-----------------------------------------------------------------------------
// Sequence class
//-----------------------------------------------------------------------------

Sequence::Sequence(const void *address, const char *string, Sequence *next) : 
	Address((void*)address),
	String(strcpy(new char[strlen(string)+1], string)),
	Next(next) {}

Sequence::~Sequence() {
	Address = NULL;
	delete[] String;
	delete Next;
}

//-----------------------------------------------------------------------------
// PatchTree class
//-----------------------------------------------------------------------------

PatchTree::PatchTree(const char *section) :
	Next(NULL), Child(NULL), Section(section?strcpy(new char[strlen(section)+1], section):NULL), Parent(NULL),
	Checked(NULL), Removed(NULL), Version(NULL), FileVersion(NULL), Original(NULL), Patch(NULL), Attach(NULL) {}

PatchTree::~PatchTree() {
	delete Next;
	delete Child;
	delete[] Section;
	delete[] Parent;
	delete Checked;
	delete Removed;
	delete Version;
	delete FileVersion;
	delete Original;
	delete Patch;
	Sequence *current = Attach;
	while(current) { delete[] (char*)current->Address; current = current->Next; }
	delete Attach;
}

PatchTree* PatchTree::BuildPatchTree() {
	Sequence *files = FindAllFiles(new Sequence(NULL, PATCH_SETTINGS, NULL), PATCH_PATH, PATCH_MASK);
	if(!files) return NULL;
	char *sections = new char[MAX_LINE];
	PatchTree *tree = NULL;
	Sequence *current = files;
	do {
		GetPrivateProfileSectionNames(sections, MAX_LINE, current->String);
		char *name = sections;
		while(*name != '\0') {
			PatchTree *patch;
			if((patch = new PatchTree(name))->ReadSection(current->String, name) ? patch->UpdateTree(&tree) : true)
				delete patch;
			name += strlen(name) + 1;
		}
		current = current->Next;
	}while(current);
	delete files;
	delete[] sections;
	return tree;
}

Sequence* PatchTree::FindAllFiles(Sequence *files, const char *startPath, const char *extensionMask) {
	char *path = strcat(strcpy(new char[MAX_LINE], startPath), "\\");
	int length = strlen(path);
	if(extensionMask) strcat(path, extensionMask);
	WIN32_FIND_DATA *fd = new WIN32_FIND_DATA;
	for(int i=0; i<2; i++) {
		if(i) strcat(path, "*.*");
		HANDLE fff = FindFirstFile(path, fd);
		if(fff!=INVALID_HANDLE_VALUE) {
			do {
				if(!(strcmp(fd->cFileName, ".") && strcmp(fd->cFileName, ".."))) continue;
				if(!i||fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					path[length] = '\0';
					strcat(path, fd->cFileName);
					files = i?FindAllFiles(files, path, extensionMask):new Sequence(NULL, path, files);
				}
			}while(FindNextFile(fff, fd));
			FindClose(fff);
		}
		path[length] = '\0';
	}
	delete fd;
	delete[] path;
	return files;
}

char* PatchTree::RemoveWhitespace(char *string) {
	int position = 0;
	int length = strlen(string);
	for(int i=0; i<length; i++) {
		if(isspace(string[i]) == 0) {
			string[position] = string[i];
			position++;
		}
	}
	string[position] = '\0';
	return string;
}

Sequence* PatchTree::SetMachineCode(const char *mcpFileName, const char *section, const char *prefix, char *line) {
	Sequence *code = NULL;
	char *key  = new char[MAX_LINE];
	GetPrivateProfileString(section, NULL, NULL, line, MAX_LINE, mcpFileName);
	char *keyname = line;
	int length = strlen(prefix);
	while(*keyname != '\0') {
		int keylen = strlen(keyname);
		if(keylen > length && _strnicmp(keyname, prefix, length) == 0) {
			GetPrivateProfileString(section, keyname, NULL, key, MAX_LINE, mcpFileName);
			char *name = &keyname[length];
			while(isspace(*name) && strlen(name)) name++;
			LPVOID address = _stricmp(PK_ATTACH, prefix) ? (void*)strtol(name, NULL, 16) : strcpy(new char[strlen(name)+1], name);
			if(strlen(RemoveWhitespace(key))>0) code = new Sequence(address, _strlwr(key), code);
		}
		keyname += keylen + 1;
	}
	delete[] key;
	return code;
}

bool PatchTree::UpdateSection(PatchTree **section) {
	PatchTree *existing = (*section);
	if(existing == this) return false;
	if(Version == NULL) return true;
	bool update = existing->Version == NULL;
	if (!update) {
		if (*Version <= *existing->Version) return true;
		if (Original == NULL && Patch == NULL && Attach == NULL) {
			bool *exchange; // for memory leak prevention
			exchange = existing->Checked;
			existing->Checked = Checked;
			Checked = exchange;
			exchange = existing->Removed;
			existing->Removed = new bool ((Removed ? *Removed : false) || (exchange ? *exchange : false));
			delete exchange;
			return true;
		}
		update = true; // typical section
	}
	if (update) {
		Next = existing->Next;
		existing->Next = NULL;
		Child = existing->Child;
		existing->Child = NULL;
		(*section) = this;
		delete existing;
		return false;
	}
	return true;
}

bool PatchTree::ReadSection(const char *mcpFileName, const char *section) {
	char *line = new char[MAX_LINE];
	GetPrivateProfileString(section, PK_PARENT, NULL, line, MAX_LINE, mcpFileName);
	if(strlen(line)>0) Parent = strcpy(new char[strlen(line)+1], line);
	GetPrivateProfileString(section, PK_CHECKED, NULL, line, MAX_LINE, mcpFileName);
	if(strlen(line)>0) Checked = new bool (_stricmp(line, "True")==0||atof(line)==1);
	GetPrivateProfileString(section, PK_REMOVED, NULL, line, MAX_LINE, mcpFileName);
	if(strlen(line)>0) Removed = new bool (_stricmp(line, "True")==0||atof(line)==1);
	GetPrivateProfileString(section, PK_VERSION, NULL, line, MAX_LINE, mcpFileName);
	if(strlen(line)>0) Version = new float ((float)atof(line)); else Version = new float (1);
	GetPrivateProfileString(section, PK_FILEVERSION, NULL, line, MAX_LINE, mcpFileName);
	if(strlen(line)>0) FileVersion = new unsigned (atoi(line));
	Original = SetMachineCode(mcpFileName, section, PK_ORIGINAL, line);
	Patch = SetMachineCode(mcpFileName, section, PK_PATCH, line);
	Attach = SetMachineCode(mcpFileName, section, PK_ATTACH, line);
	delete[] line;
	return Parent || FileVersion || Checked || Removed || Version || Original || Patch || Attach;
}

bool PatchTree::UpdateTree(PatchTree **tree) {
	PatchTree **child = tree;
	bool getdelete = false;
	char *line = strcpy(new char[MAX_LINE], "");
	if(Parent) line[strlen(strcat(line, Parent))-1]!='/'?strcat(line, "/"):strlen(line)==1?strcpy(line, ""):line;
	strcat(line, Section);
	char *path, *start, *end;
	end = start = path = strcpy(new char[strlen(line)+1], line);
	while((end = strstr(end, "/"))||start) {	
		strncpy_s(line, MAX_LINE, start, end ? end++ - start : strlen(start));
		start = end;
		if(!(*child)) (*child) = start ? new PatchTree(line) : this;
		PatchTree **next = child;
		do {
			if(_stricmp((*next)->Section, line)==0) {// If already exists.
				if(start) child = &(*next)->Child;
				else getdelete = UpdateSection(next);// Updating current section.
				break;
			}
			next = &(*next)->Next;
		}while(*next);
		if (!(*next)) {
			(*next) = start ? new PatchTree(line) : this;
			child = &(*next)->Child;
		}
		if(!start) break;
	}
	delete[] path;
	delete[] line;
	return getdelete;
}

//-----------------------------------------------------------------------------
// MachineCode class
//-----------------------------------------------------------------------------

bool MachineCode::CompareCode(void *address, BYTE *code, int length) {
	DWORD oldProtect;
	bool equal = true;
	equal = VirtualProtect (address, length, PAGE_READWRITE, &oldProtect) ? equal : false;
	for(int i=0; i<length; i++) {
		if(code[i] != *reinterpret_cast<BYTE*>((DWORD)address + i)) {
			equal = false;
			break;
		}
	}
	equal = VirtualProtect (address, length, oldProtect, &oldProtect) ? equal : false;
	return equal;
}

bool MachineCode::SetCode(void *address, BYTE *code, int length) {
	DWORD oldProtect;
	bool success = true;
	success = VirtualProtect (address, length, PAGE_READWRITE, &oldProtect) ? success : false;
	for(int i=0; i<length; i++) *reinterpret_cast<BYTE*>((DWORD)address + i) = code[i];
	success = VirtualProtect (address, length, oldProtect, &oldProtect) ? success : false;
	return success;
}

bool MachineCode::GetCode(void *address, BYTE *code, int length) {
	if(code == NULL || length <= 0) return false;
	DWORD oldProtect;
	bool success = true;
	success = VirtualProtect (address, length, PAGE_READWRITE, &oldProtect) ? success : false;
	for(int i=0; i<length; i++) code[i] = *reinterpret_cast<BYTE*>((DWORD)address + i);
	success = VirtualProtect (address, length, oldProtect, &oldProtect) ? success : false;
	return success;
}

DWORD MachineCode::GetLabelOffset(void *offsetPlacementAddress, void *labelAddress) {
	return (DWORD)labelAddress - (DWORD)offsetPlacementAddress - sizeof(DWORD);
}

DWORD MachineCode::GetAlignLength(DWORD codeSize, DWORD segmentSize) {
	return segmentSize - (codeSize % segmentSize);
}

bool MachineCode::CompareAndPatchByJumpToAttachment(void *address, BYTE *original, int length, BYTE *attachment, int size) {
	if(length >= 5 && size > 0 && CompareCode(address, original, length)) {
		DWORD dwAddress;		// Applying the patch:
		if (VirtualProtect(attachment, size, PAGE_EXECUTE_READWRITE, &dwAddress)) {
			dwAddress = 0xE9;	// Jmp opcode
			bool result = SetCode(address, (LPBYTE)&dwAddress, 1);
			dwAddress = GetLabelOffset(LPVOID(DWORD(address) + 1), attachment);
			result = SetCode(LPVOID(DWORD(address) + 1), (LPBYTE)&dwAddress, sizeof(DWORD)) ? result : false;
			dwAddress = 0x90;	// Nop
			for(int i = 5; i < length; i++) {
				result = SetCode(LPVOID(DWORD(address) + i), (LPBYTE)&dwAddress, 1) ? result : false;
			}
			return result;
		}
	}
	return false;
}

int	MachineCode::ConvertHEX(void *address, char *hex, Sequence *sections, BYTE *bytes, int hInstanceOffset) {
	int state = 0;
	int length = 0;
	int hexlen = strlen(hex) + 1;
	char *value = new char[MAX_PATH];
	for(int i = 0; i < hexlen; i++) {
		switch(state) {
		case 1: //	Hex value.
			if(isalnum(hex[i])) {
				strncpy_s(value, MAX_PATH, &hex[i-1], 2);
				char *end = value;
				bytes[length] = (BYTE)strtol(value, &end, 16);
				if(end == value + 2) {
					length++;
					state = 0;
					break;
				}
			}
			length = -1;
			i = hexlen;
			break;
		case 2: //	Offset string.
		case 3: //	Address string.
			if(state == 2 ? hex[i] == ']' : hex[i] == ')') {
				DWORD dwAddress = hInstanceOffset;
				Sequence *current = sections;
				while(current) {
					char *start = strstr(value, current->String);
					if(start) {
						dwAddress = 0;
						char *newvalue = new char[MAX_PATH];
						char *cAddress = _itoa((DWORD)current->Address, &strcpy(newvalue, value)[start - value], 16);
						strcpy(&cAddress[strlen(cAddress)], &start[strlen(current->String)]);
						delete[] value;
						value = newvalue;
						continue;
					}
					current = current->Next;
				}
				char *position = NULL;
				char *subvalue = value;
				while(subvalue != position) dwAddress += strtol(position = subvalue, &subvalue, 16);
				if(strlen(subvalue)) {
					length = -1;
					i = hexlen;
					break;
				}
				if(state == 2) dwAddress = GetLabelOffset(LPVOID(DWORD(address) + length), (LPVOID)dwAddress);
				*LPDWORD(bytes + length) = dwAddress;
				length += sizeof(DWORD);
				state = 0;
				break;
			}
			else
				strncat_s(value, MAX_PATH, &hex[i], 1);
			break;
		default: //	If state 0.
			if(hex[i] == '\0') break;
			if(isspace(hex[i])) break;
			if(hex[i] == ';') { i = hexlen; break; }
			if(hex[i] == '[') { value[0] = '\0'; state = 2; break; }
			if(hex[i] == '(') { value[0] = '\0'; state = 3; break; }
			char *end = &hex[i];
			strtol(end, &end, 16);
			if(end > &hex[i]) { value[0] = hex[i]; state = 1; break; }
			length = -1;
			i = hexlen;
			break;
		}
	}
	if(state) length = -1;
	delete[] value;
	return length;
}

int	MachineCode::GetLength(char *hexString) {
	int length = 0;
	char *start, *sq_brackets, *parentheses, *hex = hexString;
	while(start = (sq_brackets = strstr(hex, "[")) > (parentheses = strstr(hex, "(")) ?
		(parentheses ? parentheses : sq_brackets) : (sq_brackets ? sq_brackets : parentheses)) {
		length += 7 + start - hex;
		if(!(hex = strstr(start + 1, start == sq_brackets ? "]" : ")"))) return NULL;
	}
	length += strlen(hex);
	if((length%2)||(length<2)) return NULL;
	length /= 2;
	return length;
}

bool MachineCode::CompareOriginal(Sequence *original, int hInstanceOffset) {
	bool equal = true;
	Sequence *current = original;
	BYTE *bytes = new BYTE[MAX_LINE];
	while(current) {
		int length = ConvertHEX((LPVOID)((DWORD)current->Address + hInstanceOffset), current->String, NULL, bytes, hInstanceOffset);
		if(length > 0) if(CompareCode((LPVOID)((DWORD)current->Address + hInstanceOffset), bytes, length)) { current = current->Next; continue; }
		equal = false;
		break;
	}
	delete[] bytes;
	return equal;
}

bool MachineCode::SetPatch(Sequence *patch, Sequence *memoryTree, int hInstanceOffset) {
	bool success = true;
	Sequence *current = patch;
	BYTE *bytes = new BYTE[MAX_LINE];
	while(current) {
		int length = ConvertHEX((LPVOID)((DWORD)current->Address + hInstanceOffset), current->String, memoryTree, bytes, hInstanceOffset);
		if(length > 0) if(!SetCode((LPVOID)((DWORD)current->Address + hInstanceOffset), bytes, length)) success = false;
		current = current->Next;
	}
	delete[] bytes;
	return success;
}

Sequence** MachineCode::SetAttach(Sequence *attach, Sequence **memoryTree, int hInstanceOffset) {
	if(!(attach && memoryTree)) return NULL;
	Sequence **branch = memoryTree;
	while(*branch) branch = &(*branch)->Next;
	Sequence **pointer = branch;
	DWORD oldProtect;
	int length = 0;
	bool success = true;
	Sequence *current = attach;
	while(current) {
		if ((length = GetLength(current->String)) <= 0) { success = false; break; }
		(*pointer) = new Sequence(new BYTE[length], (char*)current->Address, NULL);
		_strlwr((*pointer)->String);
		pointer = &(*pointer)->Next;
		current = current->Next;
	}
	if (success) { current = attach; pointer = branch; }
	while(current) {
		if(ConvertHEX((*pointer)->Address, current->String, (*memoryTree), (LPBYTE)(*pointer)->Address, hInstanceOffset) == (length = GetLength(current->String)))
			if(VirtualProtect ((*pointer)->Address, length, PAGE_EXECUTE_READWRITE, &oldProtect)) { 
				pointer = &(*pointer)->Next;
				current = current->Next;
				continue;
			}
		success = false;
		break;			
	}
	if (success) return branch;
	pointer = branch;
	while(*pointer) { delete[] (char*)(*pointer)->Address; pointer = &(*pointer)->Next; }
	delete (*branch);
	(*branch) = NULL;
	return NULL;
}

//-----------------------------------------------------------------------------

bool ApplyPatch(PatchTree *child, bool success, Sequence *branchTree, DWORD dwVersionLS, bool isMorrowind, int hInstanceOffset) {
	PatchTree *patch = child;
	if(!patch) return success;
	do {
		if( !( (patch->FileVersion?(*patch->FileVersion)!=dwVersionLS:DEFAULTFILEVER!=dwVersionLS) ||
			(patch->Removed?(*patch->Removed):false) || (patch->Checked?!(*patch->Checked):false) ) ) {
			char *message = NULL;
			Sequence **branch = NULL;
			if(patch->Original ? !MachineCode::CompareOriginal(patch->Original, hInstanceOffset) : false) {
				message = strcat(strcat(strcpy(new char[MAX_PATH], "Patch: Original code is different for section \""), patch->Section), "\"");
			}else if(patch->Attach ? !(branch = MachineCode::SetAttach(patch->Attach, &branchTree, hInstanceOffset)) : false) {
				message = strcat(strcat(strcpy(new char[MAX_PATH], "Patch: Problems in section \""), patch->Attach->String), "\"");
			}else if(patch->Patch ? !MachineCode::SetPatch(patch->Patch, branchTree, hInstanceOffset) : false) {
				message = strcat(strcat(strcpy(new char[MAX_PATH], "Patch: Problems with patch applying in section \""), patch->Section), "\"");
			}else success = ApplyPatch(patch->Child, success, branchTree, dwVersionLS, isMorrowind, hInstanceOffset) ? success : false;
			if(message) {
				if(success && !isMorrowind) LOG::open(PATCH_LOG_NAME);
				LOG::logline(message);
				success = false;
				delete[] message;
			}
			if(branch) {
				delete (*branch);
				(*branch) = NULL;
			}
		}
	}while(patch = patch->Next);
	return success;
}

void PatchModuleMemory(HINSTANCE hModule, LPSTR lpFilename, bool isMorrowind) {
	if(!lpPatchTree || !hModule || !lpFilename) return;
	PatchTree *file = lpPatchTree;
	do {
		if((_stricmp(lpFilename, file->Section)==0)&&(file->Removed?!(*file->Removed):true)&&(file->Checked?(*file->Checked):true)&&file->Child) {
			if(ApplyPatch(file->Child, true, NULL, GetModuleNameAndVersion(hModule,NULL,NULL), isMorrowind, DWORD(hModule)-DEFAULT_INSTANCE_DLL)) {
				if(isMorrowind) LOG::logline("Patch has been applied to %s successfully", file->Section);
			}
			break;
		}
	}while(file = file->Next);
	return;
}

void PatchProcessMemory(HINSTANCE hModule, LPSTR origFilename, DWORD dwVersionLS, bool isMorrowind) {
	HINSTANCE hInstance = hModule;//GetModuleHandleA(NULL);
	if(lpPatchTree && hInstance && hModule && origFilename) {} else return;
	PatchTree *file = lpPatchTree;
	do {
		HINSTANCE hModuleInstance = _stricmp(origFilename, file->Section)==0 ? hModule : GetModuleHandleA(file->Section);
		if(hModule == hInstance && (_stricmp(TEXT_DLL_NAME, file->Section)==0 || _stricmp(MWE_DLL_NAME, file->Section)==0)) continue;
		if(hModuleInstance && (file->Removed?!(*file->Removed):true) && (file->Checked?(*file->Checked):true) && file->Child) {
			if(ApplyPatch(file->Child, true, NULL, 
				hModuleInstance == hInstance ? dwVersionLS : GetModuleNameAndVersion(hModuleInstance, NULL, NULL), isMorrowind,
				DWORD(hModuleInstance) - (hModuleInstance == hInstance ? DEFAULT_INSTANCE_EXE : DEFAULT_INSTANCE_DLL ))) {
				if(isMorrowind) LOG::logline("Patch has been applied to %s successfully", file->Section);
			}
		}
	}while(file = file->Next);
	return;
}
