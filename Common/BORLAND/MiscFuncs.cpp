//---------------------------------------------------------------------------
#pragma hdrstop

#include "MiscFuncs.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
__declspec(naked) void DEFCALL fix__fpreset(void)  // Fix conflict of BDS and TTS in FPU
{
 __asm             // Must be naked - compiler is VERY stupid
 {
  push  0x027F     // Normal FPU status
  fldcw [ESP]
  pop   EAX
  retn
 }
}
//---------------------------------------------------------------------------
void DEFCALL SaveAppLangFile(LPSTR FileName, LPSTR LangName, LPSTR LangDescr, DWORD LangID, LPSTR CodePage, LPSTR Exclude)
{
 PCLocalizer CurLocLang = new CLocalizer();
 TStringList *ExclItems = new TStringList();
 ExclItems->Text        = StringReplace(Exclude,",","\r\n",TReplaceFlags()<<rfReplaceAll);
 for(int ctr=0;ctr<ExclItems->Count;ctr++)CurLocLang->AddToExcludeList((ExclItems->Strings[ctr].Trim()).c_str());
 CurLocLang->BuildCurrentLanguage(LangName, LangDescr, CodePage, LangID);
 CurLocLang->SaveLocFile(FileName);
 delete(CurLocLang);
 delete(ExclItems);
}
//---------------------------------------------------------------------------
WORD DEFCALL ByteToHexWord(BYTE Value)
{
 WORD Result;
 BYTE HexTable[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

 ((PBYTE)&Result)[0] = HexTable[(Value >> 4)];
 ((PBYTE)&Result)[1] = HexTable[(Value & 0x0F)];
 return Result;
}
//---------------------------------------------------------------------------
LPSTR DEFCALL ConvertToDecDW(DWORD Value)
{
 char  DecNums[] = "0123456789";
 LPSTR Number    = "0000000000";
 DWORD DgCnt;
 DWORD TmpValue;
 DWORD Divid;
 Divid  = 1000000000;
 for(DgCnt = 0;DgCnt < 10;DgCnt++)
  {
   TmpValue = Value / Divid;
   ((BYTE *)Number)[DgCnt] = DecNums[TmpValue];
   Value -= TmpValue * Divid;
   Divid  = Divid / 10;
  }
 for(DgCnt = 0;(DgCnt < 9) && (Number[DgCnt] == '0');DgCnt++);
 return (LPSTR)(((DWORD)Number) + DgCnt);
}
//---------------------------------------------------------------------------
LPSTR DEFCALL MakeDiffSignature(PBYTE OrigData, PBYTE PatchData, DWORD Length, LPSTR Signature)
{
 int  SPos=0;
 int  Offset=0;
 int  Count;
 BYTE RepByte;

 for(;Offset < (int)Length;Offset++)
  {
   if(((Offset+4) < (int)Length)&&(((PDWORD)&OrigData[Offset])[0] == ((PDWORD)&PatchData[Offset])[0]))  // If four bytes match
    {
     Count   = 0;
     RepByte = OrigData[Offset];
     while((OrigData[Offset] == PatchData[Offset])&&(Offset < (int)Length)){Offset++;Count++;}
     Signature[SPos] = '*';
     lstrcpy((LPSTR)&Signature[SPos+1],ConvertToDecDW(Count));
     while(Signature[SPos])SPos++;
     Signature[SPos] = '*';
     Offset--;
     SPos++;
    }
     else
      {
       if(OrigData[Offset] != PatchData[Offset])((PWORD)&Signature[SPos])[0] = ByteToHexWord(OrigData[Offset]);
         else {Signature[SPos] ='?';Signature[SPos+1] ='?';}
       SPos +=2;
      }
  }
 Signature[SPos]=0;
 return Signature;
}
//---------------------------------------------------------------------------
/* TODO : Add add image formats, supported by TPicture */
// Convert: 0 - NoChange, 1 - BmpToJpg, 2 - JpgToBmp
int DEFCALL LoadPicture(TPicture* Image, PVOID ImageData, int DataSize, int Convert, int JpgQuality)
{
 bool               ImgIsBmp;
 TJPEGImage*        pJpgImg    = NULL;
 Graphics::TBitmap* pBmpImg    = NULL;
 TMemoryStream*     pMemStream = NULL;

 if(!Image || !ImageData || !DataSize)return 1;
 Image->Graphic = NULL;
 ImgIsBmp   = (((PWORD)ImageData)[0] == 0x4D42);
 pBmpImg    = new Graphics::TBitmap();
 pJpgImg    = new TJPEGImage();
 pMemStream = new TMemoryStream();
 pMemStream->Write(ImageData, DataSize);
 pMemStream->Position = 0;
                             
 if(ImgIsBmp)pBmpImg->LoadFromStream(pMemStream);
   else pJpgImg->LoadFromStream(pMemStream);
 pMemStream->Position = 0;
 pJpgImg->Scale       = jsFullSize;
 pJpgImg->Smoothing   = true;
 pJpgImg->Performance = jpBestQuality;
 pJpgImg->ProgressiveDisplay  = true;
 pJpgImg->ProgressiveEncoding = true;
                            
 if((Convert == 1) && ImgIsBmp)
  {           // BmpToJpg
   if(JpgQuality > 100)JpgQuality = 100;
     else {if(JpgQuality < 1)JpgQuality = 1;}
   pJpgImg->Assign(pBmpImg);
   pJpgImg->CompressionQuality = JpgQuality;
   pJpgImg->SaveToStream(pMemStream); // Compress bitmap to the stream
   pMemStream->Position = 0;
   pJpgImg->LoadFromStream(pMemStream);  // Load compressed image
   Image->Assign(pJpgImg);
  }
   else
     {
      if((Convert == 2) && !ImgIsBmp)
       {      // JpgToBmp
        pJpgImg->DIBNeeded();
        Image->Bitmap->Assign(pJpgImg);
       }
        else
         {    // NoChange (0)
          if(ImgIsBmp)Image->Assign(pBmpImg);
            else Image->Assign(pJpgImg);
         }
     }
 if(pBmpImg)delete(pBmpImg);
 if(pJpgImg)delete(pJpgImg);
 if(pMemStream)delete(pMemStream);
 return 0;
}
//---------------------------------------------------------------------------
int DEFCALL LoadPictureRes(TPicture* Image, LPSTR ResName, HMODULE Module, int Convert, int JpgQuality)
{
 DWORD   Size;
 HRSRC   hDataRes;
 HGLOBAL rcGlobal;

 hDataRes = FindResource(Module,ResName,RT_RCDATA);
 rcGlobal = LoadResource(Module,hDataRes);
 Size     = SizeofResource(Module,hDataRes);
 return LoadPicture(Image, rcGlobal, Size, Convert, JpgQuality);
}
//---------------------------------------------------------------------------
int DEFCALL LoadPictureFile(TPicture* Image, LPSTR FileName, int Convert, int JpgQuality)
{
 int     Res;
 DWORD   Size;
 PVOID   pData;
 HANDLE  hFile;
 HANDLE  hHeap;

 hFile = CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 if(hFile == INVALID_HANDLE_VALUE)return 3;
 Size  = GetFileSize(hFile, NULL);
 hHeap = GetProcessHeap();
 pData = HeapAlloc(hHeap,0,Size+32);
 ReadFile(hFile, pData, Size, &Size, NULL);
 CloseHandle(hFile);
 Res = LoadPicture(Image, pData, Size, Convert, JpgQuality);
 HeapFree(hHeap,0,pData);
 return Res;
}
//---------------------------------------------------------------------------
int DEFCALL CharCount(LPSTR String, BYTE Char)
{
 int Count=0;
 if(String){while(String[0]){if((BYTE)String[0]==Char)Count++;String++;}}
 return Count;
}
//---------------------------------------------------------------------------
int DEFCALL CharPosFromRight(LPSTR String, char Letter, int Index, int Offset)
{
 int Counter = lstrlen(String)-1;
 int Num     = 0;

 if(Offset > Counter)return -1;
 if(Index  < 0)Index = -Index;    // For all possible index manipulations
 if(Index == 0)Index = 1;
 Counter -= Offset;
 for(;;)
  {
   if(String[Counter] == Letter)Num++;
   if(Num == Index)return Counter;
   if(Counter == 0)return -1;
   Counter--;
  }
}
//---------------------------------------------------------------------------
int DEFCALL CharPosFromLeft(LPSTR String, char Letter, int Index, int Offset)
{
 int Counter = 0;
 int Num     = 0;

 if(Offset > lstrlen(String))return -1;
 if(Index  < 0)Index = -Index;    // For all possible index manipulations
 if(Index == 0)Index = 1;
 Counter += Offset;
 for(;;)
  {
   if(String[Counter] == Letter)Num++;
   if(Num == Index)return Counter;
   if(String[Counter] == 0)return -1;
   Counter++;
  }
}
//------------------------------------------------------------------------------------
int DEFCALL DeleteToRecycleBin(LPSTR Path)
{
 SHFILEOPSTRUCT FileOp;
 BYTE PathBuf[MAX_PATH + 1];

 ZeroMemory(&PathBuf, sizeof(PathBuf));
 ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT));
 lstrcpy(PathBuf, Path);   // Copy to have two NULLs at end  

 FileOp.wFunc  = FO_DELETE;
 FileOp.pFrom  = (LPSTR)&PathBuf;
 FileOp.fFlags = (FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_ALLOWUNDO);
 return SHFileOperation(&FileOp);
}
//---------------------------------------------------------------------------
bool DEFCALL IsDirectPath(LPSTR Path)
{
 if(Path[0] != 0x2E)return true;
 if(Path[1] == 0x00)return false;
 if(Path[1] != 0x2E)return true;
 if(Path[2] == 0x00)return false;
 return true;
}
//------------------------------------------------------------------------------------
bool DEFCALL DeleteFolder(LPSTR FolderPath)
{
 DWORD  PathLen;
 HANDLE hSearch;
 WIN32_FIND_DATA fdat;
 char   PathBuffer[MAX_PATH];

 PathBuffer[0] = 0;
 lstrcat(PathBuffer,FolderPath);
 PathLen = lstrlen(PathBuffer);
 PathBuffer[PathLen+0] = '\\';
 PathBuffer[PathLen+1] = '*';
 PathBuffer[PathLen+2] = 00;
 hSearch = FindFirstFile(PathBuffer,&fdat);
 if(hSearch != INVALID_HANDLE_VALUE)
  {
   do
	{
	 if(!(!IsDirectPath(fdat.cFileName)&&(fdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
	  {
       PathBuffer[PathLen+1] = 0;
       lstrcat(PathBuffer,fdat.cFileName);
	   if(fdat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)DeleteFolder(PathBuffer);
		 else
          {
           if(fdat.dwFileAttributes & FILE_ATTRIBUTE_READONLY)SetFileAttributes(PathBuffer,FILE_ATTRIBUTE_NORMAL);
           DeleteFile(PathBuffer);
          }
      }
    }
     while(FindNextFile(hSearch,&fdat));
   FindClose(hSearch);
   return RemoveDirectory(FolderPath);
  }
   else return 0;
}
//---------------------------------------------------------------------------
bool DEFCALL IsPathNameExist(LPSTR FilePath)
{
 DWORD Attrs = GetFileAttributes(FilePath);
 if(Attrs != INVALID_FILE_ATTRIBUTES)return true;   // TODO: Make more checks
 return false;
}
//---------------------------------------------------------------------------
bool DEFCALL IsFileExist(LPSTR FilePath)
{
 DWORD Attrs = GetFileAttributes(FilePath);
 if((Attrs != INVALID_FILE_ATTRIBUTES)&&!(Attrs & FILE_ATTRIBUTE_DIRECTORY))return true;   // TODO: Make more checks
 return false;
}
//---------------------------------------------------------------------------
bool DEFCALL IsDirectoryValid(LPSTR DirPath)
{
 DWORD Attrs;
 DWORD PathLen;
 BYTE  PathBuffer[MAX_PATH];

 lstrcpy((LPSTR)&PathBuffer, DirPath);
 PathLen = lstrlen(PathBuffer);
 if((PathBuffer[PathLen-1] == PATHDLML)||(PathBuffer[PathLen-1] == PATHDLMR))PathBuffer[PathLen-1] = 0;
 Attrs = GetFileAttributes((LPSTR)&PathBuffer);
 if((Attrs != INVALID_FILE_ATTRIBUTES)&&(Attrs & FILE_ATTRIBUTE_DIRECTORY))return true;
 return false;
}
//---------------------------------------------------------------------------
bool DEFCALL IsDirectoryEmpty(LPSTR DirPath)
{
 bool   Result = true;
 DWORD  PathLen;
 HANDLE hSearch;
 WIN32_FIND_DATA fdat;
 BYTE  PathBuffer[MAX_PATH];

 lstrcpy((LPSTR)&PathBuffer, DirPath);
 PathLen = lstrlen(PathBuffer);
 if((PathBuffer[PathLen-1] == PATHDLML)||(PathBuffer[PathLen-1] == PATHDLMR))PathLen--;
 PathBuffer[PathLen+0] = PATHDLML;
 PathBuffer[PathLen+1] = '*';
 PathBuffer[PathLen+2] = 00;
 hSearch = FindFirstFile((LPSTR)&PathBuffer, &fdat);  // Try to find any file in directory
 if(hSearch == INVALID_HANDLE_VALUE)return Result;
 do {if(IsDirectPath((LPSTR)&fdat.cFileName)){Result=false;break;}}
   while(FindNextFile(hSearch, &fdat));
 FindClose(hSearch);
 return Result;
}
//---------------------------------------------------------------------------
bool DEFCALL FindDirectoryPath(LPSTR InitialPath, LPSTR OutputPath, LPSTR NameToFind)
{
 int   PathLen = lstrlen(InitialPath);
 bool  NeedDir = false;   // If 'NameToFind' ends with PathDelimiter or 'NameToFind' and 'InitialPath' ends with PathDelimiter - search for directory, else for path
 LPSTR NamePtr = NULL;
 BYTE  PathBuffer[MAX_PATH];

 lstrcpy((LPSTR)&PathBuffer, InitialPath);
 if(NameToFind)
  {      // If 'NameToFind' begins with PathDelimiter or  'NameToFind' = PathDelimiter - search for directory
   if((((DWORD)NameToFind == PATHDLML)||((DWORD)NameToFind == PATHDLMR))||((NameToFind[0] == PATHDLML)||(NameToFind[0] == PATHDLMR))){NeedDir = true;NameToFind=NULL;}
   if(!NeedDir && NameToFind && NameToFind[0]){int NameLen = lstrlen(NameToFind);NeedDir = (PathLen && ((NameToFind[NameLen-1]==PATHDLML)||(NameToFind[NameLen-1]==PATHDLMR)));}
   if(NeedDir && ((PathBuffer[PathLen-1] != PATHDLML)&&(PathBuffer[PathLen-1] != PATHDLMR))){PathBuffer[PathLen]=PATHDLML;PathLen++;}
  }
 if(!OutputPath)OutputPath = InitialPath;   // If 'OutputDir' is NULL result saved to 'InitialPath'
   else OutputPath[0] = 0;
 if((PathBuffer[PathLen-1] == PATHDLML)||(PathBuffer[PathLen-1] == PATHDLMR))   // if 'NameToFind' is NULL or empty, name assumed at end of 'InitialPath'
  {
   if(NameToFind && NameToFind[0])NamePtr=NameToFind;
     else
      {
       NeedDir = true;
       PathLen--;
       while((PathBuffer[PathLen-1]!=PATHDLML)&&(PathBuffer[PathLen-1]!=PATHDLMR)&&(PathLen > 0))PathLen--;
      }
  }
    else
     {
      if(NameToFind && NameToFind[0]){PathBuffer[PathLen]=PATHDLML;NamePtr=NameToFind;}
        else {while((PathBuffer[PathLen-1]!=PATHDLML)&&(PathBuffer[PathLen-1]!=PATHDLMR)&&(PathLen > 0))PathLen--;}
     }
 while(PathLen > 0)
  {
   if(NamePtr)lstrcat((LPSTR)&PathBuffer, NamePtr);
   NamePtr = &PathBuffer[PathLen];
   DWORD Attrs = GetFileAttributes((LPSTR)&PathBuffer);
   if((Attrs != INVALID_FILE_ATTRIBUTES)&&((FILE_ATTRIBUTE_DIRECTORY & Attrs)&&NeedDir)||(!(FILE_ATTRIBUTE_DIRECTORY & Attrs)&&!NeedDir)){lstrcpy(OutputPath,(LPSTR)&PathBuffer);return true;}
   PathLen--;
   while((PathBuffer[PathLen-1]!=PATHDLML)&&(PathBuffer[PathLen-1]!=PATHDLMR)&&(PathLen > 0))PathLen--;
   PathBuffer[PathLen] = 0;
  }
 return false;
}
//---------------------------------------------------------------------------
void DEFCALL CreateDirectoryPath(LPSTR Path)
{
 BYTE FullPath[MAX_PATH];

 strcpy((LPSTR)&FullPath, Path);
 for(int Count=0;FullPath[Count] != 0;Count++)
  {
   if((FullPath[Count]==PATHDLML)||(FullPath[Count]==PATHDLMR))
    {
     FullPath[Count] = 0;
     CreateDirectory((LPSTR)&FullPath, NULL); // Faster Always create or Test it`s existance first ?
     FullPath[Count] = PATHDLML;
    } 
  }
}
//---------------------------------------------------------------------------
void DEFCALL SetNodeState(TTreeNode* Node, DWORD Flags)
{
 TTVItem tvi;
 ZeroMemory(&tvi, sizeof(TTVItem));
 tvi.hItem     = Node->ItemId;
 tvi.mask      = TVIF_STATE;
 tvi.stateMask = TVIS_BOLD | TVIS_CUT;
 tvi.state     = Flags;
 TreeView_SetItem(Node->Handle, &tvi);
}
//---------------------------------------------------------------------------
DWORD DEFCALL ComputeCRC32(PVOID Buffer, DWORD BufferSize)
{
 if(!Buffer || !BufferSize)return 0;
 DWORD Checksum = 0xFFFFFFFF;
 for(DWORD bctr=0;bctr < BufferSize;bctr++)
  {
   Checksum ^= ((PBYTE)Buffer)[bctr];
   for(int ctr=0;ctr < 8;ctr++)Checksum = ((Checksum >> 1) ^ (CRC32POLYNOME & ~((Checksum & 1) - 1))); // {if((Checksum = Checksum >> 1) & 1)Checksum ^= 0xA001A001;}
  }
 return Checksum;
}
//---------------------------------------------------------------------------
PVOID DEFCALL GetProcessExeBase(DWORD ProcessID)
{
 PROCESSENTRY32 pent32;
 MODULEENTRY32  ment32;
 HANDLE         hProcessSnap;
 HANDLE         hModulesSnap;
 PVOID          ModBase;
 LPSTR          FNamePtr;

 ModBase       = NULL;
 pent32.dwSize = sizeof(PROCESSENTRY32);
 hProcessSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
 if(Process32First(hProcessSnap, &pent32))
  {
   do
	{
	 if(pent32.th32ProcessID == ProcessID)
	  {
	   FNamePtr = pent32.szExeFile;
	   for(DWORD ctr = lstrlen(pent32.szExeFile);ctr > 0;ctr--)if(pent32.szExeFile[ctr] == '\\'){FNamePtr = (LPSTR)((DWORD)pent32.szExeFile+ctr+1);break;}
	   ment32.dwSize = sizeof(MODULEENTRY32);
	   hModulesSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pent32.th32ProcessID);
	   if(Module32First(hModulesSnap, &ment32))
		{
		 do {if(lstrcmp(FNamePtr,ment32.szModule)==0){ModBase = ment32.modBaseAddr;break;}}
		   while(Module32Next(hModulesSnap, &ment32));
		}
	   break;
	  }
	}
   while(Process32Next(hProcessSnap, &pent32));
  }
 CloseHandle(hProcessSnap);
 return ModBase;
}
//---------------------------------------------------------------------------
TTreeNode* DEFCALL GetChildNode(TTreeNode *ParentNode, LPSTR NodeName)
{
 if(ParentNode)
  {
   TTreeNode *ChildNode = ParentNode->getFirstChild();
   while(ChildNode){if(ChildNode->Text.Trim().AnsiCompareIC(NodeName)==0)return ChildNode;else ChildNode = ParentNode->GetNextChild(ChildNode);}
  }
 return NULL;
}
//---------------------------------------------------------------------------
TTreeNode* _stdcall GetSiblingNode(TTreeNode *ParentNode, LPSTR NodeName)
{
 if(ParentNode)
  {
   TTreeNode *ChildNode = ParentNode;
   while((ChildNode = ChildNode->getPrevSibling()))ParentNode = ChildNode;
   while(ParentNode){if(ParentNode->Text.Trim().AnsiCompareIC(NodeName)==0)return ParentNode;else ParentNode = ParentNode->getNextSibling();}
  }
 return NULL;
}
//---------------------------------------------------------------------------
TTreeNode* DEFCALL GetTreeItemUnderMouse(TTreeView *TgtTree)
{
 POINT     Point;
 TTreeNode *CurNode;

 GetCursorPos(&Point);
 ::ScreenToClient(TgtTree->Handle, &Point);
 if((CurNode = TgtTree->GetNodeAt(Point.x, Point.y))) // x - is no use (Node is a full row)
  {
   int ImgWidth = 0;
   TRect nrect  = CurNode->DisplayRect(true);
   if(TgtTree->StateImages)ImgWidth = TgtTree->StateImages->Width;
     else {if(TgtTree->Images)ImgWidth = TgtTree->Images->Width;}
   nrect.Left  -= ImgWidth;
   if((Point.x >= nrect.Left) && (Point.x <= nrect.right))return CurNode;
  }
 return NULL;  // No node image
}
//---------------------------------------------------------------------------
TTreeNode* DEFCALL MouseOnTreeNodeImage(TTreeView *TgtTree)
{
 int       LeftOff;
 int       ImgWidth;
 POINT     Point;
 TTreeNode *CurNode;

 GetCursorPos(&Point);
 ::ScreenToClient(TgtTree->Handle,&Point);
 CurNode  = TgtTree->GetNodeAt(Point.x, Point.y); // x - is no use (Node is a full row)
 if(CurNode && (CurNode->StateIndex >= 0) && (TgtTree->StateImages || TgtTree->Images))
  {
   ImgWidth = 0;
   LeftOff  = CurNode->DisplayRect(true).Left;  // Get a Node`s X offset
   if(TgtTree->StateImages)ImgWidth = TgtTree->StateImages->Width;
     else {if(TgtTree->Images)ImgWidth = TgtTree->Images->Width;}
   if((Point.x >= LeftOff-ImgWidth) && (Point.x <= LeftOff))return CurNode;
  }
 return NULL;  // No node image
}
//---------------------------------------------------------------------------
TTreeNode* DEFCALL ExpandNodePath(TTreeNode *ParentNode, LPSTR NodePath)
{
 if(!ParentNode || !NodePath || !NodePath[0])return ParentNode;
 int Counter = 0;
 AnsiString NodeName = NodePath;
 while((NodePath[0]!=PATHDLML)&&(NodePath[0]!=PATHDLMR)&&NodePath[0]){NodePath++;Counter++;}
 NodeName = NodeName.SetLength(Counter);
 while(ParentNode)
  {
   if(NodeName.AnsiCompareIC(ParentNode->Text)==0)
	{
	 ParentNode->Expand(false);
     ParentNode->TreeView->TopItem = ParentNode;
     return ExpandNodePath(ParentNode->getFirstChild(), ++NodePath);
    }
   ParentNode = ParentNode->getNextSibling();
  }
 return ParentNode;
}
//---------------------------------------------------------------------------
TTreeNode* DEFCALL FindTreeNodeByData(TTreeView* Tree, PVOID Data)
{
 for(int ctr=0;ctr < Tree->Items->Count;ctr++)if(Tree->Items->Item[ctr]->Data == Data)return Tree->Items->Item[ctr];
 return NULL;
}
//---------------------------------------------------------------------------
AnsiString DEFCALL ExtractRootDir(LPSTR DirPath)
{
 int Counter      = -1;
 AnsiString temps = DirPath;
 while(DirPath[Counter+1]&&(DirPath[Counter+1]!=PATHDLML)&&(DirPath[Counter+1]!=PATHDLMR))Counter++;
 if(Counter >= 0)temps = temps.SetLength(Counter+1);
 return temps;
}
//---------------------------------------------------------------------------
bool DEFCALL IsProcessPathExist(LPSTR Path, DWORD *ProcessID)
{
 PROCESSENTRY32 pent32;
 MODULEENTRY32  ment32;
 HANDLE         hProcessSnap;
 HANDLE         hModulesSnap;

 hProcessSnap  = INVALID_HANDLE_VALUE;
 hModulesSnap  = INVALID_HANDLE_VALUE;
 ment32.dwSize = sizeof(MODULEENTRY32);
 pent32.dwSize = sizeof(PROCESSENTRY32);
 hProcessSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
 if(Process32First(hProcessSnap, &pent32))
  {
   do
	{
	 if(ExtractFileName(Path).AnsiCompareIC(ExtractFileName((LPSTR)&pent32.szExeFile)) == 0)
	  {
	   hModulesSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pent32.th32ProcessID);
	   if(Module32First(hModulesSnap, &ment32))
		{
		 do
		  {
		   if((lstrcmpi(Path, ment32.szExePath)==0)&&(GetCurrentProcessId() != pent32.th32ProcessID))
			{
			 if(ProcessID)(*ProcessID) = pent32.th32ProcessID;
			 CloseHandle(hModulesSnap);
			 CloseHandle(hProcessSnap);
			 return true;
			}
		  }
		   while(Module32Next(hModulesSnap, &ment32));
		}
	   CloseHandle(hModulesSnap);
	   break;
	  }
	}
   while(Process32Next(hProcessSnap, &pent32));
  }
 CloseHandle(hProcessSnap);
 return false;
}
//---------------------------------------------------------------------------
PVOID DEFCALL GetVersionData(LPSTR StringName, PVOID VerBufer)
{
 DWORD BufferSize;
 PWSTR NameStr;
 PWSTR DataStr;
 WCHAR Buffer[128];

 // What a STUPID format of resource structure? I don`t know how to parse it, and use brute force instead.
 NameStr    = (PWSTR)&Buffer;
 DataStr    = (PWSTR)VerBufer;
 BufferSize = ((PWORD)VerBufer)[0];
 MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,StringName,-1,NameStr,sizeof(Buffer));
 for(DWORD ctr=0;ctr < (BufferSize-lstrlenW(NameStr));ctr++,DataStr++)
  {
   if(lstrcmpW(NameStr,DataStr)==0)
	{
	 DataStr += lstrlenW(DataStr);
	 DataStr  = (PWSTR)(((DWORD)DataStr & 0xFFFFFFFC)+4); // Align to DWORD
	 return DataStr;   // Return some resource data
	}
  }
 return NULL;
}
//---------------------------------------------------------------------------
DWORD DEFCALL GetProcessIdByName(LPSTR StringName)
{
 PROCESSENTRY32 pent32;
 HANDLE         hProcessSnap;

 pent32.dwSize = sizeof(PROCESSENTRY32);
 hProcessSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
 if(Process32First(hProcessSnap, &pent32))
  {
   do
	{
	 if(lstrcmpi(ExtractFileName((LPSTR)&pent32.szExeFile).c_str(), StringName) == 0)
	  {
	   CloseHandle(hProcessSnap);
	   return pent32.th32ProcessID;
	  }
	}
   while(Process32Next(hProcessSnap, &pent32));
  }
 CloseHandle(hProcessSnap);
 return NULL;
}
//---------------------------------------------------------------------------
bool DEFCALL KillProcessByID(DWORD ProcessID)
{
 HANDLE hProcess;

 hProcess = OpenProcess(PROCESS_TERMINATE,false,ProcessID);
 if(hProcess){TerminateProcess(hProcess, 1);CloseHandle(hProcess);return true;}
 return false;
}
//---------------------------------------------------------------------------
PVOID DEFCALL GetProcessModule(DWORD ProcessID, LPSTR ModuleName)
{
 MODULEENTRY32  ment32;
 HANDLE         hModulesSnap;
 PVOID          Result = NULL;

 hModulesSnap  = INVALID_HANDLE_VALUE;
 ment32.dwSize = sizeof(MODULEENTRY32);
 hModulesSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
 if(Module32First(hModulesSnap, &ment32))
  {
   do
	{
	 if(lstrcmpi(ModuleName, ment32.szModule)==0){Result = ment32.hModule;break;}
	}
	 while(Module32Next(hModulesSnap, &ment32));
  }
 CloseHandle(hModulesSnap);
 return Result;
}
//---------------------------------------------------------------------------

