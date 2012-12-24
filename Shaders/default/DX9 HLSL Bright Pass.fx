//Author: Microsoft
//This is a dx9 version of the bright pass shader.
//This is more accurate than the DX8 version

sampler s0;

static const float Luminance = 0.08f;
static const float fMiddleGray = 0.18f;
static const float fWhiteCutoff = 0.8f;

float4 BrightPassFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float3 ColorOut = tex2D( s0, Tex );

    ColorOut *= fMiddleGray / ( Luminance + 0.001f );
    ColorOut *= ( 1.0f + ( ColorOut / ( fWhiteCutoff * fWhiteCutoff ) ) );
    ColorOut -= 5.0f;
    ColorOut = max( ColorOut, 0.0f );
    ColorOut /= ( 10.0f + ColorOut );

    return float4( ColorOut, 1.0f );
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 BrightPassFilter();
    }
}
