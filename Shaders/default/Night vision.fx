//Author: Timeslip
//Creates a real night vision effect
//Green is amplified, and the screen becomes saturated in bright areas.

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4				//Need shader 1.4 to perform scaler operations on the r, g and b channels
			texld r0,t0			//Sample the backbuffer
			add_sat r0.r,r0.r,r0.b	//Add the blue channel to the red
			add_sat r0.g,r0.g,r0.r	//Add the combined red and blue channels to the green
			mul r0.rb,r0.r,r0.r		//square the red channel, and put the results in the red and blue channels
		};
	}
}
