#include "DrawRecorder.h"

//-----------------------------------------------------------------------------

void DrawRecorder::RenderRecordedDrawCallDepth(
	IDirect3DDevice9* device,
	D3DXMATRIX* view,
	D3DXMATRIX* proj,
	DepthManager* manager
) {

	if ( !manager->IsInitialized() ) {
		return;
	}

	if ( !m_recordedDrawCalls.IsEmpty() ) {

		D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0),
			L"Replaying recorded Morrowind Draw Calls");

		// We currently just have a single shader that handles all types of
		// objects that Morrowind renders.  Not great, but it works.
		manager->BeginGenericDepthRendering();	

		// Set new transforms and state	
		manager->SetViewMatrix( view );
		manager->SetProjectionMatrix( proj );

		for (
			PooledList<RecordedDrawCall>::Item* item = 
				m_recordedDrawCalls.GetFirstItem();
			item != m_recordedDrawCalls.GetEndMarker();
			item = item->next
		) {
			DrawRecorder::RecordedDrawCall& ro = *item->data;

			manager->SetAlphaTestTexture( ro.texture );

			device->SetFVF( ro.fvf );

			manager->SetBlendWeightCount( ro.vertexBlendState );

			manager->SetAlphaTest(
				ro.alphaTestEnabled ||
				(ro.alphaBlendFunc == D3DBLEND_SRCALPHA && ro.alphaBlendEnabled)
			);

			manager->SetWorldMatrixArray( ro.worldTrans, 4 );

			manager->SetWorldMatrix( ro.worldTrans );

			manager->CommitChanges();

			device->SetRenderState( D3DRS_CULLMODE, ro.cullModeState );

			if (
				ro.renderType == DrawRecorder::DRAW_INDEXED ||
				ro.renderType == DrawRecorder::DRAW_PRIM
			) {
				device->SetStreamSource( 0, ro.vBuffer, 0, ro.streamSpan );
				if ( ro.renderType == DrawRecorder::DRAW_INDEXED ) {
					device->SetIndices( ro.indices );
					device->DrawIndexedPrimitive(
						ro.primType,
						ro.baseVertexIndex,
						ro.minIndex,
						ro.numVertices,
						ro.startIndex,
						ro.primitiveCount
					);
				} else {
					device->DrawPrimitive(
						ro.primType,
						ro.startIndex,
						ro.primitiveCount
					);
				}
			} else if ( ro.renderType == DrawRecorder::DRAW_INDEXED_UP ) {
				device->DrawIndexedPrimitiveUP(
					ro.primType,
					ro.minIndex,
					ro.numVertices,
					ro.primitiveCount,
					ro.indexData,
					ro.indexDataFormat,
					ro.vertexStreamZeroData,
					ro.vertexStreamZeroSride
				);
			} else if ( ro.renderType == DrawRecorder::DRAW_PRIM_UP ) {
				device->DrawPrimitiveUP(
					ro.primType,
					ro.primitiveCount,
					ro.vertexStreamZeroData,
					ro.vertexStreamZeroSride
				);
			}
		}

		manager->EndDepthRendering();

		D3DPERF_EndEvent(); // Recorded morrowind draw calls
	}
}

//-----------------------------------------------------------------------------
