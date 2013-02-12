//Author: Gabest/Timeslip

sampler s0;
int tickcount;

float4 Flag(float2 tex : TEXCOORD0) : COLOR
{
	float4 c0 = 0;
	
	tex.xy /= 0.8;
	tex.xy -= 0.12;

	tex.x += sin(tex.x*12+( (tickcount%(20000*3.141592654))/(500*3.141592654) )/0.3)/20;
	tex.y += sin(tex.x*12+( (tickcount%(20000*3.141592654))/(500*3.141592654) )/0.3)/20;
	
	if(tex.x >= 0 && tex.x <= 1 && tex.y >= 0 && tex.y <= 1)
	{
		c0 = tex2D(s0, tex);
	}
	
	return c0;
}

technique T0
{
    pass p0
    {
        PixelShader = compile ps_2_0 Flag();
    }
}
