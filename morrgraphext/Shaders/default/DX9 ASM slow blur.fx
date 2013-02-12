//Author: Timeslip
//This is no better than the DX8 version; it's just an example of a DX9 asm shader
//Normally, you'd want to use HLSL instead
//You need to tick the DX9 mode checkbox to compile this shader

texture lastpass;
texture thisframe;

PixelShader PS = asm
{
	ps.2.0
	def c0,0.0015625,0,0,1		//Used to mod r0,r2	(Use these value for a 640,480 resolution)
	def c1,0,0.0020833,0,1		//Used to mod r1,r3
	def c2,0.25,0.25,0.25,1
	dcl t0
	dcl_2d s0
	mov r0,t0
	sub r3.rgb,r0,c1		//Copy the texture coords into the first four registers and shift them slightly
	sub r2.rgb,r0,c0
	add r1.rgb,r0,c1
	add r0.rgb,r0,c0
	texld r0,r0,s0		//Sample the backbuffer at the four different points
	texld r1,r1,s0
	texld r2,r2,s0
	texld r3,r3,s0
	add r0,r1,r0
	add r0,r2,r0
	add r0,r3,r0
	mul r0,r0,c2
	mov r0.a,c1		//And fill in the alpha channel
	mov oC0, r0
};

Technique T0
{
	Pass P0
	{
		PixelShader = <PS>;		//Use the same predifined pixelshader for all further passes
	}
	Pass P1 { 
		Texture[0] = <lastpass>;	//We want to sample the results of the last pass so that it gets more blured with each pass
		Texture[1] = <lastpass>;
		Texture[2] = <lastpass>;
		Texture[3] = <lastpass>;
		PixelShader = <PS>;
	}
	Pass P2 { PixelShader = <PS>; }
	Pass P3 { PixelShader = <PS>; }
	Pass P4 { PixelShader = <PS>; }
	Pass P5 { PixelShader = <PS>; }
	Pass P6 { PixelShader = <PS>; }
	Pass P7 { PixelShader = <PS>; }
	Pass P8 { PixelShader = <PS>; }
}
