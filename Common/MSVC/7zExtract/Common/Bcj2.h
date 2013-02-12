/* Bcj2.h -- Converter for x86 code (BCJ2)
2008-04-11
Copyright (c) 1999-2008 Igor Pavlov
Read Bra.h for license options */

#ifndef __BCJ2_H
#define __BCJ2_H

#include "Types.h"

/*
Conditions:
  outSize <= FullOutputSize, 
  where FullOutputSize is full size of output stream of x86_2 filter.

If buf0 overlaps outBuf, there are two required conditions:
  1) (buf0 >= outBuf)
  2) (buf0 + size0 >= outBuf + FullOutputSize).

Returns:
  SZ_OK
  SZ_ERROR_DATA - Data error
*/

int Bcj2_Decode(
    const BYTE *buf0, SizeT size0, 
    const BYTE *buf1, SizeT size1, 
    const BYTE *buf2, SizeT size2, 
    const BYTE *buf3, SizeT size3, 
    BYTE *outBuf, SizeT outSize);

#endif
