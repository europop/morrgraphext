
#ifndef Extract7zH
#define Extract7zH

#include <Windows.h>
#include <stdlib.h>
#include "7Zip/7zExtrMain.h"
//----------------------------------------------------------------------------------------------------------
#define CBR_NEXT  0
#define CBR_SKIP  1
#define CBR_ABORT 2

#define CRC_NEXTFILE   0
#define CRC_FILEDATA   1
#define CRC_FILEEXIST  2
#define CRC_EXTRERROR  3
#define CRC_CANTWRITE  4
#define CRC_CANTCREATE 5

#define FPATHDELIM1 0x5C  // '//'
#define FPATHDELIM2 0x2F  // '\'

#define EX7ZCALL
#define IFNOARCH if(!ArchiveOpened)

typedef unsigned __int64 QWORD;
typedef DWORD (__stdcall *EXTR_7ZIP_CALLBACK)(DWORD Reason, PVOID UserData, LPSTR FileName, QWORD FileSize, bool IsDirectoy);
//----------------------------------------------------------------------------------------------------------
class CExtract7Zip
{
private:
  bool  ArchiveOpened;
  bool  ExtrWithFullPath;
  LPSTR ExtrPath;
  PVOID CBUserData;
  EXTR_7ZIP_CALLBACK ExtrCallback;
  ISzInStream     *CurArchInStream;
  CFileInStream   ArchFileStream;
  CMemoryInStream ArchMemStream;
  CSzArEx db;
  SRes res;
  ISzAlloc allocImp;
  ISzAlloc allocTempImp;
  //   if you need cache, use these 3 variables.
  //   if you use external function, you can make these variable as static.
  UInt32  blockIndex;    
  size_t  outBufferSize; 
  BYTE    *outBuffer;     
  BYTE   ExtractPath[MAX_PATH];

  void   EX7ZCALL Initialize(void);
  QWORD  EX7ZCALL FindFileNameIndex(LPSTR FileName);
  bool   EX7ZCALL InitCurrentArchive(void);
  bool   EX7ZCALL CreateExtrFile(LPSTR FileName, bool IsDir, HANDLE *hNewFile);

public:
  bool  ExtrError;

  bool  EX7ZCALL ExtractArchive(void);
  bool  EX7ZCALL ExtractFile(QWORD FileIndex);  
  bool  EX7ZCALL ExtractFile(LPSTR FileName){return ExtractFile(FindFileNameIndex(FileName));};
  bool  EX7ZCALL CloseCurrentArchive(void);
  bool  EX7ZCALL OpenArchive(LPSTR ArchivePath);
  bool  EX7ZCALL AssignArchive(PVOID ArchBeginAddr);
  void  EX7ZCALL SetExtractionPath(LPSTR ExtractPath);
  void  EX7ZCALL SetExtractionOptions(bool WithFullPath);
  void  EX7ZCALL SetExtractionCallback(EXTR_7ZIP_CALLBACK Callback, PVOID UserData);
  QWORD EX7ZCALL GetArchivedFilesSize(QWORD *FilesCount);
  LPSTR EX7ZCALL GetArchivedFileInfo(QWORD FileIndex, QWORD *FileSize, bool *IsDir);

  EX7ZCALL ~CExtract7Zip();
  EX7ZCALL CExtract7Zip(void);
  EX7ZCALL CExtract7Zip(LPSTR ArchivePath);
};

typedef CExtract7Zip* PCExtract7Zip;
//----------------------------------------------------------------------------------------------------------
#endif	
