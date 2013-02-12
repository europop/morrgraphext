//InstNop.h
#pragma once

#include "INSTRUCTION.h"

struct INSTNOP : public INSTRUCTION
{
	virtual int GetOperands(OPCODE opcode, VPVOID operanddata)
	{
		return 1;
	}
	virtual bool execute(void)
	{
		return true;
	}
};
