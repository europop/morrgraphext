//Author: Gabest

sampler s0;
int tickcount;

float4 Wave(float2 tex : TEXCOORD0) : COLOR
{
	// don't look at this for too long, you'll get dizzy :)
	
	float4 c0 = 0;
	
	tex.x += sin(tex.x+((tickcount%(20000*3.141592654))/(500*3.141592654))/0.3)/20;
	tex.y += sin(tex.x+((tickcount%(20000*3.141592654))/(500*3.141592654))/0.3)/20;
	
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
        PixelShader = compile ps_2_0 Wave();
    }
}
