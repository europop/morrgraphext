#ifndef _WATER_MANAGER_H_
#define _WATER_MANAGER_H_

#include "FogComp.h"
#include "TransformComp.h"
#include "SunlitComp.h"
#include "FogComp.h"
#include "BlendComp.h"

//-----------------------------------------------------------------------------

/**
	This class is responsible for rendering MGE's improved water.
*/

class WaterManager :
	public ShaderEffect,
	public TransformComp,
	public SunlitComp,
	public FogComp,
	public BlendComp
{
public:

	/// Constructor
	WaterManager();

	/// Destructor
	~WaterManager();

	/**
		Allocates DirectX resources necessary for rendering the water.
		@param device The Direct3D device to render with.
		@param ref_enabled True if reflection is enabled.
		@param ref_tex_width The width of the reflection texture.
		@param ref_tex_height The height of the reflection texture.
		@return True if the function succeeds, false otherwise.
	*/
	bool Initialize(
		IDirect3DDevice9 * device,
		bool ref_enabled,
		UINT ref_tex_width = 0,
		UINT ref_tex_height = 0,
		UINT wave_tex_size = 0
	);

	/**
		Frees DirectX resources and data structures that were previously
		allocated.
	*/
	virtual void Shutdown();

	/**
		Called to set the render target to the reflection surface.
		@note Code that calls this will have to set the next render target on
		its own.
	*/
	void BeginReflectionRenderTarget();

	/**
		Called after reflection rendering is complete.
	*/
	void EndReflectionRenderTarget();

	void RenderWater( D3DXMATRIX* view, D3DXMATRIX* projection, bool distant );

	void SetWaterHeight( float height );

	void AdvanceTime( float* time_delta );

	void SetWaterLevel( float level );
	void SetCausticIntensity( float intensity );

protected:

	// Water Mesh
	IDirect3DVertexBuffer9* m_wave_vb;
	IDirect3DIndexBuffer9* m_water_ib;
	IDirect3DVertexBuffer9* m_water_vb1;
	IDirect3DVertexBuffer9* m_water_vb2;
	int m_numWaterVertices;
	int m_numWaterTris;
	int m_fineWaterMesh;
	D3DXMATRIX m_WaterWorld;

	// Reflection
	bool m_usingReflectionRenderTarget;
	IDirect3DSurface9* m_reflectionSurface;
	IDirect3DTexture9* m_reflectionTexture;
	IDirect3DSurface9* m_reflectionZBuffer;

	// Ripples
	IDirect3DSurface9* m_waveRainSurface;
	IDirect3DTexture9* m_waveRainTexture;
	IDirect3DSurface9* m_waveRippleSurface;
	IDirect3DTexture9* m_waveRippleTexture;
	IDirect3DSurface9* m_waveTempSurface;
	IDirect3DTexture9* m_waveTempTexture;
	IDirect3DVertexBuffer9* m_waveVB;
	float m_waveTexWorldRes;
	int m_waveTexResolution;

	// Water Texture Hooking
	IDirect3DVolumeTexture9* m_NewWaterTexture;
	IDirect3DTexture9* m_realWaterTextures[32];
	IDirect3DTexture9** m_morrowWaterPtrs[32];
	IDirect3DTexture9* m_fakeWaterTexture;
	IDirect3DTexture9* m_defaultLandTexture;
	unsigned char m_waterLoadedCount;

	// Parameters
	D3DXHANDLE m_ehTexProj;
	D3DXHANDLE m_ehTime;
	D3DXHANDLE m_ehOffset;
	D3DXHANDLE m_ehWaterLevel;
	D3DXHANDLE m_ehCausticIntensity;
	D3DXHANDLE m_ehWaveTexWorldSize;
	D3DXHANDLE m_ehWaveTexRcpRes;
	D3DXHANDLE m_ehRippleOrigin;

	// Textures
	D3DXHANDLE m_ehNormalWaveCausticTexture;
	D3DXHANDLE m_ehDepthTexture;
	D3DXHANDLE m_ehRefractionTexture;
	D3DXHANDLE m_ehReflectionTexture;
	D3DXHANDLE m_ehRainRippleTexture;
	D3DXHANDLE m_ehPlayerRippleTexture;

	// Techniques
	D3DXHANDLE m_ehRenderWaterFromAbove;
	D3DXHANDLE m_ehRenderWaterFromBelow;
	D3DXHANDLE m_ehRenderWaterNoReflect;
	D3DXHANDLE m_ehRenderCaustics;
	D3DXHANDLE m_ehCalculateWaves;
	D3DXHANDLE m_ehCreatePCRipples;
};

//-----------------------------------------------------------------------------

#endif