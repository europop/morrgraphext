
#include "Extract7z.h"
//----------------------------------------------------------------------------------------------------------
EX7ZCALL CExtract7Zip::~CExtract7Zip()
{
 this->CloseCurrentArchive();
}
//----------------------------------------------------------------------------------------------------------
EX7ZCALL CExtract7Zip::CExtract7Zip(void)
{
 this->Initialize();
}
//----------------------------------------------------------------------------------------------------------
EX7ZCALL CExtract7Zip::CExtract7Zip(LPSTR ArchivePath)
{
 this->Initialize();
 this->OpenArchive(ArchivePath);
}
//----------------------------------------------------------------------------------------------------------
void  EX7ZCALL CExtract7Zip::Initialize(void)
{
 res = SZ_OK;
 ExtrWithFullPath = true;
 ArchiveOpened = false;
 ExtrError = false;
 ExtrCallback = NULL;
 ExtrPath = (LPSTR)&ExtractPath;
 CurArchInStream = NULL;
 memset(ExtrPath,sizeof(ExtractPath),0);
 memset(&ArchFileStream,sizeof(CFileInStream),0);
 memset(&ArchMemStream,sizeof(CMemoryInStream),0);
 memset(&db,sizeof(CSzArEx),0);
 ArchFileStream.File = INVALID_HANDLE_VALUE;
 allocImp.Free  = SzFree;
 allocImp.Alloc = SzAlloc;
 allocTempImp.Free  = SzFreeTemp;
 allocTempImp.Alloc = SzAllocTemp;
 ArchMemStream.InStream.Read  = SzMemReadImp;
 ArchMemStream.InStream.Seek  = SzMemSeekImp;
 ArchFileStream.InStream.Read = SzFileReadImp;
 ArchFileStream.InStream.Seek = SzFileSeekImp;
 CrcGenerateTable();
}
//----------------------------------------------------------------------------------------------------------
bool  EX7ZCALL CExtract7Zip::ExtractArchive(void)
{
 int   ErrorCtr;
 QWORD Count;
 
 IFNOARCH return false;
 this->ExtrError = false;
 ErrorCtr = 0;
 Count = 0;
 while(this->ExtractFile(Count))
  {
   ErrorCtr += this->ExtrError;
   Count++;    
  }
 return !ErrorCtr;
}
//----------------------------------------------------------------------------------------------------------
bool  EX7ZCALL CExtract7Zip::ExtractFile(QWORD FileIndex) 
{
 int    CBResult;
 HANDLE hOutFile;
 size_t offset;
 size_t processedSize;
 size_t outSizeProcessed;
 CSzFileItem *FItem;
      
 IFNOARCH return false;
 this->ExtrError = false;
 if(FileIndex >= this->db.db.NumFiles)return false; // No More Files
 FItem = this->db.db.Files + FileIndex;
 if(ExtrCallback)
  {
   CBResult = this->ExtrCallback(CRC_NEXTFILE,CBUserData,FItem->Name,FItem->Size,FItem->IsDirectory);
   if(CBResult == CBR_ABORT)return false;  // As No More Files
   if(CBResult == CBR_SKIP)return true;    // Skip this File, but continue extracting
  }
                   
 // What about a very big files ?      
 res = SzAr_Extract(&db, CurArchInStream, FileIndex, &blockIndex, &outBuffer, &outBufferSize, &offset, &outSizeProcessed, &allocImp, &allocTempImp);
 if(res != SZ_OK)
  {
   ExtrError = true;
   if(ExtrCallback)
    {
     CBResult = this->ExtrCallback(CRC_EXTRERROR,CBUserData,FItem->Name,FItem->Size,FItem->IsDirectory);   // CRC error
     if(CBResult == CBR_ABORT)return false;  // As No More Files
       else ExtrError = false;     // Not an error
    }
   return true;
  }

 if(!this->CreateExtrFile(FItem->Name, FItem->IsDirectory, &hOutFile))
  {
   ExtrError = true;
   if(ExtrCallback)
    {
     CBResult = this->ExtrCallback(CRC_CANTCREATE,CBUserData,FItem->Name,FItem->Size,FItem->IsDirectory);   // Wrong path ?
     if(CBResult == CBR_ABORT)return false;  // As No More Files
    }
   return false;
  }
 if(!FItem->IsDirectory && (hOutFile != INVALID_HANDLE_VALUE))   // Handle are invalid if existing file owerwriting disgarded
  {
   processedSize = Ex7zWriteFile(hOutFile, (outBuffer + offset), outSizeProcessed);
   Ex7zCloseFile(hOutFile);
   if(processedSize != outSizeProcessed)
    {
     ExtrError = true;
     if(ExtrCallback)
      {
       CBResult = this->ExtrCallback(CRC_CANTWRITE,CBUserData,FItem->Name,FItem->Size,FItem->IsDirectory);   // Possible the disk is full
       if(CBResult == CBR_ABORT)return false;  // As No More Files
      }
     return false;
    }

   // Process blocks not a full file
   if(ExtrCallback)
    {
     CBResult = this->ExtrCallback(CRC_FILEDATA,CBUserData,(LPSTR)(outBuffer + offset),processedSize,FItem->IsDirectory);
     if(CBResult == CBR_ABORT)return false;  // As No More Files
    }
  }
 return true;  
} 
//----------------------------------------------------------------------------------------------------------
bool  EX7ZCALL CExtract7Zip::CloseCurrentArchive(void)
{
 IFNOARCH return false;
 SzArEx_Free(&db, &allocImp);
 IAlloc_Free(&allocImp, outBuffer);
 if(ArchFileStream.File != INVALID_HANDLE_VALUE){Ex7zCloseFile(ArchFileStream.File);ArchFileStream.File = INVALID_HANDLE_VALUE;}
 CurArchInStream = NULL;
 ArchiveOpened = false;
 res = SZ_OK;
 return true;
}
//----------------------------------------------------------------------------------------------------------
bool  EX7ZCALL CExtract7Zip::OpenArchive(LPSTR ArchivePath)
{
 this->CloseCurrentArchive();
 ArchFileStream.File = CreateFile(ArchivePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(ArchFileStream.File == INVALID_HANDLE_VALUE)return false;
 CurArchInStream = &ArchFileStream.InStream;
 return this->InitCurrentArchive();
}
//----------------------------------------------------------------------------------------------------------
bool  EX7ZCALL CExtract7Zip::AssignArchive(PVOID ArchBeginAddr)
{
 this->CloseCurrentArchive();
 CurArchInStream = &ArchMemStream.InStream;
 ArchMemStream.CurrentPos  = 0;
 ArchMemStream.BaseAddress = ArchBeginAddr;
 return this->InitCurrentArchive();
}
//----------------------------------------------------------------------------------------------------------
void  EX7ZCALL CExtract7Zip::SetExtractionPath(LPSTR ExtractPath)
{
 int PathLen = strlen(ExtractPath);
 strcpy(ExtrPath,ExtractPath);
 if((PathLen > 0) && !((ExtrPath[PathLen-1]==FPATHDELIM1)||(ExtrPath[PathLen-1]==FPATHDELIM2))){ExtrPath[PathLen]=FPATHDELIM1;ExtrPath[PathLen+1]=0;}
}
//----------------------------------------------------------------------------------------------------------
void  EX7ZCALL CExtract7Zip::SetExtractionOptions(bool WithFullPath)
{
 ExtrWithFullPath = WithFullPath;
}
//----------------------------------------------------------------------------------------------------------
void  EX7ZCALL CExtract7Zip::SetExtractionCallback(EXTR_7ZIP_CALLBACK Callback, PVOID UserData)
{
 CBUserData   = UserData;
 ExtrCallback = Callback;
}
//----------------------------------------------------------------------------------------------------------
QWORD EX7ZCALL CExtract7Zip::GetArchivedFilesSize(QWORD *FilesCount)
{
 QWORD FSize;
 QWORD FCount;
 CSzFileItem *FItem;

 IFNOARCH return 0;
 FSize  = 0;
 FCount = 0;
 for(QWORD ctr = 0; ctr < this->db.db.NumFiles; ctr++)
  {
   FItem  = this->db.db.Files + ctr;
   FSize += FItem->Size;
   FCount++;
  } 
 if(FilesCount)(*FilesCount) = FCount;
 return FSize;
}
//----------------------------------------------------------------------------------------------------------
LPSTR EX7ZCALL CExtract7Zip::GetArchivedFileInfo(QWORD FileIndex, QWORD *FileSize, bool *IsDir)
{
 CSzFileItem *FItem;

 IFNOARCH return NULL;
 FItem = this->db.db.Files + FileIndex;
 if(IsDir)(*IsDir) = FItem->IsDirectory;
 if(FileSize)(*FileSize) = FItem->Size;
 return FItem->Name;
}
//----------------------------------------------------------------------------------------------------------
QWORD  EX7ZCALL CExtract7Zip::FindFileNameIndex(LPSTR FileName)
{
 CSzFileItem *FItem;

 IFNOARCH return -1;
 for(QWORD ctr = 0; ctr < this->db.db.NumFiles; ctr++)
  {
   FItem = this->db.db.Files + ctr;
   if(strcmpi(FileName, FItem->Name)==0)return ctr;
  }
 return -1;
}
//----------------------------------------------------------------------------------------------------------
bool   EX7ZCALL CExtract7Zip::InitCurrentArchive(void)
{
 blockIndex    = 0xFFFFFFFF;   // it can have any value before first call (if outBuffer = 0) 
 outBufferSize = 0;            // it can have any value before first call (if outBuffer = 0) 
 outBuffer     = NULL;         // it must be 0 before first call for each new archive. 

 SzArEx_Init(&db);
 ArchiveOpened = true;
 res = SzArEx_Open(&db, CurArchInStream, &allocImp, &allocTempImp);
 if(res == SZ_OK)return true;
 this->CloseCurrentArchive();
 return false;
}
//----------------------------------------------------------------------------------------------------------
bool EX7ZCALL CExtract7Zip::CreateExtrFile(LPSTR FileName, bool IsDir, HANDLE *hNewFile)
{
 BYTE FullPath[MAX_PATH];
 BYTE Delim[] = {FPATHDELIM1,0,0};

 if(IsDir)return true;     // Directory creation is skipped
 if(hNewFile)(*hNewFile) = INVALID_HANDLE_VALUE;
 if(!ExtrWithFullPath)
  {
  // if(IsDir)return true;     // Directory creation is skipped
   for(int Count=0,Last=0;;Count++)
    {
     if((FileName[Count]==FPATHDELIM1)||(FileName[Count]==FPATHDELIM2))Last = Count+1;
     if(FileName[Count+1]==0){FileName = (LPSTR)&FileName[Last];break;}  
    }
  } 
 strcpy((LPSTR)&FullPath,ExtrPath);
 strcat((LPSTR)&FullPath,FileName);
 if(IsDir)strcat((LPSTR)&FullPath,(LPSTR)&Delim);
 
 // Create Directories
 for(int Count=0;FullPath[Count] != 0;Count++)
  {
   if((FullPath[Count]==FPATHDELIM1)||(FullPath[Count]==FPATHDELIM2))
    {
     FullPath[Count] = 0;
     CreateDirectory((LPSTR)&FullPath, NULL); // Faster Always create or Test it`s existance first ?
     FullPath[Count] = FPATHDELIM1;
    } 
  }
 if(!IsDir)
  {
   if(ExtrCallback && (GetFileAttributes((LPSTR)&FullPath)!=INVALID_FILE_ATTRIBUTES))
    {
     if(this->ExtrCallback(CRC_FILEEXIST,CBUserData,(LPSTR)&FullPath,0,IsDir)!=CBR_NEXT)return true; // Do not owerwrite existing file
    }
   DeleteFile((LPSTR)&FullPath);
   if(hNewFile)
    {
     (*hNewFile) = CreateFile((LPSTR)&FullPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
     if((*hNewFile) == INVALID_HANDLE_VALUE)return false;  // Something wrong !
    }
  }
 return true;   // Directory created
}
//----------------------------------------------------------------------------------------------------------
