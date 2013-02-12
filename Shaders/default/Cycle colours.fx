//Author: Timeslip
//Cycles the red, gren and blue colour channels.

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4		//Need shader 1.4 because the other 1.x versions don't allow scaler operation on the r, g and b channels
			texld r0,t0	//Sample the texture
			mov r1.r,r0.g	//Put the green channel into the red channel
			mov r1.g,r0.b	//Put the blue channel into the green channel
			mov r1.b,r0.r	//Put the red channel into the blue channel
			mov r0.rgb,r1	//And then put them all back into the output register
		};
	}
}
