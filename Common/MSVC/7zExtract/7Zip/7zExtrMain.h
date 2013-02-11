/* 7zItem.h -- 7z Items
2008-05-01
Igor Pavlov
Copyright (c) 1999-2008 Igor Pavlov
Read LzmaDec.h for license options */

#ifndef _7zExtrMainH
#define _7zExtrMainH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define USE_WINDOWS_FUNCTIONS
#endif

#ifdef USE_WINDOWS_FUNCTIONS
#include <windows.h>
#endif

#include "7zIn.h"
#include "7zExtract.h"
#include "7zAlloc.h"
#include "../Common/7zCrc.h"


#ifdef USE_WINDOWS_FUNCTIONS
typedef HANDLE MY_FILE_HANDLE;
#else
typedef FILE *MY_FILE_HANDLE;
#endif

#define kBufferSize (1 << 12)

#define PERIOD_4 (4 * 365 + 1)
#define PERIOD_100 (PERIOD_4 * 25 - 1)
#define PERIOD_400 (PERIOD_100 * 4 + 1)

#ifdef USE_WINDOWS_FUNCTIONS
/*
   ReadFile and WriteFile functions in Windows have BUG:
   If you Read or Write 64MB or more (probably min_failure_size = 64MB - 32KB + 1) 
   from/to Network file, it returns ERROR_NO_SYSTEM_RESOURCES 
   (Insufficient system resources exist to complete the requested service).
*/
#define kChunkSizeMax (1 << 24)
#endif

typedef struct _CFileInStream
{
  ISzInStream InStream;
  MY_FILE_HANDLE File;
} CFileInStream;

typedef struct _CMemoryInStream
{
  ISzInStream InStream;
  PVOID BaseAddress;
  DWORD CurrentPos;
} CMemoryInStream;

void PrintError(char *sz);
void ConvertNumberToString(CFileSize value, char *s);
void ConvertFileTimeToString(CNtfsFileTime *ft, char *s);

SRes SzFileReadImp(void *object, void **buffer, size_t *size);
SRes SzFileSeekImp(void *object, CFileSize pos, ESzSeek origin);
SRes SzMemReadImp(void *object, void **buffer, size_t *size);
SRes SzMemSeekImp(void *object, CFileSize pos, ESzSeek origin);

size_t Ex7zReadFile(MY_FILE_HANDLE file, void *data, size_t size);
size_t Ex7zWriteFile(MY_FILE_HANDLE file, void *data, size_t size);
int    Ex7zCloseFile(MY_FILE_HANDLE file);


#endif
