#pragma once

#ifndef MWSoundH
#define MWSoundH

#include "MWDefinitions.h"      
//====================================================================================
//                                  Sound Work
//------------------------------------------------------------------------------------
namespace MORROWIND 
{
//====================================================================================
//                                  MP3 Work
//------------------------------------------------------------------------------------
struct MP3FileInfo    // Size = 0x00D0 (208) 
{
/*0000*/ DWORD Unk00;
/*0004*/ DWORD Unk01;      
/*0008*/ DWORD SamplPerSec;
/*000C*/ DWORD Unk03;
/*0010*/ DWORD Unk04;
/*0014*/ DWORD Unk05;
/*0018*/ DWORD Unk06;
/*001C*/ DWORD Unk07;
/*0020*/ DWORD Unk08;
/*0024*/ DWORD Unk09;
/*0028*/ DWORD Unk10;
/*002C*/ DWORD Unk11;      
/*0030*/ DWORD Unk12;
/*0034*/ DWORD Unk13;
/*0038*/ DWORD Unk14;
/*003C*/ DWORD Unk15;
/*0040*/ DWORD Unk16;
/*0044*/ DWORD Unk17;
/*0048*/ DWORD Unk18;
/*004C*/ DWORD Unk19;
/*0050*/ DWORD Unk20;
/*0054*/ DWORD Unk21;      
/*0058*/ DWORD Unk22;
/*005C*/ DWORD Unk23;
/*0060*/ DWORD Unk24;
/*0064*/ DWORD Unk25;
/*0068*/ DWORD Unk26;
/*006C*/ DWORD Unk27;
/*0070*/ DWORD Unk28;
/*0074*/ DWORD Unk29;
/*0078*/ DWORD Unk30;
/*007C*/ DWORD Unk31;      
/*0080*/ DWORD Unk32;
/*0084*/ DWORD Unk33;
/*0088*/ DWORD Unk34;
/*008C*/ DWORD Unk35;
/*0090*/ DWORD Unk36;
/*0094*/ DWORD Unk37;
/*0098*/ DWORD Unk38;
/*009C*/ DWORD Unk39;
/*00A0*/ DWORD Unk40;
/*00A4*/ DWORD Unk41;      
/*00A8*/ DWORD Unk42;
/*00AC*/ DWORD Unk43;
/*00B0*/ DWORD Unk44;
/*00B4*/ DWORD Unk45;
/*00B8*/ DWORD Unk46;
/*00BC*/ DWORD Unk47;
/*00C0*/ DWORD Unk48;
/*00C4*/ DWORD Unk49;
/*00C8*/ DWORD Unk50;
/*00CC*/ DWORD Unk51;      
};
//------------------------------------------------------------------------------------
class CMP3File              // Size = 0x001C (28)
{
public:
/*0000*/ DWORD        ChunksCount;
/*0004*/ PMP3FileInfo pMP3Info;        
/*0008*/ DWORD        Unk02;
/*000C*/ DWORD        Unk03;
/*0010*/ DWORD        Unk04;
/*0014*/ DWORD        Unk05;
/*0018*/ DWORD        Unk06;
  
  // Jumps to Morrowind`s functions
  __declspec(noinline)       MWTHISCALL CMP3File(LPSTR FilePath){JUMPEXTMEMBER(EXTMODBASE, 0x00027780 )}   // Constructor
  __declspec(noinline)       MWTHISCALL ~CMP3File(void){JUMPEXTMEMBER(EXTMODBASE, 0x000277D0 )}            // Destructor    
  __declspec(noinline) bool  MWTHISCALL _ReadMp3File(LPSTR FilePath){JUMPEXTMEMBER(EXTMODBASE, 0x00027800 )} 
  
  
  // Additional 
  
};
//------------------------------------------------------------------------------------
class CMP3Chunk          // Size = 0x413C (16700)
{
public:
/*0000*/ PCMP3File pMP3File;
/*0004*/ DWORD     Unk01;
/*0008*/ DWORD     Unk02;
/*000C*/ DWORD     Unk03;
/*0010*/ DWORD     Unk04;
/*0014*/ DWORD     Unk05;        // For decompression
/*0018*/ DWORD     Unk06;
/*001C*/ WORD      ChunkBuffer[8336];
  
  // Jumps to Morrowind`s functions
  __declspec(noinline)        MWTHISCALL CMP3Chunk(PCMP3File MP3File){JUMPEXTMEMBER(EXTMODBASE, 0x00025D10 )}   // Constructor
  __declspec(noinline)        MWTHISCALL ~CMP3Chunk(void){JUMPEXTMEMBER(EXTMODBASE, 0x00025E60 )}               // Destructor     
  __declspec(noinline) PWORD  MWTHISCALL _DecodeChunk(void){JUMPEXTMEMBER(EXTMODBASE, 0x00025EC0 )}  
 
  // Additional
  
};
//====================================================================================
//                                     OTHER
//------------------------------------------------------------------------------------
struct CSndInfo        // Size = 0x0058 (88) 
{
/*0000*/ LPDIRECTSOUNDBUFFER   pDSBuffer;   
/*0004*/ LPDIRECTSOUND3DBUFFER pDS3DBuffer; 
/*0008*/ WAVEFORMATEX          WaveFmtx;    
/*001C*/ DSBUFFERDESC          DsBufDesc;   
/*0040*/ BYTE                  Unknow00;    // 4 Align  ?
/*0044*/ PWORD                 pLipSBlock;   
/*0048*/ DWORD                 Unknow01;   
/*004C*/ DWORD                 Unused00;    
/*0050*/ DWORD                 Unused01;    
/*0054*/ DWORD                 Unused02;   
};
//------------------------------------------------------------------------------------
class CSoundBase        // Size = 0x02D8 (728)
{
public:
/*0000*/ DWORD DataUnk0;  
/*0004*/ BYTE  UnkByte00;   // Watch for alignment
/*0008*/ DWORD DataUnk2;  
/*000C*/ LPDIRECTSOUND8 pDSnd8;           
/*0010*/ DWORD DataUnk4;  
/*0014*/ DWORD DataUnk5;  
/*0018*/ BYTE  UnkByte01;   // Watch for alignment
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
/*02A8*/ BYTE  SoundDisabled;  
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

  // Jumps to Morrowind`s functions
  __declspec(noinline) MWTHISCALL CSoundBase(void){JUMPEXTMEMBER(EXTMODBASE, 0x00001B10 )}        // Constructor  
  __declspec(noinline) PCSndInfo MWTHISCALL _OpenVoiceFile(LPSTR VoiceFile, DWORD Flag){JUMPEXTMEMBER(EXTMODBASE, 0x00001DB0 )}
  __declspec(noinline) DWORD     MWTHISCALL _GetBaseVolume(BOOL UnkFlag){JUMPEXTMEMBER(EXTMODBASE, 0x0010E450 )}         // If UnkFlag = 0 RetVal <= 255
  __declspec(noinline) void      MWTHISCALL _SetDSBufFrequency(PCSndInfo Sound, float FreqMult){JUMPEXTMEMBER(EXTMODBASE, 0x00002A60 )}   // No freq change will be maked if value out of bounds [100 - 100000]
  
  // Additional

};
//------------------------------------------------------------------------------------

}
//====================================================================================
#endif