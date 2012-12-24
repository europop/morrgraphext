//Code generated automatically from MGE's function documentation
#include "FUNCMGE.h"
extern HANDLE MGEpipe2;
void DoMWSEFunction();

bool FUNCWIPEALL::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7764;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCOUTPUTDEBUGSTRING::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x7064;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCOUTPUTVERSIONSTRING::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7664;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETVERSION::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5664;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	machine.push(Result1);
	return true;
}

bool FUNCGETWIDTH::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7864;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,2,&unused,0);
	machine.push(Result1);
	return true;
}

bool FUNCGETHEIGHT::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7964;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,2,&unused,0);
	machine.push(Result1);
	return true;
}

bool FUNCGETFPS::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6664;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	machine.push(Result1);
	return true;
}

bool FUNCGETEYEVEC::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5064;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	VMREGTYPE Result2=0;
	ReadFile(MGEpipe2,&Result2,4,&unused,0);
	VMREGTYPE Result3=0;
	ReadFile(MGEpipe2,&Result3,4,&unused,0);
	machine.push(Result3);
	machine.push(Result2);
	machine.push(Result1);
	return true;
}

bool FUNCENABLEDISTANTLAND::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6544;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEDISTANTLAND::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6444;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCWIPEHUDELEMENTS::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7748;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCWITHHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x5748;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCCANCELWITHHUD::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6348;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCLOADHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;

	WORD mgecode=0x6c48;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCPOSITIONHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x7048;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSCALEHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x7348;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCFULLSCREENHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x4648;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENABLEHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6548;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6448;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCCHANGEHUDTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;

	WORD mgecode=0x7448;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCCHANGEHUDEFFECT::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;

	WORD mgecode=0x4548;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCFREEHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6648;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDLOADHUD::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6c48;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDPOSITIONHUD::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;

	WORD mgecode=0x7048;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSCALEHUD::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;

	WORD mgecode=0x7348;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDFULLSCREENHUD::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4648;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDENABLEHUD::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6548;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDDISABLEHUD::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6448;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDCHANGEHUDTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x7448;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDCHANGEHUDEFFECT::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x4548;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDFREEHUD::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6648;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCMANUALLIGHTING::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6d77;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENDMANUALLIGHTING::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4d77;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSUNCOLOUR::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x6377;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTSUNCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4377;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSUNDIRECTION::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x6477;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTSUNDIRECTION::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4477;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSUNTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x7477;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTSUNTEXTURE::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5477;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETSUNCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6777;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	VMREGTYPE Result2=0;
	ReadFile(MGEpipe2,&Result2,4,&unused,0);
	VMREGTYPE Result3=0;
	ReadFile(MGEpipe2,&Result3,4,&unused,0);
	machine.push(Result3);
	machine.push(Result2);
	machine.push(Result1);
	return true;
}

bool FUNCGETSUNDIR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4777;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	VMREGTYPE Result2=0;
	ReadFile(MGEpipe2,&Result2,4,&unused,0);
	VMREGTYPE Result3=0;
	ReadFile(MGEpipe2,&Result3,4,&unused,0);
	machine.push(Result3);
	machine.push(Result2);
	machine.push(Result1);
	return true;
}

bool FUNCSETAMBIENTCOLOUR::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x6177;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTAMBIENTCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4177;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETAMBIENTCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6c77;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	VMREGTYPE Result2=0;
	ReadFile(MGEpipe2,&Result2,4,&unused,0);
	VMREGTYPE Result3=0;
	ReadFile(MGEpipe2,&Result3,4,&unused,0);
	machine.push(Result3);
	machine.push(Result2);
	machine.push(Result1);
	return true;
}

bool FUNCSETCLSCOLOR::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x6277;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTCLSCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4277;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETCLSCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4c77;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	VMREGTYPE Result2=0;
	ReadFile(MGEpipe2,&Result2,4,&unused,0);
	VMREGTYPE Result3=0;
	ReadFile(MGEpipe2,&Result3,4,&unused,0);
	machine.push(Result3);
	machine.push(Result2);
	machine.push(Result1);
	return true;
}

bool FUNCSETSKYCOLOUR::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x7377;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTSKYCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5377;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETHUDCOLOUR::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x6877;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTHUDCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4877;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTAPKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x746b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCPUSHKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x706b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCRELESEKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x726b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCHAMMERKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x686b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCUNHAMMERKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x756b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCAHAMMERKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x486b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCAUNHAMMERKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x556b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISALLOWKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x646b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCALLOWKEY::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x616b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCMOVEMOUSERIGHT::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x786b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCMOVEMOUSELEFT::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x586b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCMOVEMOUSEUP::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x796b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCMOVEMOUSEDOWN::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x596b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMOUSESPEEDRIGHT::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x766b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMOUSESPEEDLEFT::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x566b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMOUSESPEEDUP::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x776b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMOUSESPEEDDOWN::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x576b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEMOUSE::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6d6b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENABLEMOUSE::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4d6b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCBUFFEREDKEYDOWN::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x626b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCBUFFEREDKEYUP::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x426b;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENABLEFOG::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6566;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEFOG::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6466;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTOGGLEFOG::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7466;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCINCREASEFOG::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6966;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDECREASEFOG::execute(void)
{
	DWORD unused;

	//WORD mgecode=0x6466;
	WORD mgecode=0x6f66;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCINCREASEFOGBY::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x4966;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDECREASEFOGBY::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	//WORD mgecode=0x4466;
	WORD mgecode=0x4f66;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETFOG::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x7366;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCFOGZOOM::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x7a66;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSTOPFOGZOOM::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5a66;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETFOG::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6766;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	machine.push(Result1);
	return true;
}

bool FUNCSETFOGCOLOUR::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x6366;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDEFAULTFOGCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4366;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETFOGCOLOUR::execute(void)
{
	DWORD unused;

	WORD mgecode=0x4766;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	VMREGTYPE Result2=0;
	ReadFile(MGEpipe2,&Result2,4,&unused,0);
	VMREGTYPE Result3=0;
	ReadFile(MGEpipe2,&Result3,4,&unused,0);
	machine.push(Result3);
	machine.push(Result2);
	machine.push(Result1);
	return true;
}

bool FUNCENABLECAMERASHAKE::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6563;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLECAMERASHAKE::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6463;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETCAMERASHAKEMAGNITUDE::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x7363;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCCAMERASHAKEZOOM::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x7a63;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSTOPCAMERASHAKEZOOM::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5a63;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTAKESCREENSHOT::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7373;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTAKEBMPSCREENSHOT::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3073;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTAKEJPEGSCREENSHOT::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3173;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTAKEDDSSCREENSHOT::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3473;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTAKEPNGSCREENSHOT::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3573;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENABLEFPSCOUNTER::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6546;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEFPSCOUNTER::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6446;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTOGGLEFPSCOUNTER::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7446;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCROTATESCREEN::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3972;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSCREENROT0::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3072;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSCREENROT90::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3172;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSCREENROT180::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3272;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSCREENROT270::execute(void)
{
	DWORD unused;

	WORD mgecode=0x3372;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCROTATESCREENBY::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x7272;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETSCREENROTATION::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x5272;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSCREENSPIN::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x7372;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSTOPSPINSPIN::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5372;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETSCREENROTATION::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6772;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	machine.push(Result1);
	return true;
}

bool FUNCENABLEZOOM::execute(void)
{
	DWORD unused;

	WORD mgecode=0x657a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEZOOM::execute(void)
{
	DWORD unused;

	WORD mgecode=0x647a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTOGGLEZOOM::execute(void)
{
	DWORD unused;

	WORD mgecode=0x747a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZOOMIN::execute(void)
{
	DWORD unused;

	WORD mgecode=0x697a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZOOMOUT::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6f7a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZOOMINBY::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x497a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZOOMOUTBY::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x4f7a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETZOOM::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x737a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZOOM::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x7a7a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSTOPZOOM::execute(void)
{
	DWORD unused;

	WORD mgecode=0x5a7a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCGETZOOM::execute(void)
{
	DWORD unused;

	WORD mgecode=0x677a;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	VMREGTYPE Result1=0;
	ReadFile(MGEpipe2,&Result1,4,&unused,0);
	machine.push(Result1);
	return true;
}

bool FUNCLOADTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6c74;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCFREETEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6674;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCWIPETEXTURES::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7774;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCBEGINTEXDRAW::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6274;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENDTEXDRAW::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6574;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETTEXDRAWORIGIN::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;

	WORD mgecode=0x6f74;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETTEXDRAWSCALE::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;

	WORD mgecode=0x7374;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETTEXDRAWCOLOUR::execute(void)
{
	DWORD unused;
	VMREGTYPE Arg1;
	if(!machine.pop(Arg1)) return false;

	WORD mgecode=0x6374;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDRAWTEXTTOTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x5474;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDRAWTEXTURETOTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x7474;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCAUTOFREEHOOK::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6154;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDONTFREEHOOK::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6454;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCRESUMEVIDEO::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x7276;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCPAUSEVIDEO::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x7076;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSTOPVIDEO::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x7376;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCREWINDVIDEO::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x5276;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSEEKVIDEO::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE Arg2;
	if(!machine.pop(Arg2)) return false;

	WORD mgecode=0x5376;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENABLEFULLSCREENSHADER::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6553;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEFULLSCREENSHADERS::execute(void)
{
	DWORD unused;

	WORD mgecode=0x6453;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCTOGGLEFULLSCREENSHADERS::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7453;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCLOADEFFECT::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6c53;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETEFFECTTECHNIQUE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;

	WORD mgecode=0x7353;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCUSEEFFECTASFULLSCREENSHADER::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x4553;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEEFFECTASFULLSCREENSHADER::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x4453;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCFREEEFFECT::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6653;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETEFFECTVARLONG::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;
	VMREGTYPE Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x4c53;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETEFFECTVARFLOAT::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x4653;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETEFFECTVARVECTOR::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;
	VMFLOAT Arg5;
	if(!machine.pop(Arg5)) return false;
	VMFLOAT Arg6;
	if(!machine.pop(Arg6)) return false;

	WORD mgecode=0x5653;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	WriteFile(MGEpipe2,&Arg5,4,&unused,0);
	WriteFile(MGEpipe2,&Arg6,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETEFFECTVARTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;
	VMREGTYPE pArg3;
	if(!machine.pop(pArg3)) return false;
	const char* Arg3=machine.GetString((VPVOID)pArg3);
	if(!Arg3) return false;

	WORD mgecode=0x5453;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	WriteFile(MGEpipe2,Arg3,strlen(Arg3)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCWIPELOADEDEFFECTS::execute(void)
{
	DWORD unused;

	WORD mgecode=0x7753;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCWITHMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x576d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCCANCELWITHMESH::execute(void)
{
	DWORD unused;

	WORD mgecode=0x636d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCWIPELOADEDMESHES::execute(void)
{
	DWORD unused;

	WORD mgecode=0x776d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCLOADMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;

	WORD mgecode=0x6c6d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHPOSITION::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x706d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHROTATION::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x726d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHSCALE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x736d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCMOVEMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	//WORD mgecode=0x506d;
	WORD mgecode=0x4d6d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCROTATEMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x526d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSCALEMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x536d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHVELOCITY::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x766d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHANGULARVELOCITY::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x616d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHGROWTH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;
	VMFLOAT Arg4;
	if(!machine.pop(Arg4)) return false;

	WORD mgecode=0x676d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	WriteFile(MGEpipe2,&Arg4,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZEROMESHVELOCITY::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x566d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZEROMESHANGULARVELOCITY::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x416d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCZEROMESHGROWTH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x476d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCENABLEMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x656d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCDISABLEMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x646d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;
	VMREGTYPE Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x746d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg3,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCSETMESHEFFECT::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE pArg2;
	if(!machine.pop(pArg2)) return false;
	const char* Arg2=machine.GetString((VPVOID)pArg2);
	if(!Arg2) return false;

	WORD mgecode=0x456d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,Arg2,strlen(Arg2)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCMANUALMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x446d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCFREEMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x666d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCUPDATEMESHLIGHTING::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x756d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDLOADMESH::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x6c6d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHPOSITION::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x706d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHROTATION::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x726d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHSCALE::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x736d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDMOVEMESH::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	//WORD mgecode=0x506d;
	WORD mgecode=0x4d6d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDROTATEMESH::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x526d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSCALEMESH::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x536d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHVELOCITY::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x766d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHANGULARVELOCITY::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x616d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHGROWTH::execute(void)
{
	DWORD unused;
	VMFLOAT Arg1;
	if(!machine.pop(Arg1)) return false;
	VMFLOAT Arg2;
	if(!machine.pop(Arg2)) return false;
	VMFLOAT Arg3;
	if(!machine.pop(Arg3)) return false;

	WORD mgecode=0x676d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,&Arg1,4,&unused,0);
	WriteFile(MGEpipe2,&Arg2,4,&unused,0);
	WriteFile(MGEpipe2,&Arg3,4,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDZEROMESHVELOCITY::execute(void)
{
	DWORD unused;

	WORD mgecode=0x566d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDZEROMESHANGULARVELOCITY::execute(void)
{
	DWORD unused;

	WORD mgecode=0x416d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDZEROMESHGROWTH::execute(void)
{
	DWORD unused;

	WORD mgecode=0x476d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDENABLEMESH::execute(void)
{
	DWORD unused;

	WORD mgecode=0x656d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDDISABLEMESH::execute(void)
{
	DWORD unused;

	WORD mgecode=0x646d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHTEXTURE::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;
	VMREGTYPE Arg2;
	if(!machine.pop(Arg2)) return false;

	WORD mgecode=0x746d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	WriteFile(MGEpipe2,&Arg2,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDSETMESHEFFECT::execute(void)
{
	DWORD unused;
	VMREGTYPE pArg1;
	if(!machine.pop(pArg1)) return false;
	const char* Arg1=machine.GetString((VPVOID)pArg1);
	if(!Arg1) return false;

	WORD mgecode=0x456d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	WriteFile(MGEpipe2,Arg1,strlen(Arg1)+1,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDMANUALMESH::execute(void)
{
	DWORD unused;

	WORD mgecode=0x446d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDFREEMESH::execute(void)
{
	DWORD unused;

	WORD mgecode=0x666d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

bool FUNCNIDUPDATEMESHLIGHTING::execute(void)
{
	DWORD unused;

	WORD mgecode=0x756d;
	WriteFile(MGEpipe2,&mgecode,2,&unused,0);
	DoMWSEFunction();
	return true;
}

