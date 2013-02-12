
#include "SinCos.h"

//===========================================================================
__declspec(naked)__int64 _stdcall MakeSinCos(float Sine, float Cosine)
{
 __asm MOV EAX,[ESP+4]
 __asm MOV EDX,[ESP+8]
 __asm RET 8
}
//------------------------------------------------------------------------------------
void _stdcall FillSinCosTable(PQWORD SinCosTbl)
{
 for(int ctr=0;ctr<360;ctr++)SinCosTbl[ctr] = FPUSinCosDeg32(ctr);
}
//------------------------------------------------------------------------------------
int _stdcall FindNearestDegree(PQWORD SinCosTbl, QWORD SinCos)
{   // OPTIMIZE !!!
 int   CurDeg  = -1;
 float CurSin  = ((float*)&SinCos)[0];
 float CurCos  = ((float*)&SinCos)[1];
 float SinDiff = 100;  // Any BIG value
 float CosDiff = 100;  // Any BIG value
 float SinTmpD;
 float CosTmpD;

 for(int ctr=0;ctr<360;ctr++)
  {
   SinTmpD = ((float*)&SinCosTbl[ctr])[0];
   CosTmpD = ((float*)&SinCosTbl[ctr])[1];
   if(((SinTmpD < 0)&&(CurSin > 0))||((SinTmpD > 0)&&(CurSin < 0)))continue;  // Only with same sign
   if(((CosTmpD < 0)&&(CurCos > 0))||((CosTmpD > 0)&&(CurCos < 0)))continue;  // Only with same sign
   SinTmpD = (SinTmpD > CurSin)?(SinTmpD - CurSin):(CurSin - SinTmpD);
   CosTmpD = (CosTmpD > CurCos)?(CosTmpD - CurCos):(CurCos - CosTmpD);
   if((SinTmpD < SinDiff)&&(CosTmpD < CosDiff)){SinDiff=SinTmpD;CosDiff=CosTmpD;CurDeg=ctr;}
  }
 return CurDeg;
}
//------------------------------------------------------------------------------------
int _stdcall FindDegreeBySin(PQWORD SinCosTbl, float Sinus)
{   // OPTIMIZE !!!
 int   CurDeg  = -1;
 float SinDiff = 100;  // Any BIG value
 float SinTmpD;

 for(int ctr=0;ctr<180;ctr++)
  {
   SinTmpD = ((float*)&SinCosTbl[ctr])[0];
   if(((SinTmpD < 0)&&(Sinus > 0))||((SinTmpD > 0)&&(Sinus < 0)))continue;  // Only with same sign
   SinTmpD = (SinTmpD > Sinus)?(SinTmpD - Sinus):(Sinus - SinTmpD);
   if(SinTmpD < SinDiff){SinDiff=SinTmpD;CurDeg=ctr;}
  }
 return CurDeg;
}
//------------------------------------------------------------------------------------
int _stdcall FindDegreeByCos(PQWORD SinCosTbl, float Cosinus)
{   // OPTIMIZE !!!
 int   CurDeg  = -1;
 float CosDiff = 100;  // Any BIG value
 float CosTmpD;

 for(int ctr=0;ctr<180;ctr++)
  {
   CosTmpD = ((float*)&SinCosTbl[ctr])[1];
   if(((CosTmpD < 0)&&(Cosinus > 0))||((CosTmpD > 0)&&(Cosinus < 0)))continue;  // Only with same sign
   CosTmpD = (CosTmpD > Cosinus)?(CosTmpD - Cosinus):(Cosinus - CosTmpD);
   if(CosTmpD < CosDiff){CosDiff=CosTmpD;CurDeg=ctr;}
  }
 return CurDeg;
}
//------------------------------------------------------------------------------------
// TODO: Don`t do (ViewSinCos > 360), split this function in two.
// Input: SinCos or Degree
// Output: Angle Degree
int _stdcall GetNearestAngle(PQWORD SinCosTbl, QWORD ViewSinCos, QWORD TargetSinCos, long *Direction)
{
 int AngleDiff    = 0;
 int ViewDegree   = 0;
 int TargetDegree = 0;

 ViewDegree   = (ViewSinCos   > 360)?(FindNearestDegree(SinCosTbl, ViewSinCos)):(ViewSinCos);     // Angle degree or SinCos	[In input is SinCos, convert it into Degree]
 TargetDegree = (TargetSinCos > 360)?(FindNearestDegree(SinCosTbl, TargetSinCos)):(TargetSinCos); // Angle degree or SinCos [In input is SinCos, convert it into Degree]
 AngleDiff    = (ViewDegree - TargetDegree);
 if(AngleDiff < 0)AngleDiff = -AngleDiff;
 if(AngleDiff > 180)          // Here defined deault direction priority
  {
   AngleDiff = (360-AngleDiff);
   if(Direction)(*Direction) = (ViewDegree < TargetDegree)?(-1):(1);
  }
   else 
    {
     if(Direction)
      {
       if(AngleDiff != 0)(*Direction) = (ViewDegree < TargetDegree)?(1):(-1);
         else (*Direction) = 0;
      }
    }
 return AngleDiff; 
}   
//===========================================================================