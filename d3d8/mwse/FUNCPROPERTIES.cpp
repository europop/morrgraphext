//FuncProperties.cpp

#include "FUNCPROPERTIES.h"
#include "REFERENCE.h"
#include "TES3MEMMAP.h"
#include "TES3OPCODES.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////


// Return maximum condition of item.
static ULONG GetMaxCondition(TES3MACHINE &machine, VPVOID refr, ULONG type)
{
    ULONG value = 0;
    ULONG offset = offsetOfCondition(type); // misnomer, actually MaxCondition
    if ( offset ) {
        GetOffsetData(machine, refr, offset, &value);
        if ( type == WEAPON) {
            // Weapon uses upper 16 bits of value
            value >>= 16;
        }
    }
    return value;
}

// Return offset of maximum charge for item, if possible. Return 0 if
// not found.
static VPVOID GetMaxChargeOffset(TES3MACHINE &machine, VPVOID refr, ULONG type)
{
    ULONG ench = 0;
    ULONG autoCalc = 0;

    if ( type == ARMOR ) {
        // Armour.
		GetOffsetData(machine, refr, 0x30, &ench);
    } else if ( type == CLOTHING ) {
        // Clothing.
		GetOffsetData(machine, refr, 0x2d, &ench);
    } else if ( type == WEAPON ) {
        // Weapon.
		GetOffsetData(machine, refr, 0x1d, &ench);
    }

    if (ench) {  // found enchantment record
		GetOffsetData(machine,reinterpret_cast<VPVOID>(ench),0x10,&autoCalc);
    }
    // Only the low-order byte of 'autoCalc' is used.
    // FIXME: it is not clear if this is correct.
    return ((autoCalc & 0xFF)!=0) ? reinterpret_cast<VPVOID>(ench) : NULL;
}

// Return maximum charge of item.
static ULONG GetMaxCharge(TES3MACHINE &machine, VPVOID refr, ULONG type)
{
	VPVOID offset = 0;
	ULONG value = 0;
    offset = GetMaxChargeOffset(machine, refr, type);
    if (offset) {  // found enchantment record
        // actually stored as integer
		GetOffsetData(machine,offset,0xc,&value);
	}
    return value;
}

	
// GRM 15 Jan 2007
// Set value (worth in gold) of reference.
// <ref> xSetValue <long>->[no value]
bool FUNCSETVALUE::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG offset = 0;
	VMREGTYPE value = 0;
    bool set = false;
	char idstr[129];
    idstr[sizeof idstr-1] = '\0';

	if (GetTargetData(machine, &refr, &temp, &type) && machine.pop(value))
	{
		offset = offsetOfValue(type);
        // MISC is possibly gold.
        bool isGold = type == MISC &&
            GetIdString(machine, temp, idstr) &&
            strncmp(idstr, "gold_", 5) == 0;
        if (offset != 0 && !isGold) {
            if (type == CLOTHING) {
                // Clothing is a short. Since SetOffsetData only stores
                // longs, we need to fetch the value, and modify the
                // lower 16 bits.
                unsigned long oldValue;
                if (GetOffsetData(machine, temp, offset, &oldValue)) {
                    value = (oldValue & ~0xFFFF) | (value & 0xFFFF);
                }
            }
            // Note: count is ignored.
            set = SetOffsetData(machine, temp, offset, value);
        }
	}
	return machine.push(static_cast<VMREGTYPE>(set));
}

// Set current charge of item, if possible
//  ref xSetCharge <float>->[no value]
bool FUNCSETCHARGE::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	VMFLOAT charge = 0;
    bool set = false;

    if (GetTargetData(machine, &refr, &temp, &type) && machine.pop(charge))
	{
        // Set charge in attached node. If not present, cannot
        // set current charge.
        set = SetAttachData(machine, refr, VARNODE, 4, _cpp_min(charge,
                                                               GetMaxCharge(machine, temp, type)));
	}
	return machine.push(static_cast<VMREGTYPE>(set));
}

// Set current maximum charge of item, if possible
//  ref xSetCharge <float>->[no value]
bool FUNCSETMAXCHARGE::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	VMFLOAT maxcharge = 0;
    bool set = false;

	if (GetTargetData(machine, &refr, &temp, &type) && machine.pop(maxcharge))
	{
        VPVOID offset = GetMaxChargeOffset(machine, temp, type);
        if (offset) {
            VMFLOAT charge;
            // If the new max charge is less than the current charge,
            // force it to match.
            if (GetAttachData(machine, refr, VARNODE, 4, charge) &&
                charge >= maxcharge) {
                SetAttachData(machine, refr, VARNODE, 4, maxcharge);
            }
            // actually stored as integer
			set = SetOffsetData(machine,offset,0xc,static_cast<int>(maxcharge));
		}
	}
	return machine.push(static_cast<VMREGTYPE>(set));
}

// GRM 17 Jan 2007
// <ref> xSetCondition <long>->[no value]
bool FUNCSETCONDITION::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	VMREGTYPE value = 0;
    bool set = false;

    if (GetTargetData(machine, &refr, &temp, &type) && machine.pop(value)
        && (type == WEAPON || type == ARMOR))
	{
        // Condition is only stored as attached data. (Note that
        // GetCondition returns the maximum condition if the
        // attached data is not found).
        //
        // TODO: If the attached data is not found, it should be
        // created.
        set = SetAttachData(machine, refr, VARNODE, 3, _cpp_min(static_cast<ULONG>(value),
            GetMaxCondition(machine, temp, type)));
	}
	return machine.push(static_cast<VMREGTYPE>(set));
}
// GRM 17 Jan 2007
// <ref> xSetMaxCondition <long>->[no value]
bool FUNCSETMAXCONDITION::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	VMREGTYPE value = 0;
    bool set = false;

	if (GetTargetData(machine, &refr, &temp, &type) && machine.pop(value)
        && (type == WEAPON || type == ARMOR))
	{
		ULONG offset = offsetOfCondition(type); // misnomer, actually MaxCondition
        if ( offset ) {
            // If the new max condition is less than the current condition,
            // force it to match.
            VMREGTYPE condition;
            if (GetAttachData(machine, refr, VARNODE, 3, condition) &&
                condition > value) {
                    SetAttachData(machine, refr, VARNODE, 3, value);
            }

            if ( type == WEAPON) {
                // Weapon uses upper 16 bits of value
                ULONG oldValue;
                GetOffsetData(machine, temp, offset, &oldValue);
                value = (value << 16) | (oldValue & 0xFFFF);
            }

			set = SetOffsetData(machine, temp, offset, value);
        }
	}
	return machine.push(static_cast<VMREGTYPE>(set));
}

// GRM 15 Jan 2007
//  Common code for SETxxx functions.
//  ref xSetXxxxx <long|float>->[no value]
bool CommonSet::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG offset = 0;
	VMFLOAT value = 0;
    bool set = false;

    if (GetTargetData(machine, &refr, &temp, &type) && machine.pop(value) &&
        canSet(type))
	{
		offset = getTypeOffset(type);
        if (offset != 0) {
            // Note: count is ignored.
            set = SetOffsetData(machine, temp, offset, value);
		}
	}
	return machine.push(static_cast<VMREGTYPE>(set));
}


//Tp21 22-08-2006: xGetSpellEffects
//TODO does it work?
FUNCGETSPELLEFFECTS::FUNCGETSPELLEFFECTS(TES3MACHINE& vm) : machine(vm), HWBREAKPOINT()
{
}

bool FUNCGETSPELLEFFECTS::execute(void)
{
	bool result= true;
	VMREGTYPE pString= 0;
	const char* string= "null";
	VMREGTYPE count= 0;

	if(machine.pop(pString) && (string=machine.GetString((VPVOID)pString))!=0)
	{
		VMLONG strlength= strlen((const char*)string);
		parent= machine.GetFlow();
		result= machine.WriteMem((VPVOID)reltolinear(SECONDOBJECT_LENGTH_IMAGE),&strlength,sizeof(strlength))
			&& machine.WriteMem((VPVOID)reltolinear(SECONDOBJECT_IMAGE),(void*)string,strlength+1);
		if(result)
		{
			Context context= machine.GetFlow();
			context.Eip= (DWORD)reltolinear(FIXUPTEMPLATE);
			machine.SetFlow(context);
			result= machine.SetVMDebuggerBreakpoint(this);
		}
	}
	else
		result= false;

	return result;
}

BYTE FUNCGETSPELLEFFECTS::getid()
{
	return BP_FIXUPTEMPLATE;
}

bool FUNCGETSPELLEFFECTS::breakpoint()
{
	bool result= false;
	Context flow= machine.GetFlow();
	if(machine.WriteMem((VPVOID)reltolinear(SECONDOBJECT_IMAGE),&flow.Eax,sizeof(flow.Eax)))
	{
		machine.SetFlow(parent);
		result= CallOriginalFunction(machine,ORIG_GETSPELLEFFECTS);
	}
	

	return result;
}

bool FUNCGETVALUE::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG offset = 0;
	ULONG count = 0;
	ULONG value = 0;
	char idstr[129];
	idstr[128] = 0;
	if (GetTargetData(machine, &refr, &temp, &type))
	{
		offset = offsetOfValue(type);
		if (offset && GetOffsetData(machine, temp, offset, &value))
		{
			// clothing value is stored in a short not long like all others
			if (type == 'TOLC')
				value %= 65536;
	
			// gold is special (it's always worth one, but not in the CS)
			if ( type == 'CSIM' && GetIdString(machine, temp, idstr))
				if (!strncmp(idstr,"Gold_",5 )) 
					value = 1;
	
			if (GetAttachData(machine, refr, VARNODE, 0, &count))
				value *= count;
		}
	}
	return machine.push((VMREGTYPE)value);
}

bool FUNCGETOWNER::execute(void)
{
	VPVOID refr;
	ULONG data;
	ULONG id = 0;
	char idstr[129];
	idstr[128] = 0;
	if (GetTargetData(machine, &refr))
	{
		if (GetAttachData(machine, refr, VARNODE, 1, &data) && data)
		{
			if (GetIdString(machine, (VPVOID)data, idstr))
				id= (ULONG)strings.add((const char*)idstr);
			else
				id= (ULONG)strings.add("unknown");
		}
	}
		
	return machine.push((VMREGTYPE)id);
}


bool FUNCGETWEIGHT::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG offset = 0;
	ULONG count = 0;
	VMFLOAT weight = 0.0;
	
	if (GetTargetData(machine, &refr, &temp, &type))
	{
		if ( offset = offsetOfWeight(type) )
			GetOffsetData(machine,temp,offset,(ULONG*)&weight);
		
		if ( GetAttachData(machine, refr, VARNODE, 0, &count) )
			weight *= count;
	}
	return machine.push((VMFLOAT)weight);
}


bool FUNCGETENCUMB::execute(void)
{
	bool result= false;
	VMPTR<TES3REFERENCE*> pref(machine);
	VMPTR<TES3REFERENCE> ref(machine);
	VMPTR<TES3LISTNODE> stacknode(machine);
	VMPTR<TES3STACK> thisstack(machine);
	VMPTR<TES3TEMPLATE> templ(machine);
	VPLISTNODE pstacknode= 0;
	
	bool leveled = false;
	VMFLOAT totalweight = 0.0;
	VMFLOAT weight = 0.0;
	int i;
	try
	{
		pref= (TES3REFERENCE**)reltolinear(SCRIPTTARGETREF_IMAGE);
		if (!*pref) throw 0;
	
		ref= *pref;
		if ( REFERENCE::GetInventory(machine,*pref,pstacknode) )
		{
			while ( pstacknode )
			{
				stacknode = pstacknode;
				if (stacknode->dataptr)
				{
					thisstack = (VPSTACK)stacknode->dataptr;
					if (thisstack->templ)
					{
						templ = thisstack->templ;
						if ( i = offsetOfWeight(templ->type) ) 
						{
							machine.ReadMem(LONGOFFSET(thisstack->templ,i),&weight,sizeof(weight));
							totalweight += (weight * abs((int)thisstack->count));
						}
						else if ( templ->type == 'IVEL' )
						{
							totalweight += 0.000001f;
							leveled = true;
						}
					}
				}
				pstacknode = stacknode->next;
			}
		}
		if ( leveled )
			totalweight *= -1;
		result= machine.push((VMFLOAT)totalweight);
	}
	catch(...)
	{
		result= machine.push((VMFLOAT)totalweight);
	}
	
	return result;
}


bool FUNCGETCONDITION::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG value = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type))
	{
		if (!GetAttachData(machine, refr, VARNODE, 3, &value))
		{
			ULONG offset = offsetOfCondition(type);
			if ( offset )
				GetOffsetData(machine, temp, offset, &value);
			if ( type == 'PAEW' ) 
				value /= 65536;   // weapon uses short stored in upper half
		}
	}
	
	return machine.push((VMREGTYPE)value);
}

bool FUNCGETMAXCOND::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG value = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type))
	{
		ULONG offset = offsetOfCondition(type);
		if ( offset )
			GetOffsetData(machine, temp, offset, &value);
		if ( type == 'PAEW' ) 
			value /= 65536;   // weapon uses short stored in upper half
	}
	
	return machine.push((VMREGTYPE)value);
}

bool FUNCGETCHARGE::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG ench = 0;
	ULONG value = 0;
	VMFLOAT charge = 0.0;
	
	if (GetTargetData(machine, &refr, &temp, &type))
	{
		if (!GetAttachData(machine, refr, VARNODE, 4, (ULONG*)&charge))
		{ 		//	charge = *((VMFLOAT*)(&data));
			if ( type == 'OMRA' )
				GetOffsetData(machine, temp, 0x30, &ench);
			else if ( type == 'TOLC' )
				GetOffsetData(machine, temp, 0x2d, &ench);
			else if ( type == 'PAEW' ) 
				GetOffsetData(machine, temp, 0x1d, &ench);
		
			if (ench)  // found enchantment record
				GetOffsetData(machine,(VPVOID)ench,0x10,&value);
			value %= 4;	// type of enchantment
			if ( value == 1 || value == 2) // uses charges
				GetOffsetData(machine,(VPVOID)ench,0xc,&value);
			if (value)
				charge = value;  // want float format to match current charge
		}
	}
	return machine.push((VMFLOAT)charge);
}

bool FUNCGETMAXCHARGE::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG ench = 0;
	ULONG value = 0;
	VMFLOAT maxcharge = 0.0;
	
	if (GetTargetData(machine, &refr, &temp, &type))
	{
		if ( type == 'OMRA' )
			GetOffsetData(machine, temp, 0x30, &ench);
		else if ( type == 'TOLC' )
			GetOffsetData(machine, temp, 0x2d, &ench);
		else if ( type == 'PAEW' ) 
			GetOffsetData(machine, temp, 0x1d, &ench);
		
		if (ench)  // found enchantment record
			GetOffsetData(machine,(VPVOID)ench,0x10,&value);
		value %= 4;	// type of enchantment
		if ( value == 1 || value == 2 ) // uses charges
			GetOffsetData(machine,(VPVOID)ench,0xc,&value);
		if (value)
			maxcharge = value;  // want float format to match current charge
	}
	return machine.push((VMFLOAT)maxcharge);
}

bool FUNCGETQUALITY::execute(void)
{
	VPVOID refr, temp;
	ULONG type;
	ULONG offset;
	ULONG count = 0;
	VMFLOAT quality = 0.0;
	
	if (GetTargetData(machine, &refr, &temp, &type))
	{
		offset = offsetOfQuality(type);
		if (offset)
			GetOffsetData(machine,temp,offset,(ULONG*)&quality);
	}
	return machine.push((VMFLOAT)quality);
}

bool FUNCGETNAME::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	VPVOID addr = 0;
	ULONG str = 0;
	char namestr[129];
	namestr[128] = 0;
	if (GetTargetData(machine, &refr, &temp, &type, &base))
	{
		if (type == '_CPN' || type == 'AERC')
			GetOffsetData(machine,base?base:temp,0x1c,(ULONG*)&addr);
		else if (type == 'TNOC')
			GetOffsetData(machine,temp,0x1b,(ULONG*)&addr);
		else if (type == 'HGIL')
			GetOffsetData(machine,temp,0x12,(ULONG*)&addr);
		else if (type == 'TOLC' || type == 'OMRA' || type == 'PAEW' 
			|| type == 'CSIM' || type == 'KOOB' || type == 'HCLA')
			GetOffsetData(machine,temp,0x11,(ULONG*)&addr);
		else if (type == 'ITCA')
			GetOffsetData(machine,temp,0xe,(ULONG*)&addr);
		else if (type == 'ROOD')
			addr = LONGOFFSET(temp,0x0d);
		else if (type == 'APPA')
			addr = LONGOFFSET(temp,0x19);
		else if (type == 'RGNI' || type == 'APER' || type == 'BORP' || type == 'KCIP')
			addr = LONGOFFSET(temp,0x11);

		if (addr && machine.ReadMem((VPVOID)addr, namestr, 128))
				str = (ULONG)strings.add((const char*)namestr);
	}
		
	return machine.push((VMREGTYPE)str);
}

//Tp21 22-08-2006: xSetName
//TODO xSetName
bool FUNCSETNAME::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	VPVOID addr = 0;
	ULONG str = 0;

	VMREGTYPE newname = 0;
	const char* string= "null";
	

	if (GetTargetData(machine, &refr, &temp, &type, &base))
	{
		if(machine.pop(newname) && (string=machine.GetString((VPVOID)newname))!=0)
		{
			//if the type is npc_ or crea
		if (type == '_CPN' || type == 'AERC')
			GetOffsetData(machine,base?base:temp,0x1c,(ULONG*)&addr);
		else if (type == 'TNOC')
			GetOffsetData(machine,temp,0x1b,(ULONG*)&addr);
		else if (type == 'HGIL')
			GetOffsetData(machine,temp,0x12,(ULONG*)&addr);
		else if (type == 'TOLC' || type == 'OMRA' || type == 'PAEW' 
			|| type == 'CSIM' || type == 'KOOB' || type == 'HCLA')
			GetOffsetData(machine,temp,0x11,(ULONG*)&addr);
		else if (type == 'ITCA')
			GetOffsetData(machine,temp,0xe,(ULONG*)&addr);
		else if (type == 'ROOD')
			addr = LONGOFFSET(temp,0x0d);
		else if (type == 'APPA')
			addr = LONGOFFSET(temp,0x19);
		else if (type == 'RGNI' || type == 'APER' || type == 'BORP' || type == 'KCIP')
			addr = LONGOFFSET(temp,0x11);

			//this writes the string (max char= 128!)
			//no check for maximum characters
			if ( addr )
				machine.WriteMem((VPVOID)addr, string, 128);

//			if addr is true, read the name from memory
//			if (addr && machine.ReadMem((VPVOID)addr, namestr, 128))
//					str = (ULONG)strings.add((const char*)namestr);
//					machine.WriteMem((VPVOID)addr, namestr, 128);
		}
	}

	return true;
}

bool FUNCGETBASEID::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG id = 0;
	char idstr[129];
	idstr[128] = 0;
	if (GetTargetData(machine, &refr, &temp, &type, &base))
	{
		if (base && GetIdString(machine, (VPVOID)base, idstr))
			id= (ULONG)strings.add((const char*)idstr);
		else if (temp && GetIdString(machine, (VPVOID)temp, idstr))
			id= (ULONG)strings.add((const char*)idstr);
		else
			id= (ULONG)strings.add("unknown");
	}
		
	return machine.push((VMREGTYPE)id);
}



bool FUNCGETBASESTR::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0x96, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}

bool FUNCGETBASEINT::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0x99, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}

bool FUNCGETBASEWIL::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0x9c, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}

bool FUNCGETBASEAGI::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0x9f, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}

bool FUNCGETBASESPE::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0xa2, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}

bool FUNCGETBASEEND::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0xa5, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}

bool FUNCGETBASEPER::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0xa8, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}

bool FUNCGETBASELUC::execute(void)
{
	VPVOID refr;
	VMFLOAT value = -1.0;
	
	if (GetTargetData(machine, &refr))
		GetAttachData(machine, refr, 8, 0xab, (ULONG*)&value);
	return machine.push((VMFLOAT)value);
}



bool FUNCISFEMALE::execute(void)
{
	VPVOID refr, temp;
	ULONG value = 0;
	
	if (GetTargetData(machine, &refr, &temp))
		if (GetOffsetData(machine, temp, OFFSET_NPCFLAGS, &value))
			value %= 2;	// lowest bit
	return machine.push((VMREGTYPE)value);
}

bool FUNCMYCELLID::execute(void)
{
	VPVOID refr, addr;
	ULONG data;
	ULONG id = 0;
	char idstr[129];
	idstr[128] = 0;
	if (GetTargetData(machine, &refr))
	{
		if (GetOffsetData(machine, refr, 5, &data) && data)
			if (GetOffsetData(machine, (VPVOID)data, 3, (ULONG*)&addr) && addr)
				if (GetOffsetData(machine, addr, 4, &data) && data)
					if (machine.ReadMem((VPVOID)data, (VPVOID)idstr, 128))
						id= (ULONG)strings.add((const char*)idstr);
	}
		
	return machine.push((VMREGTYPE)id);
}




bool FUNCGETBASEGOLD::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG value = 0;
	ULONG gold = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type, &base))
	{
		if (!base)
			base = temp;
		if ( type == '_CPN' )
			GetOffsetData(machine, base, OFFSET_NPCBASEGOLD, &gold);
		else if ( type == 'AERC' )
			GetOffsetData(machine, base, OFFSET_CREBASEGOLD, &gold);
		gold %= 65536; // it's a short here, so lower half only
	}
	return machine.push((VMREGTYPE)gold);
}

bool FUNCGETGOLD::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG value = 0;
	ULONG gold = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type, &base))
	{
		if (!GetAttachData(machine, refr, 8, OFFSET_GOLD, &gold))
		{
			if (!base)
				base = temp;
			if ( type == '_CPN' )
				GetOffsetData(machine, base, OFFSET_NPCBASEGOLD, &gold);
			else if ( type == 'AERC' )
				GetOffsetData(machine, base, OFFSET_CREBASEGOLD, &gold);
			gold %= 65536; // it's a short here, so lower half only
		}
	}
	return machine.push((VMREGTYPE)gold);
}

bool FUNCSETBASEGOLD::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG value = 0;
	VMREGTYPE gold = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type, &base))
	{
        	if (machine.pop(gold))
		{
			if (!base)
				base = temp;
			if ( type == '_CPN' )
				if (GetOffsetData(machine, base, OFFSET_NPCBASEGOLD, &value))
				{
					value /= 65536;
					gold %= 65536;  // gold is hold the lower half here
					gold = gold + value * 65536;
					SetOffsetData(machine, base, OFFSET_NPCBASEGOLD, gold);
				}
			else if ( type == 'AERC' )
				if (GetOffsetData(machine, base, OFFSET_CREBASEGOLD, &value))
				{
					value /= 65536;
					gold %= 65536;  // gold is hold the lower half here
					gold = gold + value * 65536;
					SetOffsetData(machine, base, OFFSET_CREBASEGOLD, gold);
				}
		}
	}
	return machine.push((VMREGTYPE)gold);
}

bool FUNCSETGOLD::execute(void)
{
	bool result = false;
	VPVOID refr;
	ULONG value = 0;
	VMREGTYPE gold = 0;
	
	if (GetTargetData(machine, &refr))
	{
        	if (machine.pop(gold))
			result = SetAttachData(machine, refr, 8, OFFSET_GOLD, (ULONG)gold);
	}
	return result;
}



bool FUNCISTRADER::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG flags = 0;
	ULONG cflags = 0;
	ULONG value = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type, &base) && type == '_CPN')
	{
		if (!base || !GetOffsetData(machine, base, 1, &value) || value != '_CPN')
			base = temp;  // in case npc isn't currently loaded
		
		GetOffsetData(machine,base,OFFSET_NPCSERVICES,&flags);
		flags &= 0x000037FF;
	
		if (GetOffsetData(machine,base,OFFSET_NPCCLASS,(ULONG*)&temp) && temp) // class info
		{
			if (GetOffsetData(machine,temp,1,&value) && value == 'SALC') 
			{
				GetOffsetData(machine,temp,OFFSET_CLASSSERVICES,&cflags);
				cflags &= 0x000037FF;
			}
		}
		
		flags |= cflags; 
	}
	
	return machine.push((VMREGTYPE)flags);
}

bool FUNCISTRAINER::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG flags = 0;
	ULONG cflags = 0;
	ULONG value = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type, &base) && type == '_CPN')
	{
		if (!base || !GetOffsetData(machine, base, 1, &value) || value != '_CPN')
			base = temp;  // in case npc isn't currently loaded
		
		GetOffsetData(machine,base,OFFSET_NPCSERVICES,&flags);
		flags &= 0x00004000;
	
		if (GetOffsetData(machine,base,OFFSET_NPCCLASS,(ULONG*)&temp) && temp) // class info
		{
			if (GetOffsetData(machine,temp,1,&value) && value == 'SALC') 
			{
				GetOffsetData(machine,temp,OFFSET_CLASSSERVICES,&cflags);
				cflags &= 0x00004000;
			}
		}
		if ( flags | cflags ) 
			flags = 1;
	}
	return machine.push((VMREGTYPE)flags);
}

bool FUNCISPROVIDER::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG flags = 0;
	ULONG cflags = 0;
	ULONG value = 0;
	
	if (GetTargetData(machine, &refr, &temp, &type, &base) && type == '_CPN')
	{
		if (!base || !GetOffsetData(machine, base, 1, &value) || value != '_CPN')
			base = temp;  // in case npc isn't currently loaded
		
		GetOffsetData(machine,base,OFFSET_NPCSERVICES,&flags);
		flags &= 0x00038800;
	
		if (GetOffsetData(machine,base,OFFSET_NPCCLASS,(ULONG*)&temp) && temp) // class info
		{
			if (GetOffsetData(machine,temp,1,&value) && value == 'SALC') 
			{
				GetOffsetData(machine,temp,OFFSET_CLASSSERVICES,&cflags);
				cflags &= 0x00038800;
			}
		}
		flags |= cflags;
	}
	return machine.push((VMREGTYPE)flags);
}



// 2005-07-12  CDC  More powerful alternatives to the IsTrader/Trainer/Provider functions
bool FUNCGETSERVICE::execute(void)
{
	VPVOID refr, temp, base;
	ULONG type;
	ULONG flags = 0;
	ULONG cflags = 0;
	ULONG mask = 0x0003FFFF;
	
	if (machine.pop((VMREGTYPE&)mask)) 
		mask &= 0x0003FFFF;
	if (GetTargetData(machine, &refr, &temp, &type, &base) && type == '_CPN')
	{
		if (!base || !GetOffsetData(machine, base, 1, &type) || type != '_CPN')
			base = temp;  // in case npc isn't currently loaded
		
		GetOffsetData(machine,base,OFFSET_NPCSERVICES,&flags);		// the npc data
		//if (GetOffsetData(machine,base,OFFSET_NPCCLASS,(ULONG*)&temp) 	// class info if available?
		//	&& temp && GetOffsetData(machine,temp,1,&type) && type == 'SALC')
		//	GetOffsetData(machine,temp,OFFSET_CLASSSERVICES,&cflags);
		flags |= cflags; 
		flags &= mask;
	}
	return machine.push((VMREGTYPE)flags);
}

bool FUNCSETSERVICE::execute(void)
{
	VPVOID refr, temp, base;
	VMREGTYPE data;
	ULONG type;
	ULONG flags = 0;
	ULONG cflags = 0;
	
	if (!machine.pop(data)) return false;

	flags = data & 0x0003FFFF;
	if (GetTargetData(machine, &refr, &temp, &type, &base) && type == '_CPN')
	{
		if (!base || !GetOffsetData(machine, base, 1, &type) || type != '_CPN')
			base = temp;  // in case npc isn't currently loaded
		
		return SetOffsetData(machine,base,OFFSET_NPCSERVICES,flags);
	}
	return false;
}

bool FUNCMODSERVICE::execute(void)
{
	VPVOID refr, temp, base;
	VMREGTYPE data;
	ULONG type;
	ULONG flags = 0;
	ULONG cflags = 0;
	
	if (!machine.pop(data)) return false;
	if (GetTargetData(machine, &refr, &temp, &type, &base) && type == '_CPN')
	{
		if (!base || !GetOffsetData(machine, base, 1, &type) || type != '_CPN')
			base = temp;  // in case npc isn't currently loaded
		
		GetOffsetData(machine,base,OFFSET_NPCSERVICES,&flags);		// the npc data
		if (GetOffsetData(machine,base,OFFSET_NPCCLASS,(ULONG*)&temp) 	// class info if available?
			&& temp && GetOffsetData(machine,temp,1,&type) && type == 'SALC')
			GetOffsetData(machine,temp,OFFSET_CLASSSERVICES,&cflags);
		flags |= cflags; 

	if ( ((signed long)data) < 0 )	// Want to remove services
		flags = flags & (~((ULONG)(0-(signed long)data)));
	else				// Want to add services
		flags = flags | (data & 0x0003FFFF);
		return SetOffsetData(machine,base,OFFSET_NPCSERVICES,flags);
	}
	return false;
}
