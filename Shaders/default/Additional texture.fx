//Author: Timeslip
//Loads an additional texture and blends it into the screen

string texname1="_land_default.tga";
texture tex1;

Technique T0
{
	Pass P0
	{
		Texture[1]=<tex1>;
		PixelShader = asm
		{
			ps.1.1
			tex t0
			tex t1
			add_sat r0,t0,t1
		};
	}
}
