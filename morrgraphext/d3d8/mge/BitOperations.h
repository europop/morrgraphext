#pragma once
#ifndef _BITOPERATIONS_H_
#define _BITOPERATIONS_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Inline single bit test and operations [i386 assembly]
 * Written from scratch by krzymar / krzymark (Krzysztof Markowski)
 *
 * Changelog:
 *	v.1.00 (2009-03-14):
 *	 - up to 32 bit data variable support (initial version)
 *
 * TODO:
 *	- add inline functions supporting wider than 32 bit data:
 *	  offset = (bit >> 3) & ~3; bit &= 31; edx += offset;
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MASK(bit)	(1 << bit)

#pragma warning(push)
#pragma warning(disable : 4035)

__forceinline bool BitTst (void* var, int bit) {
    __asm {
        mov eax, bit
        mov edx, var
        bt [edx], eax
        setc al
    }
}

__forceinline bool BitSet (void* var, int bit) {
    __asm {
        mov eax, bit
        mov edx, var
        bts [edx], eax
        setc al
    }
}

__forceinline bool BitReset (void* var, int bit) {
    __asm {
        mov eax, bit
        mov edx, var
        btr [edx], eax
        setc al
    }
}

__forceinline bool BitCompliment (void* var, int bit) {
    __asm {
        mov eax, bit
        mov edx, var
        btc [edx], eax
        setc al
    }
}

#pragma warning(pop)

#endif /* _BITOPERATIONS_H_ */
