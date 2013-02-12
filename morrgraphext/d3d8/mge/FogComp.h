#ifndef _FOG_COMP_H_
#define _FOG_COMP_H_

#include "EffectComponent.h"

//-----------------------------------------------------------------------------

/**
	This class manages effects that use fog.
*/

class FogComp : public EffectComponent {
protected:
	virtual void InitComponent( ShaderEffect* owner );
	virtual void ShutdownComponent();

public:
	/// Sets begining and end of fog.
	void SetFogRange( float start, float end );

	/// Sets fog color
	void SetFogColor( DWORD color );

	/// Sets sky color.
	void SetSkyColor( DWORD color );

private:
	D3DXHANDLE m_ehFogRange;
	D3DXHANDLE m_ehFogStart;
	D3DXHANDLE m_ehFogCol;
	D3DXHANDLE m_ehSkyCol;
};

//-----------------------------------------------------------------------------

#endif