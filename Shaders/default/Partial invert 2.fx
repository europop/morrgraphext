//Author: Timeslip
//Inverts only the brighter regions of the screen
//Dark regions get their brightness doubled

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4
			texld r0,t0
			mov r0.rgb,1-r0
			mov_x2 r0.rgb,r0_bias
			cmp r0.rgb,r0,r0,-r0
			mov r0.rgb,1-r0
		};
	}
}
