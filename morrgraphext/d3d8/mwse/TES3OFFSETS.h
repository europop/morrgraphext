//Tes3Offsets.h
#pragma once

#include "TES3MACHINE.h"
#include "FUNCEXTENDER.h"
#include "STRINGS.h"

#define ULONG unsigned long int
	
#define LONGOFFSET(a,b) (VPVOID)(ULONG)( ((ULONG*)(a)) + b)

#define OFFSET_SCDT		0x16
#define OFFSET_NPCFLAGS		0x0d
#define OFFSET_NPCCLASS		0x2d
#define OFFSET_NPCSERVICES	0x3a
#define OFFSET_NPCLEVEL		0x1f
#define OFFSET_CLASSSERVICES	0x22
#define OFFSET_GOLD		0xd8
#define OFFSET_NPCBASEGOLD	0x2b
#define OFFSET_CREBASEGOLD	0x36
#define OFFSET_LEVI_LIST	0x0c

// offset of value/weight/etc. in carriable item templates
ULONG offsetOfValue(ULONG type);
ULONG offsetOfWeight(ULONG type);
ULONG offsetOfCondition(ULONG type);
ULONG offsetOfQuality(ULONG type);

const char *GetNameString(TES3MACHINE& vm, VPVOID temp, ULONG type, VPVOID base);
	
bool GetOffsetData(TES3MACHINE& vm, VPVOID base, ULONG offset, ULONG* data);
	
bool SetOffsetData(TES3MACHINE& vm, VPVOID base, ULONG offset, ULONG data);

bool GetIdString(TES3MACHINE& vm, VPVOID temp, char *id);

bool GetTargetData(TES3MACHINE& vm, VPVOID* refr, VPVOID* temp=0, ULONG* type=0, VPVOID* base=0);

bool GetAttachData(TES3MACHINE& vm, VPVOID ref, ULONG type, ULONG offset, void* data, size_t size);
bool GetAttachData(TES3MACHINE& vm, VPVOID ref, ULONG type, ULONG offset, ULONG* data);
template<class T> bool GetAttachData(TES3MACHINE& vm, VPVOID ref, ULONG type, ULONG offset, T &data)
{
    return GetAttachData(vm, ref, type, offset, reinterpret_cast<void *>(&data), sizeof(T));
}

bool SetAttachData(TES3MACHINE& vm, VPVOID ref, ULONG type, ULONG offset, ULONG data);

//what's wrong with this?




