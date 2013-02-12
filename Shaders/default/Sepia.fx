//Author: Scanti

Technique T0
{
     Pass P0
     {
           PixelShader=asm
           {
                 ps.1.0    
                 def c0,1,0.9,0.5,1
                 def c1,0.2,0.05,0,1
                 def c2,0.3,0.59,0.11,1
                 tex t0
                 dp3 r0,t0,c2                    
                 lrp_sat r0,r0,c0,c1  
                 mov r0.a,c0          
           };
     }
}
