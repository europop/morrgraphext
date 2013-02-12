//Author: Microsoft
//This shader detects edges. This should be more accurate than my DX8 outline shader

sampler s0;
float2 rcpres;

float2 PixelKernel[4] =
{
    { 0,  1},
    { 1,  0},
    { 0, -1},
    {-1,  0}
};

float3 LuminanceConv = { 0.2125f, 0.7154f, 0.0721f };

float4 PostProcessPS( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Orig = tex2D( s0, Tex );

    float4 Sum = 0;

    for( int i = 0; i < 4; i++ )
        Sum += ( abs( Orig - tex2D( s0, Tex + (PixelKernel[i]*rcpres) ) ) - 0.5f ) * 1.2 + 0.5f;

    return saturate( dot( LuminanceConv, Sum ) ) * 5;
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 PostProcessPS();
    }
}
