//Author: Timeslip
//This is a copy of HDR 2 with the bloom stripped out so that it runs on sm 1.x cards

float4 HDR;

sampler s0 = sampler_state { };

float4 HDRpass( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = tex2D( s0, Tex );
    float adjust = (0.5-HDR.r) + smoothstep(0.7,1,HDR.a)/2;
    Color *= 1+adjust;
    return saturate(Color);
}

technique T0
{
    pass p0 { PixelShader = compile ps_1_4 HDRpass(); }
}
