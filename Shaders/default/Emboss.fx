//Author: Timeslip
//Performs an emboss on each colour channel seperately

Technique T0
{
	Pass P0
	{
		PixelShader = asm
		{
			ps.1.4			//This is a 2 phase v1.4 pixel shader
			def c0,0.0015625,0,0,1		//Used to mod r0	(Use these value for a 640,480 resolution)
			def c1,0,0.0020833,0,1		//Used to mod r1
			def c2,0.0015625,0.0020833,0,1	//Used to mod r2
			def c3,-0.0015625,0,0,1		//Used to mod r5
			def c4,0,-0.0020833,0,1		//Used to mod r4
			def c5,-0.0015625,-0.0020833,0,1	//Used to mod r3
			//def c0,0.0009765625,0,0,1			//Used to mod r0	(Use these value for a 1024,768 resolution)
			//def c1,0,0.0013020833,0,1			//Used to mod r1
			//def c2,0.0009765625,0.0013020833,0,1	//Used to mod r2
			//def c3,-0.0009765625,0,0,1		//Used to mod r5
			//def c4,0,-0.0013020833,0,1		//Used to mod r4
			//def c5,-0.0009765625,-0.0013020833,0,1	//Used to mod r3
			def c6,0,0,0,1			//Used to set the alpha channel
			def c7,0.5,0.5,0.5,0			//Added to r0 at the end to greyify it
			texcrd r5.xyz,t0			//Get the real texcoords and stuff them in r5
			add r0.xyz,r5,c0		//Add the dx/dy to each register
			add r1.xyz,r5,c1
			add r2.xyz,r5,c2
			add r3.xyz,r5,c5
			add r4.xyz,r5,c4
			add r5.xyz,r5,c3
			phase			//Begin the second phase
			texld r0,r0		//Sample each texture
			texld r1,r1
			texld r2,r2
			texld r3,r3
			texld r4,r4
			texld r5,r5
			add r0,r0,r1		//Add or sub each texture to get a gradient
			add r0,r0,r2
			sub r0,r0,r3
			sub r0,r0,r4
			sub r0.rgb,r0,r5	//Use an rgb write mask so we can use the scaler pipe to write the alpha channel in the
						//same instruction
			+mov r0.a,c6		//Need to rewrite the alpha channel after phase is used
			add_sat r0,r0,c7	//Make the background grey instead of black
		};
	}
}
