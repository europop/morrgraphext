#pragma once
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "BitOperations.h"
#include "joint.h"

// MGE Generic flags
#define MGE_DISABLED_BIT		 0
#define MGE_DISABLED				MASK(MGE_DISABLED_BIT)
#define FOG_ENABLED_BIT			 1
#define FOG_ENABLED				MASK(FOG_ENABLED_BIT)
#define FPS_COUNTER_BIT			 2
#define FPS_COUNTER				MASK(FPS_COUNTER_BIT)
#define DISPLAY_MESSAGES_BIT	 3
#define DISPLAY_MESSAGES		MASK(DISPLAY_MESSAGES_BIT)
#define USE_HW_SHADER_BIT		 4
#define USE_HW_SHADER			MASK(USE_HW_SHADER_BIT)
#define NO_SHADER_IN_MENU_BIT	 5
#define NO_SHADER_IN_MENU		MASK(NO_SHADER_IN_MENU_BIT)
#define AA_COLOUR_FIX_BIT		 6
#define AA_COLOUR_FIX			MASK(AA_COLOUR_FIX_BIT)
#define INPUT_LAG_FIX_BIT		 7
#define INPUT_LAG_FIX			MASK(INPUT_LAG_FIX_BIT)
#define ZOOM_ROT_HUD_BIT		 8
#define ZOOM_ROT_HUD			MASK(ZOOM_ROT_HUD_BIT)
#define ZOOM_ASPECT_BIT			 9
#define ZOOM_ASPECT				MASK(ZOOM_ASPECT_BIT)
#define MWSE_DISABLED_BIT		10
#define MWSE_DISABLED			MASK(MWSE_DISABLED_BIT)
#define MWE_DISABLED_BIT		11
#define MWE_DISABLED			MASK(MWE_DISABLED_BIT)
#define USE_TEX_HOOKS_BIT		12
#define USE_TEX_HOOKS			MASK(USE_TEX_HOOKS_BIT)
#define SET_SHADER_VARS_BIT		13
#define SET_SHADER_VARS			MASK(SET_SHADER_VARS_BIT)
#define USE_HDR_BIT				14
#define USE_HDR					MASK(USE_HDR_BIT)
#define FPS_HOLD_BIT			15
#define FPS_HOLD				MASK(FPS_HOLD_BIT)
#define BIND_AI_TO_VIEW_BIT		16
#define BIND_AI_TO_VIEW			MASK(BIND_AI_TO_VIEW_BIT)
#define CPU_IDLE_BIT			17
#define CPU_IDLE				MASK(CPU_IDLE_BIT)
#define SHADER_DEPTH_BIT		18
#define SHADER_DEPTH			MASK(SHADER_DEPTH_BIT)
// Distant Land flags
#define USE_DISTANT_LAND_BIT	19
#define USE_DISTANT_LAND		MASK(USE_DISTANT_LAND_BIT)
#define USE_DISTANT_STATICS_BIT	20
#define USE_DISTANT_STATICS		MASK(USE_DISTANT_STATICS_BIT)
#define NO_INTERIOR_DL_BIT		21
#define NO_INTERIOR_DL			MASK(NO_INTERIOR_DL_BIT)
#define REFLECTIVE_WATER_BIT	22
#define REFLECTIVE_WATER		MASK(REFLECTIVE_WATER_BIT)
#define REFLECT_NEAR_BIT		23
#define REFLECT_NEAR			MASK(REFLECT_NEAR_BIT)
#define REFLECT_FAR_BIT			24
#define REFLECT_FAR				MASK(REFLECT_FAR_BIT)
#define NOT_USING_DL_BIT		25
#define NOT_USING_DL			MASK(NOT_USING_DL_BIT)
#define NO_MW_MGE_BLEND_BIT		26
#define NO_MW_MGE_BLEND			MASK(NO_MW_MGE_BLEND_BIT)
#define REFLECT_SKY_BIT			27
#define REFLECT_SKY				MASK(REFLECT_SKY_BIT)
#define DYNAMIC_RIPPLES_BIT		28
#define DYNAMIC_RIPPLES			MASK(DYNAMIC_RIPPLES_BIT)
#define BLUR_REFLECTIONS_BIT	29
#define BLUR_REFLECTIONS		MASK(BLUR_REFLECTIONS_BIT)
#define EXP_FOG_BIT				30
#define EXP_FOG					MASK(EXP_FOG_BIT)


#ifdef _CONF
#define EX
#else
#define EX extern
#endif

EX DWORD MGEFlags;
EX bool IntegrationDisabled;
EX bool PatchDisabled;
EX BYTE AALevel;
EX BYTE ZBufFormat;
EX BYTE VWait;
EX BYTE RefreshRate;
EX BYTE ScaleFilter;
EX BYTE AnisoLevel;
EX BYTE MipFilter;
EX float LODBias;
EX float Rotation;
EX float FOV;
EX float ScreenFOV;
EX BYTE FogMode;
EX BYTE SSFormat;
EX char SSDir[208];
EX char SSName[32];
EX BYTE SSMinNumChars;
EX float ReactionSpeed;
EX float FPSDesired;
EX float FPSTolerance;
EX float MinViewRange;
EX float MaxViewRange;
EX int IdleTime;
EX int MsgTime;
EX int TexRes;
EX int NormRes;
EX int MeshDet;
EX WORD MinStat;
EX BYTE GrassDens;
EX BYTE StatMesh;
EX int MipSkip;
EX bool Activators;
EX bool MiscObj;
EX bool StatIntExt;
EX bool StatIntWater;
EX LPVOID WizardPlugins;
EX float DL_DrawDist;
EX float DL_ShaderModel;
EX float DL_NearStaticEnd;
EX float DL_FarStaticEnd;
EX float DL_VeryFarStaticEnd;
EX float DL_FarStaticMinSize;
EX float DL_VeryFarStaticMinSize;
EX float DL_AboveWaterFogStart;
EX float DL_AboveWaterFogEnd;
EX float DL_BelowWaterFogStart;
EX float DL_BelowWaterFogEnd;
EX float DL_InteriorFogStart;
EX float DL_InteriorFogEnd;
EX float DL_ExpFogMultiplier;
EX BYTE DL_WaterWaveHeight;
EX BYTE DL_WaterCaustics;
EX BYTE DL_WaterReflect;
EX float DL_Wind[10];
EX float DL_FogD[10];
EX float DL_FgOD[10];

// Not read from INI
EX BYTE RangedFog;
EX BYTE FogPixelMode;
EX BYTE FogVertexMode;
EX BYTE InvertUsingDL;

bool LoadSettings();
bool SetDistantLand(bool EnableDL);
LPVOID GetSectionComments(LPCSTR section, LPCSTR filename);

#define DATA_FILES			"Data Files\\"
#define DL_VERSION			"Data files\\distantland\\version"
#define DL_WORLD_TEXTURE	"Data files\\distantland\\world.dds"
#define DL_WORLD_NORMALS	"Data files\\distantland\\world_n.tga"
#define DL_WORLD_MESHES 	"Data files\\distantland\\world"
#define DL_STATICS			"Data Files\\DistantLand\\Statics\\"
#define DL_STATICS_USAGE 	"Data files\\distantland\\statics\\usage.data"
#define DL_STATICS_MESHES 	"Data files\\distantland\\statics\\static_meshes"

#undef EX

#endif /* _CONFIGURATION_H_ */
