//Author: Microsoft
//Performs a tone map on the picture

sampler s0;

static const float Luminance = 0.08f;
static const float fMiddleGray = 0.18f;
static const float fWhiteCutoff = 0.8f;

float4 ToneMapFilter( float2 Tex : TEXCOORD ) : COLOR0
{
    float4 Color;

    Color = tex2D( s0, Tex ) * fMiddleGray / ( Luminance + 0.001f );
    Color *= ( 1.0f + ( Color / ( fWhiteCutoff * fWhiteCutoff ) ) );
    Color /= ( 1.0f + Color );

    return Color;
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 ToneMapFilter();
    }
}
