#ifndef _BLEND_COMP_H_
#define _BLEND_COMP_H_

#include "EffectComponent.h"

//-----------------------------------------------------------------------------

/**
	This class manages effects that use fog.
*/

class BlendComp : public EffectComponent {
protected:
	virtual void InitComponent( ShaderEffect* owner );
	virtual void ShutdownComponent();

public:
	/// Sets begining and end of blend area.
	void SetBlendRange( float start, float end );

private:
	D3DXHANDLE m_ehBlendStart;
	D3DXHANDLE m_ehBlendEnd;
};

//-----------------------------------------------------------------------------

#endif