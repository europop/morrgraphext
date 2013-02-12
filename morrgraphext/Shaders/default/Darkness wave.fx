//Author: Timeslip
//Lights up the screen for a little while, before darkening it again

VECTOR sinvar;

Technique T0
{
     Pass P0
     {
           PixelshaderConstant[1]=<sinvar>;
           PixelShader=asm
           {
                 ps.1.0
                 tex t0
                 mul r0.rgb, c1, t0
                 +mov r0.a, t0
           };
     }
}
