//Author: Timeslip
//An example of a multiple pass shader
//This could easily be done in a single pass.

texture lastpass;

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.0
			texcoord t0
			mov r0,t0
		};
	}
	Pass P1
	{
		Texture[1] = <lastpass>;
		PixelShader = asm
		{
			ps.1.1
			tex t0			//Sample the texture (The backbuffer is X8R8G8B8, so alpha is always set to 1)
			tex t1
			add_d2 r0,t0,t1
		};
	}
}
