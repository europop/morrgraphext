#ifndef _DRAW_RECORDER_H_
#define _DRAW_RECORDER_H_

#include "d3d8header.h"
#include "DepthManager.h"
#include "PooledList.h"

//-----------------------------------------------------------------------------

/**
	This class is provides a method for recording the draw calls made by
	Morrowind so that they can be re-executed later for other reasons, such
	as drawing the sky in the the water reflection or rendering a depth buffer
	that includes the Morrowind scene.  It currently adds a lot of overhead to
	the drawing process so should be used sparingly.
*/
class DrawRecorder {
public:
	/// The type of DirectX draw call that was used to render the object.
	enum DrawType {
		DRAW_INDEXED,
		DRAW_INDEXED_UP,
		DRAW_PRIM,
		DRAW_PRIM_UP
	};

	/**
		A structure containing all the information necessary to render the
		object again.
	*/
	struct RecordedDrawCall {
		DrawType renderType;
		const D3DMATERIAL8* material;
		IDirect3DTexture9* texture;
		IDirect3DVertexBuffer9* vBuffer;
		UINT streamSpan;
		IDirect3DIndexBuffer9* indices;
		DWORD fvf;
		D3DPRIMITIVETYPE primType;
		UINT baseVertexIndex;
		UINT minIndex;
		UINT numVertices;
		UINT startIndex;
		UINT primitiveCount;
		const void* indexData;
		D3DFORMAT indexDataFormat;
		const void* vertexStreamZeroData;
		UINT vertexStreamZeroSride;
		DWORD vertexBlendState;
		D3DXMATRIX worldTrans[4];
		DWORD cullModeState;
		DWORD alphaBlendFunc;
		DWORD colorOp;
		DWORD alphaOp;
		bool alphaBlendEnabled;
		bool alphaTestEnabled;
	};

	/**
		Records a single draw call.
		@param ro An object containnig all the information necessary to repeat
		the draw call.
	*/
	inline void RecordDrawCall( RecordedDrawCall& rdc );

	/**
		Clears the inernal list of recorded draw calls.
	*/
	inline void ClearRecordedDrawCalls();

	/**
		Returns false if any draw calls have been recorded.
		@return True only if no draw calls have been recorded.
	*/
	inline bool IsEmpty();

	/**
		Renders the depth of all the recorded draw calls.  Note that
		DepthManager::BeginDepthRenderTarget must be called before this
		function, and EndDepthRenderTarget must be called after.
		@param view The view matrix.
		@param proj The projection matrix.
		@param manager The depth manager.
	*/
	void RenderRecordedDrawCallDepth( 
		IDirect3DDevice9* device,
		D3DXMATRIX* view,
		D3DXMATRIX* proj,
		DepthManager* manager
	);

	/// Temporary to allow existing distant land functions to draw
	/// recorded renders
	PooledList<RecordedDrawCall>& GetList() {
		return m_recordedDrawCalls;
	}

	//Temporary for testing
	static bool SortByTexture(
		const RecordedDrawCall& l,
		const RecordedDrawCall& r
	) {
		return (l.texture < r.texture );
	}

protected:
	PooledList<RecordedDrawCall> m_recordedDrawCalls;
};

//-----------------------------------------------------------------------------
// Inline Function Definitions
//-----------------------------------------------------------------------------

inline void DrawRecorder::RecordDrawCall(
	DrawRecorder::RecordedDrawCall& rdc ) {

	RecordedDrawCall* new_item = m_recordedDrawCalls.CreateNewItem();

	*new_item = rdc;
}

//-----------------------------------------------------------------------------

inline void DrawRecorder::ClearRecordedDrawCalls() {
	m_recordedDrawCalls.FreeAllItems();
}

//-----------------------------------------------------------------------------

inline bool DrawRecorder::IsEmpty() {
	return m_recordedDrawCalls.IsEmpty();
}

//-----------------------------------------------------------------------------

#endif