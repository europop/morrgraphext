//Author: Timeslip
//Applies a realistic bloom effect

texture thisframe;
texture lastpass;

PixelShader PS = asm
{
	ps.1.4
	texcrd r0.rgb,t0
	texcrd r1.rgb,t1
	add r1.rg,r1,c0
	phase			//Second phase
	texld r0,r0
	texld r1,r1
	mul r1.rgb,r1,c1
	add r0.rgb,r0,r1
	+mov r0.a,c1
};
Technique T0
{
	Pass BrightPass
	{
		PixelShader = asm
		{
			ps.1.3
			def c0,0,0,0,1
			def c1,0.99,0.99,0.99,0
			tex t0
			sub r0,t0,c1
			mov r1,r0
			cmp r0,r1,r1,c0
			mov_x4 r0,r0
			mov_x4_sat r0,r0
		};
	}
	Pass HBlurA1
	{
		Texture[0] = <lastpass>;
		Texture[1] = <lastpass>;
		PixelShaderConstant[0] = { -0.009375,0 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA2
	{
		PixelShaderConstant[0] = { -0.0078125,0 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA3
	{
		PixelShaderConstant[0] = { -0.00625,0 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA4
	{
		PixelShaderConstant[0] = { -0.0046875,0 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA5
	{
		PixelShaderConstant[0] = { -0.003125,0 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA6
	{
		PixelShaderConstant[0] = { -0.0015625,0 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA7
	{
		PixelShaderConstant[0] = { 0,0 };
		PixelShaderConstant[1] = { 0.199471,0.199471,0.199471,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA8
	{
		PixelShaderConstant[0] = { 0.0015625,0 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA9
	{
		PixelShaderConstant[0] = { 0.003125,0 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA10
	{
		PixelShaderConstant[0] = { 0.0046875,0 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA11
	{
		PixelShaderConstant[0] = { 0.00625,0 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA12
	{
		PixelShaderConstant[0] = { 0.0078125,0 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass HBlurA13
	{
		PixelShaderConstant[0] = { 0.009375,0 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA1
	{
		PixelShaderConstant[0] = { 0,-0.009375 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA2
	{
		PixelShaderConstant[0] = { 0,-0.0078125 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA3
	{
		PixelShaderConstant[0] = { 0,-0.00625 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA4
	{
		PixelShaderConstant[0] = { 0,-0.0046875 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA5
	{
		PixelShaderConstant[0] = { 0,-0.003125 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA6
	{
		PixelShaderConstant[0] = { 0,-0.0015625 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA7
	{
		PixelShaderConstant[0] = { 0,0 };
		PixelShaderConstant[1] = { 0.199471,0.199471,0.199471,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA8
	{
		PixelShaderConstant[0] = { 0,0.0015625 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA9
	{
		PixelShaderConstant[0] = { 0,0.003125 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA10
	{
		PixelShaderConstant[0] = { 0,0.0046875 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA11
	{
		PixelShaderConstant[0] = { 0,0.00625 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA12
	{
		PixelShaderConstant[0] = { 0,0.0078125 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass VBlurA13
	{
		PixelShaderConstant[0] = { 0,0.009375 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB1
	{
		PixelShaderConstant[0] = { -0.009375,0 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB2
	{
		PixelShaderConstant[0] = { -0.0078125,0 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB3
	{
		PixelShaderConstant[0] = { -0.00625,0 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB4
	{
		PixelShaderConstant[0] = { -0.0046875,0 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB5
	{
		PixelShaderConstant[0] = { -0.003125,0 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB6
	{
		PixelShaderConstant[0] = { -0.0015625,0 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB7
	{
		PixelShaderConstant[0] = { 0,0 };
		PixelShaderConstant[1] = { 0.199471,0.199471,0.199471,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB8
	{
		PixelShaderConstant[0] = { 0.0015625,0 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB9
	{
		PixelShaderConstant[0] = { 0.003125,0 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB10
	{
		PixelShaderConstant[0] = { 0.0046875,0 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB11
	{
		PixelShaderConstant[0] = { 0.00625,0 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB12
	{
		PixelShaderConstant[0] = { 0.0078125,0 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass HBlurB13
	{
		PixelShaderConstant[0] = { 0.009375,0 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB1
	{
		PixelShaderConstant[0] = { 0,-0.009375 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB2
	{
		PixelShaderConstant[0] = { 0,-0.0078125 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB3
	{
		PixelShaderConstant[0] = { 0,-0.00625 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB4
	{
		PixelShaderConstant[0] = { 0,-0.0046875 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB5
	{
		PixelShaderConstant[0] = { 0,-0.003125 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB6
	{
		PixelShaderConstant[0] = { 0,-0.0015625 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB7
	{
		PixelShaderConstant[0] = { 0,0 };
		PixelShaderConstant[1] = { 0.199471,0.199471,0.199471,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB8
	{
		PixelShaderConstant[0] = { 0,0.0015625 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB9
	{
		PixelShaderConstant[0] = { 0,0.003125 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB10
	{
		PixelShaderConstant[0] = { 0,0.0046875 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB11
	{
		PixelShaderConstant[0] = { 0,0.00625 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB12
	{
		PixelShaderConstant[0] = { 0,0.0078125 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass VBlurB13
	{
		PixelShaderConstant[0] = { 0,0.009375 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC1
	{
		PixelShaderConstant[0] = { -0.009375,0 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC2
	{
		PixelShaderConstant[0] = { -0.0078125,0 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC3
	{
		PixelShaderConstant[0] = { -0.00625,0 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC4
	{
		PixelShaderConstant[0] = { -0.0046875,0 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC5
	{
		PixelShaderConstant[0] = { -0.003125,0 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC6
	{
		PixelShaderConstant[0] = { -0.0015625,0 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC7
	{
		PixelShaderConstant[0] = { 0,0 };
		PixelShaderConstant[1] = { 0.199471,0.199471,0.199471,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC8
	{
		PixelShaderConstant[0] = { 0.0015625,0 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC9
	{
		PixelShaderConstant[0] = { 0.003125,0 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC10
	{
		PixelShaderConstant[0] = { 0.0046875,0 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC11
	{
		PixelShaderConstant[0] = { 0.00625,0 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC12
	{
		PixelShaderConstant[0] = { 0.0078125,0 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass HBlurC13
	{
		PixelShaderConstant[0] = { 0.009375,0 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC1
	{
		PixelShaderConstant[0] = { 0,-0.009375 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC2
	{
		PixelShaderConstant[0] = { 0,-0.0078125 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC3
	{
		PixelShaderConstant[0] = { 0,-0.00625 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC4
	{
		PixelShaderConstant[0] = { 0,-0.0046875 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC5
	{
		PixelShaderConstant[0] = { 0,-0.003125 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC6
	{
		PixelShaderConstant[0] = { 0,-0.0015625 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC7
	{
		PixelShaderConstant[0] = { 0,0 };
		PixelShaderConstant[1] = { 0.199471,0.199471,0.199471,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC8
	{
		PixelShaderConstant[0] = { 0,0.0015625 };
		PixelShaderConstant[1] = { 0.176033,0.176033,0.176033,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC9
	{
		PixelShaderConstant[0] = { 0,0.003125 };
		PixelShaderConstant[1] = { 0.120985,0.120985,0.120985,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC10
	{
		PixelShaderConstant[0] = { 0,0.0046875 };
		PixelShaderConstant[1] = { 0.064759,0.064759,0.064759,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC11
	{
		PixelShaderConstant[0] = { 0,0.00625 };
		PixelShaderConstant[1] = { 0.026995,0.026995,0.026995,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC12
	{
		PixelShaderConstant[0] = { 0,0.0078125 };
		PixelShaderConstant[1] = { 0.008764,0.008764,0.008764,1 };
		PixelShader = <PS>;
	}
	Pass VBlurC13
	{
		PixelShaderConstant[0] = { 0,0.009375 };
		PixelShaderConstant[1] = { 0.002216,0.002216,0.002216,1 };
		PixelShader = <PS>;
	}
	Pass combine
	{
		Texture[0]=<thisframe>;
		PixelShader = asm {
			ps.1.1
			tex t0
			tex t1
			mov_x2 r1,t1
			add_sat r0,t0,t1
		};
	}
}
