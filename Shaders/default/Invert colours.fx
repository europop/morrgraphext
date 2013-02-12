//Author: Timeslip
//Inverts the colour of each pixel

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.0
			tex t0			//Sample the texture
			mov r0.rgb,1-t0	//Invert the texture, but if we invert the alpha channel we just end up with a black screen
						//so use an rgb mask
			+mov r0.a,t0		//Because of the rgb mask we can use the scaler pipeline to perform another operation at
						//the same time
		};
	}
}
