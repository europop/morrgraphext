//Author: Timeslip
//Brightens the screen based on the luminocity of each pixel, instead of individual colour intensities

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			
			ps.1.4
			def c0,0.33333333,0,0,1	//Use this to divide the red channel by 3 later on
			texld r0,t0			//Sample the results of the previous pass
			mov r1,r0
			add r0.r,r0.r,r0.g		//Add the green channel to it
			add r0.r,r0.r,r0.b		//And then the blue
			mul r0.rgb,r0.r,c0.r		//Multiply by c0 and put the result into the output register
			+mov r0.a,c0			//And at the same time fill in the alpha channel
			add_sat r0,r0,r1
		};
	}
}
