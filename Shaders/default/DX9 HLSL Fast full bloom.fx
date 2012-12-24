//Author: Timeslip
//A bloom shader chain combined into a single technique with a couple of passes combined for greater efficency

texture lastpass;
texture thisframe;

sampler s0 = sampler_state { texture=<thisframe>; };
sampler s1 = sampler_state { texture = <lastpass>; magfilter=linear; };

static const float Luminance = 0.08f;
static const float fMiddleGray = 0.18f;
static const float fWhiteCutoff = 0.8f;

static const int g_cKernelSize = 13;
static const float BloomScale = 1.5f;

static const int scale = 4;

float2 rcpres;

float2 PixelCoordsDownFilter[16] =
{
    { 1.5,  -1.5 },
    { 1.5,  -0.5 },
    { 1.5,   0.5 },
    { 1.5,   1.5 },

    { 0.5,  -1.5 },
    { 0.5,  -0.5 },
    { 0.5,   0.5 },
    { 0.5,   1.5 },

    {-0.5,  -1.5 },
    {-0.5,  -0.5 },
    {-0.5,   0.5 },
    {-0.5,   1.5 },

    {-1.5,  -1.5 },
    {-1.5,  -0.5 },
    {-1.5,   0.5 },
    {-1.5,   1.5 },
};

float2 PixelKernelH[g_cKernelSize] =
{
    { -6, 0 },
    { -5, 0 },
    { -4, 0 },
    { -3, 0 },
    { -2, 0 },
    { -1, 0 },
    {  0, 0 },
    {  1, 0 },
    {  2, 0 },
    {  3, 0 },
    {  4, 0 },
    {  5, 0 },
    {  6, 0 },
};

float2 PixelKernelV[g_cKernelSize] =
{
    { 0, -6 },
    { 0, -5 },
    { 0, -4 },
    { 0, -3 },
    { 0, -2 },
    { 0, -1 },
    { 0,  0 },
    { 0,  1 },
    { 0,  2 },
    { 0,  3 },
    { 0,  4 },
    { 0,  5 },
    { 0,  6 },
};

static const float BlurWeights[g_cKernelSize] = 
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};



float4 UpCombine( float2 Tex : TEXCOORD0, float2 Tex2 : TEXCOORD1 ) : COLOR0
{
    float3 ColorOrig = tex2D( s0, Tex );
    ColorOrig += tex2D( s1, Tex/scale );
    return float4( ColorOrig, 1.0f );
}

float4 BloomH( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;

    for (int i = 0; i < g_cKernelSize; i++)
    {    
        Color += tex2D( s1, Tex + (PixelKernelH[i]*rcpres) ) * BlurWeights[i];
    }

    return Color * BloomScale;
}
float4 BloomV( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;

    for (int i = 0; i < g_cKernelSize; i++)
    {    
        Color += tex2D( s1, Tex + (PixelKernelV[i]*rcpres) ) * BlurWeights[i];
    }

    return Color * BloomScale;
}

float4 DownFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;

    for (int i = 0; i < 16; i++)
    {
        Color += tex2D( s1, (Tex*scale) + (PixelCoordsDownFilter[i]*rcpres) );
    }

    return Color / 16;
}

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
    pass p1
    {
        PixelShader = compile ps_2_0 DownFilter();
    }
    pass p2
    {
        PixelShader = compile ps_2_0 BloomH();
    }
    pass p3
    {
        PixelShader = compile ps_2_0 BloomV();
    }
    pass p2b
    {
        PixelShader = compile ps_2_0 BloomH();
    }
    pass p3b
    {
        PixelShader = compile ps_2_0 BloomV();
    }
    pass p4
    {
        PixelShader = compile ps_2_0 UpCombine();
    }


}
