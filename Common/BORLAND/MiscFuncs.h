//---------------------------------------------------------------------------

#ifndef MiscFuncsH
#define MiscFuncsH

#include <Windows.h>
#include <Jpeg.hpp>
#include <Tlhelp32.h>
#include "VclLocalizer.h"

//---------------------------------------------------------------------------
#ifndef DEFCALL
#define DEFCALL __stdcall
#endif

//---------------------------------------------------------------------------
#define CRC32POLYNOME 0xEDB88320      // AABBCCDD    >> DDCCBBAA
#define PATHDLMR 0x2F
#define PATHDLML 0x5C

#define _REVERSE(Value)          (((DWORD)(Value) << 24)|((DWORD)(Value) >> 24)|(((DWORD)(Value) << 8)&0x00FF0000)|(((DWORD)(Value) >> 8)&0x0000FF00))
#define _MINIMAL(ValueA, ValueB) (((ValueA) < (ValueB))?(ValueA):(ValueB))
#define _NUMTOPERC(Num, Value)   (((Num)*100)/(Value))
#define _PERCTONUM(Perc, Value)  (((Perc)*(Value))/100)
#define _PERCTOMAX(Perc,Num)     (((Num)*100)/((Perc)?(Perc):1))

//---------------------------------------------------------------------------
void       DEFCALL fix__fpreset(void);
void       DEFCALL SaveAppLangFile(LPSTR FileName, LPSTR LangName, LPSTR LangDescr, DWORD LangID, LPSTR CodePage, LPSTR Exclude);
WORD       DEFCALL ByteToHexWord(BYTE Value);
LPSTR      DEFCALL ConvertToDecDW(DWORD Value);
LPSTR      DEFCALL MakeDiffSignature(PBYTE OrigData, PBYTE PatchData, DWORD Length, LPSTR Signature);
int        DEFCALL LoadPicture(TPicture* Image, PVOID ImageData, int DataSize, int Convert=0, int JpgQuality=50);
int        DEFCALL LoadPictureRes(TPicture* Image, LPSTR ResName, HMODULE Module=NULL, int Convert=0, int JpgQuality=50);
int        DEFCALL LoadPictureFile(TPicture* Image, LPSTR FileName, int Convert=0, int JpgQuality=50);
int        DEFCALL CharCount(LPSTR String, BYTE Char);
int        DEFCALL CharPosFromRight(LPSTR String, char Letter, int Index, int Offset);
int        DEFCALL CharPosFromLeft(LPSTR String, char Letter, int Index, int Offset);
int        DEFCALL DeleteToRecycleBin(LPSTR Path);
bool       DEFCALL IsDirectPath(LPSTR Path);
bool       DEFCALL DeleteFolder(LPSTR FolderPath);
bool       DEFCALL IsPathNameExist(LPSTR FilePath);
bool       DEFCALL IsFileExist(LPSTR FilePath);
bool       DEFCALL IsDirectoryValid(LPSTR DirPath);
bool       DEFCALL IsDirectoryEmpty(LPSTR DirPath);
bool       DEFCALL FindDirectoryPath(LPSTR InitialPath, LPSTR OutputPath=NULL, LPSTR NameToFind=NULL);
void       DEFCALL CreateDirectoryPath(LPSTR Path);
void       DEFCALL SetNodeState(TTreeNode* Node, DWORD Flags);
DWORD      DEFCALL ComputeCRC32(PVOID Buffer, DWORD BufferSize);
PVOID      DEFCALL GetProcessExeBase(DWORD ProcessID);
TTreeNode* DEFCALL GetChildNode(TTreeNode *ParentNode, LPSTR NodeName);
TTreeNode* DEFCALL GetSiblingNode(TTreeNode *ParentNode, LPSTR NodeName);
TTreeNode* DEFCALL MouseOnTreeNodeImage(TTreeView *TgtTree);   // TODO: Make ONE function
TTreeNode* DEFCALL GetTreeItemUnderMouse(TTreeView *TgtTree);
TTreeNode* DEFCALL ExpandNodePath(TTreeNode *ParentNode, LPSTR NodePath);
TTreeNode* DEFCALL FindTreeNodeByData(TTreeView* Tree, PVOID Data);
AnsiString DEFCALL ExtractRootDir(LPSTR DirPath);
bool       DEFCALL IsProcessPathExist(LPSTR Path, DWORD *ProcessID=NULL);
bool       DEFCALL KillProcessByID(DWORD ProcessID);
DWORD      DEFCALL GetProcessIdByName(LPSTR StringName);
PVOID      DEFCALL GetVersionData(LPSTR StringName, PVOID VerBufer);
PVOID      DEFCALL GetProcessModule(DWORD ProcessID, LPSTR ModuleName);

//---------------------------------------------------------------------------
#endif
