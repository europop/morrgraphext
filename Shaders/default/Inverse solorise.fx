//Author: Scanti
//Inverts the colour of bright pixels without affecting darker areas

Technique T0
{
	Pass P0
	{
		PixelShader=asm
		{
			ps.1.3
			def c0,0.30,0.59,0.11,1
			def c1,0.50,0.50,0.50,1    //set swap-over threshold
			tex t0
			dp3 r0,t0,c0  
			sub r0,r0,c1
			mov r1,r0
			cmp r0,r1,1-t0,t0    //inverse the solorise effect
			mov r0.a,c0
		};
	}
}
