//Author: Scanti
//I cant really describe what this one does. Just try it; it's cool.

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
           }
           asm {
               vs.1.1
               def c0,1,1,0,0
               def c1,2,2,1,1
 
               def c2,0.01,0.01,0,0
               def c3,0.98,0.98,1,1

               def c4,0.02,0.02,0,0
               def c5,0.96,0.96,0,0

               def c6,0.03,0.03,0,0
               def c7,0.94,0.94,0,0

               sge r0.xzw,v0,c0
               slt r0.y,v0,c1
               mul r0,r0,c1
               sub r0,r0,c0
               mov oPos,r0

               mov oT0,v7
           
               mov r1,v8
               mul r1,r1,c3
               add r1,r1,c2
               mov oT1,r1

               mov r1,v9
               mul r1,r1,c5
               add r1,r1,c4
               mov oT2,r1

               mov r1,v10
               mul r1,r1,c7
               add r1,r1,c6
               mov oT3,r1
       };

       PixelShader=asm
       {
           ps.1.1

           def c0,0,0,0,1
           
           tex t0
           tex t1
           tex t2
           tex t3

           add_d2 r0,t0,t1
           add_d2 r1,t2,t3
           add_d2 r0,r0,r1

           mov r0.a,c0
       };
   }
}
