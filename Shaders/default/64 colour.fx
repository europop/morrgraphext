//Author: Timeslip
//Reduces the screen to 64 colours

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4
			def c0,0.25,0.25,0.25,1
			def c1,0,0,0,1
			texld r0,t0
			mov r0,-r0
			mov r2,r0		//replace r0 with c0 for an ... interesting ... effect
			add r1,r0,c0
			cmp r2,r1,r2,r1
			add r1,r1,c0
			cmp r2,r1,r2,r1
			add r1,r1,c0
			cmp r2,r1,r2,r1
			phase
			mov r1.a,c0		//refill alpha channels
			mov r2.a,c0
			add r1,r1,c0
			cmp r2,r1,r2,r1
			add r0.rgb,-r0,r2
			+mov r0.a,c0
		};
	}
}
