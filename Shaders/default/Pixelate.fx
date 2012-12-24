//Author: Scanti
//Pixelates the screen

texture lastpass;

Technique T0
{
	Pass P0      
	{
		PixelShader=asm
		{
			ps.1.4
			def c0,0,0,0,1
			def c1,1,1,0,0
			texcrd r0.rgb,t0
			mov r0.a,c0
			mov_x4 r0.rgb,r0
			sub r0,r0,c1
			sub r0,r0,c1
			sub r0,r0,c1  
			phase
			texld r0,r0
		};
	}
	Pass P1
	{
		Texture[1]=<lastpass>;
		PixelShader=asm
		{
			ps.1.4
			def c0,0,0,0,1
			def c1,0.75,0.75,0,0
			texcrd r1.rgb,t1
			mov r1.a,c0
			mov_d4 r1.rgb,r1
			mov r1.a,c0
			add_sat r1,r1,c1    //Move to last octet as overwriting data being read
			phase
			texld r1,r1
			mov r0,r1
			mov r0.a,c0
		};
	}
}
