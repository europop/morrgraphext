#include "FogComp.h"
#include "d3d8header.h"

//-----------------------------------------------------------------------------

void FogComp::InitComponent( ShaderEffect* owner ) {
	// Get Parameter Handles
	m_owner->GetParamHandle( m_ehFogRange, "g_FogRange" );
	m_owner->GetParamHandle( m_ehFogStart, "g_FogStart" );
	m_owner->GetParamHandle( m_ehFogCol, "g_FogCol" );
	m_owner->GetParamHandle( m_ehSkyCol, "g_SkyCol" );
}

//-----------------------------------------------------------------------------

void FogComp::ShutdownComponent() {
	//Parameters
	m_ehFogRange = 0;
	m_ehFogStart = 0;
	m_ehFogCol = 0;
	m_ehSkyCol = 0;
}

//-----------------------------------------------------------------------------

void FogComp::SetFogRange( float start, float end ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloat( m_ehFogStart, start );
	m_owner->GetD3DEffect()->SetFloat( m_ehFogRange, end );
}

//-----------------------------------------------------------------------------

void FogComp::SetFogColor( DWORD color ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloatArray( m_ehFogCol, (float*)&RGBVECTOR(color), 3 );
}

//-----------------------------------------------------------------------------

void FogComp::SetSkyColor( DWORD color ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloatArray( m_ehSkyCol, (float*)&RGBVECTOR(color), 3 );
}

//-----------------------------------------------------------------------------
