//InstPop.cpp

#include "INSTPOP.h"

INSTPOP::INSTPOP(VIRTUALMACHINE& vm) : machine(vm)
{
}

int INSTPOP::GetOperands(OPCODE opcode, VPVOID operanddata)
{
	int result= -1;
	
	if(machine.ReadMem(operanddata,&stackincrease,sizeof(stackincrease)))
		result= sizeof(stackincrease);
	else
		result= -1;
		
	return result;
}

bool INSTPOP::execute(void)
{
	VMREGTYPE stackpointer= 0;
	
	bool result= machine.GetRegister(SP,stackpointer)
		&& machine.SetRegister(SP,stackpointer+stackincrease);
		
	return result;
}

INSTPOPREG::INSTPOPREG(VIRTUALMACHINE& vm) : machine(vm)
{
}

int INSTPOPREG::GetOperands(OPCODE opcode, VPVOID operanddata)
{
	int result= -1;
	
	if(machine.ReadMem(operanddata,&regidx,sizeof(regidx)))
		result= sizeof(regidx);
	else
		result= -1;
		
	return result;
}

bool INSTPOPREG::execute(void)
{
	VMREGTYPE val= 0;
	
	bool result= machine.pop(val) && machine.SetRegister(GP+regidx,val);
	return result;
}
