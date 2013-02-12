//Author: Scanti
//Reddens the screen around the outside

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.2
			def c0,0.80,0.80,0,1
			def c1,0.80,0.80,0.80,1
			def c2,0,1,1,1
			texcoord t0
			tex t1
			mov r0,t0_bx2
			mul r0,r0,r0
			dp3_sat r0,r0,c0
			sub r0,r0,c1
			add_sat r0,r0,c2
			mul r0,t1,1-r0
			mov r0.a,c0
		};
	}
}
