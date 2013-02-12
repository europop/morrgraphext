#pragma once
#ifdef _CONF
#ifndef _INIDATAL_H_
#define _INIDATA_H_

#include "d3d8header.h"

char mgeini [] = "MGE3/MGE.ini";

#define siniGlobGraph		sections[0]
#define siniRendState		sections[1]
#define siniInput			sections[2]
#define siniMisc			sections[3]
#define siniDL				sections[4]
#define siniDLWeather		sections[5]
#define siniDLWizardSets	sections[6]
#define siniMain			sections[7]
#define siniDLWizardPlugins	sections[8]

char* sections [] = {
	"Global Graphics",
	"Render State",
	"Input",
	"Misc",
	"Distant Land",
	"Distant Land Weather",
	"DLWizard Settings",
	"Main",
	"DLWizard Plugins"
};

tdictent dictentBool [] = {
	{False, 0},
	{True, 1},
	{"False", 0},
	{"True", 1},
	{"No", 0},
	{"Yes", 1},
	{"Off", 0},
	{"On", 1}
};
tdictionary dictBool = {countof (dictentBool), dictentBool};

tdictent dictentAA [] = {
	{"None", 0},
	{"2x", 2},
	{"3x", 3},
	{"4x", 4},
	{"5x", 5},
	{"6x", 6},
	{"7x", 7},
	{"8x", 8},
	{"9x", 9},
	{"10x", 10},
	{"11x", 11},
	{"12x", 12},
	{"13x", 13},
	{"14x", 14},
	{"15x", 15},
	{"16x", 16}
};
tdictionary dictAA = {countof (dictentAA), dictentAA};

tdictent dictentZBuf [] = {
	{"D24S8", 75},
	{"D24X8", 77},
	{"D32", 71},
	{"D16", 80},
	{"D16L", 70},
	{"D32FL", 82},
	{"INTZ", 1},
	{"RAWZ", 2},
	{"DF16", 3},
	{"DF24", 4}
};
tdictionary dictZBuf = {countof (dictentZBuf), dictentZBuf};

tdictent dictentVWait [] = {
	{"Immediate", 3},
	{"1", 1},
	{"2", 2},
	{"3", 4},
	{"4", 8},
	{"Default", 0}
};
tdictionary dictVWait = {countof (dictentVWait), dictentVWait};

tdictent dictentRefrRate [] = {
	{"Default", 0}
};
tdictionary dictRefrRate = {countof (dictentRefrRate), dictentRefrRate};

tdictent dictentScaleFilt [] = {
	{"Point", 1},
	{"Linear", 2},
	{"Anisotropic", 3},
	{"Pyramidal quad", 4},
	{"Gaussian quad", 5}
};
tdictionary dictScaleFilt = {countof (dictentScaleFilt), dictentScaleFilt};

tdictent dictentMipFilt [] = {
	{"None", 0},
	{"Point", 1},
	{"Linear", 2}
};
tdictionary dictMipFilt = {countof (dictentMipFilt), dictentMipFilt};

tdictent dictentAnisoLvl [] = {
	{"Off", 1},
	{"2x", 2},
	{"4x", 4},
	{"8x", 8},
	{"16x", 16}
};
tdictionary dictAnisoLvl = {countof (dictentAnisoLvl), dictentAnisoLvl};

tdictent dictentFogMode [] = {
	{"Depth pixel", 0},
	{"Depth vertex", 1},
	{"Range vertex", 2}
};
tdictionary dictFogMode = {countof (dictentFogMode), dictentFogMode};

tdictent dictentSSFormat [] = {
	{"BMP", 0},
	{"JPEG", 1},
	{"DDS", 2},
	{"PNG", 3},
	{"TGA", 4}
};
tdictionary dictSSFormat = {countof (dictentSSFormat), dictentSSFormat};

tdictent dictentTexRes [] = {
	{"128", 0},
	{"256", 1},
	{"512", 2},
	{"1024", 3},
	{"2048", 4},
	{"4096", 5},
	{"8192", 6}
};
tdictionary dictTexRes = {countof (dictentTexRes), dictentTexRes};

tdictent dictentMeshDet [] = {
    {"Auto", -1},
    {"Ultra High", 0},
    {"Very High", 1},
    {"High", 2},
    {"Medium", 3},
    {"Low", 4}
};
tdictionary dictMeshDet = {countof (dictentMeshDet), dictentMeshDet};

tdictent dictentSMeshDet [] = {
	{"Full", 0},
	{"90%", 1},
	{"75%", 2},
	{"50%", 3},
	{"25%", 4}
};
tdictionary dictSMeshDet = {countof (dictentSMeshDet), dictentSMeshDet};

tdictent dictentSM [] = {
	{"2.0", 2},
	{"3.0", 3}
};
tdictionary dictSM = {countof (dictentSM), dictentSM};

iniSetting iniSettings [] = {
	// Generic Flags
	{&MGEFlags, t_bit, FOG_ENABLED_BIT, siniRendState, "Enable Fog", True, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, FPS_COUNTER_BIT, siniRendState, "MGE FPS Counter", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, DISPLAY_MESSAGES_BIT, siniRendState, "MGE Messages", True, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, USE_HW_SHADER_BIT, siniRendState, "Hardware Shader", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, NO_SHADER_IN_MENU_BIT, siniRendState, "Disable Shaders In Menu", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, AA_COLOUR_FIX_BIT, siniRendState, "AA Colour Fix", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, INPUT_LAG_FIX_BIT, siniInput, "Input Lag Fix", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, ZOOM_ROT_HUD_BIT, siniMisc, "Camera Effects Affect The HUD", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, ZOOM_ASPECT_BIT, siniMisc, "Correct Aspect Ratio When Zooming", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, MGE_DISABLED_BIT, siniMisc, "MGE Disabled", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, MWSE_DISABLED_BIT, siniMisc, "Internal MWSE Disabled", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, MWE_DISABLED_BIT, siniMisc, "Internal MWE Disabled", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, USE_TEX_HOOKS_BIT, siniMisc, "Use Texture Hooks", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, SET_SHADER_VARS_BIT, siniMisc, "Update Shader Vars", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, USE_HDR_BIT, siniMisc, "Use HDR", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, FPS_HOLD_BIT, siniMisc, "Keep FPS", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, BIND_AI_TO_VIEW_BIT, siniMisc, "Bind AI To View Distance", True, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, CPU_IDLE_BIT, siniMisc, "Force CPU Idle", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, SHADER_DEPTH_BIT, siniMisc, "Update Shader Depth", False, &dictBool, DICTONLY, 0, 0},
	// Distant Land Flags
	{&MGEFlags, t_bit, USE_DISTANT_LAND_BIT, siniDL, "Distant Land", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, USE_DISTANT_STATICS_BIT, siniDL, "Distant Statics", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, NO_INTERIOR_DL_BIT, siniDL, "Disable In Interiors", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, REFLECTIVE_WATER_BIT, siniDL, "Water Reflects Land", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, REFLECT_NEAR_BIT, siniDL, "Water Reflects Near Statics", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, REFLECT_FAR_BIT, siniDL, "Water Reflects Far Statics", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, NOT_USING_DL_BIT, siniDL, "Distant Land Initially Disabled", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, NO_MW_MGE_BLEND_BIT, siniDL, "MW Blending Initially Disabled", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, REFLECT_SKY_BIT, siniDL, "Enable Sky Reflections", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, DYNAMIC_RIPPLES_BIT, siniDL, "Dynamic Ripples", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, BLUR_REFLECTIONS_BIT, siniDL, "Blur Water Reflections", False, &dictBool, DICTONLY, 0, 0},
	{&MGEFlags, t_bit, EXP_FOG_BIT, siniDL, "Use Exponential Fog", False, &dictBool, DICTONLY, 0, 0},
	// Generic Variables
	{&IntegrationDisabled, t_bool, 1, siniMisc, "Other Integration Disabled", False, &dictBool, DICTONLY, 0, 0},
	{&PatchDisabled, t_bool, 1, siniMisc, "Patch Disabled", False, &dictBool, DICTONLY, 0, 0},
	{&AALevel, t_uint8, 1, siniGlobGraph, "Antialiasing Level", "None", &dictAA, DICTONLY, 0, 0},
	{&ZBufFormat, t_uint8, 1, siniGlobGraph, "Z-Buffer Format", "D24S8", &dictZBuf, DICTONLY, 0, 0},
	{&VWait, t_uint8, 1, siniGlobGraph, "VWait", "2", &dictVWait, DICTONLY, 0, 0},
	{&RefreshRate, t_uint8, 1, siniGlobGraph, "Refresh Rate", "Default", &dictRefrRate, MINMAX, 0, 240},
	{&ScaleFilter, t_uint8, 1, siniRendState, "Scaling Filter", "Linear", &dictScaleFilt, DICTONLY, 0, 0},
	{&AnisoLevel, t_uint8, 1, siniRendState, "Anisotropic Filtering Level", "Off", &dictAnisoLvl, DICTONLY, 0, 0},
	{&MipFilter, t_uint8, 1, siniRendState, "Mipmap Filter", "Linear", &dictMipFilt, DICTONLY, 0, 0},
	{&LODBias, t_float, 1, siniRendState, "Mipmap LOD Bias", "0", NULL, MINMAX, -12, 12},
	{&Rotation, t_float, 1, siniRendState, "Screen Rotation", "0", NULL, MINMAX | MODULO, 0, 360},
	{&FOV, t_float, 1, siniRendState, "Initial Zoom", "3", NULL, MINMAX, 3, 15},
	{&ScreenFOV, t_float, 1, siniRendState, "Horizontal Screen FOV", "75", NULL, MINMAX, 5, 150},
	{&FogMode, t_uint8, 1, siniRendState, "Fog Mode", "Depth pixel", &dictFogMode, DICTONLY, 0, 0},
	{&MsgTime, t_int32, 1, siniRendState, "MGE Messages Timeout", "5000", NULL, MINMAX, 500, 50000},
	{&SSFormat, t_uint8, 1, siniRendState, "Screenshot Format", "BMP", &dictSSFormat, DICTONLY, 0, 0},
	{&SSDir, t_string, sizeof (SSDir), siniRendState, "Screenshot Output Directory", "", NULL, 0, 0, 0},
	{&SSName, t_string, sizeof (SSName), siniRendState, "Screenshot Name Prefix", "MGE Screenshot ", NULL, 0, 0, 0},
	{&SSMinNumChars, t_uint8, 1, siniRendState, "Screenshot Number Min Lenght", "1", NULL, MINMAX, 1, 5},
	{&ReactionSpeed, t_float, 1, siniMisc, "HDR Reaction Time", "2", NULL, MINMAX, 0.01, 30},
	{&FPSDesired, t_float, 1, siniMisc, "Desired FPS", "30", NULL, MINMAX, 1, 240},
	{&FPSTolerance, t_float, 1, siniMisc, "FPS Tolerance", "5", NULL, MINMAX, 0, 250},
	{&MinViewRange, t_float, 1, siniMisc, "Min View Distance", "2500", NULL, MINMAX, MINVIEWDISTANCE, MAXVIEWDISTANCE},
	{&MaxViewRange, t_float, 1, siniMisc, "Max View Distance", "7168", NULL, MINMAX, MINVIEWDISTANCE, MAXVIEWDISTANCE},
	{&IdleTime, t_int32, 1, siniMisc, "Idle Time", "0", NULL, MINMAX, 0, 1000},
	{&ForceCam3Offset, t_bool, 1, siniMisc, "Customize 3rd Person Camera", False, &dictBool, DICTONLY, 0, 0},
	{&Cam3Offset.x, t_float, 1, siniMisc, "Initial 3rd Person Camera X", "0", NULL, MINMAX, -125, 125},
	{&Cam3Offset.y, t_float, 1, siniMisc, "Initial 3rd Person Camera Y", "-160", NULL, MINMAX, -2500, -25},
	{&Cam3Offset.z, t_float, 1, siniMisc, "Initial 3rd Person Camera Z", "0", NULL, MINMAX, -125, 125},
	// Distant Land Wizard Variables
	{&TexRes, t_int32, 1, siniDLWizardSets, "World texture resolution", "2048", &dictTexRes, DICTONLY, 0, 0},
	{&NormRes, t_int32, 1, siniDLWizardSets, "World normalmap resolution", "1024", &dictTexRes, DICTONLY, 0, 0},
	{&MeshDet, t_int32, 1, siniDLWizardSets, "World mesh detail", "Auto", &dictMeshDet, DICTONLY, 0, 0},
	{&MinStat, t_uint16, 1, siniDLWizardSets, "Minimum static size", "400", NULL, MINMAX, 0, 999},
	{&GrassDens, t_uint8, 1, siniDLWizardSets, "Grass density", "100", NULL, MINMAX, 0, 100},
	{&StatMesh, t_uint8, 1, siniDLWizardSets, "Mesh detail", "Full", &dictSMeshDet, DICTONLY, 0, 0},
	{&MipSkip, t_int32, 1, siniDLWizardSets, "Skip mipmap levels", "2", NULL, MINMAX, 0, 6},
	{&Activators, t_bool, 1, siniDLWizardSets, "Include activators", False, &dictBool, DICTONLY, 0, 0},
	{&MiscObj, t_bool, 1, siniDLWizardSets, "Include misc objects", False, &dictBool, DICTONLY, 0, 0},
	{&StatIntExt, t_bool, 1, siniDLWizardSets, "Statics for behave like exterior cells", False, &dictBool, DICTONLY, 0, 0},
	{&StatIntWater, t_bool, 1, siniDLWizardSets, "Statics for interiors with water", False, &dictBool, DICTONLY, 0, 0},
	// Distant Land Variables
	{&DL_DrawDist, t_float, 1, siniDL, "Draw Distance", "13", NULL, MINMAX, 1, 300},
	{&DL_ShaderModel, t_float, 1, siniDL, "Shader Model", "2.0", &dictSM, DICTONLY, 0, 0},
	{&DL_NearStaticEnd, t_float, 1, siniDL, "Near Statics End", "3", NULL, MINMAX, 2, 299.8},
	{&DL_FarStaticEnd, t_float, 1, siniDL, "Far Statics End", "6", NULL, MINMAX, 2.1, 299.9},
	{&DL_VeryFarStaticEnd, t_float, 1, siniDL, "Very Far Statics End", "12", NULL, MINMAX, 2.2, 300},
	{&DL_FarStaticMinSize, t_float, 1, siniDL, "Far Static Min Size", "600", NULL, MINMAX, 0, 9999},
	{&DL_VeryFarStaticMinSize, t_float, 1, siniDL, "Very Far Static Min Size", "800", NULL, MINMAX, 0, 9999},
	{&DL_AboveWaterFogStart, t_float, 1, siniDL, "Above Water Fog Start", "2", NULL, MINMAX, 0, 299.9},
	{&DL_AboveWaterFogEnd, t_float, 1, siniDL, "Above Water Fog End", "13", NULL, MINMAX, 0.1, 300},
	{&DL_BelowWaterFogStart, t_float, 1, siniDL, "Below Water Fog Start", "-3", NULL, MINMAX, -99.9, 299.9},
	{&DL_BelowWaterFogEnd, t_float, 1, siniDL, "Below Water Fog End", "3", NULL, MINMAX, 0.1, 300},
	{&DL_InteriorFogStart, t_float, 1, siniDL, "Interior Fog Start", "0", NULL, MINMAX, -0.9, 9.9},
	{&DL_InteriorFogEnd, t_float, 1, siniDL, "Interior Fog End", "0.9", NULL, MINMAX, 0.1, 10},
	{&DL_ExpFogMultiplier, t_float, 1, siniDL, "Exponential Distance Multiplier", "4", NULL, MINMAX, 2.5, 5},
	{&DL_WaterWaveHeight, t_uint8, 1, siniDL, "Water Wave Height", "0", NULL, MINMAX, 0, 250},
	{&DL_WaterCaustics, t_uint8, 1, siniDL, "Water Caustics Intensity", "50", NULL, MINMAX, 0, 100},
	// wind
	{&DL_Wind[0], t_float, 1, siniDLWeather, "Clear Wind Ratio", "0.1", NULL, MINMAX, 0, 1},
	{&DL_Wind[1], t_float, 1, siniDLWeather, "Cloudy Wind Ratio", "0.2", NULL, MINMAX, 0, 1},
	{&DL_Wind[2], t_float, 1, siniDLWeather, "Foggy Wind Ratio", "0", NULL, MINMAX, 0, 1},
	{&DL_Wind[3], t_float, 1, siniDLWeather, "Overcast Wind Ratio", "0.2", NULL, MINMAX, 0, 1},
	{&DL_Wind[4], t_float, 1, siniDLWeather, "Rain Wind Ratio", "0.3", NULL, MINMAX, 0, 1},
	{&DL_Wind[5], t_float, 1, siniDLWeather, "Thunderstorm Wind Ratio", "0.5", NULL, MINMAX, 0, 1},
	{&DL_Wind[6], t_float, 1, siniDLWeather, "Ashstorm Wind Ratio", "0.8", NULL, MINMAX, 0, 1},
	{&DL_Wind[7], t_float, 1, siniDLWeather, "Blight Wind Ratio", "0.9", NULL, MINMAX, 0, 1},
	{&DL_Wind[8], t_float, 1, siniDLWeather, "Snow Wind Ratio", "0", NULL, MINMAX, 0, 1},
	{&DL_Wind[9], t_float, 1, siniDLWeather, "Blizzard Wind Ratio", "0.9", NULL, MINMAX, 0, 1},
	// day fog
	{&DL_FogD[0], t_float, 1, siniDLWeather, "Clear Fog Ratio", "1", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[1], t_float, 1, siniDLWeather, "Cloudy Fog Ratio", "0.9", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[2], t_float, 1, siniDLWeather, "Foggy Fog Ratio", "0.2", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[3], t_float, 1, siniDLWeather, "Overcast Fog Ratio", "0.7", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[4], t_float, 1, siniDLWeather, "Rain Fog Ratio", "0.5", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[5], t_float, 1, siniDLWeather, "Thunderstorm Fog Ratio", "0.5", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[6], t_float, 1, siniDLWeather, "Ashstorm Fog Ratio", "0.2", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[7], t_float, 1, siniDLWeather, "Blight Fog Ratio", "0.2", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[8], t_float, 1, siniDLWeather, "Snow Fog Ratio", "0.5", NULL, MINMAX, 0.001, 2},
	{&DL_FogD[9], t_float, 1, siniDLWeather, "Blizzard Fog Ratio", "0.16", NULL, MINMAX,  0.001, 2},
	{&DL_FgOD[0], t_float, 1, siniDLWeather, "Clear Fog Offset", "0", NULL, MINMAX, 0, 90},
	{&DL_FgOD[1], t_float, 1, siniDLWeather, "Cloudy Fog Offset", "0", NULL, MINMAX, 0, 90},
	{&DL_FgOD[2], t_float, 1, siniDLWeather, "Foggy Fog Offset", "30", NULL, MINMAX, 0, 90},
	{&DL_FgOD[3], t_float, 1, siniDLWeather, "Overcast Fog Offset", "0", NULL, MINMAX, 0, 90},
	{&DL_FgOD[4], t_float, 1, siniDLWeather, "Rain Fog Offset", "10", NULL, MINMAX, 0, 90},
	{&DL_FgOD[5], t_float, 1, siniDLWeather, "Thunderstorm Fog Offset", "20", NULL, MINMAX, 0, 90},
	{&DL_FgOD[6], t_float, 1, siniDLWeather, "Ashstorm Fog Offset", "50", NULL, MINMAX, 0, 90},
	{&DL_FgOD[7], t_float, 1, siniDLWeather, "Blight Fog Offset", "60", NULL, MINMAX, 0, 90},
	{&DL_FgOD[8], t_float, 1, siniDLWeather, "Snow Fog Offset", "40", NULL, MINMAX, 0, 90},
	{&DL_FgOD[9], t_float, 1, siniDLWeather, "Blizzard Fog Offset", "70", NULL, MINMAX, 0, 90}
};

#endif /* _INIDATA_H_ */
#endif /* _CONF */
