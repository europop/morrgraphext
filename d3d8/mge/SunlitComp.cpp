#include "SunlitComp.h"
#include "d3d8header.h"

//-----------------------------------------------------------------------------

void SunlitComp::InitComponent( ShaderEffect* owner ) {
	// Get Parameter Handles
	m_owner->GetParamHandle( m_ehSunVis, "g_SunVis" );
	m_owner->GetParamHandle( m_ehSunCol, "g_SunCol" );
	m_owner->GetParamHandle( m_ehSunPos, "g_SunPos" );
	m_owner->GetParamHandle( m_ehSunVec, "g_SunVec" );
	m_owner->GetParamHandle( m_ehSunAmb, "g_SunAmb" );
	m_owner->GetParamHandle( m_ehEyePos, "g_EyePos" );
}

//-----------------------------------------------------------------------------

void SunlitComp::ShutdownComponent() {

	//Parameters
	m_ehSunVis = 0;
	m_ehSunCol = 0;
	m_ehSunPos = 0;
	m_ehSunVec = 0;
	m_ehSunAmb = 0;
	m_ehEyePos = 0;
}

//-----------------------------------------------------------------------------

void SunlitComp::SetSunVisibility( float visibility ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloat( m_ehSunVis, visibility );
}

//-----------------------------------------------------------------------------

void SunlitComp::SetSunColor( float* color3 ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloatArray( m_ehSunCol, color3, 3 );
}

//-----------------------------------------------------------------------------

void SunlitComp::SetSunPosition( float* vector3 ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloatArray( m_ehSunPos, vector3, 3 );
}

//-----------------------------------------------------------------------------

void SunlitComp::SetSunVector( float* vector3 ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloatArray( m_ehSunVec, vector3, 3 );
}

//-----------------------------------------------------------------------------

void SunlitComp::SetSunAmbientColor( float* color3 ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloatArray( m_ehSunAmb, color3, 3 );
}

//-----------------------------------------------------------------------------

void SunlitComp::SetEyePosition( float* vector3 ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloatArray( m_ehEyePos, vector3, 3 );
}

//-----------------------------------------------------------------------------
