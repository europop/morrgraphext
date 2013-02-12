#include "DepthManager.h"
#include "Configuration.h"
#include "d3d8header.h"
#include <fstream>

//-----------------------------------------------------------------------------

DepthManager::DepthManager() {
	m_initialized = false;
	m_usingDepthRenderTarget = false;
	m_clearFlag = false;
	m_clearTexFlag = false;
	m_depthSurface = 0;
	m_depthTexture = 0;
	m_depthZBuffer = 0;
	m_depthEffect = 0;
	m_device = 0;
}

//-----------------------------------------------------------------------------

DepthManager::~DepthManager() {
	if (m_initialized) {
		Shutdown();
	}
}

//-----------------------------------------------------------------------------

bool DepthManager::Initialize(
	IDirect3DDevice9* device,
	UINT width,
	UINT height
) {
	assert( !m_initialized );
	assert( device != 0 );

	m_device = device;

	HRESULT res;

	m_initialized = true;

	//Create texture for the depth buffer
	res = m_device->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&m_depthTexture,
		NULL
	);
	if ( FAILED(res) ) {
		SETSTATUS("Failed to create depth buffer surface.");
		Shutdown();
		return false;
	}

	// Create surface for the depth buffer
	res = m_depthTexture->GetSurfaceLevel( 0, &m_depthSurface );
	if ( FAILED(res) ) {
		SETSTATUS(
			"Failed to get depth buffer texture from depth buffer surface.");
		Shutdown();
		return false;
	}

	//Initialize the depth surface to all zeros.
	ZeroDepthBuffer();

	// Create a Z-buffer for depth rendering
	res = m_device->CreateDepthStencilSurface(
		width,
		height,
		D3DFMT_D24X8,
		(D3DMULTISAMPLE_TYPE)0,
		0,
		true,
		&m_depthZBuffer,
		0
	);

	if ( FAILED(res) ) {
		SETSTATUS(
			"Failed to create depth z-buffer");
		Shutdown();
		return false;
	}

	// Load shader effect file
	LPD3DXBUFFER errors = 0;
	
	res = D3DXCreateEffectFromFile(
		m_device,
		"Data files\\shaders\\distantland\\Depth.fx",
		0,
		0,
		0,
		0,
		&m_depthEffect,
		&errors
	);

	if ( FAILED(res) ) {
		if ( errors ) {
			std::ofstream err_file ("Shader Compiler Errors - Depth.fx.txt");
			err_file.write ((const char*)errors->GetBufferPointer(), errors->GetBufferSize() );
			err_file.close ();
			errors->Release();
			errors = 0;
		}
	
		SETSTATUS ("Could not depth effect file.");
		Shutdown();
		return false;
	}

	// Get Parameter Handles
	m_ehView = m_depthEffect->GetParameterByName( 0, "g_View" );
	m_ehProj = m_depthEffect->GetParameterByName( 0, "g_Proj" );
	m_ehWorld = m_depthEffect->GetParameterByName( 0, "g_World" );
	m_ehMatrixPalette =
		m_depthEffect->GetParameterByName( 0, "g_MatrixPalette" );
	m_ehBlendWeightCount =
		m_depthEffect->GetParameterByName( 0, "g_BlendWeightCount" );
	m_ehAlphaTest = m_depthEffect->GetParameterByName( 0, "g_AlphaTest" );
	m_ehAlphaTestTexture =
		m_depthEffect->GetParameterByName( 0, "AlphaTestTexture" );

	// Get Technique Handles
	m_ehStaticDepth = m_depthEffect->GetTechniqueByName( "StaticDepth" ); 
	m_ehNoAlphaDepth = m_depthEffect->GetTechniqueByName( "NoAlphaDepth" ); 
	m_ehRenderGenericDepth =
		m_depthEffect->GetTechniqueByName( "RenderGenericDepth" ); 
	m_ehClearDepth = m_depthEffect->GetTechniqueByName( "ClearDepth" ); 

	if (
		!m_ehView ||
		!m_ehProj ||
		!m_ehWorld ||
		!m_ehMatrixPalette ||
		!m_ehBlendWeightCount ||
		!m_ehAlphaTest ||
		!m_ehAlphaTestTexture ||
		!m_ehStaticDepth ||
		!m_ehNoAlphaDepth ||
		!m_ehRenderGenericDepth ||
		!m_ehClearDepth
	) {
		SETSTATUS (
			"Failed to find a necessary parameter or technique in depth shader"
		);
		Shutdown();
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------

void DepthManager::Shutdown() {
	assert( m_initialized );

	m_initialized = false;

	m_clearFlag = false;

	if ( m_depthTexture ) {
		m_depthTexture->Release();
		m_depthTexture = 0;
	}

	if ( m_depthSurface ) {
		m_depthSurface->Release();
		m_depthSurface = 0;
	}

	if ( m_depthZBuffer ) {
		m_depthZBuffer->Release();
		m_depthZBuffer = 0;
	}

	if ( m_depthEffect ) {
		m_depthEffect->Release();
		m_depthEffect = 0;
	}

	// Parameters
	m_ehView = 0;
	m_ehProj = 0;
	m_ehWorld = 0;
	m_ehMatrixPalette = 0;
	m_ehBlendWeightCount = 0;
	m_ehAlphaTest = 0;
	m_ehAlphaTestTexture = 0;
	
	//Techniques
	m_ehStaticDepth = 0;
	m_ehNoAlphaDepth = 0;
	m_ehRenderGenericDepth = 0;
	m_ehClearDepth = 0;

	m_device = 0;
}

//-----------------------------------------------------------------------------

void DepthManager::BeginDepthRenderTarget() {
	assert( m_initialized );
	assert( !m_usingDepthRenderTarget );

	m_usingDepthRenderTarget = true;
	
	HRESULT res;
	
	res = m_device->SetRenderTarget( 0, m_depthSurface );
	if ( FAILED(res) ) {
		SETSTATUS("Failed to set depth render target");
	}

	res = m_device->SetDepthStencilSurface(m_depthZBuffer);
	if ( FAILED(res) ) {
		SETSTATUS("Failed to set depth Z-buffer");
	}

	// Check if the depth buffer has been flagged to be cleared the next time
	// this function is called.
	if ( m_clearFlag ) {
		_DoClearDepthBuffer();
		m_clearFlag = false;
	}
}

//-----------------------------------------------------------------------------

void DepthManager::EndDepthRenderTarget() {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	m_usingDepthRenderTarget = false;
}

//-----------------------------------------------------------------------------

void DepthManager::BeginStaticDepthRendering( bool alpha_test ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	HRESULT res;

	if ( alpha_test ) {
		res = m_depthEffect->SetTechnique(m_ehStaticDepth);
	} else {
		res = m_depthEffect->SetTechnique(m_ehNoAlphaDepth);
	}

	UINT passes;
	m_depthEffect->Begin( &passes, 0 );
	m_depthEffect->BeginPass(0);
}

//-----------------------------------------------------------------------------

void DepthManager::BeginGenericDepthRendering() {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	HRESULT res = m_depthEffect->SetTechnique(m_ehRenderGenericDepth);

	UINT passes;
	res = m_depthEffect->Begin( &passes, 0 );
	res = m_depthEffect->BeginPass(0);
}

//-----------------------------------------------------------------------------

void DepthManager::SetAlphaTestTexture( IDirect3DTexture9* texture ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	m_depthEffect->SetTexture( m_ehAlphaTestTexture, texture );
}

//-----------------------------------------------------------------------------

void DepthManager::SetViewMatrix( D3DXMATRIX* view ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	m_depthEffect->SetMatrix( m_ehView, view );
}

//-----------------------------------------------------------------------------

void DepthManager::SetProjectionMatrix( D3DXMATRIX* projection ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	m_depthEffect->SetMatrix( m_ehProj, projection );
}

//-----------------------------------------------------------------------------

void DepthManager::SetWorldMatrix( D3DXMATRIX* world ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	m_depthEffect->SetMatrix( m_ehWorld, world );
}

//-----------------------------------------------------------------------------

void DepthManager::SetWorldMatrixArray( D3DXMATRIX* world, UINT count ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );
	
	m_depthEffect->SetMatrixArray( m_ehMatrixPalette, world, count );
}

//-----------------------------------------------------------------------------

void DepthManager::SetBlendWeightCount( INT count ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	m_depthEffect->SetInt( m_ehBlendWeightCount, count );
}

//-----------------------------------------------------------------------------

void DepthManager::SetAlphaTest( bool is_alpha_tested ) {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	m_depthEffect->SetBool( m_ehAlphaTest, is_alpha_tested );
}

//-----------------------------------------------------------------------------

void DepthManager::CommitChanges() {
	m_depthEffect->CommitChanges();
}

//-----------------------------------------------------------------------------

D3DXHANDLE m_ehView;
D3DXHANDLE m_ehProj;
D3DXHANDLE m_ehWorld;
D3DXHANDLE m_ehMatrixPalette;
D3DXHANDLE m_ehBlendWeightCount;
D3DXHANDLE m_ehIsSkinned;
D3DXHANDLE m_ehAlphaTest;

//-----------------------------------------------------------------------------

void DepthManager::EndDepthRendering() {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );
	m_depthEffect->EndPass();
	m_depthEffect->End();
}

//-----------------------------------------------------------------------------

void DepthManager::ZeroDepthBuffer() {

	assert( m_initialized );
	
	m_device->ColorFill( m_depthSurface, 0, 0 );
}

//-----------------------------------------------------------------------------

void DepthManager::_DoClearDepthBuffer() {
	assert( m_initialized );
	assert( m_usingDepthRenderTarget );

	if( m_clearTexFlag ) {
		// Render a full-screen quad with a shader that will set the depth texture
		// to FLT_MAX

		m_depthEffect->SetTechnique(m_ehClearDepth);

		UINT passes;
		m_depthEffect->Begin( &passes, 0 );
		m_depthEffect->BeginPass(0);

		m_device->SetStreamSource(0,ShaderStruct.buffer,0,sizeof(MyVertex));
		m_device->SetFVF(MYVERTEXFORMAT);
		m_device->SetRenderState(D3DRS_CULLMODE,2);
		m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		m_depthEffect->EndPass();
		m_depthEffect->End();
	}

	//Clear the depth buffer
	m_device->Clear( 0,0,D3DCLEAR_ZBUFFER,0,1,0 );
}

//-----------------------------------------------------------------------------
