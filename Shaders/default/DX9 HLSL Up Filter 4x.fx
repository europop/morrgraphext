//Author: Microsoft
//Scales the image up 4x

sampler s0;

static const int Scale=2;

float4 UpFilterPS( float2 Tex : TEXCOORD0 ) : COLOR0
{
    return tex2D( s0, Tex/Scale );
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 UpFilterPS();
    }
}
