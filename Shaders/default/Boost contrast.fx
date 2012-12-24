//Author: Timeslip
//Boosts the contrast between light and dark areas

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.0
			tex t0
			mul_x2_sat r0,t0,t0
		};
	}
}
