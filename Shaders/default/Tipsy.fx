//Author: Scanti
//Combines the screen with the previous frame, creating a motion blur effect

texture lastframe;

Technique T0
{
	Pass P0
	{
		Texture[1]=<lastframe>;
		PixelShader=asm
		{
			ps.1.1
			tex t0
			tex t1
			add_d2 r0,t0,t1
		};
	}
}
