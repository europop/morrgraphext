#include "TransformComp.h"
#include "d3d8header.h"

//-----------------------------------------------------------------------------

void TransformComp::InitComponent( ShaderEffect* owner ) {
	// Get Parameter Handles
	m_owner->GetParamHandle( m_ehView, "g_View" );
	m_owner->GetParamHandle( m_ehProj, "g_Proj" );
	m_owner->GetParamHandle( m_ehWorld, "g_World" );
}

//-----------------------------------------------------------------------------

void TransformComp::ShutdownComponent() {
	//Parameters
	m_ehView = 0;
	m_ehProj = 0;
	m_ehWorld = 0;
}

//-----------------------------------------------------------------------------

void TransformComp::SetViewMatrix( D3DXMATRIX* view ) {
	m_owner->GetD3DEffect()->SetMatrix( m_ehView, view );
}

//-----------------------------------------------------------------------------

void TransformComp::SetProjectionMatrix( D3DXMATRIX* projection ) {
	m_owner->GetD3DEffect()->SetMatrix( m_ehProj, projection );
}

//-----------------------------------------------------------------------------

void TransformComp::SetWorldMatrix( D3DXMATRIX* world ) {
	m_owner->GetD3DEffect()->SetMatrix( m_ehWorld, world );
}

//-----------------------------------------------------------------------------

