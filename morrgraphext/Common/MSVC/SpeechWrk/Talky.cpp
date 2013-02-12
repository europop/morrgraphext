
#include "Talky.h"
//---------------------------------------------------------------------------
DEFCALL CTalky::CTalky(BOOL Init)
{
 if(Init)this->Initialize();
   else this->SetDefaults();
}
//---------------------------------------------------------------------------
DEFCALL CTalky::~CTalky()
{						
 this->UnInitialize();
}
//---------------------------------------------------------------------------
void DEFCALL CTalky::SetDefaults(void)
{
 ZeroObject(this);
 ResetVariant(&this->CurSpeechBuf, VT_ARRAY);
 this->SpeakTo = SPKTOSNDC;
 this->Asynchr = false;
}
//---------------------------------------------------------------------------
void DEFCALL CTalky::UnInitialize(void)
{
 // Use 'IsBadReadPtr' for checks (for some reason the Pointers may become invalid!)
 if(!IsBadReadPtr(this->Voice,4))this->Voice->Release();
 if(!IsBadReadPtr(this->VoicesList,4))this->VoicesList->Release();
 if(!IsBadReadPtr(this->FileStream,4))this->FileStream->Release();
 if(!IsBadReadPtr(this->MemoryStream,4))this->MemoryStream->Release();
 if(!IsBadReadPtr(this->AudioFormat,4))this->AudioFormat->Release();
 if(!IsBadReadPtr(this->SVoicesList,4))HeapFree(this->DefHeapHandle,0,this->SVoicesList);
 ReleaseVariant(&this->CurSpeechBuf);
 this->SetDefaults();
 //CoUninitialize();          // Don`t do this !!!
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::Initialize(UINT OutTgt, BOOL Async)
{
 UINT Result = 0;

 this->UnInitialize();
 if((OutTgt!=SPKTOSNDC)&&(OutTgt!=SPKTOMEMB)&&(OutTgt!=SPKTOWAVF))return -1;
 this->Asynchr       = Async;
 this->SpeakTo       = OutTgt;
 this->DefHeapHandle = GetProcessHeap();
 this->SVoicesList   = (PWSTR)HeapAlloc(this->DefHeapHandle,HEAP_ZERO_MEMORY,16);
 Result += CoInitializeEx(NULL,COINIT_MULTITHREADED|COINIT_SPEED_OVER_MEMORY);
 Result += CoCreateInstance(CLSID_SpAudioFormat, NULL, CLSCTX_ALL, __uuidof(ISpeechAudioFormat), (void **)&this->AudioFormat);
 Result += CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpeechVoice, (void **)&this->Voice);
 this->GetVoicesList((PWSTR)NULL,0,NULL);
 switch(this->SpeakTo)
  {
   case SPKTOSNDC:
     break;
   case SPKTOMEMB:
     Result += CoCreateInstance(CLSID_SpMemoryStream, NULL, CLSCTX_ALL, __uuidof(ISpeechMemoryStream), (void **)&this->MemoryStream);
     break;
   case SPKTOWAVF:
     Result += CoCreateInstance(CLSID_SpFileStream, NULL, CLSCTX_ALL, __uuidof(ISpeechFileStream), (void **)&this->FileStream);
     break;
   default: return -1;
  }
 return Result;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SpeakText(PWSTR Text)
{
 UINT    Res;
 PWSTR   WText      = NULL;
 DWORD   WBufferLen = NULL;

 if(!this->DefHeapHandle)return 9;
 WBufferLen  = (lstrlenW(Text)*2)+(this->VConfigSize*2)+16;
 WText       = (PWSTR)HeapAlloc(this->DefHeapHandle, 0, WBufferLen);
 lstrcpyW(WText, (PWSTR)&this->VoiceConfig);
 lstrcatW(WText, Text);
 Res = this->SpeakTextRaw(WText);
 if(WText)HeapFree(this->DefHeapHandle, 0, WText);
 return Res;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SpeakText(LPSTR Text)
{
 UINT    Res;
 PWSTR   WText       = NULL;
 DWORD   WBufferLen  = NULL;

 if(!this->DefHeapHandle)return 9;
 WBufferLen  = (lstrlen(Text)*2)+(this->VConfigSize*2)+16;
 WText       = (PWSTR)HeapAlloc(this->DefHeapHandle, 0, WBufferLen);
 lstrcpyW(WText, (PWSTR)&this->VoiceConfig);
 MultiByteToWideChar(CP_ACP, 0, Text, -1, (PWSTR)&WText[this->VConfigSize], WBufferLen);
 Res = this->SpeakTextRaw(WText);
 if(WText)HeapFree(this->DefHeapHandle, 0, WText);
 return Res;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SpeakTextRaw(PWSTR Text)
{
 DWORD   SpeechFlags = SPF_PURGEBEFORESPEAK;
 VARIANT VValue;

 if(!this->Voice)return 1;
 ResetVariant(&VValue, VT_I4);
 if(this->Asynchr)SpeechFlags = (SpeechFlags | SPF_ASYNC);
 switch(this->SpeakTo)
  {
   case SPKTOSNDC:
     this->Voice->put_AllowAudioOutputFormatChangesOnNextSet(TRUE);
     this->Voice->putref_AudioOutput(NULL);
     this->Voice->Speak(Text, (SpeechVoiceSpeakFlags)SpeechFlags, NULL);
     break;
   case SPKTOMEMB:
     if(!this->MemoryStream)break;
     this->MemoryStream->Seek(VValue,SSSPTRelativeToStart,&VValue);
     this->Voice->put_AllowAudioOutputFormatChangesOnNextSet(TRUE);
     this->Voice->putref_AudioOutputStream(this->MemoryStream);
     this->Voice->Speak(Text, (SpeechVoiceSpeakFlags)SpeechFlags, NULL);
     break;
   case SPKTOWAVF:
     if(!this->FileStream)break;
     DeleteFileW((PWSTR)&this->WavFilePath);
     this->FileStream->Open((PWSTR)&this->WavFilePath, SSFMCreateForWrite, FALSE);  // Default: 22KHz, 16Bit, Mono
     this->Voice->put_AllowAudioOutputFormatChangesOnNextSet(TRUE);
     this->Voice->putref_AudioOutputStream(this->FileStream);
     this->Voice->Speak(Text, (SpeechVoiceSpeakFlags)SpeechFlags, NULL);
     this->FileStream->Close();
     break;
  }
 return 0;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SetVoice(LPSTR VoiceName)
{
 WCHAR VoiceString[256];
 MultiByteToWideChar(CP_ACP, 0, VoiceName, -1, (PWSTR)&VoiceString, sizeof(VoiceString));
 return this->SetVoice((PWSTR)&VoiceString);
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SetVoice(PWSTR VoiceName)
{
 long               Count  = NULL;
 PWSTR              Descr  = NULL;
 UINT               Result = 3;
 ISpeechObjectToken *Token = NULL;

 if(!this->Voice || !this->VoicesList)return 5;
 this->VoicesList->get_Count(&Count);
 while(Count)
  {
   Count--;
   if(this->VoicesList->Item(Count, &Token))continue;
   Token->GetDescription(0, &Descr);  // Don`t need to release this Token ?
   if(!lstrcmpiW(Descr,VoiceName)){this->Voice->putref_Voice(Token);Result = 0;Count = 0;}
   Token->Release();      // !!!!!!!!!!!!!!!!
  }
 return Result;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SetVoice(UINT VoiceIndex)
{
 long               Count  = NULL;
 ISpeechObjectToken *Token = NULL;

 if(!this->Voice || !this->VoicesList)return 5;
 this->VoicesList->get_Count(&Count);
 if(VoiceIndex >= Count)return 4;
 if(this->VoicesList->Item(VoiceIndex, &Token))return 3;
 this->Voice->putref_Voice(Token);
 Token->Release();    // !!!!!!!!!!!!!!!!
 return 0;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SetVoiceConfig(LPSTR Config)
{
 this->VConfigSize = lstrlen(Config);   // Size in SYMBOLS!
 return (BOOL)(!MultiByteToWideChar(CP_ACP, 0, Config, -1, (PWSTR)&this->VoiceConfig, sizeof(this->VoiceConfig)));
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SetWavFileName(PWSTR Filename)
{
 lstrcpyW((PWSTR)&this->WavFilePath,Filename); // UNSAFE!
 return 0;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SetWavFileName(LPSTR Filename)
{
 return (BOOL)(!MultiByteToWideChar(CP_ACP, 0, Filename, -1, (PWSTR)&this->WavFilePath, sizeof(this->WavFilePath)));
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::SetOutputFormat(UINT OutputFmt)
{
 if(this->AudioFormat)AudioFormat->put_Type((SpeechAudioFormatType)OutputFmt);
   else return 8;
 if(this->FileStream)FileStream->putref_Format(this->AudioFormat);
 if(this->MemoryStream)MemoryStream->putref_Format(this->AudioFormat);
 return 0;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::GetOutputFormat(PUINT SamplePerSec, PUINT BitsPerSample, PUINT Channels)
{
 int  Fmt;
 long OutputFmt;
 int  WFormats[] = {8000,11025,12000,16000,22050,24000,32000,44100,48000};

 if(this->AudioFormat)this->AudioFormat->get_Type((SpeechAudioFormatType*)&OutputFmt);
   else return 8;
 if((OutputFmt < 4)||(OutputFmt > 39))return 2;     // SAFT8kHz8BitMono - SAFT48kHz16BitStereo
 OutputFmt -= 4;
 Fmt        = (4 - (OutputFmt % 4));
  
 if(SamplePerSec)(*SamplePerSec)   = WFormats[OutputFmt / 4]; 
 if(BitsPerSample)(*BitsPerSample) = (Fmt < 2)?(8):(16);
 if(Channels)(*Channels)           = ((Fmt == 0)||(Fmt == 2))?(1):(2);
 return 0;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::GetSpeechBuffer(PVOID *Buffer, PUINT Size)
{
 VARIANT VValue;
 					   
 if(!this->MemoryStream)return 8;
 ResetVariant(&VValue, VT_I4);
 this->MemoryStream->Seek(VValue,SSSPTRelativeToCurrentPosition,&VValue);
 if(Size)(*Size) = VValue.ulVal;    // VValue.parray->rgsabound->cElements;
 ResetVariant(&VValue, VT_ARRAY);
 this->MemoryStream->GetData(&VValue);
 if(!VValue.parray)return 2;                              
 if(Buffer)(*Buffer) = VValue.parray->pvData;  // SAFEARRAY* struct Object
 DuplicateVariant(&this->CurSpeechBuf, &VValue); // // VT_ARRAY	// VirtualFree(this->PrevVoiceBuf,0,MEM_RELEASE); 
 return 0;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::GetVoicesList(PWSTR DestBuffer, UINT BufferSize, PUINT VCount)
{
 long                Count    = NULL;
 PWSTR               Descr    = NULL;
 ISpeechObjectToken *Token    = NULL;

 if(!this->Voice)return 3;
 this->SVoicesList[0] = 0;
 if(this->VoicesList){this->VoicesList->Release();this->VoicesList = NULL;}
 if(!this->Voice->GetVoices(L"", L"", &this->VoicesList))this->VoicesList->get_Count(&Count);
 if(VCount)(*VCount)  = Count;
 while(Count)
  {
   Count--;
   if(this->VoicesList->Item(Count, &Token))continue;
   Token->GetDescription(0, &Descr);  // No need release this Token ?
   this->SVoicesList = (PWSTR)HeapReAlloc(this->DefHeapHandle,HEAP_ZERO_MEMORY,this->SVoicesList,((lstrlenW(this->SVoicesList)+lstrlenW(Descr))*2)+32);
   lstrcatW(this->SVoicesList,Descr);
   lstrcatW(this->SVoicesList,L"\r\n");
   Token->Release();
  }
 if(DestBuffer)lstrcpynW(DestBuffer,this->SVoicesList,(BufferSize/2));
 return 0;
}
//---------------------------------------------------------------------------
UINT DEFCALL CTalky::GetVoicesList(LPSTR DestBuffer, UINT BufferSize, PUINT VCount)
{
 if(UINT res = this->GetVoicesList((PWSTR)NULL,0,VCount))return res;
 return !WideCharToMultiByte(CP_ACP,0,this->SVoicesList,-1,DestBuffer,BufferSize,NULL,NULL);
}
//---------------------------------------------------------------------------
