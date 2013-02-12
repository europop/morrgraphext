//Author: Microsoft
//This shader scales the image down 4x

sampler s0 = sampler_state {
	AddressU = Clamp;
	AddressV = Clamp;
};
float2 rcpres;

static const int Scale=2;
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

float4 DownFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;

    for (int i = 0; i < 16; i++)
    {
        Color += tex2D( s0, (Tex*Scale) + (PixelCoordsDownFilter[i]*rcpres) );
    }

    return Color / 16;
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 DownFilter();
    }
}
