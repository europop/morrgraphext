#include "d3d8header.h"
#include "Configuration.h"
#include "FogFormulas.h"
#include "MWBridge.h"

float DistantFogStart() {
	if ((MGEFlags & EXP_FOG) && DL_ShaderModel >= 3) return 0;
	return ((DL_AboveWaterFogStart - DL_AboveWaterFogEnd * SmoothFogOffset) * FogRange * SmoothFogFactor);
}

float DistantFogEnd() {
	return (DL_AboveWaterFogEnd * (1.0f - SmoothFogOffset) * FogRange * SmoothFogFactor);
}

float UnderwaterFogStart() {
	if ((MGEFlags & EXP_FOG) && DL_ShaderModel >= 3) return 0;
	return (DL_BelowWaterFogStart * FogRange);
}

float UnderwaterFogEnd() {
	return (DL_BelowWaterFogEnd * FogRange);
}

float InteriorFogStart() {
	if ((MGEFlags & EXP_FOG) && DL_ShaderModel >= 3) return 0;
	DECLARE_MWBRIDGE
	return ((DL_InteriorFogStart * mwBridge->IntFogDens() + (1.0f - mwBridge->IntFogDens()) * DL_InteriorFogEnd) * FogRange);
}

float InteriorFogEnd() {
	return (DL_InteriorFogEnd * FogRange);
}

float StandardFogStart() {
	return (FogStart * FogRange);
}

float StandardFogEnd() {
	return (FogEnd * FogRange);
}
/*
float ExpDistantFogEnd() {
	return 1000.0f;
	DECLARE_MWBRIDGE
	float r = (DL_AboveWaterFogEnd * (1.0f - SmoothFogOffset) * FogRange * SmoothFogFactor);
	return mwBridge->GetViewDistance() / ( 1 - exp( - mwBridge->GetViewDistance() / r ) );
}

float ExpUnderwaterFogEnd() {
	DECLARE_MWBRIDGE
	float r = (DL_BelowWaterFogEnd * FogRange);
	return mwBridge->GetViewDistance() / ( 1 - exp( - mwBridge->GetViewDistance() / r ) );
}

float ExpInteriorFogEnd() {
	DECLARE_MWBRIDGE
	float r = (DL_InteriorFogEnd * FogRange);
	return mwBridge->GetViewDistance() / ( 1 - exp( - mwBridge->GetViewDistance() / r ) );
}
*/