#pragma once

#ifndef MWMiscH
#define MWMiscH

#include "MWDefinitions.h"
#include "MWMisc.h"    

      
//====================================================================================
//                                  Misc Classes
//------------------------------------------------------------------------------------
namespace MORROWIND 
{
//====================================================================================
struct CAListItem
{
/*0000*/ DWORD       Type;   // Default 0x0B  [0x00000008 = PCObjMACH] [0x00000001 = ???] [0x00000000 = PCAnimation]
/*0004*/ PCAListItem Next;
/*0008*/ PVOID       Data;   
};
//------------------------------------------------------------------------------------
struct ObjBSize        // Metrix relative to an object`s central vertical axis
{
/*0000*/ VECTOR3 Btm;  // x,y - negative (half size)
/*000C*/ VECTOR3 Top;  // x,y - positive (half size) [ multiply to 2 = real size (for x or y)] z = object`s height (without scaling and not fully correct)
};
//====================================================================================
struct CSoundRef     // Size = 0x0010
{
/*0000*/ PCObjREFR Reference;
/*0004*/ DWORD     pUnk01;
/*0008*/ PCSndInfo Sound;
/*000C*/ DWORD     pUnk03;   
};
//====================================================================================
struct CListItem     // Size = 0x000C
{
/*0000*/ PCListItem Prev;
/*0004*/ PCListItem Next;
/*0008*/ union {PVOID Data;PCSoundRef ObjSound;};
};
//------------------------------------------------------------------------------------
class CLinkedList    // Size = 0x0014
{
public:
/*0000*/ PVOID (*pVFTable)[1];     // VDestructor   VFT=0x007477AC
/*0004*/ DWORD      ItemCount;
/*0008*/ PCListItem FirstItem;
/*000C*/ PCListItem LastItem;   
/*0010*/ DWORD pUnk04;

  // Jumps to Morrowind`s functions
  __declspec(noinline)              MWTHISCALL CLinkedList(void){JUMPEXTMEMBER(EXTMODBASE, 0x0007E1E0 )}             // Constructor   (Check 'xrefs' to find a derived classes)
  __declspec(noinline)              MWTHISCALL ~CLinkedList(void){JUMPEXTMEMBER(EXTMODBASE, 0x0007E280 )}            // Destructor      // [virtual __thiscall std::locale::_Locimp::~_Locimp(void)]
  __declspec(noinline) PCLinkedList MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x0007E200 )}  // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]
 
  __declspec(noinline) void         MWTHISCALL UnInitialize(void){JUMPEXTMEMBER(EXTMODBASE, 0x0007E2F0 )}            // Inlined into destructors
  __declspec(noinline) PCListItem   MWTHISCALL AddItemToList(PVOID Data){JUMPEXTMEMBER(EXTMODBASE, 0x0007E360 )}     // Possible returns 'void' originally
  // Additional

};
//====================================================================================
class CTESFont            // Size = 0x0038 
{
public:
/*0000*/ DWORD pUnk00;
/*0004*/ LPSTR FontFile;
/*0008*/ DWORD pUnk02;
/*000C*/ DWORD pUnk03;
/*0010*/ DWORD pUnk04;
/*0014*/ DWORD pUnk05;
/*0018*/ DWORD pUnk06;
/*001C*/ DWORD pUnk07;
/*0020*/ DWORD pUnk08;
/*0024*/ DWORD pUnk09;
/*0028*/ DWORD pUnk10;
/*002C*/ DWORD pUnk11;
/*0030*/ DWORD pUnk12;
/*0034*/ DWORD pUnk13;

  // Jumps to Morrowind`s functions
        MWTHISCALL CTESFont(LPSTR FontFile, BOOL ToTexture){JUMPEXTMEMBER(EXTMODBASE, 0x0000AFC0 )}   // Constructor
  PVOID MWTHISCALL _PrepareText(PVOID Class_20,PCObjMACT Actor,DWORD Zero_00,DWORD Zero_01,DWORD Zero_02,float TxtWndWidth,LPSTR TopicText,DWORD Zero_03,PVOID Unknow00,PVOID Unknow01,DWORD UnkIndex){JUMPEXTMEMBER(EXTMODBASE, 0x0000BDC0 )}
  PVOID MWTHISCALL _PrepareTextFmt(PVOID Class_20,PCObjMACT Actor,DWORD Zero_00,DWORD Zero_01,DWORD Zero_02,LPSTR TopicText,DWORD Zero_03,PVOID Unknow00,PVOID Unknow01,DWORD UnkIndex){JUMPEXTMEMBER(EXTMODBASE, 0x0000B500 )}
  PVOID MWTHISCALL _TexDataOut(DWORD Zero_00,DWORD Zero_01,DWORD Zero_02, LPSTR TopicText,DWORD Zero_03,PVOID Unknow01,DWORD UnkIndex){JUMPEXTMEMBER(EXTMODBASE, 0x0000B290 )}   // All simple texts pass through this function

  // Additional
  
};
//------------------------------------------------------------------------------------
class CWindow       // Size = 0x0184  
{
public:
/*0000*/ DWORD DataUnk0;  
/*0004*/ DWORD DataUnk1;  
/*0008*/ DWORD DataUnk2;  
/*000C*/ DWORD DataUnk3;  
/*0010*/ DWORD DataUnk4;  
/*0014*/ short Identifier;  
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

  // Jumps to Morrowind`s functions
  __declspec(noinline)            MWTHISCALL CWindow(void){JUMPEXTMEMBER(EXTMODBASE, 0x00177DC0 )}   // Constructor
  __declspec(noinline)  PCWindow  MWTHISCALL _DrawTextLine(short TgtWndID, LPSTR TextLine, BOOL UnkFlag00, BOOL UnkFlag01){JUMPEXTMEMBER(EXTMODBASE, 0x00188BE0 )}      // Replacers (%NAME) in the text are resolved
  __declspec(noinline) void       MWTHISCALL _SetData(short MenuID, PVOID Data, long DataIndex){JUMPEXTMEMBER(EXTMODBASE, 0x00181F30 )}
  __declspec(noinline) PVOID*     MWTHISCALL _GetData(PVOID OutBuf, short MenuID, long DataIndex, DWORD Unk00, BYTE UnkFlag){JUMPEXTMEMBER(EXTMODBASE, 0x00181440 )} 

  // Additional

};
//------------------------------------------------------------------------------------
class CCursor   // Size = 0x0050   
{
public:
/*0000*/ PNiNode CurNormal;   
/*0004*/ PNiNode CurMove;   
/*0008*/ PNiNode CurUseItem;   
/*000C*/ PNiNode CurDrop;  
/*0010*/ PNiNode CurDropGround;   
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
/*003C*/ PNiNode pCursorRoot; 
/*0040*/ PNiNode pCursorChildRoot; 
/*0044*/ DWORD DataUnk17;  
/*0048*/ DWORD DataUnk18;  
/*004C*/ DWORD DataUnk19;  

  // Jumps to Morrowind`s functions
   __declspec(noinline)       MWTHISCALL CCursor(void){JUMPEXTMEMBER(EXTMODBASE, 0x000080E0 )}    // Constructor
   __declspec(noinline)       MWTHISCALL ~CCursor(void){JUMPEXTMEMBER(EXTMODBASE, 0x000082D0 )}   // Destructor
 
   __declspec(noinline) void  MWTHISCALL LoadCursorModelsByINI(void){JUMPEXTMEMBER(EXTMODBASE, 0x00008350 )}   
   __declspec(noinline) void  MWTHISCALL LoadCursorMode(DWORD Index, LPSTR FilePath){JUMPEXTMEMBER(EXTMODBASE, 0x000083D0 )}   

  // Additional

};
//------------------------------------------------------------------------------------
class CAnimation           // Size = 0x07E4
{
public:
/*0000*/ PNiNode CloneModel;  
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
/*0038*/ BYTE  LowerBodyAnim;
/*0039*/ BYTE  UpperBodyAnim;
/*003A*/ BYTE  LeftArmAnim;
/*003B*/ BYTE  UnkByte00;
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
/*0240*/ DWORD DataUnk144;  
/*0244*/ DWORD DataUnk145;  
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
/*02B4*/ DWORD DataUnk173;  
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
/*05B0*/ DWORD DataUnk364;  
/*05B4*/ DWORD DataUnk365;  
/*05B8*/ DWORD DataUnk366;  
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
/*0694*/ DWORD DataUnk421;  
/*0698*/ DWORD DataUnk422;  
/*069C*/ DWORD DataUnk423;  
/*06A0*/ DWORD DataUnk424;  
/*06A4*/ DWORD DataUnk425;  
/*06A8*/ DWORD DataUnk426;  
/*06AC*/ DWORD DataUnk427;  
/*06B0*/ DWORD DataUnk428;  
/*06B4*/ DWORD DataUnk429;  
/*06B8*/ DWORD DataUnk430;  
/*06BC*/ DWORD DataUnk431;  
/*06C0*/ DWORD DataUnk432;  
/*06C4*/ DWORD DataUnk433;  
/*06C8*/ DWORD DataUnk434;  
/*06CC*/ DWORD DataUnk435;  
/*06D0*/ DWORD DataUnk436;  
/*06D4*/ DWORD DataUnk437;  
/*06D8*/ DWORD DataUnk438;  
/*06DC*/ DWORD DataUnk439;  
/*06E0*/ DWORD DataUnk440;  
/*06E4*/ DWORD DataUnk441;  
/*06E8*/ DWORD DataUnk442;  
/*06EC*/ DWORD DataUnk443;  
/*06F0*/ DWORD DataUnk444;  
/*06F4*/ DWORD DataUnk445;  
/*06F8*/ DWORD DataUnk446;  
/*06FC*/ DWORD DataUnk447;  
/*0700*/ DWORD DataUnk448;  
/*0704*/ DWORD DataUnk449;  
/*0708*/ DWORD DataUnk450;  
/*070C*/ DWORD DataUnk451;  
/*0710*/ DWORD DataUnk452;  
/*0714*/ DWORD DataUnk453;  
/*0718*/ DWORD DataUnk454;  
/*071C*/ DWORD DataUnk455;  
/*0720*/ DWORD DataUnk456;  
/*0724*/ DWORD DataUnk457;  
/*0728*/ DWORD DataUnk458;  
/*072C*/ DWORD DataUnk459;  
/*0730*/ DWORD DataUnk460;  
/*0734*/ DWORD DataUnk461;  
/*0738*/ DWORD DataUnk462;  
/*073C*/ DWORD DataUnk463;  
/*0740*/ DWORD DataUnk464;  
/*0744*/ DWORD DataUnk465;  
/*0748*/ DWORD DataUnk466;  
/*074C*/ DWORD DataUnk467;  
/*0750*/ DWORD DataUnk468;  
/*0754*/ DWORD DataUnk469;  
/*0758*/ DWORD DataUnk470;  
/*075C*/ DWORD DataUnk471;  
/*0760*/ DWORD DataUnk472;  
/*0764*/ DWORD DataUnk473;  
/*0768*/ DWORD DataUnk474;  
/*076C*/ DWORD DataUnk475;  
/*0770*/ DWORD DataUnk476;  
/*0774*/ DWORD DataUnk477;  
/*0778*/ DWORD DataUnk478;  
/*077C*/ DWORD DataUnk479;  
/*0780*/ DWORD DataUnk480;  
/*0784*/ DWORD DataUnk481;  
/*0788*/ DWORD DataUnk482;  
/*078C*/ DWORD DataUnk483;  
/*0790*/ DWORD DataUnk484;  
/*0794*/ DWORD DataUnk485;  
/*0798*/ DWORD DataUnk486;  
/*079C*/ DWORD DataUnk487;  
/*07A0*/ DWORD DataUnk488;  
/*07A4*/ DWORD DataUnk489;  
/*07A8*/ DWORD DataUnk490;  
/*07AC*/ DWORD DataUnk491;  
/*07B0*/ DWORD DataUnk492;  
/*07B4*/ DWORD DataUnk493;  
/*07B8*/ DWORD DataUnk494;  
/*07BC*/ DWORD DataUnk495;  
/*07C0*/ DWORD DataUnk496;  
/*07C4*/ DWORD DataUnk497;  
/*07C8*/ DWORD DataUnk498;  
/*07CC*/ DWORD DataUnk499;  
/*07D0*/ DWORD DataUnk500;  
/*07D4*/ DWORD DataUnk501;  
/*07D8*/ DWORD DataUnk502;  
/*07DC*/ DWORD DataUnk503;  
/*07E0*/ DWORD DataUnk504;  

  // Jumps to Morrowind`s functions
  __declspec(noinline)      MWTHISCALL CAnimation(void){JUMPEXTMEMBER(EXTMODBASE, 0x0006B7A0 )}    // Constructor
  __declspec(noinline)      MWTHISCALL ~CAnimation(void){JUMPEXTMEMBER(EXTMODBASE, 0x0006B830 )}   // Destructor

  __declspec(noinline) void MWTHISCALL _HeadTrack(CObjREFR* Tracker, CObjREFR* Target){JUMPEXTMEMBER(EXTMODBASE, 0x0006F910 )}
  __declspec(noinline) void MWTHISCALL _ChangeAnimGroup(BYTE AnimGroup, long AnimType, long Unkn00, long Unkn01){JUMPEXTMEMBER(EXTMODBASE, 0x00070AE0 )}     

  // Additional

};

//------------------------------------------------------------------------------------
#pragma pack( push, 1 )
class CBaseCamera      // Size = 0x00D4      !!!UNSURE ABOUT THE NAME!!!, Virt func table 0x0074ADE8
{
public:
/*0000*/ PVOID (*pVFTable)[8];
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
/*0038*/ union {PCObjMACP Player;};  // Add other object types, if needed
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

  // Jumps to Morrowind`s functions
   __declspec(noinline)              MWTHISCALL CBaseCamera(void){JUMPEXTMEMBER(EXTMODBASE, 0x0053DA30 )}    // Constructor
   __declspec(noinline)              MWTHISCALL ~CBaseCamera(void){JUMPEXTMEMBER(EXTMODBASE, 0x0053DCB0 )}   // Destructor
   __declspec(noinline) PCBaseCamera MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x0053DC90 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]

   __declspec(noinline) void         MWTHISCALL _SwitchViewMode(void){JUMPEXTMEMBER(EXTMODBASE, 0x00142570 )}
 
  // Additional

};
//------------------------------------------------------------------------------------
class CCamera : public CBaseCamera     // Size = 0x0124     !!!UNSURE ABOUT THE NAME!!!, Virt func table 0x0074AE30
{
public:
/*00D4*/ DWORD DataUnk53;  
/*00D8*/ DWORD DataUnk54;  
/*00DC*/ DWORD DataUnk55;  
/*00E0*/ DWORD DataUnk56;  
/*00E4*/ DWORD DataUnk57;  
/*00E8*/ BYTE  ViewThirdPerson;   // Do not write the flag directly [0xE8](buggy) - use the TOGGLE flag instead [0xE9]
/*00E9*/ BYTE  SwitchViewMode;    // Set to 1 for mode switch
/*00EA*/ BYTE  UnkFlag00;         // This is exist ?
/*00EB*/ BYTE  UnkFlag01;         // This is exist ?  
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

  // Jumps to Morrowind`s functions
   __declspec(noinline)          MWTHISCALL CCamera(void){JUMPEXTMEMBER(EXTMODBASE, 0x00142290 )}    // Constructor
   __declspec(noinline)          MWTHISCALL ~CCamera(void){JUMPEXTMEMBER(EXTMODBASE, 0x001423F0 )}   // Destructor
   __declspec(noinline) PCCamera MWTHISCALL _VDestructor(DWORD RefCount){JUMPEXTMEMBER(EXTMODBASE, 0x001423D0 )}   // VFT_INDEX=0     // [virtual void * __thiscall std::locale::_Locimp::`scalar deleting destructor'(unsigned int)]

   __declspec(noinline) void     MWTHISCALL _SwitchViewMode(void){JUMPEXTMEMBER(EXTMODBASE, 0x00142570 )}
   __declspec(noinline) void     MWTHISCALL _UpdatePlayerPositions(DWORD Unkn){JUMPEXTMEMBER(EXTMODBASE, 0x001439C0 )}

  // Additional
   bool MWTHISCALL IsThirdPersonView(void){return this->ViewThirdPerson;}
   bool MWTHISCALL SetPersonViewMode(bool ThirdPerson);
 
};
//------------------------------------------------------------------------------------
class CPicker         // Size = 0x0038     !!!UNSURE ABOUT THE NAME!!!
{
public:
/*0000*/ DWORD DataUnk0;  
/*0004*/ DWORD DataUnk1;  
/*0008*/ DWORD DataUnk2;  
/*000C*/ DWORD DataUnk3;  
/*0010*/ BYTE bUnk00;;
/*0011*/ BYTE Enabled;        // 0 - No picking
/*0012*/ BYTE bUnk01; 
/*0013*/ BYTE bUnused00;
/*0014*/ PNiNode pNiNodeUnk123; 
/*0018*/ DWORD DataUnk6;  
/*001C*/ DWORD DataUnk7;  
/*0020*/ DWORD DataUnk8;  
/*0024*/ DWORD DataUnk9;     
/*0028*/ DWORD DataUnk10;  
/*002C*/ DWORD DataUnk11;  
/*0030*/ DWORD DataUnk12;  
/*0034*/ DWORD DataUnk13;  

  // Jumps to Morrowind`s functions
   __declspec(noinline)       MWTHISCALL CPicker(void){JUMPEXTMEMBER(EXTMODBASE, 0x002F2DF0 )}    // Constructor
     
   __declspec(noinline) bool  MWTHISCALL IsPickVectorIntersects(PVOID, PVOID, PVOID, PVOID){JUMPEXTMEMBER(EXTMODBASE, 0x0001D210 )}   
 
  // Additional

};
//------------------------------------------------------------------------------------
class CMenu : public CPicker     // Size = 0x00AC     !!!UNSURE ABOUT THE NAME!!!
{
public:
/*0038*/ DWORD DataUnk14;  
/*003C*/ DWORD DataUnk15;  
/*0040*/ DWORD DataUnk16;  
/*0044*/ DWORD DataUnk17;  
/*0048*/ DWORD DataUnk18;  
/*004C*/ DWORD DataUnk19;  
/*0050*/ DWORD DataUnk20;  
/*0054*/ DWORD DataUnk21;   
/*0058*/ BYTE  bUnkByte00; 
/*0059*/ BYTE  bUnkByte01; 
/*005A*/ BYTE  Activated;      // May be used as another MenuMode flag (Do not used by scripts)  // Used by MGE
/*005B*/ BYTE  bUnkByte02; 
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

  // Jumps to Morrowind`s functions
   __declspec(noinline)         MWTHISCALL CMenu(PCInterfaceManager Interf){JUMPEXTMEMBER(EXTMODBASE, 0x0018E040 )}    // Constructor

  // Additional

};
//------------------------------------------------------------------------------------
#pragma pack( pop )
//------------------------------------------------------------------------------------

}
//====================================================================================
#endif