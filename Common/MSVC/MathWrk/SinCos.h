#pragma once

#ifndef SinCosH
#define SinCosH

#include <Windows.h>
//===========================================================================
typedef unsigned __int64 QWORD;

__int64 _stdcall MakeSinCos(float Sine, float Cosine);
void    _stdcall FillSinCosTable(PQWORD SinCosTbl);
int     _stdcall FindDegreeBySin(PQWORD SinCosTbl, float Sinus);
int     _stdcall FindDegreeByCos(PQWORD SinCosTbl, float Cosinus);
int     _stdcall FindNearestDegree(PQWORD SinCosTbl, QWORD SinCos);
int     _stdcall GetNearestAngle(PQWORD SinCosTbl, QWORD ViewSinCos, QWORD TargetSinCos, long *Direction);
//===========================================================================
#endif
