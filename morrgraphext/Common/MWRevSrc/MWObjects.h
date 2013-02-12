#pragma once

#ifndef MWObjectsH
#define MWObjectsH
    
#include "MWDefinitions.h"  
#include "MWMisc.h"    
//===========================================================================
//                               Game Objects
//---------------------------------------------------------------------------
namespace MORROWIND 
{
//====================================================================================
//                                  OBJECTS
//------------------------------------------------------------------------------------
class CBaseObject      // Size = 0x0010 (NotVirtual: Created in stack on 0x00414FD3 for example)   // VFT=9  Derived: BSGN,REGN,SSCR,ANIG,RACE,CLAS,SKIL,MGEF,FACT,GLOB,INFO,DIAL,GMST,LTEX,LAND,CELL,PGRD,SCPT,SOUN,SNDG,SPLL
{
public:
/*0000*/ PVOID (*pVFTable)[128];    // Accessing to element 'XXXX->pVFTable[0][Index]'  
/*0004*/ union{DWORD Type;BYTE Types[4];};  // REFR, CREA, and so on ...
/*0008*/ WORD  Flags;  
/*000C*/ PVOID DataUnk3;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)              MWTHISCALL CBaseObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000EE8D0 )}             // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)              MWTHISCALL ~CBaseObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000EE910 )}            // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCBaseObject MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000EE8F0 )}  // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  __declspec(noinline) void         MWTHISCALL _SetObjectFlags(DWORD Flags){JUMPEXTMEMBER(EXTMODBASE, 0x000EEC00 )} 
   
  // Additional
  bool MWTHISCALL IsSameType(DWORD Type){return (Type == this->Type);}
  bool MWTHISCALL IsSameType(PCBaseObject Type){return (Type->Type == this->Type);}
 
};
//------------------------------------------------------------------------------------
class CNamedObject : public CBaseObject    // Size = ??? (Possibly does not exist)
{
public:
/*0010*/ BYTE  StringID[32];  
/*0030*/ BYTE  StringName[32];  

  // Additional
  LPSTR MWTHISCALL GetID(void){return (LPSTR)&this->StringID;}
  LPSTR MWTHISCALL GetName(void){return (LPSTR)&this->StringName;}

};
//------------------------------------------------------------------------------------
class CLinkedObject : public CBaseObject   // Size = ???   (Virtual base class)  // VFT=79  Derived: REFR, SPEL, ENCH
{
public:
/*0010*/ PNiNode Model;       
/*0014*/ DWORD DataUnk5;          // list of what npc sees [e.g.: mudcrab, doormarker, ...] ?
/*0018*/ PCObjREFR Reference;     // [NPC_,CREA,=PCObjREFR Ref][BODY=0x00000001,STAT=0x00000064]
/*001C*/ PCLinkedObject Prev;  
/*0020*/ PCLinkedObject Next;  
/*0024*/ DWORD DataUnk9;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)                MWTHISCALL CLinkedObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000F0BE0 )}             // Constructor   
  __declspec(noinline)                MWTHISCALL ~CLinkedObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000F0CA0 )}            // Destructor    
  __declspec(noinline) PCLinkedObject MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000F0C80 )}    // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  __declspec(noinline) void           MWTHISCALL _SetObjectFlags(DWORD Flags){JUMPEXTMEMBER(EXTMODBASE, 0x000EED40 )} 
  __declspec(noinline) void           MWTHISCALL _SetObjectScale(float Scale, BOOL ValidateMax){JUMPEXTMEMBER(EXTMODBASE, 0x000F0EA0 )} // Returns 'float' or nothing ?   // Max=5
  __declspec(noinline) float          MWTHISCALL _GetObjectScale(void){JUMPEXTMEMBER(EXTMODBASE, 0x000F0E60 )}
  
  // Additional
   PNiNode   MWTHISCALL FindModelByName(LPSTR NodeName){return (this->Model)?(this->Model->_FindNodeByName(NodeName)):(NULL);}
  
};
//------------------------------------------------------------------------------------
class CVisualObject : public CLinkedObject    // Size = ???   (Virtual base class)  Derived: APPA,ARMO,BODY,BOOK,CLOT,INGR,LOCK,MISC,PROB,REPA,STAT,ALCH,LEVI,WEAP
{
public:
/*0028*/ PObjBSize Sizes;       
/*002C*/ LPSTR     ObjectID;      

  // Jumps to Morrowind`s functions
  __declspec(noinline)                MWTHISCALL CVisualObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000EEE80 )}                // Constructor   
  __declspec(noinline)                MWTHISCALL ~CVisualObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000EEF50 )}               // Destructor    
  __declspec(noinline) PCVisualObject MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000EEF30 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
   
};
//------------------------------------------------------------------------------------
class CActiveObject : public CVisualObject    // Size = ???   (Virtual base class)  Derived: ACTI,DOOR,LEVC,LIGH,CContainerObject   
{
public:
/*0030*/ PVOID  ModelNamesList;   // List of pointers for model{first is LPSTR ModelName}:0x0000008B-ClosingItem (Inits in constr of this class) possible some vector

  // Jumps to Morrowind`s functions
  __declspec(noinline)                MWTHISCALL CActiveObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000F0070 )}             // Constructor
  __declspec(noinline)                MWTHISCALL ~CActiveObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x000F0140 )}            // Destructor           // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCActiveObject MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000F0120 )}  // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  // Additional
  LPSTR MWTHISCALL GetObjectID(void){return this->ObjectID;}
  
};
//------------------------------------------------------------------------------------
class CContainerObject : public CActiveObject    // Size = ???   (Virtual base class)  Derived: CONT,CREA,NPC_,BaseCONT,BaseCREA,BaseNPC_,PlayerNPC_,sub_49D4F0,sub_4A41B0 [No initialization after 0x0068 (Size=0x006C ?)]
{
public:
/*0034*/ DWORD TypeFlags;     // DW*8
/*0038*/ DWORD DataUnk14;  
/*003C*/ DWORD invflags;  
/*0040*/ DWORD DataUnk16;      // TES3LISTITERATOR inventory; MWSE
/*0044*/ DWORD DataUnk17;  
/*0048*/ DWORD DataUnk18;  
/*004C*/ DWORD DataUnk19;  
/*0050*/ DWORD DataUnk20;  
/*0054*/ DWORD DataUnk21;     // DW*8
/*0058*/ PVOID (*pVFTableUnk)[128];   // Size = 0x0014
/*005C*/ DWORD DataUnk23;  
/*0060*/ DWORD DataUnk24;  
/*0064*/ DWORD DataUnk25;  
/*0068*/ DWORD DataUnk26;
  
  // Jumps to Morrowind`s functions
  __declspec(noinline)                   MWTHISCALL CContainerObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x00094B50 )}        // Constructor
  __declspec(noinline)                   MWTHISCALL ~CContainerObject(void){JUMPEXTMEMBER(EXTMODBASE, 0x00094BE0 )}       // Destructor           // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCContainerObject MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x00094BC0 )}  // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  // Additional
 
};
//------------------------------------------------------------------------------------
class CObjCONT : public CContainerObject          // Size = 0x0070
{
public:
/*006C*/ PCObjBaseCONT Base;          // UNCHECKED !! [Initialized to '0' in constructor]

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjCONT(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A4410 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjCONT(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A4450 )}                // Destructor    
  __declspec(noinline) PCObjCONT MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000A4430 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
    
};
//------------------------------------------------------------------------------------
class CObjBaseCONT : public CContainerObject      // Size = 0x007C 
{
public:
/*006C*/ LPSTR     ObjectName;
/*0070*/ LPSTR     ModelName;
/*0074*/ PCObjSCPT Script;         // UNSURE !
/*0078*/ DWORD     DataUnk30;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)               MWTHISCALL CObjBaseCONT(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A3BE0 )}             // Constructor   
  __declspec(noinline)               MWTHISCALL ~CObjBaseCONT(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A3CA0 )}            // Destructor    
  __declspec(noinline) PCObjBaseCONT MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000A3C80 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
  LPSTR MWTHISCALL GetName(void){return this->ObjectName;}
    
};
//------------------------------------------------------------------------------------
class CObjBaseNPC_ : public CContainerObject       // Size = 0x00F0
{
public:
/*006C*/ LPSTR     Unknow;            // UNSURE !
/*0070*/ LPSTR     ObjectName;           
/*0074*/ PCObjSCPT Script;
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
/*00B0*/ PCObjRACE Race;     
/*00B4*/ PCObjCLAS Class;    
/*00B8*/ DWORD DataUnk46;  
/*00BC*/ PCObjBODY BPHead;
/*00C0*/ PCObjBODY BPHair;
/*00C4*/ DWORD DataUnk49;  
/*00C8*/ DWORD DataUnk50;  
/*00CC*/ DWORD DataUnk51;  
/*00D0*/ DWORD DataUnk52;  
/*00D4*/ DWORD DataUnk53;  
/*00D8*/ DWORD DataUnk54;  
/*00DC*/ DWORD DataUnk55;  
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57; 
/*00E8*/ DWORD DataUnk58;  
/*00EC*/ DWORD DataUnk59;

  // Jumps to Morrowind`s functions
  __declspec(noinline)               MWTHISCALL CObjBaseNPC_(void){JUMPEXTMEMBER(EXTMODBASE, 0x000D5980 )}             // Constructor   
  __declspec(noinline)               MWTHISCALL ~CObjBaseNPC_(void){JUMPEXTMEMBER(EXTMODBASE, 0x000D5B20 )}            // Destructor    
  __declspec(noinline) PCObjBaseNPC_ MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000D5B00 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
  bool  MWTHISCALL IsFemale(void){return (this->TypeFlags & 0x00000001);}    // Flags offset=0x0034
  LPSTR MWTHISCALL GetName(void){return this->ObjectName;}
 
};
//------------------------------------------------------------------------------------
class CObjBaseCREA : public CContainerObject    // Size = 0x00E8      // npc objectsWorn List
{
public:
/*006C*/ LPSTR     ModelName;            
/*0070*/ LPSTR     ObjectName;          
/*0074*/ PCObjSCPT     Script;
/*0078*/ PCObjBaseCREA SndGenCrea;  
/*007C*/ DWORD         CreatureType;   
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
/*00DC*/ DWORD DataUnk55;  
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)               MWTHISCALL CObjBaseCREA(void){JUMPEXTMEMBER(EXTMODBASE, 0x0009C0C0 )}             // Constructor   
  __declspec(noinline)               MWTHISCALL ~CObjBaseCREA(void){JUMPEXTMEMBER(EXTMODBASE, 0x0009C280 )}            // Destructor    
  __declspec(noinline) PCObjBaseCREA MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x0009C260 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
  LPSTR MWTHISCALL GetName(void){return this->ObjectName;}
  LPSTR MWTHISCALL GetCreatureType(void){return CreatureTypes[this->CreatureType];}

};
//------------------------------------------------------------------------------------
class CObjNPC_ : public CContainerObject          // Size = 0x0078
{
public:
/*006C*/ PCObjBaseNPC_ Base;   
/*0070*/ DWORD DataUnk28;       // InitialWORD[0x70] 0x0032, InitialBYTE[0x72] 0x32 
/*0074*/ DWORD DataUnk29;     

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjNPC_(void){JUMPEXTMEMBER(EXTMODBASE, 0x000D82B0 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjNPC_(void){JUMPEXTMEMBER(EXTMODBASE, 0x000D83D0 )}                // Destructor    
  __declspec(noinline) PCObjNPC_ MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000D82E0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
  LPSTR         MWTHISCALL GetBaseID(void){return this->Base->GetObjectID();}
  
};
//------------------------------------------------------------------------------------
class CObjCREA : public CContainerObject    // Size = 0x008C     
{
public:
/*006C*/ PCObjBaseCREA Base;  
/*0070*/ DWORD DataUnk28;  
/*0074*/ DWORD DataUnk29;  
/*0078*/ DWORD DataUnk30;  
/*007C*/ DWORD DataUnk31;  
/*0080*/ DWORD DataUnk32;  
/*0084*/ DWORD DataUnk33;  
/*0088*/ DWORD DataUnk34;     

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjCREA(void){JUMPEXTMEMBER(EXTMODBASE, 0x0009D410 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjCREA(void){JUMPEXTMEMBER(EXTMODBASE, 0x0009D5F0 )}                // Destructor    
  __declspec(noinline) PCObjCREA MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x0009D4D0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
  LPSTR         MWTHISCALL GetBaseID(void){return this->Base->GetObjectID();}
};
//------------------------------------------------------------------------------------
class CObjPlayerNPC_ : public CObjNPC_
{
public:
  // Jumps to Morrowind`s functions
  __declspec(noinline)   MWTHISCALL CObjPlayerNPC_(void){JUMPEXTMEMBER(EXTMODBASE, 0x000D8300 )}                 // Constructor   
  
  // Additional

};
//------------------------------------------------------------------------------------
class CObjREFR : public CLinkedObject        // Size = 0x0050 (80)     
{
public:
/*0028*/ PCActiveObject Object;       // From here 7 DWORDs  (Some 'struct' ?)      // CHECK THIS TYPE!!!!!
/*002C*/ DWORD DataUnk11;  
/*0030*/ DWORD DataUnk12;  
/*0034*/ FLOAT Unknow;  
/*0038*/ FLOAT PosX;  
/*003C*/ FLOAT PosY;  
/*0040*/ FLOAT PosZ;  
/*0044*/ PCAListItem Attachments;      // (NifModelParts ?)  // Sizes: while(pSizeClass && ((PDWORD)pSizeClass)[0])pSizeClass = (PVOID)((PDWORD)pSizeClass)[1]; return (pSizeClass)?((PVOID)((PDWORD)pSizeClass)[2]):(NULL);
/*0048*/ DWORD DataUnk18;  
/*004C*/ DWORD DataUnk19; 

  // Jumps to Morrowind`s functions
  __declspec(noinline)              MWTHISCALL CObjREFR(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E4510 )}                 // Constructor   
  __declspec(noinline)              MWTHISCALL ~CObjREFR(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E45C0 )}                // Destructor    
  __declspec(noinline) PCObjREFR    MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000E45A0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  __declspec(noinline) void         MWTHISCALL _ActivateBy(PCObjREFR Activator, BYTE UnkFlag){JUMPEXTMEMBER(EXTMODBASE, 0x000E9610 )}   // Set Flag to 1
  __declspec(noinline) PNiNode      MWTHISCALL _GetActiveModel(void){JUMPVFTMEMBER(11)}  
  __declspec(noinline) PCAListItem  MWTHISCALL _AddNewItem(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E4D00 )}  
  __declspec(noinline) PCAListItem  MWTHISCALL _GetEmptyItem(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E4D50 )} 
  __declspec(noinline) PCAnimation  MWTHISCALL _GetAnimItem(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E4CE0 )}    // Type=0x00
  __declspec(noinline) PCObjMACT    MWTHISCALL _GetActorItem(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E5750 )}   // Type=0x08  
 
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem1(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E4CE0 )}   // Type=0x01 
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem2(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E5170 )}   // Type=0x02 
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem3(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E5220 )}   // Type=0x03  // Called when new door focused
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem5(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E5340 )}   // Type=0x05  // Called when door focused [list of pointers]
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem6(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E5460 )}   // Type=0x06 
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem9(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E54F0 )}   // Type=0x09  // BOOL 
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem10(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E58D0 )}  // Type=0x0A  // &CListItem::Data    // ItemSize = 32 ???
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem61(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E6FF0 )}  // Type=0x06  // [EAX+14h]
  __declspec(noinline) PVOID MWTHISCALL _GetUnknItem62(void){JUMPEXTMEMBER(EXTMODBASE, 0x000E7020 )}  // Type=0x06  // [EAX+18h] 
   
  // Additional
   PCAnimation  MWTHISCALL GetItemData(void);  
   PCAListItem  MWTHISCALL GetEmptyItem(void);
   void         MWTHISCALL SetPosition(float PosX, float PosY, float PosZ);
   void         MWTHISCALL GetPosition(float *PosX, float *PosY, float *PosZ);
   float        MWTHISCALL GetDistance(PCObjREFR TargetRef);

   PCObjNPC_ MWTHISCALL GetObjAsNPC_(void){return (this->Object && this->Object->IsSameType(otNPC_)?((PCObjNPC_)this->Object):(NULL));}
   PCObjCREA MWTHISCALL GetObjAsCREA(void){return (this->Object && this->Object->IsSameType(otCREA)?((PCObjCREA)this->Object):(NULL));}

};
//------------------------------------------------------------------------------------
class CObjSPEL : public CLinkedObject             // Size = 0x00F8
{
public:
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
/*00DC*/ DWORD DataUnk55;  
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57;  
/*00E8*/ DWORD DataUnk58;  
/*00EC*/ DWORD DataUnk59;  
/*00F0*/ DWORD DataUnk60;  
/*00F4*/ DWORD DataUnk61;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjSPEL(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A9FD0 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjSPEL(void){JUMPEXTMEMBER(EXTMODBASE, 0x000AA0E0 )}                // Destructor    
  __declspec(noinline) PCObjSPEL MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000AA0C0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
   
};
//------------------------------------------------------------------------------------
class CObjENCH : public CLinkedObject             // Size = 0x00F8
{
public:
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
/*00DC*/ DWORD DataUnk55;  
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57;  
/*00E8*/ DWORD DataUnk58;  
/*00EC*/ DWORD DataUnk59;  
/*00F0*/ DWORD DataUnk60;  
/*00F4*/ DWORD DataUnk61;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjENCH(void){JUMPEXTMEMBER(EXTMODBASE, 0x000AADA0 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjENCH(void){JUMPEXTMEMBER(EXTMODBASE, 0x000AAEA0 )}                // Destructor    
  __declspec(noinline) PCObjENCH MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000AAE80 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
   
};
//------------------------------------------------------------------------------------
class CObjRACE : public CNamedObject   // Size = 01D8h
{
public:
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
/*00DC*/ DWORD DataUnk55;  
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57;  
/*00E8*/ DWORD DataUnk58;  
/*00EC*/ DWORD DataUnk59;  
/*00F0*/ DWORD DataUnk60;  
/*00F4*/ DWORD DataUnk61;  
/*00F8*/ DWORD DataUnk62;  
/*00FC*/ DWORD DataUnk63;  
/*0100*/ DWORD DataUnk64;  
/*0104*/ DWORD DataUnk65;  
/*0108*/ DWORD DataUnk66;  
/*010C*/ DWORD DataUnk67;  
/*0110*/ DWORD DataUnk68;  
/*0114*/ DWORD DataUnk69;  
/*0118*/ DWORD DataUnk70;  
/*011C*/ DWORD DataUnk71;  
/*0120*/ DWORD DataUnk72;  
/*0124*/ DWORD DataUnk73;  
/*0128*/ DWORD DataUnk74;  
/*012C*/ DWORD DataUnk75;  
/*0130*/ DWORD DataUnk76;  
/*0134*/ DWORD DataUnk77;  
/*0138*/ DWORD DataUnk78;  
/*013C*/ DWORD DataUnk79;  
/*0140*/ DWORD DataUnk80;  
/*0144*/ DWORD DataUnk81;  
/*0148*/ DWORD DataUnk82;  
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
/*01D4*/ DWORD DataUnk117;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjRACE(BOOL UnkFlag){JUMPEXTMEMBER(EXTMODBASE, 0x000A7530 )}         // Constructor
  __declspec(noinline)           MWTHISCALL ~CObjRACE(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A7680 )}                // Destructor           // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCObjRACE MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000A7660 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
    
  // Additional
 
};
//------------------------------------------------------------------------------------
class CObjCLAS : public CNamedObject    // Size = 0x0094
{
public:
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

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjCLAS(BOOL UnkFlag){JUMPEXTMEMBER(EXTMODBASE, 0x000A7EE0 )}    // Constructor
  __declspec(noinline)           MWTHISCALL ~CObjCLAS(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A7F90 )}           // Destructor           // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCObjCLAS MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000A7F50 )}        // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
    
  // Additional
  
};
//------------------------------------------------------------------------------------
class CObjSOUN : public CBaseObject           // Size = 0x0058     // Derived from something
{
public:
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

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjSOUN(BOOL UnkFlag){JUMPEXTMEMBER(EXTMODBASE, 0x00110490 )}    // Constructor
  __declspec(noinline)           MWTHISCALL ~CObjSOUN(void){JUMPEXTMEMBER(EXTMODBASE, 0x00110500 )}           // Destructor           // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCObjSOUN MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x001104E0 )}        // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
    
  // Additional
  
};
//------------------------------------------------------------------------------------
class CObjDIAL : public CBaseObject          // Size = 0x0030
{
public:
/*0010*/ LPSTR EntryName;  
/*0014*/ DWORD DataUnk5;  
/*0018*/ DWORD DataUnk6;  
/*001C*/ DWORD DataUnk7;  
/*0020*/ DWORD DataUnk8;  
/*0024*/ DWORD DataUnk9;  
/*0028*/ DWORD DataUnk10;  
/*002C*/ DWORD DataUnk11;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjDIAL(BOOL UnkFlag){JUMPEXTMEMBER(EXTMODBASE, 0x000B2230 )}    // Constructor
  __declspec(noinline)           MWTHISCALL ~CObjDIAL(void){JUMPEXTMEMBER(EXTMODBASE, 0x000B22D0 )}           // Destructor           // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCObjDIAL MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000B22B0 )}        // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  // Additional
 
};
//------------------------------------------------------------------------------------
class CObjINFO : public CBaseObject          // Size = 0x002C
{
public:
/*0010*/ DWORD DataUnk4;  
/*0014*/ DWORD DataUnk5;  
/*0018*/ DWORD DataUnk6;  
/*001C*/ DWORD DataUnk7;  
/*0020*/ DWORD DataUnk8;  
/*0024*/ DWORD DataUnk9;  
/*0028*/ DWORD DataUnk10;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjINFO(void){JUMPEXTMEMBER(EXTMODBASE, 0x000AE7A0 )}        // Constructor
  __declspec(noinline)           MWTHISCALL ~CObjINFO(void){JUMPEXTMEMBER(EXTMODBASE, 0x000AE8A0 )}       // Destructor           // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCObjINFO MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000AE880 )}        // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  // Additional
 
};
//------------------------------------------------------------------------------------
class CObjSCPT : public CBaseObject      // Size = 0x0070
{
public:
/*0010*/ BYTE  StringName[32]; // Begin of ScriptHeader  Size = 0x0034     
/*0030*/ DWORD DataUnk12;  
/*0034*/ DWORD DataUnk13;  
/*0038*/ DWORD DataUnk14;  
/*003C*/ DWORD ByteCodeLen;  
/*0040*/ DWORD DataUnk16;  
/*0044*/ DWORD DataUnk17;  
/*0048*/ DWORD DataUnk18;  
/*004C*/ DWORD DataUnk19;  
/*0050*/ DWORD DataUnk20;  
/*0054*/ DWORD DataUnk21;   
/*0058*/ PVOID pByteCode;  
/*005C*/ DWORD DataUnk23;  
/*0060*/ DWORD DataUnk24;  
/*0064*/ DWORD DataUnk25;  
/*0068*/ DWORD DataUnk26;  
/*006C*/ DWORD DataUnk27;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjSCPT(void){JUMPEXTMEMBER(EXTMODBASE, 0x000FD830 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjSCPT(void){JUMPEXTMEMBER(EXTMODBASE, 0x000FD8E0 )}                // Destructor    
  __declspec(noinline) PCObjSCPT MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000FD8C0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
   
};
//------------------------------------------------------------------------------------
class CObjBODY : public CVisualObject      // Size = 0x0040
{
public:
/*0030*/ LPSTR ModelName;
/*0034*/ LPSTR PartName;     
/*0038*/ DWORD DataUnk14;  
/*003C*/ DWORD DataUnk15;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjBODY(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A1340 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjBODY(void){JUMPEXTMEMBER(EXTMODBASE, 0x000A1410 )}                // Destructor    
  __declspec(noinline) PCObjBODY MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000A13F0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
   
};
//====================================================================================
//                              ACTOR Classes
//------------------------------------------------------------------------------------
#pragma pack( push, 1 )
//------------------------------------------------------------------------------------
class CObjMMBL : public CBaseObject  // Size = ???  (Base virtual class)  [MobileObject] Derived: MACT, MPRJ
{
public:
/*0010*/ DWORD MOBFlags;    // Moving state flags ? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*0014*/ PCObjREFR ObjectRef;  
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
/*007C*/ DWORD UnkFlag;       // [bool] keep alignment

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjMMBL(PCObjREFR ObjRef){JUMPEXTMEMBER(EXTMODBASE, 0x0015E3E0 )}     // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjMMBL(void){JUMPEXTMEMBER(EXTMODBASE, 0x0015E490 )}                // Destructor    
  __declspec(noinline) PCObjMMBL MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x00162DB0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  __declspec(noinline) void      MWTHISCALL _ResetMovements(void){JUMPEXTMEMBER(EXTMODBASE, 0x00161A20 )}
  
  // Additional
            MWTHISCALL CObjMMBL(void){}

};
//------------------------------------------------------------------------------------
class CObjMPRJ : public CObjMMBL     // Size = 0x00AC   [MobileProjectile]
{
public:
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

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjMPRJ(PCObjREFR ObjRef,DWORD Unkn00){JUMPEXTMEMBER(EXTMODBASE, 0x00172340 )}    // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjMPRJ(void){JUMPEXTMEMBER(EXTMODBASE, 0x00172600 )}                        // Destructor    
  __declspec(noinline) PCObjMPRJ MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x00176230 )}           // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional

};
//------------------------------------------------------------------------------------
class CObjMPSP : public CObjMPRJ         // Size = 0x00B4 
{
public:
/*00AC*/ DWORD DataUnk43;  
/*00B0*/ DWORD DataUnk44;  

 // See offsets: 00175833, 00175283 for constructors
 
};
//------------------------------------------------------------------------------------
class CObjMACT : public CObjMMBL    // Size = ???  (Base virtual class)  Derived: MACH, MACR    [Actor]   // VFTableAddr = 0x0074AB4C, Functions = 66
{
public:
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
/*00CC*/ float DataUnk51;   
/*00D0*/ DWORD DataUnk52;  
/*00D4*/ DWORD DataUnk53;  
/*00D8*/ DWORD DataUnk54;  
/*00DC*/ char  AIState;  	  // Unconfirmed
/*00DD*/ BYTE  bUndef00;
/*00DE*/ BYTE  bUndef01;
/*00DF*/ BYTE  bUndef02;
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57;  
/*00E8*/ DWORD DataUnk58;  
/*00EC*/ PCObjMACT CombatTarget;  
/*00F0*/ PCObjMACT UnknowTarget; 
/*00F4*/ DWORD DataUnk61;  
/*00F8*/ DWORD DataUnk62;  
/*00FC*/ DWORD DataUnk63;  
/*0100*/ DWORD DataUnk64;  
/*0104*/ DWORD DataUnk65;  
/*0108*/ DWORD DataUnk66;  
/*010C*/ DWORD DataUnk67;  
/*0110*/ DWORD DataUnk68;  
/*0114*/ DWORD DataUnk69;  
/*0118*/ DWORD DataUnk70;  
/*011C*/ DWORD DataUnk71;  
/*0120*/ DWORD DataUnk72;  
/*0124*/ DWORD DataUnk73;  
/*0128*/ DWORD DataUnk74;  
/*012C*/ DWORD DataUnk75;  
/*0130*/ DWORD DataUnk76;  
/*0134*/ DWORD DataUnk77;  
/*0138*/ DWORD DataUnk78;  
/*013C*/ DWORD DataUnk79;  
/*0140*/ DWORD DataUnk80;  
/*0144*/ DWORD DataUnk81;  
/*0148*/ DWORD DataUnk82;  
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
/*01D4*/ DWORD DataUnk117;  
/*01D8*/ DWORD DataUnk118;  
/*01DC*/ DWORD DataUnk119;  
/*01E0*/ DWORD DataUnk120;  
/*01E4*/ DWORD DataUnk121;  
/*01E8*/ DWORD DataUnk122;  
/*01EC*/ DWORD DataUnk123;  
/*01F0*/ DWORD DataUnk124;  
/*01F4*/ DWORD DataUnk125;  
/*01F8*/ DWORD DataUnk126;  
/*01FC*/ DWORD DataUnk127;  
/*0200*/ DWORD DataUnk128;  
/*0204*/ DWORD DataUnk129;  
/*0208*/ DWORD DataUnk130;  
/*020C*/ DWORD DataUnk131;  
/*0210*/ DWORD DataUnk132;  
/*0214*/ DWORD DataUnk133;  
/*0218*/ DWORD DataUnk134;  
/*021C*/ DWORD DataUnk135;  
/*0220*/ DWORD DataUnk136;  
/*0224*/ DWORD DataUnk137;
/*0228*/ DWORD DataUnk138;  
/*022C*/ DWORD DataUnk139;  
/*0230*/ DWORD DataUnk140;  
/*0234*/ DWORD DataUnk141;  
/*0238*/ DWORD DataUnk142;  
/*023C*/ DWORD DataUnk143;  
/*0240*/ DWORD DataUnk144;  
/*0244*/ PCBaseCamera Camera;    // !!!UNSURE ABOUT THE NAME!!!
/*0248*/ DWORD DataUnk146;  
/*024C*/ DWORD DataUnk147;  
/*0250*/ DWORD DataUnk148;  
/*0254*/ DWORD DataUnk149;  
/*0258*/ DWORD DataUnk150;  
/*025C*/ DWORD DataUnk151;  
/*0260*/ DWORD DataUnk152;  
/*0264*/ DWORD DataUnk153;  
/*0268*/ DWORD DataUnk154;  
/*026C*/ DWORD DataUnk155;  
/*0270*/ DWORD DataUnk156;  
/*0274*/ DWORD DataUnk157;  
/*0278*/ DWORD DataUnk158;  
/*027C*/ DWORD DataUnk159;  
/*0280*/ DWORD DataUnk160;  
/*0284*/ DWORD DataUnk161;  
/*0288*/ DWORD DataUnk162;  
/*028C*/ DWORD DataUnk163;  
/*0290*/ DWORD DataUnk164;  
/*0294*/ DWORD DataUnk165;  
/*0298*/ DWORD DataUnk166;  
/*029C*/ DWORD DataUnk167;  
/*02A0*/ DWORD DataUnk168;  
/*02A4*/ DWORD DataUnk169;  
/*02A8*/ DWORD DataUnk170;  
/*02AC*/ DWORD DataUnk171;  
/*02B0*/ DWORD DataUnk172;  
/*02B4*/ DWORD DataUnk173;  // Parameters class!!! (struct {PVOID VTblPtr, float MaxHealth, float CurrentHealth, ...})
/*02B8*/ DWORD DataUnk174;  
/*02BC*/ DWORD DataUnk175;  
/*02C0*/ DWORD DataUnk176;  
/*02C4*/ DWORD DataUnk177;  
/*02C8*/ DWORD DataUnk178;  
/*02CC*/ DWORD DataUnk179;  
/*02D0*/ DWORD DataUnk180;  
/*02D4*/ DWORD DataUnk181;  
/*02D8*/ DWORD DataUnk182;  
/*02DC*/ DWORD DataUnk183;  
/*02E0*/ DWORD DataUnk184;  
/*02E4*/ DWORD DataUnk185;  
/*02E8*/ DWORD DataUnk186;  
/*02EC*/ DWORD DataUnk187;  
/*02F0*/ DWORD DataUnk188;  
/*02F4*/ DWORD DataUnk189;  
/*02F8*/ DWORD DataUnk190;  
/*02FC*/ DWORD DataUnk191;  
/*0300*/ DWORD DataUnk192;  
/*0304*/ DWORD DataUnk193;  
/*0308*/ DWORD DataUnk194;  
/*030C*/ DWORD DataUnk195;  
/*0310*/ DWORD DataUnk196;  
/*0314*/ DWORD DataUnk197;  
/*0318*/ DWORD DataUnk198;  
/*031C*/ DWORD DataUnk199;  
/*0320*/ DWORD DataUnk200;  
/*0324*/ DWORD DataUnk201;  
/*0328*/ DWORD DataUnk202;  
/*032C*/ DWORD DataUnk203;  
/*0330*/ DWORD DataUnk204;  
/*0334*/ DWORD DataUnk205;  
/*0338*/ DWORD DataUnk206;  
/*033C*/ DWORD DataUnk207;  
/*0340*/ DWORD DataUnk208;  
/*0344*/ DWORD DataUnk209;  
/*0348*/ DWORD DataUnk210;  
/*034C*/ DWORD DataUnk211;  
/*0350*/ DWORD DataUnk212;  
/*0354*/ DWORD DataUnk213;  
/*0358*/ DWORD DataUnk214;  
/*035C*/ DWORD DataUnk215;  
/*0360*/ DWORD DataUnk216;  
/*0364*/ DWORD DataUnk217;  
/*0368*/ DWORD DataUnk218;  
/*036C*/ DWORD DataUnk219;  
/*0370*/ DWORD DataUnk220;  
/*0374*/ DWORD DataUnk221;  
/*0378*/ DWORD DataUnk222;  
/*037C*/ DWORD DataUnk223;  
/*0380*/ DWORD DataUnk224;  
/*0384*/ DWORD DataUnk225;  
/*0388*/ DWORD DataUnk226;  
/*038C*/ DWORD DataUnk227;  
/*0390*/ DWORD DataUnk228;  
/*0394*/ DWORD DataUnk229;  
/*0398*/ DWORD DataUnk230;  
/*039C*/ DWORD DataUnk231;  
/*03A0*/ DWORD DataUnk232;


  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjMACT(PCObjREFR ObjRef){JUMPEXTMEMBER(EXTMODBASE, 0x0011F600 )}    // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjMACT(void){JUMPEXTMEMBER(EXTMODBASE, 0x0011F970 )}               // Destructor    
  __declspec(noinline) PCObjMACT MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x0011F940 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  // Additional
               MWTHISCALL CObjMACT(void){}
 
 bool          MWTHISCALL SetActorSneaking(bool Sneak);
 bool          MWTHISCALL IsActorSneaking(void){return ((this->Flags >> 10) & 1);}
 bool          MWTHISCALL IsActorNPC(void){return this->IsSameType(otMACH);}
 bool          MWTHISCALL IsActorCreature(void){return this->IsSameType(otMACR);}
 bool          MWTHISCALL IsActorPlayer(void){return this->IsSameType(otMACP);}
 bool          MWTHISCALL IsActorFemale(void){return ((this->IsActorNPC()||this->IsActorPlayer())&&this->GetObjectAsNPC()->Base->IsFemale());}
 PCObjREFR     MWTHISCALL GetReference(void){return this->ObjectRef;}
 PCObjMACT     MWTHISCALL GetCombatTargetActor(void){return this->CombatTarget;}
 PCObjCREA     MWTHISCALL GetObjectAsCreature(void){return (this->IsActorCreature())?((PCObjCREA)this->ObjectRef->Object):(NULL);}
 PCObjNPC_     MWTHISCALL GetObjectAsNPC(void){return (this->IsSameType(otMACH)||this->IsSameType(otMACP))?((PCObjNPC_)this->ObjectRef->Object):(NULL);}
 LPSTR         MWTHISCALL GetActorID(void){return this->GetReference()->Object->GetObjectID();}
 LPSTR         MWTHISCALL GetActorBaseID(void){return (this->IsActorNPC()||this->IsActorPlayer())?(this->GetObjectAsNPC()->GetBaseID()):(this->GetObjectAsCreature()->GetBaseID());}
 LPSTR         MWTHISCALL GetActorRaceID(void){return (this->IsActorNPC()||this->IsActorPlayer())?(this->GetObjectAsNPC()->Base->Race->GetID()):(this->GetObjectAsCreature()->Base->GetCreatureType());}                                     
 LPSTR         MWTHISCALL GetActorBaseName(void){return (this->IsActorNPC()||this->IsActorPlayer())?(this->GetObjectAsNPC()->Base->GetName()):(this->GetObjectAsCreature()->Base->GetName());}

};
//------------------------------------------------------------------------------------
class CObjMACR : public CObjMACT     // Creature          // Size = 0x03d8   // VFTableAddr = 0x0074AFA4, Functions = 67
{
public:
/*03A4*/ DWORD DataUnk233;  
/*03A8*/ DWORD DataUnk234;  
/*03AC*/ DWORD DataUnk235;  
/*03B0*/ DWORD DataUnk236;  
/*03B4*/ DWORD DataUnk237;  
/*03B8*/ DWORD DataUnk238;  
/*03BC*/ DWORD DataUnk239;  
/*03C0*/ DWORD DataUnk240;  
/*03C4*/ DWORD DataUnk241;  
/*03C8*/ DWORD DataUnk242;  
/*03CC*/ DWORD DataUnk243;  
/*03D0*/ DWORD DataUnk244;  
/*03D4*/ DWORD DataUnk245;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjMACR(PCObjREFR ObjRef){JUMPEXTMEMBER(EXTMODBASE, 0x0015A2E0 )}    // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjMACR(void){JUMPEXTMEMBER(EXTMODBASE, 0x0015A390 )}               // Destructor    
  __declspec(noinline) PCObjMACR MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x0015A370 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  // Additional
  
};
//------------------------------------------------------------------------------------
class CObjMACH : public CObjMACT    // Character        // Size = 0x056C    // VFTableAddr = 0x0074AE6C, Functions = 68
{
public:
/*03A4*/ DWORD DataUnk233;  
/*03A8*/ DWORD DataUnk234;  
/*03AC*/ DWORD DataUnk235;  
/*03B0*/ DWORD DataUnk236;  
/*03B4*/ DWORD DataUnk237;  
/*03B8*/ DWORD DataUnk238;  
/*03BC*/ DWORD DataUnk239;  
/*03C0*/ DWORD DataUnk240;  
/*03C4*/ DWORD DataUnk241;  
/*03C8*/ DWORD DataUnk242;  
/*03CC*/ DWORD DataUnk243;  
/*03D0*/ DWORD DataUnk244;  
/*03D4*/ DWORD DataUnk245;  
/*03D8*/ DWORD DataUnk246;  
/*03DC*/ DWORD DataUnk247;  
/*03E0*/ DWORD DataUnk248;  
/*03E4*/ DWORD DataUnk249;  
/*03E8*/ DWORD DataUnk250;  
/*03EC*/ DWORD DataUnk251;  
/*03F0*/ DWORD DataUnk252;  
/*03F4*/ DWORD DataUnk253;  
/*03F8*/ DWORD DataUnk254;  
/*03FC*/ DWORD DataUnk255;  
/*0400*/ DWORD DataUnk256;  
/*0404*/ DWORD DataUnk257;  
/*0408*/ DWORD DataUnk258;  
/*040C*/ DWORD DataUnk259;  
/*0410*/ DWORD DataUnk260;  
/*0414*/ DWORD DataUnk261;  
/*0418*/ DWORD DataUnk262;  
/*041C*/ DWORD DataUnk263;  
/*0420*/ DWORD DataUnk264;  
/*0424*/ DWORD DataUnk265;  
/*0428*/ DWORD DataUnk266;  
/*042C*/ DWORD DataUnk267;  
/*0430*/ DWORD DataUnk268;  
/*0434*/ DWORD DataUnk269;  
/*0438*/ DWORD DataUnk270;  
/*043C*/ DWORD DataUnk271;  
/*0440*/ DWORD DataUnk272;  
/*0444*/ DWORD DataUnk273;  
/*0448*/ DWORD DataUnk274;  
/*044C*/ DWORD DataUnk275;  
/*0450*/ DWORD DataUnk276;  
/*0454*/ DWORD DataUnk277;  
/*0458*/ DWORD DataUnk278;  
/*045C*/ DWORD DataUnk279;  
/*0460*/ DWORD DataUnk280;  
/*0464*/ DWORD DataUnk281;  
/*0468*/ DWORD DataUnk282;  
/*046C*/ DWORD DataUnk283;  
/*0470*/ DWORD DataUnk284;  
/*0474*/ DWORD DataUnk285;  
/*0478*/ DWORD DataUnk286;  
/*047C*/ DWORD DataUnk287;  
/*0480*/ DWORD DataUnk288;  
/*0484*/ DWORD DataUnk289;  
/*0488*/ DWORD DataUnk290;  
/*048C*/ DWORD DataUnk291;  
/*0490*/ DWORD DataUnk292;  
/*0494*/ DWORD DataUnk293;  
/*0498*/ DWORD DataUnk294;  
/*049C*/ DWORD DataUnk295;  
/*04A0*/ DWORD DataUnk296;  
/*04A4*/ DWORD DataUnk297;  
/*04A8*/ DWORD DataUnk298;  
/*04AC*/ DWORD DataUnk299;  
/*04B0*/ DWORD DataUnk300;  
/*04B4*/ DWORD DataUnk301;  
/*04B8*/ DWORD DataUnk302;  
/*04BC*/ DWORD DataUnk303;  
/*04C0*/ DWORD DataUnk304;  
/*04C4*/ DWORD DataUnk305;  
/*04C8*/ DWORD DataUnk306;  
/*04CC*/ DWORD DataUnk307;  
/*04D0*/ DWORD DataUnk308;  
/*04D4*/ DWORD DataUnk309;  
/*04D8*/ DWORD DataUnk310;  
/*04DC*/ DWORD DataUnk311;  
/*04E0*/ DWORD DataUnk312;  
/*04E4*/ DWORD DataUnk313;  
/*04E8*/ DWORD DataUnk314;  
/*04EC*/ DWORD DataUnk315;  
/*04F0*/ DWORD DataUnk316;  
/*04F4*/ DWORD DataUnk317;  
/*04F8*/ DWORD DataUnk318;  
/*04FC*/ DWORD DataUnk319;  
/*0500*/ DWORD DataUnk320;  
/*0504*/ DWORD DataUnk321;  
/*0508*/ DWORD DataUnk322;  
/*050C*/ DWORD DataUnk323;  
/*0510*/ DWORD DataUnk324;  
/*0514*/ DWORD DataUnk325;  
/*0518*/ DWORD DataUnk326;  
/*051C*/ DWORD DataUnk327;  
/*0520*/ DWORD DataUnk328;  
/*0524*/ DWORD DataUnk329;  
/*0528*/ DWORD DataUnk330;  
/*052C*/ DWORD DataUnk331;  
/*0530*/ DWORD DataUnk332;  
/*0534*/ DWORD DataUnk333;  
/*0538*/ DWORD DataUnk334;  
/*053C*/ DWORD DataUnk335;  
/*0540*/ DWORD DataUnk336;  
/*0544*/ DWORD DataUnk337;  
/*0548*/ DWORD DataUnk338;  
/*054C*/ DWORD DataUnk339;  
/*0550*/ DWORD DataUnk340;  
/*0554*/ DWORD DataUnk341;  
/*0558*/ DWORD DataUnk342;  
/*055C*/ DWORD DataUnk343; 
/*0560*/ DWORD DataUnk344;  
/*0564*/ DWORD DataUnk345;  
/*0568*/ DWORD DataUnk346;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjMACH(PCObjREFR ObjRef){JUMPEXTMEMBER(EXTMODBASE, 0x0014C930 )}     // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjMACH(void){JUMPEXTMEMBER(EXTMODBASE, 0x0014CA50 )}                // Destructor    
  __declspec(noinline) PCObjMACH MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x0014CA20 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  __declspec(noinline) void      MWTHISCALL _AIDoActorAttack(void){JUMPEXTMEMBER(EXTMODBASE, 0x00159120 )}   

  // Additional
       MWTHISCALL CObjMACH(void){}
       
};
//------------------------------------------------------------------------------------
class CObjMACP : public CObjMACH     // Player     // Size = 0x0694   // VFTableAddr = 0x0074B174, Functions = 68
{
public:
/*056C*/ DWORD DataUnk347;  
/*0570*/ DWORD DataUnk348;  
/*0574*/ DWORD DataUnk349;  
/*0578*/ DWORD DataUnk350;  
/*057C*/ DWORD DataUnk351;  
/*0580*/ DWORD DataUnk352;  
/*0584*/ DWORD DataUnk353;  
/*0588*/ DWORD DataUnk354;  
/*058C*/ DWORD DataUnk355;  
/*0590*/ DWORD DataUnk356;  
/*0594*/ DWORD DataUnk357;  
/*0598*/ DWORD DataUnk358;  
/*059C*/ DWORD DataUnk359;  
/*05A0*/ DWORD DataUnk360;  
/*05A4*/ DWORD DataUnk361;  
/*05A8*/ DWORD DataUnk362;  
/*05AC*/ DWORD DataUnk363;  
/*05B0*/ BYTE  KeyboardDisabled;        
/*05B1*/ BYTE  JumpingDisabled;
/*05B2*/ BYTE  MouseDisabled;
/*05B3*/ BYTE  ViewSwitchDisabled;
/*05B4*/ BYTE  bUnk00; 
/*05B5*/ BYTE  bUnk01;
/*05B6*/ BYTE  bUnk02;
/*05B7*/ BYTE  bUnk03;
/*05B8*/ BYTE  bUnk04; 
/*05B9*/ BYTE  bUnk05;
/*05BA*/ BYTE  bUnk06;
/*05BB*/ BYTE  bUnk07;
/*05BC*/ DWORD DataUnk367;  
/*05C0*/ DWORD DataUnk368;  
/*05C4*/ DWORD DataUnk369;  
/*05C8*/ DWORD DataUnk370;  
/*05CC*/ DWORD DataUnk371;  
/*05D0*/ DWORD DataUnk372;  
/*05D4*/ DWORD DataUnk373;  
/*05D8*/ DWORD DataUnk374;  
/*05DC*/ DWORD DataUnk375;  
/*05E0*/ DWORD DataUnk376;  
/*05E4*/ DWORD DataUnk377;  
/*05E8*/ DWORD DataUnk378;  
/*05EC*/ DWORD DataUnk379;  
/*05F0*/ DWORD DataUnk380;  
/*05F4*/ DWORD DataUnk381;  
/*05F8*/ DWORD DataUnk382;  
/*05FC*/ DWORD DataUnk383;  
/*0600*/ DWORD DataUnk384;  
/*0604*/ DWORD DataUnk385;  
/*0608*/ DWORD DataUnk386;  
/*060C*/ DWORD DataUnk387;  
/*0610*/ DWORD DataUnk388;  
/*0614*/ DWORD DataUnk389;  
/*0618*/ DWORD DataUnk390;  
/*061C*/ DWORD DataUnk391;  
/*0620*/ DWORD DataUnk392;  
/*0624*/ DWORD DataUnk393;  
/*0628*/ DWORD DataUnk394;  
/*062C*/ DWORD DataUnk395;  
/*0630*/ DWORD DataUnk396;  
/*0634*/ DWORD DataUnk397;  
/*0638*/ DWORD DataUnk398;  
/*063C*/ DWORD DataUnk399;  
/*0640*/ DWORD DataUnk400;  
/*0644*/ DWORD DataUnk401;  
/*0648*/ DWORD DataUnk402;  
/*064C*/ DWORD DataUnk403;  
/*0650*/ DWORD DataUnk404;  
/*0654*/ DWORD DataUnk405;  
/*0658*/ DWORD DataUnk406;  
/*065C*/ DWORD DataUnk407;  
/*0660*/ DWORD DataUnk408;  
/*0664*/ DWORD DataUnk409;  
/*0668*/ DWORD DataUnk410;  
/*066C*/ DWORD DataUnk411;  
/*0670*/ DWORD DataUnk412;  
/*0674*/ DWORD DataUnk413;  
/*0678*/ DWORD DataUnk414;  
/*067C*/ DWORD DataUnk415;  
/*0680*/ DWORD DataUnk416;  
/*0684*/ DWORD DataUnk417;  
/*0688*/ DWORD DataUnk418;  
/*068C*/ DWORD DataUnk419;  
/*0690*/ DWORD DataUnk420; 

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjMACP(void){JUMPEXTMEMBER(EXTMODBASE, 0x00165B80 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjMACP(void){JUMPEXTMEMBER(EXTMODBASE, 0x001662F0 )}                // Destructor    
  __declspec(noinline) PCObjMACP MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x00165F50 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
  
  __declspec(noinline) void      MWTHISCALL _UpdateMovement(void){JUMPEXTMEMBER(EXTMODBASE, 0x00167CB0 )}               // VFT_INDEX=8
  __declspec(noinline) void      MWTHISCALL _GoToJailChecking(void){JUMPEXTMEMBER(EXTMODBASE, 0x0016A620 )}
  __declspec(noinline) void      MWTHISCALL _SetLookDisabled(void){JUMPEXTMEMBER(EXTMODBASE, 0x0010FA90 )}
  __declspec(noinline) void      MWTHISCALL _UpdadeSceneControls(void){JUMPEXTMEMBER(EXTMODBASE, 0x00168910 )}   // Update scene, based on the player position and look direction

            
  // Additional 
  bool      MWTHISCALL EnableLooking(bool Enable);
  bool      MWTHISCALL EnableMovement(bool Enable);
  bool      MWTHISCALL GetLooking(void){return !this->MouseDisabled;}
  bool      MWTHISCALL GetMovement(void){return !this->KeyboardDisabled;}
 
};
//------------------------------------------------------------------------------------
#pragma pack( pop )
//------------------------------------------------------------------------------------
class CObjREGN : public CBaseObject   // Size = 0x0078   // VFTableAddr = 0x007477BC, Functions = 3
{
public:
/*0010*/ BYTE  Name[32];  
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
/*005C*/ DWORD DataUnk23;  // LEVC
/*0060*/ DWORD DataUnk24;  
/*0064*/ DWORD DataUnk25;  
/*0068*/ DWORD DataUnk26;  
/*006C*/ DWORD DataUnk27;  
/*0070*/ DWORD DataUnk28;  
/*0074*/ DWORD DataUnk29;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjREGN(void){JUMPEXTMEMBER(EXTMODBASE, 0x00080C40 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjREGN(void){JUMPEXTMEMBER(EXTMODBASE, 0x00080D10 )}                // Destructor    
  __declspec(noinline) PCObjREGN MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x00080CF0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]

  // Additional 

};
//-----------------------------------------------------------------------------------
class CObjCELL : public CBaseObject   // Size = 0x0094   // VFTableAddr = 0x0074A0F8, Functions = 9
{
public:
/*0010*/ DWORD DataUnk4;  
/*0014*/ DWORD DataUnk5;  
/*0018*/ DWORD DataUnk6;  
/*001C*/ DWORD DataUnk7;  
/*0020*/ PCObjLAND Land;   
/*0024*/ DWORD DataUnk9;  
/*0028*/ DWORD DataUnk10;  
/*002C*/ DWORD DataUnk11;  
/*0030*/ DWORD DataUnk12;  
/*0034*/ PCObjREFR UnknRef00;   // REFR crea
/*0038*/ PCObjREFR UnknRef01;   // REFR crea 
/*003C*/ PCObjCELL UnkCell00;   // Self 
/*0040*/ DWORD DataUnk16;  
/*0044*/ PCObjREFR UnknRef02;   // REFR levc 
/*0048*/ PCObjREFR UnknRef03;   // REFR levc  
/*004C*/ PCObjCELL UnkCell01;   // Self 
/*0050*/ DWORD DataUnk20;  
/*0054*/ DWORD DataUnk21;  
/*0058*/ DWORD DataUnk22;  
/*005C*/ PCObjREFR UnknRef04;   // REFR cont
/*0060*/ PCObjREFR UnknRef05;   // REFR stat
/*0064*/ PCObjCELL UnkCell02;   // Self
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
/*0090*/ PCObjREGN Region;

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjCELL(void){JUMPEXTMEMBER(EXTMODBASE, 0x000DB500 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjCELL(void){JUMPEXTMEMBER(EXTMODBASE, 0x000DB660 )}                // Destructor    
  __declspec(noinline) PCObjCELL MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000DB640 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]

  // Additional 

};
//------------------------------------------------------------------------------------
class CObjLAND : public CBaseObject   // Size = 0x0240   // VFTableAddr = 0x007498D8, Functions = 9
{
public:
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
/*00DC*/ DWORD DataUnk55;  
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57;  
/*00E8*/ DWORD DataUnk58;  
/*00EC*/ DWORD DataUnk59;  
/*00F0*/ DWORD DataUnk60;  
/*00F4*/ DWORD DataUnk61;  
/*00F8*/ DWORD DataUnk62;  
/*00FC*/ DWORD DataUnk63;  
/*0100*/ DWORD DataUnk64;  
/*0104*/ DWORD DataUnk65;  
/*0108*/ DWORD DataUnk66;  
/*010C*/ DWORD DataUnk67;  
/*0110*/ DWORD DataUnk68;  
/*0114*/ DWORD DataUnk69;  
/*0118*/ DWORD DataUnk70;  
/*011C*/ DWORD DataUnk71;  
/*0120*/ DWORD DataUnk72;  
/*0124*/ DWORD DataUnk73;  
/*0128*/ DWORD DataUnk74;  
/*012C*/ DWORD DataUnk75;  
/*0130*/ DWORD DataUnk76;  
/*0134*/ DWORD DataUnk77;  
/*0138*/ DWORD DataUnk78;  
/*013C*/ DWORD DataUnk79;  
/*0140*/ DWORD DataUnk80;  
/*0144*/ DWORD DataUnk81;  
/*0148*/ DWORD DataUnk82;  
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
/*01D4*/ DWORD DataUnk117;  
/*01D8*/ DWORD DataUnk118;  
/*01DC*/ DWORD DataUnk119;  
/*01E0*/ DWORD DataUnk120;  
/*01E4*/ DWORD DataUnk121;  
/*01E8*/ DWORD DataUnk122;  
/*01EC*/ DWORD DataUnk123;  
/*01F0*/ DWORD DataUnk124;  
/*01F4*/ DWORD DataUnk125;  
/*01F8*/ DWORD DataUnk126;  
/*01FC*/ DWORD DataUnk127;  
/*0200*/ DWORD DataUnk128;  
/*0204*/ DWORD DataUnk129;  
/*0208*/ DWORD DataUnk130;  
/*020C*/ DWORD DataUnk131;  
/*0210*/ DWORD DataUnk132;  
/*0214*/ DWORD DataUnk133;  
/*0218*/ DWORD DataUnk134;  
/*021C*/ DWORD DataUnk135;  
/*0220*/ DWORD DataUnk136;  
/*0224*/ DWORD DataUnk137;  
/*0228*/ DWORD DataUnk138;  
/*022C*/ DWORD DataUnk139;  
/*0230*/ DWORD DataUnk140;  
/*0234*/ DWORD DataUnk141;  
/*0238*/ DWORD DataUnk142;  
/*023C*/ DWORD DataUnk143;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)           MWTHISCALL CObjLAND(void){JUMPEXTMEMBER(EXTMODBASE, 0x000C9FF0 )}                 // Constructor   
  __declspec(noinline)           MWTHISCALL ~CObjLAND(void){JUMPEXTMEMBER(EXTMODBASE, 0x000CA1A0 )}                // Destructor    
  __declspec(noinline) PCObjLAND MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x000CA180 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]

  // Additional 

};
//------------------------------------------------------------------------------------
}
//====================================================================================
#endif