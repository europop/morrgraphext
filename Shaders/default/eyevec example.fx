//Author: Timeslip
//Modifies the screen colour dependant on the directory the player is looking
//This shader doesn't work in the preview window because there is no player to do the looking

Vector eyevec;

Technique T0
{
	Pass P0
	{
		Pixelshaderconstant[0]=<eyevec>;
		PixelShader = asm
		{
			ps.1.1
			tex t0
			mul r1,c0,c0
			mul_x2_sat r0,t0,r1
		};
	}
}
