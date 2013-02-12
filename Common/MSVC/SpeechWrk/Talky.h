#pragma once

#ifndef TalkyH
#define TalkyH

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		 // Exclude rarely-used stuff from Windows headers
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT  0x0500     // For CoInitializeEx support
#endif
//---------------------------------------------------------------------------
#include <Windows.h>
#include <objbase.h>
#include "sapi.h"
#include "sapidef.h"

//---------------------------------------------------------------------------
#ifndef DEFCALL
#define DEFCALL __stdcall
#endif
//---------------------------------------------------------------------------
#define SPKTOSNDC 0
#define SPKTOMEMB 1
#define SPKTOWAVF 2
//---------------------------------------------------------------------------
#define  SOAF8kHz8BitM     4
#define  SOAF8kHz8BitS     5
#define  SOAF8kHz16BitM    6
#define  SOAF8kHz16BitS    7
#define  SOAF11kHz8BitM    8
#define  SOAF11kHz8BitS    9
#define  SOAF11kHz16BitM  10
#define  SOAF11kHz16BitS  11
#define  SOAF12kHz8BitM   12
#define  SOAF12kHz8BitS   13
#define  SOAF12kHz16BitM  14
#define  SOAF12kHz16BitS  15
#define  SOAF16kHz8BitM   16
#define  SOAF16kHz8BitS   17
#define  SOAF16kHz16BitM  18
#define  SOAF16kHz16BitS  19
#define  SOAF22kHz8BitM   20
#define  SOAF22kHz8BitS   21
#define  SOAF22kHz16BitM  22
#define  SOAF22kHz16BitS  23
#define  SOAF24kHz8BitM   24
#define  SOAF24kHz8BitS   25
#define  SOAF24kHz16BitM  26
#define  SOAF24kHz16BitS  27
#define  SOAF32kHz8BitM   28
#define  SOAF32kHz8BitS   29
#define  SOAF32kHz16BitM  30
#define  SOAF32kHz16BitS  31
#define  SOAF44kHz8BitM   32
#define  SOAF44kHz8BitS   33
#define  SOAF44kHz16BitM  34
#define  SOAF44kHz16BitS  35
#define  SOAF48kHz8BitM   36
#define  SOAF48kHz8BitS   37
#define  SOAF48kHz16BitM  38
#define  SOAF48kHz16BitS  39

//---------------------------------------------------------------------------
// Used 'Automation' TTS SAPI interface because CPP 'SpVoice' interfece
//   do not have 'GetVoices' method, and it is a bit simplier.
//
class CTalky
{
 private:
   UINT    VConfigSize;
 //  PVOID   PrevVoiceBuf;
   HANDLE  DefHeapHandle;
   VARIANT CurSpeechBuf;
   WCHAR   VoiceConfig[256];
   WCHAR   WavFilePath[256];
	 ISpeechVoice        *Voice;       // Used only in 'SetVoice' and 'SpeakText'
     ISpeechFileStream   *FileStream;
	 ISpeechMemoryStream *MemoryStream;
	 ISpeechObjectTokens *VoicesList;
     ISpeechAudioFormat  *AudioFormat;

 public:
   BOOL  Asynchr;
   UINT  SpeakTo;
   PWSTR SVoicesList;       // Current list of Voices

   UINT DEFCALL SpeakText(PWSTR Text);
   UINT DEFCALL SpeakText(LPSTR Text);
   UINT DEFCALL SpeakTextRaw(PWSTR Text);  // Speaks text as is (Without appending configuration)

   UINT DEFCALL SetVoice(UINT VoiceIndex);
   UINT DEFCALL SetVoice(PWSTR VoiceName);
   UINT DEFCALL SetVoice(LPSTR VoiceName);
   UINT DEFCALL SetVoiceConfig(LPSTR Config);     // No UNICODE for this?
   UINT DEFCALL SetWavFileName(PWSTR Filename);
   UINT DEFCALL SetWavFileName(LPSTR Filename);
   UINT DEFCALL SetOutputFormat(UINT OutputFmt);
   UINT DEFCALL GetSpeechBuffer(PVOID *Buffer, PUINT Size);
   UINT DEFCALL GetVoicesList(PWSTR DestBuffer, UINT BufferSize, PUINT VCount);
   UINT DEFCALL GetVoicesList(LPSTR DestBuffer, UINT BufferSize, PUINT VCount);
   UINT DEFCALL GetOutputFormat(PUINT SamplePerSec, PUINT BitsPerSample, PUINT Channels);
   UINT DEFCALL Initialize(UINT OutTgt=SPKTOSNDC, BOOL Async=FALSE);
   void DEFCALL UnInitialize(void);
   void DEFCALL SetDefaults(void);

   DEFCALL CTalky(BOOL Init=FALSE);
   DEFCALL ~CTalky();
//-----------------------------------
inline VARIANT* ResetVariant(VARIANT *pv,  VARTYPE type)
{
 ZeroObject(pv);
 VariantInit(pv);
 pv->vt = type;
}
//-----------------------------------
void ReleaseVariant(VARIANT *pv)
{
 UINT Type =  pv->vt;
 VariantClear(pv);
 ResetVariant(pv,Type);
}
//-----------------------------------
void DuplicateVariant(VARIANT *pvdst, VARIANT *pvsrc)
{
 ReleaseVariant(pvdst);
 CopyObject(pvdst, pvsrc);
}
//-----------------------------------
template<class T> void ZeroObject(T *Obj)
{
 for(UINT ctr=0;ctr<(sizeof(T)/sizeof(DWORD));ctr++)reinterpret_cast<PDWORD>(Obj)[ctr] = 0;
 for(UINT ctr=0;ctr<(sizeof(T)%sizeof(DWORD));ctr++)reinterpret_cast<PBYTE>(Obj)[ctr]  = 0;
}
//-----------------------------------
template<class T> void CopyObject(T *ObjDst, T *ObjSrc)
{
 for(UINT ctr=0;ctr<(sizeof(T)/sizeof(DWORD));ctr++)reinterpret_cast<PDWORD>(ObjDst)[ctr] = reinterpret_cast<PDWORD>(ObjSrc)[ctr];
 for(UINT ctr=0;ctr<(sizeof(T)%sizeof(DWORD));ctr++)reinterpret_cast<PBYTE>(ObjDst)[ctr]  = reinterpret_cast<PBYTE>(ObjSrc)[ctr];
}
//-----------------------------------
};
typedef CTalky* PCTalky;
//---------------------------------------------------------------------------
#endif
