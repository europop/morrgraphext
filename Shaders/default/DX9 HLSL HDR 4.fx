//Author: Timeslip
//A contrast boosting HDR shader

float4 HDR;

sampler s0 = sampler_state { };

float4 HDRpass( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float neg = smoothstep(0.2,1,HDR.r)/4;
    float pos = 0.75 + smoothstep(0,0.8,HDR.r)/4;
    float3 res = saturate((tex2D(s0, Tex)-neg)/(pos-neg));
    return float4(res, 1 );
}

technique T0
{
    pass p0 { PixelShader = compile ps_2_0 HDRpass(); }
}
