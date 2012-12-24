//Author: Microsoft
//This shader combines the result of the previous shader with the initial picture

texture thisframe;
texture lastshader;

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
    ColorOrig += tex2D( s1, Tex );
    return float4( ColorOrig, 1.0f );
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 Combine();
    }
}
