#ifndef _QUAD_TREE_NODE_H_
#define _QUAD_TREE_NODE_H_

#include "DLMath.h"
#include "MemoryPool.h"
#include "PooledList.h"
#include "DepthManager.h"
#include <vector>
#include <map>
#include <deque>

using namespace std;

//-----------------------------------------------------------------------------

struct QuadTreeMesh {
	QuadTreeMesh* next_mesh;

	BoundingSphere sphere;
	BoundingBox box;
	
	IDirect3DTexture9* tex;
	D3DXMATRIX transform;
	int verts;
	IDirect3DVertexBuffer9* vBuffer;
	int faces;
	IDirect3DIndexBuffer9* iBuffer;

	QuadTreeMesh(
		BoundingSphere b_sphere,
		BoundingBox b_box,
		D3DXMATRIX transform,
		IDirect3DTexture9* tex,
		int verts,
		IDirect3DVertexBuffer9* vBuffer,
		int faces,
		IDirect3DIndexBuffer9* iBuffer
	);

	~QuadTreeMesh();
	QuadTreeMesh & operator=( const QuadTreeMesh & rh );
	QuadTreeMesh( const QuadTreeMesh & rh );

	bool operator==( const QuadTreeMesh & rh );

	typedef QuadTreeMesh* qtmptr;

	static bool CompareByState (
		const qtmptr& lh,
		const qtmptr& rh
	);

	static bool CompareByTexture (
		const qtmptr& lh,
		const qtmptr& rh
	);
};

//-----------------------------------------------------------------------------

class VisibleSet {
public:
	void AddMesh(QuadTreeMesh* mesh);
	void RemoveAll();
	VisibleSet();
	~VisibleSet();

	//typedef void (*CallbackFunction)();

	//void RenderInstanced(IDirect3DDevice9* device);
	void Render(
		IDirect3DDevice9* device,
		ID3DXEffect* effect,
		D3DXHANDLE* texture_handle,
		D3DXHANDLE* world_matrix_handle,
		unsigned int vertex_size );

	void RenderDepth(
		IDirect3DDevice9* device,
		unsigned int vertex_size,
		DepthManager* manager
	);

	void SortByState();
	void SortByTexture();
	
private:
	friend class VisibleSetRenderer;

	PooledList<QuadTreeMesh*> visible_set;

	//CallbackFunction m_SetTransformFn;
	//CallbackFunction m_SetTextureFn;

	//CallbackFunction m_preRenderMeshFn;
	//CallbackFunction m_postRenderMeshFn;

};

//-----------------------------------------------------------------------------

class QuadTree;

struct QuadTreeNode {
	static const size_t QUADTREE_MAX_DEPTH = 5;
	static const float QUADTREE_MIN_DIST;
	
	QuadTree* m_owner;
	QuadTreeNode* children[4];
	float box_size;
	D3DXVECTOR2 box_center;
	BoundingSphere sphere;

	QuadTreeMesh* mesh_list;

	void GetVisibleMeshes( const ViewFrustum & frustum, VisibleSet & visible_set, bool inside = false );
	void AddMesh( QuadTreeMesh * new_mesh, int depth = QUADTREE_MAX_DEPTH );
		
	void PushDown( QuadTreeMesh * new_mesh, int depth );
	bool Optimize();
	//void Report( int depth = QUADTREE_MAX_DEPTH );
	BoundingSphere CalcVolume();
	int GetChildCount() const;
	QuadTreeNode(QuadTree* owner);
	void ClearChildren();
	~QuadTreeNode();

protected:
	QuadTreeNode( const QuadTreeNode& rh );
	void _InsertMesh( QuadTreeMesh* new_mesh );
};

//-----------------------------------------------------------------------------

class QuadTree {
public:

	QuadTree();
	QuadTree(size_t node_objects, size_t mesh_objects);
	~QuadTree();
	void AddMesh(
		BoundingSphere sphere,
		BoundingBox box,
		D3DXMATRIX transform,
		IDirect3DTexture9* tex,
		int verts,
		IDirect3DVertexBuffer9* vBuffer,
		int faces,
		IDirect3DIndexBuffer9* iBuffer
	);
	bool Optimize();
	void Clear();
	void GetVisibleMeshes( const ViewFrustum & frustum, VisibleSet & visible_set );
	void SetBoxSize(float size);
	void SetBoxCenter( D3DXVECTOR2 &center );
	void CalcVolume();

	QuadTreeNode* m_root_node;
	static MemoryPool* m_node_pool;
	static MemoryPool* m_mesh_pool;
protected:
	friend struct QuadTreeNode;
	QuadTreeNode* CreateNode();
	QuadTreeMesh* CreateMesh(
		BoundingSphere sphere,
		BoundingBox box,
		D3DXMATRIX transform,
		IDirect3DTexture9* tex,
		int verts,
		IDirect3DVertexBuffer9* vBuffer,
		int faces,
		IDirect3DIndexBuffer9* iBuffer	
	);
private:
	//Disallow copy and assignment
	QuadTree& operator=(QuadTree&);
	QuadTree(QuadTree&);
};

#endif
