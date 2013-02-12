// This shader contains techniques used to render objects to a floating point
// depth buffer.

//-----------------------------------------------------------------------------
// Global Parameters
//-----------------------------------------------------------------------------

Matrix g_View;
Matrix g_Proj;
Matrix g_World;
float4x4 g_MatrixPalette [4] : MATRIXPALETTE;
int g_BlendWeightCount;
bool g_AlphaTest;

static const float FLT_MAX = 340282346600000000000000000000000000000.0f;

texture AlphaTestTexture;
sampler AlphaTestSampler = sampler_state {
	texture = <AlphaTestTexture>;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	addressu = clamp;
	addressv = clamp;
};

//-----------------------------------------------------------------------------
// Shader Code
//-----------------------------------------------------------------------------

struct StaticDepthVertIn {
	float4 pos: POSITION;
	half2 texcoords: TEXCOORD0;
};

//-----------------------------------------------------------------------------

struct StaticDepthVertOut {
	float4 pos : POSITION;
	float depth: TEXCOORD0;
	half2 texcoords : TEXCOORD1;
};

//-----------------------------------------------------------------------------

StaticDepthVertOut StaticDepthVS (StaticDepthVertIn IN) {
	StaticDepthVertOut OUT;

	// Projection
	OUT.pos = mul (IN.pos, g_World);
	OUT.pos = mul (OUT.pos, g_View);
	OUT.pos = mul (OUT.pos, g_Proj);

	OUT.texcoords = IN.texcoords;
	OUT.depth = OUT.pos.w;

	return OUT;
}

//-----------------------------------------------------------------------------

float4 StaticDepthPS (StaticDepthVertOut IN): COLOR0 {
	// Perform alpha testing
	float alpha = tex2D (AlphaTestSampler, IN.texcoords).a;
	clip (alpha - 0.5);

	// Return depth
	return float4 (IN.depth, 1, 1, 1);
}
	

//-----------------------------------------------------------------------------

struct NoAlphaDepthVertOut {
	float4 pos : POSITION;
	float depth: TEXCOORD0;
};

//-----------------------------------------------------------------------------

NoAlphaDepthVertOut NoAlphaDepthVS (float4 pos : POSITION) {
	NoAlphaDepthVertOut OUT;

	// Projection
	OUT.pos = mul (pos, g_World);
	OUT.pos = mul (OUT.pos, g_View);
	OUT.pos = mul (OUT.pos, g_Proj);

	OUT.depth = OUT.pos.w;

	return OUT;
}

//-----------------------------------------------------------------------------

float4 NoAlphaDepthPS (StaticDepthVertOut IN): COLOR0 {
	// Return depth
	return float4 (IN.depth, 1, 1, 1);
}

//-----------------------------------------------------------------------------

struct RenderDepthVertIn {
	float4 pos: POSITION;
	float4 blendWeights: BLENDWEIGHT;
	float2 texcoords: TEXCOORD0;
};

//-----------------------------------------------------------------------------

struct RenderDepthVertOut {
	float4 pos: POSITION;
	float4 position: TEXCOORD0;
	// float4 col: COLOR0;
	float2 texcoords: TEXCOORD1;
};

//-----------------------------------------------------------------------------

RenderDepthVertOut RenderGenericDepthVS (in RenderDepthVertIn IN) {
	RenderDepthVertOut OUT;

	// float4 worldpos = mul (IN.pos, g_MatrixPalette [0]);

	// float blendWeightsArray [4] = (float [4])IN.blendWeights;

	float4 worldpos = 0.0f;

	float4 blend = IN.blendWeights;

	if (g_BlendWeightCount == 3) {
		blend [3] = 1 - blend [0] - blend [1] - blend [2];
	} else if (g_BlendWeightCount == 2) {
		blend [2] = 1 - blend [0] - blend [1];
	} else if (g_BlendWeightCount == 1) {
		blend [1] = 1 - blend [0];
	}

	// calculate pos
	if (g_BlendWeightCount == 0) {
		// OUT.col = 0.5;
		worldpos = mul (IN.pos, g_MatrixPalette [0]);
	} else {
		// OUT.col = blend;
		if (g_BlendWeightCount >= 1) {
			worldpos += mul (IN.pos, g_MatrixPalette [0]) * blend [0];
			worldpos += mul (IN.pos, g_MatrixPalette [1]) * blend [1];
		}
		if (g_BlendWeightCount >= 2) {
			worldpos += mul (IN.pos, g_MatrixPalette [2]) * blend [2];
		}
		if (g_BlendWeightCount >= 3) {
			worldpos += mul (IN.pos, g_MatrixPalette [3]) * blend [3];
		}
	}

	OUT.pos = mul (worldpos, g_View);
	OUT.pos = mul (OUT.pos, g_Proj);

	OUT.position = OUT.pos;
	OUT.texcoords = IN.texcoords;

	return OUT;
}

//-----------------------------------------------------------------------------

float4 RenderGenericDepthPS (RenderDepthVertOut IN): COLOR0 {
	// return IN.col;

	if (g_AlphaTest == true) {
		// Apply alpha testing
		float alpha = tex2D (AlphaTestSampler, IN.texcoords).a;
		clip (alpha - 0.5);
	}

	//// Define % to Blend
	//float blend =
	//	saturate ((BlendEnd - IN.position.w) / (BlendEnd - BlendStart));

	//float3 color = lerp (float3 (1, 1, 1), float3 (0, 0, 0), blend);

	return float4 (IN.position.w, 1, 1, 1);
}

//-----------------------------------------------------------------------------

struct ScreenVertIn {
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
};

//-----------------------------------------------------------------------------

struct ScreenVertOut {
	float4 pos: POSITION;
	float2 texcoord : TEXCOORD0;
};

//-----------------------------------------------------------------------------

ScreenVertOut ScreenQuadVS (in ScreenVertIn IN) {
	ScreenVertOut OUT;

	OUT.pos = mul (IN.pos, g_World);
	OUT.pos = mul (OUT.pos, g_View);
	OUT.pos = mul (OUT.pos, g_Proj);

	OUT.texcoord = IN.texcoord;

	return OUT;
}

//-----------------------------------------------------------------------------

float4 ClearDepthPS (ScreenVertOut IN): COLOR0 {
	return float4 (FLT_MAX / 2, 1, 1, 1);
}


////////////////////////////////////////////////////////////////////////////////

VertexShader g_shadersVS[2] = { compile vs_2_0 StaticDepthVS(), compile vs_2_0 RenderGenericDepthVS() };
PixelShader  g_shadersPS[2] = { compile ps_2_0 StaticDepthPS(), compile ps_2_0 RenderGenericDepthPS() };


//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------

// Used for rendering unskined objects to the depth buffer using alpha testing
technique StaticDepth {
	pass p0 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = CW;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 StaticDepthVS ();
		PixelShader = compile ps_2_0 StaticDepthPS ();
	}
}

//-----------------------------------------------------------------------------

// Used for rendering things with no alpha texture to the depth texture
technique NoAlphaDepth {
	pass p0 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = CW;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;

		VertexShader = compile vs_2_0 NoAlphaDepthVS ();
		PixelShader = compile ps_2_0 NoAlphaDepthPS ();
	}
}

//-----------------------------------------------------------------------------

// Used for rendering the Morrowind depth texture
technique RenderGenericDepth {
	pass p0 {
		AlphaBlendEnable = false;
		FogEnable = false;
		CullMode = CW;
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		AlphaTestEnable = false;

		VertexShader = g_shadersVS[ !!g_BlendWeightCount ];
		PixelShader = g_shadersPS[ !!g_BlendWeightCount ];
	}
}
	
//-----------------------------------------------------------------------------

// Used for clearing the depth buffer to FLT_MAX
technique ClearDepth {
	pass p0 {
		FogEnable = false;
		CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 ScreenQuadVS ();
		PixelShader = compile ps_2_0 ClearDepthPS ();
	}
}

//-----------------------------------------------------------------------------
