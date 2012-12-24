//Author: Timeslip
//Inverts only dark regions of the screen

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4
			texld r0,t0			//Sample the texture
			mov_x2 r0,r0_bias		//Convert the texture to between -1 and 1
			cmp r0.rgb,r0,r0,-r0		//r0 = abs r0 (Don't touch the alpha channel; we don't want to invert it
		};
	}
}
