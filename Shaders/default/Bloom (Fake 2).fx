//Author: Timeslip
//Another fake bloom shader

texture lastpass;
texture thisframe;

PixelShader PS = asm
{
	ps.1.4
	def c0,0.0015625,0,0,1		//Used to mod r0,r2	(Use these value for a 640,480 resolution)
	def c1,0,0.0020833,0,1		//Used to mod r1,r3
	texcrd r0.rgb,t0	//Get texture coordinates
	sub r3.rgb,r0,c1		//Copy the texture coords into the first four registers and shift them slightly
	sub r2.rgb,r0,c0
	add r1.rgb,r0,c1
	add r0.rgb,r0,c0
	phase			//Second phase
	texld r0,r0		//Sample the backbuffer at the four different points
	texld r1,r1
	texld r2,r2
	texld r3,r3
	add r0,r1,r0
	add r0,r2,r0
	add_d4 r0.rgb,r3,r0
	+mov r0.a,c1		//And fill in the alpha channel again. (It gets lost on the phase instruction)
};

Technique T0
{
	Pass brightpass
	{
		PixelShader = asm
		{
			ps.1.3
			def c0,0,0,0,1
			def c1,0.99,0.99,0.99,0
			tex t0
			sub r0,t0,c1
			mov r1,r0
			cmp r0,r1,r1,c0
			mov_x4 r0,r0
			mov_x4 r0,r0
			mov_x4 r0.rgb,r0
			+mov r0.a,c0
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
	Pass P2 { PixelShader = <PS>; }	//Repeat until we get a useful sized bloom
	Pass P3 { PixelShader = <PS>; }
	Pass P4 { PixelShader = <PS>; }
	Pass P5 { PixelShader = <PS>; }
	Pass P6 { PixelShader = <PS>; }
	Pass P7 { PixelShader = <PS>; }
	Pass P8 { PixelShader = <PS>; }
	Pass P9 { PixelShader = <PS>; }
	Pass P11 { PixelShader = <PS>; }
	Pass P12 { PixelShader = <PS>; }
	Pass P13 { PixelShader = <PS>; }
	Pass P14 { PixelShader = <PS>; }
	Pass P15 { PixelShader = <PS>; }
	Pass P16 { PixelShader = <PS>; }
	Pass P17 { PixelShader = <PS>; }
	Pass P18 { PixelShader = <PS>; }
	Pass P19 { PixelShader = <PS>; }
	Pass P20 { PixelShader = <PS>; }
	Pass combine
	{
		Texture[0] = <thisframe>;
		PixelShader = asm	//End by blending the bloom effect with the original backbuffer so that we get a useful picture
		{
			ps.1.1		//Need 1.1 because 1.0 doesn't allow you to read from two texture registers at once
			tex t0		//Sample the final bloom results
			tex t1		//Sample the original backbuffer
			mov_x4 r0,t1
			add_sat r0,t0,r0	//Add 'em up
			//mov r0,r1		//Uncomment this line to see the bloom without the background picture
		};
	}
}
