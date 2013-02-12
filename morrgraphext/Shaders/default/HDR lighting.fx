//Author: Timeslip
//Changes the overall screen brightness depending on what you're looking at

Texture thisframe;

Technique T0
{
	Pass P0
	{
		Texture[1]=<thisframe>;
		PixelShader=asm
		{
			ps.1.4
			def c0,0.5,0.5,0,1
			def c1,0.30,0.59,0.11,1
			texld r0,t0
			mov r1,c0
			phase
			texld r1,r1
			dp3 r1,r1,c1
			mov_d8 r1,r1
			sub r0.rgb,r0,r1
			+mov r0.a,c0
		};
	}
}
