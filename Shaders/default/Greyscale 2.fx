//Author: Scanti
//A ps.1.0 version of the greyscale shader

Technique T0
{
	Pass P0
	{
		PixelShader=asm
		{
			ps.1.0
			def c0,0.30,0.59,0.11,1
			tex t0
			dp3 r0,t0,c0
			mov r0.a,c0
		};
	}
}
