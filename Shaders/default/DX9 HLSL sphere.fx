//Author: Timeslip
sampler s0;

float4 SphereFilter( in float2 Tex : TEXCOORD0 ) : COLOR0
{
	Tex -= 0.5;
	float r = pow((pow(Tex.x, 2) + pow(Tex.y, 2) ), 0.5);
	float3 ColourOut = r;
	Tex *= r*pow(2, 0.5);
	Tex += 0.5;

	float3 ColorOut = tex2D( s0, Tex );
    	return float4( ColorOut, 1.0f );
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 SphereFilter();
    }
}
