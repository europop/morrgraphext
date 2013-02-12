/* CpuArch.h
2008-03-26
Igor Pavlov
Public domain */

#ifndef __CPUARCH_H
#define __CPUARCH_H

/* 
LITTLE_ENDIAN_UNALIGN means:
  1) CPU is LITTLE_ENDIAN
  2) it's allowed to make unaligned memory accesses
if LITTLE_ENDIAN_UNALIGN is not defined, it means that we don't know 
about these properties of platform.
*/

#if defined(_M_IX86) || defined(_M_X64) || defined(_M_AMD64) || defined(__i386__) || defined(__x86_64__)
#define LITTLE_ENDIAN_UNALIGN
#endif

#ifdef LITTLE_ENDIAN_UNALIGN

#define GetUi16(p) (*(const UInt16 *)(p))
#define GetUi32(p) (*(const UInt32 *)(p))
#define GetUi64(p) (*(const UInt64 *)(p))
#define SetUi32(p, d) *(UInt32 *)(p) = d;

#else

#define GetUi16(p) (((const BYTE *)(p))[0] | \
           ((UInt16)((const BYTE *)(p))[1] << 8))

#define GetUi32(p) (((const BYTE *)(p))[0]        | \
           ((UInt32)((const BYTE *)(p))[1] << 8 ) | \
           ((UInt32)((const BYTE *)(p))[2] << 16) | \
           ((UInt32)((const BYTE *)(p))[3] << 24))

#define GetUi64(p) (GetUi32(p) | (UInt64)GetUi32(((const BYTE *)(p)) + 4) << 32)

#define SetUi32(p, d) { UInt32 _x_ = (d); \
    ((BYTE *)(p))[0] = (BYTE)_x_; \
    ((BYTE *)(p))[1] = (BYTE)(_x_ >> 8); \
    ((BYTE *)(p))[2] = (BYTE)(_x_ >> 16); \
    ((BYTE *)(p))[3] = (BYTE)(_x_ >> 24); }

#endif

#endif
