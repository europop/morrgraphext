#ifndef _EFFECT_COMPONENT_H_
#define _EFFECT_COMPONENT_H_

#include "ShaderEffect.h"

//-----------------------------------------------------------------------------

/// This class manages rendering objects with a specific shader effect.
class EffectComponent {
public:

	inline EffectComponent();
	inline ~EffectComponent();

	inline void InitComponent( ShaderEffect* owner );
	inline void ShutdownComponent();

private:
	/**
		This function should create any DirectX resources needed to use this
		component. as well as looking up shader parameters, textures, and
		techniques by name. It should report any errors that occur to the
		owning ShaderEffect.
	*/
	virtual void DoInit() = 0;

	/**
		This function should release any DirectX resources that were aquired,
		and zero out any shader parameters.
	*/
	virtual void DoShutdown() = 0;

protected:
	ShaderEffect* m_owner;
};

//-----------------------------------------------------------------------------

inline EffectComponent::EffectComponent() {
	m_owner = 0;
}

//-----------------------------------------------------------------------------

inline EffectComponent::~EffectComponent() {}

//-----------------------------------------------------------------------------

inline void EffectComponent::InitComponent( ShaderEffect* owner ) {
	m_owner = owner;
	DoInit();
}

//-----------------------------------------------------------------------------

inline void EffectComponent::ShutdownComponent() {
	m_owner = 0;
	DoShutdown();
}

//-----------------------------------------------------------------------------

#endif