//Author: Scanti
//Blackens the screen, except for a small area in the center

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.2
			def c0,0.70,0.70,0,1
			def c1,0.25,0.25,0.25,1
			texcoord t0
			tex t1
			mov r0,t0_bias
			mov r1,r0	// Cant use the same registor for source and dest with cmp or dp4
			cmp r0,r1,1-t0,t0
			dp3_sat r0,r0,c0
			sub r0,r0,c1
			mul r0,t1,r0
			mov r0.a,c0
		};
	}
}
