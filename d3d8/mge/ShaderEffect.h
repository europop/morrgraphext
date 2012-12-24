#ifndef _SHADER_EFFECT_H_
#define _SHADER_EFFECT_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>
#include <sstream>
#include <vector>
using namespace std;

//-----------------------------------------------------------------------------

/**
	This class manages rendering objects with a specific shader effect.
*/

class ShaderEffect {
public:

	/// Constructor
	ShaderEffect();

	/// Destructor
	virtual ~ShaderEffect();

	/**
		Allocates DirectX resources necessary for rendering with this effect.
	*/
	virtual bool Initialize( IDirect3DDevice9 * device );

	/**
		Frees DirectX resources and data structures that were previously
		allocated.
	*/
	virtual void Shutdown();

	/**
		Checks whether the effect manager has been initialized.
		@return True if the effect manager has been initialized, false if not.
	*/
	inline bool IsInitialized();

	/**
		Commits any changes that were made to the shader parameters.
	*/
	void CommitChanges();

	/// Sets the text file that any loading errors will be written to.
	inline void SetErrorFile( const char* file_name );

	/// Adds an error to the error stream.
	inline void AddError( string text );
	inline void AddError( const char* text, size_t length );

	/// Clears all errors that have been added.
	inline void ClearErrors();

	/// Gets the number of errors that have occured since the last time they
	/// were cleared.
	inline size_t GetErrorCount();

	// These functions should only be called by the classes derived from
	// Effect Component
	void GetParamHandle( D3DXHANDLE& handle, const char* name );
	void GetTexHandle( D3DXHANDLE& handle, const char* name );
	void GetTechHandle( D3DXHANDLE& handle, const char* name );
	inline ID3DXEffect* GetD3DEffect();

protected:
	void LoadEffect( const char* file_name );
	void AbortLoad( const string& message );
	void AddDefine( const char* name, const char* definition );
	template <class T>
	inline void D3DRelease( T* ptr );
	
	ID3DXEffect* m_effect;
	IDirect3DDevice9* m_device;
	bool m_initialized;

	struct Macro {
		string name;
		string definition;
	};

	// Preprocessor defines
	vector<Macro> m_defines;

	// Error reporting
	stringstream m_load_errors;
	size_t m_error_count;
	const char* m_error_file;

};

//-----------------------------------------------------------------------------
// Inline Function Definitions
//-----------------------------------------------------------------------------
inline bool ShaderEffect::IsInitialized() {
	return m_initialized;
}

//-----------------------------------------------------------------------------

inline size_t ShaderEffect::GetErrorCount() {
	return m_error_count;
}

//-----------------------------------------------------------------------------

inline void ShaderEffect::SetErrorFile( const char* file_name ) {
	m_error_file = file_name;
}

//-----------------------------------------------------------------------------

inline void ShaderEffect::AddError( string text ) {
	m_load_errors << text;
	++m_error_count;
}

//-----------------------------------------------------------------------------

inline void ShaderEffect::AddError( const char* text, size_t length ) {
	m_load_errors.write( text, length );
	++m_error_count;
}

//-----------------------------------------------------------------------------

inline void ShaderEffect::ClearErrors() {
	m_error_count = 0;
	m_load_errors.clear();
}

//-----------------------------------------------------------------------------

inline ID3DXEffect* ShaderEffect::GetD3DEffect() {
	return m_effect;
}

//-----------------------------------------------------------------------------

template <class T>
inline void ShaderEffect::D3DRelease( T* ptr ) {
	if ( ptr ) {
		ptr->Release();
		ptr = 0;
	}
}

//-----------------------------------------------------------------------------

#endif