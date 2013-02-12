//Tes3Machine.cpp

#include "TES3MACHINE.h"
#include "TES3MEMMAP.h"
#include "BUFSPACE.h"
#include "PROCESSMEM.h"

#include <algorithm>

#include "TES3OPCODES.h"

#include "INSTCALL.h"
#include "INSTCOPY.h"
#include "INSTFLOAT.h"
#include "INSTINT.h"
#include "INSTJUMP.h"
#include "INSTNOP.h"
#include "INSTPOP.h"
#include "INSTPUSH.h"
#include "FUNCVARIABLES.h"
#include "FUNCAI.h"
#include "FUNCPOSITION.h"
#include "FUNCREFERENCE.h"
#include "FUNCPROPERTIES.h"
#include "FUNCTEXT.h"
#include "FUNCLOG.h"
#include "FUNCFILE.h"
#include "FUNCINVENTORY.h"
#include "FUNCEXTENDER.h"
#include "FUNCMGE.h"
#include "../mge/Configuration.h"


#define PLACEHOLDER 0

#define SUPER VIRTUALMACHINE

TES3MACHINE::TES3MACHINE() 
: SUPER()
	,scriptaddr(0)
	,script()
	,flow()
	,instructionpointer(0)
	,stackpointer(0)
	,executable(new BUFSPACE(SCRIPTMEM_SIZE))
{
	if(!AddAddressSpace(SCRIPTMEM_VPOS,executable))
		LOG::log("TES3MACHINE: AddAddressSpace SCRIPTMEM failed\n");
	if(!AddAddressSpace(STACKMEM_VPOS, new BUFSPACE(STACKMEM_SIZE)))
		LOG::log("TES3MACHINE: AddAddressSpace STACKMEM failed\n");
	if(!AddAddressSpace(GENERALMEM_VPOS, new PROCESSMEM(GENERALMEM_VPOS,GENERALMEM_SIZE)))
		LOG::log("TES3MACHINE: AddAddressSpace GENERALMEM failed\n");

	AddInstruction(CALL,new INSTCALL(*this));
	AddInstruction(CALLSHORT,new INSTCALLSHORT(*this));
	AddInstruction(RETURN,new INSTRETURN(*this));
	AddInstruction(RETURNP,new INSTRETURNP(*this));
	AddInstruction(RETURNVP,new INSTRETURNVP(*this));

	AddInstruction(COPYREG,new INSTCOPYREG(*this));
	AddInstruction(COPYFROMSTACK,new INSTCOPYFROMSTACK(*this));
	AddInstruction(COPYTOSTACK,new INSTCOPYTOSTACK(*this));

	AddInstruction(JUMP,new INSTJUMP(*this));
	AddInstruction(JUMPSHORT,new INSTJUMPSHORT(*this));
	AddInstruction(JUMPZERO,new INSTJUMPZERO(*this));
	AddInstruction(JUMPSHORTZERO,new INSTJUMPSHORTZERO(*this));
	AddInstruction(JUMPNOTZERO,new INSTJUMPNOTZERO(*this));
	AddInstruction(JUMPSHORTNOTZERO,new INSTJUMPSHORTNOTZERO(*this));
	AddInstruction(JUMPPOSITIVE,new INSTJUMPPOSITIVE(*this));
	AddInstruction(JUMPSHORTPOSITIVE,new INSTJUMPSHORTPOSITIVE(*this));
	AddInstruction(JUMPNEGATIVE,new INSTJUMPNEGATIVE(*this));
	AddInstruction(JUMPSHORTNEGATIVE,new INSTJUMPSHORTNEGATIVE(*this));

	AddInstruction(POP,new INSTPOP(*this));
	AddInstruction(POPREG,new INSTPOPREG(*this));

	AddInstruction(PUSH,new INSTPUSH(*this));
	AddInstruction(PUSHB,new INSTPUSHB(*this));
	AddInstruction(PUSHS,new INSTPUSHS(*this));
	AddInstruction(PUSHREG,new INSTPUSHREG(*this));
	AddInstruction(DECLARELOCAL,new INSTDECLARELOCAL(*this));

	AddInstruction(ADD,new INSTADD(*this));
	AddInstruction(SUB,new INSTSUB(*this));
	AddInstruction(MUL,new INSTMUL(*this));
	AddInstruction(DIV,new INSTDIV(*this));
	AddInstruction(MOD,new INSTMOD(*this));

	AddInstruction(ITOF,new INSTITOF(*this));
	AddInstruction(FTOI,new INSTFTOI(*this));
	AddInstruction(FADD,new INSTFADD(*this));
	AddInstruction(FSUB,new INSTFSUB(*this));
	AddInstruction(FMUL,new INSTFMUL(*this));
	AddInstruction(FDIV,new INSTFDIV(*this));

	AddInstruction(TAN,new INSTTAN(*this));
	AddInstruction(SIN,new INSTSIN(*this));
	AddInstruction(COS,new INSTCOS(*this));
	AddInstruction(ARCTAN,new INSTARCTAN(*this));
	AddInstruction(ARCSIN,new INSTARCSIN(*this));
	AddInstruction(ARCCOS,new INSTARCCOS(*this));
	AddInstruction(DEGRAD,new INSTDEGRAD(*this));
	AddInstruction(RADDEG,new INSTRADDEG(*this));
	AddInstruction(SQRT,new INSTSQRT(*this));
	AddInstruction(HYPOT,new INSTHYPOT(*this));


	AddInstruction(GETLOCAL,new FUNCGETLOCAL(*this));
	AddInstruction(GETFOREIGN,new FUNCGETLOCAL(*this));
	AddInstruction(SETLOCAL,new FUNCSETLOCAL(*this));
	AddInstruction(SETFOREIGN,new FUNCSETLOCAL(*this));
	
	AddInstruction(AITRAVEL,new FUNCAITRAVEL(*this));
	AddInstruction(POSITION,new FUNCPOSITION(*this));
	
	AddInstruction(REFPCTARGET,new FUNCREFPCTARGET(*this));
	AddInstruction(GETPCTARGET,new FUNCGETPCTARGET(*this));
	AddInstruction(REFTYPE,new FUNCREFTYPE(*this));
	
	AddInstruction(LOGMSG,new FUNCLOG(*this));
	
	AddInstruction(FILEREWIND,new FUNCFILEREWIND(*this));
	AddInstruction(FILEREADSHORT,new FUNCFILEREADTWOBYTES(*this));
	AddInstruction(FILEREADLONG, new FUNCFILEREADFOURBYTES(*this));
	AddInstruction(FILEREADFLOAT, new FUNCFILEREADFOURBYTES(*this));
	AddInstruction(FILEREADSTRING, new FUNCFILEREADSTRING(*this));
	AddInstruction(FILESEEK, new FUNCFILESEEK(*this));

	AddInstruction(SETREF,new FUNCREF(*this));
	AddInstruction(FIRSTNPC,new FUNCFIRSTNPC(*this));
	AddInstruction(NEXTREF,new FUNCNEXTREF(*this));
	AddInstruction(REFID,new FUNCREFID(*this));
	AddInstruction(GETREF,new FUNCGETREF(*this));
	AddInstruction(FIRSTITEM,new FUNCFIRSTITEM(*this));
	AddInstruction(FIRSTSTATIC,new FUNCFIRSTSTATIC(*this));
	
	AddInstruction(GETCOMBAT,new FUNCGETCOMBAT(*this));
	AddInstruction(STARTCOMBAT,new FUNCSTARTCOMBAT(*this));
	AddInstruction(DISTANCE,new FUNCDISTANCE(*this));

	AddInstruction(ADDITEM,new FUNCADDITEM(*this));
	AddInstruction(REMOVEITEM,new FUNCREMOVEITEM(*this));
	AddInstruction(INVENTORY,new FUNCINVENTORY(*this));
	AddInstruction(NEXTSTACK,new FUNCNEXTSTACK(*this));
	AddInstruction(POSITIONCELL,new FUNCPOSITIONCELL(*this));
	AddInstruction(PCCELLID,new FUNCPCCELLID(*this));
	
	AddInstruction(PLACE, new FUNCPLACEAT(*this));
	AddInstruction(STRCMP, new FUNCSTRCMP(*this));
	AddInstruction(HASEQUIPED, new FUNCHASEQUIPED(*this));

	AddInstruction(FILEWRITESHORT,new FUNCFILEWRITETWOBYTES(*this));
	AddInstruction(FILEWRITELONG, new FUNCFILEWRITEFOURBYTES(*this));
	AddInstruction(FILEWRITEFLOAT, new FUNCFILEWRITEFOURBYTES(*this));
	AddInstruction(FILEWRITESTRING, new FUNCFILEWRITESTRING(*this));

	// 2005-02-14  CDC
	AddInstruction(GETVALUE, new FUNCGETVALUE(*this));
	AddInstruction(GETOWNER, new FUNCGETOWNER(*this));
	AddInstruction(GETWEIGHT, new FUNCGETWEIGHT(*this));
	AddInstruction(GETENCUMB, new FUNCGETENCUMB(*this));
	AddInstruction(GETMAXCOND, new FUNCGETMAXCOND(*this));
	AddInstruction(GETCONDITION, new FUNCGETCONDITION(*this));
	AddInstruction(GETMAXCHARGE, new FUNCGETMAXCHARGE(*this));
	AddInstruction(GETCHARGE, new FUNCGETCHARGE(*this));
	AddInstruction(GETQUALITY, new FUNCGETQUALITY(*this));

	AddInstruction(GETNAME, new FUNCGETNAME(*this));
	AddInstruction(GETBASEID, new FUNCGETBASEID(*this));

	AddInstruction(GETBASESTR, new FUNCGETBASESTR(*this));
	AddInstruction(GETBASEINT, new FUNCGETBASEINT(*this));
	AddInstruction(GETBASEWIL, new FUNCGETBASEWIL(*this));
	AddInstruction(GETBASEAGI, new FUNCGETBASEAGI(*this));
	AddInstruction(GETBASESPE, new FUNCGETBASESPE(*this));
	AddInstruction(GETBASEEND, new FUNCGETBASEEND(*this));
	AddInstruction(GETBASEPER, new FUNCGETBASEPER(*this));
	AddInstruction(GETBASELUC, new FUNCGETBASELUC(*this));

	AddInstruction(ISFEMALE, new FUNCISFEMALE(*this));
	AddInstruction(MYCELLID, new FUNCMYCELLID(*this));

	AddInstruction(GETBASEGOLD, new FUNCGETBASEGOLD(*this));
	AddInstruction(GETGOLD, new FUNCGETGOLD(*this));

	AddInstruction(SETBASEGOLD, new FUNCSETBASEGOLD(*this));
	AddInstruction(SETGOLD, new FUNCSETGOLD(*this));

	AddInstruction(ISTRADER, new FUNCISTRADER(*this));
	AddInstruction(ISTRAINER, new FUNCISTRAINER(*this));
	AddInstruction(ISPROVIDER, new FUNCISPROVIDER(*this));

	AddInstruction(MESSAGEFIX, new FUNCMESSAGEFIX(*this));


	// 2005-07-10  CDC  0.9.2 functions
	AddInstruction(KEYPRESSED, new FUNCKEYPRESSED(*this));	
	AddInstruction(TEXTINPUT, new FUNCTEXTINPUT(*this));
	AddInstruction(TEXTINPUTALT, new FUNCTEXTINPUTALT(*this));
	AddInstruction(CONTENTLIST, new FUNCCONTENTLIST(*this));

	AddInstruction(GETSERVICE, new FUNCGETSERVICE(*this));
	AddInstruction(SETSERVICE, new FUNCSETSERVICE(*this));
	AddInstruction(MODSERVICE, new FUNCMODSERVICE(*this));

	AddInstruction(FILEREADTEXT, new FUNCFILEREADTEXT(*this));
	AddInstruction(FILEWRITETEXT, new FUNCFILEWRITETEXT(*this));
	AddInstruction(STRINGLENGTH, new FUNCSTRINGLENGTH(*this));
	AddInstruction(STRINGBUILD, new FUNCSTRINGBUILD(*this));
	AddInstruction(STRINGPARSE, new FUNCSTRINGPARSE(*this));

	AddInstruction(MEMLOOK, new FUNCMEMLOOK(*this));

	//Fliggerty 12-27-06
	AddInstruction(ADDSPELL, new FUNCADDSPELL(*this));
	AddInstruction(REMOVESPELL, new FUNCREMOVESPELL(*this));

	//Tp21 22-08-2006
	AddInstruction(DROPITEM, new FUNCDROPITEM(*this));
	AddInstruction(EQUIPITEM, new FUNCEQUIPITEM(*this));
	AddInstruction(SETNAME, new FUNCSETNAME(*this));
	AddInstruction(GETSPELLEFFECTS, new FUNCGETSPELLEFFECTS(*this));
	AddInstruction(CAST, new FUNCCAST(*this));

#define ADDINSTRUCTION(name) AddInstruction(name, new FUNC##name(*this))
	ADDINSTRUCTION(SETVALUE);
	ADDINSTRUCTION(SETWEIGHT);
	ADDINSTRUCTION(SETQUALITY);
	ADDINSTRUCTION(SETCONDITION);
	ADDINSTRUCTION(SETCHARGE);
	ADDINSTRUCTION(SETMAXCHARGE);
    ADDINSTRUCTION(SETMAXCONDITION);
	ADDINSTRUCTION(STRINGMATCH);

	if (!(MGEFlags & MGE_DISABLED)) {
	//Timeslip 2007-19-01 : MGE functions (Autogenerated code)
	AddInstruction(WIPEALL, new FUNCWIPEALL(*this));
	AddInstruction(OUTPUTDEBUGSTRING, new FUNCOUTPUTDEBUGSTRING(*this));
	AddInstruction(OUTPUTVERSIONSTRING, new FUNCOUTPUTVERSIONSTRING(*this));
	AddInstruction(GETVERSION, new FUNCGETVERSION(*this));
	AddInstruction(GETWIDTH, new FUNCGETWIDTH(*this));
	AddInstruction(GETHEIGHT, new FUNCGETHEIGHT(*this));
	AddInstruction(GETFPS, new FUNCGETFPS(*this));
	AddInstruction(GETEYEVEC, new FUNCGETEYEVEC(*this));
	AddInstruction(ENABLEDISTANTLAND, new FUNCENABLEDISTANTLAND(*this));
	AddInstruction(DISABLEDISTANTLAND, new FUNCDISABLEDISTANTLAND(*this));
	AddInstruction(WIPEHUDELEMENTS, new FUNCWIPEHUDELEMENTS(*this));
	AddInstruction(WITHHUD, new FUNCWITHHUD(*this));
	AddInstruction(CANCELWITHHUD, new FUNCCANCELWITHHUD(*this));
	AddInstruction(LOADHUD, new FUNCLOADHUD(*this));
	AddInstruction(POSITIONHUD, new FUNCPOSITIONHUD(*this));
	AddInstruction(SCALEHUD, new FUNCSCALEHUD(*this));
	AddInstruction(FULLSCREENHUD, new FUNCFULLSCREENHUD(*this));
	AddInstruction(ENABLEHUD, new FUNCENABLEHUD(*this));
	AddInstruction(DISABLEHUD, new FUNCDISABLEHUD(*this));
	AddInstruction(CHANGEHUDTEXTURE, new FUNCCHANGEHUDTEXTURE(*this));
	AddInstruction(CHANGEHUDEFFECT, new FUNCCHANGEHUDEFFECT(*this));
	AddInstruction(FREEHUD, new FUNCFREEHUD(*this));
	AddInstruction(NIDLOADHUD, new FUNCNIDLOADHUD(*this));
	AddInstruction(NIDPOSITIONHUD, new FUNCNIDPOSITIONHUD(*this));
	AddInstruction(NIDSCALEHUD, new FUNCNIDSCALEHUD(*this));
	AddInstruction(NIDFULLSCREENHUD, new FUNCNIDFULLSCREENHUD(*this));
	AddInstruction(NIDENABLEHUD, new FUNCNIDENABLEHUD(*this));
	AddInstruction(NIDDISABLEHUD, new FUNCNIDDISABLEHUD(*this));
	AddInstruction(NIDCHANGEHUDTEXTURE, new FUNCNIDCHANGEHUDTEXTURE(*this));
	AddInstruction(NIDCHANGEHUDEFFECT, new FUNCNIDCHANGEHUDEFFECT(*this));
	AddInstruction(NIDFREEHUD, new FUNCNIDFREEHUD(*this));
	AddInstruction(MANUALLIGHTING, new FUNCMANUALLIGHTING(*this));
	AddInstruction(ENDMANUALLIGHTING, new FUNCENDMANUALLIGHTING(*this));
	AddInstruction(SETSUNCOLOUR, new FUNCSETSUNCOLOUR(*this));
	AddInstruction(DEFAULTSUNCOLOUR, new FUNCDEFAULTSUNCOLOUR(*this));
	AddInstruction(SETSUNDIRECTION, new FUNCSETSUNDIRECTION(*this));
	AddInstruction(DEFAULTSUNDIRECTION, new FUNCDEFAULTSUNDIRECTION(*this));
	AddInstruction(SETSUNTEXTURE, new FUNCSETSUNTEXTURE(*this));
	AddInstruction(DEFAULTSUNTEXTURE, new FUNCDEFAULTSUNTEXTURE(*this));
	AddInstruction(GETSUNCOLOUR, new FUNCGETSUNCOLOUR(*this));
	AddInstruction(GETSUNDIR, new FUNCGETSUNDIR(*this));
	AddInstruction(SETAMBIENTCOLOUR, new FUNCSETAMBIENTCOLOUR(*this));
	AddInstruction(DEFAULTAMBIENTCOLOUR, new FUNCDEFAULTAMBIENTCOLOUR(*this));
	AddInstruction(GETAMBIENTCOLOUR, new FUNCGETAMBIENTCOLOUR(*this));
	AddInstruction(SETCLSCOLOR, new FUNCSETCLSCOLOR(*this));
	AddInstruction(DEFAULTCLSCOLOUR, new FUNCDEFAULTCLSCOLOUR(*this));
	AddInstruction(GETCLSCOLOUR, new FUNCGETCLSCOLOUR(*this));
	AddInstruction(SETSKYCOLOUR, new FUNCSETSKYCOLOUR(*this));
	AddInstruction(DEFAULTSKYCOLOUR, new FUNCDEFAULTSKYCOLOUR(*this));
	AddInstruction(SETHUDCOLOUR, new FUNCSETHUDCOLOUR(*this));
	AddInstruction(DEFAULTHUDCOLOUR, new FUNCDEFAULTHUDCOLOUR(*this));
	AddInstruction(TAPKEY, new FUNCTAPKEY(*this));
	AddInstruction(PUSHKEY, new FUNCPUSHKEY(*this));
	AddInstruction(RELESEKEY, new FUNCRELESEKEY(*this));
	AddInstruction(HAMMERKEY, new FUNCHAMMERKEY(*this));
	AddInstruction(UNHAMMERKEY, new FUNCUNHAMMERKEY(*this));
	AddInstruction(AHAMMERKEY, new FUNCAHAMMERKEY(*this));
	AddInstruction(AUNHAMMERKEY, new FUNCAUNHAMMERKEY(*this));
	AddInstruction(DISALLOWKEY, new FUNCDISALLOWKEY(*this));
	AddInstruction(ALLOWKEY, new FUNCALLOWKEY(*this));
	AddInstruction(MOVEMOUSERIGHT, new FUNCMOVEMOUSERIGHT(*this));
	AddInstruction(MOVEMOUSELEFT, new FUNCMOVEMOUSELEFT(*this));
	AddInstruction(MOVEMOUSEUP, new FUNCMOVEMOUSEUP(*this));
	AddInstruction(MOVEMOUSEDOWN, new FUNCMOVEMOUSEDOWN(*this));
	AddInstruction(SETMOUSESPEEDRIGHT, new FUNCSETMOUSESPEEDRIGHT(*this));
	AddInstruction(SETMOUSESPEEDLEFT, new FUNCSETMOUSESPEEDLEFT(*this));
	AddInstruction(SETMOUSESPEEDUP, new FUNCSETMOUSESPEEDUP(*this));
	AddInstruction(SETMOUSESPEEDDOWN, new FUNCSETMOUSESPEEDDOWN(*this));
	AddInstruction(DISABLEMOUSE, new FUNCDISABLEMOUSE(*this));
	AddInstruction(ENABLEMOUSE, new FUNCENABLEMOUSE(*this));
	AddInstruction(BUFFEREDKEYDOWN, new FUNCBUFFEREDKEYDOWN(*this));
	AddInstruction(BUFFEREDKEYUP, new FUNCBUFFEREDKEYUP(*this));
	AddInstruction(ENABLEFOG, new FUNCENABLEFOG(*this));
	AddInstruction(DISABLEFOG, new FUNCDISABLEFOG(*this));
	AddInstruction(TOGGLEFOG, new FUNCTOGGLEFOG(*this));
	AddInstruction(INCREASEFOG, new FUNCINCREASEFOG(*this));
	AddInstruction(DECREASEFOG, new FUNCDECREASEFOG(*this));
	AddInstruction(INCREASEFOGBY, new FUNCINCREASEFOGBY(*this));
	AddInstruction(DECREASEFOGBY, new FUNCDECREASEFOGBY(*this));
	AddInstruction(SETFOG, new FUNCSETFOG(*this));
	AddInstruction(FOGZOOM, new FUNCFOGZOOM(*this));
	AddInstruction(STOPFOGZOOM, new FUNCSTOPFOGZOOM(*this));
	AddInstruction(GETFOG, new FUNCGETFOG(*this));
	AddInstruction(SETFOGCOLOUR, new FUNCSETFOGCOLOUR(*this));
	AddInstruction(DEFAULTFOGCOLOUR, new FUNCDEFAULTFOGCOLOUR(*this));
	AddInstruction(GETFOGCOLOUR, new FUNCGETFOGCOLOUR(*this));
	AddInstruction(ENABLECAMERASHAKE, new FUNCENABLECAMERASHAKE(*this));
	AddInstruction(DISABLECAMERASHAKE, new FUNCDISABLECAMERASHAKE(*this));
	AddInstruction(SETCAMERASHAKEMAGNITUDE, new FUNCSETCAMERASHAKEMAGNITUDE(*this));
	AddInstruction(CAMERASHAKEZOOM, new FUNCCAMERASHAKEZOOM(*this));
	AddInstruction(STOPCAMERASHAKEZOOM, new FUNCSTOPCAMERASHAKEZOOM(*this));
	AddInstruction(TAKESCREENSHOT, new FUNCTAKESCREENSHOT(*this));
	AddInstruction(TAKEBMPSCREENSHOT, new FUNCTAKEBMPSCREENSHOT(*this));
	AddInstruction(TAKEJPEGSCREENSHOT, new FUNCTAKEJPEGSCREENSHOT(*this));
	AddInstruction(TAKEDDSSCREENSHOT, new FUNCTAKEDDSSCREENSHOT(*this));
	AddInstruction(TAKEPNGSCREENSHOT, new FUNCTAKEPNGSCREENSHOT(*this));
	AddInstruction(ENABLEFPSCOUNTER, new FUNCENABLEFPSCOUNTER(*this));
	AddInstruction(DISABLEFPSCOUNTER, new FUNCDISABLEFPSCOUNTER(*this));
	AddInstruction(TOGGLEFPSCOUNTER, new FUNCTOGGLEFPSCOUNTER(*this));
	AddInstruction(ROTATESCREEN, new FUNCROTATESCREEN(*this));
	AddInstruction(SETSCREENROT0, new FUNCSETSCREENROT0(*this));
	AddInstruction(SETSCREENROT90, new FUNCSETSCREENROT90(*this));
	AddInstruction(SETSCREENROT180, new FUNCSETSCREENROT180(*this));
	AddInstruction(SETSCREENROT270, new FUNCSETSCREENROT270(*this));
	AddInstruction(ROTATESCREENBY, new FUNCROTATESCREENBY(*this));
	AddInstruction(SETSCREENROTATION, new FUNCSETSCREENROTATION(*this));
	AddInstruction(SCREENSPIN, new FUNCSCREENSPIN(*this));
	AddInstruction(STOPSPINSPIN, new FUNCSTOPSPINSPIN(*this));
	AddInstruction(GETSCREENROTATION, new FUNCGETSCREENROTATION(*this));
	AddInstruction(ENABLEZOOM, new FUNCENABLEZOOM(*this));
	AddInstruction(DISABLEZOOM, new FUNCDISABLEZOOM(*this));
	AddInstruction(TOGGLEZOOM, new FUNCTOGGLEZOOM(*this));
	AddInstruction(ZOOMIN, new FUNCZOOMIN(*this));
	AddInstruction(ZOOMOUT, new FUNCZOOMOUT(*this));
	AddInstruction(ZOOMINBY, new FUNCZOOMINBY(*this));
	AddInstruction(ZOOMOUTBY, new FUNCZOOMOUTBY(*this));
	AddInstruction(SETZOOM, new FUNCSETZOOM(*this));
	AddInstruction(ZOOM, new FUNCZOOM(*this));
	AddInstruction(STOPZOOM, new FUNCSTOPZOOM(*this));
	AddInstruction(GETZOOM, new FUNCGETZOOM(*this));
	AddInstruction(LOADTEXTURE, new FUNCLOADTEXTURE(*this));
	AddInstruction(FREETEXTURE, new FUNCFREETEXTURE(*this));
	AddInstruction(WIPETEXTURES, new FUNCWIPETEXTURES(*this));
	AddInstruction(BEGINTEXDRAW, new FUNCBEGINTEXDRAW(*this));
	AddInstruction(ENDTEXDRAW, new FUNCENDTEXDRAW(*this));
	AddInstruction(SETTEXDRAWORIGIN, new FUNCSETTEXDRAWORIGIN(*this));
	AddInstruction(SETTEXDRAWSCALE, new FUNCSETTEXDRAWSCALE(*this));
	AddInstruction(SETTEXDRAWCOLOUR, new FUNCSETTEXDRAWCOLOUR(*this));
	AddInstruction(DRAWTEXTTOTEXTURE, new FUNCDRAWTEXTTOTEXTURE(*this));
	AddInstruction(DRAWTEXTURETOTEXTURE, new FUNCDRAWTEXTURETOTEXTURE(*this));
	AddInstruction(AUTOFREEHOOK, new FUNCAUTOFREEHOOK(*this));
	AddInstruction(DONTFREEHOOK, new FUNCDONTFREEHOOK(*this));
	AddInstruction(RESUMEVIDEO, new FUNCRESUMEVIDEO(*this));
	AddInstruction(PAUSEVIDEO, new FUNCPAUSEVIDEO(*this));
	AddInstruction(STOPVIDEO, new FUNCSTOPVIDEO(*this));
	AddInstruction(REWINDVIDEO, new FUNCREWINDVIDEO(*this));
	AddInstruction(SEEKVIDEO, new FUNCSEEKVIDEO(*this));
	AddInstruction(ENABLEFULLSCREENSHADER, new FUNCENABLEFULLSCREENSHADER(*this));
	AddInstruction(DISABLEFULLSCREENSHADERS, new FUNCDISABLEFULLSCREENSHADERS(*this));
	AddInstruction(TOGGLEFULLSCREENSHADERS, new FUNCTOGGLEFULLSCREENSHADERS(*this));
	AddInstruction(LOADEFFECT, new FUNCLOADEFFECT(*this));
	AddInstruction(SETEFFECTTECHNIQUE, new FUNCSETEFFECTTECHNIQUE(*this));
	AddInstruction(USEEFFECTASFULLSCREENSHADER, new FUNCUSEEFFECTASFULLSCREENSHADER(*this));
	AddInstruction(DISABLEEFFECTASFULLSCREENSHADER, new FUNCDISABLEEFFECTASFULLSCREENSHADER(*this));
	AddInstruction(FREEEFFECT, new FUNCFREEEFFECT(*this));
	AddInstruction(SETEFFECTVARLONG, new FUNCSETEFFECTVARLONG(*this));
	AddInstruction(SETEFFECTVARFLOAT, new FUNCSETEFFECTVARFLOAT(*this));
	AddInstruction(SETEFFECTVARVECTOR, new FUNCSETEFFECTVARVECTOR(*this));
	AddInstruction(SETEFFECTVARTEXTURE, new FUNCSETEFFECTVARTEXTURE(*this));
	AddInstruction(WIPELOADEDEFFECTS, new FUNCWIPELOADEDEFFECTS(*this));
	AddInstruction(WITHMESH, new FUNCWITHMESH(*this));
	AddInstruction(CANCELWITHMESH, new FUNCCANCELWITHMESH(*this));
	AddInstruction(WIPELOADEDMESHES, new FUNCWIPELOADEDMESHES(*this));
	AddInstruction(LOADMESH, new FUNCLOADMESH(*this));
	AddInstruction(SETMESHPOSITION, new FUNCSETMESHPOSITION(*this));
	AddInstruction(SETMESHROTATION, new FUNCSETMESHROTATION(*this));
	AddInstruction(SETMESHSCALE, new FUNCSETMESHSCALE(*this));
	AddInstruction(MOVEMESH, new FUNCMOVEMESH(*this));
	AddInstruction(ROTATEMESH, new FUNCROTATEMESH(*this));
	AddInstruction(SCALEMESH, new FUNCSCALEMESH(*this));
	AddInstruction(SETMESHVELOCITY, new FUNCSETMESHVELOCITY(*this));
	AddInstruction(SETMESHANGULARVELOCITY, new FUNCSETMESHANGULARVELOCITY(*this));
	AddInstruction(SETMESHGROWTH, new FUNCSETMESHGROWTH(*this));
	AddInstruction(ZEROMESHVELOCITY, new FUNCZEROMESHVELOCITY(*this));
	AddInstruction(ZEROMESHANGULARVELOCITY, new FUNCZEROMESHANGULARVELOCITY(*this));
	AddInstruction(ZEROMESHGROWTH, new FUNCZEROMESHGROWTH(*this));
	AddInstruction(ENABLEMESH, new FUNCENABLEMESH(*this));
	AddInstruction(DISABLEMESH, new FUNCDISABLEMESH(*this));
	AddInstruction(SETMESHTEXTURE, new FUNCSETMESHTEXTURE(*this));
	AddInstruction(SETMESHEFFECT, new FUNCSETMESHEFFECT(*this));
	AddInstruction(MANUALMESH, new FUNCMANUALMESH(*this));
	AddInstruction(FREEMESH, new FUNCFREEMESH(*this));
	AddInstruction(UPDATEMESHLIGHTING, new FUNCUPDATEMESHLIGHTING(*this));
	AddInstruction(NIDLOADMESH, new FUNCNIDLOADMESH(*this));
	AddInstruction(NIDSETMESHPOSITION, new FUNCNIDSETMESHPOSITION(*this));
	AddInstruction(NIDSETMESHROTATION, new FUNCNIDSETMESHROTATION(*this));
	AddInstruction(NIDSETMESHSCALE, new FUNCNIDSETMESHSCALE(*this));
	AddInstruction(NIDMOVEMESH, new FUNCNIDMOVEMESH(*this));
	AddInstruction(NIDROTATEMESH, new FUNCNIDROTATEMESH(*this));
	AddInstruction(NIDSCALEMESH, new FUNCNIDSCALEMESH(*this));
	AddInstruction(NIDSETMESHVELOCITY, new FUNCNIDSETMESHVELOCITY(*this));
	AddInstruction(NIDSETMESHANGULARVELOCITY, new FUNCNIDSETMESHANGULARVELOCITY(*this));
	AddInstruction(NIDSETMESHGROWTH, new FUNCNIDSETMESHGROWTH(*this));
	AddInstruction(NIDZEROMESHVELOCITY, new FUNCNIDZEROMESHVELOCITY(*this));
	AddInstruction(NIDZEROMESHANGULARVELOCITY, new FUNCNIDZEROMESHANGULARVELOCITY(*this));
	AddInstruction(NIDZEROMESHGROWTH, new FUNCNIDZEROMESHGROWTH(*this));
	AddInstruction(NIDENABLEMESH, new FUNCNIDENABLEMESH(*this));
	AddInstruction(NIDDISABLEMESH, new FUNCNIDDISABLEMESH(*this));
	AddInstruction(NIDSETMESHTEXTURE, new FUNCNIDSETMESHTEXTURE(*this));
	AddInstruction(NIDSETMESHEFFECT, new FUNCNIDSETMESHEFFECT(*this));
	AddInstruction(NIDMANUALMESH, new FUNCNIDMANUALMESH(*this));
	AddInstruction(NIDFREEMESH, new FUNCNIDFREEMESH(*this));
	AddInstruction(NIDUPDATEMESHLIGHTING, new FUNCNIDUPDATEMESHLIGHTING(*this));
	}
}

bool TES3MACHINE::GetRegister(WORD regidx, VMREGTYPE& value) //get the info from a processor register (EDX,EIP,...)
{
	bool result= true;
	
	switch(regidx)
	{
	case IP:
		value= instructionpointer;
		result= true;
		break;
	case SP:
		value= stackpointer;
		break;
	default:
		if(regidx<=GPMAX)
			value= generalregs[regidx-GP];
		else
			result= false;
	}
	
	return result;
}

bool TES3MACHINE::SetRegister(WORD regidx, VMREGTYPE value) //write stuff into a processor register (EDX,EIP,...)
{
	bool result= true;
	
	switch(regidx)
	{
	case IP:
		instructionpointer= value;
		result= true;
		break;
	case SP:
		//fixed error 4018 (hope it works :P)
		if(value && value < (signed)STACKMEM_MIN) // || value>STACKMEM_MAX) // how do I test the upper limit?
			result= false;
		else
			stackpointer= value;
		break;
	default:
		if(regidx<=GPMAX)
		{
			SetFlags(value);
			generalregs[regidx-GP]= value;
		}
		else
			result= false;
	}
	
	return result;
}

bool TES3MACHINE::SetScript(VPSCRIPT pscript)
{
	scriptaddr=	pscript;
	return ReadMem((VPVOID)pscript,&script,sizeof(script));
}

bool TES3MACHINE::Interrupt(VMINTERRUPT num)
{
	bool result= false;
	if(num==INTSWITCHREFERENCE)
	{
		BYTE buf[512];
		VMSIZE read= 0;
		VMSIZE offset= 0;
		result= true;

//having min define for the next line hurts
#undef min
		while(result && (read=std::min((VMSIZE)sizeof(buf),script.scdtlength-offset))>0)
#define min(a,b) (((a) < (b)) ? (a) : (b))
		{
			result= (ReadMem(script.scdt+offset,buf,read)
				&& WriteMem((VPVOID)SCRIPTMEM_VPOS+offset,buf,read));
			offset+= read;
		}
		strings.clear();
		SetRegister(SP,0);
	}
		
	return result;
}

const Context TES3MACHINE::GetFlow(void) //return the Flow context, (registers, ...)
{
	return flow;
}

void TES3MACHINE::SetFlow(const Context newflow) //set the flow context (registers, ...)
{
	this->flow= newflow;
}

bool TES3MACHINE::SetVMDebuggerBreakpoint(HWBREAKPOINT* breakpoint)
{
	if(this->breakpoint && breakpoint) return false;
	this->breakpoint= breakpoint;
	return true;
}

HWBREAKPOINT* TES3MACHINE::GetVMDebuggerBreakpoint()
{
	return breakpoint;
}

const char* TES3MACHINE::GetScriptName(void)
{
	const char* result= NULL;
	if(scriptaddr)
		result= (const char*)script.name;
		
	return result;
}

const char* TES3MACHINE::GetString(VPVOID addr)
{
	const char* result= 0;
	VMBYTE blen= 0;
	
//	LOG::log("TES3MACHINE::GetString(%lx)\n",addr);
	if(addr && (LPVOID)addr<(LPVOID)32767)
	{
		if(ReadMem(addr,&blen,sizeof(blen)))
		{
			char* string= new char[blen+4];
			if(ReadMem(addr+sizeof(blen),string,blen))
			{
				string[blen]= '\0';
				result= strings.add(string);
			}
			delete []string;
		}
	}
	else
		result= (const char*)addr;
		
//	const char* printable= result;
//	if(!printable)
//		printable= "null";
		
//	LOG::log("%s= TES3MACHINE::GetString(%lx)\n",printable,addr);
	return result;
}


bool TES3MACHINE::dumpmem(VPVOID ptr, int size)
{
	bool result= false;
	BYTE* buf= new BYTE[size];
	if(ReadMem(ptr,buf,size))
	{
		LOG::logbinary(buf,size);
		result= true;
	}
	else
		LOG::log("dumpmem: ReadMem failed");
	delete[] buf;
	return result;
}

void TES3MACHINE::dumpscriptstack(void)
{
	VMREGTYPE sp= 0;
	if(GetRegister(SP,sp))
	{
		LOG::log("script sp %lx\n",sp);
		int size= 0-sp;
		dumpmem((VPVOID)sp,size<64?size:64);
	}
	else
		LOG::log("dumpscriptstack: GetRegister failed");
}

void TES3MACHINE::dumpscript(void)
{
	LOG::log("Stack at %lx\n",scriptaddr);
	LOG::logbinary((LPVOID)&script,sizeof(script));
	LOG::log("SCDT\n");
	dumpmem(script.scdt,script.scdtlength);
}

void TES3MACHINE::dumptemplate(VPTEMPLATE ptempl)
{
	TES3TEMPLATE templ;
	char buf[256];
	if(ReadMem((VPVOID)ptempl,(void*)&templ,sizeof(TES3TEMPLATE)))
	{
		LOG::log("Template from %lx\n",ptempl);
		LOG::logbinary((void*)&templ,sizeof(TES3TEMPLATE));
		if(ReadMem((VPVOID)templ.objectid,buf,32))
		{
			LOG::log("ObjectID from %lx\n",templ.objectid);
			LOG::logbinary(buf,32);
		}
		else
			LOG::log("dumpobject: ObjectID failed\n");
	}
	else
		LOG::log("dumpobject: Template failed\n");
}

void TES3MACHINE::dumpobject(VPREFERENCE pref)
{
	// 2005-02-05  CDC     2005-07-06  0 is an address, non-digits quit now
	// Allows for interactive exploration of memory - not pretty, but works
	// You'll need a fairly wide screen and should start the extender from
	// a CygWin shell window for best effect.
	// The MEMLOOK function will trigger this code which breaks out of the
	// program and dumps the REFR data for the current object.
	// Warning: Memory addresses in the listing are in little-endian order
	// but those listed on the first line aren't.
	// You can type a memory address (in hex) to dump a section of memory
	// starting at that address or a letter (other than a through f and
	// A through F) to return to the game.  If you type a small hex number 
	// the value will be used like an array subscript to pick a memory 
	// address from the currently shown data treating the data as an array 
	// of long values.  Some interesting values include:
	//   From the REFR data:
	//      0A  the template for the current object
	//      11  the attachments for the current object
	//      05  a cell link field (from there 03 takes you to the cell template)
	//   From the template data area:
	//	0B  the objectid for this object
	//	1B  a second template I call the base template
	//   For NPC_ type template or base template data:
	//	14  the character's real name
	//	2C  the character's race
	//	2D  the character's class
	//	2E  the character's head
	//	2F  the character's hair
	//   For the attachment list, the first item is a type code
	//	01  is the next attachment list item
	//	02  is the data for that attachment
	//   For attachement type 6, the first field (00) is a count.
	//	01  is the owner
	//	05  is the script
	//   Attachement type 8 is long and has lot's of interesting data
	unsigned long addr = (unsigned long) pref;
        unsigned x, y, size;
	BYTE buffer[1024];
	BYTE* ptr= buffer;
	char buf[1024];

	size = sizeof(TES3REFERENCE);
	printf("Script %lx %lx: Reference from ",scriptaddr,script);
	do {
        if ( addr <= 0x40 ) 
		addr = *((unsigned long *)(ptr+addr*4));
	printf("%lX - %lX\n", addr, addr+size);
	if(!ReadMem((VPVOID)addr,(void*)&buffer,size))
		return;
	for(y=0;y<size;y+=32)
	{
		for(x=0;x<32 && (x+y)<size;x++) {
			printf("%02X",((int)ptr[y+x])&0xFF);
			if ( x == 3 || x == 11 || x == 19 || x == 27 )
				printf(" ");
			if ( x == 7 || x == 23 )
				printf("  ");
			if ( x == 15 )
				printf(" | ");
		}
		printf(" ");
		for(x=0;x<32 && (x+y)<size;x++) {
			if(isprint(ptr[y+x]))
				printf("%c",ptr[y+x]);
			else
				printf(".");
			if ( x == 3 || x == 11 || x == 19 || x == 27 )
				printf(" ");
			if ( x == 7 || x == 23 )
				printf("  ");
			if ( x == 15 )
				printf(" | ");
		}

		printf("\n");
	}
	fflush(stdout);
	size = 256;
	scanf("%1020s", buf);
	sscanf(buf,"%lx", &addr);
	} while ( (buf[0]>='0' && buf[0]<='9') || (buf[0]>='a' && buf[0]<='f') || ( buf[0]>= 'A'&& buf[0]<='F') );
}

void TES3MACHINE::dumpobjects(void)
{
	const Context& context= GetFlow();
	VPVOID master2= 0;
	VPVOID target= 0;
	VPVOID pvariables= 0;
	TES3VARIABLES variables;
	
	LOG::log("Stack from %lx\n",context.Esp);
	if(!dumpmem((VPVOID)context.Esp,64))
		LOG::log("dump: Stack failed\n");
		
	LOG::log("Base from %lx\n",context.Ebp);
	if(!dumpmem((VPVOID)context.Ebp,64))
		LOG::log("dump: Stack failed\n");

	LOG::log("View Target:\n");
	if(ReadMem((VPVOID)reltolinear(MASTER2_IMAGE),&master2,sizeof(master2)))
	{
		LOG::log("master2= %lx\n",master2);
		if(ReadMem(master2+0x4c+0x9c,&target,sizeof(target)))
			dumpobject((VPREFERENCE)target);
		else
			LOG::log("dump: target failed\n");
	}
	else
		LOG::log("dump: ViewData failed\n");

	LOG::log("Script Local Variables\n");
	dumpobject((VPREFERENCE)context.Ecx);
	
	if(ReadMem((VPVOID)reltolinear(LOCALVARIABLES_IMAGE),&pvariables,sizeof(pvariables))
		&& ReadMem(pvariables,&variables,sizeof(variables)))
	{
		LOG::log("LOCALVARIABLES from %lx\n",pvariables);
		LOG::logbinary(&variables,sizeof(variables));
	}
	else
		LOG::log("dump: LOCALVARIABLES failed\n");
		
	LOG::log("script at %lx\n",scriptaddr);
	LOG::logbinary((LPVOID)&script,sizeof(script));
}

void TES3MACHINE::searchforscripttarget(void)
{
	const Context& context= GetFlow();
	DWORD scripttarget= context.Ecx;
	LOG::log("Searching for script target %lx\n",scripttarget);

	for(VPVOID pagestart= (VPVOID)0;pagestart<(VPVOID)0x80000000;pagestart+= 0x1000)
	{
		BYTE buf[0x1000];
		if(ReadMem(pagestart,buf,sizeof(buf)))
		{
			for(int i=0;i<= sizeof(buf)-sizeof(DWORD);i++)
			{
				DWORD val= *(DWORD*)&buf[i];
				if(val==scripttarget)
					LOG::log("found scripttarget pointer %lx at %lx\n",val,pagestart+i);
			}
		}
	}
}
