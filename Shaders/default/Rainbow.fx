//Author: Timeslip
//Does some quite weird things to the screen
//In white fog, a rainbow forms around the player

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4
			def c0,0.25,0.25,0.25,1
			texld r0,t0
			mov r0,-r0
			mov r2,c0
			add r1,r0,c0
			cmp r2,r1,r2,r1
			add r1,r1,c0
			cmp r2,r1,r2,r1
			add r1,r1,c0
			cmp r2,r1,r2,r1
			phase
			add r1.rgb,r1,c0
			+mov r1.a,c0
			mov r2.a,c0
			cmp r2,r1,r2,r1
			sub r0.rgb,-r0,r2
			+mov r0.a,c0
		};
	}
}
