
#include <Morrowind.h>

using namespace MORROWIND;
//====================================================================================
bool MWTHISCALL CCamera::SetPersonViewMode(bool ThirdPerson)
{
 bool LastState = this->ViewThirdPerson;
 if(ThirdPerson != (bool)this->ViewThirdPerson)this->SwitchViewMode = 1;
 return LastState;
}
//====================================================================================

//====================================================================================
