#pragma once

#ifndef MWDefinitionsH
#define MWDefinitionsH

#include <windows.h>
#include <dsound.h>
#include <d3dx9.h>
//===========================================================================     
//
//  All structures aligned to 4
//
//  Function   - Ordinary function
//  _Function  - Redirection to external function
//  __Function - Function is hook of external code 
//
//extern  HMODULE       ExeModuleBase;
//#define EXTMODBASE    ExeModuleBase
#define EXTMODBASE  0x00400000   // Any problem with this ?
#define MWTHISCALL               // 'this' pointer placed in 'ECX' register

#define ADDROFFSET(addr,offset) ((addr)+(offset))

#ifndef _REVERSE
#define _REVERSE(Value)          (((DWORD)(Value) << 24)|((DWORD)(Value) >> 24)|(((DWORD)(Value) << 8)&0x00FF0000)|(((DWORD)(Value) >> 8)&0x0000FF00))
#endif
//---------------------------------------------------------------------------
// Compiler always generates Stack Frame Pointers for member functions - used 'pop EBP' to restore it
// How to force the compiler do not make stack frame pointer ?
// How to force the compiler do not push ECX ?
// For __thiscall
#ifndef JUMPVFTMEMBER
#define JUMPVFTMEMBER(Index) __asm pop  ECX           \
                             __asm pop  EBP           \
                             __asm mov  EAX, [ECX]    \
                             __asm add  EAX, Index*4  \
                             __asm jmp  [EAX] 
//---------------------------------------------------------------------------
#endif
#ifndef JUMPEXTMEMBER
#define JUMPEXTMEMBER(Base,Offset) __asm pop  ECX                \
                                   __asm pop  EBP                \
                                   __asm mov  EAX, Base          \
                                   __asm lea  EAX, [EAX+Offset]  \
                                   __asm jmp  EAX 
//---------------------------------------------------------------------------
#endif
            
const int MP3WAVCHUNKSIZE   = 2304; 
const LPSTR CreatureTypes[] = {"Creature","Deadra","Undead","Humanoid"};
const LPSTR BaseRaces[]     = {"Dark Elf","High Elf","Wood Elf","Redguard","Imperial","Argonian","Khajiit","Breton","Nord","Orc"};
const LPSTR DlgReplacers[]  = {"Name","Race","Class","Faction","Rank","Cell","PCName","PCRace","PCClass","PCRank","NextPCRank","PCCrimeLevel","ActionForward","ActionBack","ActionSlideLeft","ActionSlideRight","ActionUse","ActionActivate","ActionReadyItem","ActionReadyMagic","ActionCrouch","ActionRun","ActionToggleRun","ActionAutoRun","ActionJump","ActionNextWeapon","ActionPrevWeapon","ActionNextSpell","ActionPrevSpell","ActionTogglePOV","ActionMenuMode","ActionJournal","ActionRestMenu"};

enum MWTopType  {tGreeting=0,tTopicName=1,tTopicText=5};
enum MWAnimType {atLowerBodyAnim,atUpperBodyAnim,atLeftArmAnim};
enum MWObjects  {otACTI=_REVERSE('ACTI'),otAPPA=_REVERSE('APPA'),otARMO=_REVERSE('ARMO'),otBODY=_REVERSE('BODY'),otBOOK=_REVERSE('BOOK'),otCLOT=_REVERSE('CLOT'),otCONT=_REVERSE('CONT'),otDOOR=_REVERSE('DOOR'),otINGR=_REVERSE('INGR'),otLIGH=_REVERSE('LIGH'),otLOCK=_REVERSE('LOCK'),otMISC=_REVERSE('MISC'),otPROB=_REVERSE('PROB'),otREPA=_REVERSE('REPA'),otSTAT=_REVERSE('STAT'),otWEAP=_REVERSE('WEAP'),otNPC_=_REVERSE('NPC_'),otCREA=_REVERSE('CREA'),otLEVC=_REVERSE('LEVC'),otSPEL=_REVERSE('SPEL'),otENCH=_REVERSE('ENCH'),otALCH=_REVERSE('ALCH'),otLEVI=_REVERSE('LEVI')};
enum MWRecords  {otMACR=_REVERSE('MACR'),otMACH=_REVERSE('MACH'),otMACP=_REVERSE('MACP'),otREFR=_REVERSE('REFR'),otRACE=_REVERSE('RACE'),otCLAS=_REVERSE('CLAS'),otFACT=_REVERSE('FACT')};
enum MWMenuIDs  {miTitle=0xF00D,miHotKeys=0xFD5D,miConsole=0xFF5F,miOptions=0xFDB2,miResting=0xFD31,miDialogue=0xFF1A,miInventory=0x80B4,miContainer=0xFF5A,miJournal=0xFE95,miBook=0xFF98,miScroll=0xFD1D,miMessageBox=0xFE02,miRestHours=0xFC45,miCGCharName=0xFDC2,miCGCharSel=0xFD4F,miCGClass=0xFF6B,miCGBirthSign=0xFFA8,miCGSummary=0xFC7C};
enum MWAnimGroups {Idle, anIdle2, anIdle3, anIdle4, anIdle5, anIdle6, anIdle7, anIdle8, anIdle9, anIdlehh, anIdle1h, anIdle2c, anIdle2w, anIdleSwim, anIdleSpell, anIdleCrossbow, anIdleSneak, anIdleStorm, anTorch, anHit1, anHit2, anHit3, anHit4, anHit5, anSwimHit1, anSwimHit2, anSwimHit3, anDeath1, anDeath2, anDeath3, anDeath4, anDeath5, anDeathKnockDown, anDeathKnockOut, anKnockDown, anKnockOut, anSwimDeath, anSwimDeath2, anSwimDeath3, anSwimDeathKnockDown, anSwimDeathKnockOut, anSwimKnockOut, anSwimKnockDown, anSwimWalkForward, anSwimWalkBack, anSwimWalkLeft, anSwimWalkRight, anSwimRunForward, anSwimRunBack, anSwimRunLeft, anSwimRunRight, anSwimTurnLeft, anSwimTurnRight, anWalkForward, anWalkBack, anWalkLeft, anWalkRight, anTurnLeft, anTurnRight, anRunForward, anRunBack, anRunLeft, anRunRight, anSneakForward, anSneakBack, anSneakLeft, anSneakRight, anJump, anWalkForwardhh, anWalkBackhh, anWalkLefthh, anWalkRighthh, anTurnLefthh, anTurnRighthh, anRunForwardhh, anRunBackhh, anRunLefthh, anRunRighthh, anSneakForwardhh, anSneakBackhh, anSneakLefthh, anSneakRighthh, anJumphh, anWalkForward1h, anWalkBack1h, anWalkLeft1h, anWalkRight1h, anTurnLeft1h, anTurnRight1h, anRunForward1h, anRunBack1h, anRunLeft1h, anRunRight1h, anSneakForward1h, anSneakBack1h, anSneakLeft1h, anSneakRight1h, anJump1h, anWalkForward2c, anWalkBack2c, anWalkLeft2c, anWalkRight2c, anTurnLeft2c, anTurnRight2c, anRunForward2c, anRunBack2c, anRunLeft2c, anRunRight2c, anSneakForward2c, anSneakBack2c, anSneakLeft2c, anSneakRight2c, anJump2c, anWalkForward2w, anWalkBack2w, anWalkLeft2w, anWalkRight2w, anTurnLeft2w, anTurnRight2w, anRunForward2w, anRunBack2w, anRunLeft2w, anRunRight2w, anSneakForward2w, anSneakBack2w, anSneakLeft2w, anSneakRight2w, anJump2w, anSpellCast, anSpellTurnLeft, anSpellTurnRight, anAttack1, anAttack2, anAttack3, anSwimAttack1, anSwimAttack2, anSwimAttack3, anHandToHand, anCrossbow, anBowAndArrow, anThrowWeapon, anWeaponOneHand, anWeaponTwoHand, anWeaponTwoWide, anShield, anPickProbe, anInventoryHandToHand, anInventoryWeaponOneHand, anInventoryWeaponTwoHand, anInventoryWeaponTwoWide};



template<class T> void __fastcall SetAddress(T &Dst,int Addr){Dst = reinterpret_cast<T>(Addr);}

struct MATRIX3x3
{
 union
  {
   float M[3*3];
   float m[3][3];
  };
};
struct MATRIX4x4
{
 union
  {
   float M[4*4];
   float m[4][4];
  };
};
struct VECTOR3
{
 float x;
 float y;
 float z; 
};
struct VECTOR4
{
 float x;
 float y;
 float z; 
 float w;  
};
struct QUATERNION
{
 float x;
 float y;
 float z; 
 float w; 
};
//====================================================================================
namespace MORROWIND 
{
// --------- Morrowind.h ------------------
class   CTESGame;
class   CRenderer;
class   CGameScene;     
class   CMapManager;
class   CMobManager; 
class   CProcessManager;
class   CInterfaceManager;
class   CConsoleInput;
       
typedef CTESGame*          PCTESGame; 
typedef CRenderer*         PCRenderer;                                                                                      
typedef CGameScene*        PCGameScene;                                                                                       
typedef CMapManager*       PCMapManager; 
typedef CMobManager*       PCMobManager; 
typedef CProcessManager*   PCProcessManager;                                                                                       
typedef CInterfaceManager* PCInterfaceManager;                                                                                      
typedef CConsoleInput*     PCConsoleInput; 

// --------- MWObjects.h ------------------
class   CObjREFR;
class   CObjREGN;
class   CObjCELL;
class   CObjLAND;
class   CObjSPEL;
class   CObjENCH;
class   CObjNPC_;
class   CObjCREA;
class   CObjCONT;
class   CObjSOUN;
class   CObjINFO; 
class   CObjDIAL;
class   CObjCLAS; 
class   CObjRACE;
class   CObjSCPT;
class   CObjBODY;
class   CObjMMBL;
class   CObjMPSP;
class   CObjMPRJ;
class   CObjMACT;         
class   CObjMACR;
class   CObjMACH;
class   CObjMACP;
class   CObjBaseNPC_;
class   CObjBaseCREA;
class   CObjBaseCONT;
class   CObjPlayerNPC_;

class   CVisualObject;
class   CBaseObject;
class   CActiveObject;
class   CLinkedObject;
class   CContainerObject;

typedef CObjREFR*       PCObjREFR; 
typedef CObjREGN*       PCObjREGN; 
typedef CObjCELL*       PCObjCELL; 
typedef CObjLAND*       PCObjLAND; 
typedef CObjSPEL*       PCObjSPEL;                                                                                       
typedef CObjENCH*       PCObjENCH;                                                                                                                                                                             
typedef CObjBODY*       PCObjBODY;
typedef CObjSCPT*       PCObjSCPT;
typedef CObjNPC_*       PCObjNPC_;
typedef CObjCREA*       PCObjCREA;
typedef CObjCONT*       PCObjCONT;
typedef CObjSOUN*       PCObjSOUN;
typedef CObjINFO*       PCObjINFO;
typedef CObjDIAL*       PCObjDIAL;
typedef CObjCLAS*       PCObjCLAS;
typedef CObjRACE*       PCObjRACE;
typedef CObjMMBL*       PCObjMMBL;
typedef CObjMPSP*       PCObjMPSP;
typedef CObjMPRJ*       PCObjMPRJ;
typedef CObjMACT*       PCObjMACT;
typedef CObjMACR*       PCObjMACR;
typedef CObjMACH*       PCObjMACH;
typedef CObjMACP*       PCObjMACP;
typedef CObjBaseNPC_*   PCObjBaseNPC_;
typedef CObjBaseCREA*   PCObjBaseCREA;
typedef CObjBaseCONT*   PCObjBaseCONT;
typedef CObjPlayerNPC_* PCObjPlayerNPC_;

typedef CVisualObject*      PCVisualObject;
typedef CBaseObject*        PCBaseObject;
typedef CActiveObject*      PCActiveObject;
typedef CLinkedObject*      PCLinkedObject;                                                                                       
typedef CContainerObject*   PCContainerObject;                                                                                       

// --------- MWNifModel.h ------------------
class  NiBBox;
class  NiBoxBBox;
class  NiUnionBBox;
class  NiNode;
class  NiObject;
class  NiCamera;
class  NiExtraData; 
class  NiDirectionalLight;
class  NiFogProperty; 
class  CCameraView;
class  CSceneGraph;
struct NiVelocity;

typedef NiBBox*             PNiBBox;
typedef NiBoxBBox*          PNiBoxBBox;
typedef NiUnionBBox*        PNiUnionBBox;
typedef NiObject*           PNiObject;
typedef NiNode*             PNiNode;
typedef NiCamera*           PNiCamera;
typedef NiExtraData*        PNiExtraData;
typedef NiFogProperty*      PNiFogProperty;                                                                                       
typedef NiDirectionalLight* PNiDirectionalLight; 
typedef CCameraView*        PCCameraView;
typedef CSceneGraph*        PCSceneGraph;
typedef NiVelocity*         PNiVelocity;

// --------- MWSound.h ------------------
class   CMP3File;
class   CMP3Chunk;
struct  MP3FileInfo;
struct  CSndInfo;
class   CSoundBase; 

typedef CMP3File*    PCMP3File;
typedef CMP3Chunk*   PCMP3Chunk;
typedef MP3FileInfo* PMP3FileInfo;
typedef CSndInfo*    PCSndInfo;
typedef CSoundBase*  PCSoundBase;

// --------- MWMisc.h ------------------
class   CMenu;
class   CPicker;
class   CCursor;
class   CTESFont; 
class   CWindow; 
class   CCamera;
class   CBaseCamera;
class   CLinkedList;
class   CAnimation; 
struct  CAListItem;
struct  CListItem;
struct  CSoundRef;
struct  ObjBSize;

typedef CMenu*             PCMenu;
typedef CPicker*           PCPicker;
typedef CCursor*           PCCursor;
typedef CTESFont*          PCTESFont;
typedef CWindow*          PCWindow;
typedef CCamera*           PCCamera; 
typedef CBaseCamera*       PCBaseCamera; 
typedef CLinkedList*       PCLinkedList; 
typedef CAnimation*        PCAnimation; 
typedef CAListItem*        PCAListItem;                                                                                       
typedef ObjBSize*          PObjBSize;
typedef CListItem*         PCListItem;
typedef CSoundRef*         PCSoundRef;
                                                                                      
//====================================================================================
struct  GLOBALS;
typedef GLOBALS* PGLOBALS;

struct GLOBALS
  {
   bool        *MouseInvAxisY;
   LPSTR       *PreparedText;
   PCWindow    *ppWindowsList;
   PCTESGame   *ppMWGame;   
   PCRenderer  *ppMWRenderer;   
   PCGameScene *ppGameScene;  
  
   PVOID (__cdecl *MWGetMem)(DWORD Amount);       // Operator 'new' of Morrowind.exe
   void  (__cdecl *MWFreeMem)(PVOID MemPtr);      // Operator 'delete' of Morrowind.exe
   void  (__cdecl *OpenBook)(LPSTR BookText);     // Can Return Book open sound (SOUN)   
   void  (__cdecl *OpenDocument)(PCWindow TxtClass, LPSTR DocText);         
   void  (__cdecl *SetConsoleObjectRef)(PCObjREFR Reference);    
   void  (__cdecl *ShowPopUpMessage)(LPSTR Msg, DWORD Unkn00, DWORD Unkn01);   
   void  (__cdecl *OutConsoleMessage)(PCGameScene gs, LPSTR MsgFmt, ...);
   void  (__cdecl *StartGreeting)(LPSTR Greeting, PCObjDIAL Dialogue, DWORD Unkn00, DWORD Unkn01);
   void  (__cdecl *ChangeDlgTopic)(PVOID Unkn00, LPSTR Text, DWORD Type, DWORD Unkn01);      //Type: 0-Greeting, 1-Topic Name, 5-Topic Text, 2-??? 
   BOOL  (__cdecl *CloseAllMenus)(DWORD UnknONE);  // Always set this parameter to 1    
   BOOL  (__cdecl *SetMenuMode)(WORD MenuMode);      
   BOOL  (__cdecl *MakeJournalPage)(PCWindow TxtClass00,short TgtWndID00,DWORD UnkRndTime,short TgtWndID01,PCWindow TxtClass01);           
   BOOL  (__cdecl *BookPageForward)(PCWindow TxtClass00,short TgtWndID00,DWORD UnkRndTime,short TgtWndID01,PCWindow TxtClass01);  
   BOOL  (__cdecl *BookPageBackward)(PCWindow TxtClass00,short TgtWndID00,DWORD UnkRndTime,short TgtWndID01,PCWindow TxtClass01);   
   PCWindow  (__cdecl *GetWindowByID)(short WindowID);   // Both functions returns different instances of same window !!! 
   PCWindow  (__cdecl *FindWindowByID)(short WindowID);  
       
        
        
   PGLOBALS __fastcall Initialize(void)    
    {
     // No signature checking - the functions may be hooked
     SetAddress(this->MWGetMem,ADDROFFSET(EXTMODBASE, 0x00327692 ));
     SetAddress(this->MWFreeMem,ADDROFFSET(EXTMODBASE, 0x00327732 ));
     SetAddress(this->SetMenuMode,ADDROFFSET(EXTMODBASE, 0x00195230 ));  
     SetAddress(this->CloseAllMenus,ADDROFFSET(EXTMODBASE, 0x00194C10 ));  
     SetAddress(this->GetWindowByID,ADDROFFSET(EXTMODBASE, 0x00195400 ));  
     SetAddress(this->FindWindowByID,ADDROFFSET(EXTMODBASE, 0x00195370 ));           
     SetAddress(this->OpenBook,ADDROFFSET(EXTMODBASE, 0x001AC2A0 ));
     SetAddress(this->OpenDocument,ADDROFFSET(EXTMODBASE, 0x00213060 ));
     SetAddress(this->MakeJournalPage,ADDROFFSET(EXTMODBASE, 0x001D7260 ));
     SetAddress(this->BookPageForward,ADDROFFSET(EXTMODBASE, 0x001AC970 ));
     SetAddress(this->BookPageBackward,ADDROFFSET(EXTMODBASE, 0x001AC8C0 ));  
     SetAddress(this->SetConsoleObjectRef,ADDROFFSET(EXTMODBASE, 0x001B2F00 ));  
     SetAddress(this->StartGreeting,ADDROFFSET(EXTMODBASE, 0x001C0050 ));
     SetAddress(this->ChangeDlgTopic,ADDROFFSET(EXTMODBASE, 0x001C00D0 ));   
     SetAddress(this->ShowPopUpMessage,ADDROFFSET(EXTMODBASE, 0x001F90C0 ));  
     SetAddress(this->OutConsoleMessage,ADDROFFSET(EXTMODBASE, 0x0000f970 )); 

     // Global variables
     SetAddress(this->MouseInvAxisY,ADDROFFSET(EXTMODBASE, 0x003D16DD ));
     SetAddress(this->ppWindowsList,ADDROFFSET(EXTMODBASE, 0x003D1C74 ));
     SetAddress(this->PreparedText,ADDROFFSET(EXTMODBASE, 0x003C6568 ));  
     SetAddress(this->ppMWRenderer,ADDROFFSET(EXTMODBASE, 0x003C6CDC ));
     SetAddress(this->ppGameScene,ADDROFFSET(EXTMODBASE, 0x003C67DC ));
     SetAddress(this->ppMWGame,ADDROFFSET(EXTMODBASE, 0x003C67E0 ));
     return this;                                          
    }           
  };

}
//====================================================================================




//====================================================================================
#endif








