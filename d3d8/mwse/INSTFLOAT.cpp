//InstFloat.cpp

#include "INSTFLOAT.h"
#include "math.h"

#define M_PI 3.141592653f
//#if sizeof(VMREGTYPE) <> sizeof(VMFLOAT)
//#error "VMREGTYPE and VMFLOAT must be the same size!"
//#endif

VMREGTYPE ifromf(VMFLOAT f)
{
	return *(VMREGTYPE*)&f;
}

VMFLOAT ffromi(VMREGTYPE i)
{
	return *(VMFLOAT*)&i;
}

INSTITOF::INSTITOF(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTITOF::execute(void)
{
	bool result= false;
	VMREGTYPE val= 0,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= (VMFLOAT)val;
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTFTOI::INSTFTOI(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTFTOI::execute(void)
{
	bool result= false;
	VMREGTYPE val= 0,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		sum= (VMREGTYPE)f;
		result= machine.push(sum);
	}
	
	return result;
}

INSTFADD::INSTFADD(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTFADD::execute(void)
{
	bool result= false;
	VMREGTYPE val1,val2,sum= 0;
	if(machine.pop(val1)
		&& machine.pop(val2))
	{
		VMFLOAT f1= ffromi(val1);
		VMFLOAT f2= ffromi(val2);
		f1= f1 + f2;
		sum= ifromf(f1);
		result= machine.push(sum);
	}
	
	return result;
}

INSTFSUB::INSTFSUB(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTFSUB::execute(void)
{
	bool result= false;
	VMREGTYPE val1,val2,sum= 0;
	if(machine.pop(val1)
		&& machine.pop(val2))
	{
		VMFLOAT f1= ffromi(val1);
		VMFLOAT f2= ffromi(val2);
		f1= f1 - f2;
		sum= ifromf(f1);
		result= machine.push(sum);
	}

	return result;
}

INSTFMUL::INSTFMUL(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTFMUL::execute(void)
{
	bool result= false;
	VMREGTYPE val1,val2,sum= 0;
	if(machine.pop(val1)
		&& machine.pop(val2))
	{
		VMFLOAT f1= ffromi(val1);
		VMFLOAT f2= ffromi(val2);
		f1= f1 * f2;
		sum= ifromf(f1);
		result= machine.push(sum);
	}

	return result;
}

INSTFDIV::INSTFDIV(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTFDIV::execute(void)
{
	bool result= false;
	VMREGTYPE val1,val2,sum= 0;
	if(machine.pop(val1)
		&& machine.pop(val2))
	{
		VMFLOAT f1= ffromi(val1);
		VMFLOAT f2= ffromi(val2);
		f1= f1 / f2;
		sum= ifromf(f1);
		result= machine.push(sum);
	}
	
	return result;
}

INSTTAN::INSTTAN(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTTAN::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= tan(f);
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTSIN::INSTSIN(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTSIN::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= sin(f);
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTCOS::INSTCOS(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTCOS::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= cos(f);
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}


INSTARCTAN::INSTARCTAN(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTARCTAN::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= atan(f);
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTARCSIN::INSTARCSIN(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTARCSIN::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= asin(f);
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTARCCOS::INSTARCCOS(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTARCCOS::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= acos(f);
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTDEGRAD::INSTDEGRAD(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTDEGRAD::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= f*M_PI/180;
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTRADDEG::INSTRADDEG(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTRADDEG::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= f/M_PI*180;
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTSQRT::INSTSQRT(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTSQRT::execute(void)
{
	bool result= false;
	VMREGTYPE val,sum= 0;
	if(machine.pop(val))
	{
		VMFLOAT f= ffromi(val);
		f= sqrt(f);
		sum= ifromf(f);
		result= machine.push(sum);
	}
	
	return result;
}

INSTHYPOT::INSTHYPOT(VIRTUALMACHINE& vm) : machine(vm)
{
}

bool INSTHYPOT::execute(void)
{
	bool result= false;
	VMREGTYPE val1,val2,sum= 0;
	if(machine.pop(val1)
		&& machine.pop(val2))
	{
		VMFLOAT f1= ffromi(val1);
		VMFLOAT f2= ffromi(val2);
		f1=(VMFLOAT) _hypot(f1,f2);
		sum= ifromf(f1);
		result= machine.push(sum);
	}
	
	return result;
}
