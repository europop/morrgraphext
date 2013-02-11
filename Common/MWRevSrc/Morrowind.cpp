
#include <Morrowind.h>

using namespace MORROWIND;
//====================================================================================
bool MWTHISCALL CGameScene::EnableCrosshair(bool ChEnable)   
{
 bool LastState = !this->CrosshairDisabled;
 this->CrosshairDisabled = !ChEnable; 
 return LastState;
}
//---------------------------------------------------------------------------
bool MWTHISCALL CGameScene::ShowCrosshair(bool ChShow)
{
 bool LastState = false;
 if(this->pCenterTarget)            
  {
   LastState = this->pCenterTarget->GetObjectCulled();
   this->pCenterTarget->SetObjectCulled(!ChShow);
  }
 return LastState; 
}
//--------------------------------------------------------------------------- 
bool MWTHISCALL CGameScene::SetGodMode(bool Enable)	 
{
 bool prev = (this->InterfManager->StateFlags & 0x00000004); // 0x00000004 = GodMode; 0x00000008 = AI_Off;
 if(Enable)this->InterfManager->StateFlags |= 0x00000004;
   else this->InterfManager->StateFlags &= ~0x00000004; 
 return prev;
}
//---------------------------------------------------------------------------

bool MWTHISCALL CGameScene::SetEnabledAI(bool Enable)	 
{
 bool prev = (this->InterfManager->StateFlags & 0x00000008); // 0x00000004 = GodMode; 0x00000008 = AI_Off;
 if(Enable)this->InterfManager->StateFlags &= ~0x00000008;
   else this->InterfManager->StateFlags |= 0x00000008; 
 return !prev;
}
//---------------------------------------------------------------------------
void MWTHISCALL CGameScene::SetCurrentFOV(float NewFOV)
{
 this->ShadowSceneGraph.Camera._ChangeFOV(NewFOV);
 this->WorldSceneGraph.Camera._ChangeFOV(NewFOV);
}
//====================================================================================
// Returned sound may be FREED by other thread at any time!!! - ALWAYS make checks
PCSoundRef MWTHISCALL CTESGame::FindSoundForObject(PCObjREFR ObjectRef, int *MatchIndex)
{
 PCLinkedList SndList;
 
 if(!ObjectRef || !ObjectRef->Object)return NULL;
 if(ObjectRef->Object->Type == otLIGH)SndList = this->LightSounds;
   else SndList = ActiSounds;
 PCListItem SItem    = SndList->FirstItem;
 int        MCounter = (MatchIndex)?(*MatchIndex):(0);  
 for(int ctr=0,index=0;SItem;ctr++)
  {
   if(SItem->Data&&(((PCSoundRef)SItem->Data)->Reference == ObjectRef))
    {
     index++;   
     if(index > MCounter)
      {      
       if(MatchIndex)(*MatchIndex) = index;
       return (!IsBadReadPtr(SItem,sizeof(CListItem)))?((PCSoundRef)SItem->Data):(NULL);
      } 
    } 
   SItem = SItem->Next;
  }
 return NULL;
}   
//====================================================================================
