/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//-----------------------------------NOTICE----------------------------------//
// Some of this file is automatically filled in by a Python script.  Only    //
// add custom code in the designated areas or it will be overwritten during  //
// the next update.                                                          //
//-----------------------------------NOTICE----------------------------------//

//--BEGIN FILE HEAD CUSTOM CODE--//
#include "../../include/obj/NiSkinInstance.h"
#include "../../include/obj/NiSkinData.h"
#include "../../include/obj/NiSkinPartition.h"
#include "../../include/obj/NiTriBasedGeomData.h"
#include "../../include/obj/NiBinaryExtraData.h"

using namespace Niflib;

#include <vector>
#include <utility>
#include <algorithm>

typedef std::vector<float> WeightList;
typedef std::vector<unsigned short> BoneList;
typedef std::vector<unsigned short> Strip;
typedef std::vector<Strip> Strips;
typedef std::vector<Triangle> Triangles;

typedef std::pair<int,float> BoneWeight;
typedef std::vector<BoneWeight> BoneWeightList;
//--END CUSTOM CODE--//

#include "../../include/FixLink.h"
#include "../../include/ObjectRegistry.h"
#include "../../include/NIF_IO.h"
#include "../../include/obj/NiTriBasedGeom.h"
#include "../../include/obj/NiNode.h"
using namespace Niflib;

//Definition of TYPE constant
const Type NiTriBasedGeom::TYPE("NiTriBasedGeom", &NiGeometry::TYPE );

NiTriBasedGeom::NiTriBasedGeom() {
	//--BEGIN CONSTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

NiTriBasedGeom::~NiTriBasedGeom() {
	//--BEGIN DESTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

const Type & NiTriBasedGeom::GetType() const {
	return TYPE;
}

NiObject * NiTriBasedGeom::Create() {
	return new NiTriBasedGeom;
}

void NiTriBasedGeom::Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiGeometry::Read( in, link_stack, info );

	//--BEGIN POST-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTriBasedGeom::Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const {
	//--BEGIN PRE-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiGeometry::Write( out, link_map, info );

	//--BEGIN POST-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::string NiTriBasedGeom::asString( bool verbose ) const {
	//--BEGIN PRE-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//

	stringstream out;
	unsigned int array_output_count = 0;
	out << NiGeometry::asString();
	return out.str();

	//--BEGIN POST-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTriBasedGeom::FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiGeometry::FixLinks( objects, link_stack, info );

	//--BEGIN POST-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::list<NiObjectRef> NiTriBasedGeom::GetRefs() const {
	list<Ref<NiObject> > refs;
	refs = NiGeometry::GetRefs();
	return refs;
}

//--BEGIN MISC CUSTOM CODE--//

void NiTriBasedGeom::ClearHardareSkinInfo() {
   // Clear the partition info in both places.
   NiSkinInstanceRef skinInst = GetSkinInstance();
   if ( skinInst != NULL ) {
      skinInst->SetSkinPartition( NULL );

      NiSkinDataRef skinData = skinInst->GetSkinData();
      if (skinData != NULL) {
         skinData->SetSkinPartition( NULL );
      }
   }
}


void NiTriBasedGeom::GenHardwareSkinInfo( int max_bones_per_partition /*= 4*/, int max_bones_per_vertex /*= INT_MAX*/ ) {
   NiSkinPartitionRef skinPart; 
   if ( max_bones_per_partition == 0 ) //old method
      skinPart = new NiSkinPartition( this );
   else
      skinPart = new NiSkinPartition( this, max_bones_per_partition, max_bones_per_vertex );

   // Set the partition info in both places and it will be handled when exported.
   NiSkinInstanceRef skinInst = GetSkinInstance();
   if ( skinInst != NULL ) {
      skinInst->SetSkinPartition( skinPart );

      NiSkinDataRef skinData = skinInst->GetSkinData();
      if (skinData != NULL) {
         skinData->SetSkinPartition( skinPart );
      }
   }
}

void NiTriBasedGeom::ReduceSkinWeights( int max_bones_per_tri, int max_bones_per_vert ) {
	NiSkinInstanceRef skinInst = this->GetSkinInstance();
	if ( skinInst == NULL ) {
		throw runtime_error( "You must bind a skin before calling ReduceSkinWeights.  No NiSkinInstance found." );
	}
	NiSkinDataRef skinData = skinInst->GetSkinData();
	if ( skinData == NULL ) {
		throw runtime_error( "You must bind a skin before calling ReduceSkinWeights.  No NiSkinData found." );
	}
	NiTriBasedGeomDataRef geomData = DynamicCast<NiTriBasedGeomData>(this->GetData() );
	if ( geomData == NULL ) {
		throw runtime_error( "Attempted to call ReduceSkinWeights on a mesh with no geometry data." );
	}

	// read in the weights from NiSkinData
	vector<Vector3> verts = geomData->GetVertices();
	vector< BoneWeightList > weights;
	if (verts.empty()){
		throw runtime_error( "Attempted to call ReduceSkinWeights on a mesh with no vertices." );
	}

	Triangles triangles = geomData->GetTriangles();
	if (triangles.empty()) {
		throw runtime_error( "Attempted to call ReduceSkinWeights on a mesh with no triangles." );
	}

	weights.resize( verts.size() );
	int numBones = skinData->GetBoneCount();
	for ( int bone = 0; bone < numBones; bone++ )
	{
		vector<SkinWeight> vertexWeights = skinData->GetBoneWeights(bone);
		for (int r = 0; r < int(vertexWeights.size()); ++r ){
			int vertex = vertexWeights[r].index;
			float weight = vertexWeights[r].weight;
			if ( vertex >= int(weights.size()) )
				throw runtime_error( "bad NiSkinData - vertex count does not match" );
			weights[vertex].insert( weights[vertex].end(), BoneWeight(bone, weight) );
		}
	}

	// count min and max bones per vertex
	int minBones, maxBones;
	minBones = maxBones = (int)weights[0].size();
	for(vector< BoneWeightList >::iterator itr = weights.begin(); itr != weights.end(); ++itr ){
		int n = int( (*itr).size() );
		minBones = min(n, minBones);
		maxBones = max(n, maxBones);
	}

	if ( minBones <= 0 )
		throw runtime_error( "bad NiSkinData - some vertices have no weights at all" );

	// reduce vertex influences if necessary
	if ( maxBones > max_bones_per_vert )
	{
		int c = 0;
		for ( vector< BoneWeightList >::iterator it = weights.begin(); it != weights.end(); ++it )
		{
			BoneWeightList & lst = *it;
			if ( int(lst.size()) > max_bones_per_vert )
				c++;

			while ( int(lst.size()) > max_bones_per_vert ) {
				int j = 0;
				float weight = lst.front().second;
				for ( int i = 0; i < int(lst.size()); i++ )
				{
					if ( lst[i].second < weight )
						j = i;
				}
				BoneWeightList::iterator jit = lst.begin() + j;
				lst.erase( jit );
			}

			float totalWeight = 0;
			for (BoneWeightList::iterator bw = lst.begin(); bw != lst.end(); ++bw) {
				totalWeight += (*bw).second;
			}
			for (BoneWeightList::iterator bw = lst.begin(); bw != lst.end(); ++bw) {
				(*bw).second /= totalWeight;
			}
		}
		//qWarning() << "reduced" << c << "vertices to" << max_bones_per_vert << "bone influences (maximum number of bones per vertex was" << maxBones << ")";
	}

	maxBones = max_bones_per_vert;

	// reduces bone weights so that the triangles fit into the partitions

	typedef multimap<int,int> matchmap;
	typedef pair<matchmap::iterator, matchmap::iterator> matchrange;
	matchmap match;
	bool doMatch = true;

	BoneList tribones;
	int cnt = 0;
	for (Triangles::iterator itr = triangles.begin(); itr != triangles.end(); ++itr) {
		Triangle& tri = (*itr);
		do
		{
			tribones.clear();
			for ( int c = 0; c < 3; c++ ) {
				BoneWeightList& bwl = weights[tri[c]];
				for (BoneWeightList::iterator bw = bwl.begin(); bw != bwl.end(); ++bw) {
					if ( tribones.end() == find(tribones.begin(), tribones.end(), (*bw).first ) )
						tribones.insert(tribones.end(), (*bw).first );
				}
			}

			if ( int(tribones.size()) > max_bones_per_tri )
			{
				// sum up the weights for each bone
				// bones with weight == 1 can't be removed

				map<int, float> sum;
				vector<int> nono;

				for ( int t = 0; t < 3; t++ ) {
					BoneWeightList& bwl = weights[tri[t]];
					if ( bwl.size() == 1 )
						nono.insert(nono.end(), bwl.front().first );

					for (BoneWeightList::iterator bw = bwl.begin(); bw != bwl.end(); ++bw) {
						sum[ (*bw).first ] += (*bw).second;
					}                 
				}

				// select the bone to remove

				float minWeight = 5.0;
				int minBone = -1;

				for (map<int, float>::iterator sitr = sum.begin(); sitr != sum.end(); ++sitr) {
					int b = (*sitr).first;
					if ( (find(nono.begin(), nono.end(), b) == nono.end()) && sum[b] < minWeight) {
						minWeight = sum[b];
						minBone = b;
					}
				}

				if ( minBone < 0 )	// this shouldn't never happen
					throw runtime_error( "internal error 0x01" );

				// do a vertex match detect
				if ( doMatch ) {
					for ( int a = 0; a < (int)verts.size(); a++ ) {
						match.insert(matchmap::value_type(a, a));
						for ( int b = a + 1; b < (int)verts.size(); b++ ) {
							if ( verts[a] == verts[b] && weights[a] == weights[b] ) {
								match.insert(matchmap::value_type(a, b));
								match.insert(matchmap::value_type(b, a));
							}
						}
					}
				}

				// now remove that bone from all vertices of this triangle and from all matching vertices too
				for ( int t = 0; t < 3; t++ ) {
					bool rem = false;

					matchrange range = match.equal_range(tri[t]);
					for (matchmap::iterator itr = range.first; itr != range.second; ++itr) {
						int v = (*itr).second;

						BoneWeightList & bws = weights[ v ];
						BoneWeightList::iterator it = bws.begin();
						while ( it != bws.end() ) {
							BoneWeight & bw = *it;
							if ( bw.first == minBone ) {
								it = bws.erase(it);
								rem = true;
							} else {
								++it;
							}
						}

						float totalWeight = 0;

						for (BoneWeightList::iterator bw = bws.begin(); bw != bws.end(); ++bw) {
							totalWeight += (*bw).second;
						}

						if ( totalWeight == 0 )
							throw runtime_error( "internal error 0x02" );

						// normalize
						for (BoneWeightList::iterator bw = bws.begin(); bw != bws.end(); ++bw) {
							(*bw).second /= totalWeight;
						}
					}
					if ( rem )
						cnt++;
				}
			}
		} while ( int(tribones.size()) > max_bones_per_tri );
	}

	//Build new vertex weight lists for each bone
	size_t weights_size = weights.size();
	SkinWeight temp_weight;

	vector<NiNodeRef> old_bones = skinInst->GetBones();

	vector< vector<SkinWeight> > bone_weights;
	vector<NiNodeRef> new_bones;

	for ( size_t bone = 0; bone < numBones; ++bone ) {

		vector<SkinWeight> vertexWeights;

		//Find all weights that are relevant to this bone
		for ( size_t v = 0; v < weights_size; ++v ) {

			size_t weights_v_size = weights[v].size();
			for ( size_t j = 0; j < weights_v_size; ++j ) {
				if ( weights[v][j].first == bone ) {
					//This weight is for the current bone, so add it to the list
					temp_weight.index = (unsigned short)v;
					temp_weight.weight = weights[v][j].second;
					vertexWeights.push_back( temp_weight );
				}
			}
		}

		//Save the new list if there's anything in it
		if (vertexWeights.size() > 0 ) {
			bone_weights.push_back( vertexWeights );
			new_bones.push_back( old_bones[bone] );
		}
	}

	//Unbind and rebind the mesh to the new list of bones in case some were removed completely
	this->UnbindSkin();
	this->BindSkin( new_bones );

	//Set weights for each bone
	size_t new_bones_size = new_bones.size();
	for ( unsigned int i = 0; i < new_bones_size; ++i ) {
		this->SetBoneWeights( i, bone_weights[i] );
	}
}

void NiTriBasedGeom::UpdateTangentSpace() {

	NiTriBasedGeomDataRef niTriGeomData = DynamicCast<NiTriBasedGeomData>(this->data);

	/* No data, no tangent space */
	if( niTriGeomData == NULL ) {
		throw runtime_error("There is no NiTriBasedGeomData attached the NiGeometry upon which UpdateTangentSpace was called.");
	}

	//Check if there are any UVs or Vertices before trying to retrive them
	if ( niTriGeomData->GetUVSetCount() == 0 ) {
		//There are no UVs, do nothing
		return;
	}

	if ( niTriGeomData->GetVertexCount() == 0 ) {
		//There are no Vertices, do nothing
		return;
	}

	//Get mesh data from data object
	vector<Vector3> verts = niTriGeomData->GetVertices();
	vector<Vector3> norms = niTriGeomData->GetNormals();
	vector<Triangle> tris = niTriGeomData->GetTriangles();
	vector<TexCoord> uvs = niTriGeomData->GetUVSet(0);

	/* check for data validity */
	if(
		verts.size() != norms.size() ||
		verts.size() != uvs.size() ||
		tris.empty()
	) {
		//Do nothing, there is no shape in this data.
		return;
	}

	vector<Vector3> tangents( verts.size() );
	vector<Vector3> binormals( verts.size() );

	for( int t = 0; t < (int)tris.size(); t++ ) {
		Triangle & tri = tris[t];

		int i1 = tri[0];
		int i2 = tri[1];
		int i3 = tri[2];
		
		const Vector3 v1 = verts[i1];
		const Vector3 v2 = verts[i2];
		const Vector3 v3 = verts[i3];
		
		const TexCoord w1 = uvs[i1];
		const TexCoord w2 = uvs[i2];
		const TexCoord w3 = uvs[i3];
		
		Vector3 v2v1 = v2 - v1;
		Vector3 v3v1 = v3 - v1;
		
		TexCoord w2w1(w2.u - w1.u, w2.v - w1.v);
		TexCoord w3w1(w3.u - w1.u, w3.v - w1.v);
		
		float r = w2w1.u * w3w1.v - w3w1.u * w2w1.v;
		
		if ( abs( r ) <= 10e-5 ){
			continue;
		}
		
		r = 1.0f / r;
		
		Vector3 sdir( 
			( w3w1.v * v2v1.x - w2w1.v * v3v1.x ) * r,
			( w3w1.v * v2v1.y - w2w1.v * v3v1.y ) * r,
			( w3w1.v * v2v1.z - w2w1.v * v3v1.z ) * r
		);
		
		Vector3 tdir( 
			( w2w1.u * v3v1.x - w3w1.u * v2v1.x ) * r,
			( w2w1.u * v3v1.y - w3w1.u * v2v1.y ) * r,
			( w2w1.u * v3v1.z - w3w1.u * v2v1.z ) * r
		);

		// no duplication, just smoothing
		for ( int j = 0; j < 3; j++ ) {	
			int i = tri[j];
			
			tangents[i] += sdir.Normalized();
			binormals[i] += tdir.Normalized();
		}
	}

	// for each vertex calculate tangent and binormal
	for ( unsigned i = 0; i < verts.size(); i++ ) {	
		const Vector3 & n = norms[i];
		
		Vector3 & t = tangents[i];
		Vector3 & b = binormals[i];
		
		if ( t == Vector3() || b == Vector3() ) {
			t.x = n.y;
			t.y = n.z;
			t.z = n.x;
			b = n.CrossProduct(t);
		} else {
			t = ( t - n * n.DotProduct(t) );
			t = t.Normalized();
			b = ( b - n * n.DotProduct(b) );
			b = b.Normalized();
		}
	}

	// generate the byte data
	size_t vCount = verts.size();
	int fSize = sizeof(float[3]);
	vector<byte> binData( 2 * vCount * fSize );

	for( unsigned i = 0; i < verts.size(); i++ ) {
		float tan_xyz[3], bin_xyz[3];

		tan_xyz[0] = tangents[i].x;
		tan_xyz[1] = tangents[i].y;
		tan_xyz[2] = tangents[i].z;

		bin_xyz[0] = binormals[i].x;
		bin_xyz[1] = binormals[i].y;
		bin_xyz[2] = binormals[i].z;

		char * tan_Bytes = (char*)tan_xyz;
		char * bin_Bytes = (char*)bin_xyz;

		for( int j = 0; j < fSize; j++ ) {
			binData[ i           * fSize + j] = tan_Bytes[j];
			binData[(i + vCount) * fSize + j] = bin_Bytes[j];
		}
	}

	// update or create the tangent space extra data
	NiBinaryExtraDataRef TSpaceRef;

	std::list<NiExtraDataRef> props = this->GetExtraData();
	std::list<NiExtraDataRef>::iterator prop;

	for( prop = props.begin(); prop != props.end(); ++prop ){
		if((*prop)->GetName() == "Tangent space (binormal & tangent vectors)") {
			TSpaceRef = DynamicCast<NiBinaryExtraData>(*prop);
			break;
		}
	}

	if( TSpaceRef == NULL ) {
		TSpaceRef = new NiBinaryExtraData();
		TSpaceRef->SetName( "Tangent space (binormal & tangent vectors)" );
		this->AddExtraData( StaticCast<NiExtraData>(TSpaceRef) );
	}

	TSpaceRef->SetData(binData);
}

//--END CUSTOM CODE--//
