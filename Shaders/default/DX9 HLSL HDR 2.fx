//Author: Timeslip
//This is a copy of my original HDR shader tweaked to prevent grey-outs when staring straight at the sun

texture lastpass;
float4 HDR;
float4 rcpres;

sampler s0 = sampler_state { AddressU=Clamp; AddressV=Clamp; };
sampler s1 = sampler_state { texture=<lastpass>; AddressU=Clamp; AddressV=Clamp; };

static const float BloomScale = 2;
static const float HDRScale = -0.9;
static const float HDRAdjust = 0.25;

float2 PixelKernelH[13] =
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

float2 PixelKernelV[13] =
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

static const float BlurWeights[13] = 
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

float4 Bloom( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = tex2D( s0, Tex );
    float adjust = (0.5-HDR.r) + smoothstep(0.7,1,HDR.a)/2;
    Color *= 1+adjust;

    float4 Color2=0;
    for (int i = 0; i < 13; i++)
    {    
        Color2 += tex2D( s1, Tex + (PixelKernelH[i]*rcpres) ) * BlurWeights[i];
        Color2 += tex2D( s1, Tex + (PixelKernelV[i]*rcpres) ) * BlurWeights[i];
    }
    Color2 *= BloomScale;

    return saturate(Color + Color2);
    //return saturate(Color);
}
float4 HDRBrightPass( in float2 Tex : TEXCOORD0 ) : COLOR0
{
	float eye = HDR.r - (smoothstep(0.8,1,HDR.a)/4);
	float4 color = tex2D( s0, Tex );
	float4 adjust = (color-eye)-HDRAdjust;
	color = saturate((adjust*adjust*adjust*adjust*adjust)*8);
	color.a = 1;
	return color;
}

technique T0
{
    pass p0 { PixelShader = compile ps_2_0 HDRBrightPass(); }
    pass p1 { PixelShader = compile ps_2_0 Bloom();         }
}
