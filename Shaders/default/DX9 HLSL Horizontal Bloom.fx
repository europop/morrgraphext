//Author: Microsoft
//This blurs the picture horizontally, and increases the brightness

sampler s0;

static const int g_cKernelSize = 13;
static const float BloomScale = 1.5f;

float2 rcpres;

float2 PixelKernel[g_cKernelSize] =
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

float4 PostProcessPS( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;

    for (int i = 0; i < g_cKernelSize; i++)
    {    
        Color += tex2D( s0, Tex + PixelKernel[i]*rcpres ) * BlurWeights[i];
    }

    return Color * BloomScale;
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 PostProcessPS();
    }
}
