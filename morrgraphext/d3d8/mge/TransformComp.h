#ifndef _TRANSFORM_COMP_H_
#define _TRANSFORM_COMP_H_

#include "EffectComponent.h"

//-----------------------------------------------------------------------------

/// This component adds parameters that control 3D transforms
class TransformComp : public EffectComponent {
protected:
	virtual void InitComponent( ShaderEffect* owner );
	virtual void ShutdownComponent();

public:
	void SetViewMatrix( D3DXMATRIX* view );
	void SetProjectionMatrix( D3DXMATRIX* projection );
	void SetWorldMatrix( D3DXMATRIX* world );

private:
	D3DXHANDLE m_ehView;
	D3DXHANDLE m_ehProj;
	D3DXHANDLE m_ehWorld;};

//-----------------------------------------------------------------------------

#endif