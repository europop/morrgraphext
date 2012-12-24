//Author: Scanti
//Performs a blur using vertex shaders to do most of the maths work. Should be faster than using pixel shaders
//Make sure you change the constants in the vertex shader, or this effect will appear to do nothing

Texture thisframe;
Texture lastpass;

PixelShader PS=asm
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

VertexShader VS=decl
{
     stream 0;
     float v0[4];  // Position RH
     float v7[2];  // Texture Coord1
     float v8[2];  // Texture Coord2
     float v9[2];  // Texture Coord3
     float v10[2]; // Texture Coord4
     float v11[2]; // Texture Coord5
     float v12[2]; // Texture Coord6
} asm {
     vs.1.1
     def c0,1,1,0,0
     def c1,2,2,1,1
                                      // x,y = screen resolution
                                      // i.e. 800x600
     //def c2,0.00125,0.0,0,0           // c2,1/x,0,0,0    i.e. 1/800,0,0,0
     //def c3,0.0,0.00166667,0,0        // c3,0,1/y,0,0    i.e. 0,1/600,0,0
     //def c4,0.00125,0.00166667,0,0    // c4,1/x,1/y,0,0  i.e. 1/800,1/600,0,0
	def c2,0.001953125,0,0,0
	def c3,0,0.001953125,0,0
	def c4,0.001953125,0.001953125,0,0

     sge r0.xzw,v0,c0
     slt r0.y,v0,c1
     mul r0,r0,c1
     sub r0,r0,c0
     mov oPos,r0

     mov oT0,v7
                 
     mov r1,v8
     add r1,r1,c2
     mov oT1,r1

     mov r1,v9
     add r1,r1,c3
     mov oT2,r1

     mov r1,v10
     add r1,r1,c4
     mov oT3,r1
};

Technique T0
{
     Pass P0
     {
           Texture[1]=<thisframe>;
           Texture[2]=<thisframe>;
           Texture[3]=<thisframe>;

           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P1
     {
           Texture[0]=<lastpass>;
           Texture[1]=<lastpass>;
           Texture[2]=<lastpass>;
           Texture[3]=<lastpass>;

           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P2
     {
           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P3
     {
           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P4
     {
           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P5
     {
           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P6
     {
           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P7
     {
           VertexShader = <VS>;
           PixelShader = <PS>;
     }
     Pass P8
     {
           VertexShader = <VS>;
           PixelShader = <PS>;
     }            
}
