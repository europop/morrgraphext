#include "QuadTree.h"
#include <assert.h>
#include <algorithm>

const float QuadTreeNode::QUADTREE_MIN_DIST = 20.0f;

MemoryPool* QuadTree::m_node_pool = 0;
MemoryPool* QuadTree::m_mesh_pool = 0;

//-----------------------------------------------------------------------------
// QuadTreeMesh class
//-----------------------------------------------------------------------------

QuadTreeMesh::QuadTreeMesh(
	BoundingSphere sphere,
	BoundingBox box,
	D3DXMATRIX transform,
	IDirect3DTexture9* tex,
	int verts,
	IDirect3DVertexBuffer9* vBuffer,
	int faces,
	IDirect3DIndexBuffer9* iBuffer
) {
	next_mesh = 0;
	this->sphere = sphere;
	this->box = box;
	this->transform = transform;
	this->tex = tex;
	this->verts = verts;
	this->vBuffer = vBuffer;
	this->faces = faces;
	this->iBuffer = iBuffer;
}

//-----------------------------------------------------------------------------

//QuadTreeMesh::QuadTreeMesh( MeshCollection::VisFlags flags, UsedDistantStatic* uds, DistantSubset* sub ) : visible(flags) {

	//for ( size_t i = 0; i < 16; ++i ) {
	//	transform[i] = uds->transform[i];
	//}

	//D3DXVECTOR4 output;
	//D3DXVec3Transform( &output, &sub->center, &uds->transform );
	//sphere.center.x = output.x;
	//sphere.center.y = output.y;
	//sphere.center.z = output.z;
	//sphere.radius = sub->radius * uds->scale;

	//tex = sub->tex;
	//verts = sub->verts;
	//vBuffer = sub->vBuffer;
	//faces = sub->faces;
	//iBuffer = sub->iBuffer;	

//}

//-----------------------------------------------------------------------------

QuadTreeMesh::~QuadTreeMesh() {
	//Don't free anything because it's all owned by other things
}

//-----------------------------------------------------------------------------

QuadTreeMesh & QuadTreeMesh::operator=( const QuadTreeMesh & rh ) {
	next_mesh = 0;
	sphere.center = rh.sphere.center;
	sphere.radius = rh.sphere.radius;
	box = rh.box;
	transform = rh.transform;
	tex = rh.tex;
	verts = rh.verts;
	vBuffer = rh.vBuffer;
	faces = rh.faces;
	iBuffer = rh.iBuffer;

	return *this;
}

//-----------------------------------------------------------------------------

QuadTreeMesh::QuadTreeMesh( const QuadTreeMesh & rh ) {
	*this = rh;
}

//-----------------------------------------------------------------------------

bool QuadTreeMesh::CompareByState (
	const qtmptr& lh,
	const qtmptr& rh
) {
   	if ( lh->tex < rh->tex ) { return true; }
	if ( lh->tex == rh->tex && lh->vBuffer < rh->vBuffer ) { return true; }
	return false;
}

//-----------------------------------------------------------------------------

bool QuadTreeMesh::CompareByTexture (
	const qtmptr& lh,
	const qtmptr& rh
) {
	return ( lh->tex < rh->tex );
}

//-----------------------------------------------------------------------------

bool QuadTreeMesh::operator==( const QuadTreeMesh & rh ) {
	if ( tex == rh.tex && vBuffer == rh.vBuffer ) {
		return true;
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------
// VisibleSet class
//-----------------------------------------------------------------------------

void VisibleSet::AddMesh( QuadTreeMesh* mesh ) {

	//visible_set[mesh->tex][mesh->data].push_back(mesh->transform);

	*(visible_set.CreateNewItem()) = mesh;


	/*
	TextureGroups::iterator mesh_group = visible_set.find(mesh->tex);

	if ( mesh_group == visible_set.end() ) {
		visible_set.insert( pair<IDirect3DTexture9*, MeshGroups>(mesh->tex, MeshGroups()) );
		mesh_group = visible_set.find(mesh->tex);
		assert( mesh_group != visible_set.end() );
	}

	MeshGroups::iterator instance_list = mesh_group->second.find(mesh->data);

	if ( instance_list == mesh_group->second.end() ) {
		mesh_group->second.insert( pair<MeshData, InstanceList>(mesh->data, InstanceList()) );
		instance_list = mesh_group->second.find(mesh->data);
		assert( instance_list != mesh_group->second.end() );
	}

	instance_list->second.push_back(mesh->transform);
	*/
}

//-----------------------------------------------------------------------------

void VisibleSet::RemoveAll() {
	visible_set.FreeAllItems();
}

//-----------------------------------------------------------------------------

/*
void VisibleSet::RenderInstanced(IDirect3DDevice9* device) {
if ( range == RANGE_REFLECT ) {
			DL_Effect->BeginPass(PASS_REFLECTSTATICSINST);
		} else {
			DL_Effect->BeginPass(PASS_RENDERSTATICSINST);
		}

		//UINT vis_count = 0;
		
		//Advanced to the next hardware instance vertex buffer
		//++HWInstVBIndex;
		//if ( HWInstVBIndex >= HWINSTBUFS ) {
		//	HWInstVBIndex = 0;
		//}
		IDirect3DVertexBuffer9* HWInstVB = HWInstVBs[HWInstVBIndex];
		
		RealDevice->SetVertexDeclaration( HWInstDecl );
		RealDevice->SetStreamSourceFreq( 1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul );

		D3DXMATRIX* gpu_data;
		//D3DXMATRIX cpu_data[MAXINSTANCES];
		//HWInstVB->Lock( 0, sizeof(D3DXMATRIX) * vis_count, (void**)&gpu_data, 0);

		//QueryPerformanceCounter( &start_time);

		size_t textures_size = StaticCollection.textures.size();
		if ( textures_size > 0 ) {
			TextureGroup** textures_arr = &(StaticCollection.textures[0]);
			for ( size_t t = 0; t < textures_size; ++t ) {
				TextureGroup* tg = textures_arr[t];

				if ( tg->visible == false ) {
					continue;
				}

				//Set Texture
				DL_Effect->SetTexture(ehTex3, tg->tex);
				DL_Effect->CommitChanges();
				++textures;

				size_t meshes_size = tg->meshes.size();
				if ( meshes_size > 0 ) {
					MeshGroup** meshes_arr = &(tg->meshes[0]);
					for ( size_t m = 0; m < meshes_size; ++m ) {
						MeshGroup* mg = meshes_arr[m];

						if ( mg->visible == false ) {
							continue;
						}

						UINT inst_count = mg->inst_vis.size();

						//Write a matrix for all visible objects to the instance buffer
						UINT vis_count = min( mg->GetVisibleCount(), MAXINSTANCES );

						//Determine if the new instance data can fit in the current buffer
						//log_file << "HWInstBufferPos(" << HWInstBufferPos << ") + vis_count(" << vis_count << ") >= MAXINSTANCES(" << MAXINSTANCES << ")?" << endl;
						if ( HWInstBufferPos + vis_count >= MAXINSTANCES ) {
							//Data cannot fit, so start from the begining and lock with discard
							//log_file << "Yes, Locking " << vis_count << " items at " << HWInstBufferPos << " with D3DLOCK_DISCARD" << endl;
							HWInstBufferPos = 0;
							HWInstVB->Lock( 0, vis_count * sizeof(D3DXMATRIX), (void**)&gpu_data, D3DLOCK_DISCARD);
						} else {
							//Data can fit, so start from current buffer pos and lock with no overwrite
							//log_file << "No, Locking " << vis_count << " items at " << HWInstBufferPos << " with D3DLOCK_NOOVERWRITE" << endl;
							HWInstVB->Lock( HWInstBufferPos * sizeof(D3DXMATRIX), vis_count * sizeof(D3DXMATRIX), (void**)&gpu_data, D3DLOCK_NOOVERWRITE);
						}

						UINT copy_count = 0;
						unsigned char* inst_vis_ptr = &(mg->inst_vis[0]);
						D3DMATRIX* inst_trans_ptr = &(mg->inst_trans[0]);
						for ( size_t i = 0; i < inst_count; ++i ) {
							if( inst_vis_ptr[i] != 0 ) {

								gpu_data[copy_count] = inst_trans_ptr[i];

								++copy_count;

								if ( copy_count >= MAXINSTANCES ) {
									//log_file << mg->GetVisibleCount() << " objects won't fit in buffer.  Discarding " << mg->GetVisibleCount() - copy_count << " objects." << endl;
									break;
								}
							}
						}

						//Unlock buffer
						HWInstVB->Unlock();

						//Set mesh stream and indices
						RealDevice->SetIndices( mg->iBuffer );

						RealDevice->SetStreamSource( 0, mg->vBuffer, 0, SizeOfStaticVert );
						RealDevice->SetStreamSourceFreq( 0, D3DSTREAMSOURCE_INDEXEDDATA | vis_count );

						RealDevice->SetStreamSource( 1, HWInstVB, HWInstBufferPos * sizeof(D3DXMATRIX), sizeof(D3DXMATRIX));		

						//Draw instance group
						RealDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, mg->verts, 0, mg->faces );
						++meshes;
						++draw_calls;

						//Increment buffer position
						HWInstBufferPos += vis_count;
					}
				}
			}
		}

		//log_file << "Took " << double(end_time.QuadPart - start_time.QuadPart) / double(time_freq.QuadPart) << " milliseconds to render." << endl;


		//Reset stream frequencies
		RealDevice->SetStreamSourceFreq( 0, 1 );
		RealDevice->SetStreamSourceFreq( 1, 1 );
}

*/

//-----------------------------------------------------------------------------

VisibleSet::VisibleSet() :
	visible_set(2000)
{}

//-----------------------------------------------------------------------------


VisibleSet::~VisibleSet() {}

//-----------------------------------------------------------------------------

void VisibleSet::Render(
	IDirect3DDevice9* device,
	ID3DXEffect* effect,
	D3DXHANDLE* texture_handle,
	D3DXHANDLE* world_matrix_handle,
	unsigned int vertex_size ) {

	//Iterate through each object in the visible set
	PooledList<QuadTreeMesh*>::Item* mesh = visible_set.GetFirstItem();
	PooledList<QuadTreeMesh*>::Item* meshs_end = visible_set.GetEndMarker();

	IDirect3DTexture9* last_texture = 0;
	IDirect3DVertexBuffer9* last_buffer = 0;

	#ifdef _DEBUG
		size_t texture_changes = 0;
		size_t mesh_changes = 0;
		size_t draw_calls = 0;
	#endif

	for ( /**/ ; mesh != meshs_end; mesh = mesh->next ) {
		bool effect_changed = false;

		const QuadTreeMesh* const m = *(mesh->data);
		
		//Set Texture if it has changed
		if (effect && texture_handle && last_texture != m->tex) {
			effect->SetTexture(*texture_handle, m->tex);
			effect_changed = true;
			last_texture = m->tex;
			#ifdef _DEBUG
				++texture_changes;
			#endif
		}

		//Set buffer if it has changed
		if ( last_buffer != m->vBuffer ) {
			device->SetIndices( m->iBuffer );
			device->SetStreamSource( 0, m->vBuffer, 0, vertex_size );
			#ifdef _DEBUG
				++mesh_changes;
			#endif
			last_buffer = m->vBuffer;
		}

		//Set transform matrix
		if ( effect && world_matrix_handle ) {
			effect->SetMatrix( *world_matrix_handle, &m->transform );
			effect_changed = true;
		}

		//Commit any changes that were made to the effect
		if (effect_changed) {
			effect->CommitChanges();
		}

		//Draw mesh
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			m->verts,
			0,
			m->faces
		);
		#ifdef _DEBUG
			++draw_calls;
		#endif
	}
}

//-----------------------------------------------------------------------------


void VisibleSet::RenderDepth(
	IDirect3DDevice9* device,
	unsigned int vertex_size,
	DepthManager* manager
) {

	//Iterate through each object in the visible set
	PooledList<QuadTreeMesh*>::Item* mesh = visible_set.GetFirstItem();
	PooledList<QuadTreeMesh*>::Item* meshs_end = visible_set.GetEndMarker();

	IDirect3DTexture9* last_texture = 0;
	IDirect3DVertexBuffer9* last_buffer = 0;

	#ifdef _DEBUG
		size_t texture_changes = 0;
		size_t mesh_changes = 0;
		size_t draw_calls = 0;
	#endif

	for ( /**/ ; mesh != meshs_end; mesh = mesh->next ) {
		bool effect_changed = false;

		const QuadTreeMesh* const m = *(mesh->data);
		
		//Set Texture if it has changed
		
		if (last_texture != m->tex) {
			manager->SetAlphaTestTexture( m->tex );
			effect_changed = true;
			last_texture = m->tex;
			#ifdef _DEBUG
				++texture_changes;
			#endif
		}

		//Set buffer if it has changed
		if ( last_buffer != m->vBuffer ) {
			device->SetIndices( m->iBuffer );
			device->SetStreamSource( 0, m->vBuffer, 0, vertex_size );
			#ifdef _DEBUG
				++mesh_changes;
			#endif
			last_buffer = m->vBuffer;
		}

		//Set transform matrix
		manager->SetWorldMatrix( &(*mesh->data)->transform );
		effect_changed = true;

		//Commit any changes that were made to the effect
		if (effect_changed) {
			manager->CommitChanges();
		}

		//Draw mesh
		device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			(*mesh->data)->verts,
			0,
			(*mesh->data)->faces
		);
		#ifdef _DEBUG
			++draw_calls;
		#endif
	}
}

//-----------------------------------------------------------------------------

void VisibleSet::SortByState() {
	visible_set.Sort( QuadTreeMesh::CompareByState );
}

//-----------------------------------------------------------------------------

void VisibleSet::SortByTexture() {
	visible_set.Sort( QuadTreeMesh::CompareByTexture );
}

//-----------------------------------------------------------------------------
// QuadTreeNode class
//-----------------------------------------------------------------------------

void QuadTreeNode::GetVisibleMeshes( const ViewFrustum & frustum, VisibleSet & visible_set, bool inside ) {
	////Check if this node is too small on the screen to care about
	//float dist = sqrt( 
	//	(sphere.center.x-view_pos.x)*(sphere.center.x-view_pos.x) +
	//	(sphere.center.y-view_pos.y)*(sphere.center.y-view_pos.y) +
	//	(sphere.center.z-view_pos.z)*(sphere.center.z-view_pos.z) );

	////float max_dist = (float)Options.DL_AboveWaterFogEnd*FogRange * 1.2f;
	//float ratio_limit = 200.0f;
	//float error_metric = 10000.0f;
	//
	//if ( (error_metric * sphere.radius) / dist < ratio_limit ) {
	//	//Too small
	//	return;
	//}

	//Check if this node is fully outside the frustum.
	//If inside = true then that means it has already been determined that this entire branch is visible
	if ( inside == false ) {
		ViewFrustum::Containment result = frustum.ContainsSphere(sphere);
		if ( result == ViewFrustum::OUTSIDE ) {
			return;
		} else if ( result == ViewFrustum::INSIDE ) {
			inside = true;
		}
	}

	//If this node has children, check them
	if ( GetChildCount() > 0 ) {
		if ( children[0] ) { children[0]->GetVisibleMeshes(frustum, visible_set, inside ); }
		if ( children[1] ) { children[1]->GetVisibleMeshes(frustum, visible_set, inside ); }
		if ( children[2] ) { children[2]->GetVisibleMeshes(frustum, visible_set, inside ); }
		if ( children[3] ) { children[3]->GetVisibleMeshes(frustum, visible_set, inside ); }
		return;
	}

	//If this node has any meshes, check each of their visibility and add them to the list if they're not completely outside the frustum
	;
	for ( QuadTreeMesh* mesh = mesh_list; mesh; mesh = mesh->next_mesh ) {

		////Check if this mesh is too small on the screen to care about
		//float dist = sqrt( 
		//	(mesh.sphere.center.x-view_pos.x)*(mesh.sphere.center.x-view_pos.x) +
		//	(mesh.sphere.center.y-view_pos.y)*(mesh.sphere.center.y-view_pos.y) +
		//	(mesh.sphere.center.z-view_pos.z)*(mesh.sphere.center.z-view_pos.z) );


		//if ( (error_metric * mesh.sphere.radius) / dist < ratio_limit ) {
		//	//Too small
		//	continue;
		//}

		if ( inside == false ) {
			ViewFrustum::Containment res = frustum.ContainsSphere(mesh->sphere);
			if ( res == ViewFrustum::OUTSIDE ) { continue; }
			if ( res == ViewFrustum::INTERSECTS ) {
				//The sphere sphere intersects one of the edges of the screen, so try the box test
				res = frustum.ContainsBox(mesh->box);
				if ( res == ViewFrustum::OUTSIDE ) { continue; }
			}
		}

		visible_set.AddMesh( mesh );
	}
}

//-----------------------------------------------------------------------------

void QuadTreeNode::AddMesh( QuadTreeMesh* new_mesh, int depth ) {

	// If this node has no meshes or children, go ahead and add the mesh to it
	if ( GetChildCount() == 0 && !mesh_list ) {
		_InsertMesh( new_mesh );
		return;
	}

	// If we have reached the last level of the quad tree, add the mesh and
	// return
	if ( depth == 0 ) {
		_InsertMesh(new_mesh);
		return;
	}
	
	// If there are already meshes at this node, add it anyway if it's close
	// enough to the first one
	if ( mesh_list ) {
		D3DXVECTOR3 dif = new_mesh->sphere.center - mesh_list->sphere.center;
		float dist = D3DXVec3Length(&dif);
		if ( dist <= QUADTREE_MIN_DIST ) {
			//Go ahead and add the mesh here
			_InsertMesh(new_mesh);
			return;
		}
	}

	// Push down the new mesh and the existing ones
	PushDown(new_mesh, depth);

	// Any existing meshes need to be pushed down too
	QuadTreeMesh* mesh = mesh_list;
	while (mesh) {
		QuadTreeMesh* next_mesh = mesh->next_mesh;
		PushDown( mesh, depth );
		mesh = next_mesh;
	}

	// Clear mesh list
	mesh_list = 0;
}

//-----------------------------------------------------------------------------
	
void QuadTreeNode::PushDown( QuadTreeMesh * new_mesh, int depth ) {
	if ( new_mesh->sphere.center.y > box_center.y ) {
		if (new_mesh->sphere.center.x > box_center.x ) {
			//Quadrent I (0)
			if ( children[0] ) {
				//child already exists, so add the mesh to it
				children[0]->AddMesh(new_mesh, depth - 1);
			} else {
				//create child
				children[0] = m_owner->CreateNode();
				children[0]->box_size = box_size / 2;
				children[0]->box_center.x = box_center.x + box_size / 4;
				children[0]->box_center.y = box_center.y + box_size / 4;

				//Add mesh to new child
				children[0]->AddMesh(new_mesh, depth - 1);
			}
		} else {
			//Quadrent II (1)
			if ( children[1] ) {
				//child already exists, so add the mesh to it
				children[1]->AddMesh(new_mesh, depth - 1);
			} else {
				//create child
				children[1] = m_owner->CreateNode();
				children[1]->box_size = box_size / 2;
				children[1]->box_center.x = box_center.x - box_size / 4;
				children[1]->box_center.y = box_center.y + box_size / 4;

				//Add mesh to new child
				children[1]->AddMesh(new_mesh, depth - 1);
			}
		}
	} else {
		if (new_mesh->sphere.center.x < box_center.x ) {
			//Quadrent III (2)
			if ( children[2] ) {
				//child already exists, so add the mesh to it
				children[2]->AddMesh(new_mesh, depth - 1);
			} else {
				//create child
				children[2] = m_owner->CreateNode();
				children[2]->box_size = box_size / 2;
				children[2]->box_center.x = box_center.x - box_size / 4;
				children[2]->box_center.y = box_center.y - box_size / 4;

				//Add mesh to new child
				children[2]->AddMesh(new_mesh, depth - 1);
			}
		} else {
			//Quadrent IV (3)
			if ( children[3] ) {
				//child already exists, so add the mesh to it
				children[3]->AddMesh(new_mesh, depth - 1);
			} else {
				//create child
				children[3] = m_owner->CreateNode();
				children[3]->box_size = box_size / 2;
				children[3]->box_center.x = box_center.x + box_size / 4;
				children[3]->box_center.y = box_center.y - box_size / 4;

				//Add mesh to new child
				children[3]->AddMesh(new_mesh, depth - 1);
			}
		}
	}
}

//-----------------------------------------------------------------------------

bool QuadTreeNode::Optimize() {
	//Call this function on each child
	for ( size_t i = 0; i < 4; ++i ) {
		QuadTreeNode* old_child = children[i];
		if ( old_child && old_child->Optimize() ) {
			//This child has only one child of its own so it should be removed
			//First get the child
			QuadTreeNode* new_child = 0;
			for ( size_t j = 0; j < 4; ++j ) {
				new_child = old_child->children[j];
				if (new_child) {
					old_child->children[j] = 0;
					break;
				}
			}

			//Replace this child with its child
			children[i] = new_child;
			//delete old_child;
		}
	}
			
	//Report number of children remaining in this node to calling function after optimization is complete
	if ( GetChildCount() == 1 ) {
		//This node should be removed
		return true;
	}
		
	return false;
}

//-----------------------------------------------------------------------------

//void QuadTreeNode::Report( int depth = QUADTREE_MAX_DEPTH ) {
//	for ( int i = 0; i < QUADTREE_MAX_DEPTH - depth; ++ i ) {
//		log_file << "\t";
//	}

//	if ( GetChildCount() > 0 ) {
//		log_file << "+ " << sphere.radius << " (" << sphere.center.x << ", " << sphere.center.y << ", " << sphere.center.z << ")" << endl;

//		if ( children[0] ) { children[0]->Report( depth - 1 ); }
//		if ( children[1] ) { children[1]->Report( depth - 1 ); }
//		if ( children[2] ) { children[2]->Report( depth - 1 ); }
//		if ( children[3] ) { children[3]->Report( depth - 1 ); }
//	} else {
//		log_file << "* "  << sphere.radius << " (" << sphere.center.x << ", " << sphere.center.y << ", " << sphere.center.z << ") : " << meshes.size() << " meshes." << endl;
//	}
//}

//-----------------------------------------------------------------------------

BoundingSphere QuadTreeNode::CalcVolume() {
	//traverse down until we reach the leaf nodes, then percolate the bounding spheres back up
	if ( GetChildCount() != 0 ) {
		bool first_sphere = true;

		if ( children[0] ) {
			if ( first_sphere ) {
				sphere = children[0]->CalcVolume();
				first_sphere = false;
			} else {
				sphere += children[0]->CalcVolume();
			}
		}
		if ( children[1] ) {
			if ( first_sphere ) {
				sphere = children[1]->CalcVolume();
				first_sphere = false;
			} else {
				sphere += children[1]->CalcVolume();
			}
		}
		if ( children[2] ) {
			if ( first_sphere ) {
				sphere = children[2]->CalcVolume();
				first_sphere = false;
			} else {
				sphere += children[2]->CalcVolume();
			}
		}
		if ( children[3] ) {
			if ( first_sphere ) {
				sphere = children[3]->CalcVolume();
				first_sphere = false;
			} else {
				sphere += children[3]->CalcVolume();
			}
		}

		//Return sum of all child spheres
		return sphere;
	}

	//This node dosen't have any children, so sum the bounds of its meshes
	if ( mesh_list ) {

		sphere = mesh_list->sphere;
		QuadTreeMesh* mesh = mesh_list->next_mesh;
		for ( /**/; mesh; mesh = mesh->next_mesh ) {
			sphere += mesh->sphere;
		}
	}

	return sphere;
}

//-----------------------------------------------------------------------------

int QuadTreeNode::GetChildCount() const {
	int count = 0;
	if ( children[0] ) { ++count; };
	if ( children[1] ) { ++count; };
	if ( children[2] ) { ++count; };
	if ( children[3] ) { ++count; };

	return count;
}

//-----------------------------------------------------------------------------

QuadTreeNode::QuadTreeNode(QuadTree* owner) {
	children[0] = 0;
	children[1] = 0;
	children[2] = 0;
	children[3] = 0;

	float box_size = 0.0f;
	box_center.x = 0.0f;
	box_center.y = 0.0f;

	m_owner = owner;
	mesh_list = 0;
}

//-----------------------------------------------------------------------------

QuadTreeNode::~QuadTreeNode() {
	//Don't delete meshes or children because the tree doesn't own them.
	mesh_list = 0;
}

//-----------------------------------------------------------------------------


void QuadTreeNode::_InsertMesh( QuadTreeMesh* new_mesh ) {
	//Insert the mesh at the head of the list
	new_mesh->next_mesh = mesh_list;
	mesh_list = new_mesh;
}

//-----------------------------------------------------------------------------
// QuadTree class
//-----------------------------------------------------------------------------

QuadTree::QuadTree(size_t node_objects, size_t mesh_objects) {

	//Check if the memory pools have been created yet
	if( !m_node_pool ) {
		m_node_pool = new MemoryPool(
			"QuadTree Node Pool",
			sizeof(QuadTreeNode),
			node_objects
		);
	}

	if ( !m_mesh_pool ) {
		m_mesh_pool = new MemoryPool(
			"QuadTree Mesh Pool",
			sizeof(QuadTreeMesh),
			mesh_objects
		);
	}

	//Create the root node
	m_root_node = CreateNode();
}

//-----------------------------------------------------------------------------

QuadTree::QuadTree() {
	assert( m_node_pool );
	assert( m_mesh_pool );
	//Create the root node
	m_root_node = CreateNode();
}

//-----------------------------------------------------------------------------

QuadTree::~QuadTree() {
	//Blow away the memory pools
	if(m_node_pool) m_node_pool->Flush();
	if(m_mesh_pool) m_mesh_pool->Flush();

	m_root_node = 0;
}

//-----------------------------------------------------------------------------

void QuadTree::AddMesh( 
	BoundingSphere sphere,
	BoundingBox box,
	D3DXMATRIX transform,
	IDirect3DTexture9* tex,
	int verts,
	IDirect3DVertexBuffer9* vBuffer,
	int faces,
	IDirect3DIndexBuffer9* iBuffer				   
) {
	QuadTreeMesh* new_mesh = CreateMesh(
		sphere,
		box,
		transform,
		tex,
		verts,
		vBuffer,
		faces,
		iBuffer
	);

	m_root_node->AddMesh( new_mesh );
}

//-----------------------------------------------------------------------------

bool QuadTree::Optimize() {
	return m_root_node->Optimize();
}

//-----------------------------------------------------------------------------

void QuadTree::Clear() {
	//Blow away the memory pools
	if(m_node_pool) m_node_pool->Flush();
	if(m_mesh_pool) m_mesh_pool->Flush();

	//Create a new root node
	if(m_node_pool) m_root_node = CreateNode();
}

//-----------------------------------------------------------------------------

void QuadTree::GetVisibleMeshes( const ViewFrustum & frustum, VisibleSet & visible_set ) {
	m_root_node->GetVisibleMeshes( frustum, visible_set );
}

//-----------------------------------------------------------------------------

void QuadTree::SetBoxSize(float size) {
	m_root_node->box_size = size;
}

//-----------------------------------------------------------------------------

void QuadTree::SetBoxCenter( D3DXVECTOR2 &center ) {
	m_root_node->box_center = center;
}

//-----------------------------------------------------------------------------

void QuadTree::CalcVolume() {
	m_root_node->CalcVolume();
}

//-----------------------------------------------------------------------------

QuadTreeNode* QuadTree::CreateNode() {
	QuadTreeNode* new_obj = (QuadTreeNode*)m_node_pool->Alloc();
	//Use placement new to construct the object
	::new(new_obj) QuadTreeNode(this);

	//Return the new object
	return new_obj;
}

//-----------------------------------------------------------------------------

QuadTreeMesh* QuadTree::CreateMesh(
	BoundingSphere sphere,
	BoundingBox box,
	D3DXMATRIX transform,
	IDirect3DTexture9* tex,
	int verts,
	IDirect3DVertexBuffer9* vBuffer,
	int faces,
	IDirect3DIndexBuffer9* iBuffer								   
) {

	QuadTreeMesh* new_obj = (QuadTreeMesh*)m_mesh_pool->Alloc();
	//Use placement new to construct the object
	::new(new_obj) QuadTreeMesh(
		sphere,
		box,
		transform,
		tex,
		verts,
		vBuffer,
		faces,
		iBuffer
	);

	//Return the new object
	return new_obj;
}

//-----------------------------------------------------------------------------
