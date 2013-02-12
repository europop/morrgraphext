// This shader contains techniques used to render objects to a floating point
// depth buffer.

#include "Fog.hlsl"

//-----------------------------------------------------------------------------
// Global Parameters
//-----------------------------------------------------------------------------

Matrix g_View;
Matrix g_Proj;
Matrix g_World;
Matrix g_TexProj;
float3 g_EyePos;
float g_Time;
float2 g_Offset;
float g_WaterLevel;
float g_CausticIntensity;

// Sun
float g_SunVis;
float3 g_SunCol;
float3 g_SunPos;
float3 g_SunVec;
float3 g_SunAmb;

// Blending
float g_BlendStart;
float g_BlendEnd;

//Waves and Ripples
float g_WaveTexWorldSize;
float g_WaveTexRcpRes;
float2 g_RippleOrigin;

//-----------------------------------------------------------------------------
// Textures
//-----------------------------------------------------------------------------

texture NormalWaveCausticTexture;
sampler NormalSampler = sampler_state {
	texture = <NormalWaveCausticTexture>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	addressu = wrap;
	addressv = wrap;
};

sampler WaveSampler = sampler_state {
	texture = <NormalWaveCausticTexture>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = none;
	addressu = wrap;
	addressv = wrap;
};

texture DepthTexture;
sampler DepthSampler = sampler_state {
	texture = <DepthTexture>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;
};

texture RefractionTexture;
sampler RefractionSampler = sampler_state {
	texture = <RefractionTexture>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;
};

texture ReflectionTexture;
sampler ReflectionSampler = sampler_state {
	texture = <ReflectionTexture>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;
};

texture RainRippleTexture;
sampler RainRippleSampler = sampler_state {
	texture = <RainRippleTexture>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = wrap;
	addressv = wrap;
};

texture PlayerRippleTexture;
sampler PlayerRippleSampler = sampler_state {
	texture = <tex5>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	bordercolor = 0x80808080;
	addressu = border;
	addressv = border;
};

//-----------------------------------------------------------------------------
// Static Constants
//-----------------------------------------------------------------------------

// water & dynamic ripples settings
#if WAVEHEIGHT > 0
	// use tesselated circular water mesh? false uses simple mesh
	const bool fineWaterMesh = true;		
#else
	// use tesselated circular water mesh? false uses simple mesh
	const bool fineWaterMesh = false;	
#endif

// texture resolution for rain ripples and player waves
const int waveTexResolution = 512;	

// world size of each player wave texture pixel
const float waveTexWorldResolution = 2.50f;	

static const float3 watercolour = { 0.4, 0.45, 0.5 };
static const float FLT_MAX = 340282346600000000000000000000000000000.0f;

static const float _lightfactor = 1 - pow(1 - g_SunVis, 2);

static const float3 _depthcolor =
	_lightfactor * g_SunCol * float3 (0.03, 0.04, 0.05) +
	(2 * g_SkyCol + g_FogCol) * float3 (0.075, 0.08, 0.085);

static const float3 SunCollf = g_SunCol * _lightfactor;

static const float cauststr = 0.0001f * g_CausticIntensity *
	saturate(0.75 * _lightfactor + 0.35 * length(g_FogCol));

static const float waveTexRcpRes2 = 1.5 * g_WaveTexRcpRes;

// 12 world units radius
static const float playerWaveSize = 12.0f / g_WaveTexWorldSize; 


//-----------------------------------------------------------------------------
// Shader Code
//-----------------------------------------------------------------------------

struct WaterVertOut {
	float4 position : POSITION;
	float4 pos : TEXCOORD0;
	float2 texcoord1 : TEXCOORD1;
	float2 texcoord2 : TEXCOORD2;
	float2 texcoord3 : TEXCOORD3;
	float4 screenpos : TEXCOORD5;
	#if WAVEHEIGHT > 0
		float4 screenposclamp : TEXCOORD7;
	#endif
	float w : TEXCOORD6;
};

//-----------------------------------------------------------------------------

WaterVertOut WaterVS (in float4 pos : POSITION) {
	WaterVertOut OUT;

	OUT.pos = float4 (pos.xy - g_Offset, pos.zw);

	// Calculate various texture coordinates
	OUT.texcoord1 = OUT.pos.xy / 3900;
	OUT.texcoord2 = OUT.pos.xy / 1104;
	OUT.texcoord3 = OUT.pos.xy / 892;

	#if WAVEHEIGHT > 0
		float time = g_Time / 2;

		float height = tex3Dlod(s1, float4 (OUT.texcoord2, time, 0)).a;
		float height2 = tex3Dlod(s1, float4 (OUT.texcoord1, time, 0)).a;

		float dist = length(g_EyePos.xyz - OUT.pos.xyz);

		float addheight =
			WAVEHEIGHT * (lerp(height, height2, saturate(dist / 8000)) - 0.5f)
			* saturate(1 - dist / 6400) * saturate(dist / 200);

		float4 newPos = pos + float4 (0, 0, addheight, 0);

		OUT.pos.zw = newPos.zw;

		OUT.position = mul(newPos, g_World);
		OUT.position = mul(OUT.position, g_View);
		OUT.position = mul(OUT.position, g_Proj);
		OUT.w = OUT.position.w;

		OUT.screenpos = mul(newPos , g_TexProj);
		newPos = pos - float4 (0, 0, abs(addheight), 0);
		OUT.screenposclamp = mul(newPos , g_TexProj);
	#else
		OUT.position = mul(pos, g_World);
		OUT.position = mul(OUT.position, g_View);
		OUT.position = mul(OUT.position, g_Proj);
		OUT.w = OUT.position.w;

		OUT.screenpos = mul(pos, g_TexProj);
	#endif

	return OUT;
}

//-----------------------------------------------------------------------------

float3 GetFinalWaterNormal (
	float2 texcoord1,
	float2 texcoord2,
	float2 texcoord3,
	float dist,
	float2 IN_pos_xy
) : NORMAL {
	// Calculate the W texture coordinate based on the time that has passed
	float time = g_Time / 2;
	float3 tex1 = float3(texcoord1, time);
	float3 tex2 = float3(texcoord2, time);
	
	// Blend together the normals from different sized areas of the same texture
	float2 far_normal = tex3D(NormalSampler, tex1).rg;
	float2 close_normal = tex3D(NormalSampler, tex2).rg;
	#ifdef MGE_DYNAMICRIPPLES
		// add ripples
		#if SHADER_MODEL >= 300
			close_normal.rg += tex2D(RainRippleSampler, texcoord3).ba; // rain
			close_normal.rg +=
				tex2D(PlayerRippleSampler, (IN_pos_xy - g_RippleOrigin) /
				g_WaveTexWorldSize).ba * 2 - 1.5; // player
		#else
			close_normal.rg += (tex2D(RainRippleSampler, texcoord3).ba // rain
			 + tex2D(PlayerRippleSampler, (IN_pos_xy - g_RippleOrigin)
			 / g_WaveTexWorldSize).ba) * 2 - 2; // player
		#endif
	#endif
	float2 normal_R =
		lerp(close_normal, far_normal, saturate(dist / 8000)) * 2 - 1;
	return normalize(float3(normal_R,1));
}

//-----------------------------------------------------------------------------

float4 ReflectionShader (in WaterVertOut IN): COLOR0 {
	// Calculate eye vector
	float3 EyeVec = g_EyePos.xyz - IN.pos.xyz;
	float dist = length(EyeVec);
	EyeVec /= dist;

	// Define % to Fog
	float fog = getFog (dist);
	
	// Calculate water normal
	float3 normal = GetFinalWaterNormal (
		IN.texcoord1, IN.texcoord2, IN.texcoord3, dist, IN.pos.xy);

	// reflection / refraction strength factor, wind strength increases
	// distortion
	#if SHADER_MODEL >= 300
		float2 reffactor = ((dist / 130 * _windfactor) + 0.1) * (normal.xy);
	#else
		float2 reffactor = (dist / 50) * (normal.xy);
	#endif

	// Distort refraction dependent on depth
	float4 newscrpos = IN.screenpos + float4 (reffactor.yx, 0, 0);

	// Sample depth texture
	#if SHADER_MODEL >= 300
		float depth = max(0, tex2Dproj(DepthSampler, newscrpos).r - IN.w);
	#else
		float depth = tex2Dproj(DepthSampler, newscrpos).r - IN.w;
	#endif
	newscrpos =
		IN.screenpos + saturate(depth / 100) * float4 (reffactor.yx, 0, 0);

	// Sample refraction texture
	float3 Refract = tex2Dproj(RefractionSampler, newscrpos).rgb;

	#if SHADER_MODEL >= 300
		// Get distorted depth
		depth = max(0, tex2Dproj(DepthSampler, newscrpos).r - IN.w);
		depth /=
			dot(-EyeVec, float3 (g_View [0][2], g_View [1][2], g_View [2][2]));

		float depthscale = saturate(exp(-(depth) / 800));
		float shorefactor = pow(depthscale, 25);

		float3 depthcolor = lerp(g_FogCol, _depthcolor, fog);

		// Make transition between actual refraction image and depth color
		// depending on water depth
		Refract =
			lerp(depthcolor, Refract, 0.8 * depthscale + 0.2 * shorefactor);

		// Sample reflection texture
		float3 Reflect = {0, 0, 0};

		// float totalalpha;

		if (g_EyePos.z < g_WaterLevel - 4) {
			Reflect = _depthcolor;
		} else {
			#ifdef WATERBLUR
				float blurwidth = IN.w * (1 - EyeVec.z) * 0.005;
				float4 scrpos = 
				#if WAVEHEIGHT > 0
					IN.screenposclamp
				#else
					IN.screenpos
				#endif
				 - float4 (2.5 * reffactor.x, -abs(reffactor.y), 0, 0)
					+ 0.0015 * IN.w * float4 (0, 1, 0, 0);

				// unfolded blurring allows for better optimization
				float4 shift = blurwidth * float4 (1, 0, 0, 0);
				Reflect +=
					0.100 * (tex2Dproj(ReflectionSampler, scrpos + shift).rgb
					+ tex2Dproj(ReflectionSampler, scrpos - shift).rgb);
				shift.xy = blurwidth * float2 (0.67, 0.33);
				Reflect +=
					0.115 * (tex2Dproj(ReflectionSampler, scrpos + shift).rgb
					+ tex2Dproj(ReflectionSampler, scrpos - shift).rgb);
				shift.xy = blurwidth * float2 (0.33, -0.33);
				Reflect +=
					0.135 * (tex2Dproj(ReflectionSampler, scrpos + shift).rgb
					+ tex2Dproj(ReflectionSampler, scrpos - shift).rgb);
				shift.xy = float2 (0.0, blurwidth * 0.67);
				Reflect +=
					0.150 * (tex2Dproj(ReflectionSampler, scrpos + shift).rgb
					+ tex2Dproj(ReflectionSampler, scrpos - shift).rgb);
			#else
				Reflect = tex2Dproj(ReflectionSampler, 
				#if WAVEHEIGHT > 0
					IN.screenposclamp
				#else
					IN.screenpos
				#endif
				- float4 (2.5 * reffactor.x, -abs(reffactor.y), 0, 0) + 0.0015
					* IN.w * float4 (0, 1, 0, 0));
			#endif
		}

		Reflect = lerp(g_FogCol, Reflect, fog);

		float3 adjustnormal =
			lerp(float3 (0, 0, 0.1), normal, pow(saturate(1.05 * fog), 2));
		adjustnormal =
			lerp (adjustnormal, float3 (0, 0, 1.0), (1 - EyeVec.z) *
			(1 - saturate(1 / (dist / 1000 + 1))));

		float fresnel = dot(EyeVec, adjustnormal);
		fresnel = 0.02 + pow(saturate(0.9988 - 0.28 * fresnel), 16);
		float3 Result = lerp(Refract, Reflect, fresnel);

		// Specular lighting
		float reflection =
			saturate(1.0025 * dot(EyeVec, reflect(-g_SunPos, normal)));
		float3 spec =
			SunCollf * (pow(reflection, 150) + 0.07 * pow(reflection, 4))
			* fog;

		// Result = 1500 / (1500 + tex2Dproj (DepthSampler, IN.screenpos).r) ;

		// smooth transition at shore line
		Result = lerp(Result + spec, Refract, shorefactor * fog);
	#else
		// Make transition between actual refraction image and depth color
		// depending on water depth
		Refract = lerp(Refract, _depthcolor, saturate(depth / 1500 + 0.3));

		float3 Reflect = tex2Dproj(ReflectionSampler, IN.screenpos -
			float4 (2.5 * reffactor.x, -abs(reffactor.y), 0, 0)) / 2;

		float fresnel = saturate (dot (EyeVec, float3 (0, 0, 1)) * 1.5);
		float3 Result = lerp (Reflect, Refract, fresnel);

		//Specular lighting
		float reflection = saturate(dot(EyeVec, reflect(-g_SunPos, normal)));
		float3 spec = SunCollf * pow(reflection, 150);

		// Additional Specular - gives water a shiny look
		spec += pow(dot(normal, float3 (0, 0, 1)), 100) * g_SunAmb / 12;

		// smooth transition at shore line
		Result += spec;

		Result = lerp(g_FogCol, Result, fog);
	#endif

	return float4 (Result, 1);
}

//-----------------------------------------------------------------------------

float4 NonReflectionShader (in WaterVertOut IN): COLOR0 {
	// Calculate eye vector
	float3 EyeVec = (g_EyePos - IN.pos.xyz);
	float dist = length(EyeVec);
	EyeVec /= dist;

	// Define % to Fog
	//float fog = saturate ((FogRange - dist) / FogRangeMinusStart);
	float fog = getFog (dist);
	
	// Calculate water normal
	float3 normal = GetFinalWaterNormal (
		IN.texcoord1, IN.texcoord2, IN.texcoord3, dist, IN.pos.xy);

	// reflection / refraction strength factor, wind strength increases
	// distortion
	#if SHADER_MODEL >= 300
		float2 reffactor = ((dist / 130 * _windfactor) + 0.1) * (normal.xy);
	#else
		float2 reffactor = (dist / 50) * (normal.xy);
	#endif

	// Distort refraction dependent on depth
	float4 newscrpos = IN.screenpos + float4 (reffactor.yx, 0, 0);

	// Sample depth texture
	float depth = max(0, tex2Dproj(DepthSampler, newscrpos).r - IN.w);
	newscrpos =
		IN.screenpos + saturate(depth / 100) * float4 (reffactor.yx, 0, 0);

	// Sample refraction texture
	float3 Refract = tex2Dproj(RefractionSampler, newscrpos).rgb;

	#if SHADER_MODEL >= 300
		// Get distorted depth
		depth = max(0, tex2Dproj(DepthSampler, newscrpos).r - IN.w);
		depth /=
			dot(-EyeVec, float3 (g_View [0][2], g_View [1][2], g_View [2][2]));

		float depthscale = saturate(exp(-depth / 800));
		float shorefactor = pow(depthscale, 25);

		// Make transition between actual refraction image and depth color
		// depending on water depth
		Refract =
			lerp(_depthcolor, Refract, 0.8 * depthscale + 0.2 * shorefactor);

		float fresnel =
			dot(EyeVec, lerp(normal, float3 (0, 0, 1),
			(1 - EyeVec.z) * (1 - exp(-dist / 2500)))) * fog;
		fresnel = 0.02 + pow(saturate(0.9988 - 0.28 * fresnel), 16);

		float3 Reflect = (g_FogCol + g_SkyCol) * 0.4;
		if (g_SunVec.z > 0.9) {
			Reflect = g_FogCol * 0.55;
		}
		Reflect = lerp(g_FogCol, Reflect, fog);

		float3 Result = lerp(Refract, Reflect, fresnel);

		// Specular lighting
		float reflection =
			saturate(1.0025 * dot(EyeVec, reflect(-g_SunPos, normal)));
		float3 spec =
			SunCollf * (pow(reflection, 150) + 0.07 * pow(reflection, 4))
			* fog;

		Result = lerp (Result + spec, Refract, shorefactor * fog);
	#else
		// Make transition between actual refraction image and depth color
		// depending on water depth
		Refract = lerp(Refract, _depthcolor, saturate(depth / 1500 + 0.3));

		float fresnel = saturate(dot(EyeVec,float3(0,0,1)));

		float3 Reflect = g_SkyCol / 2;

		float3 Result = lerp(Reflect, Refract, saturate(fresnel*(depth+2)));

		//Specular lighting
		float reflection =
			saturate(1.0025 * dot(EyeVec, reflect(-g_SunPos, normal)));
		float3 spec = SunCollf * pow(reflection, 150);

		// Additional Specular - gives water a shiny look
		spec += pow(dot(normal, float3 (0, 0, 1)), 100) * g_SunAmb / 12;

		Result += spec;

		//Fogging
		Result = lerp(g_FogCol, Result, fog);
	#endif

	return float4 (Result, 1);
}

//-----------------------------------------------------------------------------

float4 UnderWaterPS (in WaterVertOut IN): COLOR0 {
	// Calculate eye vector
	float3 EyeVec = (g_EyePos - IN.pos.xyz);
	float dist = length(EyeVec);
	EyeVec /= dist;

	// Calculate water normal
	float3 normal = -GetFinalWaterNormal (
		IN.texcoord1, IN.texcoord2, IN.texcoord3, dist, IN.pos.xy);

	// Define % to Fog
	// #ifdef EXPFOG
	// 	float fog = exp (-2 * dist / FogRange);
	// #else
	// 	float fog = saturate (1 - (dist / FogRange));
	// 	fog *= fog;
	// #endif
	float fog = getFog (dist);

	#if SHADER_MODEL >= 300
		// reflection / refraction strength factor, wind strength increases
		// distortion
		// float2 reffactor = -((dist / 20 * (_windfactor)) + 0.25) * float2 (
		//	dot (normal, float3 (g_View [0][0], g_View [1][0], g_View [2][0])), 
		//	dot (normal, float3 (g_View [0][1], g_View [1][1], g_View [2][1]))
		// );
		float2 reffactor = ((dist / 65 * _windfactor) + 0.03) * (normal.xy);
	#else
		float2 reffactor = dist / 20 * normal.xy;
	#endif

	// Sample depth texture
	float depth = max(
			0,
			tex2Dproj(DepthSampler, IN.screenpos + 
				float4(reffactor.xy, 0, 0)).r - IN.w
	);

	float depthfactor = saturate (depth / 150);

	#if SHADER_MODEL >= 300
		depth = max (
			0,
			tex2Dproj(DepthSampler, IN.screenpos + depthfactor *
				float4(reffactor.xy, 0, 0)).r - IN.w
		);
	#else
		depth = tex2Dproj(
			DepthSampler,
			IN.screenpos + depthfactor * float4(reffactor.xy, 0, 0)).r - IN.w;
	#endif

	// Sample refraction texture
	float3 Refract = tex2Dproj(
		RefractionSampler,
		IN.screenpos + depthfactor * float4(reffactor.xy, 0, 0)).rgb;

	depthfactor = saturate(depth / 150);

	// Specular lighting
	float reflection =
		saturate(1.005 * dot(EyeVec, reflect(
		float3(-g_SunPos.x, -g_SunPos.y, g_SunPos.z), normal)));

	float3 Specular =
		saturate(1.5 * SunCollf) * (pow(reflection, 120) + 0.07
		* pow(reflection, 2));

	#if SHADER_MODEL >= 300
		Refract += Specular * depthfactor * fog;
	#else
		Refract += Specular;
	#endif

	// fresnel factor
	float fresnel = saturate(dot(EyeVec, normal));
	fresnel = 0.4 * pow(saturate(1.3 - fresnel), 8) * fog;

	#if SHADER_MODEL >= 300
		if (g_SunVec.z > 0.9) fresnel *= 0.6;
	#endif

	float3 Result = Refract * (1 - fresnel * depthfactor);

	// Realistic Fresnel
	// fresnel = saturate (0.015 / max (0.01, fresnel - 0.645) - 0.02);
	// Refract = tex2Dproj (RefractionSampler, IN.screenpos + float4
	//	(reffactor.xy, 0, 0) / (1.001 - fresnel)).rgb;
	// float3 Result = lerp (Refract, g_FogCol, fresnel);

	return float4 (Result, 1);
}

//-----------------------------------------------------------------------------

struct SceenVertIn {
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
};

//-----------------------------------------------------------------------------

struct ScreenVertOut {
	float4 pos: POSITION;
	float2 texcoord : TEXCOORD0;
};


//-----------------------------------------------------------------------------

struct CausticsVertOut {
	float4 pos: POSITION;
	float2 tex : TEXCOORD0;
};

//-----------------------------------------------------------------------------

CausticsVertOut CausticsVS (in SceenVertIn IN) {
	CausticsVertOut OUT;
	OUT.pos = mul(IN.pos, g_Proj);
	OUT.tex = IN.texcoord;
	return OUT;
}

//-----------------------------------------------------------------------------

float4 CausticsPS (in CausticsVertOut IN): COLOR0 {
//#if SHADER_MODEL >= 300
	float3 color = tex2D(RefractionSampler, IN.tex);
	
	float depth = tex2D( DepthSampler, IN.tex ).r;
	float fog = getFog(depth);
	float blend =
		saturate( (g_BlendEnd - depth) / (g_BlendEnd - g_BlendStart) );
	
	float3 eyevec = float3(g_View[0][2],g_View[1][2],g_View[2][2]);
	eyevec += (1/g_Proj[0][0] * (2*IN.tex.x-1)).xxx *
		float3(g_View[0][0],g_View[1][0],g_View[2][0]);
	eyevec += (-1/g_Proj[1][1] * (2*IN.tex.y-1)).xxx *
		float3(g_View[0][1],g_View[1][1],g_View[2][1]);
	
	float3 uwpos = g_EyePos + eyevec * depth;
	uwpos.z -= g_WaterLevel;
	float sunraypath = (uwpos.z / g_SunVec.z);
	float caust = tex3D(NormalSampler,
		float3( (uwpos - g_SunVec * sunraypath).xy / 1104, g_Time/2) ).b;
	color *= 1.00 + caust * blend * ( max(-uwpos.z, 0) *
		saturate(exp (uwpos.z / 400)) ) * fog * fog * cauststr;
	
	return float4(color,1);
//#else
//	return float4(1,1,1,1);
//#endif
}

//-----------------------------------------------------------------------------

ScreenVertOut WaveVS (in SceenVertIn IN) {
	ScreenVertOut OUT;
	OUT.pos = mul(IN.pos, g_Proj);
	OUT.texcoord = IN.texcoord;
	return OUT;
}

//-----------------------------------------------------------------------------

float4 WavePS (in float2 Tex : TEXCOORD0): COLOR0 {
	float4 c = 2 * tex2D(WaveSampler, Tex) - (1.0).xxxx;
	float4 ret = {0, c.r, 0, 0};

	float4 n = {
		tex2D(WaveSampler, Tex + float2 (g_WaveTexRcpRes, 0)).r, 
		tex2D(WaveSampler, Tex + float2 (-g_WaveTexRcpRes, 0)).r, 
		tex2D(WaveSampler, Tex + float2 (0, g_WaveTexRcpRes)).r, 
		tex2D(WaveSampler, Tex + float2 (0, -g_WaveTexRcpRes)).r
	};
	float4 n2 = {
		tex2D(WaveSampler, Tex + float2 (waveTexRcpRes2, 0)).r, 
		tex2D(WaveSampler, Tex + float2 (-waveTexRcpRes2, 0)).r, 
		tex2D(WaveSampler, Tex + float2 (0, waveTexRcpRes2)).r, 
		tex2D(WaveSampler, Tex + float2 (0, -waveTexRcpRes2)).r
	};

	n = 2 * n - (1.0).xxxx;
	// n2 = 2 * n2 - (1.0).xxxx;

	float nsum = n.x + n.y + n.z + n.w;

	// dampened discrete two - dimensional wave equation
	// red channel: u (t)
	// green channel: u (t - 1)
	// u (t + 1) = (1 - udamp) * u (t) + a * (nsum - 4 * u (t)) + (1 - vdamp)
	//	* (u (t) - u (t - 1))
	//		= a * nsum + ((2 - udamp - vdamp) - 4 * a) * u (t) - (1 - vdamp) *
	//			u (t - 1);

	#if SHADER_MODEL >= 300
		ret.r = 0.14 * nsum + (1.96 - 0.56) * c.r - 0.98 * c.g;
	#else
		ret.r = 0.10 * nsum + (1.96 - 0.40) * c.r - 0.98 * c.g;
	#endif

	// if (abs (ret.r) < 0.01) ret.r = 0;
	// if (abs (ret.g) < 0.01) ret.g = 0;

	// calculate normal map
	ret.ba = 2 * (n.xy - n.zw) + (n2.xy - n2.zw);

	ret = 0.5f * ret + (0.5f).xxxx;

	return ret;
}

//-----------------------------------------------------------------------------

float4 PlayerWavePS (in float2 Tex : TEXCOORD0): COLOR0 {
	float4 ret = tex2D (WaveSampler, Tex);
	float wavesize =
		(1.0 + 0.055 * sin(16 * g_Time) + 0.065 * sin(12.87645 * g_Time))
		* playerWaveSize;
	ret.rg *= saturate(2 * abs(length (Tex - g_RippleOrigin) / wavesize - 1));
	return ret;
}

//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------


// Used to render the water from above
technique RenderWaterFromAbove {
	pass p0 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		CullMode = CW;
		FogEnable = false;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		#if SHADER_MODEL >= 300
			VertexShader = compile vs_3_0 WaterVS ();
			PixelShader = compile ps_3_0 ReflectionShader ();
		#else
			VertexShader = compile vs_2_0 WaterVS ();
			PixelShader = compile ps_2_0 ReflectionShader ();
		#endif
	}
}

//-----------------------------------------------------------------------------

// Used for rendering the water from underneath
technique RenderWaterFromBelow {
	pass p0 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		CullMode = CCW;
		FogEnable = false;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		#if SHADER_MODEL >= 300
			VertexShader = compile vs_3_0 WaterVS ();
			PixelShader = compile ps_3_0 UnderWaterPS ();
		#else
			VertexShader = compile vs_2_0 WaterVS ();
			PixelShader = compile ps_2_0 UnderWaterPS ();
		#endif
	}
}

//-----------------------------------------------------------------------------

// Used for rendering water in non - reflective mode
technique RenderWaterNoReflect {
	pass p0 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		CullMode = CW;
		FogEnable = false;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		#if SHADER_MODEL >= 300
			VertexShader = compile vs_3_0 WaterVS ();
			PixelShader = compile ps_3_0 NonReflectionShader ();
		#else
			VertexShader = compile vs_2_0 WaterVS ();
			PixelShader = compile ps_2_0 NonReflectionShader ();
		#endif
	}
}

//-----------------------------------------------------------------------------

// Used for rendering the underwater caustic lighting
technique RenderCaustics {
	pass p0 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 CausticsVS ();
		PixelShader = compile ps_2_0 CausticsPS ();
	}
}

//-----------------------------------------------------------------------------

// Used for calculating waves
technique CalculateWaves {
	pass p0 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 WaveVS ();
		PixelShader = compile ps_2_0 WavePS ();
	}
}

//-----------------------------------------------------------------------------

// Used for creating ripples around PC
technique CreatePCRipples {
	pass p0 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 WaveVS ();
		PixelShader = compile ps_2_0 PlayerWavePS ();
	}
}

//-----------------------------------------------------------------------------

