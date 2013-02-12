//Author: Scanti
//A ps.1.0 version of the nightvision shader

Technique T0
{
	Pass P0
	{
		PixelShader=asm
		{
			ps.1.0
			def c0,0.3,0.59,0.11,0.25
			def c1,0.45,1,0.45,1
			tex t0               //sample current pixel
			mov_x2 r1,c0         //Because the constant registers are clamped between 0 and 1
			dp3 r0,t0,r1         //convert to grey and multiply by 4
			mul_x2 r0,r0,c1      //double the green component.
			mov r0.a,c1          //set alpha
		};
	}
}
