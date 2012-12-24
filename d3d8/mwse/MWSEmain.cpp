#include "MWSEmain.h"
#include "Breakpoint.h"
#include "TES3MACHINE.h"
#include "TES3MemMap.h"
#include "FILECACHE.h"

static void OnDecode(Context* context);
static void OnRunFuncIdxError(Context* context);
static void OnRunScript(Context* context);
static void OnFixupScript(Context* context);

static TES3MACHINE* scriptmachine;
static VPVOID prevscript;
static HWBREAKPOINT* HWbreakpoint;

static void OnOtherBreakpoint(Context* context) {
	RemoveBreakpoint(HWbreakpoint->getid());
	scriptmachine->SetVMDebuggerBreakpoint(0);
	scriptmachine->SetFlow(*context);
	HWbreakpoint->breakpoint();
	*context= scriptmachine->GetFlow();
	HWbreakpoint= scriptmachine->GetVMDebuggerBreakpoint();
	if(HWbreakpoint) AddBreakpoint(HWbreakpoint->getid(), OnOtherBreakpoint);
}

static bool ChangeReference(VPVOID pscript)
{
	bool result= false;
	if(pscript!=prevscript)
	{
		prevscript= pscript;
		if(scriptmachine->SetScript((VPSCRIPT)pscript))
		{
			if(scriptmachine->Interrupt(INTSWITCHREFERENCE))
			{
				result= true;
			}
			else
				LOG::log("ChangeReference: Interrupt failed\n");
		}
		else
			LOG::log("ChangeReference: SetScript failed: %s\n");
	}

	return result;
}

static void OnDecode(Context* context) {
	if(scriptmachine->IsInstruction(context->Eax)) //opcode //check if the instruction exists in our instruction table
	{
		ChangeReference((VPVOID)context->Ebx); //script

		VMREGTYPE ip= (VMREGTYPE)context->Ebp; //sip
		if(scriptmachine->SetRegister(IP,ip))
		{
			while(scriptmachine->step(true))
			{
			}
			scriptmachine->step(true);

			if(scriptmachine->GetRegister(IP,ip))
			{
				OPCODE opcode= 0;
				if(scriptmachine->GetInstruction((VPVOID)ip,opcode)
					&& scriptmachine->WriteMem((VPVOID)context->Esp+sizeof(context->Edi),&ip,sizeof(ip)))
				{
					context->Eax= (DWORD)opcode;
					context->Ebp= (DWORD)ip;
				}
				else
					LOG::log("OnDecode: GetInstruction failed\n");
			}
			else
				LOG::log("OnDecode: GetRegister(IP) failed\n");
		}
		else
			LOG::log("OnDecode: SetRegister(IP) failed\n");
	}
}

static void OnRunFuncIdxError(Context* context) {
	OPCODE opcode= RUNFUNCIDXTOCODE(context->Edx);
	VPVOID stack= (VPVOID)context->Esp;
	if(scriptmachine->IsInstruction(opcode))
	{
		VPVOID ppscript= (VPVOID)context->Ebp+0x48;
		VPVOID pscript= 0;
		if(scriptmachine->ReadMem(ppscript,&pscript,sizeof(pscript)))
		{
			ChangeReference(pscript);

			VMREGTYPE ip= 0;
			if(scriptmachine->ReadMem(stack+sizeof(context->Edi),&ip,sizeof(ip)))
			{
				ip-= sizeof(OPCODE);
				scriptmachine->SetRegister(IP,(VMREGTYPE)ip);

				context->Eip= (DWORD)reltolinear(RUNFUNCIDXERRCONTINUE);
				scriptmachine->SetVMDebuggerBreakpoint(0);
				scriptmachine->SetFlow(*context);
				scriptmachine->step();

				if(scriptmachine->GetRegister(IP,ip))
				{
					if(scriptmachine->WriteMem(stack+sizeof(context->Edi),&ip,sizeof(ip)))
					{
						*context= scriptmachine->GetFlow();
						HWbreakpoint= scriptmachine->GetVMDebuggerBreakpoint();
						if(HWbreakpoint) AddBreakpoint(HWbreakpoint->getid(), OnOtherBreakpoint);
					}
					else
						LOG::log("OnRunFuncIdxError: WriteMem IP failed\n");
				}
				else
					LOG::log("OnRunFuncIdxError: GetRegister(IP) post run failed\n");
			}
			else
				LOG::log("OnRunFuncIdxError: GetRegister(IP) pre run failed\n");
		}
		else
			LOG::log("OnRunFuncIdxError: ReadMem pscript failed: %s\n");
	}
}

static void OnRunScript(Context* context) {
	AddBreakpoint(BP_FIXUPSCRIPT, OnFixupScript);
	RemoveBreakpoint(BP_RUNSCRIPT);

	AddBreakpoint(BP_RUNFUNCIDXERR, OnRunFuncIdxError);
	RemoveBreakpoint(BP_DECODEWITHINFO);
}

static void OnFixupScript(Context* context) {
	AddBreakpoint(BP_RUNSCRIPT, OnRunScript);
	RemoveBreakpoint(BP_FIXUPSCRIPT);

	AddBreakpoint(BP_DECODEWITHINFO, OnDecode);
	RemoveBreakpoint(BP_RUNFUNCIDXERR);
}

void MWSEOnProcessStart(HANDLE pipe) {
	HANDLE h=CreateFileA("mge3\\mwse", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(h!=INVALID_HANDLE_VALUE) {
		CloseHandle(h);
		LOG::logline("MWSE: Disabled");
		return;
	}
	LOG::logline("MWSE: setting page protection");
	SetupPageProtection();
	LOG::logline("MWSE: setting initial breakpoints");
	AddBreakpoint(BP_RUNSCRIPT, OnRunScript);
	AddBreakpoint(BP_FIXUPSCRIPT, OnFixupScript);

	scriptmachine=new TES3MACHINE();
	if (pipe != INVALID_HANDLE_VALUE) FILECACHE::SetPipe(pipe);
}