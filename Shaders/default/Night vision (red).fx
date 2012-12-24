//Author: Timeslip
//Similar to the standard nightvision shader, but the red channel is amplified instead.

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4				//Need shader 1.4 to perform scaler operations on the r, g and b channels
			texld r0,t0			//Sample the backbuffer
			add_sat r0.g,r0.g,r0.b	//Add the blue channel to the green
			add_sat r0.r,r0.r,r0.g	//Add the green and blue channels to the red
			mul r0.gb,r0.g,r0.g		//Square the green channel, and put the results into both the green and blue channels
		};
	}
}
