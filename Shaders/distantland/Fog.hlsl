// This shader can be included in other shaders to implement consistant fogging

//-----------------------------------------------------------------------------
// Global Parameters
//-----------------------------------------------------------------------------

float g_FogRange;
float g_FogStart;
float3 g_FogCol;
float3 g_SkyCol;

//-----------------------------------------------------------------------------
// Static Constants
//-----------------------------------------------------------------------------

static const float FogRangeMinusStart = g_FogRange - g_FogStart;

//-----------------------------------------------------------------------------
// Shader Code
//-----------------------------------------------------------------------------

#ifdef EXPFOG

//-----------------------------------------------------------------------------

static const float ExpFogStart = max (0, g_FogStart);
static const float ExpFogMult = saturate (g_FogRange / FogRangeMinusStart);
static const float ExpFogRangeMinusStart = g_FogRange - ExpFogStart;

//-----------------------------------------------------------------------------

float3 skyfogblend (float z) {
	return lerp (
		g_FogCol,
		g_SkyCol, 1 - pow (saturate (1 - 2.22 * saturate (z - 0.075)), 1.15)
	);
}

//-----------------------------------------------------------------------------

float getFog (float dist) {
	return saturate( exp (-(dist - ExpFogStart) / ExpFogRangeMinusStart) )
		* ExpFogMult;
}

//-----------------------------------------------------------------------------

float3 applyfog (float3 col, float4 fog) {
	return lerp (fog.rgb, col, fog.a);
}

//-----------------------------------------------------------------------------

#else

float getFog (float dist) {
	return saturate ((g_FogRange - dist) / FogRangeMinusStart);
}

#endif

//-----------------------------------------------------------------------------
