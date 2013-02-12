//Author: Timeslip
//Only displays the brightest parts of the screen

Technique T0
{
	Pass BrightPass
	{
		PixelShader = asm
		{
			ps.1.4
			def c0,0.561797752,0.561797752,0.561797752,1
			def c1,0.78125,0.78125,0.78125,1
			def c2,1,1,1,1
			def c3,0.1,0.1,0.1,1
			texld r0,t0
			mul_x4 r0,r0,c0
			mul_x2 r1,r0,c1
			add r1,r1,c2
			mul r0,r0,r1
			mov_x4 r2,c2
			add r2,r2,c2
			sub r0,r0,r2
			mul_sat r0,r0,c3  //In a real bright pass, this should be [r0 = r0 / (10 + r0)], but this instruction is impossible in ps.1.x
		};
	}
}
