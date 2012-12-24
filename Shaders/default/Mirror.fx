//Author: Timeslip
//Draws four copies of the screen, each one mirrored in a different direction
//This shader appears to be broken in game, despite working in the shader editor.

Technique T0
{
	Pass P0
	{
		AddressU[0]=2;
		AddressV[0]=2;
		PixelShader = asm
		{
			ps.1.4
			def c0,0.5,0.5,0,0
			texcrd r0.rgb,t0
			mov_x4 r1,c0
			mul r0.rgb,r0,r1
			phase
			texld r0,r0
		};
	}
}
