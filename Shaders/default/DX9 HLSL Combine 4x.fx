//Author: Microsoft
//This shader takes the result of the previous shader, scales it up 4x and then combines it with the original picture

texture thisframe;
texture lastshader;

static const int Scale = 2;

sampler s0 = sampler_state
{
	texture = <thisframe>;
};
sampler s1 = sampler_state
{
	texture = <lastshader>;
};

float4 Combine( float2 Tex : TEXCOORD0, float2 Tex2 : TEXCOORD1 ) : COLOR0
{
    float3 ColorOrig = tex2D( s0, Tex2 );
    ColorOrig += tex2D( s1, Tex/Scale );
    return float4( ColorOrig, 1.0f );
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 Combine();
    }
}
