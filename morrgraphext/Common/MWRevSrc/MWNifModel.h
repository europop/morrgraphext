#pragma once

#ifndef MWNifModelH
#define MWNifModelH

#include "MWDefinitions.h"
//====================================================================================
//                                 NIF Classes
//------------------------------------------------------------------------------------
namespace MORROWIND 
{
enum NiAVObjFlags          {AppCulled=0x0001, Visual=0x0008}; 
enum NiAVObjFlagsPropagate {PROP_NONE=0x0000,PROP_USE_TRIANGLES=0x0002,PROP_USE_OBBS=0x0004,PROP_CONTINUE=0x0006}; 
//====================================================================================
struct NiVelocity
{
 VECTOR3 kLocalVelocity;
 VECTOR3 kWorldVelocity;
};
//------------------------------------------------------------------------------------
class NiObject        // Size = 0x0008  (Virtual base class)    // Check constructor xrefs to find a derived classes
{
public:
/*0000*/ PVOID (*pVFT_NiObject)[128];    // Accessing to element 'XXXX->pVFTable[0][Index]'  
/*0004*/ DWORD uiRefCount;  
//static   DWORD ObjectsInMemory;     // Offset = 0x003DECE0  
 
  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL NiObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x002E98A0 )}                // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)           MWTHISCALL ~NiObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x002E98F0 )}               // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PNiObject MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x002E98D0 )}  // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
    
  // Additional
 
};                           
//------------------------------------------------------------------------------------
class NiObjectNET : public NiObject    // Size = 0x0014  (Virtual base class) 
{                                                 
public:
/*0008*/ LPSTR pName;               // Bounding box,Head,...   
/*000C*/ PNiExtraData ExtraData;    // VFT=0x0074A774                              
/*0010*/ PVOID dwUnk01;             // VFT=0x00751460, Constructor=0x00710F30, Size=0x0078 [Something with a matrix]                       
 
};
//------------------------------------------------------------------------------------
class NiAVObject : public NiObjectNET    // Size = 0x0090  (Virtual base class) 
{                                                 
public:
/*0014*/ WORD        Flags;            // [bAppCulled=0b00000001][bVisual=0b00001000][ePropagate=0b00000000>>NONE][ePropagate=0b00000010>>USE_TRIANGLES][ePropagate=0b00000100>>USE_OBBS][ePropagate=0b00000110>>CONTINUE]    
/*0018*/ PNiNode     ParentNode;                            
/*001C*/ QUATERNION  kWorldBound;      // float * 4 (3 + 1)                
//----------- STRUCTURE ------------
/*002C*/ MATRIX3x3*  LocalRotate;      // Don`t fotget: this is a POINTER                    
/*0030*/ VECTOR3     LocalTranslate;   // May be 'LocalTransform'[See offset: 0x002EC2DF]
/*003C*/ float       LocalScale;                     
//----------- STRUCTURE ------------
/*0040*/ MATRIX3x3   WorldRotate;   
/*0064*/ VECTOR3     WorldTranslate;                       
/*0070*/ float       WorldScale;   
//----------------------------------                        
/*0074*/ PNiVelocity Velocity;         // Array: VECTOR3 kLocalVelocity;VECTOR3 kWorldVelocity;   
//----------------------------------                         
/*0078*/ PNiBBox BoundingBoxA;         // m_pkModelABV           
/*007C*/ PNiBBox BoundingBoxB;         // Both of same derived type                     
/*0080*/ PVOID dwUnk02;                // Pointer to 0x0055EF30(DWORD __cdecl Function(Arg)) for 'CLONE PlayerSaveGame' [Setted in 0x0052B9D0 ]                    
/*0084*/ PVOID dwUnk03;                // Pointer to MACP for 'CLONE PlayerSaveGame'                    
/*0088*/ PVOID FirstProperty;          // This pair possibly a 'vector' structure [NiProperty VFT=0x00746A24]                          
/*008C*/ PVOID PropertiesList;                                     

  // Jumps to Morrowind`s functions
  __declspec(noinline) BOOL MWTHISCALL _GetObjectCulled(void){JUMPEXTMEMBER(EXTMODBASE, 0x00042B30 )}               
  __declspec(noinline) void MWTHISCALL _SetObjectCulled(bool Culled){JUMPEXTMEMBER(EXTMODBASE, 0x00042B10 )}              
  
  // Additional
 BOOL MWTHISCALL GetObjectCulled(void){return (this->Flags & AppCulled);}              
 void MWTHISCALL SetObjectCulled(bool Culled){if(Culled)this->Flags &= AppCulled;else this->Flags |= ~AppCulled;}
                                     
}; 
//------------------------------------------------------------------------------------
class NiNode : public NiAVObject   // Size = 0x00B0           // VFT=0x0074F9C8
{
public:
/*0090*/ PVOID   (*pVFT_NiNode)[1];   // VFT=0x0074FA70  [Destructor only]                          
/*0094*/ PNiNode ChildNodes;          // Child Ptr Table
/*0098*/ DWORD   UnkCounter;          // Max Slots\Initial slots ?
/*009C*/ DWORD   ChildSlots;          // In 'ChildNodes' table
/*00A0*/ DWORD   Children;            // In 'ChildNodes' table
/*00A4*/ DWORD   UnknowOne;           // 'bool' ?
/*00A8*/ PVOID   FirstEffect;         // This pair possibly a 'vector' structure
/*00AC*/ PVOID   EffectsList;         // List Item format: DATA_PTR, NEXT_ITEM   // NULL if only one effect present

  // Jumps to Morrowind`s functions
  __declspec(noinline)         MWTHISCALL NiNode(PVOID Data){JUMPEXTMEMBER(EXTMODBASE, 0x002C81E0 )}            // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)         MWTHISCALL ~NiNode(void){JUMPEXTMEMBER(EXTMODBASE, 0x002C82C0 )}                 // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PNiNode MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x002C82A0 )}  // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
  __declspec(noinline) PNiNode MWTHISCALL _FindNodeByName(LPSTR NodeName){JUMPEXTMEMBER(EXTMODBASE, 0x002CA0A0 )}
                                 
}; 
//------------------------------------------------------------------------------------
class NiCamera : public NiAVObject       // Size = 0x01E0
{
public:
/*0090*/ float fUnk0;                                    
/*0094*/ float fUnk1;
/*0098*/ float fUnk2;
/*009C*/ float fUnk3;
/*00A0*/ float fUnk4;
/*00A4*/ float fUnk5;
/*00A8*/ float fUnk6;
/*00AC*/ float fUnk7;
/*00B0*/ float fUnk8;
/*00B4*/ float fUnk9;
/*00B8*/ float fUnk10;
/*00BC*/ float fUnk11;
/*00C0*/ float fUnk12;
/*00C4*/ float fUnk13;
/*00C8*/ float fUnk14;
/*00CC*/ float fUnk15;
/*00D0*/ float fUnk16;
/*00D4*/ float fUnk17;
/*00D8*/ float fUnk18;
//------------------------------------------------------------------
/*00DC*/ VECTOR3 kWorldDir;    
/*00E8*/ VECTOR3 kWorldUp;     
/*00F4*/ VECTOR3 kWorldRight; 
 // --- m_kFrustum ---
/*0100*/ float Frustum_L;
/*0104*/ float Frustum_R;
/*0108*/ float Frustum_T;
/*010C*/ float Frustum_B;
/*0110*/ float Frustum_N;
/*0114*/ float Frustum_F;
 // --- m_kPort ---
/*0118*/ float Port_L;
/*011C*/ float Port_R;
/*0120*/ float Port_T;
/*0124*/ float Port_B;
// --- ---
/*0128*/ DWORD DataUnk74;  
/*012C*/ DWORD DataUnk75;  
/*0130*/ DWORD DataUnk76;  
/*0134*/ DWORD DataUnk77;  
/*0138*/ DWORD DataUnk78;  
/*013C*/ DWORD DataUnk79;  
/*0140*/ DWORD DataUnk80;  
/*0144*/ DWORD DataUnk81;  
/*0148*/ DWORD DataUnk82;   // Some Class begin
/*014C*/ DWORD DataUnk83;  
/*0150*/ DWORD DataUnk84;  
/*0154*/ DWORD DataUnk85;  
/*0158*/ DWORD DataUnk86;  
/*015C*/ DWORD DataUnk87;  
/*0160*/ DWORD DataUnk88;  
/*0164*/ DWORD DataUnk89;  
/*0168*/ DWORD DataUnk90;  
/*016C*/ DWORD DataUnk91;  
/*0170*/ DWORD DataUnk92;  
/*0174*/ DWORD DataUnk93;  
/*0178*/ DWORD DataUnk94;  
/*017C*/ DWORD DataUnk95;  
/*0180*/ DWORD DataUnk96;  
/*0184*/ DWORD DataUnk97;  
/*0188*/ DWORD DataUnk98;  
/*018C*/ DWORD DataUnk99;  
/*0190*/ DWORD DataUnk100;  
/*0194*/ DWORD DataUnk101;  
/*0198*/ DWORD DataUnk102;  
/*019C*/ DWORD DataUnk103;  
/*01A0*/ DWORD DataUnk104;  
/*01A4*/ DWORD DataUnk105;  
/*01A8*/ DWORD DataUnk106;  
/*01AC*/ DWORD DataUnk107;  
/*01B0*/ DWORD DataUnk108;  
/*01B4*/ DWORD DataUnk109;  
/*01B8*/ DWORD DataUnk110;  
/*01BC*/ DWORD DataUnk111;  
/*01C0*/ DWORD DataUnk112;  
/*01C4*/ DWORD DataUnk113;  
/*01C8*/ DWORD DataUnk114;  
/*01CC*/ DWORD DataUnk115;  
/*01D0*/ DWORD DataUnk116;  
/*01D4*/ DWORD DataUnk117;    // ClassPtr: Size = 0x0018
/*01D8*/ DWORD DataUnk118;  
/*01DC*/ DWORD DataUnk119;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)            MWTHISCALL NiCamera(void){JUMPEXTMEMBER(EXTMODBASE, 0x002CC200 )}                // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)            MWTHISCALL ~NiCamera(void){JUMPEXTMEMBER(EXTMODBASE, 0x002CC510 )}               // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PNiCamera  MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x002CC4F0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional

};
//------------------------------------------------------------------------------------
class NiFogProperty          // Size = ???  // 007466F4 NiFog_VFT    
{
public:
/*0000*/ PVOID (*pVFTable)[13];  
/*0004*/ DWORD DataUnk1;  
/*0008*/ DWORD DataUnk2;  
/*000C*/ DWORD DataUnk3;  
/*0010*/ DWORD DataUnk4;  
/*0014*/ DWORD DataUnk5;  
/*0018*/ DWORD DataUnk6;  
/*001C*/ DWORD DataUnk7;  
/*0020*/ DWORD DataUnk8;  
/*0024*/ DWORD DataUnk9;  
/*0028*/ DWORD DataUnk10;  
/*002C*/ DWORD DataUnk11;  
/*0030*/ DWORD DataUnk12;  
/*0034*/ DWORD DataUnk13;  
/*0038*/ DWORD DataUnk14;  
/*003C*/ DWORD DataUnk15;  
/*0040*/ DWORD DataUnk16;  
/*0044*/ DWORD DataUnk17;  
/*0048*/ DWORD DataUnk18;  
/*004C*/ DWORD DataUnk19;  
/*0050*/ DWORD DataUnk20;  
/*0054*/ DWORD DataUnk21;  
/*0058*/ DWORD DataUnk22;  
/*005C*/ DWORD DataUnk23;  
/*0060*/ DWORD DataUnk24;  
/*0064*/ DWORD DataUnk25;  
/*0068*/ DWORD DataUnk26;  
/*006C*/ DWORD DataUnk27;  
/*0070*/ DWORD DataUnk28;  
/*0074*/ DWORD DataUnk29;  
/*0078*/ DWORD DataUnk30;  
/*007C*/ DWORD DataUnk31;  
/*0080*/ DWORD DataUnk32;  
/*0084*/ DWORD DataUnk33;  
/*0088*/ DWORD DataUnk34;  
/*008C*/ DWORD DataUnk35;  
/*0090*/ DWORD DataUnk36;  
/*0094*/ DWORD DataUnk37;  
/*0098*/ DWORD DataUnk38;  
/*009C*/ DWORD DataUnk39;  
/*00A0*/ DWORD DataUnk40;  
/*00A4*/ DWORD DataUnk41;  
/*00A8*/ DWORD DataUnk42;  
/*00AC*/ DWORD DataUnk43;  
/*00B0*/ DWORD DataUnk44;  
/*00B4*/ DWORD DataUnk45;  
/*00B8*/ DWORD DataUnk46;  
/*00BC*/ DWORD DataUnk47;  
/*00C0*/ DWORD DataUnk48;  
/*00C4*/ DWORD DataUnk49;  
/*00C8*/ DWORD DataUnk50;  
/*00CC*/ DWORD DataUnk51;  
/*00D0*/ DWORD DataUnk52;  
/*00D4*/ DWORD DataUnk53;  
/*00D8*/ DWORD DataUnk54;  

  // Jumps to Morrowind`s functions
 // __declspec(noinline) MWTHISCALL NiFogProperty(void){JUMPEXTMEMBER(EXTMODBASE, ?????? )}    // Constructor
  __declspec(noinline)                MWTHISCALL ~NiFogProperty(void){JUMPEXTMEMBER(EXTMODBASE, 0x00019600 )}   // Destructor              // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PNiFogProperty MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x00005B40 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  // Additional

};
//------------------------------------------------------------------------------------
class NiDirectionalLight          // Size = 0x00DC 
{
public:
/*0000*/ PVOID (*pVFTable)[38];  
/*0004*/ DWORD DataUnk1;  
/*0008*/ DWORD DataUnk2;  
/*000C*/ DWORD DataUnk3;  
/*0010*/ DWORD DataUnk4;  
/*0014*/ DWORD DataUnk5;  
/*0018*/ DWORD DataUnk6;  
/*001C*/ DWORD DataUnk7;  
/*0020*/ DWORD DataUnk8;  
/*0024*/ DWORD DataUnk9;  
/*0028*/ DWORD DataUnk10;  
/*002C*/ DWORD DataUnk11;  
/*0030*/ DWORD DataUnk12;  
/*0034*/ DWORD DataUnk13;  
/*0038*/ DWORD DataUnk14;  
/*003C*/ DWORD DataUnk15;  
/*0040*/ DWORD DataUnk16;  
/*0044*/ DWORD DataUnk17;  
/*0048*/ DWORD DataUnk18;  
/*004C*/ DWORD DataUnk19;  
/*0050*/ DWORD DataUnk20;  
/*0054*/ DWORD DataUnk21;  
/*0058*/ DWORD DataUnk22;  
/*005C*/ DWORD DataUnk23;  
/*0060*/ DWORD DataUnk24;  
/*0064*/ DWORD DataUnk25;  
/*0068*/ DWORD DataUnk26;  
/*006C*/ DWORD DataUnk27;  
/*0070*/ DWORD DataUnk28;  
/*0074*/ DWORD DataUnk29;  
/*0078*/ DWORD DataUnk30;  
/*007C*/ DWORD DataUnk31;  
/*0080*/ DWORD DataUnk32;  
/*0084*/ DWORD DataUnk33;  
/*0088*/ DWORD DataUnk34;  
/*008C*/ DWORD DataUnk35;  
/*0090*/ DWORD DataUnk36;  
/*0094*/ DWORD DataUnk37;  
/*0098*/ DWORD DataUnk38;  
/*009C*/ DWORD DataUnk39;  
/*00A0*/ DWORD DataUnk40;  
/*00A4*/ DWORD DataUnk41;  
/*00A8*/ DWORD DataUnk42;  
/*00AC*/ DWORD DataUnk43;  
/*00B0*/ DWORD DataUnk44;  
/*00B4*/ DWORD DataUnk45;  
/*00B8*/ DWORD DataUnk46;  
/*00BC*/ DWORD DataUnk47;  
/*00C0*/ DWORD DataUnk48;  
/*00C4*/ DWORD DataUnk49;  
/*00C8*/ DWORD DataUnk50;  
/*00CC*/ DWORD DataUnk51;  
/*00D0*/ DWORD DataUnk52;  
/*00D4*/ DWORD DataUnk53;  
/*00D8*/ DWORD DataUnk54;  

  // Jumps to Morrowind`s functions
  __declspec(noinline) MWTHISCALL NiDirectionalLight(void){JUMPEXTMEMBER(EXTMODBASE, 0x002D73F0 )}    // Constructor
  
  // Additional

};
//------------------------------------------------------------------------------------
class NiExtraData : public NiObject    // Size = 0x0018   // Real parent Constr=0x006C5790 ???
{
/*0008*/ DWORD        uiSize;        // Size of what ?
/*000C*/ LPSTR        Name;          // Unsure !
/*0010*/ PNiExtraData Next; 
/*0014*/ PVOID        Data;         

  // Jumps to Morrowind`s functions
  __declspec(noinline)              MWTHISCALL NiExtraData(PVOID Data){JUMPEXTMEMBER(EXTMODBASE, 0x000F1B10 )}                // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)              MWTHISCALL ~NiExtraData(void){JUMPEXTMEMBER(EXTMODBASE, 0x000F1B60 )}               // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PNiExtraData MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000F1B40 )}  // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional

};
//------------------------------------------------------------------------------------
class NiBBox    // Size = ???  (Virtual base class)   VFT=0x0074F25C
{
public:
 /*0000*/ PVOID (*pVFTable)[12];
 // No other data members, only virtual functions to work with data of derived classes
};
//------------------------------------------------------------------------------------
class NiUnionBBox : public NiBBox  // [UNION_BV]       // Size = 0x0020
{
public:
/*0004*/ PVOID (*pVFTable2)[1];   // Only Destructor
/*0008*/ PNiBoxBBox* Boundings;   // List of pointers to 'NiBoxBBox'
/*000C*/ DWORD fUnk02;      // Initial = 2
/*0010*/ DWORD ChildSlots;  // UNSURE
/*0014*/ DWORD Children;    // UNSURE
/*0018*/ DWORD fUnk05;      // Initial = 1
/*001C*/ DWORD fUnk06;

  // Jumps to Morrowind`s functions
  __declspec(noinline)              MWTHISCALL NiUnionBBox(PVOID Data){JUMPEXTMEMBER(EXTMODBASE, 0x002A1610 )}          // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)              MWTHISCALL ~NiUnionBBox(void){JUMPEXTMEMBER(EXTMODBASE, 0x002A16A0 )}               // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PNiUnionBBox MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x002A1680 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional

};
//------------------------------------------------------------------------------------
class NiBoxBBox : public NiBBox  // [BOX_BV]        // Size = 0x0040
{
public:
/*0004*/ VECTOR3 Unkn01;
/*0010*/ VECTOR3 Unkn02;
/*001C*/ VECTOR3 Unkn03;
/*0028*/ VECTOR3 Unkn04; 
/*0034*/ VECTOR3 Unkn00;

  // Jumps to Morrowind`s functions
  __declspec(noinline)            MWTHISCALL NiBoxBBox(VECTOR3 Unkn00,VECTOR3 Unkn01,VECTOR3 Unkn02,VECTOR3 Unkn03,VECTOR3 Unkn04){JUMPEXTMEMBER(EXTMODBASE, 0x00694C10 )}          // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)            MWTHISCALL ~NiBoxBBox(void){JUMPEXTMEMBER(EXTMODBASE, 0x002A16A0 )}               // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PNiBoxBBox MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x002A1680 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional

};
//------------------------------------------------------------------------------------
class CCameraView
{
public:
/*0000*/ PNiCamera pNiCameraNode;      // VFT=0x0074FAA8  
/*0004*/ DWORD     dwUnk00;            // Unsure, may be 'float' or 'PTR'
/*0008*/ float     CurrentFOV;         // You can set this manually and then call '_UpdateViewMatrix' or use '_ChangeFOV'
/*000C*/ float     fUnk01;
/*0010*/ float     fUnk02;
/*0014*/ long      SurfaceWidth;       // Width of the game screen
/*0018*/ long      SurfaceHeight;      // Height of the game screen

  // Jumps to Morrowind`s functions
  __declspec(noinline) void MWTHISCALL _ChangeFOV(float NewFOV){JUMPEXTMEMBER(EXTMODBASE, 0x0010F8C0 )}
  __declspec(noinline) void MWTHISCALL _UpdateViewMatrix(void){JUMPEXTMEMBER(EXTMODBASE, 0x00004960 )}
  
  // Additional

};
//------------------------------------------------------------------------------------
class CSceneGraph           // Size = 0x002C
{
public:
/*0000*/ PVOID       (*pVFTable)[7];  
/*0004*/ PVOID       pUnkPtr;
/*0008*/ PNiNode     pMainNode;
/*000C*/ PNiNode     pCameraNode;        // Class size = 0x00B0, Constructed=0x0043335B
/*0010*/ CCameraView Camera;            // VFT=0x0074FAA8  

  // Jumps to Morrowind`s functions
  __declspec(noinline) MWTHISCALL CSceneGraph(void){JUMPEXTMEMBER(EXTMODBASE, 0x00032E60 )}     // Constructor 
  
  // Additional

};


//====================================================================================
}
//====================================================================================
#endif