//File generated automatically from MGE's function documentation
#pragma once

#include "MWSEHeader.h"
#include "TES3MACHINE.h"
#include "FUNCTION.h"

struct FUNCWIPEALL : public FUNCTION
{
	FUNCWIPEALL(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCOUTPUTDEBUGSTRING : public FUNCTION
{
	FUNCOUTPUTDEBUGSTRING(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCOUTPUTVERSIONSTRING : public FUNCTION
{
	FUNCOUTPUTVERSIONSTRING(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETVERSION : public FUNCTION
{
	FUNCGETVERSION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETWIDTH : public FUNCTION
{
	FUNCGETWIDTH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETHEIGHT : public FUNCTION
{
	FUNCGETHEIGHT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETFPS : public FUNCTION
{
	FUNCGETFPS(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETEYEVEC : public FUNCTION
{
	FUNCGETEYEVEC(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEDISTANTLAND : public FUNCTION
{
	FUNCENABLEDISTANTLAND(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEDISTANTLAND : public FUNCTION
{
	FUNCDISABLEDISTANTLAND(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCWIPEHUDELEMENTS : public FUNCTION
{
	FUNCWIPEHUDELEMENTS(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCWITHHUD : public FUNCTION
{
	FUNCWITHHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCCANCELWITHHUD : public FUNCTION
{
	FUNCCANCELWITHHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCLOADHUD : public FUNCTION
{
	FUNCLOADHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCPOSITIONHUD : public FUNCTION
{
	FUNCPOSITIONHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSCALEHUD : public FUNCTION
{
	FUNCSCALEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCFULLSCREENHUD : public FUNCTION
{
	FUNCFULLSCREENHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEHUD : public FUNCTION
{
	FUNCENABLEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEHUD : public FUNCTION
{
	FUNCDISABLEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCCHANGEHUDTEXTURE : public FUNCTION
{
	FUNCCHANGEHUDTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCCHANGEHUDEFFECT : public FUNCTION
{
	FUNCCHANGEHUDEFFECT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCFREEHUD : public FUNCTION
{
	FUNCFREEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDLOADHUD : public FUNCTION
{
	FUNCNIDLOADHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDPOSITIONHUD : public FUNCTION
{
	FUNCNIDPOSITIONHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSCALEHUD : public FUNCTION
{
	FUNCNIDSCALEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDFULLSCREENHUD : public FUNCTION
{
	FUNCNIDFULLSCREENHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDENABLEHUD : public FUNCTION
{
	FUNCNIDENABLEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDDISABLEHUD : public FUNCTION
{
	FUNCNIDDISABLEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDCHANGEHUDTEXTURE : public FUNCTION
{
	FUNCNIDCHANGEHUDTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDCHANGEHUDEFFECT : public FUNCTION
{
	FUNCNIDCHANGEHUDEFFECT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDFREEHUD : public FUNCTION
{
	FUNCNIDFREEHUD(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCMANUALLIGHTING : public FUNCTION
{
	FUNCMANUALLIGHTING(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENDMANUALLIGHTING : public FUNCTION
{
	FUNCENDMANUALLIGHTING(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSUNCOLOUR : public FUNCTION
{
	FUNCSETSUNCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTSUNCOLOUR : public FUNCTION
{
	FUNCDEFAULTSUNCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSUNDIRECTION : public FUNCTION
{
	FUNCSETSUNDIRECTION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTSUNDIRECTION : public FUNCTION
{
	FUNCDEFAULTSUNDIRECTION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSUNTEXTURE : public FUNCTION
{
	FUNCSETSUNTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTSUNTEXTURE : public FUNCTION
{
	FUNCDEFAULTSUNTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETSUNCOLOUR : public FUNCTION
{
	FUNCGETSUNCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETSUNDIR : public FUNCTION
{
	FUNCGETSUNDIR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETAMBIENTCOLOUR : public FUNCTION
{
	FUNCSETAMBIENTCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTAMBIENTCOLOUR : public FUNCTION
{
	FUNCDEFAULTAMBIENTCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETAMBIENTCOLOUR : public FUNCTION
{
	FUNCGETAMBIENTCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETCLSCOLOR : public FUNCTION
{
	FUNCSETCLSCOLOR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTCLSCOLOUR : public FUNCTION
{
	FUNCDEFAULTCLSCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETCLSCOLOUR : public FUNCTION
{
	FUNCGETCLSCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSKYCOLOUR : public FUNCTION
{
	FUNCSETSKYCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTSKYCOLOUR : public FUNCTION
{
	FUNCDEFAULTSKYCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETHUDCOLOUR : public FUNCTION
{
	FUNCSETHUDCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTHUDCOLOUR : public FUNCTION
{
	FUNCDEFAULTHUDCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTAPKEY : public FUNCTION
{
	FUNCTAPKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCPUSHKEY : public FUNCTION
{
	FUNCPUSHKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCRELESEKEY : public FUNCTION
{
	FUNCRELESEKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCHAMMERKEY : public FUNCTION
{
	FUNCHAMMERKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCUNHAMMERKEY : public FUNCTION
{
	FUNCUNHAMMERKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCAHAMMERKEY : public FUNCTION
{
	FUNCAHAMMERKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCAUNHAMMERKEY : public FUNCTION
{
	FUNCAUNHAMMERKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISALLOWKEY : public FUNCTION
{
	FUNCDISALLOWKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCALLOWKEY : public FUNCTION
{
	FUNCALLOWKEY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCMOVEMOUSERIGHT : public FUNCTION
{
	FUNCMOVEMOUSERIGHT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCMOVEMOUSELEFT : public FUNCTION
{
	FUNCMOVEMOUSELEFT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCMOVEMOUSEUP : public FUNCTION
{
	FUNCMOVEMOUSEUP(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCMOVEMOUSEDOWN : public FUNCTION
{
	FUNCMOVEMOUSEDOWN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMOUSESPEEDRIGHT : public FUNCTION
{
	FUNCSETMOUSESPEEDRIGHT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMOUSESPEEDLEFT : public FUNCTION
{
	FUNCSETMOUSESPEEDLEFT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMOUSESPEEDUP : public FUNCTION
{
	FUNCSETMOUSESPEEDUP(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMOUSESPEEDDOWN : public FUNCTION
{
	FUNCSETMOUSESPEEDDOWN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEMOUSE : public FUNCTION
{
	FUNCDISABLEMOUSE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEMOUSE : public FUNCTION
{
	FUNCENABLEMOUSE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCBUFFEREDKEYDOWN : public FUNCTION
{
	FUNCBUFFEREDKEYDOWN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCBUFFEREDKEYUP : public FUNCTION
{
	FUNCBUFFEREDKEYUP(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEFOG : public FUNCTION
{
	FUNCENABLEFOG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEFOG : public FUNCTION
{
	FUNCDISABLEFOG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTOGGLEFOG : public FUNCTION
{
	FUNCTOGGLEFOG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCINCREASEFOG : public FUNCTION
{
	FUNCINCREASEFOG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDECREASEFOG : public FUNCTION
{
	FUNCDECREASEFOG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCINCREASEFOGBY : public FUNCTION
{
	FUNCINCREASEFOGBY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDECREASEFOGBY : public FUNCTION
{
	FUNCDECREASEFOGBY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETFOG : public FUNCTION
{
	FUNCSETFOG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCFOGZOOM : public FUNCTION
{
	FUNCFOGZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSTOPFOGZOOM : public FUNCTION
{
	FUNCSTOPFOGZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETFOG : public FUNCTION
{
	FUNCGETFOG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETFOGCOLOUR : public FUNCTION
{
	FUNCSETFOGCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDEFAULTFOGCOLOUR : public FUNCTION
{
	FUNCDEFAULTFOGCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETFOGCOLOUR : public FUNCTION
{
	FUNCGETFOGCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLECAMERASHAKE : public FUNCTION
{
	FUNCENABLECAMERASHAKE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLECAMERASHAKE : public FUNCTION
{
	FUNCDISABLECAMERASHAKE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETCAMERASHAKEMAGNITUDE : public FUNCTION
{
	FUNCSETCAMERASHAKEMAGNITUDE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCCAMERASHAKEZOOM : public FUNCTION
{
	FUNCCAMERASHAKEZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSTOPCAMERASHAKEZOOM : public FUNCTION
{
	FUNCSTOPCAMERASHAKEZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTAKESCREENSHOT : public FUNCTION
{
	FUNCTAKESCREENSHOT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTAKEBMPSCREENSHOT : public FUNCTION
{
	FUNCTAKEBMPSCREENSHOT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTAKEJPEGSCREENSHOT : public FUNCTION
{
	FUNCTAKEJPEGSCREENSHOT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTAKEDDSSCREENSHOT : public FUNCTION
{
	FUNCTAKEDDSSCREENSHOT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTAKEPNGSCREENSHOT : public FUNCTION
{
	FUNCTAKEPNGSCREENSHOT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEFPSCOUNTER : public FUNCTION
{
	FUNCENABLEFPSCOUNTER(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEFPSCOUNTER : public FUNCTION
{
	FUNCDISABLEFPSCOUNTER(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTOGGLEFPSCOUNTER : public FUNCTION
{
	FUNCTOGGLEFPSCOUNTER(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCROTATESCREEN : public FUNCTION
{
	FUNCROTATESCREEN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSCREENROT0 : public FUNCTION
{
	FUNCSETSCREENROT0(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSCREENROT90 : public FUNCTION
{
	FUNCSETSCREENROT90(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSCREENROT180 : public FUNCTION
{
	FUNCSETSCREENROT180(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSCREENROT270 : public FUNCTION
{
	FUNCSETSCREENROT270(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCROTATESCREENBY : public FUNCTION
{
	FUNCROTATESCREENBY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETSCREENROTATION : public FUNCTION
{
	FUNCSETSCREENROTATION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSCREENSPIN : public FUNCTION
{
	FUNCSCREENSPIN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSTOPSPINSPIN : public FUNCTION
{
	FUNCSTOPSPINSPIN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETSCREENROTATION : public FUNCTION
{
	FUNCGETSCREENROTATION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEZOOM : public FUNCTION
{
	FUNCENABLEZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEZOOM : public FUNCTION
{
	FUNCDISABLEZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTOGGLEZOOM : public FUNCTION
{
	FUNCTOGGLEZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZOOMIN : public FUNCTION
{
	FUNCZOOMIN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZOOMOUT : public FUNCTION
{
	FUNCZOOMOUT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZOOMINBY : public FUNCTION
{
	FUNCZOOMINBY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZOOMOUTBY : public FUNCTION
{
	FUNCZOOMOUTBY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETZOOM : public FUNCTION
{
	FUNCSETZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZOOM : public FUNCTION
{
	FUNCZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSTOPZOOM : public FUNCTION
{
	FUNCSTOPZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCGETZOOM : public FUNCTION
{
	FUNCGETZOOM(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCLOADTEXTURE : public FUNCTION
{
	FUNCLOADTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCFREETEXTURE : public FUNCTION
{
	FUNCFREETEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCWIPETEXTURES : public FUNCTION
{
	FUNCWIPETEXTURES(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCBEGINTEXDRAW : public FUNCTION
{
	FUNCBEGINTEXDRAW(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENDTEXDRAW : public FUNCTION
{
	FUNCENDTEXDRAW(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETTEXDRAWORIGIN : public FUNCTION
{
	FUNCSETTEXDRAWORIGIN(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETTEXDRAWSCALE : public FUNCTION
{
	FUNCSETTEXDRAWSCALE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETTEXDRAWCOLOUR : public FUNCTION
{
	FUNCSETTEXDRAWCOLOUR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDRAWTEXTTOTEXTURE : public FUNCTION
{
	FUNCDRAWTEXTTOTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDRAWTEXTURETOTEXTURE : public FUNCTION
{
	FUNCDRAWTEXTURETOTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCAUTOFREEHOOK : public FUNCTION
{
	FUNCAUTOFREEHOOK(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDONTFREEHOOK : public FUNCTION
{
	FUNCDONTFREEHOOK(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCRESUMEVIDEO : public FUNCTION
{
	FUNCRESUMEVIDEO(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCPAUSEVIDEO : public FUNCTION
{
	FUNCPAUSEVIDEO(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSTOPVIDEO : public FUNCTION
{
	FUNCSTOPVIDEO(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCREWINDVIDEO : public FUNCTION
{
	FUNCREWINDVIDEO(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSEEKVIDEO : public FUNCTION
{
	FUNCSEEKVIDEO(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEFULLSCREENSHADER : public FUNCTION
{
	FUNCENABLEFULLSCREENSHADER(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEFULLSCREENSHADERS : public FUNCTION
{
	FUNCDISABLEFULLSCREENSHADERS(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCTOGGLEFULLSCREENSHADERS : public FUNCTION
{
	FUNCTOGGLEFULLSCREENSHADERS(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCLOADEFFECT : public FUNCTION
{
	FUNCLOADEFFECT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETEFFECTTECHNIQUE : public FUNCTION
{
	FUNCSETEFFECTTECHNIQUE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCUSEEFFECTASFULLSCREENSHADER : public FUNCTION
{
	FUNCUSEEFFECTASFULLSCREENSHADER(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEEFFECTASFULLSCREENSHADER : public FUNCTION
{
	FUNCDISABLEEFFECTASFULLSCREENSHADER(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCFREEEFFECT : public FUNCTION
{
	FUNCFREEEFFECT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETEFFECTVARLONG : public FUNCTION
{
	FUNCSETEFFECTVARLONG(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETEFFECTVARFLOAT : public FUNCTION
{
	FUNCSETEFFECTVARFLOAT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETEFFECTVARVECTOR : public FUNCTION
{
	FUNCSETEFFECTVARVECTOR(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETEFFECTVARTEXTURE : public FUNCTION
{
	FUNCSETEFFECTVARTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCWIPELOADEDEFFECTS : public FUNCTION
{
	FUNCWIPELOADEDEFFECTS(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCWITHMESH : public FUNCTION
{
	FUNCWITHMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCCANCELWITHMESH : public FUNCTION
{
	FUNCCANCELWITHMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCWIPELOADEDMESHES : public FUNCTION
{
	FUNCWIPELOADEDMESHES(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCLOADMESH : public FUNCTION
{
	FUNCLOADMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHPOSITION : public FUNCTION
{
	FUNCSETMESHPOSITION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHROTATION : public FUNCTION
{
	FUNCSETMESHROTATION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHSCALE : public FUNCTION
{
	FUNCSETMESHSCALE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCMOVEMESH : public FUNCTION
{
	FUNCMOVEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCROTATEMESH : public FUNCTION
{
	FUNCROTATEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSCALEMESH : public FUNCTION
{
	FUNCSCALEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHVELOCITY : public FUNCTION
{
	FUNCSETMESHVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHANGULARVELOCITY : public FUNCTION
{
	FUNCSETMESHANGULARVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHGROWTH : public FUNCTION
{
	FUNCSETMESHGROWTH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZEROMESHVELOCITY : public FUNCTION
{
	FUNCZEROMESHVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZEROMESHANGULARVELOCITY : public FUNCTION
{
	FUNCZEROMESHANGULARVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCZEROMESHGROWTH : public FUNCTION
{
	FUNCZEROMESHGROWTH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCENABLEMESH : public FUNCTION
{
	FUNCENABLEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCDISABLEMESH : public FUNCTION
{
	FUNCDISABLEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHTEXTURE : public FUNCTION
{
	FUNCSETMESHTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCSETMESHEFFECT : public FUNCTION
{
	FUNCSETMESHEFFECT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCMANUALMESH : public FUNCTION
{
	FUNCMANUALMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCFREEMESH : public FUNCTION
{
	FUNCFREEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCUPDATEMESHLIGHTING : public FUNCTION
{
	FUNCUPDATEMESHLIGHTING(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDLOADMESH : public FUNCTION
{
	FUNCNIDLOADMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHPOSITION : public FUNCTION
{
	FUNCNIDSETMESHPOSITION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHROTATION : public FUNCTION
{
	FUNCNIDSETMESHROTATION(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHSCALE : public FUNCTION
{
	FUNCNIDSETMESHSCALE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDMOVEMESH : public FUNCTION
{
	FUNCNIDMOVEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDROTATEMESH : public FUNCTION
{
	FUNCNIDROTATEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSCALEMESH : public FUNCTION
{
	FUNCNIDSCALEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHVELOCITY : public FUNCTION
{
	FUNCNIDSETMESHVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHANGULARVELOCITY : public FUNCTION
{
	FUNCNIDSETMESHANGULARVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHGROWTH : public FUNCTION
{
	FUNCNIDSETMESHGROWTH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDZEROMESHVELOCITY : public FUNCTION
{
	FUNCNIDZEROMESHVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDZEROMESHANGULARVELOCITY : public FUNCTION
{
	FUNCNIDZEROMESHANGULARVELOCITY(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDZEROMESHGROWTH : public FUNCTION
{
	FUNCNIDZEROMESHGROWTH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDENABLEMESH : public FUNCTION
{
	FUNCNIDENABLEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDDISABLEMESH : public FUNCTION
{
	FUNCNIDDISABLEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHTEXTURE : public FUNCTION
{
	FUNCNIDSETMESHTEXTURE(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDSETMESHEFFECT : public FUNCTION
{
	FUNCNIDSETMESHEFFECT(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDMANUALMESH : public FUNCTION
{
	FUNCNIDMANUALMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDFREEMESH : public FUNCTION
{
	FUNCNIDFREEMESH(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};

struct FUNCNIDUPDATEMESHLIGHTING : public FUNCTION
{
	FUNCNIDUPDATEMESHLIGHTING(TES3MACHINE& vm) :machine(vm) {}
	virtual bool execute(void);
private:
	TES3MACHINE& machine;
};