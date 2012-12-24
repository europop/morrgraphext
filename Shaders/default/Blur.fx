//Author: Timeslip
//Blurs the screen

texture thisframe;
texture lastpass;

PixelShader PS = asm
{
	ps.1.4
	def c0,0.0015625,0,0,1	//Used to mod r0,r2	(Use these value for a 640,480 resolution)
	def c1,0,0.0020833,0,1	//Used to mod r1,r3
	def c2,1.5,1.5,1.5,0.5	//Used to set the alpha channel
	texcrd r0.rgb,t0		//Get texture coordinates
	mov r0.a,c2		//And put a value in r0.a so the validater doesn't complain about uninitalized registers
	sub r3,r0,c1		//Copy the texture coords into the first four registers and shift them slightly
	sub r2,r0,c0
	add r1,r0,c1
	add r0,r0,c0
	phase				//Second phase
	texld r0,r0			//Sample the backbuffer at the four different points
	texld r1,r1
	texld r2,r2
	texld r3,r3
	add r0,r0,r1		//Add 'em all up and divide by 4
	add r0,r0,r2
	add_d4 r0,r0,r3
};

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.0		//Start by copying the backbuffer without doing anything interesting to it
			tex t0
			mov r0,t0
		};
	}
	Pass P1
	{
		Texture[0] = <lastpass>;	//We want to sample the results of the last pass so that it gets more blured with each pass
		Texture[1] = <lastpass>;
		Texture[2] = <lastpass>;
		Texture[3] = <lastpass>;
		PixelShader = <PS>;		//Use the same predifined pixelshader for all further passes
	}
	Pass P2 { PixelShader = <PS>; }
	Pass P3 { PixelShader = <PS>; }
	Pass P4 { PixelShader = <PS>; }
	Pass P5 { PixelShader = <PS>; }
	Pass P6 { PixelShader = <PS>; }
	Pass P7 { PixelShader = <PS>; }
	Pass P8 { PixelShader = <PS>; }
	Pass P9 { PixelShader = <PS>; }
}
