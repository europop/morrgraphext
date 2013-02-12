//Author: Timeslip
//This is a direct copy of my oblivion FakeHDR mod
//It does do some bloom, but it isn't very pronounced. On the plus side, it's fairly fast.

texture lastpass;
float4 HDR;
float4 rcpres;

sampler s0 = sampler_state { };
sampler s1 = sampler_state { texture=<lastpass>; };

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
    Color *= 1+((HDR.r - 0.5)*HDRScale);
    //Color *= 1+((HDR.a - HDR.r)*2);  //Uncomment this line for abrubt brightness changes

    float4 Color2=0;
    for (int i = 0; i < 13; i++)
    {    
        Color2 += tex2D( s1, Tex + (PixelKernelH[i]*rcpres) ) * BlurWeights[i];
        Color2 += tex2D( s1, Tex + (PixelKernelV[i]*rcpres) ) * BlurWeights[i];
    }
    Color2 *= BloomScale;

    return saturate(Color + Color2);
}
float4 HDRBrightPass( in float2 Tex : TEXCOORD0 ) : COLOR0
{
	float4 color = tex2D( s0, Tex );
	//color += saturate(tex2D( s1, Tex ) - (HDR.r + 0.1)); //Uncomment this line for retina burn
	float4 adjust = (color-HDR.r)-HDRAdjust;
	color = saturate((adjust*adjust*adjust*adjust*adjust)*8);
	color.a = 1;
	return color;
}

technique T0
{
    pass p0 { PixelShader = compile ps_2_0 HDRBrightPass(); }
    pass p1 { PixelShader = compile ps_2_0 Bloom();         }
}
