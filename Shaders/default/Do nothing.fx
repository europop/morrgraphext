//Author: Timeslip
//Does absolutely nothing
//Illistrates the basic layout of a technique

Technique T0				//Begin the technique
{
	Pass P0			//Begin the pass (Names don't matter, but are generally P0, P1, P2 and so on)
	{
		VertexShader = 1540;	//Not using a vertex shader. This line can be left out, but if you want to use a vertex shader then 
					//make a note of the FVF used.
		PixelShader = asm	//Using an assembly style shader
		{
			ps.1.0		//Use a version 1.0 pixel shader
			tex t0		//Sample the backbuffer
			mov r0,t0	//Move the result into the output register
		};			//Remember this ';' at all costs!!!
	}				//End the pass
}					//End the technique. (Only ever use 1 technique in a fullscreen shader)
