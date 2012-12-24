//Author: Timeslip
//A combination of HDR 4 and 5
//Warning: Slow!

texture thisframe;
texture lastpass;

sampler s0 = sampler_state { texture = <thisframe>; };
sampler s1 = sampler_state { texture = <lastpass>; magfilter=linear; };

static const float Luminance = 0.08f;
static const float fMiddleGray = 0.18f;
static const float fWhiteCutoff = 0.8f;

static const int g_cKernelSize = 13;
static const float BloomScale = 1.6f;

static const float HDRScale = -0.9;
static const float HDRAdjust = 0;

static const int Scale=2;

float2 rcpres;
float4 HDR;

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

float4 HDRUpCombine( float2 Tex : TEXCOORD0, float2 Tex2 : TEXCOORD1 ) : COLOR0
{
    float neg = smoothstep(0.2,1,HDR.r)/4;
    float pos = 0.75 + smoothstep(0,0.8,HDR.r)/4;
    float3 result = saturate((tex2D(s0, Tex)-neg)/(pos-neg));
    float3 bloom = tex2D(s1, Tex/Scale);
    result = sqrt((result*result)+(bloom*bloom));
    return float4(result, 1 );
}

float4 UpFilter( float2 Tex : TEXCOORD0 ) : COLOR0
{
    return tex2D( s1, Tex/Scale );
}

float4 Bloom( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;

    for (int i = 0; i < g_cKernelSize; i++)
    {    
        Color += tex2D( s1, Tex + (PixelKernelV[i]*rcpres) ) * BlurWeights[i];
        Color += tex2D( s1, Tex + (PixelKernelH[i]*rcpres) ) * BlurWeights[i];
    }

    return Color * BloomScale/2;
}

float4 DownFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;

    for (int i = 0; i < 16; i++)
    {
        Color += tex2D( s1, (Tex*Scale) + (PixelCoordsDownFilter[i]*rcpres) );
    }

    return Color / 16;
}

float4 HDRBrightPassFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 color = tex2D( s0, Tex );
    float4 adjust = (color-HDR.r)-HDRAdjust;
    color = saturate((adjust*adjust*adjust*adjust*adjust)*8);
    color.a = 1;
    return color;
}

technique T0
{
    pass p0 { PixelShader = compile ps_2_0 HDRBrightPassFilter(); }
    pass p1 { PixelShader = compile ps_2_0 DownFilter();          }
    pass p2 { PixelShader = compile ps_2_0 DownFilter();          }
    pass p4 { PixelShader = compile ps_2_0 Bloom();               }
    pass p6 { PixelShader = compile ps_2_0 Bloom();               }
    pass p7 { PixelShader = compile ps_2_0 UpFilter();            }
    pass p8 { PixelShader = compile ps_2_0 HDRUpCombine();        }
}
