//Author: Scanti
//Changes the colour saturation based on the value of the saturate vector. 0 = greyscale, 0.5 = normal, 1 - double saturation

vector saturate = {1,1,1,1};

Technique T0
{
     Pass P0
     {
           PixelShaderConstant[1]=<saturate>;

           PixelShader=asm
           {
                 ps.1.0    
                 def c0,0.3,0.59,0.11,1
                 
                 tex t0
                 dp3 r0,t0,c0
                 sub r1,t0,r0
                 add r1,t0,r1                    
                 lrp_sat r0,c1,r1,r0  
                 mov r0.a,c0          
           };
     }
}
