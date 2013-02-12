#include "BlendComp.h"
#include "d3d8header.h"

//-----------------------------------------------------------------------------

void BlendComp::InitComponent( ShaderEffect* owner ) {
	// Get Parameter Handles
	m_owner->GetParamHandle( m_ehBlendStart, "g_BlendStart" );
	m_owner->GetParamHandle( m_ehBlendEnd, "g_BlendEnd" );
}

//-----------------------------------------------------------------------------

void BlendComp::ShutdownComponent() {
	//Parameters
	m_ehBlendStart = 0;
	m_ehBlendEnd = 0;
}

//-----------------------------------------------------------------------------

void BlendComp::SetBlendRange( float start, float end ) {
	assert( m_owner->IsInitialized() );
	m_owner->GetD3DEffect()->SetFloat( m_ehBlendStart, start );
	m_owner->GetD3DEffect()->SetFloat( m_ehBlendEnd, end );
}

//-----------------------------------------------------------------------------
