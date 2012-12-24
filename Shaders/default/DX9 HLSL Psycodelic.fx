//Author: Timeslip

sampler s0;
int tickcount;

float4 Psycodelic(float2 tex : TEXCOORD0) : COLOR
{
	
	float4 c0 = tex2D(s0, tex);

	c0.r += sin(c0.r+((tickcount%(20000*3.141592654))/(400*3.141592654))/0.3)/8;
	c0.g += sin(c0.g+((tickcount%(20000*3.141592654))/(300*3.141592654))/0.3)/8;
	c0.b += sin(c0.b-((tickcount%(20000*3.141592654))/(250*3.141592654))/0.3)/8;
	
	return c0;
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 Psycodelic();
    }
}
