#pragma once

#include "../mwse/WinHeader.h"

#define DEFAULT_INSTANCE_EXE	0x00400000
#define DEFAULT_INSTANCE_DLL	0x10000000

#define MAX_LINE		65535
#define DEFAULTFILEVER	1820
#define PATCH_LOG_NAME	"patch log.txt"
#define PATCH_SETTINGS	"mge3/PatchSettings"
#define PATCH_PATH		"mge3"
#define PATCH_MASK		"*.mcp"

#define PK_PARENT		"Parent"
#define PK_VERSION		"Version"
#define PK_FILEVERSION	"FileVersion"
#define PK_CHECKED		"Checked"
#define PK_REMOVED		"Removed"
#define PK_ORIGINAL		"Original"
#define PK_PATCH		"Patch"
#define PK_ATTACH		"Attach"

//-----------------------------------------------------------------------------

class Sequence {
public:
	Sequence(const void *address, const char *string, Sequence *next);
	~Sequence();
	void *Address;
	char *String;
	Sequence *Next;
};

//-----------------------------------------------------------------------------

class PatchTree {
public:
	PatchTree(const char *Section);
	~PatchTree();
	static PatchTree* BuildPatchTree();
	static Sequence* FindAllFiles(Sequence *files, const char *startPath, const char *extensionMask);
	bool ReadSection(const char *mcpFileName, const char *section);
	bool UpdateTree(PatchTree **tree);
private:
	char* RemoveWhitespace(char *string);
	Sequence* SetMachineCode(const char *mcpFileName, const char *section, const char *prefix, char *line);
	bool UpdateSection(PatchTree **section);
public:
	PatchTree *Next;
	PatchTree *Child;
	char *Section;
	char *Parent;
	bool *Checked;
	bool *Removed;
	float *Version;
	unsigned *FileVersion;
	Sequence *Original;
	Sequence *Patch;
	Sequence *Attach;
};

//-----------------------------------------------------------------------------

class MachineCode {
public:
	static bool CompareOriginal(Sequence *original, int hInstanceOffset);
	static bool SetPatch(Sequence *patch, Sequence *memoryTree, int hInstanceOffset);
	static Sequence** SetAttach(Sequence *attach, Sequence **memoryTree, int hInstanceOffset);

	static bool CompareCode(void *address, BYTE *code, int length);
	static bool SetCode(void *address, BYTE *code, int length);
	static bool GetCode(void *address, BYTE *code, int length);

	static bool CompareAndPatchByJumpToAttachment(void *address, BYTE *original, int length, BYTE *attachment, int size);
	static DWORD GetLabelOffset(void *offsetPlacementAddress, void *labelAddress);
	static DWORD GetAlignLength(DWORD codeSize, DWORD segmentSize);
private:
	static int  ConvertHEX(void *address, char *hex, Sequence *sections, BYTE *bytes, int hInstanceOffset);
	static int  GetLength(char *hexString);
};

//-----------------------------------------------------------------------------

#ifdef _PATCH
PatchTree *lpPatchTree = NULL;
#else
extern PatchTree *lpPatchTree;
#endif

void PatchModuleMemory(HINSTANCE hModule, LPSTR lpFilename, bool isMorrowind);
void PatchProcessMemory(HINSTANCE hInstance, LPSTR origFilename, DWORD dwVersionLS, bool isMorrowind);
