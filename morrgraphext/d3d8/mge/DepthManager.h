#ifndef _DEPTH_MANAGER_H_
#define _DEPTH_MANAGER_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>

//-----------------------------------------------------------------------------

/**
	This class is responsible for providing a depth buffer.  It allocates the
	buffer, allows other functions to draw into it.
*/
class DepthManager {
public:

	/// Constructor
	DepthManager();

	/// Destructor
	~DepthManager();

	/**
		Allocates DirectX resources necessary for rendering a depth buffer and
		prepares any other necessary data structures.
		@return True if the function succeeds, false otherwise.
	*/
	bool Initialize( IDirect3DDevice9 * device, UINT width, UINT height );

	/**
		Frees DirectX resources and data structures that were previously
		allocated.
	*/
	void Shutdown();

	/**
		Checks whether the depth manager has been initialized.
		@return True if the depth manager has been initialized, false if not.
	*/
	inline bool IsInitialized();

	/**
		Called to set the render target to the depth surface.  Note that code
		that calls this will have to set the next render target on its own.
	*/
	void BeginDepthRenderTarget();

	/**
		Called after depth rendering is complete.
	*/
	void EndDepthRenderTarget();

	/**
		Called to begin rendering depth using one of the standard depth
		shaders.  These shaders intentionally do not handle grass deformation.
		Note that any other active D3D effect should be ended before this call.
		@param alpha_test Whether or not alpha testing should be enabled while
		rendering.  If alpha testing is used, then SetAlphaTestTexture must be
		called each time the texture to use for alpha testing changes.
		@note Must be called within BeginDepthRenderTarget and
		EndDepthRenderTarget calls.
	*/
	void BeginStaticDepthRendering( bool alpha_test );

	/**
		Called to begin rendering depth using a very generic shader that can
		handle various circumstances.  Alpha testing is always enabled and
		skin morphing is always possible, making this shader inefficient.
		SetAlphaTestTexture must be called each time a texture changes and
		solid objects should eithe have textures with no alpha channel or all
		pixels in the alpha channel set to 1.0.
		@note Must be called within BeginDepthRenderTarget and
		EndDepthRenderTarget calls.
	*/
	void BeginGenericDepthRendering();

	/**
		Called to set the texture that will be used for alpha testing.  The
		alpha channel will be used to determine if a pixel should be drawn.
		@param texture The new alpha test texture to use.
		@note Must be called within BeginDepthRenderTarget and
		EndDepthRenderTarget calls.
	*/
	void SetAlphaTestTexture( IDirect3DTexture9* texture );


	void SetViewMatrix( D3DXMATRIX* view );
	void SetProjectionMatrix( D3DXMATRIX* projection );
	void SetWorldMatrix( D3DXMATRIX* world );
	void SetWorldMatrixArray( D3DXMATRIX* world, UINT count );
	void SetBlendWeightCount( INT count );
	void SetAlphaTest( bool is_alpha_tested );
	void CommitChanges();

	/**
		Called when depth rendering is complete to end the D3D effect.
	*/
	void EndDepthRendering();

	/**
		Clears the depth buffer to a value that represents maximum depth.
		@note If this is not called between calls to BeginDepthRenderTarget and
		EndDepthRenderTarget, then the actual clearing of the depth buffer will
		be delayed until the next call to BeginDepthRenderTarget.
	*/
	inline void ClearDepthBuffer( bool clear_texture_too = false );

	/**
		Clears the depth buffer to minimum depth.
	*/
	void ZeroDepthBuffer();

	/**
		Returns a pointer to a texture that contains the contents of the current
		depth buffer.
		@note Must be called outside of BeginDepthRenderTarget and
		EndDepthRenderTarget calls.
	*/
	inline IDirect3DTexture9* GetDepthTexture();

protected:
	bool m_initialized;
	bool m_usingDepthRenderTarget;
	bool m_clearFlag, m_clearTexFlag;
	IDirect3DSurface9* m_depthSurface;
	IDirect3DTexture9* m_depthTexture;
	IDirect3DSurface9* m_depthZBuffer;
	ID3DXEffect* m_depthEffect;
	IDirect3DDevice9* m_device;

	//Parameters
	D3DXHANDLE m_ehView;
	D3DXHANDLE m_ehProj;
	D3DXHANDLE m_ehWorld;
	D3DXHANDLE m_ehMatrixPalette;
	D3DXHANDLE m_ehBlendWeightCount;
	D3DXHANDLE m_ehAlphaTest;

	//Textures
	D3DXHANDLE m_ehAlphaTestTexture;

	//Techniques
	D3DXHANDLE m_ehStaticDepth;
	D3DXHANDLE m_ehNoAlphaDepth;
	D3DXHANDLE m_ehRenderGenericDepth;
	D3DXHANDLE m_ehClearDepth;

	/**
		Clears the depth buffer to a value that represents maximum depth.
		@note Must be called within BeginDepthRenderTarget and
		EndDepthRenderTarget calls.
	*/
	void _DoClearDepthBuffer();
};

//-----------------------------------------------------------------------------
// Inline Function Definitions
//-----------------------------------------------------------------------------
inline bool DepthManager::IsInitialized() {
	return m_initialized;
}

//-----------------------------------------------------------------------------

inline IDirect3DTexture9* DepthManager::GetDepthTexture() {
	assert( m_initialized );
	return m_depthTexture;
}

//-----------------------------------------------------------------------------

inline void DepthManager::ClearDepthBuffer( bool clear_texture_too ) {
	assert( m_initialized );

	m_clearTexFlag = clear_texture_too;

	if ( m_usingDepthRenderTarget ) {
		//Clear the depth buffer immediately
		_DoClearDepthBuffer();
	} else {
		// Set a flag to clear the depth buffer later
		m_clearFlag = true;
	}
}

//-----------------------------------------------------------------------------

#endif