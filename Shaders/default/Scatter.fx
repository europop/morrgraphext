//Author: Scanti
//A very fast blur effect which works by adding noise to the position of each pixel

string texname1="mge/default/scatter1.tga";
texture tex1;

//offset within noise texture - used to animate the scatter effect
vector noiseoffset={0.5,0.5,0,0};
//percentage to scatter pixels. 0 = no change, 1 = pixel could be scattered anywhere on screen;
vector scatterperc={0.01,0.01,1,1};

Technique T0
{
	Pass P0
	{
		Texture[1]=<tex1>;
		AddressU[1]=wrap;
		AddressV[1]=wrap;
		PixelShaderConstant[0]=<scatterperc>;
		PixelShaderConstant[4]=<noiseoffset>;
		
		VertexShader = decl
    		{
			stream 0;
			float v0[4]; // Position RH
			float v7[2]; // Texture Coord1
			float v8[2]; // Texture Coord2
			float v9[2]; // Texture Coord3
			float v10[2]; // Texture Coord4
			float v11[2]; // Texture Coord5
			float v12[2]; // Texture Coord6
		}
    		 asm {
			vs.1.1
			def c0,1,1,0,0
			def c1,2,2,1,1
			
			// c2 = 0.5/x,0.5/y,0,0
			def c2,0.000625,0.00083333,0,0
			// c3 = repeat x,repeat y,1,1 - 10 and 8 should do
			def c3,10,8,1,1
						
			sge r0.xzw,v0,c0
			slt r0.y,v0,c1
			mul r0,r0,c1
			sub r0,r0,c0
			mov oPos,r0
			
			add r1,v7,c2
			mov oT0,v7
	
			mul r1,v8,c3
			add r1,r1,c4
			mov oT1,r1			
		};

		PixelShader = asm
		{
			ps.1.4

			def c1,0,0,0,1

			texcrd r0.rgb,t0
			texld r1,t1

			mul r1,r1,c0
			mov r0.a,c1
			add r0,r0,r1
			
			phase

			texld r0,r0
			mov r0.a,c1

		};
	}
}
