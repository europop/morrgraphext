#include "WaterManager.h"
#include "Configuration.h"
#include "d3d8header.h"
#include "MWBridge.h"

#define WATERFORMAT D3DFVF_XYZ
#define SIZEOFWVERT 12
#define TEMPSTR_SIZE 512

//-----------------------------------------------------------------------------

WaterManager::WaterManager() {
	m_usingReflectionRenderTarget = false;
	m_reflectionSurface = 0;
	m_reflectionTexture = 0;
	m_reflectionZBuffer = 0;
	m_fakeWaterTexture = 0;
	m_defaultLandTexture = 0;
	m_waterLoadedCount = 0;
}

//-----------------------------------------------------------------------------

WaterManager::~WaterManager() {
}

//-----------------------------------------------------------------------------

bool WaterManager::Initialize(
	IDirect3DDevice9 * device,
	bool ref_enabled,
	UINT ref_tex_width,
	UINT ref_tex_height,
	UINT wave_tex_size
) {
	ShaderEffect::Initialize(device);
	assert( m_initialized );

	m_waveTexResolution = wave_tex_size;

	SetErrorFile("Water Effect Load Errors.txt");

	HRESULT res;

	//Create texture for the reflection buffer
	res = m_device->CreateTexture(
		ref_tex_width,
		ref_tex_height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_reflectionTexture,
		NULL
	);
	if ( FAILED(res) ) {
		AbortLoad("Failed to create water reflection surface.");
		return false;
	}

	// Create surface for the depth buffer
	res = m_reflectionTexture->GetSurfaceLevel( 0, &m_reflectionSurface );
	if ( FAILED(res) ) {
		AbortLoad("Failed to get reflection texture from reflection surface.");
		return false;
	}

	// Create a Z-buffer for reflection rendering
	res = m_device->CreateDepthStencilSurface(
		ref_tex_width,
		ref_tex_height,
		D3DFMT_D24X8,
		(D3DMULTISAMPLE_TYPE)0,
		0,
		true,
		&m_reflectionZBuffer,
		0
	);

	if ( FAILED(res) ) {
		AbortLoad("Failed to create reflection z-buffer");
		return false;
	}



	// Set up shader pre-processor defines
	char *tempstr = new char [TEMPSTR_SIZE];
	tempstr[0] = '\0';

	sprintf_s (
		tempstr,
		TEMPSTR_SIZE,
		"%i",
		(int)(DL_ShaderModel * 100)
	);
	AddDefine( "SHADER_MODEL", tempstr );

	sprintf_s (
		tempstr,
		TEMPSTR_SIZE,
		"%i",
		(DL_ShaderModel >= 3 ? DL_WaterWaveHeight : 0)
	);
	AddDefine( "WAVEHEIGHT", tempstr );


	if( MGEFlags & DYNAMIC_RIPPLES ) {
		AddDefine( "MGE_DYNAMICRIPPLES", 0 );

		DECLARE_MWBRIDGE
		mwBridge->TurnOldRipples(false);
	}

	if ( DL_ShaderModel >= 3 ) {
		if ( MGEFlags & EXP_FOG ) {
			AddDefine( "EXPFOG", 0 );
		}
		if ( MGEFlags & BLUR_REFLECTIONS ) {
			AddDefine( "WATERBLUR", 0 );
		}
	}

	// Load shader effect file
	ClearErrors();
	LoadEffect( "Water.fx" );

	if ( GetErrorCount() ) {
		AbortLoad("Failed to compile water effect file.");
		return false;
	}
	
	// Get Parameter Handles
	GetParamHandle( m_ehTexProj, "g_TexProj" );
	GetParamHandle( m_ehTime, "g_Time" );
	GetParamHandle( m_ehOffset, "g_Offset" );
	GetParamHandle( m_ehWaterLevel, "g_WaterLevel" );
	GetParamHandle( m_ehCausticIntensity, "g_CausticIntensity" );
	GetParamHandle( m_ehWaveTexWorldSize, "g_WaveTexWorldSize" );
	GetParamHandle( m_ehWaveTexRcpRes, "g_WaveTexRcpRes" );
	GetParamHandle( m_ehRippleOrigin, "g_RippleOrigin" );

	// Get Texture Handles
	GetTexHandle( m_ehNormalWaveCausticTexture, "NormalWaveCausticTexture" );
	GetTexHandle( m_ehDepthTexture, "DepthTexture" );
	GetTexHandle( m_ehRefractionTexture, "RefractionTexture" );
	GetTexHandle( m_ehReflectionTexture, "ReflectionTexture" );
	GetTexHandle( m_ehRainRippleTexture, "RainRippleTexture" );
	GetTexHandle( m_ehPlayerRippleTexture, "PlayerRippleTexture" );

	// Get Technique Handles
	GetTechHandle( m_ehRenderWaterFromAbove, "RenderWaterFromAbove" );
	GetTechHandle( m_ehRenderWaterFromBelow, "RenderWaterFromBelow" );
	GetTechHandle( m_ehRenderWaterNoReflect, "RenderWaterNoReflect" );
	GetTechHandle( m_ehRenderCaustics, "RenderCaustics" );
	GetTechHandle( m_ehCalculateWaves, "CalculateWaves" );
	GetTechHandle( m_ehCreatePCRipples, "CreatePCRipples" );

	if ( GetErrorCount() ) {
		AbortLoad("Failed to load water effect parameters.");
		return false;
	}

	// Check if InGame.fx demands a tesselated water mesh
	m_fineWaterMesh = 0;
	m_effect->GetBool(
		m_effect->GetParameterByName(0, "fineWaterMesh"),
		&m_fineWaterMesh
	);

	// Read demanded water wave resolution
	m_effect->GetInt(
		m_effect->GetParameterByName(0, "m_waveTexResolution"),
		&m_waveTexResolution
	);

	// Crop resolution to power of 2, min size 128
	int resolution = 128; 
	while( resolution < m_waveTexResolution ) {
		resolution <<= 1;
	}
	m_waveTexResolution = res;

	// Read wave texture world resolution
	m_effect->GetFloat(
		m_effect->GetParameterByName(0, "m_waveTexWorldResolution"),
		&m_waveTexWorldRes
	);
	if( m_waveTexWorldRes < 0.25f ) {
		m_waveTexWorldRes = 0.25f;
	}

	m_effect->SetFloat(
		m_effect->GetParameterByName(0, "waveTexWorldSize"),
		m_waveTexWorldRes * m_waveTexResolution
	);
	m_effect->SetFloat(
		m_effect->GetParameterByName(0, "waveTexRcpRes"),
		1.0f / (m_waveTexResolution - 1)
	);

	// Fake water textures
	if(FAILED(D3DXCreateTextureFromFile(
		m_device,
		"data files\\textures\\mge\\default\\fakewater.dds",
		&m_fakeWaterTexture
	))) {
		AbortLoad("Failed to load distant water texture");
		return false;
	}
	for( int i = 0; i < 32; ++i ) {
		m_fakeWaterTexture->AddRef();
	}

	//Load volumetric water normal map texture
	if(FAILED(D3DXCreateVolumeTextureFromFileEx(
		m_device,
		"data files\\textures\\mge\\default\\water\\water_NRM.dds",
		0,
		0,
		0,
		0,
		0,
		D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		0,
		0,
		&m_NewWaterTexture
	))) {
		AbortLoad("Could not load the volumetric water normal texture");
		return false;
	}
	// Create surfaces for wave calculation
	if( MGEFlags & DYNAMIC_RIPPLES ) {
		m_device->CreateTexture(
			m_waveTexResolution,
			m_waveTexResolution,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&m_waveRainTexture,
			NULL
		);

		if(FAILED(m_waveRainTexture->GetSurfaceLevel(0,&m_waveRainSurface))) {
			AbortLoad("Failed to create wave rain surface.");
			return false;
		}
		m_device->ColorFill(m_waveRainSurface,0,0x80808080);

		m_device->CreateTexture(
			m_waveTexResolution,
			m_waveTexResolution,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&m_waveRippleTexture,
			NULL
		);

		if(FAILED(
			m_waveRippleTexture->GetSurfaceLevel(0,&m_waveRippleSurface))
		) {
			AbortLoad("Failed to create wave ripple surface.");
			return false;
		}
		m_device->ColorFill(m_waveRippleSurface,0,0x80808080);

		m_device->CreateTexture(
			m_waveTexResolution,
			m_waveTexResolution,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&m_waveTempTexture,
			NULL
		);

		if(FAILED(m_waveTempTexture->GetSurfaceLevel(0,&m_waveTempSurface))) {
			AbortLoad("Failed to create wave ripple surface.");
			return false;
		}
		m_device->ColorFill(m_waveTempSurface,0,0x80808080);

		//Vertex buffer for wave texture
		RealDevice->CreateVertexBuffer(
			3*sizeof(MyVertex),
			D3DUSAGE_WRITEONLY,
			MYVERTEXFORMAT,
			D3DPOOL_DEFAULT,
			&m_wave_vb,
			0
		);
		int xx = (m_waveTexResolution) / 2;

		// Use only one tri over the whole texture to prevent seams at tri edges
		// (no idea why they even appear)
		static MyVertex waveVertices[] = {		
			{ -m_waveTexResolution / 2 - 0.5f, m_waveTexResolution / 2 - 0.5f,
				0, 1, -0.5, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ m_waveTexResolution - 0.5f, 2 * m_waveTexResolution - 0.5f,
				0, 1, 1.0, 2.0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
			{ m_waveTexResolution - 0.5f, -m_waveTexResolution - 0.5f,
				0, 1, 1.0, -1.0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
		};
		byte* VertexPointer;
		m_wave_vb->Lock( 0, 0, (void**)&VertexPointer, 0 );
		CopyMemory( VertexPointer, waveVertices, sizeof(waveVertices) );
		m_wave_vb->Unlock();
	}

	// Initialize Components
	TransformComp::InitComponent(this);
	SunlitComp::InitComponent(this);
	FogComp::InitComponent(this);
	BlendComp::InitComponent(this);

	if ( GetErrorCount() ) {
		AbortLoad("Failed to initialize water effect components.");
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------

void WaterManager::Shutdown() {

	ShaderEffect::Shutdown();

	if ( m_reflectionTexture ) {
		m_reflectionTexture->Release();
		m_reflectionTexture = 0;
	}

	if ( m_reflectionSurface ) {
		m_reflectionSurface->Release();
		m_reflectionSurface = 0;
	}

	if ( m_reflectionZBuffer ) {
		m_reflectionZBuffer->Release();
		m_reflectionZBuffer = 0;
	}

	//Parameters
	m_ehTexProj = 0;
	m_ehTime = 0;
	m_ehOffset = 0;
	m_ehWaterLevel = 0;
	m_ehCausticIntensity = 0;
	m_ehWaveTexWorldSize = 0;
	m_ehWaveTexRcpRes = 0;
	m_ehRippleOrigin = 0;

	// Textures
	m_ehNormalWaveCausticTexture = 0;
	m_ehDepthTexture = 0;
	m_ehRefractionTexture = 0;
	m_ehReflectionTexture = 0;
	m_ehRainRippleTexture = 0;
	m_ehPlayerRippleTexture = 0;

	// Techniques
	m_ehRenderWaterFromAbove = 0;
	m_ehRenderWaterFromBelow = 0;
	m_ehRenderWaterNoReflect = 0;
	m_ehRenderCaustics = 0;
	m_ehCalculateWaves = 0;
	m_ehCreatePCRipples = 0;

	// Components
	TransformComp::ShutdownComponent();
	SunlitComp::ShutdownComponent();
	FogComp::ShutdownComponent();
	BlendComp::ShutdownComponent();

	// D3D Resources
	D3DRelease(m_wave_vb);
	D3DRelease(m_water_ib);
	D3DRelease(m_water_vb1);
	D3DRelease(m_water_vb2);
	D3DRelease(m_reflectionSurface);
	D3DRelease(m_reflectionTexture);
	D3DRelease(m_reflectionZBuffer);
	D3DRelease(m_waveRainSurface);
	D3DRelease(m_waveRainTexture);
	D3DRelease(m_waveRippleSurface);
	D3DRelease(m_waveRippleTexture);
	D3DRelease(m_waveTempSurface);
	D3DRelease(m_waveTempTexture);
	D3DRelease(m_waveVB);
	D3DRelease(m_NewWaterTexture);
	D3DRelease(m_fakeWaterTexture);
	D3DRelease(m_defaultLandTexture);

	// Water Hooking
	m_waterLoadedCount = 0;

	// D3D Device
	m_device = 0;
}

//-----------------------------------------------------------------------------

void WaterManager::BeginReflectionRenderTarget() {
	assert( m_initialized );
	assert( !m_usingReflectionRenderTarget );

	m_usingReflectionRenderTarget = true;
	
	HRESULT res;
	
	res = m_device->SetRenderTarget( 0, m_reflectionSurface );
	if ( FAILED(res) ) {
		SETSTATUS("Failed to set reflection render target");
	}

	res = m_device->SetDepthStencilSurface(m_reflectionZBuffer);
	if ( FAILED(res) ) {
		SETSTATUS("Failed to set reflection Z-buffer");
	}
}

//-----------------------------------------------------------------------------

void WaterManager::EndReflectionRenderTarget() {
	assert( m_initialized );
	assert( m_usingReflectionRenderTarget );

	m_usingReflectionRenderTarget = false;
}

//-----------------------------------------------------------------------------

void WaterManager::RenderWater(
	D3DXMATRIX* view,
	D3DXMATRIX* projection,
	bool distant
) {

	assert( IsInitialized() );
	UINT passes;

	DECLARE_MWBRIDGE
	if (
		( (MGEFlags & NO_INTERIOR_DL) && !mwBridge->IsExterior() )
		|| !mwBridge->CellHasWater()
	) {
		return;
	}

	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Water");

	//Set vertex format
	m_device->SetFVF(WATERFORMAT);

	
	//Render water texture, only once per frame
	if(
		!mwBridge->IsMenu() && distant == (
			!(MGEFlags & NO_INTERIOR_DL) || mwBridge->IsExterior()
		) 
	) {
		
		//Save render target
		IDirect3DSurface *oldRenderTarget;
		m_device->GetRenderTarget(0, &oldRenderTarget);

		m_device->SetFVF(MYVERTEXFORMAT);

		//calculate dynamic ripples
		if( MGEFlags & DYNAMIC_RIPPLES ) {
			static bool resetRippleSurface = true;

			m_device->SetStreamSource( 0, m_wave_vb, 0, sizeof(MyVertex) );

			static float remainingWaveTime = 0;
			static const float waveStep = 0.0167f; // time per wave frame
			remainingWaveTime += FrameTime > 0.5f ? 0.5f : FrameTime;

			//number of wave iterations
			int numWaveSteps = (int)(remainingWaveTime / waveStep); 
			remainingWaveTime -= numWaveSteps * waveStep;

			//rain ripples
			if( mwBridge->CellHasWeather() ) {
				//reset surface when not needed next time
				resetRippleSurface = true;

				static double randomizer = 0.546372819;
				float ripplePos[2];

				static float remainingRipples = 0;

				// rain=4; thunderstorm=5; snow=8; blizzard=9;
				float currentWeather = (
					(mwBridge->GetCurrentWeather() == 4) ||
					(mwBridge->GetCurrentWeather() == 5) ||
					(mwBridge->GetCurrentWeather() == 8) ||
					(mwBridge->GetCurrentWeather() == 9))
					? 2.50f : 0;

				float nextWeather = (
					(mwBridge->GetNextWeather() == 4) ||
					(mwBridge->GetNextWeather() == 5) ||
					(mwBridge->GetNextWeather() == 8) ||
					(mwBridge->GetNextWeather() == 9))
					? 2.50f : 0;

				float rippleFrequency = 
					currentWeather* ( 1 - mwBridge->GetWeatherRatio() ) +
					nextWeather * mwBridge->GetWeatherRatio() - 1.50f;
				if( rippleFrequency < 0 ) {
					rippleFrequency = 0;
				} else {
					rippleFrequency *= 1.0f + 0.5f * (
						(mwBridge->GetCurrentWeather() == 5) *
						(1 - mwBridge->GetWeatherRatio()) +
						(mwBridge->GetNextWeather() == 5) *
						mwBridge->GetWeatherRatio()
					);

					// 150 drops per second for normal rain
					rippleFrequency *= 150.0f; 

					remainingRipples += rippleFrequency *
						( FrameTime > 0.5f ? 0.5f : FrameTime );
					RECT drop;

					while( remainingRipples >= 1.0f ) {
						//place rain ripple at random location
						for(int i=0;i<2;i++) {
							randomizer = randomizer *
								( 1337.134511337451 + (double)rand() * 0.0001 )
								+ 0.12351523;
							randomizer -= (double)((int)randomizer);
							ripplePos[i] =
								(float)randomizer * m_waveTexResolution;
						}

						drop.left = (int)(ripplePos[0]) - 2;
						drop.right = (int)(ripplePos[0]) + 2;
						drop.top = (int)(ripplePos[1]) - 1;
						drop.bottom = (int)(ripplePos[1]) + 1;
						m_device->ColorFill(
							m_waveRainSurface, &drop, 0x80804040 );

						drop.left = (int)(ripplePos[0]) - 1;
						drop.right = (int)(ripplePos[0]) + 1;
						drop.top = (int)(ripplePos[1]) - 2;
						drop.bottom = (int)(ripplePos[1]) + 2;
						m_device->ColorFill(
							m_waveRainSurface, &drop, 0x80804040 );

						drop.left = (int)(ripplePos[0]) - 1;
						drop.right = (int)(ripplePos[0]) + 1;
						drop.top = (int)(ripplePos[1]) - 1;
						drop.bottom = (int)(ripplePos[1]) + 1;
						m_device->ColorFill(
							m_waveRainSurface, &drop, 0x80800000 );

						remainingRipples -= 1;
					}
				}

				m_device->SetRenderTarget( 0, m_waveRainSurface );

				m_effect->SetTexture(
					m_ehRainRippleTexture, m_waveRainTexture );

				//apply wave equation numWaveSteps times
				
				m_effect->SetTechnique( m_ehCalculateWaves );
				m_effect->Begin( &passes, 0 );
				m_effect->BeginPass(0);
				for( int i = 0; i < numWaveSteps; i++) {
					m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );
				}
				m_effect->EndPass();
				m_effect->End();

			} else if(resetRippleSurface) {
				// no weather - clear rain ripples
				m_device->ColorFill( m_waveRainSurface, 0, 0x80808080 );
				resetRippleSurface = false;
			}
			
			// calculate waves in ripple tex
			m_device->SetRenderTarget( 0, m_waveRippleSurface );
			m_effect->SetTexture( m_ehRainRippleTexture, m_waveRippleTexture );

			// move ripple texture with player
			// move it aligned with the texture pixels to prevent visible
			// movement
			static int lastXpos = (int)floor( PlayerPos.x / m_waveTexWorldRes );
			static int lastYpos = (int)floor( PlayerPos.y / m_waveTexWorldRes );
			
			int newXpos = (int)floor( PlayerPos.x / m_waveTexWorldRes );
			int newYpos = (int)floor( PlayerPos.y / m_waveTexWorldRes );

			int shiftX = newXpos - lastXpos;
			int shiftY = newYpos - lastYpos;

			lastXpos = newXpos;
			lastYpos = newYpos;

			int shiftXp = shiftX > 0 ? + shiftX : 0;
			int shiftXn = shiftX < 0 ? - shiftX : 0;
			int shiftYp = shiftY > 0 ? + shiftY : 0;
			int shiftYn = shiftY < 0 ? - shiftY : 0;

			//shift texture by (shiftX,shiftY) pixels
			RECT source;
			source.left = 1 + shiftXp;
			source.right = m_waveTexResolution - shiftXn;
			source.top = 1 + shiftYp;
			source.bottom = m_waveTexResolution - shiftYn;

			RECT target;
			target.left = 1 + shiftXn;
			target.right = m_waveTexResolution - shiftXp;
			target.top = 1 + shiftYn;
			target.bottom = m_waveTexResolution - shiftYp;

			m_device->ColorFill( m_waveTempSurface, 0, 0x80808080 );
			m_device->StretchRect(
				m_waveRippleSurface,
				&source,
				m_waveTempSurface,
				&target,D3DTEXF_NONE
			);
			m_device->StretchRect(
				m_waveTempSurface,
				0,
				m_waveRippleSurface,
				0,
				D3DTEXF_NONE
			);

			//create waves around the player
			float rippleOrigin[2];
			if( 
				PlayerPos.z - mwBridge->WaterLevel() < 0 &&
				PlayerPos.z - mwBridge->WaterLevel() > -128.0f *
				mwBridge->PlayerHeight()
			) {
				m_effect->SetTechnique(m_ehCreatePCRipples);
				m_effect->Begin( &passes, 0 );
				m_effect->BeginPass(0);
				for( int i = 0; i < numWaveSteps; i++ ) {
					// interpolate between starting and ending point, so that
					// low framerates do not cause less waves
					rippleOrigin[0] = -(float)i / (float)numWaveSteps /
						(float)m_waveTexResolution;
					rippleOrigin[1] = rippleOrigin[0] * shiftY + 0.5f;
					rippleOrigin[0] = rippleOrigin[0] * shiftX + 0.5f;
					m_effect->SetFloatArray(
						m_ehRippleOrigin, rippleOrigin, 2 );
					m_effect->CommitChanges();
					m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );
				}
				m_effect->EndPass();
				m_effect->End();
			}

			//apply wave equation numWaveSteps times
			m_effect->SetTechnique( m_ehCalculateWaves );
			m_effect->Begin( &passes, 0 );
			m_effect->BeginPass(0);
			for( int i = 0; i < numWaveSteps; i++ ) {
				m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );
			}
			m_effect->EndPass();
			m_effect->End();
			
			// set wave texture world origin
			static float halfWaveTexWorldSize =
				0.5f * m_waveTexWorldRes * m_waveTexResolution;
			rippleOrigin[0] =
				lastXpos * m_waveTexWorldRes - halfWaveTexWorldSize;
			rippleOrigin[1] =
				lastYpos * m_waveTexWorldRes - halfWaveTexWorldSize;
			m_effect->SetFloatArray( m_ehRippleOrigin, rippleOrigin, 2 );

			m_device->SetRenderTarget( 0, oldRenderTarget );
		}
	}

	m_effect->SetTexture( m_ehNormalWaveCausticTexture, m_NewWaterTexture );
	m_effect->SetTexture( m_ehRainRippleTexture, m_waveRainTexture );
	m_effect->SetTexture( m_ehPlayerRippleTexture, m_waveRippleTexture );
	m_effect->SetTexture( m_ehReflectionTexture, m_reflectionTexture );
	
	// moved here from DL_RenderLand and added WaterLevel() translation
	D3DXMatrixTranslation(
		&m_WaterWorld,
		EyePos.x,
		EyePos.y,
		mwBridge->WaterLevel()
	);

	//Set up transforms
	D3DXMATRIX tempmat = m_WaterWorld * TransView * (*projection);
	SetWorldMatrix( &m_WaterWorld );
	SetViewMatrix( &TransView );
	SetProjectionMatrix( projection );

	//set EyePos to relative position to water plane
	D3DXVECTOR3 relativeEyePos = EyePos;
	relativeEyePos.z -= mwBridge->WaterLevel();
	SetEyePosition( (float*)&relativeEyePos );

	D3DXMATRIX T;
	T(0,0) = 0.5f;	T(0,1) =  0.0f;  T(0,2) = 0.0f;  T(0,3) = 0.0f;
	T(1,0) = 0.0f;	T(1,1) = -0.5f;  T(1,2) = 0.0f;  T(1,3) = 0.0f;
	T(2,0) = 0.0f;	T(2,1) =  0.0f;  T(2,2) = 0.5f;  T(2,3) = 0.0f;
	T(3,0) = 0.5f;	T(3,1) =  0.5f;  T(3,2) = 0.5f;  T(3,3) = 1.0f;
	
	tempmat = m_WaterWorld * TransView * (*projection)*T;
	m_effect->SetMatrix( m_ehTexProj, &tempmat );

	m_effect->SetTexture( m_ehRefractionTexture, ShaderStruct.thisframe );

	if(DL_WaterCaustics > 0 && !(distant && !(MGEFlags & SHADER_DEPTH)) ) {		
		// Copy current back buffer contents to "this frame" surface to use for
		// rendering caustics
		m_device->StretchRect(
			BackBuffer, 0, ShaderStruct.thisframeSurface, 0, D3DTEXF_NONE );

		if(distant) {
			SetBlendRange( 0.0f, -1.0f );
		} else {
			// Render caustics in the MW rendering range
			float view_dist = min( mwBridge->GetViewDistance(), 7168.0f );
			SetBlendRange( view_dist - 700.0f, view_dist );
		}

		// Pass depth texture containing both distant land and statics
		if ( g_DepthManager.IsInitialized() ) {
			m_effect->SetTexture(
				m_ehDepthTexture, g_DepthManager.GetDepthTexture() );
		}

		m_effect->CommitChanges();

		D3DPERF_BeginEvent(
			D3DCOLOR_ARGB(255, 255, 0, 0), L"Rendering caustic lights");

		m_device->SetStreamSource(
			0, ShaderStruct.buffer, 0, sizeof(MyVertex) );
		m_device->SetFVF(MYVERTEXFORMAT);

		m_effect->SetTechnique(m_ehRenderCaustics);
		m_effect->Begin( &passes, 0 );
		m_effect->BeginPass(0);
		m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		m_effect->EndPass();
		m_effect->End();

		D3DPERF_EndEvent(); // Caustics
	}

	if( g_DepthManager.IsInitialized() && distant ) {
		m_effect->SetTexture(
			m_ehDepthTexture, g_DepthManager.GetDepthTexture() );
	}
	
	// Copy current back buffer contents to "this frame" surface to use for
	// refraction
	m_device->StretchRect(
		BackBuffer, 0, ShaderStruct.thisframeSurface, 0, D3DTEXF_NONE );
	m_effect->CommitChanges();

	m_device->SetIndices(m_water_ib);
	m_device->SetStreamSource( 0, m_water_vb1, 0, SIZEOFWVERT );
	m_device->SetFVF(WATERFORMAT);

	//Draw the water
	if (mwBridge->IsAboveWater()) {
		if (
			((MGEFlags & REFLECT_SKY) || DL_WaterReflect > 0) &&
			mwBridge->WaterReflects()
		) {
			m_effect->SetTechnique(m_ehRenderWaterFromAbove);
		} else if (mwBridge->CellHasWater()) {
			m_effect->SetTechnique(m_ehRenderWaterNoReflect);
		}
	} else {
		m_effect->SetTechnique(m_ehRenderWaterFromBelow);
	}

	m_effect->Begin( &passes, 0 );
	m_effect->BeginPass(0);
	m_device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 0, 0, m_numWaterVertices, 0, m_numWaterTris );
	m_effect->EndPass();
	m_effect->End();

	//End effect
	m_effect->End();

	//Restore old EyePos
	SetEyePosition( (float*)&EyePos );

	D3DPERF_EndEvent();
}

//-----------------------------------------------------------------------------
