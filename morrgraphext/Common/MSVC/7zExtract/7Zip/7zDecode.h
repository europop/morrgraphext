/* 7zDecode.h -- Decoding from 7z folder
2008-04-09
Igor Pavlov
Copyright (c) 1999-2008 Igor Pavlov
Read 7zItem.h for license options */

#ifndef _7zDecodeH
#define _7zDecodeH

#include "7zItem.h"

#include "7zIn.h"

SRes SzDecode(const CFileSize *packSizes, const CSzFolder *folder,
    /*
    #ifdef _LZMA_IN_CB
    */
    ISzInStream *stream, CFileSize startPos,
    /*
    #else
    const BYTE *inBuffer,
    #endif
    */
    BYTE *outBuffer, size_t outSize, ISzAlloc *allocMain);

#endif
