//Author: Scanti
//Brightens and greyscales dark areas without affecting bright areas
//This should be broken. the constant registers don't accept numbers > 1

Technique T0
{
	Pass P0
	{
		PixelShader=asm
		{
			ps.1.0    //The lower the version-the more cards it will work with
			def c0,1.20,2.36,0.44,1
			def c1,2.40,4.72,0.88,1
			def c2,0.2,0.2,0.2,1
			tex t0
			dp3 r0,t0,c0          //convert to mono and multiply by 4
			dp3 r1,t0,c1          //convert to mono and multiply by 8
			add_sat r1,r1,c2      //add an offset, so screen isn't too grey
			lrp_sat r0,r1,t0,r0   //interpolate between greyscale and colour using modified colour brightness
			mov r0.a,c0           //set the alpha channel
		};
	}
}
