//Author: Timeslip
//Only draws areas where some change has happened since the last frame

texture lastframe;

Technique T0
{
	Pass P0
	{
		Texture[1] = <lastframe>;	//Use the previous frame as the second texture
		PixelShader = asm
		{
			ps.1.3
			def c0,0,0,0,1	//Used for filling the alpha channel
			tex t0			//Sample this frame
			tex t1			//Sample the last frame
			sub_sat r0,t0,t1	//Subtract each from each other and clamp between 0 and 1
			sub_sat r1,t1,t0
			add r0.rgb,r0,r1	//Then add them. This is the same as abs(r0-r1) would be if ps 1.x supported the abs instruction
			+mov r0.a,c0		//Use the scaler pipe to fill in the alpha channel
		};
	}
}
