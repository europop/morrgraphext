//Author: Timeslip
//An example of a vertex shader

Technique T0
{
	Pass P0
	{
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
    		} asm {
			vs.1.1
			def c0,1,1,0,0
			def c1,2,2,1,1
			sge r0.xzw,v0,c0
			slt r0.y,v0,c0
			mul r0,r0,c1
			sub r0,r0,c0
			mov oPos,r0
			mov oT0,v7
		};
	}
}
