//Author: Timeslip
//Doubles the brightness of each pixel

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.0
			tex t0		//Sample texture 0
			mov_x2_sat r0,t0	//Double all channels of the texture and clamp them result between 0 and 1
		};
	}
}
