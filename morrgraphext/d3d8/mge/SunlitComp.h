#ifndef _SUNLIT_COMP_H_
#define _SUNLIT_COMP_H_

#include "EffectComponent.h"

//-----------------------------------------------------------------------------

/// This component adds parameters to control lighting from the sun.
class SunlitComp : public EffectComponent {
protected:
	virtual void InitComponent( ShaderEffect* owner );
	virtual void ShutdownComponent();

public:
	void SetSunVisibility( float visibility );
	void SetSunColor( float* color3 );
	void SetSunPosition( float* vector3 );
	void SetSunVector( float* vector3 );
	void SetSunAmbientColor( float* color3 );
	void SetEyePosition( float* vector3 );
private:
	D3DXHANDLE m_ehSunVis;
	D3DXHANDLE m_ehSunCol;
	D3DXHANDLE m_ehSunPos;
	D3DXHANDLE m_ehSunVec;
	D3DXHANDLE m_ehSunAmb;
	D3DXHANDLE m_ehEyePos;
};

//-----------------------------------------------------------------------------

#endif