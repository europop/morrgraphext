#ifndef _DISTANT_LAND_H_
#define _DISTANT_LAND_H_

#include "Threading.h"
#include "DrawRecorder.h"
#include "DepthManager.h"

EX DrawRecorder DepthDrawRecorder;
EX DrawRecorder SkyDrawRecorder;
EX BYTE WaterLoadedCount;

enum StaticRange {
	RANGE_NEAR,
	RANGE_FAR,
	RANGE_VERY_FAR,
	RANGE_REFLECT,
	RANGE_DEPTH
};

struct DistantStatic;

bool _stdcall DL_Initialize();
void DL_SubmitCullingTasks();
void CullDistantStatics(VisibleSet& visible_set, D3DXMATRIX* view, D3DXMATRIX* proj, StaticRange range, bool blend );
void LoadDS(DistantStatic *ds, LPBYTE buffer);
void DL_SubmitNearGrassCullingTask();
void DL_RenderGrass( D3DXMATRIX* view, D3DXMATRIX* proj, bool distant, bool blend, bool depth );
void DL_CullGrass( VisibleSet& visible_set, D3DXMATRIX* view, D3DXMATRIX* proj, bool distant, bool blend, bool depth );
void DL_RenderLand();
void DL_Shutdown();
void DL_RenderLowestLand( D3DXMATRIX* projection , bool renderdepth );
void DL_RenderWater( D3DXMATRIX * projection, bool distant );
void DL_BlendNearAndFarView();
void DL_PlayerPos(float x,float y);
bool DL_HasStatics();
bool DL_Activate();
void DL_Deactivate();

int DL_CreateMeshes();


class StaticCullTask : public ThreadTask {
public: 
	StaticCullTask(
		ThreadFlag* flag,
		VisibleSet* visible_set,
		D3DXMATRIX* view,
		D3DXMATRIX* proj,
		StaticRange range,
		bool blend
	) :
		ThreadTask(flag),
		m_visible_set(visible_set),
		m_view(view),
		m_proj(proj),
		m_range(range),
		m_blend(blend)
	{}

	virtual void Run() {
		CullDistantStatics( *m_visible_set, m_view, m_proj, m_range, m_blend );
	}
protected:
	VisibleSet* m_visible_set;
	D3DXMATRIX* m_view;
	D3DXMATRIX* m_proj;
	StaticRange m_range;
	bool m_blend;
};


class GrassCullTask : public ThreadTask  {
public:
	GrassCullTask(
		ThreadFlag* flag,
		VisibleSet* visible_set,
		D3DXMATRIX* view,
		D3DXMATRIX* proj,
		bool distant,
		bool blend,
		bool depth
	) :
		ThreadTask(flag),
		m_visible_set(visible_set),
		m_view(view),
		m_proj(proj),
		m_distant(distant),
		m_blend(blend),
		m_depth(depth)
	{}

	virtual ~GrassCullTask() {}

	virtual void Run() {
		DL_CullGrass( *m_visible_set, m_view, m_proj, m_distant, m_blend, m_depth );
	}
protected:
	VisibleSet* m_visible_set;
	D3DXMATRIX* m_view;
	D3DXMATRIX* m_proj;
	bool m_distant;
	bool m_blend;
	bool m_depth;
};

class LoadDSCullTask : public ThreadTask {
public: 
	LoadDSCullTask(
		ThreadFlag* flag,
		DistantStatic *ds,
		LPBYTE buffer
	) :
		ThreadTask(flag),
		m_ds(ds),
		m_buffer(buffer)
	{}

	virtual void Run() {
		LoadDS( m_ds, m_buffer );
	}
protected:
	DistantStatic *m_ds;
	LPBYTE m_buffer;
};

struct CullingVisibleSets {
	VisibleSet static_reflected;
	VisibleSet static_vf;
	VisibleSet static_f;
	VisibleSet static_n;
	VisibleSet grass_d;
	void ClearAll() {
		static_reflected.RemoveAll();
		static_vf.RemoveAll();
		static_f.RemoveAll();
		static_n.RemoveAll();
		grass_d.RemoveAll();
	}
};

struct CullingFlags {
	ThreadFlag static_reflected;
	ThreadFlag static_vf;
	ThreadFlag static_f;
	ThreadFlag static_n;
	ThreadFlag grass_d;
};

#endif