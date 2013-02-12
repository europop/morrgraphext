#include "ShaderEffect.h"
#include <fstream>
#include "d3d8header.h"
#include "Configuration.h"

//-----------------------------------------------------------------------------

ShaderEffect::ShaderEffect() {
	m_initialized = false;
	m_effect = 0;
	m_device = 0;
}

//-----------------------------------------------------------------------------

ShaderEffect::~ShaderEffect() {
	if (m_initialized) {
		this->Shutdown();
	}
}

//-----------------------------------------------------------------------------

bool ShaderEffect::Initialize( IDirect3DDevice9 * device ) {
	m_device = device;

	if ( device != 0 ) {
		m_initialized = true;
	}

	return (m_initialized);
}

//-----------------------------------------------------------------------------

void ShaderEffect::Shutdown() {
	assert( m_initialized );
	m_initialized = false;

	m_device = 0;

	if ( m_effect ) {
		m_effect->Release();
		m_effect = 0;
	}
}

//-----------------------------------------------------------------------------

void ShaderEffect::CommitChanges() {
	m_effect->CommitChanges();
}

//-----------------------------------------------------------------------------

void ShaderEffect::GetParamHandle( D3DXHANDLE& handle, const char* name ) {
	handle = m_effect->GetParameterByName( 0, name );
	if ( !handle  ) {
		m_load_errors << "Failed to find " << name 
			<< " global variable in shader." << endl;
		++m_error_count;
	}
}

//-----------------------------------------------------------------------------

void ShaderEffect::GetTexHandle( D3DXHANDLE& handle, const char* name ) {
	handle = m_effect->GetParameterByName( 0, name );
	if ( !handle  ) {
		m_load_errors << "Failed to find " << name 
			<< " texture in shader." << endl;
		++m_error_count;
	}
}

//-----------------------------------------------------------------------------

void ShaderEffect::GetTechHandle( D3DXHANDLE& handle, const char* name ) {
	handle = m_effect->GetTechniqueByName( name );
	if ( !handle  ) {
		m_load_errors << "Failed to find " << name 
			<< " global variable in shader." << endl;
		++m_error_count;
	}
}

//-----------------------------------------------------------------------------

void ShaderEffect::LoadEffect( const char* file_name ) {

	D3DXMACRO* defines = 0;

	if ( !m_defines.empty() ) {
		// Build macro array
		size_t count = m_defines.size();
		defines = new D3DXMACRO[count + 1];
		size_t i = 0;
		for ( /**/; i < count; ++i ) {
			defines[i].Name = m_defines[i].name.c_str();
			defines[i].Definition = m_defines[i].definition.c_str();
		}
		
		//Add a final NULL define to signal the end of the array
		defines[i].Definition = 0;
		defines[i].Name = 0;
	}


	// Load shader effect file
	LPD3DXBUFFER errors = 0;
	HRESULT res = D3DXCreateEffectFromFile(
		m_device,
		(string("Data files\\shaders\\distantland\\") + file_name).c_str(),
		defines,
		0,
		0,
		0,
		&m_effect,
		&errors
	);

	if ( FAILED(res) ) {
		if ( errors ) {
			m_load_errors.write(
				(const char*)errors->GetBufferPointer(),
				errors->GetBufferSize()
			);
			errors->Release();
			errors = 0;
		} else {
			m_load_errors << file_name << " shader failed to compile.";
		}

		++m_error_count;
	}

	// Free maro array
	if ( defines ) {
		delete [] defines;
	}
}

//-----------------------------------------------------------------------------

void ShaderEffect::AbortLoad( const string& message ) {

	m_load_errors << message;

	std::ofstream err_file( m_error_file );
	err_file << m_load_errors.str();
	err_file.close();

	#ifdef _DEBUG
	OutputDebugString( m_load_errors.str().c_str() );
	#endif

	SETSTATUS(message.c_str());

	// Loading failed, so shut down
	this->Shutdown();
}

//-----------------------------------------------------------------------------

void ShaderEffect::AddDefine( const char* name, const char* definition ) {
	Macro define;
	define.name = name;
	define.definition = definition;

	m_defines.push_back(define);
}

//-----------------------------------------------------------------------------
