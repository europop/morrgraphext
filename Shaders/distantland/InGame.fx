// Original by Timeslip and LizTail, mods by harlanrm, phal and krzymar
// To be used with MGE 3.8.2 rev. 175 and derivatives
Matrix TexProj;
Matrix view;
Matrix proj;
Matrix world;
float4x4 MatrixPalette [4] : MATRIXPALETTE;
float3 SunVec;
float3 SunCol;
float3 EyePos;
float3 SunAmb;
float3 SkyCol;
float ticks;
float2 offset;
float TexBlend;
float2 rcpres;
int FogCol;
float3 FogCol2;
float FogStart;
float FogRange;
float BlendStart;
float BlendEnd;
float GrassDist;
float2 WindVec;
float PixelWidth;
float PixelHeight;
float3 SunPos;
float SunVis;
float3 PlayerPos;
float2 RippleOrigin;
float WaterLevel;
float AlphaMultiplier;
float CausticIntens;

texture tex0;
texture tex1;
texture tex2;
texture tex3;
texture tex4;
texture tex5;
sampler s0 = sampler_state { texture = <tex0>; minfilter = linear; magfilter = linear; mipfilter = linear; addressu = clamp; addressv = clamp; };
sampler s1 = sampler_state { texture = <tex1>; minfilter = linear; magfilter = linear; mipfilter = linear; addressu = wrap; addressv = wrap; addressw = wrap; };
sampler s2 = sampler_state { texture = <tex2>; minfilter = linear; magfilter = linear; mipfilter = linear; addressu = wrap; addressv = wrap; };
sampler s3 = sampler_state { texture = <tex3>; minfilter = linear; magfilter = linear; mipfilter = linear; addressu = wrap; addressv = wrap; };
sampler s4 = sampler_state { texture = <tex3>; minfilter = linear; magfilter = linear; mipfilter = linear; addressu = clamp; addressv = clamp; };

// For sampling the far water texture
sampler s5 = sampler_state { texture = <tex1>; minfilter = linear; magfilter = linear; mipfilter = none; addressu = wrap; addressv = wrap; };
sampler s6 = sampler_state { texture = <tex2>; minfilter = linear; magfilter = linear; mipfilter = none; addressu = wrap; addressv = wrap; };
sampler s7 = sampler_state { texture = <tex4>; minfilter = linear; magfilter = linear; mipfilter = linear; addressu = clamp; addressv = clamp; };
sampler s8 = sampler_state { texture = <tex5>; minfilter = linear; magfilter = linear; mipfilter = linear; bordercolor = 0x80808080 ; addressu = border; addressv = border; };

//------------------------------------------------------------

// water & dynamic ripples settings
#if WAVEHEIGHT > 0
	const bool fineWaterMesh = true;		// use tesselated circular water mesh? false uses simple mesh
#else
	const bool fineWaterMesh = false;	// use tesselated circular water mesh? false uses simple mesh
#endif

const int waveTexResolution = 512;		// texture resolution for rain ripples and player waves
const float waveTexWorldResolution = 2.50f;	// world size of each player wave texture pixel

float waveTexWorldSize;	// set by MGE
float waveTexRcpRes;	// set by MGE

static const float3 watercolour = { 0.4, 0.45, 0.5 };
static const float FLT_MAX = 340282346600000000000000000000000000000.0f;

static const float FogRangeMinusStart = FogRange - FogStart;

//------------------------------------------------------------

static const float _lightfactor = 1 - pow (1 - SunVis, 2);
static const float3 _depthcolor = _lightfactor * SunCol * float3 (0.03, 0.04, 0.05) + (2 * SkyCol + FogCol2) * float3 (0.075, 0.08, 0.085);
static const float3 SunCollf = SunCol * _lightfactor;
static const float cauststr = 0.0001f * CausticIntens * saturate (0.75 * _lightfactor + 0.35 * length (FogCol2));

#if SHADER_MODEL >= 300
	static const float _windfactor = length (WindVec) + 1.5;
	static const float TwiceWaterLevel = 2 * WaterLevel;
#endif

//------------------------------------------------------------

#ifdef EXPFOG
	static const float ExpFogStart = max (0, FogStart);
	static const float ExpFogMult = saturate (FogRange / FogRangeMinusStart);
	static const float ExpFogRangeMinusStart = FogRange - ExpFogStart;

	float3 skyfogblend (float z) {
		return lerp (FogCol2, SkyCol, 1 - pow (saturate (1 - 2.22 * saturate (z - 0.075)), 1.15));
	}

	float getFog (float dist) {
		return saturate (exp (-(dist - ExpFogStart) / ExpFogRangeMinusStart)) * ExpFogMult;
	}

	float3 applyfog (float3 col, float4 fog) {
		return lerp (fog.rgb, col, fog.a);
	}
#else
	float getFog (float dist) {
		return saturate ((FogRange - dist) / FogRangeMinusStart);
	}
#endif

//------------------------------------------------------------

float4 RenderShader (in float2 TexCoords : TEXCOORD0, in float3 Light : TEXCOORD1, in float4 Height : TEXCOORD2) : COLOR0 {
	clip (Height);
	float3 Result = tex2D (s3, TexCoords).rgb * Light;
	return float4 (Result, 1);
}

//------------------------------------------------------------

struct StatVertIn {
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR0;
	float2 texcoords : TEXCOORD0;
};

//------------------------------------------------------------

struct StatVertOut {
	float4 pos : POSITION;
	float2 texcoords : TEXCOORD0;
	float4 color : TEXCOORD1;
	float3 normal : TEXCOORD2;
	#ifdef EXPFOG
		float4 fog : TEXCOORD3;
	#else
		float fog : TEXCOORD3;
	#endif
	float blend : TEXCOORD5;
	float emissive : TEXCOORD6;
};

//------------------------------------------------------------

StatVertOut StaticVS (StatVertIn IN) {
	StatVertOut OUT;

	OUT.texcoords = IN.texcoords;

	// Define % to Fog
	float4 worldpos = mul (IN.pos, world);

	#ifdef EXPFOG
		float3 EyeVec = worldpos.xyz - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;
		OUT.fog = float4 (skyfogblend (EyeVec.z), getFog (dist));
	#else
		float dist = length (worldpos.xyz - EyePos.xyz);
		OUT.fog = getFog (dist);
	#endif

	// Define % to Blend
	OUT.blend = saturate ((BlendEnd - dist) / (BlendEnd - BlendStart));

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	// Lighting
	OUT.normal = normalize (IN.normal.xyz * 2 - 1); // Decompress
	OUT.normal = normalize (mul (OUT.normal.xyz, (float3x3)world)); // Rotate normals but don't translate them
	OUT.color = IN.color;
	OUT.emissive = IN.normal.w; // Emissive stored in 4th value in normal vector


	return OUT;
}

//------------------------------------------------------------

struct StatInstVertIn {
	float4 pos : POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR0;
	float2 texcoords : TEXCOORD0;
	float4 m1 : TEXCOORD1;
	float4 m2 : TEXCOORD2;
	float4 m3 : TEXCOORD3;
	float4 m4 : TEXCOORD4;
};

//------------------------------------------------------------

StatVertOut StaticInstVS (StatInstVertIn IN) {
	StatVertOut OUT;

	OUT.texcoords = IN.texcoords;

	// Pack instance transform
	float4x4 worldmat = float4x4 (IN.m1, IN.m2, IN.m3, IN.m4);

	// Define % to Fog
	float4 worldpos = mul (IN.pos, worldmat);

	#ifdef EXPFOG
		float3 EyeVec = worldpos.xyz - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;
		OUT.fog = float4 (skyfogblend (EyeVec.z), getFog (dist));
	#else
		float dist = length (worldpos.xyz - EyePos.xyz);
		OUT.fog = getFog (dist);
	#endif

	// Define % to Blend
	OUT.blend = saturate ((BlendEnd - dist) / (BlendEnd - BlendStart));

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	// Lighting
	OUT.normal = normalize (IN.normal.xyz * 2 - 1); // Decompress
	OUT.normal = normalize (mul (OUT.normal.xyz, (float3x3)world)); // Rotate normals but don't translate them
	OUT.color = IN.color;
	OUT.emissive = IN.normal.w; // Emissive stored in 4th value in normal vector

	return OUT;
}

//------------------------------------------------------------

float4 StaticPS (StatVertOut IN): COLOR0 {
	float4 Result = tex2D (s3, IN.texcoords);

	// Lighting
	float3 normal = normalize (IN.normal);
	float3 diffuse = saturate (dot (-SunVec, normal)) * SunCol * IN.color.rgb;
	float3 ambient = SunAmb * IN.color.rgb;
	Result.rgb *= saturate (diffuse + ambient + IN.emissive);

	// Fogging
	#ifdef EXPFOG
		Result.rgb = lerp (IN.fog.rgb, Result.rgb, IN.fog.a);
	#else
		Result.rgb = lerp (FogCol2, Result.rgb, IN.fog);
	#endif

	// Blending
	Result.a = saturate (IN.color.a * Result.a * AlphaMultiplier) * IN.blend;

	return Result;
}

//------------------------------------------------------------

struct StatRefVertOut {
	float4 pos : POSITION;
	float2 texcoords : TEXCOORD0;
	float4 color : TEXCOORD1;
	float3 normal : TEXCOORD2;
	float fog : TEXCOORD3;
	float height : TEXCOORD6;
};

//------------------------------------------------------------

StatRefVertOut StaticReflectionVS (StatVertIn IN) {
	StatRefVertOut OUT;

	OUT.texcoords = IN.texcoords;

	float4 worldpos = mul (IN.pos, world);

	// Define % to Fog
	#if SHADER_MODEL >= 300
		float3 EyeVec = float3 (worldpos.xy, TwiceWaterLevel - worldpos.z) - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;

		#ifdef EXPFOG
			OUT.fog = saturate (exp (-(max (0, worldpos.z) / (0.0001 + saturate (-EyeVec.z))) / ExpFogRangeMinusStart));
		#else
			OUT.fog = saturate (
				getFog (dist) / 
				(0.001 + saturate ((FogRange - (EyePos.z - WaterLevel) / (0.001 + saturate (-EyeVec.z))) / (FogRangeMinusStart)))
			);
		#endif
	#else
		float dist = length (worldpos.xyz - EyePos.xyz);
		OUT.fog = getFog (dist);
	#endif

	//// Define % to Blend
	//OUT.blend = saturate ((BlendEnd - dist) / (BlendEnd - BlendStart));

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	// Lighting
	OUT.normal = normalize (IN.normal.xyz * 2 - 1); // Decompress
	OUT.normal = normalize (mul (OUT.normal, (float3x3)world)); // Rotate normals but don't translate them
	OUT.color = IN.color;

	// Figure hight of pixel in world coordinates
	OUT.height = worldpos.z;

	return OUT;
}

//------------------------------------------------------------

StatRefVertOut StaticReflectionInstVS (StatInstVertIn IN) {
	StatRefVertOut OUT;

	OUT.texcoords = IN.texcoords;

	// Pack instance transform
	float4x4 worldmat = float4x4 (IN.m1, IN.m2, IN.m3, IN.m4);

	float4 worldpos = mul (IN.pos, worldmat);

	// Define % to Fog
	#if SHADER_MODEL >= 300
		float3 EyeVec = float3 (worldpos.xy, TwiceWaterLevel - worldpos.z) - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;

		#ifdef EXPFOG
			OUT.fog = saturate (exp (-(max (0, worldpos.z) / (0.0001 + saturate (-EyeVec.z))) / ExpFogRangeMinusStart));
		#else
			OUT.fog = saturate (
				getFog (dist) / 
				(0.001 + saturate ((FogRange - (EyePos.z - WaterLevel) / (0.001 + saturate (-EyeVec.z))) / (FogRangeMinusStart)))
			);
		#endif
	#else
		float dist = length (worldpos.xyz - EyePos.xyz);
		OUT.fog = getFog (dist);
	#endif

	//// Define % to Blend
	//OUT.blend = saturate ((BlendEnd - dist) / (BlendEnd - BlendStart));

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	// Lighting
	OUT.normal = normalize (mul (IN.normal.xyz, (float3x3)worldmat));
	OUT.color = IN.color;

	// Figure height of pixel in world coordinates
	OUT.height = worldpos.z;

	return OUT;
}

//------------------------------------------------------------

float4 StaticReflectionPS (StatRefVertOut IN): COLOR0 {
	// Clip to water plane
	clip (IN.height + 4 - WaterLevel);
	clip (IN.fog - 0.001);

	float4 Result = tex2D (s3, IN.texcoords);
	clip (Result.a - 0.5);

	// Lighting
	float3 normal = normalize (IN.normal);
	Result.rgb *= saturate ((saturate (dot (-SunVec, normal)) * SunCol + SunAmb) * IN.color.rgb);

	// Fogging
	Result.rgb = lerp (FogCol2, Result.rgb, IN.fog);

	return float4 (Result.rgb, 1);
}

//------------------------------------------------------------

struct LowestLandVertOut {
	float4 position : POSITION;
	float4 pos : TEXCOORD0;
	float2 texcoords : TEXCOORD1;
	float3 light : COLOR0;
};

//------------------------------------------------------------

LowestLandVertOut LowestLandVS (in float4 pos : POSITION) {
	LowestLandVertOut OUT;

	OUT.position = mul (pos, world);
	OUT.position = mul (OUT.position, view);
	OUT.position = mul (OUT.position, proj);

	OUT.pos = float4 (pos.xy - offset, pos.zw);

	// Calculate texture coordinates
	OUT.texcoords = OUT.pos.xy / 600;

	// Lighting
	OUT.light = saturate(-SunVec.z * SunCol);
	OUT.light = saturate (OUT.light + SunAmb);

	return OUT;
}

//------------------------------------------------------------

float4 LowestLandPS (in LowestLandVertOut IN): COLOR0 {
	// Define % to Fog
	float dist = length (IN.pos.xyz - EyePos.xyz);

	float3 Result = tex2D (s3, IN.texcoords).rgb;
	Result *= IN.light;

	// Fogging
	float fog;
	fog = getFog (dist);
	Result = lerp (FogCol2, Result, fog);

	return float4 (Result, 1);
}

//------------------------------------------------------------

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

//------------------------------------------------------------

WaterVertOut WaterVS (in float4 pos : POSITION) {
	WaterVertOut OUT;

	OUT.pos = float4 (pos.xy - offset, pos.zw);

	// Calculate various texture coordinates
	OUT.texcoord1 = OUT.pos.xy / 3900;
	OUT.texcoord2 = OUT.pos.xy / 1104;
	OUT.texcoord3 = OUT.pos.xy / 892;

	#if WAVEHEIGHT > 0
		float time = ticks / 2;

		float height = tex3Dlod (s1, float4 (OUT.texcoord2, time, 0)).a;
		float height2 = tex3Dlod (s1, float4 (OUT.texcoord1, time, 0)).a;

		float dist = length (EyePos.xyz - OUT.pos.xyz);

		float addheight = WAVEHEIGHT * (lerp (height, height2, saturate (dist / 8000)) - 0.5f) * saturate (1 - dist / 6400) * saturate (dist / 200);

		float4 newPos = pos + float4 (0, 0, addheight, 0);

		OUT.pos.zw = newPos.zw;

		OUT.position = mul (newPos, world);
		OUT.position = mul (OUT.position, view);
		OUT.position = mul (OUT.position, proj);
		OUT.w = OUT.position.w;

		OUT.screenpos = mul (newPos , TexProj);
		newPos = pos - float4 (0, 0, abs (addheight), 0);
		OUT.screenposclamp = mul (newPos , TexProj);
	#else
		OUT.position = mul (pos, world);
		OUT.position = mul (OUT.position, view);
		OUT.position = mul (OUT.position, proj);
		OUT.w = OUT.position.w;

		OUT.screenpos = mul (pos, TexProj);
	#endif

	return OUT;
}

//------------------------------------------------------------

float3 GetFinalWaterNormal (float2 texcoord1, float2 texcoord2, float2 texcoord3, float dist, float2 IN_pos_xy) : NORMAL {
	// Calculate the W texture coordinate based on the time that has passed
	float time = ticks / 2;
	float3 tex1 = float3(texcoord1, time);
	float3 tex2 = float3(texcoord2, time);
	
	// Blend together the normals from different sized areas of the same texture.
	float2 far_normal = tex3D (s5, tex1).rg;
	float2 close_normal = tex3D (s5, tex2).rg;
	#ifdef MGE_DYNAMICRIPPLES
		// add ripples
		#if SHADER_MODEL >= 300
			close_normal.rg += tex2D (s2, texcoord3).ba; // rain
			close_normal.rg += tex2D (s8, (IN_pos_xy - RippleOrigin) / waveTexWorldSize).ba * 2 - 1.5; // player
		#else
			close_normal.rg += (tex2D (s2, texcoord3).ba // rain
			 + tex2D (s8, (IN_pos_xy - RippleOrigin) / waveTexWorldSize).ba) * 2 - 2; // player
		#endif
	#endif
	float2 normal_R = lerp (close_normal, far_normal, saturate (dist / 8000)) * 2 - 1;
	return normalize(float3(normal_R,1));
}

//------------------------------------------------------------

float4 ReflectionShader (in WaterVertOut IN): COLOR0 {
	// Calculate eye vector
	float3 EyeVec = EyePos.xyz - IN.pos.xyz;
	float dist = length (EyeVec);
	EyeVec /= dist;

	// Define % to Fog
	float fog = getFog (dist);
	
	// Calculate water normal
	float3 normal = GetFinalWaterNormal (IN.texcoord1, IN.texcoord2, IN.texcoord3, dist, IN.pos.xy);

	// reflection / refraction strength factor, wind strength increases distortion
	#if SHADER_MODEL >= 300
		float2 reffactor = ((dist / 130 * _windfactor) + 0.1) * (normal.xy);
	#else
		float2 reffactor = (dist / 50) * (normal.xy);
	#endif

	// Distort refraction dependent on depth
	float4 newscrpos = IN.screenpos + float4 (reffactor.yx, 0, 0);

	// Sample depth texture
	#if SHADER_MODEL >= 300
		float depth = max (0, tex2Dproj (s7, newscrpos).r - IN.w);
	#else
		float depth = tex2Dproj (s7, newscrpos).r - IN.w;
	#endif
	newscrpos = IN.screenpos + saturate (depth / 100) * float4 (reffactor.yx, 0, 0);

	// Sample refraction texture
	float3 Refract = tex2Dproj (s4, newscrpos).rgb;

	#if SHADER_MODEL >= 300
		// Get distorted depth
		depth = max (0, tex2Dproj (s7, newscrpos).r - IN.w);
		depth /= dot (-EyeVec, float3 (view [0][2], view [1][2], view [2][2]));

		float depthscale = saturate (exp (-(depth) / 800));
		float shorefactor = pow (depthscale, 25);

		float3 depthcolor = lerp (FogCol2, _depthcolor, fog);

		// Make transition between actual refraction image and depth color depending on water depth
		Refract = lerp (depthcolor, Refract, 0.8 * depthscale + 0.2 * shorefactor);

		// Sample reflection texture
		float3 Reflect = {0, 0, 0};

		// float totalalpha;

		if (EyePos.z < WaterLevel - 4) {
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
				 - float4 (2.5 * reffactor.x, -abs (reffactor.y), 0, 0) + 0.0015 * IN.w * float4 (0, 1, 0, 0);

				// unfolded blurring allows for better optimization
				float4 shift = blurwidth * float4 (1, 0, 0, 0);
				Reflect += 0.100 * (tex2Dproj (s0, scrpos + shift).rgb
				 + tex2Dproj (s0, scrpos - shift).rgb);
				shift.xy = blurwidth * float2 (0.67, 0.33);
				Reflect += 0.115 * (tex2Dproj (s0, scrpos + shift).rgb
				 + tex2Dproj (s0, scrpos - shift).rgb);
				shift.xy = blurwidth * float2 (0.33, -0.33);
				Reflect += 0.135 * (tex2Dproj (s0, scrpos + shift).rgb
				 + tex2Dproj (s0, scrpos - shift).rgb);
				shift.xy = float2 (0.0, blurwidth * 0.67);
				Reflect += 0.150 * (tex2Dproj (s0, scrpos + shift).rgb
				 + tex2Dproj (s0, scrpos - shift).rgb);
			#else
				Reflect = tex2Dproj (s0, 
				#if WAVEHEIGHT > 0
					IN.screenposclamp
				#else
					IN.screenpos
				#endif
				- float4 (2.5 * reffactor.x, -abs (reffactor.y), 0, 0) + 0.0015 * IN.w * float4 (0, 1, 0, 0));
			#endif
		}

		Reflect = lerp (FogCol2, Reflect, fog);

		float3 adjustnormal = lerp (float3 (0, 0, 0.1), normal, pow (saturate (1.05 * fog), 2));
		adjustnormal = lerp (adjustnormal, float3 (0, 0, 1.0), (1 - EyeVec.z) * (1 - saturate (1 / (dist / 1000 + 1))));

		float fresnel = dot (EyeVec, adjustnormal);
		fresnel = 0.02 + pow (saturate (0.9988 - 0.28 * fresnel), 16);
		float3 Result = lerp (Refract, Reflect, fresnel);

		// Specular lighting
		float reflection = saturate (1.0025 * dot (EyeVec, reflect (-SunPos, normal)));
		float3 spec = SunCollf * (pow (reflection, 150) + 0.07 * pow (reflection, 4)) * fog;

		// Result = 1500 / (1500 + tex2Dproj (s7, IN.screenpos).r) ;

		// smooth transition at shore line
		Result = lerp (Result + spec, Refract, shorefactor * fog);
	#else
		// Make transition between actual refraction image and depth color depending on water depth
		Refract = lerp (Refract, _depthcolor, saturate (depth / 1500 + 0.3));

		float3 Reflect = tex2Dproj (s0, IN.screenpos - float4 (2.5 * reffactor.x, -abs (reffactor.y), 0, 0)) / 2;

		float fresnel = saturate (dot (EyeVec, float3 (0, 0, 1)) * 1.5);
		float3 Result = lerp (Reflect, Refract, fresnel);

		//Specular lighting
		float reflection = saturate (dot (EyeVec, reflect (-SunPos, normal)));
		float3 spec = SunCollf * pow (reflection, 150);

		// Additional Specular
		spec += pow (dot (normal, float3 (0, 0, 1)), 100) * SunAmb / 12; // gives water a shiny look

		// smooth transition at shore line
		Result += spec;

		Result = lerp (FogCol2, Result, fog);
	#endif

	return float4 (Result, 1);
}

//------------------------------------------------------------

float4 NonReflectionShader (in WaterVertOut IN): COLOR0 {
	// Calculate eye vector
	float3 EyeVec = (EyePos - IN.pos.xyz);
	float dist = length (EyeVec);
	EyeVec /= dist;

	// Define % to Fog
	//float fog = saturate ((FogRange - dist) / FogRangeMinusStart);
	float fog = getFog (dist);
	
	// Calculate water normal
	float3 normal = GetFinalWaterNormal (IN.texcoord1, IN.texcoord2, IN.texcoord3, dist, IN.pos.xy);

	// reflection / refraction strength factor, wind strength increases distortion
	#if SHADER_MODEL >= 300
		float2 reffactor = ((dist / 130 * _windfactor) + 0.1) * (normal.xy);
	#else
		float2 reffactor = (dist / 50) * (normal.xy);
	#endif

	// Distort refraction dependent on depth
	float4 newscrpos = IN.screenpos + float4 (reffactor.yx, 0, 0);

	// Sample depth texture
	float depth = max (0, tex2Dproj (s7, newscrpos).r - IN.w);
	newscrpos = IN.screenpos + saturate (depth / 100) * float4 (reffactor.yx, 0, 0);

	// Sample refraction texture
	float3 Refract = tex2Dproj (s4, newscrpos).rgb;

	#if SHADER_MODEL >= 300
		// Get distorted depth
		depth = max (0, tex2Dproj (s7, newscrpos).r - IN.w);
		depth /= dot (-EyeVec, float3 (view [0][2], view [1][2], view [2][2]));

		float depthscale = saturate (exp (-depth / 800));
		float shorefactor = pow (depthscale, 25);

		// Make transition between actual refraction image and depth color depending on water depth
		Refract = lerp (_depthcolor, Refract, 0.8 * depthscale + 0.2 * shorefactor);

		float fresnel = dot (EyeVec, lerp (normal, float3 (0, 0, 1), (1 - EyeVec.z) * (1 - exp (-dist / 2500)))) * fog;
		fresnel = 0.02 + pow (saturate (0.9988 - 0.28 * fresnel), 16);

		float3 Reflect = (FogCol2 + SkyCol) * 0.4;
		if (SunVec.z > 0.9) {
			Reflect = FogCol2 * 0.55;
		}
		Reflect = lerp (FogCol2, Reflect, fog);

		float3 Result = lerp (Refract, Reflect, fresnel);

		// Specular lighting
		float reflection = saturate (1.0025 * dot (EyeVec, reflect (-SunPos, normal)));
		float3 spec = SunCollf * (pow (reflection, 150) + 0.07 * pow (reflection, 4)) * fog;

		Result = lerp (Result + spec, Refract, shorefactor * fog);
	#else
		// Make transition between actual refraction image and depth color depending on water depth
		Refract = lerp (Refract, _depthcolor, saturate (depth / 1500 + 0.3));

		float fresnel = saturate(dot(EyeVec,float3(0,0,1)));

		float3 Reflect = SkyCol / 2;

		float3 Result = lerp (Reflect, Refract, saturate(fresnel*(depth+2)));

		//Specular lighting
		float reflection = saturate (1.0025 * dot (EyeVec, reflect (-SunPos, normal)));
		float3 spec = SunCollf * pow (reflection, 150);

		// Additional Specular
		spec += pow (dot (normal, float3 (0, 0, 1)), 100) * SunAmb / 12; // gives water a shiny look

		Result += spec;

		//Fogging
		Result = lerp (FogCol2, Result, fog);
	#endif

	return float4 (Result, 1);
}

//------------------------------------------------------------

float4 UnderWaterPS (in WaterVertOut IN): COLOR0 {
	// Calculate eye vector
	float3 EyeVec = (EyePos - IN.pos.xyz);
	float dist = length (EyeVec);
	EyeVec /= dist;

	// Calculate water normal
	float3 normal = -GetFinalWaterNormal (IN.texcoord1, IN.texcoord2, IN.texcoord3, dist, IN.pos.xy);

	// Define % to Fog
	// #ifdef EXPFOG
	// 	float fog = exp (-2 * dist / FogRange);
	// #else
	// 	float fog = saturate (1 - (dist / FogRange));
	// 	fog *= fog;
	// #endif
	float fog = getFog (dist);

	#if SHADER_MODEL >= 300
		// reflection / refraction strength factor, wind strength increases distortion
		// float2 reffactor = -((dist / 20 * (_windfactor)) + 0.25) * float2 (
		//	dot (normal, float3 (view [0][0], view [1][0], view [2][0])), 
		//	dot (normal, float3 (view [0][1], view [1][1], view [2][1]))
		// );
		float2 reffactor = ((dist / 65 * _windfactor) + 0.03) * (normal.xy);
	#else
		float2 reffactor = dist / 20 * normal.xy;
	#endif

	// Sample depth texture
	float depth = max (0, tex2Dproj (s7, IN.screenpos + float4 (reffactor.xy, 0, 0)).r - IN.w);
	float depthfactor = saturate (depth / 150);

	#if SHADER_MODEL >= 300
		depth = max (0, tex2Dproj (s7, IN.screenpos + depthfactor * float4 (reffactor.xy, 0, 0)).r - IN.w);
	#else
		depth = tex2Dproj (s7, IN.screenpos + depthfactor * float4 (reffactor.xy, 0, 0)).r - IN.w;
	#endif

	// Sample refraction texture
	float3 Refract = tex2Dproj (s4, IN.screenpos + depthfactor * float4 (reffactor.xy, 0, 0)).rgb;

	depthfactor = saturate (depth / 150);

	// Specular lighting
	float reflection = saturate (1.005 * dot (EyeVec, reflect (float3 (-SunPos.x, -SunPos.y, SunPos.z), normal)));
	float3 Specular = saturate (1.5 * SunCollf) * (pow (reflection, 120) + 0.07 * pow (reflection, 2));

	#if SHADER_MODEL >= 300
		Refract += Specular * depthfactor * fog;
	#else
		Refract += Specular;
	#endif

	// fresnel factor
	// float fresnel = saturate (dot (EyeVec, normal / float3 ((1.0 + 0.5 * normal.z).xx, 1)));
	float fresnel = saturate (dot (EyeVec, normal));
	fresnel = 0.4 * pow (saturate (1.3 - fresnel), 8) * fog;

	#if SHADER_MODEL >= 300
		if (SunVec.z > 0.9) fresnel *= 0.6;
	#endif

	float3 Result = Refract * (1 - fresnel * depthfactor);

	// Realistic Fresnel
	// fresnel = saturate (0.015 / max (0.01, fresnel - 0.645) - 0.02);
	// Refract = tex2Dproj (s4, IN.screenpos + float4 (reffactor.xy, 0, 0) / (1.001 - fresnel)).rgb;
	// float3 Result = lerp (Refract, FogCol2, fresnel);

	return float4 (Result, 1);
}

//------------------------------------------------------------

struct LandVertOut {
	float4 pos: POSITION;
	float2 texcoord: TEXCOORD1;
	float3 position: TEXCOORD2;
	#ifdef EXPFOG
		float4 fog : TEXCOORD3;
	#else
		float fog : TEXCOORD3;
	#endif
};

//------------------------------------------------------------

LandVertOut LandscapeVertexShader (float4 pos: POSITION, float2 texcoord: TEXCOORD0) {
	LandVertOut OUT;

	// Define % to Fog
	#ifdef EXPFOG
		float3 EyeVec = pos.xyz - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;
		OUT.fog = float4 (skyfogblend (EyeVec.z), getFog (dist));
	#else
		float dist = length (pos.xyz - EyePos.xyz);
		OUT.fog = getFog (dist);
	#endif

	// Lower vertices that are close to the camera to ensure they are always below it
	float disp = lerp (200, 0, saturate (dist / 2000));
	pos.z -= disp;

	// OUT.pos = mul (pos, worldviewproj);
	OUT.pos = mul (pos, world);
	OUT.pos = mul (OUT.pos, view);
	OUT.pos = mul (OUT.pos, proj);

	OUT.texcoord = texcoord;
	OUT.position = pos.xyz;

	return OUT;
}

//------------------------------------------------------------

float4 LandscapePixelShader (in LandVertOut IN): COLOR0 {
	// Normalize incoming normal
	float3 normal = normalize (tex2D (s1, IN.texcoord).rgb * 2 - 1);

	// World Texture
	float3 Result = tex2D (s3, IN.texcoord).rgb;

	// Detail Texture
	float detail = tex2D (s2, IN.texcoord * 1064).g + 0.5;
	detail *= tex2D (s2, IN.texcoord * 333).g + 0.5;
	detail *= tex2D (s2, IN.texcoord * 90).g + 0.5;

	// Lighting
	Result *= detail * saturate ((saturate (dot (-SunVec, normal)) * SunCol) + SunAmb);

	// Define % to Fog
	#ifdef EXPFOG
		Result = lerp (IN.fog.rgb, Result, IN.fog.a);
		return float4 (Result, 1 - IN.fog.a);
	#else
		Result = lerp (FogCol2, Result, IN.fog);
		return float4 (Result, 1 - IN.fog);
	#endif
}

//------------------------------------------------------------

struct LandRefVertOut {
	float4 pos: POSITION;
	float2 texcoord: TEXCOORD1;
	float fog : TEXCOORD2;
	float height : TEXCOORD3;
};

//------------------------------------------------------------

LandRefVertOut LandscapeRefVS (float4 pos: POSITION, float2 texcoord: TEXCOORD0) {
	LandRefVertOut OUT;

	#if SHADER_MODEL >= 300
		//// Lower vertices so there are fewer seams visible in the reflection
		// pos.z -= 20 * saturate (1 - (pos.z / 100));
		float3 EyeVec = float3 (pos.xy, TwiceWaterLevel - pos.z) - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;

		// Define % to Fog
		#ifdef EXPFOG
			OUT.fog = saturate (exp ( -(max (0, pos.z) / (0.0001 + saturate (-EyeVec.z))) / ExpFogRangeMinusStart));
		#else
			OUT.fog = saturate (
				getFog (dist) / 
				(0.001 + saturate ((FogRange - (EyePos.z - WaterLevel) / (0.001 + saturate (-EyeVec.z))) / FogRangeMinusStart))
			);
		#endif
	#else
		// Lower vertices so there are fewer seams visible in the reflection
		// pos.z -= 100;

		// Define % to Fog
		OUT.fog = getFog (length (pos.xyz - EyePos.xyz));
	#endif

	// OUT.pos = mul (pos, worldviewproj);
	OUT.pos = mul (pos, world);
	OUT.pos = mul (OUT.pos, view);
	OUT.pos = mul (OUT.pos, proj);
	OUT.texcoord = texcoord;

	// Define height of pixel for clipping to water plane
	OUT.height = pos.z;

	return OUT;
}

//------------------------------------------------------------

float4 LandscapeRefPS (in LandRefVertOut IN): COLOR0 {
	// Clip to water plane
	clip (IN.height + 4 - WaterLevel);
	clip (IN.fog - 0.001);

	// Normalize incoming normal
	float3 normal = normalize (tex2D (s1, IN.texcoord).rgb * 2 - 1);

	// World Texture
	float3 Result = tex2D (s3, IN.texcoord).rgb;

	// Detail Texture
	float detail = tex2D (s2, IN.texcoord * 1064).g + 0.5;
	detail *= tex2D (s2, IN.texcoord * 333).g + 0.5;
	detail *= tex2D (s2, IN.texcoord * 90).g + 0.5;

	// Lighting
	Result *= detail * saturate ((saturate (dot (-SunVec, normal)) * SunCol) + SunAmb);

	// let reflections fade to water color

	Result.rgb = lerp (FogCol2, Result.rgb, IN.fog);

	//return float4 (Result, saturate (3.5 * IN.fog));
	return float4 (Result, 1);
}

//------------------------------------------------------------

struct GrassVertIn {
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR0;
	float2 texcoords : TEXCOORD0;
};

//------------------------------------------------------------

struct GrassVertOut {
	float4 pos : POSITION;
	float2 texcoords : TEXCOORD0;
	float3 color : COLOR0;
	#ifdef EXPFOG
		float4 fog : TEXCOORD1;
	#else
		float fog : TEXCOORD1;
	#endif
	float4 screenpos: TEXCOORD4;
	float blend : TEXCOORD5;
};

//------------------------------------------------------------

GrassVertOut GrassVS (GrassVertIn IN) {
	GrassVertOut OUT;

	OUT.texcoords = IN.texcoords;

	// Animate grass
	float4 worldpos = mul (IN.pos, world);

	float height = clamp (IN.pos.z, 0, 100) / 100;

	float2 wind = WindVec * 2.5 + 0.25;

	float2 pos_ticks1 = worldpos.xy / 1000 + ticks;
	float2 pos_ticks2 = worldpos.xy / 750 + ticks * 2;
	float2 pos_ticks3 = worldpos.xy / 500 + ticks * 3;
	float2 pos_ticks4 = worldpos.xy / 200 + ticks * 4;

	worldpos.xy += (sin (pos_ticks1) + cos (pos_ticks2) + sin (pos_ticks3) + cos (pos_ticks4)) * height * wind * 10;

	// Define % to Fog
	#ifdef EXPFOG
		float3 EyeVec = worldpos.xyz - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;
		OUT.fog = float4 (skyfogblend (EyeVec.z), getFog (dist));
	#else
		float dist = length (worldpos.xyz - EyePos.xyz);
		OUT.fog = getFog (dist);
	#endif

	// Define % to Blend
	OUT.blend = saturate ((BlendEnd - dist) / (BlendEnd - BlendStart));

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	// Texture Projection
	OUT.screenpos = mul (worldpos, TexProj);

	// Lighting
	OUT.color = (SunCol * 0.25) + SunAmb;

	return OUT;
}

//------------------------------------------------------------

GrassVertOut GrassInstVS (StatInstVertIn IN) {
	GrassVertOut OUT;

	// Pack instance transform
	float4x4 worldmat = float4x4 (IN.m1, IN.m2, IN.m3, IN.m4);

	OUT.texcoords = IN.texcoords;

	// Define % to Fog
	float4 worldpos = mul (IN.pos, worldmat);

	#ifdef EXPFOG
		float3 EyeVec = worldpos.xyz - EyePos.xyz;
		float dist = length (EyeVec);
		EyeVec /= dist;
		OUT.fog = float4 (skyfogblend (EyeVec.z), getFog (dist));
	#else
		float dist = length (worldpos.xyz - EyePos.xyz);
		OUT.fog = getFog (dist);
	#endif

	OUT.blend = 1 - (saturate ((GrassDist - dist) / (GrassDist / 5)));
	OUT.blend += 0.4;
	OUT.screenpos = 0;

	// Animate grass
	float height = clamp (IN.pos.z, 0, 100) / 100;

	float2 wind = WindVec / 6;

	float2 pos_ticks1 = worldpos.xy / 1000 + ticks;
	float2 pos_ticks2 = worldpos.xy / 750 + ticks * 2;
	float2 pos_ticks3 = worldpos.xy / 500 + ticks * 3;
	float2 pos_ticks4 = worldpos.xy / 250 + ticks * 4;

	worldpos.xy += (sin (pos_ticks1) + cos (pos_ticks2) + sin (pos_ticks3) + cos (pos_ticks4)) * height * wind * 10;

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	// Lighting
	OUT.color = (SunCol * 0.25) + SunAmb;

	return OUT;
}

//------------------------------------------------------------

float4 GrassPS (GrassVertOut IN): COLOR0 {
	float4 Result = tex2D (s3, IN.texcoords);
	// clip (Result.a - 0.4);

	// Lighting
	Result.rgb *= IN.color;

	// Fogging
	#ifdef EXPFOG
		Result.rgb = lerp (IN.fog.rgb, Result.rgb, IN.fog.a);
	#else
		Result.rgb = lerp (FogCol2, Result.rgb, IN.fog);
	#endif


	// Blending
	Result.a = saturate (Result.a * AlphaMultiplier) * IN.blend;
	return float4 (Result.rgb, Result.a);
}

//------------------------------------------------------------

struct BlendDepthVertIn {
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
};

//------------------------------------------------------------

struct ScreenVertOut {
	float4 pos: POSITION;
	float2 texcoord : TEXCOORD0;
};

//------------------------------------------------------------

ScreenVertOut ScreenQuadVS (in BlendDepthVertIn IN) {
	ScreenVertOut OUT;

	OUT.pos = mul (IN.pos, world);
	OUT.pos = mul (OUT.pos, view);
	OUT.pos = mul (OUT.pos, proj);

	OUT.texcoord = IN.texcoord;

	return OUT;
}

//------------------------------------------------------------

float4 BlendDepthPS (ScreenVertOut IN): COLOR0 {
	// return float4 (tex2D (s0, IN.texcoord).rgb, 1);

	float3 color = tex2D (s4, IN.texcoord).rgb;
	float depth = tex2D (s0, IN.texcoord).r;
	depth = min( tex2D (s0, IN.texcoord + float2(PixelWidth, 0)).r, depth);
	depth = min( tex2D (s0, IN.texcoord + float2(-PixelWidth, 0)).r, depth);
	depth = min( tex2D (s0, IN.texcoord + float2(0, PixelHeight)).r, depth);
	depth = min( tex2D (s0, IN.texcoord + float2(0, -PixelHeight)).r, depth);

	//// Display depth buffer for testing
	// depth = 1 - saturate (depth / FogRange);
	// return float4 (depth, depth, depth, 1);

	// Define % to Blend
	float blend = saturate ((BlendEnd - depth) / (BlendEnd - BlendStart));

	//// Display blend for testing
	//return float4( blend, blend, blend, 1 );

	return float4 (color, 1 - blend);
}

//------------------------------------------------------------

float4 ClearDepthPS (ScreenVertOut IN): COLOR0 {
	return float4 (FLT_MAX / 2, 1, 1, 1);
}

//------------------------------------------------------------

// float blur_weights [4] = { 0.47442968, 0.23392642, 0.02804153, 0.00081723 };

float blur_weights [7] = { 0.199471, 0.176033, 0.120985, 0.064759, 0.026995, 0.008764, 0.002216 };

float2 TwelveKernel [12];

//------------------------------------------------------------

float4 HorizontalBlurPS (ScreenVertOut IN): COLOR0 {
	float4 Original = tex2D (s4, IN.texcoord);
	float3 Blurred = 0;
	float total_weight = 0;

	// Early out to prevent night sky blur - tetchy
	if (Original.a >= 0.9999) {
		return Original;
	} else {

		for (int i = 0; i < 12; ++i) {
			float4 Current = tex2D (s4, IN.texcoord + TwelveKernel [i]);
			float weight = saturate (Original.a * Current.a);
			Blurred += Current.rgb * weight;
			total_weight += weight;
		}
		Blurred = (Blurred + Original.rgb) / (total_weight + 1.0);

		return float4 (Blurred, Original.a);
	}
}

//------------------------------------------------------------

float4 VerticalBlurPS (ScreenVertOut IN): COLOR0 {
	float4 orig_color = tex2D (s4, IN.texcoord);

	// return float4 (orig_color.a, orig_color.a, orig_color.a, orig_color.a);

	// Early out to prevent night sky blur - tetchy
	if (orig_color.a >= 0.9999) {
		return orig_color;
	} else {

		float2 offset = float2 (0, PixelHeight);

		float4 color = 0;

		color += tex2D (s4, IN.texcoord - offset * 6) * blur_weights [6];
		color += tex2D (s4, IN.texcoord - offset * 5) * blur_weights [5];
		color += tex2D (s4, IN.texcoord - offset * 4) * blur_weights [4];
		color += tex2D (s4, IN.texcoord - offset * 3) * blur_weights [3];
		color += tex2D (s4, IN.texcoord - offset * 2) * blur_weights [2];
		color += tex2D (s4, IN.texcoord - offset) * blur_weights [1];
		color += orig_color * blur_weights [0];
		color += tex2D (s4, IN.texcoord + offset) * blur_weights [1];
		color += tex2D (s4, IN.texcoord + offset * 2) * blur_weights [2];
		color += tex2D (s4, IN.texcoord + offset * 3) * blur_weights [3];
		color += tex2D (s4, IN.texcoord + offset * 4) * blur_weights [4];
		color += tex2D (s4, IN.texcoord + offset * 5) * blur_weights [5];
		color += tex2D (s4, IN.texcoord + offset * 6) * blur_weights [6];

		float blur = saturate (color.a * orig_color.a);

	//// Don't let the sky get too blurry// test no longer needed - tetchy
	// if (orig_color.a <= 0.0001) {
	// blur = max (0.75, blur);
	// }

		color.rgb = lerp (color.rgb, orig_color.rgb, blur);

		return float4 (color.rgb, orig_color.a);
	}
}

//------------------------------------------------------------

float4 CopyAndSetAlphaPS (ScreenVertOut IN): COLOR0 {
	float3 color = tex2D (s4, IN.texcoord).rgb;

	return float4 (color, 1);
}

//------------------------------------------------------------

struct RenderDepthVertOut {
	float4 pos: POSITION;
	float4 position: TEXCOORD0;
	// float4 col: COLOR0;
	float2 texcoords: TEXCOORD1;
};

//------------------------------------------------------------

RenderDepthVertOut GrassDepthVS (GrassVertIn IN) {
	RenderDepthVertOut OUT;

	OUT.texcoords = IN.texcoords;

	// Animate grass
	float4 worldpos = mul (IN.pos, world);

	float height = clamp (IN.pos.z, 0, 100) / 100;

	float2 wind = WindVec * 2.5 + 0.25;

	float2 pos_ticks1 = worldpos.xy / 1000 + ticks;
	float2 pos_ticks2 = worldpos.xy / 750 + ticks * 2;
	float2 pos_ticks3 = worldpos.xy / 500 + ticks * 3;
	float2 pos_ticks4 = worldpos.xy / 200 + ticks * 4;

	worldpos.xy += (sin (pos_ticks1) + cos (pos_ticks2) + sin (pos_ticks3) + cos (pos_ticks4)) * height * wind * 10;

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	OUT.position = OUT.pos;

	return OUT;
}

//------------------------------------------------------------

float4 GrassDepthPS (RenderDepthVertOut IN): COLOR0 {
	// Apply alpha testing
	float alpha = tex2D (s3, IN.texcoords).a;
	clip (alpha - 0.5);

	return float4 (IN.position.w, 1, 1, 1);
}

//------------------------------------------------------------

RenderDepthVertOut GrassDepthInstVS (StatInstVertIn IN) {
	RenderDepthVertOut OUT;

	// Pack instance transform
	float4x4 worldmat = float4x4 (IN.m1, IN.m2, IN.m3, IN.m4);

	OUT.texcoords = IN.texcoords;

	float4 worldpos = mul (IN.pos, worldmat);

	// Animate grass
	float height = clamp (IN.pos.z, 0, 100) / 100;

	float2 wind = WindVec / 6;

	float2 pos_ticks1 = worldpos.xy / 1000 + ticks;
	float2 pos_ticks2 = worldpos.xy / 750 + ticks * 2;
	float2 pos_ticks3 = worldpos.xy / 500 + ticks * 3;
	float2 pos_ticks4 = worldpos.xy / 250 + ticks * 4;

	worldpos.xy += (sin (pos_ticks1) + cos (pos_ticks2) + sin (pos_ticks3) + cos (pos_ticks4)) * height * wind * 10;

	// Projection
	OUT.pos = mul (worldpos, view);
	OUT.pos = mul (OUT.pos, proj);

	OUT.position = OUT.pos;

	return OUT;
}

//------------------------------------------------------------

struct CausticsVertOut {
	float4 pos: POSITION;
	float2 tex : TEXCOORD0;
};

//------------------------------------------------------------

CausticsVertOut CausticsVS (in BlendDepthVertIn IN) {
	CausticsVertOut OUT;
	OUT.pos = mul (IN.pos, proj);
	OUT.tex = IN.texcoord;
	return OUT;
}

//------------------------------------------------------------

float4 CausticsPS (in CausticsVertOut IN): COLOR0 {
//#if SHADER_MODEL >= 300
	float3 color = tex2D(s4, IN.tex);
	
	float depth = tex2D( s7, IN.tex ).r;
	float fog = getFog(depth);
	float blend = saturate( (BlendEnd - depth) / (BlendEnd - BlendStart) );
	
	float3 eyevec = float3(view[0][2],view[1][2],view[2][2]);
	eyevec += (1/proj[0][0] * (2*IN.tex.x-1)).xxx * float3(view[0][0],view[1][0],view[2][0]);
	eyevec += (-1/proj[1][1] * (2*IN.tex.y-1)).xxx * float3(view[0][1],view[1][1],view[2][1]);
	
	float3 uwpos = EyePos + eyevec * depth;
	uwpos.z -= WaterLevel;
	float sunraypath = (uwpos.z / SunVec.z);
	float caust = tex3D (s5, float3( (uwpos - SunVec * sunraypath).xy / 1104, ticks/2) ).b;
	color *= 1.00 + caust * blend * ( max (-uwpos.z, 0) * saturate(exp (uwpos.z / 400)) ) * fog * fog * cauststr;
	
	return float4(color,1);
//#else
//	return float4(1,1,1,1);
//#endif
}

//------------------------------------------------------------

ScreenVertOut WaveVS (in BlendDepthVertIn IN) {
	ScreenVertOut OUT;
	OUT.pos = mul (IN.pos, proj);
	OUT.texcoord = IN.texcoord;
	return OUT;
}

//------------------------------------------------------------

static const float waveTexRcpRes2 = 1.5 * waveTexRcpRes;

//------------------------------------------------------------

float4 WavePS (in float2 Tex : TEXCOORD0): COLOR0 {
	float4 c = 2 * tex2D (s6, Tex) - (1.0).xxxx;
	float4 ret = {0, c.r, 0, 0};

	float4 n = {
		tex2D (s6, Tex + float2 (waveTexRcpRes, 0)).r, 
		tex2D (s6, Tex + float2 (-waveTexRcpRes, 0)).r, 
		tex2D (s6, Tex + float2 (0, waveTexRcpRes)).r, 
		tex2D (s6, Tex + float2 (0, -waveTexRcpRes)).r
	};
	float4 n2 = {
		tex2D (s6, Tex + float2 (waveTexRcpRes2, 0)).r, 
		tex2D (s6, Tex + float2 (-waveTexRcpRes2, 0)).r, 
		tex2D (s6, Tex + float2 (0, waveTexRcpRes2)).r, 
		tex2D (s6, Tex + float2 (0, -waveTexRcpRes2)).r
	};

	n = 2 * n - (1.0).xxxx;
	// n2 = 2 * n2 - (1.0).xxxx;

	float nsum = n.x + n.y + n.z + n.w;

	// dampened discrete two - dimensional wave equation
	// red channel: u (t)
	// green channel: u (t - 1)
	// u (t + 1) = (1 - udamp) * u (t) + a * (nsum - 4 * u (t)) + (1 - vdamp) * (u (t) - u (t - 1))
	//		= a * nsum + ((2 - udamp - vdamp) - 4 * a) * u (t) - (1 - vdamp) * u (t - 1);

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

//------------------------------------------------------------

static const float playerWaveSize = 12.0f / waveTexWorldSize; // 12 world units radius

//------------------------------------------------------------

float4 PlayerWavePS (in float2 Tex : TEXCOORD0): COLOR0 {
	float4 ret = tex2D (s6, Tex);
	float wavesize = (1.0 + 0.055 * sin (16 * ticks) + 0.065 * sin (12.87645 * ticks)) * playerWaveSize;
	ret.rg *= saturate (2 * abs (length (Tex - RippleOrigin) / wavesize - 1));
	return ret;
}

//------------------------------------------------------------

Technique T0 {
	//------------------------------------------------------------
	// Used to render the reflected landscape
	Pass P00 {
		#if SHADER_MODEL >= 300
			AlphaBlendEnable = true;
			SRCBLEND = SRCALPHA;
			DESTBLEND = INVSRCALPHA;
		#else
			AlphaBlendEnable = false;
		#endif

		AlphaTestEnable = false;
		FogEnable = false;
		#if SHADER_MODEL >= 300
			CullMode = CCW;
		#else
			CullMode = None;
		#endif

		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 LandscapeRefVS ();
		PixelShader = compile ps_2_0 LandscapeRefPS ();
	}
	//------------------------------------------------------------
	// Used to render the landscape
	Pass P01 {
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		CullMode = CW;
		FogEnable = false;

		VertexShader = compile vs_2_0 LandscapeVertexShader ();
		PixelShader = compile ps_2_0 LandscapePixelShader ();
	}
	//------------------------------------------------------------
	// Used to render the water in SM 2.0 mode
	Pass P02 {
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
	//------------------------------------------------------------
	// Used to render the water in SM 3.0 mode
	Pass P03 {
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
	//------------------------------------------------------------
	// Used for rendering distant statics
	Pass P04 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = CW;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 StaticVS ();
		PixelShader = compile ps_2_0 StaticPS ();
	}
	//------------------------------------------------------------
	// Used for rendering the water from underneath
	Pass P05 {
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
	//------------------------------------------------------------
	// Used for rendering water in non - reflective mode
	Pass P06 {
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
	//------------------------------------------------------------
	// Used for rendering reflected statics
	Pass P07 {
		#if SHADER_MODEL >= 300
			AlphaBlendEnable = true;
			SRCBLEND = SRCALPHA;
			DESTBLEND = INVSRCALPHA;
			AlphaTestEnable = false;
		#else
			AlphaBlendEnable = false;
		#endif
		FogEnable = false;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 StaticReflectionVS ();
		PixelShader = compile ps_2_0 StaticReflectionPS ();
	}
	//------------------------------------------------------------
	// Used for rendering the lowest possible land out into infinity
	Pass P08 {
		AlphaBlendEnable = false;
		#if SHADER_MODEL >= 300
			AlphaTestEnable = false;
		#endif
		CullMode = CW;
		FogEnable = false;
		ZEnable = true;
		ZWriteEnable = false;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 LowestLandVS ();
		PixelShader = compile ps_2_0 LowestLandPS ();
	}
	//------------------------------------------------------------
	// Used for rendering distant statics with HW instancing
	Pass P09 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = CW;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		#if SHADER_MODEL >= 300
			VertexShader = compile vs_3_0 StaticInstVS ();
			PixelShader = compile ps_3_0 StaticPS ();
		#else
			VertexShader = compile vs_2_0 StaticInstVS ();
			PixelShader = compile ps_2_0 StaticPS ();
		#endif
	}
	//------------------------------------------------------------
	// Used for rendering reflected distant statics with HW instancing
	Pass P10 {
		#if SHADER_MODEL >= 300
			AlphaBlendEnable = true;
			SRCBLEND = SRCALPHA;
			DESTBLEND = INVSRCALPHA;
			AlphaTestEnable = false;
		#else
			AlphaBlendEnable = false;
		#endif
		FogEnable = false;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		#if SHADER_MODEL >= 300
			VertexShader = compile vs_3_0 StaticReflectionInstVS ();
			PixelShader = compile ps_3_0 StaticReflectionPS ();
		#else
			VertexShader = compile vs_2_0 StaticReflectionInstVS ();
			PixelShader = compile ps_2_0 StaticReflectionPS ();
		#endif
	}
	//------------------------------------------------------------
	// Used for rendering grass
	Pass P11 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 GrassVS ();
		PixelShader = compile ps_2_0 GrassPS ();
	}
	//------------------------------------------------------------
	// Used for rendering grass with instancing
	Pass P12 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		#if SHADER_MODEL >= 300
			VertexShader = compile vs_3_0 GrassInstVS ();
			PixelShader = compile ps_3_0 GrassPS ();
		#else
			VertexShader = compile vs_2_0 GrassInstVS ();
			PixelShader = compile ps_2_0 GrassPS ();
		#endif
	}
	//------------------------------------------------------------
	// Used for blending what Morrowind draws with what MGE draws
	Pass P13 {
		AlphaBlendEnable = true;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 ScreenQuadVS ();
		PixelShader = compile ps_2_0 BlendDepthPS ();
	}
	//------------------------------------------------------------
	// Used for horizontal blurring
	Pass P14 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 ScreenQuadVS ();
		PixelShader = compile ps_2_0 HorizontalBlurPS ();
	}
	//------------------------------------------------------------
	// Used for vertical blurring
	Pass P15 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 ScreenQuadVS ();
		PixelShader = compile ps_2_0 VerticalBlurPS ();
	}
	//------------------------------------------------------------
	// Used to copy a texture to the screen while also setting alpha to 0
	Pass P16 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 ScreenQuadVS ();
		PixelShader = compile ps_2_0 CopyAndSetAlphaPS ();
	}
	//------------------------------------------------------------
	// Used for rendering grass depth
	Pass P17 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 GrassDepthVS ();
		PixelShader = compile ps_2_0 GrassDepthPS ();
	}
	//------------------------------------------------------------
	// Used for rendering grass depth with instancing
	Pass P18 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		#if SHADER_MODEL >= 300
			VertexShader = compile vs_3_0 GrassDepthInstVS ();
			PixelShader = compile ps_3_0 GrassDepthPS ();
		#else
			VertexShader = compile vs_2_0 GrassDepthInstVS ();
			PixelShader = compile ps_2_0 GrassDepthPS ();
		#endif
	}
	//------------------------------------------------------------
	// Used for rendering the underwater caustic lighting
	Pass P19 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 CausticsVS ();
		PixelShader = compile ps_2_0 CausticsPS ();
	}
	//------------------------------------------------------------
	// Used for calculating waves
	Pass P20 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 WaveVS ();
		PixelShader = compile ps_2_0 WavePS ();
	}
	//------------------------------------------------------------
	// Used for creating ripples around PC
	Pass P21 {
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 WaveVS ();
		PixelShader = compile ps_2_0 PlayerWavePS ();
	}
	//------------------------------------------------------------
	// Used to set up the render state
	Pass Zero { // 22
		AlphaBlendEnable = false;
		AlphaTestEnable = false;
		FogEnable = false;
		FogColor = <FogCol>;
		ColorVertex = false;
		Lighting = false;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		StencilEnable = false;
		ColorWriteEnable = red | green | blue;
		CullMode = CCW;
		ShadeMode = gouraud;
		ColorOp [0] = selectarg1;
		ColorOp [1] = disable;
		AlphaOp [0] = disable;
		AlphaOp [1] = disable;
		ResultArg [0] = current;
		Texture [0] = null;
		Texture [1] = null;
		Texture [2] = null;
		Texture [3] = null;
		Texture [4] = null;
		Texture [5] = null;
		Texture [6] = null;
		Texture [7] = null;
		TexCoordIndex [0] = 0;
		TexCoordIndex [1] = 1;
		TexCoordIndex [2] = 2;
		TexCoordIndex [3] = 3;
		TexCoordIndex [4] = 4;
		TexCoordIndex [5] = 5;
		TexCoordIndex [6] = 6;
		TexCoordIndex [7] = 7;
		TextureTransformFlags [0] = 0;
		TextureTransformFlags [1] = 0;
		TextureTransformFlags [2] = 0;
		TextureTransformFlags [3] = 0;
		TextureTransformFlags [4] = 0;
		TextureTransformFlags [5] = 0;
		TextureTransformFlags [6] = 0;
		TextureTransformFlags [7] = 0;
		MagFilter [0] = Linear;
		MinFilter [0] = Linear;
		MipFilter [0] = Linear;
		Clipping = true;
	}
	//------------------------------------------------------------
}