
#include <Morrowind.h>

using namespace MORROWIND;
//====================================================================================
bool MWTHISCALL CObjMACT::SetActorSneaking(bool Sneak)
{
 bool LastState = ((this->Flags >> 10) & 1); 
 if(Sneak)this->Flags = ((this->Flags & 0xFDFF) | 0x0500);                         
   else this->Flags = ((this->Flags & 0xFAFF) | 0x0200); 
 return LastState;         
}
//====================================================================================
bool MWTHISCALL CObjMACP::EnableLooking(bool Enable)
{
 bool LastState = this->MouseDisabled;
 this->MouseDisabled = !Enable;
 return !LastState;
}
//====================================================================================
bool MWTHISCALL CObjMACP::EnableMovement(bool Enable)
{
 bool LastState = this->KeyboardDisabled;
 this->KeyboardDisabled = !Enable;
 return !LastState;
}
//====================================================================================
PCAnimation MWTHISCALL CObjREFR::GetItemData(void)
{
 PCAListItem ListItem = this->Attachments;
 while(ListItem){if(ListItem->Type==0)return (PCAnimation)ListItem->Data;else ListItem = ListItem->Next;}
 return NULL;
}   
//------------------------------------------------------------------------------------
PCAListItem MWTHISCALL CObjREFR::GetEmptyItem(void)
{
 PCAListItem ListItem = this->Attachments;
 while(ListItem){if(ListItem->Type==0x0B)return ListItem;else ListItem = ListItem->Next;}
 return NULL;
}
//====================================================================================
void MWTHISCALL CObjREFR::SetPosition(float PosX, float PosY, float PosZ)
{
 if(!this)return;
 this->PosX = PosX;
 this->PosY = PosY;
 this->PosZ = PosZ;
}    
//====================================================================================
void MWTHISCALL CObjREFR::GetPosition(float *PosX, float *PosY, float *PosZ)
{
 if(!this){if(PosX)(*PosX)=0;if(PosY)(*PosY)=0;if(PosZ)(*PosZ)=0;return;}
 if(PosX)(*PosX) = this->PosX;       
 if(PosY)(*PosY) = this->PosY; 
 if(PosZ)(*PosZ) = this->PosZ; 
}
//====================================================================================
float MWTHISCALL CObjREFR::GetDistance(PCObjREFR TargetRef)
{
 if(!this)return -1;
 float DiffX = (TargetRef->PosX-this->PosX);     
 float DiffY = (TargetRef->PosY-this->PosY);      
 float DiffZ = (TargetRef->PosZ-this->PosZ);
 return sqrt((DiffX*DiffX)+(DiffY*DiffY)+(DiffZ*DiffZ));
}           
//====================================================================================

//====================================================================================
