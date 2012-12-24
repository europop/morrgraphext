#define _DistantLand
#include "Configuration.h"
#include "d3d8header.h"
#include "MorrowindBSA.h"
#include "assert.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <new>
#include "MemoryPool.h"
#include "FogFormulas.h"
#include "MWBridge.h"
#include "QuadTree.h"
#include "Threading.h"
#include "DistantLand.h"

using std::vector;
using std::ofstream;
using std::stringstream;
using std::setw;
using std::setfill;

//Global data for culling
CullingVisibleSets g_vis_sets;
CullingFlags* g_cull_flags = 0;

ThreadFlag* g_cull_flag_grass_n = 0;
VisibleSet g_vis_set_grass_n;

#define ReadPStream(stream, type) *(type*)stream; stream += sizeof(type);

static vector<char*> WorldSpaces;

//ofstream log_file( "C:\\mge_log.txt" );
//ofstream Interior_file( "C:\\Interior_log.txt" ); //d4w

//#define aDistantLand(x,y) DistantLand[(x)+((Options.DL_NearStaticEnd*2+1)*(y))]
#define DLINITERR(a) SETSTATUS(a); UsingDistantLand = false; return false;

//#define DL_VERTEXFORMAT D3DFVF_XYZ|D3DFVF_TEX1
//#define DL_STATICFORMAT D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1
#define DL_WATERFORMAT D3DFVF_XYZ

//Static vertex declaration
IDirect3DVertexDeclaration9* StaticDecl = 0;
const D3DVERTEXELEMENT9 StaticElem[] = {
	//Stream 0 (mesh data)
	{0, 0,  D3DDECLTYPE_FLOAT16_4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 8,  D3DDECLTYPE_UBYTE4N,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 12, D3DDECLTYPE_D3DCOLOR,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	{0, 16, D3DDECLTYPE_FLOAT16_2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

const D3DVERTEXELEMENT9 StaticElemFullFloat[] = {
	//Stream 0 (mesh data)
	{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_UBYTE4N,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 16, D3DDECLTYPE_D3DCOLOR,  D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

//World mesh vertex declaration
IDirect3DVertexDeclaration9* LandDecl = 0;
const D3DVERTEXELEMENT9 LandElem[] = {
	//Stream 0 (mesh data)
	{0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_SHORT2N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

//Instancing data
IDirect3DVertexDeclaration9* HWInstDecl = 0;
const D3DVERTEXELEMENT9 HWInstElem[] = {
	//Stream 0 (mesh data)
	{0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	{0, 28, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},

	//Stream 1 (instance transform matrix)
	{1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	{1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
	{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
	{1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
	D3DDECL_END()
};

TaskScheduler Scheduler;

//Used to improve the frame rate while in menu mode
IDirect3DSurface* distantRenderSurface1;
IDirect3DTexture* distantRenderTexture1;
IDirect3DSurface* distantRenderSurface2;
IDirect3DTexture* distantRenderTexture2;

float lastViewDistance = 0.0f;

float DLProgressBar = 0.0f;

//HW instance buffers which are cycled through
#define HWINSTBUFS 1
IDirect3DVertexBuffer9* HWInstVBs [HWINSTBUFS] = {0};

typedef void (_cdecl *TESSELLATELANDSCAPE)( char* file_path, float* height_data, unsigned int data_height, unsigned int data_width, float top, float left, float bottom, float right, float error_tolerance );
typedef float (_stdcall *PROCESSNIF)(char* data, int datasize, float simplify, float cutoff, BYTE static_type, BYTE old);
typedef void (_stdcall *BEGINSTATICCREATION)(IDirect3DDevice9* _device, char* outpath);
typedef void (_stdcall *ENDSTATICCREATION)();

#define STATICINSTSIZE 50

//Current HW instance buffer
UINT HWInstVBIndex = 0;
UINT HWInstBufferPos = 0;

#define SIZEOFINSTDATA 64
#define MAXINSTANCES 65536

#define SIZEOFVERT 16
static unsigned int SizeOfStaticVert = 20;
#define SIZEOFWVERT 12

#define PASS_REFLECTLAND 0
#define PASS_RENDERLAND 1
#define PASS_SM2WATER 2
#define PASS_SM3WATER 3
#define PASS_RENDERSTATICS 4
#define PASS_UNDERWATER 5
#define PASS_NONREFLECTIVEWATER 6
#define PASS_REFLECTSTATICS 7
#define PASS_LOWESTLAND 8
#define PASS_RENDERSTATICSINST 9
#define PASS_REFLECTSTATICSINST 10
#define PASS_RENDERGRASS 11
#define PASS_RENDERGRASSINST 12
//#define PASS_RENDER_MW_DEPTH 13
#define PASS_BLENDDEPTH 13
#define PASS_BLURHORIZ 14
#define PASS_BLURVERT 15
#define PASS_COPYANDSETALPHA 16
#define PASS_RENDERGRASSDEPTH 17 //phal
#define PASS_RENDERGRASSDEPTHINST 18 //phal
#define PASS_CAUSTICS 19 //phal
#define PASS_WAVE 20 //phal
#define PASS_PLAYERWAVE 21 //phal
#define PASS_SETUP 22

struct WaterVertex { float x,y,z; };

static short OldX,OldY,TempX,TempY;
static bool SetPos;
static size_t DlLoadCount = 0;
static bool Initialized=false;
static bool Initializing=false;
static bool UseInstancing=false;
static bool LastFrameWasMenu=false;
static bool HalfFloatSupported=true;
static bool AlphaToCoverageSupported=false;
static DWORD VendorID = 0;
enum VendorID {
	VENDOR_ATI = 0x1002,
	VENDOR_NVIDIA = 0x10DE,
	VENDOR_INTEL = 0x8086
};

static bool Zoomed=false;
static D3DXMATRIX ReflectProj;
static D3DXMATRIX ReflectView;
static D3DXMATRIX WorldProj;
static D3DXMATRIX CellProj;
static D3DXMATRIX CellProj2; //Used for water reflections. Closer near-z plane.
//static IDirect3DIndexBuffer9* WorldIB;
//static IDirect3DIndexBuffer9* CellIB;
//static DWORD WorldVerts;
//static DWORD WorldFaces;
//extern bool DistantLandActive; //moved to d3d8header.h //phal

static ID3DXEffect* DL_Effect;
static D3DXHANDLE ehWorldViewProj;
static D3DXHANDLE ehMatrixPalette;
static D3DXHANDLE ehView;
static D3DXHANDLE ehProj;
static D3DXHANDLE ehTexProj;
static D3DXHANDLE ehWorld;
static D3DXHANDLE ehTex0;
static D3DXHANDLE ehTex1;
static D3DXHANDLE ehTex2;
static D3DXHANDLE ehTex3;
static D3DXHANDLE ehTex4;
static D3DXHANDLE ehTex5;
static D3DXHANDLE ehSunCol;
static D3DXHANDLE ehSkyCol;
static D3DXHANDLE ehSunAmb;
static D3DXHANDLE ehSunVec;
static D3DXHANDLE ehFogCol;
static D3DXHANDLE ehFogCol2;
static D3DXHANDLE ehFogStart;
static D3DXHANDLE ehFogRange;
static D3DXHANDLE ehBlendStart;
static D3DXHANDLE ehBlendEnd;
static D3DXHANDLE ehGrassDist;
static D3DXHANDLE ehEyePos;
static D3DXHANDLE ehTicks;
static D3DXHANDLE ehOffset;
static D3DXHANDLE ehTexBlend;
static D3DXHANDLE ehWindVec;
static D3DXHANDLE ehPixelWidth;
static D3DXHANDLE ehPixelHeight;
static D3DXHANDLE ehTwelveKernel;
static D3DXHANDLE ehWaterLevel;
static D3DXHANDLE ehAlphaMultiplier;
static D3DXHANDLE ehSunPos;//phal
static D3DXHANDLE ehSunVis;//phal
static D3DXHANDLE ehPlayerPos;//phal
static D3DXHANDLE ehRippleOrigin;//phal
static D3DXHANDLE ehCausticIntens;
static float TotalTime;
static float GrassTime;
static DWORD TimeCounter;
static D3DXVECTOR2 SmoothWindVec;
static bool WeatherRead = false;
float SmoothWindSpeed;
float PixelWidth, PixelHeight;

static D3DXMATRIX WaterWorld;
static IDirect3DIndexBuffer9* WaterIB = 0;
static IDirect3DVertexBuffer9* WaterVB1 = 0;
static IDirect3DVertexBuffer9* WaterVB2 = 0;
static int numWaterVertices = 0; //phal
static int numWaterTris = 0; //phal
static int fineWaterMesh=0; //phal

static IDirect3DIndexBuffer9* LoLandIB = 0; //phal
static IDirect3DVertexBuffer9* LoLandVB = 0; //phal

//To hold our new water normal maps
static IDirect3DVolumeTexture9* NewWaterTexture;
//static IDirect3DTexture* WaterBlendTexture; //phal
//static IDirect3DSurface* WaterBlendSurface = 0; //phal
//static IDirect3DVertexBuffer9* WaterBlendVB = 0; //phal

//Water ripple&wave calculation //phal
static IDirect3DSurface* waveRainSurface;
static IDirect3DTexture* waveRainTexture;
static IDirect3DSurface* waveRippleSurface;
static IDirect3DTexture* waveRippleTexture;
static IDirect3DSurface* waveTempSurface;
static IDirect3DTexture* waveTempTexture;
static IDirect3DVertexBuffer9* WaveVB;
static float waveTexWorldRes;
static int waveTexResolution;

IDirect3DTexture9* RealWaterTextures[32];
IDirect3DTexture9** MorrowWaterPtrs[32];
IDirect3DTexture9* FakeWaterTexture = 0;
IDirect3DTexture9* DefaultLandTexture = 0;

static IDirect3DSurface9* ReflectionZBuffer = 0;

struct InstVertex {
	D3DXVECTOR4 m1;
	D3DXVECTOR4 m2;
	D3DXVECTOR4 m3;
	D3DXVECTOR4 m4;
};
struct DistantSubset {
	float radius;	//radius of bounding sphere
	D3DXVECTOR3 center; //center of bounding sphere
	//Corners of the Axis-alligned bounding box
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	IDirect3DTexture9* tex;
	IDirect3DVertexBuffer9* vBuffer;
	IDirect3DIndexBuffer9* iBuffer;
	int verts;
	int faces;
	//D3DXMATRIX transform;
};

enum StaticType {
    STATIC_AUTO = 0,
    STATIC_NEAR = 1,
    STATIC_FAR = 2,
    STATIC_VERY_FAR = 3,
    STATIC_GRASS = 4,
	STATIC_TREE = 5,
	STATIC_BUILDING = 6
};

struct DistantStatic {
	int NumSubsets;
	float radius;	//radius of bounding sphere
	D3DXVECTOR3 center; //center of bounding sphere

	DistantSubset* Subsets;
	unsigned char Type;

	void Render(D3DXMATRIX* transform, D3DXMATRIX* viewproj) {
		D3DXMATRIX worldviewproj;
		D3DXMATRIX world;
		D3DXMATRIX tempmat;
		world=(*transform);
		worldviewproj=world*(*viewproj);
		DL_Effect->SetMatrix(ehWorld, &world);
		D3DXMatrixInverse(&tempmat, 0, &world);
		D3DXMatrixTranspose(&world, &tempmat);
		//DL_Effect->SetMatrix(ehWorldViewProj, &worldviewproj);
		DL_Effect->SetMatrix(ehProj, &world);

		for(int i=0;i<NumSubsets;i++) {
			//world=(*transform)*Subsets[i].transform;
			////world=Subsets[i].transform*(*transform);
			//worldviewproj=world*(*viewproj);
			//DL_Effect->SetMatrix(ehWorld, &world);
			//D3DXMatrixInverse(&tempmat, 0, &world);
			//D3DXMatrixTranspose(&world, &tempmat);
			DL_Effect->SetTexture(ehTex3, Subsets[i].tex);
			//DL_Effect->SetMatrix(ehWorldViewProj, &worldviewproj);
			//DL_Effect->SetMatrix(ehProj, &world);
			DL_Effect->CommitChanges();
			RealDevice->SetStreamSource(0, Subsets[i].vBuffer, 0, SizeOfStaticVert);
			RealDevice->SetIndices(Subsets[i].iBuffer);
			RealDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, Subsets[i].verts, 0, Subsets[i].faces);
		}
	}
};
static DistantStatic* DistantStatics;
struct UsedDistantStatic {
	DWORD StaticRef;
	//DWORD ID;
	D3DXMATRIX transform;
	float radius; // radius * scale
	D3DXVECTOR3 center; //Center + transform
	float scale;
	D3DXVECTOR3 pos;

	float distance() {
		return (EyePos.x-pos.x)*(EyePos.x-pos.x) + (EyePos.y-pos.y)*(EyePos.y-pos.y);
	}

	//bool CheckVisable(ViewFrustum* frustum) {
	//	for(int i=0;i<6;i++) {
	//		if(D3DXPlaneDotCoord(&frustum->frustum[i], &center) + radius < 0 )
	//			return false;
	//	}
 //       return true;
	//}
	void Render(D3DXMATRIX* viewproj) {
		DistantStatics[StaticRef].Render(&transform, viewproj);
	}

	BoundingSphere GetBoundingSphere( DistantSubset& sub ) {

		BoundingSphere sphere;

		D3DXVECTOR4 output;
		D3DXVec3Transform( &output, &sub.center, &transform );


		sphere.center.x = output.x;
		sphere.center.y = output.y;
		sphere.center.z = output.z;

		sphere.radius = sub.radius * scale;

		return sphere;
	}

	BoundingBox GetBoundingBox( DistantSubset& sub ) {
		BoundingBox box;
		box.Set( sub.min, sub.max );
		box.Transform( transform );

		return box;
	}

};
static vector<vector<UsedDistantStatic>> UsedDistantStatics;
static int DistantStaticCount=0;


class MeshGroup {
public:
	int verts;
	IDirect3DVertexBuffer9* vBuffer;
	int faces;
	IDirect3DIndexBuffer9* iBuffer;
	bool visible;

	//Instance data
	vector<unsigned char> inst_vis;
	vector<D3DXMATRIX> inst_trans;

	MeshGroup() {
		verts = 0;
		vBuffer = 0;
		faces = 0;
		iBuffer = 0;
	}

	MeshGroup( const MeshGroup& rh ) {
		*this = rh;
	}
	MeshGroup& operator=( const MeshGroup& rh ) {
		verts = rh.verts;
		vBuffer = rh.vBuffer;
		faces = rh.faces;
		iBuffer = rh.iBuffer;
		visible = rh.visible;
		inst_vis = rh.inst_vis;
		inst_trans = rh.inst_trans;
		return *this;
	}

	MeshGroup( int vertices, IDirect3DVertexBuffer9* vBuf, int triangles, IDirect3DIndexBuffer9* iBuf ) {
		verts = vertices;
		vBuffer = vBuf;
		faces = triangles;
		iBuffer = iBuf;
	}

	size_t GetVisibleCount() {
		size_t instances = inst_vis.size();
		size_t count = 0;
		unsigned char* inst_vis_ptr = &(inst_vis[0]);
		for ( size_t i = 0; i < instances; ++i ) {
			if ( inst_vis_ptr[i] != 0 ) {
				++count;
			}
		}

		return count;
	}

	~MeshGroup() {}

	static MemoryPool pool;
	static void FlushMemoryPool() {
		pool.Flush();
	}

	static MeshGroup* Create( int vertices, IDirect3DVertexBuffer9* vBuf, int triangles, IDirect3DIndexBuffer9* iBuf ) {
		MeshGroup* new_obj = (MeshGroup*)pool.Alloc();
		//Use placement new to construct the object
		::new(new_obj) MeshGroup( vertices, vBuf, triangles, iBuf );

		//Return the new object
		return new_obj;
	}
};

//Initialize static pool
MemoryPool MeshGroup::pool( "MeshGroup", sizeof(MeshGroup), 3008 );

class TextureGroup {
public:
	IDirect3DTexture9* tex;
	vector<MeshGroup*> meshes;
	bool visible;

	TextureGroup() {
		tex = 0;
	}
	TextureGroup( const TextureGroup& rh ) {
		*this = rh;
	}
	TextureGroup& operator=( const TextureGroup& rh ) {
		tex = rh.tex;
		meshes = rh.meshes;
		visible = rh.visible;

		return *this;
	}

	TextureGroup(IDirect3DTexture9* new_tex) {
		tex = new_tex;
	}
	~TextureGroup() {}

	static MemoryPool pool;
	static void FlushMemoryPool() {
		pool.Flush();
	}

	static TextureGroup* Create( IDirect3DTexture9* new_tex ) {
		TextureGroup* new_obj = (TextureGroup*)pool.Alloc();
		//Use placement new to construct the object
		::new(new_obj) TextureGroup( new_tex );

		//Return the new object
		return new_obj;
	}
};

//Initialize static pool
MemoryPool TextureGroup::pool( "TextureGroup", sizeof(TextureGroup), 512 );

class MeshCollection {
public:
	vector<TextureGroup*> textures;

	struct VisFlags {
		VisFlags() {
			collection = 0;
			tex = 0;
			mesh = 0;
			inst = 0;
		}
		VisFlags( MeshCollection* col, size_t t, size_t m, size_t i ) {
			collection = col;
			tex = t;
			mesh = m;
			inst = i;
		}
		VisFlags( const VisFlags & rh ) {
			*this = rh;
		}
		VisFlags & operator=( const VisFlags& rh ) {
			collection = rh.collection;
			tex = rh.tex;
			mesh = rh.mesh;
			inst = rh.inst;

			return *this;
		}
		~VisFlags() {}

		void Set() {
			if ( collection ) {
				collection->SetFlag( tex, mesh, inst );
			}
		}
	//private:
		MeshCollection* collection;
		size_t tex;
		size_t mesh;
		size_t inst;
	};

	VisFlags Add( IDirect3DTexture9* tex, int verts, IDirect3DVertexBuffer9* vBuffer, int faces, IDirect3DIndexBuffer9* iBuffer, D3DXMATRIX trans ) {

		//First search for the texture
		size_t tex_grp = 0xFFFFFFFF;
		size_t textures_size = textures.size();
		TextureGroup** textures_arr = NULL;
		if ( textures_size > 0 ) {
			textures_arr = &(textures[0]);
			for ( size_t i = 0; i < textures_size; ++i ) {
				if ( textures_arr[i]->tex == tex ) {
					tex_grp = i;
					break;
				}
			}
		}

		//If the texture wasn't found, then add it
		if ( tex_grp == 0xFFFFFFFF ) {
			tex_grp = textures.size();
			textures.push_back( TextureGroup::Create(tex) );
			textures_arr = &(textures[0]);
		}

		//Next search for the mesh
		size_t mesh_grp = 0xFFFFFFFF;
		vector<MeshGroup*> & meshes = textures_arr[tex_grp]->meshes;
		size_t meshes_size = meshes.size();
		MeshGroup** meshes_arr = 0;
		if ( meshes_size > 0 ) {
			meshes_arr = &(meshes[0]);
			for ( size_t i = 0; i < meshes_size; ++i ) {
				if ( meshes_arr[i]->vBuffer == vBuffer ) {
					mesh_grp = i;
					break;
				}
			}
		}

		//If the mesh wasn't found, then add it
		if ( mesh_grp == 0xFFFFFFFF ) {
			mesh_grp = meshes.size();
			meshes.push_back( MeshGroup::Create( verts, vBuffer, faces, iBuffer ) );
			meshes_arr = &(meshes[0]);
		}

		//Finally, add the instance transform
		size_t inst_ind = meshes_arr[mesh_grp]->inst_trans.size();
		meshes_arr[mesh_grp]->inst_trans.push_back( trans );
		meshes_arr[mesh_grp]->inst_vis.push_back( false );

		return VisFlags( this, tex_grp, mesh_grp, inst_ind );
	}

	VisFlags Add( UsedDistantStatic* uds, DistantSubset* sub ) {
		return Add( sub->tex, sub->verts, sub->vBuffer, sub->faces, sub->iBuffer, uds->transform );
	}

	void SetFlag( size_t tex, size_t mesh, size_t inst ) {
		textures[tex]->visible = true;
		textures[tex]->meshes[mesh]->visible = true;
		textures[tex]->meshes[mesh]->inst_vis[inst] = true;
	}

	void ResetFlags() {
		size_t tex_size = textures.size();
		for ( size_t t = 0; t < tex_size; ++t ) {
			TextureGroup & tex = *textures[t];
			tex.visible = false;
			size_t meshes_size = tex.meshes.size();
			for ( size_t m = 0; m < meshes_size; ++m ) {
				MeshGroup & mesh = *tex.meshes[m];
				mesh.visible = false;
				//memset( &(mesh.inst_vis[0]), 0, mesh.inst_vis.size() * sizeof(bool) );

				vector<unsigned char> & inst_vis = mesh.inst_vis;

				size_t inst_size = inst_vis.size();

				//Clear in groups of 32-bits
				unsigned int * inst_vis_ptr32 = (unsigned int*)&(inst_vis[0]);
				size_t inst_size32 = inst_size / 4;
				for( size_t i = 0; i < inst_size32; ++i ) {
					inst_vis_ptr32[i] = 0;
				}

				//Clear the remaining bytes one at a time
				unsigned char * inst_vis_ptr = &(inst_vis[0]);
				for ( size_t i = inst_size32 * 4; i < inst_size; ++i ) {
					inst_vis_ptr[i] = 0;
				}
			}
		}
	}

	void RemoveAll() {
		textures.clear();
	}
};

static MeshCollection StaticCollection;
static MeshCollection GrassCollection;

//Global root of the distant object quadtree
static int CurrentWorldSpace = -1;
static vector<QuadTree*> NearQuadTree;
static QuadTree* NearStatics;
static vector<QuadTree*> FarQuadTree;
static QuadTree* FarStatics;
static vector<QuadTree*> VeryFarQuadTree;
static QuadTree* VeryFarStatics;
static vector<QuadTree*> GrassQuadTree;
static QuadTree* GrassStatics;
static vector<QuadTree*> TreeQuadTree;
static QuadTree* TreeStatics;

const D3DVERTEXELEMENT9 Elements[] = {
	//Stream 0 - position and texture coordinates
	{0, 0,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	//Stream 1 - normals and vertex colors
	{1, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0},
	{1, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,   0},    
	//Stream 2 - Texture weights
	{2, 0,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,   1},
	D3DDECL_END()
};

const D3DVERTEXELEMENT9 NormalElements[] = {
	//Stream 0 - position and texture coordinates
	{0, 0,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	//Stream 1 - normals and vertex colors
	{1, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0},
	{1, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,   0},    
	D3DDECL_END()
};

class DistantLandCreator {
public:
	class LTEX {
	public:
		int index;
		char *FilePath;
		IDirect3DTexture9* tex;
		static const DWORD Section = 0x5845544C;
		LTEX(char *filePath) {
			int length = strlen(filePath) + 1;
			tex = NULL;
			index = NULL;
			FilePath = _strlwr(strncpy(new char[length], filePath, length));
			LoadTexture();
		}
		LTEX() {
			tex = NULL;
			index = NULL;
			FilePath = NULL;
		}
		~LTEX() {
			delete[] FilePath;
		}
		void LoadTexture() {
			tex = BSALoadTexture(DistantLandCreator::device, FilePath, 0);
		}
		static void ReleaseCache() {
			ClearTexturesCache();
		}
		static bool SortPredicate(LTEX *a, LTEX *b) {
            return a->index < b->index;
        }
	};
	struct ARGB {
		byte x, r, g, b;
		ARGB(byte _r, byte _g, byte _b) : x(0), r(_r), g(_g), b(_b) {}
		ARGB() : x(0) { r = g = b = 255; }	// White
	};
	struct Normal {
		float X, Y, Z;
		Normal(char x, char y, char z) : X(x), Y(y), Z(z) {
			float mag = sqrt(X*X + Y*Y + Z*Z);
			X /= mag;
			Y /= mag;
			Z /= mag;
		}
		Normal(float x, float y, float z) : X(x), Y(y), Z(z) {}
		Normal() : X(0), Y(0), Z(1) {}	// Up
	};
	class LAND {
	public:
		int Heights[65][65];
		ARGB Color[65][65];
		Normal Normals[65][65];
		int Tex[16][16];
		int xpos;
		int ypos;
		std::vector<LTEX*> *Textures;
		static const DWORD Section = 0x444E414C;
		LAND() {
			xpos = -999;
			ypos = -999;
			Textures = NULL;
			memset(Tex, 0, sizeof(Tex));
			for(int y = 0; y < 65; y++) {
				for(int x = 0; x < 65; x++) {
					Heights[x][y] = -256;
				}
			}
		}
	};

	class CellTexCreator {
	private:
		class TextureBank {
		public:
			LTEX *t1, *t2, *t3, *t4;
			IDirect3DVertexBuffer9* wBuffer;
			struct WeightVertex {
				byte w1, w2, w3, w4;
				static const DWORD Format = D3DFVF_DIFFUSE;
				static const int Stride = 4;
			};
			TextureBank() {
				t1 = t2 = t3 = t4 = NULL;
				device->CreateVertexBuffer(4225 * WeightVertex::Stride, D3DUSAGE_WRITEONLY, WeightVertex::Format, D3DPOOL_MANAGED, &wBuffer, NULL);
			}
			void SetSingleTexture( LTEX *tex1 ) {
				//There this will create a default bank which can be used for default land which has one texture
				t1 = tex1;
				WeightVertex *WeightData;
				wBuffer->Lock(0, 0, (void**)&WeightData, NULL);
				fill(LPDWORD(WeightData), LPDWORD(WeightData) + 4225, DWORD(255));	//Write values
				wBuffer->Unlock();
			}
		private:
			WeightVertex* SampleWeightData(WeightVertex *wvArray, int x, int y) {
				//Ensure that x and y do not escape the bounds of the array.
				if (x < 0) { x = 0; }
				if (y < 0) { y = 0; }
				if (x > 64) { x = 64; }
				if (y > 64) { y = 64; }
				//Return the value at the constrained location
				return &wvArray[y * 65 + x];
			}
		public:
			void CalcWeights(LAND *cell) {
				WeightVertex WeightData[65 * 65];;
				memset(WeightData, 0, sizeof(WeightData));
				for (int y = 0; y <= 64; y++) {
					for (int x = 0; x <= 64; x++) {
						//Figure out which index to use
						int i = x * 65 + y;
						//Figure out which texture is used here
						int cell_x = cell->xpos;
						int cell_y = cell->ypos;
						int tex_x = (int)floor(((float)y - 1.0f) / 4.0f);//-2.0f
						int tex_y = (int)floor(((float)x - 3.0f) / 4.0f);//-2.0f
						if (tex_x < 0) { cell_x--; tex_x += 16; } else if (tex_x >= 16) { cell_x++; tex_x -= 16; }
						if (tex_y < 0) { cell_y--; tex_y += 16; } else if (tex_y >= 16) { cell_y++; tex_y -= 16; }
						LTEX *tex_i = DefaultTex;
						if (Map[cell_x][cell_y] != NULL) tex_i = (*Map[cell_x][cell_y]->Textures)[Map[cell_x][cell_y]->Tex[tex_x][tex_y]];
						//Write values
						if (t1 != NULL && t1->FilePath == tex_i->FilePath) { WeightData[i].w1 = 255; continue; }
						if (t2 != NULL && t2->FilePath == tex_i->FilePath) { WeightData[i].w2 = 255; continue; }
						if (t3 != NULL && t3->FilePath == tex_i->FilePath) { WeightData[i].w3 = 255; continue; }
						if (t4 != NULL && t4->FilePath == tex_i->FilePath) { WeightData[i].w4 = 255; continue; }
					}
				}
				//Blur the weights as we transfer them so the transitions aren't quite so blocky and horrible.
				//Reduced center influence
				float bf1 = 0.1f;
				float bf2 = 0.15f;
				float bf3 = 0.2f;
				float bf4 = 0.1f;
				//Horizontal Pass
				WeightVertex FirstPassWD[65 * 65];
				memset(FirstPassWD, 0, sizeof(FirstPassWD));
				for (int y = 0; y <= 64; y++) {
					for (int x = 0; x <= 64; x++) {
						//Figure out which index to use
						int i = y * 65 + x;
						//We're at the edge, so just copy the value (don't want to interfere with the way the edges of cells look
						if (x == 0 || x == 64 || y == 0 || y == 64) { FirstPassWD[i] = WeightData[i]; continue; }
						//We're not at the edge, so add some influence from the surrounding weights
						//Additional incides
						WeightVertex *wv0, *wv1, *wv2, *wv3, *wv4, *wv5, *wv6;

						wv0 = SampleWeightData( WeightData, x - 3, y);
						wv1 = SampleWeightData( WeightData, x - 2, y);
						wv2 = SampleWeightData( WeightData, x - 1, y);
						wv3 = SampleWeightData( WeightData, x, y);
						wv4 = SampleWeightData( WeightData, x + 1, y);
						wv5 = SampleWeightData( WeightData, x + 2, y);
						wv6 = SampleWeightData( WeightData, x + 2, y);

						float value;
						value = 0.0f;
						value += (float)wv0->w1 * bf1;
						value += (float)wv1->w1 * bf2;
						value += (float)wv2->w1 * bf3;
						value += (float)wv3->w1 * bf4;
						value += (float)wv4->w1 * bf3;
						value += (float)wv5->w1 * bf2;
						value += (float)wv6->w1 * bf1;
						FirstPassWD[i].w1 = (byte)value;

						value = 0.0f;
						value += (float)wv0->w2 * bf1;
						value += (float)wv1->w2 * bf2;
						value += (float)wv2->w2 * bf3;
						value += (float)wv3->w2 * bf4;
						value += (float)wv4->w2 * bf3;
						value += (float)wv5->w2 * bf2;
						value += (float)wv6->w2 * bf1;
						FirstPassWD[i].w2 = (byte)value;

						value = 0.0f;
						value += (float)wv0->w3 * bf1;
						value += (float)wv1->w3 * bf2;
						value += (float)wv2->w3 * bf3;
						value += (float)wv3->w3 * bf4;
						value += (float)wv4->w3 * bf3;
						value += (float)wv5->w3 * bf2;
						value += (float)wv6->w3 * bf1;
						FirstPassWD[i].w3 = (byte)value;

						value = 0.0f;
						value += (float)wv0->w4 * bf1;
						value += (float)wv1->w4 * bf2;
						value += (float)wv2->w4 * bf3;
						value += (float)wv3->w4 * bf4;
						value += (float)wv4->w4 * bf3;
						value += (float)wv5->w4 * bf2;
						value += (float)wv6->w4 * bf1;
						FirstPassWD[i].w4 = (byte)value;
					}
				}
				//Vertical pass - writes to final vertex buffer
				WeightVertex *FinalWeightData;
				wBuffer->Lock(0, 0, (void**)&FinalWeightData, NULL);
				//Blur the weights as we transfer them so the transitions aren't quite so blocky and horrible.
				for (int y = 0; y <= 64; y++) {
					for (int x = 0; x <= 64; x++) {
						//Figure out which index to use
						int i = y * 65 + x;
						//We're at the edge, so just copy the value (don't want to interfere with the way the edges of cells look
						if (x == 0 || x == 64 || y == 0 || y == 64) { FinalWeightData[i] = WeightData[i]; continue; }
						//We're not at the edge, so add some influence from the surrounding weights
						//Additional incides
						WeightVertex *wv0, *wv1, *wv2, *wv3, *wv4, *wv5, *wv6;
						wv0 = SampleWeightData( FirstPassWD, x, y - 2);
						wv1 = SampleWeightData( FirstPassWD, x, y - 2);
						wv2 = SampleWeightData( FirstPassWD, x, y - 1);
						wv3 = SampleWeightData( FirstPassWD, x, y);
						wv4 = SampleWeightData( FirstPassWD, x, y + 1);
						wv5 = SampleWeightData( FirstPassWD, x, y + 2);
						wv6 = SampleWeightData( FirstPassWD, x, y - 2);

						float value;
						value = 0.0f;
						value += (float)wv0->w1 * bf1;
						value += (float)wv1->w1 * bf2;
						value += (float)wv2->w1 * bf3;
						value += (float)wv3->w1 * bf4;
						value += (float)wv4->w1 * bf3;
						value += (float)wv5->w1 * bf2;
						value += (float)wv6->w1 * bf1;
						FinalWeightData[i].w1 = (byte)value;

						value = 0.0f;
						value += (float)wv0->w2 * bf1;
						value += (float)wv1->w2 * bf2;
						value += (float)wv2->w2 * bf3;
						value += (float)wv3->w2 * bf4;
						value += (float)wv4->w2 * bf3;
						value += (float)wv5->w2 * bf2;
						value += (float)wv6->w2 * bf1;
						FinalWeightData[i].w2 = (byte)value;

						value = 0.0f;
						value += (float)wv0->w3 * bf1;
						value += (float)wv1->w3 * bf2;
						value += (float)wv2->w3 * bf3;
						value += (float)wv3->w3 * bf4;
						value += (float)wv4->w3 * bf3;
						value += (float)wv5->w3 * bf2;
						value += (float)wv6->w3 * bf1;
						FinalWeightData[i].w3 = (byte)value;

						value = 0.0f;
						value += (float)wv0->w4 * bf1;
						value += (float)wv1->w4 * bf2;
						value += (float)wv2->w4 * bf3;
						value += (float)wv3->w4 * bf4;
						value += (float)wv4->w4 * bf3;
						value += (float)wv5->w4 * bf2;
						value += (float)wv6->w4 * bf1;
						FinalWeightData[i].w4 = (byte)value;
					}
				}
				wBuffer->Unlock();
			}
			~TextureBank() {
				wBuffer->Release();
			}
		};
		struct CellVertex {
			float x, y, z, w;
			float u, v;
			static const DWORD Format = D3DFVF_XYZ | D3DFVF_TEX1;
			static const int Stride = 24;
		};
		struct NormalColorVertex {
			float nx, ny, nz;
			byte b, g, r, a;
			static const DWORD Format = D3DFVF_NORMAL | D3DFVF_DIFFUSE;
			static const int Stride = 16;
		};

		IDirect3DVertexBuffer9* vBuffer;
		IDirect3DVertexBuffer9* colorBuffer;
		std::vector<TextureBank*> texBanks;
		IDirect3DIndexBuffer9* iBuffer;
		IDirect3DTexture9* RenderTargetTex;
		IDirect3DTexture9* CompressedTex;
		IDirect3DSurface9* RenderTarget;
		ID3DXEffect* effect;
		//private bool vcolor;
		D3DXHANDLE m1h;
		D3DXHANDLE t1h;
		D3DXHANDLE t2h;
		D3DXHANDLE t3h;
		D3DXHANDLE t4h;

	public:
		CellTexCreator(int Res) {
			//Create basic vertex buffer that can bue used for all cells which has positions and texture coordinates
			device->CreateVertexBuffer(4225 * CellVertex::Stride, D3DUSAGE_WRITEONLY, CellVertex::Format, D3DPOOL_MANAGED, &vBuffer, NULL);
			CellVertex *CellData;
			vBuffer->Lock(0, 0, (void**)&CellData, NULL);
			//vBuffers=new VertexBuffer[64, 64];
			float mult = float(Res / 64);
			for(int y = 0; y <= 64; y++) {
				for(int x = 0; x <= 64; x++) {
					//Figure out which index to use
					int i = y * 65 + x;
					//Write values
					CellData[i].x = ((float)x / 64.0f) * 2.0f - 1.0f;
					CellData[i].y = ((float)y / 64.0f) * 2.0f - 1.0f;
					CellData[i].z = 0.5f;
					CellData[i].w = 1.0f;
					CellData[i].u = (float)x / 16.0f;
					CellData[i].v = (float)y / 16.0f;
				}
			}
			vBuffer->Unlock();
			//Create triangle strip index buffer
			//Size is 2r + 2rc + 2(r-1) where r is rows and c is colums (squares, not vertices)
			device->CreateIndexBuffer(8446 * sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &iBuffer, 0);
			short *iBuf;
			iBuffer->Lock(0, 0, (void**)&iBuf, NULL);
			int idx = 0;
			for(int y = 0; y < 64; y++) {
				//If this is is a continuation strip, we need to add two extra vertices to create degenerat triangles
				//and get us back to the left side
				if ( y > 0 ) {
					iBuf[idx] = short(y * 65 + (63+1));
					iBuf[idx + 1] = short(y * 65 + 0);
					idx += 2;
				}
				//Start the row off with a vertex in the lower left corner of the square
				iBuf[idx] = short(y * 65 + 0);
				++idx;
				for(int x = 0; x < 64; x++) {
					//Add the top left and bottom right vertex of each square
					iBuf[idx] = short((y+1) * 65 + x);
					iBuf[idx + 1] = short(y * 65 + (x+1));
					idx += 2;
				}
				//End the row with the top right vertex
				iBuf[idx] = short((y+1) * 65 + (63+1));
				++idx;
			}
			iBuffer->Unlock();
			//Create the buffers that will contain different information during each render
			device->CreateVertexBuffer(4225 * NormalColorVertex::Stride, D3DUSAGE_WRITEONLY, NormalColorVertex::Format, D3DPOOL_MANAGED, &colorBuffer, NULL);
			ResetColorsAndNormals();
			device->CreateTexture(Res, Res, 0, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &RenderTargetTex, NULL);
			device->CreateTexture(Res, Res, 0, NULL, D3DFMT_DXT1, D3DPOOL_SYSTEMMEM, &CompressedTex, NULL);
			RenderTargetTex->GetSurfaceLevel(0, &RenderTarget);
			D3DXCreateEffectFromFile(device, "data files\\shaders\\distantland\\CellTexBlend.fx", NULL, NULL, 1 << 11, NULL, &effect, NULL);
			m1h = effect->GetParameterByName(NULL, "transform");
			t1h = effect->GetParameterByName(NULL, "t1");
			t2h = effect->GetParameterByName(NULL, "t2");
			t3h = effect->GetParameterByName(NULL, "t3");
			t4h = effect->GetParameterByName(NULL, "t4");
		}
		void ResetColorsAndNormals() {
			//By default, the normal will be up and the color will be white
			NormalColorVertex *ColorNormalData, ncVertex;
			ncVertex.r = 255;
			ncVertex.g = 255;
			ncVertex.b = 255;
			ncVertex.a = 255;
			ncVertex.nx = 0.0f;
			ncVertex.ny = 0.0f;
			ncVertex.nz = 1.0f;
			colorBuffer->Lock(0, 0, (void**)&ColorNormalData, NULL);
			fill(ColorNormalData, ColorNormalData + 4225, ncVertex);	//Write values
			colorBuffer->Unlock();
		}
		void SetDefaultCell(LTEX *tex) {
			ResetColorsAndNormals();
			for(size_t i=0; i<texBanks.size(); i++) delete texBanks[i];
			texBanks.clear();
			TextureBank *tb = new TextureBank();
			tb->SetSingleTexture(tex);
			texBanks.push_back(tb);
		}
		void SetCell( LAND *cell ) {    
			//Write the new colors and normals into the color buffer
			NormalColorVertex *ColorNormalData;
			colorBuffer->Lock(0, 0, (void**)&ColorNormalData, NULL);
			for (int y = 0; y <= 64; y++) {
				for (int x = 0; x <= 64; x++) {
					//Figure out which index to use
					int i = y * 65 + x;
					//Write values
					ColorNormalData[i].r = cell->Color[x][y].r;
					ColorNormalData[i].g = cell->Color[x][y].g;
					ColorNormalData[i].b = cell->Color[x][y].b;
					ColorNormalData[i].a = 255;
					ColorNormalData[i].nx = cell->Normals[x][y].X;
					ColorNormalData[i].ny = cell->Normals[x][y].Y;
					ColorNormalData[i].nz = cell->Normals[x][y].Z;
				}
			}
			colorBuffer->Unlock();
			//Dispose of any current texture banks
			for(size_t i=0; i<texBanks.size(); i++) delete texBanks[i];
			texBanks.clear();
			//Group the unique textures in this cell in fours
			//Find all te uniqe textures in this cell
			std::map<char*, LTEX*> tex_dict;
			for (int x = 0; x <= 64; ++x)  {
				for (int y = 0; y <= 64; ++y) {
					int cell_x = cell->xpos;
					int cell_y = cell->ypos;
					int tex_x = (int)floor(((float)y - 1.0f) / 4.0f);//-2.0f
					int tex_y = (int)floor(((float)x - 3.0f) / 4.0f);//-2.0f
					if (tex_x < 0) { cell_x--; tex_x += 16; } else if (tex_x >= 16) { cell_x++; tex_x -= 16; }					
					if (tex_y < 0) { cell_y--; tex_y += 16; } else if (tex_y >= 16) { cell_y++; tex_y -= 16; }
					LTEX *tex_i = DefaultTex;
					if (Map[cell_x][cell_y] != NULL) tex_i = (*Map[cell_x][cell_y]->Textures)[Map[cell_x][cell_y]->Tex[tex_x][tex_y]];
					tex_dict[tex_i->FilePath] = tex_i;
				}
			}
			//Create one bank for each group of 4 textures
			int index = 0;
			TextureBank *tb = new TextureBank();
			for(std::map<char*, LTEX*>::const_iterator it=tex_dict.begin(); it!=tex_dict.end(); ++it) {
				LTEX *tex = it->second;
				switch (index) {
					case 0: tb->t1 = tex; ++index; break;
					case 1: tb->t2 = tex; ++index; break;
					case 2: tb->t3 = tex; ++index; break;
					case 3: tb->t4 = tex; texBanks.push_back(tb); tb = new TextureBank(); index = 0; break;
				}
			}
			if (index != 0) { texBanks.push_back(tb); } else delete tb;
			//Calculate weights for all banks		
			for(size_t i=0; i<texBanks.size(); i++) { texBanks[i]->CalcWeights(cell); }
			tex_dict.clear();
		}
		void Dispose() {
//			device->SetRenderTarget(0, BackBuffer);
			vBuffer->Release();
			colorBuffer->Release();
			for(size_t i=0; i<texBanks.size(); i++) delete texBanks[i];
			texBanks.clear();
			RenderTarget->Release();
			RenderTargetTex->Release();
			CompressedTex->Release();
			effect->Release();
		}
		void Begin() {
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			device->SetRenderState(D3DRS_CLIPPING, TRUE);
			device->SetFVF(CellVertex::Format);
			device->SetStreamSource(0, vBuffer, 0, CellVertex::Stride);
			device->SetStreamSource(1, colorBuffer, 0, NormalColorVertex::Stride);
			IDirect3DVertexDeclaration9* decl;
			device->CreateVertexDeclaration(Elements, &decl);
			device->SetIndices(iBuffer);
			device->SetVertexDeclaration(decl);
		}
		void BeginNormalMap() {
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			device->SetRenderState(D3DRS_CLIPPING, TRUE);
			device->SetFVF(CellVertex::Format);
			device->SetStreamSource(0, vBuffer, 0, CellVertex::Stride);
			device->SetStreamSource(1, colorBuffer, 0, NormalColorVertex::Stride);
			IDirect3DVertexDeclaration9* decl;
			device->CreateVertexDeclaration(NormalElements, &decl);
			device->SetIndices(iBuffer);
			device->SetVertexDeclaration(decl);
		}
		void Render( float pos_x, float pos_y, float scale_x, float scale_y ) {
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			mat._41 = pos_x;
			mat._42 = pos_y;
			mat._11 = scale_x;
			mat._22 = scale_y;
			effect->SetMatrix(m1h, &mat);
			for(size_t i=0; i<texBanks.size(); i++) {
				effect->SetTexture(t1h, texBanks[i]->t1->tex);
				if (texBanks[i]->t2 != NULL) { effect->SetTexture(t2h, texBanks[i]->t2->tex); } else { effect->SetTexture(t2h, texBanks[i]->t1->tex); }
				if (texBanks[i]->t3 != NULL) { effect->SetTexture(t3h, texBanks[i]->t3->tex); } else { effect->SetTexture(t3h, texBanks[i]->t1->tex); }
				if (texBanks[i]->t4 != NULL) { effect->SetTexture(t4h, texBanks[i]->t4->tex); } else { effect->SetTexture(t4h, texBanks[i]->t1->tex); }
				effect->CommitChanges();
				device->SetStreamSource(2, texBanks[i]->wBuffer, 0, TextureBank::WeightVertex::Stride);
				device->BeginScene();
				UINT passes;
				effect->Begin(&passes, NULL);
				effect->BeginPass(0);
				device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4225, 0, 8444);
				effect->EndPass();
				effect->End();
				device->EndScene();
			}
			device->BeginScene();
			UINT passes;
			effect->Begin(&passes, NULL);
			effect->BeginPass(2);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4225, 0, 8444);
			effect->EndPass();
			effect->End();
			device->EndScene();
		}
		void RenderNormalMap(float pos_x, float pos_y, float scale_x, float scale_y) {
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			mat._41 = pos_x;
			mat._42 = pos_y;
			mat._11 = scale_x;
			mat._22 = scale_y;
			effect->SetMatrix(m1h, &mat);
			effect->CommitChanges();
			device->BeginScene();
			UINT passes;
			effect->Begin(&passes, NULL);
			effect->BeginPass(1);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4225, 0, 8444);
			effect->EndPass();
			effect->End();
			device->EndScene();
		}
		void End() {}
		void EndNormalMap() { End(); }
	};

	class WorldTexCreator {
	private:
		IDirect3DTexture9* CompressedTex;
		IDirect3DTexture9* UncompressedTex;
		IDirect3DTexture9* RenderTargetTex;
		IDirect3DSurface9* RenderTarget;
		int MapMinX, MapMaxX, MapMinY, MapMaxY, MapSpanX, MapSpanY;
	public:	
		float x_scale, y_scale, x_spacing, y_spacing;
		WorldTexCreator(int Res, int map_min_x, int map_max_x, int map_min_y, int map_max_y) {
			device->CreateTexture(Res, Res, 0, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &RenderTargetTex, NULL);
			device->CreateTexture(Res, Res, 0, NULL, D3DFMT_DXT1, D3DPOOL_SYSTEMMEM, &CompressedTex, NULL);
			device->CreateTexture(Res, Res, 0, NULL, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &UncompressedTex, NULL);
			RenderTargetTex->GetSurfaceLevel(0, &RenderTarget);
			MapMinX = map_min_x;
			MapMaxX = map_max_x;
			MapMinY = map_min_y;
			MapMaxY = map_max_y;
			MapSpanX = MapMaxX - MapMinX + 1;
			MapSpanY = MapMaxY - MapMinY + 1;
			x_scale = 1.0f / (float)MapSpanX;
			y_scale = 1.0f / (float)MapSpanY;
			x_spacing = x_scale * 2.0f;
			y_spacing = y_scale * 2.0f;
		}
		void Begin() {
			IDirect3DSurface9* ppRenderTarget;
			device->GetRenderTarget(0, &ppRenderTarget);
			if (ppRenderTarget != RenderTarget) {
				device->SetRenderTarget(0, RenderTarget);
			}
			device->Clear(0, NULL, D3DCLEAR_TARGET, 0, 0.0f, 0);
		}
		void FinishCompressed(char *path, bool twoStep) {
			LPDIRECT3DSURFACE9 output;
			if(twoStep) {
				LPDIRECT3DSURFACE9 tmp;
				UncompressedTex->GetSurfaceLevel(0, &tmp);
				D3DXLoadSurfaceFromSurface(tmp, NULL, NULL, RenderTarget, NULL, NULL, D3DX_FILTER_NONE, 0);
				CompressedTex->GetSurfaceLevel(0, &output);
				D3DXLoadSurfaceFromSurface(output, NULL, NULL, tmp, NULL, NULL, D3DX_FILTER_NONE, 0);
				tmp->Release();
			} else {
				CompressedTex->GetSurfaceLevel(0, &output);
				D3DXLoadSurfaceFromSurface(output, NULL, NULL, RenderTarget, NULL, NULL, D3DX_FILTER_NONE, 0);
			}
			D3DXFilterTexture(CompressedTex, NULL, 0, D3DX_FILTER_BOX);
			D3DXSaveTextureToFile(path, D3DXIFF_DDS, CompressedTex, NULL);
			output->Release();
		}
		void FinishUncompressed(char *path) {
			LPDIRECT3DSURFACE9 output;
			UncompressedTex->GetSurfaceLevel(0, &output);
			D3DXLoadSurfaceFromSurface(output, NULL, NULL, RenderTarget, NULL, NULL, D3DX_FILTER_NONE, 0);
			D3DXFilterTexture(UncompressedTex, NULL, 0, D3DX_FILTER_BOX);
			D3DXSaveTextureToFile(path, D3DXIFF_TGA, UncompressedTex, NULL);
			output->Release();
		}
		void Dispose() {
			RenderTarget->Release();
			CompressedTex->Release();
			UncompressedTex->Release();
			RenderTargetTex->Release();
		}
	};
	class TES3 {
	public:
		static const DWORD Section = 0x33534554;
		static const DWORD MAST = 0x5453414D;
	};
	class CELL {
	public:
		static const DWORD Section = 0x4C4C4543;
		static const DWORD DATA = 0x41544144;
		static const DWORD FRMR = 0x524D5246;
		static const DWORD DELE = 0x454C4544;
		static const DWORD NAME = 0x454D414E;
		static const DWORD XSCL = 0x4C435358;
	};
	class Static {
	public:
		static const DWORD STAT = 0x54415453;
		static const DWORD ACTI = 0x49544341;
		static const DWORD DOOR = 0x524F4F44;
		static const DWORD CONT = 0x544E4F43;
		static const DWORD LIGH = 0x4847494C;
		static const DWORD NAME = 0x454D414E;
		static const DWORD MODL = 0x4C444F4D;
		static const DWORD SCRI = 0x49524353;
		static bool IsStatic(DWORD dwLabel) {
			return dwLabel == STAT || dwLabel == ACTI || dwLabel == DOOR || dwLabel == CONT || dwLabel == LIGH;
		}
	public:
		char *name;
		char *mesh;
		float size;
		Static(char *Name, char *Mesh) {
			int nmsize = strlen(Name) + 1;
			name = _strlwr(strncpy(new char[nmsize], Name, nmsize));
			nmsize = strlen(Mesh) + 1;
			mesh = _strlwr(strncpy(new char[nmsize], Mesh, nmsize));
			size = 0;
		}
	};
	class StaticReference {
	public:
		char *name;
		float x, y, z;
		float yaw, pitch, roll;
		float scale;
		unsigned staticID;
		void SetID(map<string, Static*> *StaticsList, map<string, unsigned> *StaticMap) {
			char *file = (*StaticsList)[name]->mesh;
			if (StaticMap->find(file) != StaticMap->end()) 
				staticID = (*StaticMap)[file];
			else {
				staticID = (unsigned)StaticMap->size();
				(*StaticMap)[file] = staticID;
			}
		}
		void Write(HANDLE hFile) {
			DWORD dwNumberOfBytes;
			WriteFile(hFile, &staticID, sizeof(staticID), &dwNumberOfBytes, NULL);
			WriteFile(hFile, &x, sizeof(x), &dwNumberOfBytes, NULL);
			WriteFile(hFile, &y, sizeof(y), &dwNumberOfBytes, NULL);
			WriteFile(hFile, &z, sizeof(z), &dwNumberOfBytes, NULL);
			WriteFile(hFile, &yaw, sizeof(yaw), &dwNumberOfBytes, NULL);
			WriteFile(hFile, &pitch, sizeof(pitch), &dwNumberOfBytes, NULL);
			WriteFile(hFile, &roll, sizeof(roll), &dwNumberOfBytes, NULL);
			WriteFile(hFile, &scale, sizeof(scale), &dwNumberOfBytes, NULL);
		}
	};
	struct StaticToRemove {
	public:
		string worldspace;
		string reference;
		StaticToRemove(string Worldspace, string Reference) {
			worldspace = Worldspace;
			reference = Reference;
		}
	};
	enum StaticType {
		Auto = 0,
		Near = 1,
		Far = 2,
		VeryFar = 3,
		Grass = 4,
		Tree = 5,
		Building = 6
	};

private:
	class staticOverride {
	public:
		bool Ignore;
		StaticType Type;
		bool OldSimplify;
		bool overrideSimplify;
		float Simplify;
		float Density;
		bool StaticsOnly;
		bool NoScript;
		bool NamesNoIgnore;

		staticOverride(byte value) {
			OldSimplify = false;
			Simplify = 1;
			Density = -1;
			overrideSimplify = false;
			Ignore = false;
			StaticsOnly = false;
			Type = Auto;
			NoScript = false;
			NamesNoIgnore = false;
			switch (value) {
				case 0:
					Ignore = true;
					break;
				case 1:
					Type = Near;
					break;
				case 2:
					Type = Far;
					break;
				case 3:
					break;
				case 4:
					overrideSimplify = true;
					Type = Near;
					break;
				case 5:
					overrideSimplify = true;
					Type = Far;
					break;
				case 6:
					overrideSimplify = true;
					break;
			}
		}
		staticOverride(staticOverride *value, bool enabledInNames) {
			OldSimplify = value->OldSimplify;
			Simplify = value->Simplify;
			Density = value->Density;
			overrideSimplify = value->overrideSimplify;
			Ignore = value->Ignore;
			StaticsOnly = value->StaticsOnly;
			Type = value->Type;
			NoScript = value->NoScript;
			NamesNoIgnore = enabledInNames;
		}

	};
	enum CreateMethods {
		None = 0,
		Textures = 1,
		WorldMesh = 2,
		Statics = 3
	};
	class CreateCullTask : public ThreadTask {
	public: 
		CreateCullTask( ThreadFlag* flag, DistantLandCreator *dlc, CreateMethods method ) : ThreadTask(flag), m_dlc(dlc), m_method(method) {}
		virtual void Run() {
			switch (m_method) {
				case Textures:
					m_dlc->CreateWorldTexture(false);
					m_dlc->CreateWorldTexture(true);
					break;
				case WorldMesh:
					m_dlc->GenerateWorldMesh();
					break;
				case Statics:
					m_dlc->CreateStatics();
					break;
			}
		}
	protected:
		DistantLandCreator *m_dlc;
		CreateMethods m_method;
	};
	class LoadPluginCullTask : public ThreadTask {
	public: 
		LoadPluginCullTask( ThreadFlag* flag, DistantLandCreator *dlc, DWORD section, DWORD size, LPBYTE lpBuffer, LPVOID lpParameter ) :
			ThreadTask(flag), m_dlc(dlc), m_section(section), m_size(size), m_lpBuffer(lpBuffer), m_lpParameter(lpParameter) {}
		virtual void Run() {
			m_dlc->LoadPlugin( m_section, m_size, m_lpBuffer, m_lpParameter );
		}
	protected:
		DistantLandCreator *m_dlc;
		DWORD m_section, m_size;
		LPBYTE m_lpBuffer;
		LPVOID m_lpParameter;
	};

private:
	HMODULE hMGEfuncs;
	LPDIRECT3D9 lpDirect3d;
	CRITICAL_SECTION land_crit_sec, ltex_crit_sec, list_crit_sec;
	MWBridge* mwBridge;

	// Keeps track of map extents in terms of cells
	int MapSize;
	int MapMaxX;
	int MapMaxY;
	int MapMinX;
	int MapMinY;
	int CellCount;
	int VeryHigh;
	int VeryLow;
	static const int LowerBoundX = -250;
	static const int UpperBoundX = 250;
	static const int LowerBoundY = -250;
	static const int UpperBoundY = 250;
	static LAND ***Map;
	LAND defaultland;
	static LTEX *DefaultTex;
	static IDirect3DDevice9* device;
	vector<char*> GameFiles;
	map<string, Static*> StaticsList;
	map<string, map<string, StaticReference*>*> UsedStaticsList;
	map<string, unsigned> StaticMap;
	map<string, bool> DisableScripts;
	map<string, staticOverride*> OverrideList;
	map<string, bool> IgnoreList;
	map<string, bool> CellList;

public:
	DistantLandCreator() {
		hMGEfuncs = LoadLibrary("MGE3\\MGEfuncs.dll");
		lpDirect3d = Direct3DCreate9( D3D_SDK_VERSION );
		mwBridge = MWBridge::Inst();
		device = CreateDevice();
		SetPluginFiles();
		MapSize = 0;
		MapMaxX = INT_MIN;
		MapMaxY = INT_MIN;
		MapMinX = INT_MAX;
		MapMinY = INT_MAX;
		VeryHigh = INT_MIN;
		VeryLow = INT_MAX;
		InitializeCriticalSection(&land_crit_sec);
		InitializeCriticalSection(&ltex_crit_sec);
		InitializeCriticalSection(&list_crit_sec);
		Map = new LAND**[UpperBoundX - LowerBoundX + 1];
		for(int i = 0; i <= UpperBoundX - LowerBoundX; i++) {
			Map[i] = new LAND*[UpperBoundY - LowerBoundY + 1];
			for(int j = 0; j <= UpperBoundY - LowerBoundY; j++) Map[i][j] = NULL;
			Map[i] -= LowerBoundY;
		}
		Map -= LowerBoundX;
	}
	~DistantLandCreator() {
		Map += LowerBoundX;
		for(int i = 0; i <= UpperBoundX - LowerBoundX; i++) {
			Map[i] += LowerBoundY;
			for(int j = 0; j <= UpperBoundY - LowerBoundY; j++) delete Map[i][j];
			delete[] Map[i];
		}
		delete[] Map;
		Map = NULL;
		for(map<string, map<string, StaticReference*>*>::const_iterator i = UsedStaticsList.begin(); i != UsedStaticsList.end(); ++i) {
			for(map<string, StaticReference*>::const_iterator it = i->second->begin(); it != i->second->end(); ++it) delete it->second;
			i->second->clear();
			delete i->second;
		}
		for(map<string, staticOverride*>::const_iterator i = OverrideList.begin(); i != OverrideList.end(); ++i) delete i->second;
		for(map<string, Static*>::const_iterator i = StaticsList.begin(); i != StaticsList.end(); ++i) delete i->second;
		UsedStaticsList.clear();
		OverrideList.clear();
		StaticsList.clear();
		CellList.clear();
		StaticMap.clear();
		IgnoreList.clear();
		DisableScripts.clear();
		DeleteCriticalSection(&land_crit_sec);
		DeleteCriticalSection(&ltex_crit_sec);
		DeleteCriticalSection(&list_crit_sec);
		ClearPluginFiles();
		LTEX::ReleaseCache();
		device->Release();
		lpDirect3d->Release();
		if(hMGEfuncs) FreeLibrary(hMGEfuncs);
	}
	static bool GetDLFilesExist() {
		return GetSize(DL_WORLD_TEXTURE) > 0 && GetSize(DL_WORLD_NORMALS) > 0 && GetSize(DL_WORLD_MESHES) > 0;
	}
	static bool GetDSFilesExist() {
		return GetSize(DL_STATICS_MESHES) > 0 && GetSize(DL_STATICS_USAGE) > 0;
	}
private:
	IDirect3DDevice9* CreateDevice() {
		IDirect3DDevice9* device9;
		D3DDISPLAYMODE displayMode;
/**/	lpDirect3d->GetAdapterDisplayMode(/**/0/**/, &displayMode);	/*	Get Adapter from Morrowind registry */
		D3DPRESENT_PARAMETERS d3dpp;
		memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
		d3dpp.BackBufferCount = 1;
		d3dpp.BackBufferFormat = displayMode.Format;
		d3dpp.BackBufferHeight = 1024;
		d3dpp.BackBufferWidth = 1024;
		d3dpp.EnableAutoDepthStencil = FALSE;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.Windowed = TRUE;
		HWND hWnd = FindWindow("Morrowind", "Morrowind");
		lpDirect3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_FPU_PRESERVE | D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device9 );
		return device9;
	}
	static int GetSize(char *lpFilePath) {
		HANDLE hFile = CreateFile(lpFilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		int size = -1;
		if(hFile != INVALID_HANDLE_VALUE) {
			size = GetFileSize(hFile, NULL);
			CloseHandle(hFile);
		}
		return size;
	}
	static LTEX* GetTex(int cellx, int celly, int texx, int texy) {
		if (Map[cellx][celly] != NULL) return (*Map[cellx][celly]->Textures)[Map[cellx][celly]->Tex[texx][texy]];
		else return DefaultTex;
	}
	int GetWorldMeshDetail() {
		D3DCAPS9 caps;
		// Set max mesh detail here
		int levelsCount = 5;
		if(device->GetDeviceCaps( &caps )) return levelsCount/2;
		DWORD VertsRequired = (MapSize * 2 + 1) * (MapSize * 2 + 1) * 64 * 64 + (MapSize * 2 + 1) * 128 + 1;
		DWORD PrimsRequired = (MapSize * 2 + 1) * (MapSize * 2 + 1) * 64 * 64 * 2;
		int index = 0;
		while ((VertsRequired > caps.MaxVertexIndex || PrimsRequired > caps.MaxPrimitiveCount) && ++index < levelsCount - 1) {
			VertsRequired /= 4;
			PrimsRequired /= 4;
		}
		return index;
	}
	void GrassDensityThreshold (float GrassDensity, map<string, map<string, StaticReference*>*>::const_iterator cellStatics, map<string, StaticReference*>::const_iterator pair, vector<StaticToRemove*> *UsedStaticsToRemove) {
		if (GrassDensity < 1.0f) {
			//remove some of the grass to improve performance
			float NextDouble = float(rand())/RAND_MAX;
			if (NextDouble >= GrassDensity) UsedStaticsToRemove->push_back(new StaticToRemove(cellStatics->first, pair->first));
		} else if (GrassDensity == 0.0f) {
			//Remove all grass
			UsedStaticsToRemove->push_back(new StaticToRemove (cellStatics->first, pair->first));
		}
	}
	void AddGameFile(LPCSTR filename) {
		int length = strlen(filename) + 1;
		for(size_t i = 0; i < GameFiles.size(); i++) if(_stricmp(filename, GameFiles[i]) == 0) { length = 0; break; }
		if (length) {
			LPSTR name = strlwr(strncpy(new char[length], filename, length));
			LPSTR slash = name;
			while(slash = strstr(slash, "/")) *slash = '\\';
			GameFiles.push_back(name);
		}
	}
	std::vector<std::string>* GetCommentedPluginFiles() {
		//GetSectionComments(siniDLWizardPlugins, mgeini);
		std::vector<std::string> *commented = NULL;//new std::vector<std::string>;
		return commented;
	}
	int SetPluginFiles() {
		ClearPluginFiles();
		char *filename = new char[MAX_PATH];
		if (WizardPlugins) {
			for(size_t i = 0; i < ((std::vector<std::string>*)WizardPlugins)->size(); i++)
				AddGameFile((*(std::vector<std::string>*)WizardPlugins)[i].c_str());
		} else {
			char *name = new char[MAX_PATH];
			for(int i=0; true; i++) {
				strcpy_s(filename, MAX_PATH, "GameFile");
				_itoa(i, filename + strlen(filename), 10);
				GetPrivateProfileString("Game Files", filename, NULL, name, MAX_PATH, "mge3\\..\\Morrowind.ini");
				if(strlen(name) > 0) AddGameFile(name); else break;
			}
			delete[] name;
		}
		WIN32_FIND_DATA *fd = new WIN32_FIND_DATA;
		for(int i=0; i<2; i++) {
			strncat(strncpy(filename, DL_STATICS, MAX_PATH), i ? "*.esp" : "*.esm", MAX_PATH);
			HANDLE fff = FindFirstFile(filename, fd);
			if(fff!=INVALID_HANDLE_VALUE) {
				do{ if(!(strcmp(fd->cFileName, ".") && strcmp(fd->cFileName, ".."))) continue;
					AddGameFile(strncat(strncpy(filename, DL_STATICS, MAX_PATH), fd->cFileName, MAX_PATH));
				}while(FindNextFile(fff, fd));
				FindClose(fff);
			}
		}
		delete fd;
		delete[] filename;
		return GameFiles.size();
	}
	void ClearPluginFiles() {
		for(size_t i = 0; i < GameFiles.size(); i++) delete[] GameFiles[i];
		GameFiles.clear();
	}

	void LoadOverrideList(){
	}

	void LoadPlugin(DWORD section, DWORD size, LPBYTE lpBuffer, LPVOID lpParameter) {
		LPBYTE lpByte = lpBuffer;
		LPWORD& lpWord = (LPWORD&)lpByte;
		LPDWORD& lpDword = (LPDWORD&)lpByte;
		if(section == LAND::Section) {
			int m_VeryHigh, m_VeryLow;
			//------------------------------
			EnterCriticalSection(&land_crit_sec);
			m_VeryHigh = VeryHigh;
			m_VeryLow = VeryLow;
			LeaveCriticalSection(&land_crit_sec);
			//------------------------------
			LAND *land = new LAND();
			land->Textures = (vector<LTEX*>*)lpParameter;
			int lx = -999;
			int ly = -999;
			lpDword++;
			lpDword++;
			while (lpByte < lpBuffer + size) {
				DWORD label = *(lpDword++);
				DWORD size2 = *(lpDword++);	
				switch (label) {
					case 0x56544E49: {	// "INTV"
						land->xpos = lx = *(lpDword++);
						land->ypos = ly = *(lpDword++);
						break;
					}
					case 0x54474856: {	// "VHGT"
						int offset = (int)*PFLOAT(lpDword++);
						for (int y = 0; y < 65; y++) {
							offset += (char)*(lpByte++);
							land->Heights[0][y] = offset;
							int pos = offset;
							for (int x = 1; x < 65; x++) {
								pos += (char)*(lpByte++);
								land->Heights[x][y] = pos;
								if (pos > m_VeryHigh) m_VeryHigh = pos;
								if (pos < m_VeryLow) m_VeryLow = pos;
							}
						}
						lpByte += 3 * sizeof(BYTE);
						break;
					}
					case 0x4C4D4E56: {	// "VNML"
						for (int y = 0; y < 65; y++) {
							for (int x = 0; x < 65; x++) {
								char vx = *(lpByte++);
								char vy = *(lpByte++);
								char vz = *(lpByte++);
								land->Normals[x][y] = Normal(vx, vy, vz);
							}
						}
						break;
					}
					case 0x524C4356: {	// "VCLR"
						for (int y = 0; y < 65; y++) {
							for (int x = 0; x < 65; x++) {										
								land->Color[x][y].r = *(lpByte++);
								land->Color[x][y].g = *(lpByte++);
								land->Color[x][y].b = *(lpByte++);
							}
						}
						break;
					}
					case 0x58455456: {	// "VTEX"
						for (int y1 = 0; y1 < 4; y1++) {
							for (int x1 = 0; x1 < 4; x1++) {
								for (int y2 = 0; y2 < 4; y2++) {
									for (int x2 = 0; x2 < 4; x2++) {
										land->Tex[x1 * 4 + x2][y1 * 4 + y2] = *(lpWord++);
									}
								}
							}
						}
						break;
					}
					default: {
						lpByte += size2;
						break;
					}
				}
			}
			//------------------------------
			EnterCriticalSection(&land_crit_sec);
			if (m_VeryHigh > VeryHigh) VeryHigh = m_VeryHigh;							
			if (m_VeryLow < VeryLow) VeryLow = m_VeryLow;
			if (lx != -999 && ly != -999) {
				//Keep track of map extents
				if (lx > MapMaxX) { MapMaxX = lx; }
				if (ly > MapMaxY) { MapMaxY = ly; }
				if (lx < MapMinX) { MapMinX = lx; }
				if (ly < MapMinY) { MapMinY = ly; }
				int Max = max(MapMaxX - MapMinX, MapMaxY - MapMinY);
				if (Max > MapSize) MapSize = Max;
				if (Map[lx][ly] == NULL) CellCount++; else delete Map[lx][ly];
				Map[lx][ly] = land;
			}
			LeaveCriticalSection(&land_crit_sec);
			//------------------------------
		} else if(section == LTEX::Section) {
			//TODO: Fix!
			//Presumably, not all LTEX's have the subrecords in an identical order
			LTEX *tex = new LTEX();
			lpByte += 3 * sizeof(DWORD);
			lpByte += *(lpDword++);
			lpByte += 2 * sizeof(DWORD);
			tex->index = *(lpDword++) + 1;
			lpDword++;
			char *lpTextureName = new CHAR[*(lpDword++)];
			tex->FilePath = _strlwr(strncpy(lpTextureName, (char*)lpByte, lpBuffer + size - lpByte));
			tex->LoadTexture();
			if(!tex->tex) tex->tex = DefaultTex->tex;		
			//------------------------------
			EnterCriticalSection(&ltex_crit_sec);
			((vector<LTEX*>*)lpParameter)->push_back(tex);
			LeaveCriticalSection(&ltex_crit_sec);
			//------------------------------
		} else if(Static::IsStatic(section)) {
			// Parsing for statics definitions
			bool activator = (section == Static::ACTI);
			bool misc = (section == Static::DOOR || section == Static::CONT || section == Static::LIGH);
			char *name = NULL;
			char *model = NULL;
			char *script = NULL;
			while (lpByte < lpBuffer + size) {
				DWORD label = *(lpDword++);
				DWORD size2 = *(lpDword++);
				switch (label) {
					case Static::NAME:
					case Static::MODL:
					case Static::SCRI: {
						char *lpString = _strlwr(strncpy(new char[size2], (char*)lpByte, size2));
						switch (label) {
							case Static::NAME: name = lpString; break;
							case Static::MODL: model = lpString; break;
							case Static::SCRI: script = lpString; break;
						}
						break;
					}
				}
				lpByte += size2;
			}
			if (name && (model ? strlen(model) > strspn(model, " \t\n\r") : false)) {
				//Named exceptions
				if (IgnoreList.find(name) != IgnoreList.end()) {
					if (!IgnoreList[name]) {
						Static *lpStatic = new Static(name, model);
						//------------------------------
						EnterCriticalSection(&list_crit_sec);
						StaticsList[name] = lpStatic;
						if ((OverrideList.find(model) != OverrideList.end()) ? OverrideList[model]->Ignore : false) {
							OverrideList[model] = new staticOverride(OverrideList[model], true);
						}
						LeaveCriticalSection(&list_crit_sec);
						//------------------------------
					}
				} else {
					//NoScript override
					bool Special_exceptions = (_stricmp(name, "chargen boat")==0 || _stricmp(name, "chargen_plank")==0); // Special exceptions
					bool Special_model = (_stricmp(model, "f\\active_blight_large.nif")==0); // Special model exceptions
					//------------------------------
					EnterCriticalSection(&list_crit_sec);
					bool OverrideListContains = (OverrideList.find(model) != OverrideList.end());
					if (script && (OverrideListContains ? OverrideList[model]->NoScript : false)) {
						delete[] script;
						script = NULL;
					}
					if(!(Special_exceptions || (Special_model ? (!OverrideListContains) : false))) {
						if ( ((misc && !MiscObj) || activator && !Activators) ? 
							(OverrideListContains ? !OverrideList[model]->Ignore : false) :
							(OverrideList.empty() || !OverrideListContains || !OverrideList[model]->Ignore) ) {
							if (!(script && ((DisableScripts.find(script) != DisableScripts.end()) ? DisableScripts[script] : false))) {
								StaticsList[name] = new Static(name, model);
							}
						}
					}
					LeaveCriticalSection(&list_crit_sec);
					//------------------------------
				}
			}
			delete name;
			delete model;
			delete script;
		} else if(section == TES3::Section) {
			lpDword++;
			lpDword++;
			while (lpByte < lpBuffer + size) {
				DWORD label = *(lpDword++);
				DWORD size2 = *(lpDword++);	
				switch (label) {
					case TES3::MAST: {
						char *lpString = _strlwr(strncpy(new char[size2], (char*)lpByte, size2));
						((vector<string>*)lpParameter)->push_back(lpString);
						delete[] lpString;
						break;
					}
					default: {
						lpByte += size2;
						break;
					}
				}
			}
		} else if(section == CELL::Section) {
			// Parsing for used statics
			char CellName[MAX_PATH] = "";
			bool IsValidCell = false;
			bool InReference = false;
			bool ReferenceHasScale = false;
			bool ReferenceDeleted = false;
			bool ReadData = false;
			bool Interior = false;
			DWORD RefID = 0;
			int MastID = 0;
			int CellX = 0;
			int CellY = 0;
			StaticReference *sr = new StaticReference();
			while (lpByte < lpBuffer + size) {
				DWORD label = *(lpDword++);
				DWORD size2 = *(lpDword++);
				LPBYTE lpPos = lpByte;
				switch (label) {
					case CELL::DATA: // "DATA"
						if (!InReference) {
							if (ReadData) break;
							DWORD flags = *(lpDword++);

							if ((flags & 0x01) == 0 || (StatIntExt && (flags & 0x81) == 0x81) || (StatIntWater && (flags & 0x83) == 0x03)) IsValidCell = true;
							if ((flags & 0x01) != 0) {
								Interior = true;
								if (CellList.find(CellName) != CellList.end()) IsValidCell = CellList[CellName];
							}
							else Interior = false;
							CellX = *(lpDword++);
							CellY = *(lpDword++);
							ReadData = true;
						} else {
							sr->x = *PFLOAT(lpDword++);
							sr->y = *PFLOAT(lpDword++);
							sr->z = *PFLOAT(lpDword++);
							sr->yaw = *PFLOAT(lpDword++);
							sr->pitch = *PFLOAT(lpDword++);
							sr->roll = *PFLOAT(lpDword++);
						}
						break;
					case CELL::FRMR: // "FRMR"
						if (!IsValidCell) break;
						if (InReference) {
							if (!ReferenceHasScale) sr->scale = 1;
							if (StaticsList.find(sr->name) != StaticsList.end() && (IgnoreList.find(sr->name) == IgnoreList.end() ? true : !IgnoreList[sr->name])) {
								string Worldspace = Interior ? CellName : "";
								string Reference = (*((vector<string>*)lpParameter))[MastID];
								Reference += "\001";
								char value[32] = "";
								if(!Interior) {
									Reference += _itoa(CellX, value, 10);
									Reference += "\002";
									Reference += _itoa(CellY, value, 10);
									Reference += "\001";
								}
								Reference += _itoa(RefID, value, 10);
								//------------------------------
								EnterCriticalSection(&list_crit_sec);
								sr->SetID(&StaticsList, &StaticMap);
								if (ReferenceDeleted) {
									if (UsedStaticsList.find(Worldspace) != UsedStaticsList.end() ? UsedStaticsList[Worldspace]->find(Reference) != UsedStaticsList[Worldspace]->end() : false)
										UsedStaticsList[Worldspace]->erase(Reference);
								} else {
									if (UsedStaticsList.find(Worldspace) == UsedStaticsList.end()) UsedStaticsList[Worldspace] = new map<string, StaticReference*>;
									(*UsedStaticsList[Worldspace])[Reference] = sr;
								}
								LeaveCriticalSection(&list_crit_sec);
								//------------------------------
							}
						}
						RefID = *(lpDword++);
						MastID = (int)(RefID >> 24);
						RefID &= 0x00FFFFFF;
						sr = new StaticReference();
						InReference = true;
						ReferenceHasScale = false;
						ReferenceDeleted = false;
						break;
					case CELL::DELE:	// "DELE"
						ReferenceDeleted = true;
						break;
					case CELL::NAME:	// "NAME"
						if (InReference) sr->name = _strlwr(strncpy(new char[size2], (char*)lpByte, size2));
						else _strlwr(strncpy(CellName, (char*)lpByte, size2));
						break;
					case CELL::XSCL:	// "XSCL"
						if (!InReference) break;
						sr->scale = *PFLOAT(lpDword++);
						ReferenceHasScale = true;
						break;
				}
				lpByte = lpPos + size2;
			}
			if (InReference) {
				if (!ReferenceHasScale) sr->scale = 1;
				if (StaticsList.find(sr->name) != StaticsList.end() && (IgnoreList.find(sr->name) == IgnoreList.end() ? true : !IgnoreList[sr->name])) {
					string Worldspace = Interior ? CellName : "";
					string Reference = (*((vector<string>*)lpParameter))[MastID];
					Reference += "\001";
					char value[32] = "";
					if(!Interior) {
						Reference += _itoa(CellX, value, 10);
						Reference += "\002";
						Reference += _itoa(CellY, value, 10);
						Reference += "\001";
					}
					Reference += _itoa(RefID, value, 10);
					//------------------------------
					EnterCriticalSection(&list_crit_sec);
					sr->SetID(&StaticsList, &StaticMap);
					if (ReferenceDeleted) {
						if (UsedStaticsList.find(Worldspace) != UsedStaticsList.end() ? UsedStaticsList[Worldspace]->find(Reference) != UsedStaticsList[Worldspace]->end() : false)
							UsedStaticsList[Worldspace]->erase(Reference);
					} else {
						if (UsedStaticsList.find(Worldspace) == UsedStaticsList.end()) UsedStaticsList[Worldspace] = new map<string, StaticReference*>;
						(*UsedStaticsList[Worldspace])[Reference] = sr;
					}
					LeaveCriticalSection(&list_crit_sec);
					//------------------------------
				}
			}
		}
		delete[] lpBuffer;
	}

public:
	bool LoadPlugins() {
		// Load default ground texture
		DefaultTex = new LTEX("_land_default.tga");
		CellCount = 0;
		StaticsList.clear();
		UsedStaticsList.clear();
		StaticMap.clear();
		LoadOverrideList();
		map<string, vector<ThreadFlag*>*> cell_flags;
		map<string, vector<LoadPluginCullTask*>*> cell_tasks;
		vector<ThreadFlag*> *flags_cache;
		vector<LoadPluginCullTask*> *tasks_cache;
		map<string, vector<string>*> Masters_cache;
		mwBridge->ShowProgressBar(++DLProgressBar);
		float FilesCount = 0;
		for(size_t i = 0; i < GameFiles.size(); i++) {
			char *name = new char[MAX_PATH];
			strcpy_s(name, MAX_PATH, strstr(GameFiles[i], "\\") ? "" : DATA_FILES);
			strcat_s(name, MAX_PATH, GameFiles[i]);
			HANDLE hTesFile = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
			delete[] name;
			if(hTesFile == INVALID_HANDLE_VALUE) continue;
			DWORD label, dwBytesRead, size1, dwSize = GetFileSize(hTesFile, NULL);
			flags_cache = new vector<ThreadFlag*>;
			tasks_cache = new vector<LoadPluginCullTask*>;
			vector<LTEX*> *Textures = new vector<LTEX*>;
			vector<string> *Masters = new vector<string>;
			//Add the default texture first, because all vtex id's are +1 compared to the ltex ids
			Textures->push_back(DefaultTex);
			Masters->push_back(GameFiles[i]);
			vector<ThreadFlag*> flags;
			while (DWORD(SetFilePointer(hTesFile, NULL, NULL, FILE_CURRENT)) < dwSize) {
				if(ReadFile(hTesFile, &label, sizeof(DWORD), &dwBytesRead, NULL) ? sizeof(DWORD) != dwBytesRead : true) break;
				if(ReadFile(hTesFile, &size1, sizeof(DWORD), &dwBytesRead, NULL) ? sizeof(DWORD) != dwBytesRead || !size1 || size1 > dwSize : true) break;
				size1 += 2 * sizeof(DWORD);
				if(label == LAND::Section || label == LTEX::Section || Static::IsStatic(label) || label == TES3::Section || label == CELL::Section) {
					LPBYTE lpBuffer = new BYTE[size1];
					if(ReadFile(hTesFile, lpBuffer, size1, &dwBytesRead, NULL) ? size1 != dwBytesRead : true) {delete[] lpBuffer; break; }
					ThreadFlag *flag = new ThreadFlag();
					LoadPluginCullTask *ctLoadPlugin = new LoadPluginCullTask( flag, this, label, size1, lpBuffer,
						(label == TES3::Section || label == CELL::Section) ? LPVOID(Masters) : LPVOID(Textures));
					if (label == CELL::Section) {
						flags_cache->push_back(flag);
						tasks_cache->push_back(ctLoadPlugin);
					} else {
						flags.push_back(flag);
						Scheduler.ScheduleTask( ctLoadPlugin );
					}
				} else {
					if(SetFilePointer(hTesFile, size1, NULL, FILE_CURRENT) == -1) break;
				}
			}
			CloseHandle(hTesFile);
			Scheduler.DoTasksInCurrentThread();
			for(size_t j = 0; j < flags.size(); j++) { flags[j]->Wait(); delete flags[j]; }
			flags.clear();
			std::sort(Textures->begin(), Textures->end(), LTEX::SortPredicate);
			cell_flags[GameFiles[i]] = flags_cache;
			cell_tasks[GameFiles[i]] = tasks_cache;
			Masters_cache[GameFiles[i]] = Masters;
			if(FilesCount <= i) {
				FilesCount += float(GameFiles.size())/5;	// 5 percent at all
				mwBridge->ShowProgressBar(++DLProgressBar);
			}
		}
		UsedStaticsList[""] = new map<string, StaticReference*>;
		FilesCount = 0;
		// Parsing plugins for used statics
		for(size_t i = 0; i < GameFiles.size(); i++) {
			if(cell_flags.find(GameFiles[i]) == cell_flags.end() || cell_tasks.find(GameFiles[i]) == cell_tasks.end()) continue;
			tasks_cache = cell_tasks[GameFiles[i]];
			size_t cache_size = tasks_cache->size();
			for(size_t j = 0; j < cache_size; j++) { Scheduler.ScheduleTask( (*tasks_cache)[j]); }
			Scheduler.DoTasksInCurrentThread();
			flags_cache = cell_flags[GameFiles[i]];
			cache_size = flags_cache->size();
			for(size_t j = 0; j < cache_size; j++) { (*flags_cache)[j]->Wait(); delete (*flags_cache)[j]; }
			flags_cache->clear();
			tasks_cache->clear();
			Masters_cache[GameFiles[i]]->clear();
			delete flags_cache;
			delete tasks_cache;
			delete Masters_cache[GameFiles[i]];
			if(FilesCount <= i) {
				FilesCount += float(GameFiles.size())/5;	// 5 percent at all
				mwBridge->ShowProgressBar(++DLProgressBar);
			}
		}
		cell_flags.clear();
		cell_tasks.clear();
		Masters_cache.clear();
		mwBridge->ShowProgressBar(++DLProgressBar);
		return CellCount > 0;
	}

	bool CreateWorldTexture(bool normal) {
		CellTexCreator *ctc = new CellTexCreator(4);
        //Render world texture
		WorldTexCreator *wtc = new WorldTexCreator(128 << (normal ? NormRes : TexRes), MapMinX, MapMaxX, MapMinY, MapMaxY);
        wtc->Begin();
		if(normal) ctc->BeginNormalMap(); else ctc->Begin();
        for (int y = MapMinY; y <= MapMaxY; y++) {
            for (int x = MapMinX; x <= MapMaxX; x++) {
				if (Map[x][y] == NULL || Map[x][y] == &defaultland) ctc->SetDefaultCell(DefaultTex);
                //Set the colors and normals
				else ctc->SetCell(Map[x][y]);
                float x_pos = (((float)(x - MapMinX) + 0.5f) * wtc->x_spacing) - 1.0f;
                float y_pos = (((float)(y - MapMinY) + 0.5f) * wtc->y_spacing) - 1.0f;
				if(normal) ctc->RenderNormalMap(x_pos, y_pos, wtc->x_scale, wtc->y_scale);
				else ctc->Render(x_pos, y_pos, wtc->x_scale, wtc->y_scale);
            }
        }
		if(normal) ctc->EndNormalMap(); else ctc->End();
		//------------------------------
		EnterCriticalSection(&list_crit_sec);
		mwBridge->ShowProgressBar(DLProgressBar+=10);
		LeaveCriticalSection(&list_crit_sec);
		//------------------------------
        //Save texture.
		if(normal) wtc->FinishUncompressed(DL_WORLD_NORMALS); else wtc->FinishCompressed(DL_WORLD_TEXTURE, true);
        wtc->Dispose();
        ctc->Dispose();
		delete wtc;
		delete ctc;
		//------------------------------
		EnterCriticalSection(&list_crit_sec);
		mwBridge->ShowProgressBar(DLProgressBar+=4);
		LeaveCriticalSection(&list_crit_sec);
		//------------------------------
		return false;
	}

	bool GenerateWorldMesh() {
		int detail = (MeshDet == -1 ? GetWorldMeshDetail() : MeshDet);
		if(hMGEfuncs == NULL) return false;
		TESSELLATELANDSCAPE TessellateLandscape = (TESSELLATELANDSCAPE)GetProcAddress(hMGEfuncs, "TessellateLandscape");
		if(TessellateLandscape) {
			int vCount = 64 / (1 << detail);
			float vSpacing = 128.0f * (float)(1 << detail);
			int VertsAcross = (MapMaxX - MapMinX + 1) * vCount;
			int VertsDown = (MapMaxY - MapMinY + 1) * vCount;
			int tFaces = (VertsAcross - 1) * (VertsDown - 1) * 2;
			int tVerts = VertsAcross * VertsDown;
			float texMultX = 1 / (float)VertsAcross;
			float texMultY = 1 / (float)VertsDown;
			//Generate optimized landscape mesh
			int MapSpanX = MapMaxX - MapMinX + 1;
			int MapSpanY = MapMaxY - MapMinY + 1;
			int MapOffsetX = 0 - MapMinX;
			int MapOffsetY = 0 - MapMinY;
			int DataSpanX = MapSpanX * 64;
			int DataSpanY = MapSpanY * 64;
			float *height_data = new float[DataSpanX * DataSpanY];
			for (int y1 = MapMinY; y1 <= MapMaxY; y1++) {
				for (int y2 = 0; y2 < 64; y2++) {
					for (int x1 = MapMinX; x1 <= MapMaxX; x1++) {
						for (int x2 = 0; x2 < 64; x2++) {
							int y = (y1 + MapOffsetY) * 64 + y2;
							int x = (x1 + MapOffsetX) * 64 + x2;
							if (Map[x1][y1] != NULL && x1 > LowerBoundX && x1 < UpperBoundX && y1 > LowerBoundY && y1 < UpperBoundY) {
								height_data[y * DataSpanX + x] = (float)Map[x1][y1]->Heights[x2][y2] * 8.0f;
							} else height_data[y * DataSpanX + x] = -2048.0f;
						}
					}
				}
			}
			float left = ((float)MapMinX * 64) * 128.0f;
			float right = ((float)(MapMaxX * 64 + 64)) * 128.0f;
			float bottom = ((float)MapMinY * 64) * 128.0f;
			float top = ((float)(MapMaxY * 64 + 64)) * 128.0f;
			// Tesselating World Mesh
			float et = 125.0f;
			switch (detail) {
				case 0: et = 15.0f; break;
				case 1: et = 70.0f; break;
				case 2:	et = 125.0f; break;
				case 3: et = 180.0f; break;
				case 4: et = 235.0f; break;
			}
			//------------------------------
			EnterCriticalSection(&list_crit_sec);
			mwBridge->ShowProgressBar(DLProgressBar+=3);
			LeaveCriticalSection(&list_crit_sec);
			//------------------------------
			TessellateLandscape(DL_WORLD_MESHES, height_data, (unsigned)DataSpanY, (unsigned)DataSpanX, top, left, bottom, right, et);
			delete[] height_data;
			//------------------------------
			EnterCriticalSection(&list_crit_sec);
			mwBridge->ShowProgressBar(DLProgressBar+=24);
			LeaveCriticalSection(&list_crit_sec);
			//------------------------------
		}
		return true;
	}

	void CreateStatics() {
		float simplify = 1.0f;
		switch (StatMesh) {
			case 1: simplify = 0.9f; break;
			case 2: simplify = 0.75f; break;
			case 3: simplify = 0.5f; break;
			case 4: simplify = 0.25f; break;
			default: simplify = 1.0f; break;
		}
		if(hMGEfuncs == NULL) return;
		PROCESSNIF ProcessNif = (PROCESSNIF)GetProcAddress(hMGEfuncs, "ProcessNif");
		BEGINSTATICCREATION BeginStaticCreation = (BEGINSTATICCREATION)GetProcAddress(hMGEfuncs, "BeginStaticCreation");
		ENDSTATICCREATION EndStaticCreation = (ENDSTATICCREATION)GetProcAddress(hMGEfuncs, "EndStaticCreation");
		if(ProcessNif == NULL || BeginStaticCreation == NULL || EndStaticCreation == NULL) return;
		CreateDirectory(DL_STATICS, NULL);
		// Generate a list of the NIF files we need to load
		vector<string> UsedNifList;
		for(map<string, map<string, StaticReference*>*>::const_iterator cellStatics = UsedStaticsList.begin(); cellStatics != UsedStaticsList.end(); ++cellStatics) {
			for(map<string, StaticReference*>::const_iterator pair = cellStatics->second->begin(); pair != cellStatics->second->end(); ++pair) {
				string nif_name = StaticsList[pair->second->name]->mesh;
				if (find(UsedNifList.begin(), UsedNifList.end(), nif_name) == UsedNifList.end()) UsedNifList.push_back(nif_name);
			}
		}
		//------------------------------
		EnterCriticalSection(&list_crit_sec);
		mwBridge->ShowProgressBar(++DLProgressBar);
		LeaveCriticalSection(&list_crit_sec);
		//------------------------------
		// Processing NIF files
		BeginStaticCreation(device, DL_STATICS_MESHES);
		srand(GetTickCount());
		float StaticCount = 0;
		int StaticCounter = 0;
		int UsedNifListSize = UsedNifList.size();
		// Try to load the NIFs and remove any from the list that fail or are too small
		for (size_t i = 0; i < UsedNifList.size(); i++) {
			string name = UsedNifList[i];
			RetData data(0, 0);
			//Try to load a version of the file that ends with '_dist' first.
			// This allows people to supply a different version of the NIF to be
			// rendered by distant land.  For example, a low poly nif.
			string file_name = name;
			string extention = "";
			int dot_pos = file_name.find_last_of(".");
			if (dot_pos != -1) {
				extention = file_name.substr(dot_pos, file_name.length() - dot_pos);
				file_name = file_name.substr(0, dot_pos);
			}
			string dist_name = file_name;
			dist_name += "_dist";
			dist_name += extention;
			data = GetNif (dist_name.c_str());
			if (data.size == 0) {
				// We didn't find a NIF file with '_dist' in its name,
				// so search for the normal NIF file now.
				data = GetNif (name.c_str());
			}
			if (data.size == 0) {
				UsedNifList.erase(UsedNifList.begin() + i--);
			} else {
				float size = -1;
				if (OverrideList.find(name) != OverrideList.end()) {
					staticOverride *so = OverrideList[name];
					if (!so->Ignore || so->NamesNoIgnore) size = ProcessNif ((char*)data.ptr, data.size, so->overrideSimplify ? so->Simplify : simplify, MinStat, (byte)so->Type, (so->OldSimplify ? (byte)1 : (byte)0));
				} else {
					//Check if this is a grass file based on the file name
					if (name.compare(0, strlen("grass\\"), "grass\\") == 0) size = ProcessNif ((char*)data.ptr, data.size, simplify, MinStat, (byte)Grass, 0);
					else if (name.compare(0, strlen("trees\\"), "trees\\") == 0) size = ProcessNif ((char*)data.ptr, data.size, simplify, MinStat, (byte)Tree, 0);
					else if (name.compare(0, strlen("x\\"), "x\\") == 0) size = ProcessNif ((char*)data.ptr, data.size, simplify, MinStat, (byte)Building, 0);
					else size = ProcessNif ((char*)data.ptr, data.size, simplify, MinStat, (byte)Auto, 0);
				}
				if (size < 0) UsedNifList.erase (UsedNifList.begin() + i--);
			}
			if(StaticCount <= StaticCounter++) {
				StaticCount += float(UsedNifListSize)/23;	// 23 percent at all
				//------------------------------
				EnterCriticalSection(&list_crit_sec);
				mwBridge->ShowProgressBar(++DLProgressBar);
				LeaveCriticalSection(&list_crit_sec);
				//------------------------------
			}
		}
		EndStaticCreation();
		
		// Reset used distant static ID numbers to match NIF list order
		map<string, unsigned> NifMap;
		unsigned count = 0;
		for(size_t i = 0; i < UsedNifList.size(); i++) {
			NifMap[UsedNifList[i]] = count++;
		}
		//------------------------------
		EnterCriticalSection(&list_crit_sec);
		mwBridge->ShowProgressBar(++DLProgressBar);
		LeaveCriticalSection(&list_crit_sec);
		//------------------------------
		vector<StaticToRemove*> UsedStaticsToRemove;
		// Determine floating point grass density
		float GrassDensity = (float)GrassDens / 100.0f;
		for(map<string, map<string, StaticReference*>*>::const_iterator cellStatics = UsedStaticsList.begin(); cellStatics != UsedStaticsList.end(); ++cellStatics) {
			for(map<string, StaticReference*>::const_iterator pair = cellStatics->second->begin(); pair != cellStatics->second->end(); ++pair) {
				string nif_name = StaticsList[pair->second->name]->mesh;
				if (NifMap.find(nif_name) != NifMap.end()) pair->second->staticID = NifMap[nif_name];
				else UsedStaticsToRemove.push_back(new StaticToRemove(cellStatics->first, pair->first));
				if (nif_name.compare(0, strlen("grass\\"), "grass\\")==0 || (OverrideList.find(nif_name) != OverrideList.end() ? OverrideList[nif_name]->Type == Grass : false)) {
					if (OverrideList.find(nif_name) != OverrideList.end()) {
						if (OverrideList[nif_name]->Density >= 0) GrassDensityThreshold (OverrideList[nif_name]->Density, cellStatics, pair, &UsedStaticsToRemove);
						else GrassDensityThreshold (GrassDensity, cellStatics, pair, &UsedStaticsToRemove);
					} else GrassDensityThreshold (GrassDensity, cellStatics, pair, &UsedStaticsToRemove);
				}
			}
		}
		//------------------------------
		EnterCriticalSection(&list_crit_sec);
		mwBridge->ShowProgressBar(++DLProgressBar);
		LeaveCriticalSection(&list_crit_sec);
		//------------------------------
		for(size_t i = 0; i < UsedStaticsToRemove.size(); i++) {
			UsedStaticsList[UsedStaticsToRemove[i]->worldspace]->erase(UsedStaticsToRemove[i]->reference);
		}
		// Writing out placement info
		HANDLE hFile = CreateFile(DL_STATICS_USAGE, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if(hFile == INVALID_HANDLE_VALUE) return;			
		DWORD dwNumberOfBytes;
		DWORD dwCount = UsedNifList.size();
		WriteFile(hFile, &dwCount, sizeof(DWORD), &dwNumberOfBytes, NULL);
		dwCount = UsedStaticsList[""]->size();
		WriteFile(hFile, &dwCount, sizeof(DWORD), &dwNumberOfBytes, NULL);
		for(map<string, StaticReference*>::const_iterator pair = UsedStaticsList[""]->begin(); pair != UsedStaticsList[""]->end(); ++pair) {
			pair->second->Write(hFile);
		}
		//------------------------------
		EnterCriticalSection(&list_crit_sec);
		mwBridge->ShowProgressBar(++DLProgressBar);
		LeaveCriticalSection(&list_crit_sec);
		//------------------------------
		UsedStaticsList[""]->clear();
		UsedStaticsList.erase(UsedStaticsList.find(""));
		char *cellName = new char[64];
		for(map<string, map<string, StaticReference*>*>::const_iterator cellStatics = UsedStaticsList.begin(); cellStatics != UsedStaticsList.end(); ++cellStatics) {
			dwCount = cellStatics->second->size();
			WriteFile(hFile, &dwCount, sizeof(DWORD), &dwNumberOfBytes, NULL);
			memset(cellName, 0, 64);
			strncpy(cellName, cellStatics->first.c_str(), 64);
			WriteFile(hFile, &cellName, 64, &dwNumberOfBytes, NULL);
			for(map<string, StaticReference*>::const_iterator pair = cellStatics->second->begin(); pair != cellStatics->second->end(); ++pair) {
				pair->second->Write(hFile);
			}
		}
		delete[] cellName;
		dwCount = 0;
		WriteFile(hFile, &dwCount, sizeof(DWORD), &dwNumberOfBytes, NULL);
		FLOAT fValue = MinStat;
		WriteFile(hFile, &fValue, sizeof(FLOAT), &dwNumberOfBytes, NULL);
		CloseHandle(hFile);
		//------------------------------
		EnterCriticalSection(&list_crit_sec);
		mwBridge->ShowProgressBar(++DLProgressBar);
		LeaveCriticalSection(&list_crit_sec);
		//------------------------------
    }

	bool CreateVersionFile(){
		bool created = false;
		HANDLE hFile = CreateFile(DL_VERSION, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
		if(hFile != INVALID_HANDLE_VALUE) {
			DWORD dwNumberOfBytes;
			BYTE version = MGE_DL_VERSION;
			created = WriteFile(hFile, &version, sizeof(BYTE), &dwNumberOfBytes, NULL) ? dwNumberOfBytes == sizeof(BYTE) : false;
			CloseHandle(hFile);
			mwBridge->ShowProgressBar(++DLProgressBar);
		}
		return created;
	}

	bool CreateDistantLand(bool CreateOnlyDS) {
		ThreadFlag *flag;
		vector<ThreadFlag*> flags;
		CreateDirectory("Data files\\DistantLand", NULL);
		// CreateStatics
		flags.push_back(flag = new ThreadFlag());
		Scheduler.ScheduleTask( new CreateCullTask( flag, this, Statics ) );
		if (!CreateOnlyDS) {
			// CreateTextures
			flags.push_back(flag = new ThreadFlag());
			Scheduler.ScheduleTask( new CreateCullTask( flag, this, Textures ) );
			// GenerateWorldMesh
			flags.push_back(flag = new ThreadFlag());
			Scheduler.ScheduleTask( new CreateCullTask( flag, this, WorldMesh ) );
		} else mwBridge->ShowProgressBar(DLProgressBar+=55);
		// Try DoTasks
		Scheduler.DoTasksInCurrentThread();
		// Wait for tasks to complete
		for(size_t j = 0; j < flags.size(); j++) { flags[j]->Wait(10); delete flags[j]; }
		flags.clear();
		if (!GetDSFilesExist() != !(MGEFlags & USE_DISTANT_STATICS)) {
			MGEFlags ^= USE_DISTANT_STATICS;
			WritePrivateProfileString("Distant Land", "Distant Statics", MGEFlags & USE_DISTANT_STATICS ? "On" : "Off", "MGE3/MGE.ini");
			mwBridge->ShowProgressBar(++DLProgressBar);
		}
		return CreateOnlyDS ? true : GetDLFilesExist() ? CreateVersionFile() : false;
	}
};

IDirect3DDevice9* DistantLandCreator::device = NULL;
DistantLandCreator::LAND ***DistantLandCreator::Map = NULL;
DistantLandCreator::LTEX *DistantLandCreator::DefaultTex = NULL;

class Landscape {
public:
    IDirect3DTexture* tex;
	IDirect3DTexture* normal_tex;
	IDirect3DTexture* detail_tex;
    D3DXMATRIX world;
	MeshCollection collection;
	QuadTree *quadtree;
	VisibleSet m_visible_set;

	class LandMesh {
	public:
		BoundingSphere sphere;
		BoundingBox box;
		DWORD verts;
		DWORD faces;
		IDirect3DVertexBuffer9* vBuffer;
		IDirect3DIndexBuffer9* iBuffer;

		LandMesh() {
			verts = 0;
			faces = 0;
			vBuffer = 0;
			iBuffer = 0;
		}

		~LandMesh() {
			if (vBuffer) {
				vBuffer->Release();
				vBuffer = 0;
			}
			if (iBuffer) {
				iBuffer->Release();
				iBuffer = 0;
			}
		}

		HRESULT Load( HANDLE & file ) {
			DWORD unused;

			ReadFile(file,&sphere.radius,4,&unused,0);
			ReadFile(file,&sphere.center,12,&unused,0);

			D3DXVECTOR3 min, max;
			ReadFile(file,&min,12,&unused,0);
			ReadFile(file,&max,12,&unused,0);
			box.Set( min, max );

			ReadFile(file,&verts,4,&unused,0);
			ReadFile(file,&faces,4,&unused,0);

			byte* data;
			bool large;
			if(verts>0xFFFF||faces>0xFFFF) large=true; else large=false;
			RealDevice->CreateVertexBuffer(verts*SIZEOFVERT,D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&vBuffer, 0);
			vBuffer->Lock(0,0,(void**)&data,0);
			ReadFile(file,data,verts*SIZEOFVERT,&unused,0);
			vBuffer->Unlock();
			RealDevice->CreateIndexBuffer(faces*(large?4:2)*3,D3DUSAGE_WRITEONLY,large?D3DFMT_INDEX32:D3DFMT_INDEX16,D3DPOOL_DEFAULT,&iBuffer, 0);
			iBuffer->Lock(0,0,(void**)&data,0);
			ReadFile(file,data,faces*(large?4:2)*3,&unused,0);
			iBuffer->Unlock();

			return S_OK;
		}
	};

	vector<LandMesh> meshes;

	IDirect3DVertexBuffer* vb;

	Landscape() {
		tex = 0;
		quadtree = 0;
		normal_tex = 0;
		detail_tex = 0;
		D3DXMatrixIdentity(&world);
	}

	void Unload() {
		meshes.resize(0);

		collection.RemoveAll();

		if(tex) {
			tex->Release();
			tex = 0;
		}
		if(normal_tex) {
			normal_tex->Release();
			normal_tex = 0;
		}
		if(detail_tex) {
			detail_tex->Release();
			detail_tex = 0;
		}

		quadtree->Clear();
	}


	~Landscape() {
		Unload();
	}

	HRESULT Load() {
		DECLARE_MWBRIDGE
		mwBridge->ShowProgressBar(++DLProgressBar);
		//mendres: We have to clear quadtree during reinitialization, otherwise quadtree.CalcVolume() will crash the game.
		quadtree->Clear();
		mwBridge->ShowProgressBar(++DLProgressBar);
		//Load Textures
		if(FAILED(D3DXCreateTextureFromFileEx(RealDevice, DL_WORLD_TEXTURE, 0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
			D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&tex))) {
			ERRORMSG("Could not load world texture for distant land");
			return E_FAIL;
		}
		mwBridge->ShowProgressBar(++DLProgressBar);
		if(FAILED(D3DXCreateTextureFromFileEx(RealDevice, DL_WORLD_NORMALS, 0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
			D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&normal_tex))) {
			ERRORMSG("Could not load world normal map texture for distant land");
			return E_FAIL;
		}
		mwBridge->ShowProgressBar(++DLProgressBar);
		if(FAILED(D3DXCreateTextureFromFileEx(RealDevice, "Data files\\Textures\\MGE\\Default\\world_detail.dds",0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
			D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&detail_tex))) {
			ERRORMSG("Could not load world detail texture for distant land");
			return E_FAIL;
		}

		//Load Meshes and calculate max size of quadtree
		HANDLE file=CreateFile(DL_WORLD_MESHES ,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
		if(file==INVALID_HANDLE_VALUE) return E_FAIL;
		DWORD mesh_count, unused;

		ReadFile(file,&mesh_count,4,&unused,0);

		meshes.resize(mesh_count);

		D3DXVECTOR2 max = D3DXVECTOR2(FLT_MIN, FLT_MIN);
		D3DXVECTOR2 min = D3DXVECTOR2(FLT_MAX, FLT_MAX);

		size_t meshes_size = meshes.size();
		if ( meshes_size > 0 ) {
			Landscape::LandMesh* meshes_ptr = &(meshes[0]);
			for ( DWORD i = 0; i < meshes_size; ++i ) {
				if ( meshes_ptr[i].Load(file) != S_OK ) {
					return E_FAIL;
				}


				float x_max = meshes[i].sphere.center.x + meshes[i].sphere.radius;
				float x_min = meshes[i].sphere.center.x - meshes[i].sphere.radius;
				float y_max = meshes[i].sphere.center.y + meshes[i].sphere.radius;
				float y_min = meshes[i].sphere.center.y - meshes[i].sphere.radius;


				if ( x_max > max.x ) { max.x = x_max; }
				if ( y_max > max.y ) { max.y = y_max; }

				if ( x_min < min.x ) { min.x = x_min; }
				if ( y_min < min.y ) { min.y = y_min; }
			}

			mwBridge->ShowProgressBar(++DLProgressBar);
			if ( max.x - min.x > max.y - min.y ) {
				quadtree->SetBoxSize( max.x - min.x );
			} else {
				quadtree->SetBoxSize( max.y - min.y );
			}

			quadtree->SetBoxCenter( (max + min) / 2 );

			//Add mesh to the quadtree
			for ( size_t i = 0; i < meshes_size; ++i ) {
				MeshCollection::VisFlags vf = collection.Add(
					tex,
					meshes_ptr[i].verts,
					meshes_ptr[i].vBuffer,
					meshes_ptr[i].faces,
					meshes_ptr[i].iBuffer,
					world
				);

				quadtree->AddMesh(
					meshes_ptr[i].sphere,
					meshes_ptr[i].box,
					world,
					tex,
					meshes_ptr[i].verts,
					meshes_ptr[i].vBuffer,
					meshes_ptr[i].faces,
					meshes_ptr[i].iBuffer
				);
			}
		}

		CloseHandle(file);

		quadtree->CalcVolume();
		mwBridge->ShowProgressBar(++DLProgressBar);
		return S_OK;
	}

	void Draw( D3DXMATRIX* view, D3DXMATRIX* proj ) {
		D3DXMATRIX viewproj = (*view) * (*proj);

		//Set vertex declaration
		RealDevice->SetVertexDeclaration( LandDecl );

		//Set shader constants
		DL_Effect->SetMatrix(ehWorld,&world);
		DL_Effect->SetMatrix(ehView,view);
		DL_Effect->SetMatrix(ehProj,proj);

		DL_Effect->SetTexture(ehTex1, normal_tex); // Normal Map
		DL_Effect->SetTexture(ehTex2, detail_tex); // Detail texture
		DL_Effect->SetTexture(ehTex3, tex); // Base texture
		DL_Effect->CommitChanges();

		ViewFrustum frustum(&viewproj);

		m_visible_set.RemoveAll();
		quadtree->GetVisibleMeshes( frustum, m_visible_set );

		m_visible_set.Render( RealDevice, 0, 0, 0, SIZEOFVERT );
	}

	void DrawDepth( D3DXMATRIX* view, D3DXMATRIX* proj, DepthManager* manager ) {
		D3DXMATRIX viewproj = (*view) * (*proj);

		//Set vertex declaration
		RealDevice->SetVertexDeclaration( LandDecl );

		//Set shader constants
		manager->SetWorldMatrix(&world);
		manager->SetViewMatrix(view);
		manager->SetProjectionMatrix(proj);

		manager->CommitChanges();

		ViewFrustum frustum(&viewproj);

		m_visible_set.RemoveAll();
		quadtree->GetVisibleMeshes( frustum, m_visible_set );

		manager->BeginStaticDepthRendering(false);
		m_visible_set.RenderDepth( RealDevice, SIZEOFVERT, manager );
		manager->EndDepthRendering();
	}
};

static Landscape DistantWorld;

struct DistTrans {
	DistTrans( D3DXMATRIX* transform, D3DXVECTOR3* eye_pos ) : trans(*transform) {
		D3DXVECTOR3 v( trans._41, trans._42, trans._43 );
		v -= *eye_pos;
		dist_sq = D3DXVec3LengthSq( &v );
	}
	DistTrans( const DistTrans& rh ) : dist_sq(rh.dist_sq), trans(rh.trans) {}
	DistTrans& operator=( const DistTrans& rh ) {
		dist_sq = rh.dist_sq;
		trans = rh.trans;
		return *this;
	}

	float dist_sq;
	D3DXMATRIX trans;
};

static bool DistSort ( const DistTrans & lh, const DistTrans & rh ) {
	return lh.dist_sq < rh.dist_sq;
}

void DL_EnableAlphaToCoverage(IDirect3DDevice* device, ID3DXEffect* effect, D3DXHANDLE alpha_handle ) {
	if (!AlphaToCoverageSupported || AALevel == 0) {
		//The best we can do is alpha testing
		device->SetRenderState( D3DRS_ALPHATESTENABLE, 1 );
		device->SetRenderState( D3DRS_ALPHAREF, 111 );
		device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		effect->SetFloat( alpha_handle, 1.0f );
		return;
	}

	//Use vendor specific hacks to enable alpha to coverage
	if ( VendorID == VENDOR_ATI ) {
		//ATI
		device->SetRenderState(D3DRS_POINTSIZE, MAKEFOURCC('A','2','M','1'));
		effect->SetFloat( alpha_handle, 1.75f );
	} else if ( VendorID == VENDOR_NVIDIA ) {
		//NVIDIA
		device->SetRenderState(D3DRS_ALPHATESTENABLE, 1 );
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		device->SetRenderState(D3DRS_ALPHAREF, 0x0000000F);
		device->SetRenderState(D3DRS_ADAPTIVETESS_Y, (D3DFORMAT)MAKEFOURCC('A','T','O','C'));
		effect->SetFloat( alpha_handle, 1.25f );
	}
}

void DL_DisableAlphaToCoverage(IDirect3DDevice* device) {
	if ( !AlphaToCoverageSupported || AALevel == 0 ) {
		//The best we can do is alpha testing
		device->SetRenderState( D3DRS_ALPHATESTENABLE, 0 );
		return;
	}

	//Use vendor specific hacks to disable alpha to coverage
	if ( VendorID == VENDOR_ATI ) {
		//ATI
		device->SetRenderState(D3DRS_POINTSIZE, MAKEFOURCC('A','2','M','0'));
	} else if ( VendorID == VENDOR_NVIDIA ) {
		//NVIDIA
		device->SetRenderState(D3DRS_ADAPTIVETESS_Y, D3DFMT_UNKNOWN);
		device->SetRenderState( D3DRS_ALPHATESTENABLE, 0 );
	}
}

void CalcGrassDistances( bool distant, float& zf, float& zn, float& blend_start, float&blend_end ) {
	DECLARE_MWBRIDGE

	zf = DL_NearStaticEnd;
	zn = min( mwBridge->GetViewDistance(), 7168.0f);
	zf = zn + (zf - zn) / 4.0f;

	if ( distant ) {
		blend_start = zf - ((zf - zn) * 0.5f);
		blend_end = zf;
	} else {
		//Set blend far enough away that it will do nothing
		blend_end = blend_start = DL_VeryFarStaticEnd;
	}
}

//void DL_RenderGrass( D3DXMATRIX* view, D3DXMATRIX* proj, bool distant, bool blend ) { //phal
void DL_CullGrass( VisibleSet& visible_set, D3DXMATRIX* view, D3DXMATRIX* proj, bool distant, bool blend, bool depth ) {
	//if(!DistantLandActive) return;
	DECLARE_MWBRIDGE
	if (!UseGrass || !(MGEFlags & USE_DISTANT_STATICS) || !DistantLandActive || !DL_HasStatics() || ((MGEFlags & NO_INTERIOR_DL) && !mwBridge->IsExterior())) return;


	D3DXMATRIX viewproj=(*view)*(*proj);

	ViewFrustum frustum(&viewproj);

	float zf, zn, blend_start, blend_end;
	CalcGrassDistances(distant, zf, zn, blend_start, blend_end);

	//Reset visibility flags
	//GrassCollection.ResetFlags();

	if ( distant ) {
		//Don't render anything if the far clipping plane is less than or equal to the near one
		if ( zf <= zn ) {
			return;
		}

		D3DXMATRIX near_proj = *proj;

		near_proj._33=zf/(zf-zn);
		near_proj._43=-zn*zf/(zf-zn);

		D3DXMATRIX near_viewproj = (*view)*(near_proj);

		frustum = ViewFrustum(&near_viewproj);
	} else {
		//Get visible tree objects
		TreeStatics->GetVisibleMeshes(frustum, visible_set);
	}

	//Get visible grass objects
	if (UseGrass) {
		GrassStatics->GetVisibleMeshes(frustum, visible_set);
	}

	//Sort by state to speed up drawing
	visible_set.SortByState();
}

void DL_RenderGrass( D3DXMATRIX* view, D3DXMATRIX* proj, bool distant, bool blend, bool depth ) {
	if(!UseGrass) return;
	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Grass");



	VisibleSet* visible_set = 0;
	if ( distant ) {
		if ( !g_cull_flags ) {
			return;
		}
		g_cull_flags->grass_d.Wait();
		visible_set = &g_vis_sets.grass_d;
	} else {
		if ( !g_cull_flag_grass_n ) {
			return;
		}
		g_cull_flag_grass_n->Wait();
		visible_set = &g_vis_set_grass_n;
	}

	float zf, zn, blend_start, blend_end;
	CalcGrassDistances(distant, zf, zn, blend_start, blend_end);

	if(depth) {  //phal
		//Set depth surface as render target
		g_DepthManager.BeginDepthRenderTarget();
	}

	//Use the static vertex declaration
	RealDevice->SetVertexDeclaration( StaticDecl );

	/*
	if(!depth) { //phal
		//If we're in blend mode, copy the contens of the rendering buffer to a texture
		if (DL_DistantBlurStrength > 0) {
			RealDevice->StretchRect(distantRenderSurface1,0,ShaderStruct.thisframeSurface,0,D3DTEXF_NONE );
		} else {
			RealDevice->StretchRect(BackBuffer,0,ShaderStruct.thisframeSurface,0,D3DTEXF_NONE );
		}
	}
	*/

	DL_EnableAlphaToCoverage( RealDevice, DL_Effect, ehAlphaMultiplier );

	size_t textures_size = GrassCollection.textures.size();

	size_t textures = 0;
	size_t meshes = 0;
	size_t draw_calls = 0;

	//Set up shader
	UINT passes;
	DL_Effect->Begin(&passes,0);

	DL_Effect->SetMatrix(ehView, view);
	DL_Effect->SetMatrix(ehProj, proj);

	DL_Effect->SetFloat( ehGrassDist, zf );
	//DL_Effect->SetFloat(ehTicks,GrassTime); //Moved to DL_RenderLand

	if(!depth) {//phal
		if ( blend ) {
			DL_Effect->SetFloat( ehBlendStart, blend_start );
			DL_Effect->SetFloat( ehBlendEnd, blend_end );
		} else {
			//Not using blending, so set the start and end to the far clipping plane
			DL_Effect->SetFloat(ehBlendStart, DL_DrawDist);
			DL_Effect->SetFloat(ehBlendEnd, DL_DrawDist + 10.0f);
		}
	} else {
		DL_Effect->SetFloat( ehTexBlend, 1 );
	}
	DL_Effect->SetTexture( ehTex0, ShaderStruct.thisframe);

	//Set up texture projection for blending
	D3DXMATRIX T;
	T(0,0) = 0.5f;	T(0,1) =  0.0f;  T(0,2) = 0.0f;  T(0,3) = 0.0f;
	T(1,0) = 0.0f;	T(1,1) = -0.5f;  T(1,2) = 0.0f;  T(1,3) = 0.0f;
	T(2,0) = 0.0f;	T(2,1) =  0.0f;  T(2,2) = 0.5f;  T(2,3) = 0.0f;
	T(3,0) = 0.5f;	T(3,1) =  0.5f;  T(3,2) = 0.5f;  T(3,3) = 1.0f;

	//tempmat = WaterWorld*TransView*(*projection)*T;
	D3DXMATRIX tempmat = TransView*(*proj)*T;
	DL_Effect->SetMatrix(ehTexProj, &tempmat);

	/*
	if ( UseInstancing ) {
		//phal
		if(depth) DL_Effect->BeginPass(PASS_RENDERGRASSDEPTHINST);
		else DL_Effect->BeginPass(PASS_RENDERGRASSINST);
		// DL_Effect->BeginPass(PASS_RENDERGRASSINST);

		IDirect3DVertexBuffer9* HWInstVB = HWInstVBs[0];

		RealDevice->SetVertexDeclaration( HWInstDecl );
		RealDevice->SetStreamSourceFreq( 1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul );

		D3DXMATRIX* gpu_data;

		size_t textures_size = GrassCollection.textures.size();
		if ( textures_size > 0 ) {
			TextureGroup** textures_arr = &(GrassCollection.textures[0]);
			for ( size_t t = 0; t < textures_size; ++t ) {
				TextureGroup* tg = textures_arr[t];

				if ( tg->visible == false ) {
					continue;
				}

				//Set Texture
				if(depth) { //phal
					DL_Effect->SetTexture(ehTex1, tg->tex);
				} else {
					DL_Effect->SetTexture(ehTex3, tg->tex);
				}
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
							HWInstBufferPos = 0;
							HWInstVB->Lock( 0, vis_count * sizeof(D3DXMATRIX), (void**)&gpu_data, D3DLOCK_DISCARD);
						} else {
							//Data can fit, so start from current buffer pos and lock with no overwrite
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

						//log_file << " mg->vBuffer(" << mg->vBuffer << ") mg->iBuffer(" << mg->iBuffer << ") HWInstVB(" << HWInstVB << ") vis_count(" << vis_count << ") mg->verts(" << mg->verts << ") mg->faces(" << mg->faces << ") HWInstBufferPos(" << HWInstBufferPos << ")" << endl;

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

		//Reset stream frequencies
		RealDevice->SetStreamSourceFreq( 0, 1 );
		RealDevice->SetStreamSourceFreq( 1, 1 );
	} else { */


		//phal
		if(depth) DL_Effect->BeginPass(PASS_RENDERGRASSDEPTH);
		else DL_Effect->BeginPass(PASS_RENDERGRASS);
		// DL_Effect->BeginPass(PASS_RENDERGRASS);

		D3DXHANDLE* texture_handle = 0;

		if(depth) { //phal
			texture_handle = &ehTex1;
		} else {
			texture_handle = &ehTex3;
		}

		visible_set->Render( RealDevice, DL_Effect, texture_handle, &ehWorld, SizeOfStaticVert );

	/* } */

	//log_file << "Grass Report:  " << textures << " textures, " << meshes << " meshes, " << draw_calls << " draw calls." << endl;

	DL_Effect->EndPass();
	DL_Effect->End();

	DL_DisableAlphaToCoverage(RealDevice);

	if(depth) { //phal
		//Restore render target
		g_DepthManager.EndDepthRenderTarget();
		RealDevice->SetRenderTarget(0,BackBuffer);
		RealDevice->SetDepthStencilSurface(DepthBuffer);
	}

	D3DPERF_EndEvent();
}

bool SetWorldSpace(int WorldSpace) {
	if (CurrentWorldSpace != WorldSpace) {
		NearStatics = NearQuadTree[WorldSpace];
		FarStatics = FarQuadTree[WorldSpace];
		VeryFarStatics = VeryFarQuadTree[WorldSpace];
		GrassStatics = GrassQuadTree[WorldSpace];
		TreeStatics = TreeQuadTree[WorldSpace];
		CurrentWorldSpace = WorldSpace;
	}
	return true;
}

bool DL_HasStatics() {
	if (!Initialized) {
		return false;
	}
	DECLARE_MWBRIDGE
	if(!(MGEFlags & USE_DISTANT_STATICS)) return mwBridge->IsExterior(); //phal
	if (mwBridge->IsExterior()) return SetWorldSpace(0);
	size_t NumWorldSpaces = WorldSpaces.size();
	if (NumWorldSpaces > 1) {
		char CellName[64];
		mwBridge->IntName(CellName);
		//LOG::logline("Cell: %s", CellName);
		for (size_t i = 1; i < NumWorldSpaces; ++i) if (_stricmp(CellName, WorldSpaces[i]) == 0) return SetWorldSpace(i);
	}
	return false;
}

static void CalculateStaticRangeDistances( StaticRange range, float& zf, float& zn, float& blend_start, float& blend_end, float& FogDist ) {
	DECLARE_MWBRIDGE
	float viewDistance = min( mwBridge->GetViewDistance(), 7168.0f );

	//Near clipping plane is always a certain distance in from the MW view distance except in reflection mode
	zn = viewDistance - 700.0f;

	if ( range == RANGE_NEAR ) {
		//Near
		zf = DL_NearStaticEnd;

		blend_start = zf - 700.0f;//((zf - zn) * 0.25f);
		blend_end = zf;
	} else if ( range == RANGE_FAR ) {
		//Far
		zf = DL_FarStaticEnd;

		blend_start = zf - 700.0f;//((zf - (float)Options.DL_NearStaticEnd) * 0.25f);
		blend_end = zf;
	} else if ( range == RANGE_VERY_FAR ) {
		//Very Far
		zf = DL_VeryFarStaticEnd;

		blend_start = zf - 700.0f;//((zf - (float)Options.DL_FarStaticEnd) * 0.25f);
		blend_end = zf;
	} else if ( range == RANGE_REFLECT || range == RANGE_DEPTH ) {
		//near plane is closer in reflect mode
		zn = 100.0f;

		//First we'll only include the near static distance
		zf = DL_NearStaticEnd;
	}

	//Don't render anything if the far clipping plane is less than or equal to the near one
	if ( zf <= zn ) {
		return;
	}

	//Don't let the far clipping plane be further out than the end of the fog
	if (DL_HasStatics()) {
		if (mwBridge->IsAboveWater()) {
			if (mwBridge->CellHasWeather()) FogDist = DistantFogEnd();
			else FogDist = InteriorFogEnd();
		} else FogDist = UnderwaterFogEnd();
		if ((MGEFlags & EXP_FOG) && DL_ShaderModel >= 3) FogDist *= DL_ExpFogMultiplier;
	} else FogDist = StandardFogEnd();
	zf = min( FogDist, zf );

	blend_start = zf - 700.0f;
	blend_end = zf;
}

void CullDistantStatics(VisibleSet& visible_set, D3DXMATRIX* view, D3DXMATRIX* proj, StaticRange range, bool blend ) {
	//if (!IsExterior()) return; //d4w
	//if (((MGEFlags & NO_INTERIOR_DL) && !IsExterior()) || !DL_HasStatics()) return;
	DECLARE_MWBRIDGE
	if (!UseDistantStatics || !(MGEFlags & USE_DISTANT_STATICS) || ((MGEFlags & NO_INTERIOR_DL) && !mwBridge->IsExterior()) || !DL_HasStatics()) return;

	D3DXMATRIX viewproj=(*view)*(*proj);
	ViewFrustum frustum(&viewproj);

	//Create a clipping frustum for the near and distant objects

	D3DXMATRIX range_viewproj;
	float zf, zn;
	float blend_start, blend_end;
	float FogDist = 0.0f;
	CalculateStaticRangeDistances( range, zf, zn, blend_start, blend_end, FogDist );

	//Determine view frustum
	D3DXMATRIX range_proj = *proj;
	range_proj._33=zf/(zf-zn);
	range_proj._43=-zn*zf/(zf-zn);

	range_viewproj = (*view)*(range_proj);
	ViewFrustum range_frustum(&range_viewproj);

	////In reflection mode, we only want to render out to the far static distance if the user has requested
	////and never want to render beyond that
	//if ( reflection) {
	//	if ( Options.FarStaticsReflect ) {
	//		very_far_frustum = far_frustum;
	//	} else {
	//		far_frustum = near_frustum;
	//		very_far_frustum = near_frustum;
	//	}
	//}

	//In reflection mode, we'll render the far and very far statics at the same time

	//Get visible objects from quad tree
	//StaticCollection.ResetFlags();

	if ( range == RANGE_NEAR ) {
		NearStatics->GetVisibleMeshes(range_frustum, visible_set);
	} else if ( range == RANGE_FAR ) {
		FarStatics->GetVisibleMeshes(range_frustum, visible_set);
	} else if ( range == RANGE_VERY_FAR ) {
		VeryFarStatics->GetVisibleMeshes(range_frustum, visible_set);
	} else if ( range == RANGE_REFLECT || range == RANGE_DEPTH ) {
		NearStatics->GetVisibleMeshes(range_frustum, visible_set);
		//If far statics reflect, we need to extend the frustum before flagging the objects that will be drawn
		//if ( Options.FarStaticsReflect ) {
		if (DL_WaterReflect > 2 && UseDistantStatics) {
			zf = DL_VeryFarStaticEnd;

			range_proj = *proj;
			range_proj._33=zf/(zf-zn);
			range_proj._43=-zn*zf/(zf-zn);

			range_viewproj = (*view)*(range_proj);
			range_frustum = ViewFrustum(&range_viewproj);
		}

		FarStatics->GetVisibleMeshes(range_frustum, visible_set);
		VeryFarStatics->GetVisibleMeshes(range_frustum, visible_set);
	}

	//Sort objects by render states to improve draw performance
	visible_set.SortByState();

	//QueryPerformanceCounter( &end_time);

	//log_file.setf(ios::fixed, ios::floatfield);
	//log_file << setprecision(5);

	//log_file << "Took " << double(end_time.QuadPart - start_time.QuadPart) / double(time_freq.QuadPart) << " milliseconds to cull." << endl;

	////Exit if there are no statics to draw
	//if ( visible_set.size() == 0 ) {
	//	return;
	//}

}

static void RenderDistantStatics( VisibleSet& visible_set, D3DXMATRIX* view, D3DXMATRIX* proj, StaticRange range, bool blend ) {

	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Distant Static Group");

	/*
	//If we're in blend mode, copy the contens of the rendering buffer to a texture
	if (DL_DistantBlurStrength > 0) {
		RealDevice->StretchRect(distantRenderSurface1,0,ShaderStruct.thisframeSurface,0,D3DTEXF_NONE );
	} else {
		RealDevice->StretchRect(BackBuffer,0,ShaderStruct.thisframeSurface,0,D3DTEXF_NONE );
	}
	*/

	DL_EnableAlphaToCoverage( RealDevice, DL_Effect, ehAlphaMultiplier );

	float zf, zn;
	float blend_start, blend_end;
	float FogDist = 0.0f;
	CalculateStaticRangeDistances( range, zf, zn, blend_start, blend_end, FogDist );

	//Use the static vertex declaration
	RealDevice->SetVertexDeclaration( StaticDecl );

	//Render the objects that were returned
	IDirect3DTexture9* prev_tex = 0;
	IDirect3DVertexBuffer9* prev_v_buf = 0;

	size_t draw_calls = 0;
	size_t textures = 0;
	size_t meshes = 0;
	if (range != RANGE_DEPTH ) {
		DL_Effect->SetMatrix(ehView, view);
		DL_Effect->SetMatrix(ehProj, proj);
		if ( blend ) {
			DL_Effect->SetFloat( ehBlendStart, blend_start );
			DL_Effect->SetFloat( ehBlendEnd, blend_end );
		} else {
			//Not using blending, so set the start and end to the far clipping plane
			DL_Effect->SetFloat(ehBlendStart, DL_DrawDist);
			DL_Effect->SetFloat(ehBlendEnd, DL_DrawDist + 10.0f);
		}
	} else {
		g_DepthManager.SetViewMatrix( view );
		g_DepthManager.SetProjectionMatrix( proj );
	}

	//DL_Effect->SetTexture( ehTex0, ShaderStruct.thisframe );

	////Set up texture projection for blending
	//D3DXMATRIX T;
	//T(0,0) = 0.5f;	T(0,1) =  0.0f;  T(0,2) = 0.0f;  T(0,3) = 0.0f;
	//T(1,0) = 0.0f;	T(1,1) = -0.5f;  T(1,2) = 0.0f;  T(1,3) = 0.0f;
	//T(2,0) = 0.0f;	T(2,1) =  0.0f;  T(2,2) = 0.5f;  T(2,3) = 0.0f;
	//T(3,0) = 0.5f;	T(3,1) =  0.5f;  T(3,2) = 0.5f;  T(3,3) = 1.0f;
	//
	////tempmat = WaterWorld*TransView*(*projection)*T;
	//D3DXMATRIX tempmat = TransView*(*proj)*T;
	//DL_Effect->SetMatrix(ehTexProj, &tempmat);

	/*

	if ( UseInstancing ) {
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
	} else { */

		if ( range == RANGE_REFLECT ) {
			DL_Effect->BeginPass(PASS_REFLECTSTATICS);
		} else if ( range == RANGE_DEPTH ) {
			// Do nothing
		} else{
			DL_Effect->BeginPass(PASS_RENDERSTATICS);
		}

		if ( range != RANGE_DEPTH ) {
			visible_set.Render( RealDevice, DL_Effect, &ehTex3, &ehWorld, SizeOfStaticVert );
		} else {
			visible_set.RenderDepth( RealDevice, SizeOfStaticVert, &g_DepthManager );
		}

		if ( range != RANGE_DEPTH ) {
			DL_Effect->EndPass();
		}

	/* } */


	//QueryPerformanceCounter( &end_time);
	//log_file << "Took " << double(end_time.QuadPart - start_time.QuadPart) / double(time_freq.QuadPart) << " milliseconds to render." << endl;

	//log_file << "   " << draw_calls << " draw calls " << meshes << " meshes " << textures << " textures." << endl;

	DL_DisableAlphaToCoverage(RealDevice);

	D3DPERF_EndEvent(); // Distant Static Group
}

//Functions to translate half floats to floats from OpenEXR
static inline unsigned int HalfToFloatI(unsigned short y)
{
	register int s = (y >> 15) & 0x00000001;
	register int e = (y >> 10) & 0x0000001f;
	register int m =  y        & 0x000003ff;

	if (e == 0)
	{
		if (m == 0) // Plus or minus zero
		{
			return s << 31;
		}
		else // Denormalized number -- renormalize it
		{
			while (!(m & 0x00000400))
			{
				m <<= 1;
				e -=  1;
			}

			e += 1;
			m &= ~0x00000400;
		}
	}
	else if (e == 31)
	{
		if (m == 0) // Inf
		{
			return (s << 31) | 0x7f800000;
		}
		else // NaN
		{
			return (s << 31) | 0x7f800000 | (m << 13);
		}
	}

	e = e + (127 - 15);
	m = m << 13;

	return (s << 31) | (e << 23) | m;
}
static inline float HalfToFloat(unsigned short y)
{
	union { float f; unsigned int i; } v;
	v.i = HalfToFloatI(y);
	return v.f;
}

struct StaticVertHalf {
	unsigned short Position[4];
	unsigned char Normal[4];
	unsigned char Color[4];
	unsigned short TexCoord[2];
};

struct StaticVertFull {
	float Position[3];
	unsigned char Normal[4];
	unsigned char Color[4];
	float TexCoord[2];
};

struct GrassVertHalf {
	unsigned short Position[4];
	unsigned short TexCoord[2];
};

struct GrassVertFull {
	float Position[3];
	float TexCoord[2];
};

#define READ_FROM_BUFFER( src, dest, size ) memcpy( (void*)dest, (void*)src, size );  src += size;

void LoadDS(DistantStatic *ds, LPBYTE buffer) {
	LPBYTE pos = buffer;
	READ_FROM_BUFFER( pos, &ds->NumSubsets, 4 );
	READ_FROM_BUFFER( pos, &ds->radius, 4 );
	READ_FROM_BUFFER( pos, &ds->center, 12 );
	READ_FROM_BUFFER( pos, &ds->Type, 1 );
	ds->Subsets = new DistantSubset[ds->NumSubsets];

	unsigned char HeaderData[48];
	unsigned char* dataPos;

	for(int j=0;j<ds->NumSubsets;j++) {
		UINT verts, faces;
		void* data;
		D3DXVECTOR3 center;
		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
		float radius;
		IDirect3DVertexBuffer9* vb;
		IDirect3DIndexBuffer9* ib;
		D3DXMATRIX tempmat;

		//Read header data
		READ_FROM_BUFFER( pos, HeaderData, 48 );
		dataPos = HeaderData;

		//Get radius and center;
		radius = *(float*)dataPos;
		dataPos += 4;
		center.x = *(float*)dataPos;
		dataPos += 4;
		center.y = *(float*)dataPos;
		dataPos += 4;
		center.z = *(float*)dataPos;
		dataPos += 4;

		//Get min and max (bounding box corners)
		min.x = *(float*)dataPos;
		dataPos += 4;
		min.y = *(float*)dataPos;
		dataPos += 4;
		min.z = *(float*)dataPos;
		dataPos += 4;

		max.x = *(float*)dataPos;
		dataPos += 4;
		max.y = *(float*)dataPos;
		dataPos += 4;
		max.z = *(float*)dataPos;
		dataPos += 4;

		//Get vertex and face count
		verts = *(UINT*)dataPos;
		dataPos += 4;
		faces = *(UINT*)dataPos;
		dataPos += 4;

		if ( HalfFloatSupported ) {
			//Data is already in correct format, read directly to vertex buffer
			RealDevice->CreateVertexBuffer(verts*sizeof(StaticVertHalf),D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&vb, 0);

			vb->Lock(0,0,&data,0);
			READ_FROM_BUFFER( pos, data, verts*sizeof(StaticVertHalf) );
			vb->Unlock();
		} else {
			//Data needs to be reformatted to float32.  Read to interum buffer.
			StaticVertHalf* halfVerts = new StaticVertHalf[verts];
			READ_FROM_BUFFER( pos, halfVerts, verts * sizeof(StaticVertHalf) );

			//Convert half floats to full floats
			StaticVertFull* fullVerts = new StaticVertFull[verts];
			for ( UINT i = 0; i < verts; ++i ) {
				//Position
				fullVerts[i].Position[0] = HalfToFloat( halfVerts[i].Position[0] );
				fullVerts[i].Position[1] = HalfToFloat( halfVerts[i].Position[1] );
				fullVerts[i].Position[2] = HalfToFloat( halfVerts[i].Position[2] );

				//Normal
				fullVerts[i].Normal[0] = halfVerts[i].Normal[0];
				fullVerts[i].Normal[1] = halfVerts[i].Normal[1];
				fullVerts[i].Normal[2] = halfVerts[i].Normal[2];
				fullVerts[i].Normal[3] = halfVerts[i].Normal[3];

				//Color
				fullVerts[i].Color[0] = halfVerts[i].Color[0];
				fullVerts[i].Color[1] = halfVerts[i].Color[1];
				fullVerts[i].Color[2] = halfVerts[i].Color[2];
				fullVerts[i].Color[3] = halfVerts[i].Color[3];

				//Texcoord
				fullVerts[i].TexCoord[0] = HalfToFloat( halfVerts[i].TexCoord[0] );
				fullVerts[i].TexCoord[1] = HalfToFloat( halfVerts[i].TexCoord[1] );
			}

			//Write new data to vertex buffer
			RealDevice->CreateVertexBuffer(verts*sizeof(StaticVertFull),D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&vb, 0);

			vb->Lock(0,0,&data,0);
			memcpy( data, fullVerts, verts * sizeof(StaticVertFull) );
			vb->Unlock();

			delete [] fullVerts;
		}

		RealDevice->CreateIndexBuffer(faces*6,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&ib, 0);
		ib->Lock(0,0,&data,0);
		READ_FROM_BUFFER( pos, data, faces*6 ); //Morrowind nifs don't support 32 bit indicies?
		ib->Unlock();

		short texsize;
		READ_FROM_BUFFER( pos, &texsize, 2 );
		char* texname=new char[texsize];
		READ_FROM_BUFFER( pos, texname, texsize );

		IDirect3DTexture9* tex=BSALoadTexture(texname);
		if( !tex ) {
			tex=DefaultLandTexture;
		}

		//Set values from temporary values into structure
		ds->Subsets[j].center=center;
		ds->Subsets[j].radius=radius;
		ds->Subsets[j].min=min;
		ds->Subsets[j].max=max;
		ds->Subsets[j].verts=verts;
		ds->Subsets[j].faces=faces;
		ds->Subsets[j].vBuffer=vb;
		ds->Subsets[j].iBuffer=ib;
		ds->Subsets[j].tex=tex;

		delete[] texname;
	}
}

static bool LoadDistantStatics() {
	if (!(MGEFlags & USE_DISTANT_STATICS)) return false;
	DEBUGSTRING("Loading distant statics\n");
	DECLARE_MWBRIDGE

	//Create vertex buffers for per-instance data
	for ( UINT i = 0; i < HWINSTBUFS; ++i ) {
		RealDevice->CreateVertexBuffer(sizeof(D3DXMATRIX)*MAXINSTANCES, D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &HWInstVBs[i], 0);
	}

	DWORD unused;
	InitBSAs();
	HANDLE h = CreateFile(DL_STATICS_USAGE, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(h==INVALID_HANDLE_VALUE) {
		DLINITERR("Required distant statics data is missing or corrupted");
	}
	ReadFile(h, &DistantStaticCount, 4, &unused, 0);
	DistantStatics=new DistantStatic[DistantStaticCount];
	mwBridge->ShowProgressBar(++DLProgressBar);
	HANDLE h2 = CreateFile(DL_STATICS_MESHES, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(h2==INVALID_HANDLE_VALUE) {
		DLINITERR("Required distant statics data is missing or corrupted");
	}

	// Read entire file into one big memory buffer
	DWORD file_size = GetFileSize( h2, NULL );
	unsigned char * file_buffer = new unsigned char[file_size];
	unsigned char * pos = file_buffer;
	ReadFile( h2, file_buffer, file_size, &unused, NULL );
	CloseHandle(h2);
	mwBridge->ShowProgressBar(++DLProgressBar);
	ThreadFlag *flags = new ThreadFlag[DistantStaticCount];
	LoadDSCullTask **tasks = new LoadDSCullTask*[DistantStaticCount];
	for(int i=0; i<DistantStaticCount; i++) {
		tasks[i] = new LoadDSCullTask( &flags[i], DistantStatics + i, pos );
		int numSubsets = *LPDWORD(pos);
		pos += (sizeof(int) + sizeof(float) + sizeof(D3DXVECTOR3) + sizeof(BYTE));
		for(int j=0; j < numSubsets; j++) {
			pos += *LPDWORD(pos + 10 * sizeof(DWORD)) * sizeof(StaticVertHalf) + *LPDWORD(pos + 11 * sizeof(DWORD)) * 6 + 12 * sizeof(DWORD);
			pos += *LPWORD(pos) + sizeof(WORD);
		}
	}
	mwBridge->ShowProgressBar(++DLProgressBar);
	float StaticCount = 0;
	for(int i=0; i<DistantStaticCount; i++) {
		int j = i&0x01 ? (DistantStaticCount-1 - i)|0x01 : i ; // permutation
		Scheduler.ScheduleTask( tasks[j] );
		if(StaticCount <= i) {
			StaticCount += float(DistantStaticCount)/64;	// 64 percent at all
			mwBridge->ShowProgressBar(++DLProgressBar);
		}
	}
	Scheduler.DoTasksInCurrentThread();
	mwBridge->ShowProgressBar(DLProgressBar+=3);
	for(int i=0; i<DistantStaticCount; i++) flags[i].Wait();
	delete [] flags;
	delete [] tasks;
	//Free file data buffer
	delete [] file_buffer;
	mwBridge->ShowProgressBar(++DLProgressBar);
	if (WorldSpaces.size() != 0) {
		for (size_t i = 1; i < WorldSpaces.size(); ++i) delete [] WorldSpaces[i];
		WorldSpaces.clear();
	}
	mwBridge->ShowProgressBar(++DLProgressBar);
	for (size_t WorldSpace = 0; true; ++WorldSpace) {
		size_t UsedDistantStaticCount;
		size_t toread;
		ReadFile(h, &UsedDistantStaticCount, 4, &unused, 0);
		if (WorldSpace != 0 && UsedDistantStaticCount == 0) break;
		toread = UsedDistantStaticCount * 32;
		UsedDistantStatics.push_back(vector<UsedDistantStatic>());
		if (WorldSpace == 0) {
			WorldSpaces.push_back(0);
			if (UsedDistantStaticCount == 0) continue;
		} else toread += 64;
		unsigned char* UsedDistantStaticData = new unsigned char[toread];
		unsigned char* dataPos = UsedDistantStaticData;
		ReadFile(h, UsedDistantStaticData, toread, &unused, 0);
		if (WorldSpace != 0) {
			int size = strlen((char*)dataPos) + 1;
			if (size > 64) size = 65;
			WorldSpaces.push_back(new char[size]);
			strcpy_s(WorldSpaces[WorldSpace], size, (char*)dataPos);
			dataPos += 64;
		}
		vector<UsedDistantStatic>* ThisWorldStatics = &UsedDistantStatics[WorldSpace];
		for (size_t i = 0; i < UsedDistantStaticCount; ++i) {
			UsedDistantStatic NewUsedStatic;
			NewUsedStatic.StaticRef = ReadPStream(dataPos, DWORD);
			NewUsedStatic.pos.x = ReadPStream(dataPos, float);
			NewUsedStatic.pos.y = ReadPStream(dataPos, float);
			NewUsedStatic.pos.z = ReadPStream(dataPos, float);

			float yaw = ReadPStream(dataPos, float);
			float pitch = ReadPStream(dataPos, float);
			float roll = ReadPStream(dataPos, float);
			float scale = ReadPStream(dataPos, float);
			if (scale == 0.0f) scale = 1.0f;

			NewUsedStatic.radius = DistantStatics[NewUsedStatic.StaticRef].radius * scale;
			NewUsedStatic.center = DistantStatics[NewUsedStatic.StaticRef].center + NewUsedStatic.pos;
			NewUsedStatic.scale = scale;

			D3DXMATRIX transmat, rotmatx, rotmaty, rotmatz, scalemat;
			D3DXMatrixTranslation(&transmat, NewUsedStatic.pos.x, NewUsedStatic.pos.y, NewUsedStatic.pos.z);
			D3DXMatrixRotationX(&rotmatx, -yaw);
			D3DXMatrixRotationY(&rotmaty, -pitch);
			D3DXMatrixRotationZ(&rotmatz, -roll);
			D3DXMatrixScaling(&scalemat, scale, scale, scale);
			NewUsedStatic.transform = scalemat * rotmatz * rotmaty * rotmatx * transmat;

			ThisWorldStatics->push_back(NewUsedStatic);
		}
		delete [] UsedDistantStaticData;
	}

	CloseHandle(h);
	DEBUGSTRING("Finished loading distant statics\n");
	mwBridge->ShowProgressBar(DLProgressBar+=3);
	//Success
	return true;
}

static void UnloadDistantStatics() {
	if(!(MGEFlags & USE_DISTANT_STATICS)) return;
	DEBUGSTRING("Unloading distant statics\n");

	//Free instance buffers
	for ( UINT i = 0; i < HWINSTBUFS; ++i ) {
		if (HWInstVBs[i]) {
			HWInstVBs[i]->Release();
			HWInstVBs[i] = 0;
		}
	}

	if ( DistantStatics ) {
		for(int i=0;i<DistantStaticCount;i++) {
			for(int j=0;j<DistantStatics[i].NumSubsets;j++) {

				if ( DistantStatics[i].Subsets[j].vBuffer ) {
					DistantStatics[i].Subsets[j].vBuffer->Release();
					DistantStatics[i].Subsets[j].vBuffer = 0;
				}

				if ( DistantStatics[i].Subsets[j].iBuffer ) {
					DistantStatics[i].Subsets[j].iBuffer->Release();
					DistantStatics[i].Subsets[j].iBuffer = 0;
				}

				if ( DistantStatics[i].Subsets[j].tex ) {
					DistantStatics[i].Subsets[j].tex->Release();
					DistantStatics[i].Subsets[j].tex = 0;
				}
			}

			delete [] DistantStatics[i].Subsets;
			DistantStatics[i].Subsets = 0;
		}

		delete [] DistantStatics;
		DistantStatics = 0;
	}

	size_t tmp_size = UsedDistantStatics.size();
	if (tmp_size != 0) {
		for (size_t i = 0; i < tmp_size; ++i) UsedDistantStatics[i].clear();
		UsedDistantStatics.clear();
	}
	tmp_size = WorldSpaces.size();
	if (tmp_size != 0) {
		for (size_t i = 1; i < tmp_size; ++i) delete [] WorldSpaces[i];
		WorldSpaces.clear();
	}

	DEBUGSTRING("Finished unloading distant statics\n");
}

void HookWaterLoad(IDirect3DTexture9** ptrAddr) {
	if(WaterLoadedCount==32) return;
	DECLARE_MWBRIDGE
	//LOG::logline("MGE: Water texture %d hooked", (DWORD)WaterLoadedCount);
	MorrowWaterPtrs[WaterLoadedCount]=ptrAddr;
	RealWaterTextures[WaterLoadedCount]=*ptrAddr;
	WaterLoadedCount++;
}

bool IsFakeWaterTex(IDirect3DTexture9* cmp) {
	return FakeWaterTexture == cmp;
}
#ifndef RELEASE
void ReloadShader() {
    if(DL_Effect) DL_Effect->Release();
	DL_Effect=0;
	HRESULT hr;
	hr=D3DXCreateEffectFromFile(RealDevice,"Data files\\shaders\\distantland\\InGame.fx",0,0,0,0,&DL_Effect,NULL);
	if(hr!=D3D_OK) {
		int iii=1;
	}
	DL_Effect->SetFloatArray("rcpres",(float*)&RcpRes,2);
	ehWorldViewProj=DL_Effect->GetParameterByName(0, "worldviewproj");
	ehProj=DL_Effect->GetParameterByName(0, "world");
	ehWorld=DL_Effect->GetParameterByName(0, "world2");
	ehTex0=DL_Effect->GetParameterByName(0, "tex0");
	ehTex1=DL_Effect->GetParameterByName(0, "tex1");
	ehTex2=DL_Effect->GetParameterByName(0, "tex2");
	ehTex3=DL_Effect->GetParameterByName(0, "tex3");
	ehSunCol=DL_Effect->GetParameterByName(0, "SunCol");
	ehSunAmb=DL_Effect->GetParameterByName(0, "SunAmb");
	ehSunVec=DL_Effect->GetParameterByName(0, "SunVec");
	ehFogCol=DL_Effect->GetParameterByName(0, "FogCol");
	ehFogCol2=DL_Effect->GetParameterByName(0, "FogCol2");
	ehFogRange=DL_Effect->GetParameterByName(0, "FogRange");
	ehEyePos=DL_Effect->GetParameterByName(0, "EyePos");
	ehTicks=DL_Effect->GetParameterByName(0, "ticks");
	ehOffset=DL_Effect->GetParameterByName(0, "offset");
	ehTexBlend=DL_Effect->GetParameterByName(0, "TexBlend");
}
#endif
//void FreeCell(short x, short y) {
//    if(aDistantLand(x,y).tex!=0) aDistantLand(x,y).RelPointers();
//}
//HRESULT LoadMesh(char* path,sDistantLand* land) {
//	HANDLE file=CreateFile(path,GENERIC_READ,0,0,OPEN_EXISTING,0,0);
//	if(file==INVALID_HANDLE_VALUE) return E_FAIL;
//	DWORD unused;
//	byte* data;
//	RealDevice->CreateVertexBuffer(0x1081*SIZEOFVERT,D3DUSAGE_WRITEONLY,DL_VERTEXFORMAT,D3DPOOL_DEFAULT,&land->vb, 0);
//	land->vb->Lock(0,0,(void**)&data,0);
//	ReadFile(file,data,0x1081*SIZEOFVERT,&unused,0);
//	land->vb->Unlock();
//	CloseHandle(file);
//	return S_OK;
//}
//void LoadSingleCell(short cellx,short celly,short posx, short posy) {
//    D3DXMatrixTranslation(&aDistantLand(posx,posy).world,(float)(cellx*8192),(float)(celly*8192),0);//+128
//    char path[260];
//    char temp[10];
//    strcpy(path,"data files\\distantland\\__");
//    _itoa(cellx,temp,10);
//    strcat(path,temp);
//    strcat(path,"_");
//    _itoa(celly,temp,10);
//    strcat(path,temp);
//
//	if(FAILED(LoadMesh(path,&aDistantLand(posx,posy)))) {
//		aDistantLand(posx,posy).tex=0;
//		return;
//    } else {
//		strcat(path,".dds");
//		if(FAILED(D3DXCreateTextureFromFileEx(RealDevice,path,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
//			D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&aDistantLand(posx,posy).tex))) {
//			aDistantLand(posx,posy).vb->Release();
//			aDistantLand(posx,posy).vb=0;
//            aDistantLand(posx,posy).tex=0;
//			return;
//        }
//
//    }
//}

void ChangeCell(short cellx, short celly) {
    //Stick the projection stuff here, since there's nowhere better to put it
    float zf = DL_NearStaticEnd;
    float zn=2000; //2000
    CellProj=TransProj;
    CellProj._33=zf/(zf-zn);
    CellProj._43=-zn*zf/(zf-zn);
	zn=10;
	CellProj2=TransProj;
    CellProj2._33=zf/(zf-zn);
    CellProj2._43=-zn*zf/(zf-zn);
    zn=zf/5;
	zf = DL_AboveWaterFogEnd;
    WorldProj=TransProj;
    WorldProj._33=zf/(zf-zn);
    WorldProj._43=-zn*zf/(zf-zn);
    //if(abs(cellx-OldX)+abs(celly-OldY)==1) {
    //    if(cellx!=OldX) {
    //        if(cellx>OldX) {
    //            for(int y=0;y<=Options.DL_NearStaticEnd*2;y++) {
    //                for(int x=0;x<=Options.DL_NearStaticEnd*2;x++) {
    //                    if(x==0)FreeCell(x,y);
    //                    if(x==Options.DL_NearStaticEnd*2) LoadSingleCell(cellx+x-Options.DL_NearStaticEnd,celly+y-Options.DL_NearStaticEnd,x,y);
				//		else aDistantLand(x,y)=aDistantLand(x+1,y);
    //                }
    //            }
    //        } else {
    //            for(int y=0;y<=Options.DL_NearStaticEnd*2;y++) {
    //                for(int x=Options.DL_NearStaticEnd*2;x>=0;x--) {
    //                    if(x==Options.DL_NearStaticEnd*2) FreeCell(x,y);
    //                    if(x==0) LoadSingleCell(cellx+x-Options.DL_NearStaticEnd,celly+y-Options.DL_NearStaticEnd,x,y);
    //                    else aDistantLand(x,y)=aDistantLand(x-1,y);
    //                }
    //            }
    //        }
    //    } else {
    //        if(celly>OldY) {
    //            for(int y=0;y<=Options.DL_NearStaticEnd*2;y++) {
    //                for(int x=0;x<=Options.DL_NearStaticEnd*2;x++) {
    //                    if(y==0) FreeCell(x,y);
    //                    if(y==Options.DL_NearStaticEnd*2) LoadSingleCell(cellx+x-Options.DL_NearStaticEnd,celly+y-Options.DL_NearStaticEnd,x,y);
    //                    else aDistantLand(x,y)=aDistantLand(x,y+1);
    //                }
    //            }
    //        } else {
    //            for(int y=Options.DL_NearStaticEnd*2;y>=0;y--) {
    //                for(int x=0;x<=Options.DL_NearStaticEnd*2;x++) {
    //                    if(y==Options.DL_NearStaticEnd*2) FreeCell(x,y);
    //                    if(y==0) LoadSingleCell(cellx+x-Options.DL_NearStaticEnd,celly+y-Options.DL_NearStaticEnd,x,y);
    //                    else aDistantLand(x,y)=aDistantLand(x,y-1);
    //                }
    //            }
    //        }
    //    }
    //} else {
    //    for(int y=-Options.DL_NearStaticEnd;y<=Options.DL_NearStaticEnd;y++) {
    //        for(int x=-Options.DL_NearStaticEnd;x<=Options.DL_NearStaticEnd;x++) {
    //            FreeCell(x+Options.DL_NearStaticEnd,y+Options.DL_NearStaticEnd);
    //            LoadSingleCell(x+cellx,y+celly,x+Options.DL_NearStaticEnd,y+Options.DL_NearStaticEnd);
    //        }
    //    }
    //}
    OldX=cellx;
    OldY=celly;
}


bool DL_Initialization() {
	//mendres: We should be able to reinitialize Distant Land.

	//Only load distant land once per play session.  Since we can't re-hook the water, we can't fully
	//reinitialize anyway, so focus on letting the player get back into Morrowind quickly with minimal
	//chance of a crash so they can save their game if they accidentally switch out or change resolution
	//while playing.	

	//if ( DlLoadCount == 1 ) {
	//	DlLoadCount = 2;
	//	DLINITERR("Distant Land cannot be restored after you change resolution or use Alt+Tab to switch out of Morrowind.  Save and restart the game to re-enable.");
	//} else if ( DlLoadCount > 1 ) {
	//	//Only show the above message one time
	//	return false;
	//}

	//Increment load count
	//++DlLoadCount;

	//Make sure the user's video card meets the minimum requirements
	D3DCAPS9 caps;
	bool vertexDataTypeOK = true;
	RealDevice->GetDeviceCaps( &caps );

	if ( D3DSHADER_VERSION_MAJOR(caps.PixelShaderVersion) < 2 ) {
		DLINITERR("Your video card does not support Pixel Shader 2.0.  You will not be able to use distant land.");
	}

	if ( D3DSHADER_VERSION_MAJOR(caps.VertexShaderVersion) < 2 ) {
		DLINITERR("Your video card does not support Vertex Shader 2.0.  You will not be able to use distant land.");
	}

	if ( RealObject->CheckDeviceFormat(0, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_TEXTURE, D3DFMT_R32F) != D3D_OK) {
		DLINITERR("Your video card does not support the D3DFMT_R32F texture format.  You will not be able to use distant land.");
	}

	if ( (caps.DeclTypes & D3DDTCAPS_UBYTE4N) == false ) {
		vertexDataTypeOK = false;
	}
	if ( (caps.DeclTypes & D3DDTCAPS_SHORT2N) == false ) {
		vertexDataTypeOK = false;
	}

	if ( vertexDataTypeOK == false ) {
		DLINITERR("Your video card does not support the UBYTE4N and SHORT2N vertex data types.  You will not be able to use distant land.");
	}

	//Check for half float support
	if ( (caps.DeclTypes & D3DDTCAPS_FLOAT16_2) == false ) {
		HalfFloatSupported = false;
	}
	if ( (caps.DeclTypes & D3DDTCAPS_FLOAT16_4) == false ) {
		HalfFloatSupported = false;
	}

	if (!HalfFloatSupported) {
		SETSTATUS("16-bit floats are not supported by your video card.  32-bit floats will be used instead.");
	}

	//Check for the vendor of the video card
	static D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
	HRESULT hr = RealObject->GetAdapterIdentifier(0, 0, &AdapterIdentifier);
	bool success = false;
	if ( hr == S_OK ) {
		VendorID = AdapterIdentifier.VendorId;

		if ( VendorID == VENDOR_ATI ) {
			//Supposedly all DX9 ATI cards support Alpha to coverage
			AlphaToCoverageSupported = true;
		} else if ( VendorID == VENDOR_NVIDIA ) {
			//Some NVIDIA DX9 cards don't support alpha to coverage.  Check for the
			//capability
			if ( RealObject->CheckDeviceFormat(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				D3DFMT_X8R8G8B8,
				0,
				D3DRTYPE_SURFACE,
				(D3DFORMAT)MAKEFOURCC('A', 'T', 'O', 'C')) == S_OK )
			{
				AlphaToCoverageSupported = true;
			} else {
				SETSTATUS("It appears that your NVIDIA video card does not support alpha to coverage.  Statics will not be blended.\n");
			}
		} else {
			SETSTATUS("It appears that your video card was not made by ATI or NVIDIA so statics will not be blended.\n");
		}
	} else {
		SETSTATUS("Failed to detect the manufacturer of your video card.  Statics will not be blended.\n");
	}

	bool createDL = false;
	HANDLE hDLver = CreateFile(DL_VERSION, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(hDLver == INVALID_HANDLE_VALUE) createDL = true;
	else {
		CloseHandle(hDLver);
		createDL = !DistantLandCreator::GetDLFilesExist();
	}
	if(createDL || ((MGEFlags & USE_DISTANT_STATICS) && !DistantLandCreator::GetDSFilesExist())) {
		DECLARE_MWBRIDGE
		bool created = false;
		DLProgressBar = 0;
		mwBridge->SetProgressBarName(createDL ? "Creating Distant Land" : "Creating Distant Statics");
		mwBridge->ShowProgressBar(++DLProgressBar);
		InitBSAs();
		mwBridge->ShowProgressBar(++DLProgressBar);
		DistantLandCreator *dlc = new DistantLandCreator();
		mwBridge->ShowProgressBar(++DLProgressBar);
		if(dlc->LoadPlugins()) {
  			created = dlc->CreateDistantLand(!createDL);
			mwBridge->ShowProgressBar(created ? 100.0f: 0.0f);
			mwBridge->HideProgressBar();
		}
		delete dlc;
		if(!created) {
			if(hDLver != INVALID_HANDLE_VALUE) {
				DLINITERR("Could not create Distant Land files.\n"
					"You need to run the distant land setup wizard");
			} else {
				DLINITERR("Distant land was enabled, but distant land version file doesn't exist.\n"
					"You need to run the distant land setup wizard");
			}
		}
	}
	hDLver = CreateFile(DL_VERSION, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if(hDLver != INVALID_HANDLE_VALUE) {
		byte version = 0xff;
		DWORD unused;
		ReadFile(hDLver, &version, 1, &unused, 0);
		if(version != MGE_DL_VERSION) {
			DLINITERR("Distant land was enabled, but distant land files are from an old version of MGE.\n"
				"You need to rerun the distant land setup wizard");
		}
		CloseHandle(hDLver);
	}

	DLProgressBar = 0;
	DECLARE_MWBRIDGE
	mwBridge->SetProgressBarName("Loading Distant Land");
	mwBridge->ShowProgressBar(++DLProgressBar);
    DEBUGSTRING("DistantLand init\n");
    OldX=OldY=TempX=TempY=-999;
    SetPos=false;

	LPD3DXBUFFER errors = 0;

/*	if(Options.DLSM1Mode) {
		if(FAILED(D3DXCreateEffectFromFile(RealDevice,"Data files\\shaders\\distantland\\InGame1.fx",0,0,0,0,&DL_Effect,&errors))) {
			ofstream err_file("Shader Compiler Errors.txt");
			err_file.write( (const char*)errors->GetBufferPointer(), errors->GetBufferSize() );
			err_file.close();
			DLINITERR("Could not load distant land effect file");
		}
	} else {*/
//		if(FAILED(D3DXCreateEffectFromFile(RealDevice,"Data files\\shaders\\distantland\\InGame.fx",0,0,0,0,&DL_Effect,&errors))) {
//			ofstream err_file("Shader Compiler Errors.txt");
//			err_file.write( (const char*)errors->GetBufferPointer(), errors->GetBufferSize() );
//			err_file.close();
//			DLINITERR("Could not load distant land effect file");
//		}
	//}

	//pass settings constants to ingame.fx shader
	//std::string effectstr ("");
	int len;
	#define EFFECTSTR_SIZE 4096
	char *effectstr = new char [EFFECTSTR_SIZE];
	effectstr [0] = '\0';
	sprintf_s (effectstr, EFFECTSTR_SIZE, "%sSHADER_MODEL %i\n", define, (int)(DL_ShaderModel * 100));
	if(MGEFlags & DYNAMIC_RIPPLES) {
		//effectstr.append("#define MGE_DYNAMICRIPPLES\n");
		len = strlen (effectstr);
		sprintf_s (effectstr + len, EFFECTSTR_SIZE - len, "%sMGE_DYNAMICRIPPLES\n", define);
		DECLARE_MWBRIDGE
		mwBridge->TurnOldRipples (false);
	}
	mwBridge->ShowProgressBar(++DLProgressBar);
	if (DL_ShaderModel >= 3) {
		if (MGEFlags & EXP_FOG) {
			len = strlen (effectstr);
			sprintf_s (effectstr + len, EFFECTSTR_SIZE - len, "%sEXPFOG\n", define);
		}
		if (MGEFlags & BLUR_REFLECTIONS) {
			len = strlen (effectstr);
			sprintf_s (effectstr + len, EFFECTSTR_SIZE - len, "%sWATERBLUR\n", define);
		}
	}
	len = strlen (effectstr);
	sprintf_s (effectstr + len, EFFECTSTR_SIZE - len, "%sWAVEHEIGHT %i\n", define, (DL_ShaderModel >= 3 ? DL_WaterWaveHeight : 0));
	len = strlen (effectstr);
	//sprintf_s (effectstr + len, EFFECTSTR_SIZE - len, "%sCAUSTICINT %i\n", define, (DL_ShaderModel >= 3 ? DL_WaterCaustics : 0));
	//effectstr.append("#include \"Data files\\shaders\\distantland\\InGame.fx\"");
	strcat_s (effectstr, EFFECTSTR_SIZE, "#include \"Data files\\shaders\\distantland\\InGame.fx\"");
	//MSG(effectstr.c_str());
	mwBridge->ShowProgressBar(++DLProgressBar);
	//if (FAILED (D3DXCreateEffect (RealDevice, effectstr.c_str (), effectstr.length (), 0, 0, 0, 0, &DL_Effect, &errors))) {
	if (FAILED (D3DXCreateEffect (RealDevice, effectstr, strlen (effectstr), 0, 0, 0, 0, &DL_Effect, &errors))) {
		delete [] effectstr;
		ofstream err_file ("Shader Compiler Errors - InGame.fx.txt");
		err_file.write ((const char*)errors->GetBufferPointer (), errors->GetBufferSize ());
		err_file.close ();
		DLINITERR ("Could not load distant land effect file");
	}
	delete [] effectstr;

	if (errors) {
		DEBUGSTRING ((char*)errors->GetBufferPointer ());
		errors->Release ();
		errors = 0;
	}
	mwBridge->ShowProgressBar(++DLProgressBar);
	DL_Effect->SetFloatArray("rcpres",(float*)&RcpRes,2);
	//ehWorldViewProj=DL_Effect->GetParameterByName(0, "worldviewproj");
	ehView=DL_Effect->GetParameterByName(0, "view");
	ehProj=DL_Effect->GetParameterByName(0, "proj");
	ehTexProj=DL_Effect->GetParameterByName(0, "TexProj");
	ehWorld=DL_Effect->GetParameterByName(0, "world");
	ehMatrixPalette=DL_Effect->GetParameterByName(0, "MatrixPalette");
	ehTex0=DL_Effect->GetParameterByName(0, "tex0");
	ehTex1=DL_Effect->GetParameterByName(0, "tex1");
	ehTex2=DL_Effect->GetParameterByName(0, "tex2");
	ehTex3=DL_Effect->GetParameterByName(0, "tex3");
	ehTex4=DL_Effect->GetParameterByName(0, "tex4");
	ehTex5=DL_Effect->GetParameterByName(0, "tex5");
	ehSunCol=DL_Effect->GetParameterByName(0, "SunCol");
	ehSkyCol=DL_Effect->GetParameterByName(0, "SkyCol");
	ehSunAmb=DL_Effect->GetParameterByName(0, "SunAmb");
	ehSunVec=DL_Effect->GetParameterByName(0, "SunVec");
	ehFogCol=DL_Effect->GetParameterByName(0, "FogCol");
	ehFogCol2=DL_Effect->GetParameterByName(0, "FogCol2");
	mwBridge->ShowProgressBar(++DLProgressBar);
	ehFogStart=DL_Effect->GetParameterByName(0, "FogStart");
	ehFogRange=DL_Effect->GetParameterByName(0, "FogRange");
	ehBlendStart=DL_Effect->GetParameterByName(0, "BlendStart");
	ehBlendEnd=DL_Effect->GetParameterByName(0, "BlendEnd");
	ehGrassDist=DL_Effect->GetParameterByName(0, "GrassDist");
	ehEyePos=DL_Effect->GetParameterByName(0, "EyePos");
	ehTicks=DL_Effect->GetParameterByName(0, "ticks");
	ehOffset=DL_Effect->GetParameterByName(0, "offset");
	ehTexBlend=DL_Effect->GetParameterByName(0, "TexBlend");
	ehWindVec=DL_Effect->GetParameterByName(0, "WindVec");
	ehPixelWidth=DL_Effect->GetParameterByName(0, "PixelWidth");
	ehPixelHeight=DL_Effect->GetParameterByName(0, "PixelHeight");
	ehTwelveKernel=DL_Effect->GetParameterByName(0, "TwelveKernel");
	ehWaterLevel=DL_Effect->GetParameterByName(0, "WaterLevel");
	ehAlphaMultiplier = DL_Effect->GetParameterByName( 0, "AlphaMultiplier" );
	ehSunPos=DL_Effect->GetParameterByName(0, "SunPos");//phal
	ehSunVis=DL_Effect->GetParameterByName(0, "SunVis");//phal
	ehPlayerPos=DL_Effect->GetParameterByName(0, "PlayerPos");//phal
	ehRippleOrigin=DL_Effect->GetParameterByName(0, "RippleOrigin");//phal
	ehCausticIntens = DL_Effect->GetParameterByName (0, "CausticIntens");
	mwBridge->ShowProgressBar(++DLProgressBar);
	// check if InGame.fx demands a tesselated water mesh
	fineWaterMesh = 0;
	DL_Effect->GetBool(DL_Effect->GetParameterByName(0, "fineWaterMesh"), &fineWaterMesh);
	// read demanded water wave resolution
	DL_Effect->GetInt(DL_Effect->GetParameterByName(0, "waveTexResolution"), &waveTexResolution);
	//crop resolution to power of 2, min size 128
	int res=128;
	while( res < waveTexResolution ) res<<=1;
	waveTexResolution = res;
	// read wave texture world resolution
	DL_Effect->GetFloat(DL_Effect->GetParameterByName(0, "waveTexWorldResolution"), &waveTexWorldRes);
	if(waveTexWorldRes<0.25f) waveTexWorldRes = 0.25f;
	DL_Effect->SetFloat(DL_Effect->GetParameterByName(0, "waveTexWorldSize"),waveTexWorldRes*waveTexResolution);
	DL_Effect->SetFloat(DL_Effect->GetParameterByName(0, "waveTexRcpRes"),1.0f/(waveTexResolution-1));

	DistantWorld.quadtree = new QuadTree(5000, 100000);
	mwBridge->ShowProgressBar(++DLProgressBar);

	if( FAILED(DistantWorld.Load()) ) {	// 6 percent
		DLINITERR("Could not load world mesh for distant land");
	}

	if(FAILED(RealDevice->CreateDepthStencilSurface(JointInfo.GraphicsWidth,JointInfo.GraphicsHeight,D3DFMT_D24X8,(D3DMULTISAMPLE_TYPE)0,0,true,&ReflectionZBuffer,0))) {
		DLINITERR("Failed to create distant land z-buffer");
	}

	if(FAILED(D3DXCreateTextureFromFile(RealDevice,"data files\\textures\\mge\\default\\fakewater.dds",&FakeWaterTexture))) {
		DLINITERR("Failed to load distant water texture");
	}

	mwBridge->ShowProgressBar(++DLProgressBar);

	for(int i=0;i<32;i++) FakeWaterTexture->AddRef();

	//Load volumetric water normal map texture
	if(FAILED(D3DXCreateVolumeTextureFromFileEx(RealDevice,"data files\\textures\\mge\\default\\water\\water_NRM.dds",0,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_DEFAULT,
		D3DX_DEFAULT,D3DX_DEFAULT,0,0,0,&NewWaterTexture))) {
		DLINITERR("Could not load the volumetric water normal texture");
	}
	mwBridge->ShowProgressBar(++DLProgressBar);
	// Update fullscreen shader water texture
	if( (MGEFlags & SET_SHADER_VARS) ) {
		for( size_t i=0; i < ShaderStruct.DefEffectCount; i++) {
			ShaderStruct.DefaultEffects[i].effect->SetTexture("watertex", NewWaterTexture);
		}
	}

	mwBridge->ShowProgressBar(++DLProgressBar);

/*
	//Create surface for water texture interpolation //phal
	if(FAILED(WaterBlendTexture->GetSurfaceLevel(0,&WaterBlendSurface))) {
		DLINITERR("Failed to get water texture surface from water texture.");
	}
	//Vertex buffer for interpolation
	RealDevice->CreateVertexBuffer(4*sizeof(MyVertex),D3DUSAGE_WRITEONLY,MYVERTEXFORMAT,D3DPOOL_DEFAULT, &WaterBlendVB, 0);
	static MyVertex blendVertices[] = {
		{-0.5f,						-0.5f,						0,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{-0.5f,						WaterSurfInfo.Height-0.5f,	0,1,0,1,0,1,0,1,0,1,0,1,0,1},
		{WaterSurfInfo.Width-0.5f,	-0.5f,						0,1,1,0,1,0,1,0,1,0,1,0,1,0},
		{WaterSurfInfo.Width-0.5f,	WaterSurfInfo.Height-0.5f,	0,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	byte* VertexPointer;
	WaterBlendVB->Lock(0,0,(void**)&VertexPointer,0);
	CopyMemory(VertexPointer,blendVertices,sizeof(blendVertices));
	WaterBlendVB->Unlock();
*/
	if(MGEFlags & DYNAMIC_RIPPLES) {
		//Create surfaces for wave calculation //phal
		RealDevice->CreateTexture(waveTexResolution,waveTexResolution,1,D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT,&waveRainTexture,NULL);
		if(FAILED(waveRainTexture->GetSurfaceLevel(0,&waveRainSurface))) { DLINITERR("Failed to create wave rain surface.");}
		RealDevice->ColorFill(waveRainSurface,0,0x80808080);

		RealDevice->CreateTexture(waveTexResolution,waveTexResolution,1,D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT,&waveRippleTexture,NULL);
		if(FAILED(waveRippleTexture->GetSurfaceLevel(0,&waveRippleSurface))) { DLINITERR("Failed to create wave ripple surface.");}
		RealDevice->ColorFill(waveRippleSurface,0,0x80808080);

		RealDevice->CreateTexture(waveTexResolution,waveTexResolution,1,D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT,&waveTempTexture,NULL);
		if(FAILED(waveTempTexture->GetSurfaceLevel(0,&waveTempSurface))) { DLINITERR("Failed to create wave ripple surface.");}
		RealDevice->ColorFill(waveTempSurface,0,0x80808080);

		//Vertex buffer for wave texture
		RealDevice->CreateVertexBuffer(3*sizeof(MyVertex),D3DUSAGE_WRITEONLY,MYVERTEXFORMAT,D3DPOOL_DEFAULT, &WaveVB, 0);
		int xx = (waveTexResolution)/2;
		static MyVertex waveVertices[] = {
			/*{ -0.5f,					-0.5f,					0,1,0,0,0,0,0,0,0,0,0,0,0,0},
			{ -0.5f,					waveTexResolution-0.5f,	0,1,0,1,0,1,0,1,0,1,0,1,0,1},
			{waveTexResolution-0.5f,	-0.5f,					0,1,1,0,1,0,1,0,1,0,1,0,1,0},
			{waveTexResolution-0.5f,	waveTexResolution-0.5f,	0,1,1,1,1,1,1,1,1,1,1,1,1,1}*/
			// use only one tri over the whole texture to prevent seams at tri edges (no idea why they even appear)
			{ -waveTexResolution/2	-0.5f,	waveTexResolution/2	-0.5f,	0,1,-0.5,0.5,	0,0,0,0,0,0,0,0,0,0},
			{ waveTexResolution		-0.5f,	2*waveTexResolution	-0.5f,	0,1,1.0,2.0,	0,1,0,1,0,1,0,1,0,1},
			{ waveTexResolution		-0.5f,	-waveTexResolution	-0.5f,	0,1,1.0,-1.0,	1,1,1,1,1,1,1,1,1,1}
		};
		byte* VertexPointer;
		WaveVB->Lock(0,0,(void**)&VertexPointer,0);
		CopyMemory(VertexPointer,waveVertices,sizeof(waveVertices));
		WaveVB->Unlock();
	}
	mwBridge->ShowProgressBar(++DLProgressBar);

	//Load Default land texture
	if(FAILED(D3DXCreateTextureFromFile(RealDevice,"data files\\textures\\mge\\default\\land_default.tga",&DefaultLandTexture))) {
		DLINITERR("Failed to load default land texture");
	}

	//Create instance buffer vertex declaration
	if(FAILED(RealDevice->CreateVertexDeclaration( HWInstElem, &HWInstDecl ))) {
		DLINITERR("Failed to create hardware instancing vertex declaration.");
	}

	//Create static and grass vertex declarations
	if ( HalfFloatSupported ) {
		if(FAILED(RealDevice->CreateVertexDeclaration( StaticElem, &StaticDecl ))) {
			DLINITERR("Failed to to create static vertex declaration.");
		}
		SizeOfStaticVert = sizeof(StaticVertHalf);

	} else {
		if(FAILED(RealDevice->CreateVertexDeclaration( StaticElemFullFloat, &StaticDecl ))) {
			DLINITERR("Failed to to create static vertex declaration.");
		}
		SizeOfStaticVert = sizeof(StaticVertFull);
	}
	mwBridge->ShowProgressBar(++DLProgressBar);

	//Create world vertex declaration
	if(FAILED(RealDevice->CreateVertexDeclaration( LandElem, &LandDecl ))) {
		DLINITERR("Failed to to create world vertex declaration.");
	}

	//Create texture to save the previous distant land render
	D3DSURFACE_DESC BackBufferDesc;
	RealDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuffer);
	BackBuffer->GetDesc(&BackBufferDesc);

	//Calculate Pixel width and height
	PixelWidth = 1.0f / (BackBufferDesc.Width - 1);
	PixelHeight = 1.0f / (BackBufferDesc.Height - 1);

	if(FAILED(RealDevice->CreateTexture(BackBufferDesc.Width,BackBufferDesc.Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,&distantRenderTexture1,NULL))) {
		DLINITERR("Failed to create distant land buffer surface.");
	}
	if(FAILED(distantRenderTexture1->GetSurfaceLevel(0,&distantRenderSurface1))) {
		DLINITERR("Failed to get distant land texture from distant land buffer surface.");
	}

	if(FAILED(RealDevice->CreateTexture(BackBufferDesc.Width,BackBufferDesc.Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,&distantRenderTexture2,NULL))) {
		DLINITERR("Failed to create distant land buffer 2 surface.");
	}
	if(FAILED(distantRenderTexture2->GetSurfaceLevel(0,&distantRenderSurface2))) {
		DLINITERR("Failed to get distant land texture from distant land buffer 2 surface.");
	}
	/* phal */
	if(FAILED(RealDevice->CreateTexture(BackBufferDesc.Width,BackBufferDesc.Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,&distantReflectionTexture,NULL))) {
		DLINITERR("Failed to create distant reflection surface.");
	}
	if(FAILED(distantReflectionTexture->GetSurfaceLevel(0,&distantReflectionSurface))) {
		DLINITERR("Failed to get texture from reflection surface.");
	}/* /phal */
	if(FAILED(RealDevice->CreateTexture(BackBufferDesc.Width,BackBufferDesc.Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,&distantRenderTexture1,NULL))) {
		DLINITERR("Failed to create distant land buffer surface.");
	}
	if(FAILED(distantRenderTexture1->GetSurfaceLevel(0,&distantRenderSurface1))) {
		DLINITERR("Failed to get distant land texture from distant land buffer surface.");
	}

	switch( DL_CreateMeshes() ) {	// 3 percent
	case 1:
		DLINITERR("Failed to create distant water vertex buffer");
		break;
	case 2:
		DLINITERR("Failed to create distant water index buffer");
		break;
	case 3:
		DLINITERR("Failed to create lowest land vertex buffer");
		break;
	case 4:
		DLINITERR("Failed to create lowest land index buffer");
		break;
	default:
		break;
	}
/*
	//We can't just have a single quad with edges 1,000,000 units across, because the far edges will get too far out in some places and
	//cause the edge of the water to vibrate due to inadequate floating point percision.  So we'll make an inner square in the center of
	//that's small enough that nothing within it will vibrate within it.

	WaterVertex* data;
	if(FAILED(RealDevice->CreateVertexBuffer(8*SIZEOFWVERT,D3DUSAGE_WRITEONLY,DL_WATERFORMAT,D3DPOOL_DEFAULT,&WaterVB1,0))) {
		DLINITERR("Failed to create distant water vertex buffer");
	}
	WaterVB1->Lock(0,0,(void**)&data,0);

	//Outer Square
	data[3].x = 500000.0f;
	data[3].y = 500000.0f;
	data[3].z = -4.0f;
	data[2].x = -500000.0f;
	data[2].y = 500000.0f;
	data[2].z = -4.0f;
	data[1].x = -500000.0f;
	data[1].y = -500000.0f;
	data[1].z = -4.0f;
	data[0].x = 500000.0f;
	data[0].y = -500000.0f;
	data[0].z = -4.0f;
	//Inner square
	data[7].x = 8192.0f;
	data[7].y = 8192.0f;
	data[7].z = -4.0f;
	data[6].x = -8192.0f;
	data[6].y = 8192.0f;
	data[6].z = -4.0f;
	data[5].x = -8192.0f;
	data[5].y = -8192.0f;
	data[5].z = -4.0f;
	data[4].x = 8192.0f;
	data[4].y = -8192.0f;
	data[4].z = -4.0f;

	WaterVB1->Unlock();

	if(FAILED(RealDevice->CreateIndexBuffer(30,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&WaterIB,0))) {
		DLINITERR("Failed to create distant water index buffer");
	}
	WORD* i;
	WaterIB->Lock(0,0,(void**)&i,0);
	i[0]  = 7;  i[1]  = 6;  i[2]  = 4;
	i[3]  = 6;  i[4]  = 5;  i[5]  = 4;
	i[6]  = 5;  i[7]  = 0;  i[8]  = 4;
	i[9]  = 4;  i[10] = 0;  i[11] = 3;
	i[12] = 7;  i[13] = 4;  i[14] = 3;
	i[15] = 2;  i[16] = 7;  i[17] = 3;
	i[18] = 2;  i[19] = 6;  i[20] = 7;
	i[21] = 2;  i[22] = 1;  i[23] = 6;
	i[24] = 6;  i[25] = 1;  i[26] = 5;
	i[27] = 5;  i[28] = 1;  i[29] = 0;
	WaterIB->Unlock();
*/
	if (MGEFlags & USE_DISTANT_STATICS) {
		//Load distant objects
		if (!LoadDistantStatics()) {	// 75 percent at all
			return false;
		}

		size_t NumWorldSpaces = WorldSpaces.size();
		for (size_t WorldSpace = 0; WorldSpace < NumWorldSpaces; ++WorldSpace) {
			size_t UsedDistantStaticCount = UsedDistantStatics[WorldSpace].size();
			//Add objects to quadtree

			NearQuadTree.push_back(new QuadTree());
			QuadTree* NQTR = NearQuadTree[WorldSpace];
			FarQuadTree.push_back(new QuadTree());
			QuadTree* FQTR = FarQuadTree[WorldSpace];
			VeryFarQuadTree.push_back(new QuadTree());
			QuadTree* VFQTR = VeryFarQuadTree[WorldSpace];
			GrassQuadTree.push_back(new QuadTree());
			QuadTree* GQTR = GrassQuadTree[WorldSpace];
			TreeQuadTree.push_back(new QuadTree());
			QuadTree* TQTR = TreeQuadTree[WorldSpace];

			//Calclulate optimal initial quadtree size
			D3DXVECTOR2 max = D3DXVECTOR2(FLT_MIN, FLT_MIN);
			D3DXVECTOR2 min = D3DXVECTOR2(FLT_MAX, FLT_MAX);

			//Find minimum and maximum x, y, and z positions
			for (size_t i = 0; i < UsedDistantStaticCount; ++i) {
				UsedDistantStatic* CurrentStatic = &UsedDistantStatics[WorldSpace][i];
				float x = CurrentStatic->pos.x;
				float y = CurrentStatic->pos.y;

				if ( x > max.x ) { max.x = x; }
				if ( y > max.y ) { max.y = y; }

				if ( x < min.x ) { min.x = x; }
				if ( y < min.y ) { min.y = y; }
			}

			float box_size;
			D3DXVECTOR2 box_center;
			if ( max.x - min.x > max.y - min.y ) box_size = max.x - min.x;
			else box_size = max.y - min.y;
			box_center = (max + min) / 2;

			NQTR->SetBoxSize(box_size);
			NQTR->SetBoxCenter(box_center);

			FQTR->SetBoxSize(box_size);
			FQTR->SetBoxCenter(box_center);

			VFQTR->SetBoxSize(box_size);
			VFQTR->SetBoxCenter(box_center);

			GQTR->SetBoxSize(box_size);
			GQTR->SetBoxCenter(box_center);

			TQTR->SetBoxSize(box_size);
			TQTR->SetBoxCenter(box_center);

			for (size_t i = 0; i < UsedDistantStaticCount; ++i) {
				UsedDistantStatic* CurrentStatic = &UsedDistantStatics[WorldSpace][i];
				//Add all objects to the main quad tree
				DistantStatic* stat = &DistantStatics[CurrentStatic->StaticRef];

				for ( int s = 0; s < stat->NumSubsets; ++s ) {
					MeshCollection::VisFlags vf;

					if ( stat->Type == STATIC_GRASS || stat->Type == STATIC_TREE ) {
						//Add to Grass mesh collection
						vf = GrassCollection.Add(CurrentStatic, &stat->Subsets[s]);
						if ( stat->Type == STATIC_GRASS ) {
							GQTR->AddMesh(
								CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
								CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
								CurrentStatic->transform,
								stat->Subsets[s].tex,
								stat->Subsets[s].verts,
								stat->Subsets[s].vBuffer,
								stat->Subsets[s].faces,
								stat->Subsets[s].iBuffer
							);

							continue;
						} else {
							TQTR->AddMesh(
								CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
								CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
								CurrentStatic->transform,
								stat->Subsets[s].tex,
								stat->Subsets[s].verts,
								stat->Subsets[s].vBuffer,
								stat->Subsets[s].faces,
								stat->Subsets[s].iBuffer
							);
						}
					}

					//Add to static mesh collection
					vf = StaticCollection.Add(CurrentStatic, &stat->Subsets[s] );

					//Add objects to appropriate quadtree
					if ( stat->Type == STATIC_AUTO || stat->Type == STATIC_TREE || stat->Type == STATIC_BUILDING ) {
						float multiplier = 1.0f;
						if ( stat->Type == STATIC_BUILDING ) {
							multiplier = 2.0f;
						}

						if (stat->radius <= DL_FarStaticMinSize * multiplier) {
							NQTR->AddMesh(
								CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
								CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
								CurrentStatic->transform,
								stat->Subsets[s].tex,
								stat->Subsets[s].verts,
								stat->Subsets[s].vBuffer,
								stat->Subsets[s].faces,
								stat->Subsets[s].iBuffer
							);
						} else {
							if (stat->radius <= DL_VeryFarStaticMinSize * multiplier) {
								FQTR->AddMesh(
									CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
									CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
									CurrentStatic->transform,
									stat->Subsets[s].tex,
									stat->Subsets[s].verts,
									stat->Subsets[s].vBuffer,
									stat->Subsets[s].faces,
									stat->Subsets[s].iBuffer
								);
							} else {
								VFQTR->AddMesh(
									CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
									CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
									CurrentStatic->transform,
									stat->Subsets[s].tex,
									stat->Subsets[s].verts,
									stat->Subsets[s].vBuffer,
									stat->Subsets[s].faces,
									stat->Subsets[s].iBuffer
								);
							}
						}
					} else if ( stat->Type == STATIC_NEAR ) {
						NQTR->AddMesh(
							CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
							CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
							CurrentStatic->transform,
							stat->Subsets[s].tex,
							stat->Subsets[s].verts,
							stat->Subsets[s].vBuffer,
							stat->Subsets[s].faces,
							stat->Subsets[s].iBuffer
						);
					} else if ( stat->Type == STATIC_FAR ) {
						FQTR->AddMesh(
							CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
							CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
							CurrentStatic->transform,
							stat->Subsets[s].tex,
							stat->Subsets[s].verts,
							stat->Subsets[s].vBuffer,
							stat->Subsets[s].faces,
							stat->Subsets[s].iBuffer
						);
					} else if ( stat->Type == STATIC_VERY_FAR ) {
						VFQTR->AddMesh(
							CurrentStatic->GetBoundingSphere( stat->Subsets[s] ),
							CurrentStatic->GetBoundingBox( stat->Subsets[s] ),
							CurrentStatic->transform,
							stat->Subsets[s].tex,
							stat->Subsets[s].verts,
							stat->Subsets[s].vBuffer,
							stat->Subsets[s].faces,
							stat->Subsets[s].iBuffer
						);
					}
				}
			}

			NQTR->Optimize();
			FQTR->Optimize();
			VFQTR->Optimize();
			GQTR->Optimize();
			TQTR->Optimize();

			NQTR->CalcVolume();
			FQTR->CalcVolume();
			VFQTR->CalcVolume();
			GQTR->CalcVolume();
			TQTR->CalcVolume();

			//NearQuadTree.Report();
			//FarQuadTree.Report();

			UsedDistantStatics[WorldSpace].clear();
		}
		mwBridge->ShowProgressBar(DLProgressBar+=3);
		//Remove UsedDistantStatic object
		UsedDistantStatics.clear();
	}

	//Initialization is complete
	Initialized = true;
	DlDoneHooking=true;
	TimeCounter = 0;
	TotalTime = 0.0f;
	GrassTime = 0.0f;
	SmoothWindVec[0] = 0.0f;
	SmoothWindVec[1] = 0.0f;
	SmoothWindSpeed = 0.0f;
	SmoothFogFactor = 1.0f;
	SmoothFogOffset = 0.0f;

	mwBridge->ShowProgressBar(100.0f);	// DL Load Progress 100

	DEBUGSTRING("Distant land init complete\n");
	DlLoadCount = 1;	//mendres
	return true;
}

bool _stdcall DL_Initialize() {
	if(Initialized) return Initialized;
	if (!(MGEFlags & USE_DISTANT_LAND)) return false;
	DECLARE_MWBRIDGE
	bool result = DL_Initialization();
	mwBridge->ShowProgressBar(result ? 100.0f: 0.0f);
	mwBridge->HideProgressBar();
	return result;
}

bool DL_Init() {
	if(Initialized) return Initialized;
	if(Initializing) return false;
	DECLARE_MWBRIDGE
	////////////////////////////////
	Initializing = true;
	mwBridge->SetScreen(0.5f);
	bool result = DL_Initialize();
	mwBridge->UnsetScreen(0.5f);
	Initializing = false;
	////////////////////////////////
	if(InvertUsingDL == 0x02) InvertUsingDL++;
	return result;
}


void DL_Shutdown() {
	DL_Deactivate();	//mendres: Deactivate Distant Land before reinitialization.
	//Release all distant land resources
	if ( !Initialized ) {
		return;
	}

	Initialized=false;

    DEBUGSTRING("DistantLand shut down\n");

	if ( g_cull_flags ) {
		//Wait for all threads to finish
		if (DL_WaterReflect > 0) {
			g_cull_flags->static_reflected.Wait();
		}
		g_cull_flags->grass_d.Wait();
		g_cull_flags->static_f.Wait();
		g_cull_flags->static_n.Wait();

		g_cull_flags->static_vf.Wait();

		delete g_cull_flags;
		g_cull_flags = 0;
	}

	g_vis_sets.ClearAll();

	if ( DL_Effect ) {
		DL_Effect->Release();
		DL_Effect = 0;
	}

	if ( g_cull_flag_grass_n ) {
		//Wait for thread to finish
		g_cull_flag_grass_n->Wait();
		delete g_cull_flag_grass_n;
		g_cull_flag_grass_n = 0;
	}

	g_vis_sets.ClearAll();

	DistantWorld.Unload();

	if ( ReflectionZBuffer ) {
		ReflectionZBuffer->Release();
		ReflectionZBuffer = 0;
	}

	if ( FakeWaterTexture ) {
		FakeWaterTexture->Release();
		FakeWaterTexture = 0;
	}

	if ( NewWaterTexture ) {
		NewWaterTexture->Release();
		NewWaterTexture = 0;
	}

	if ( DefaultLandTexture ) {
		DefaultLandTexture->Release();
		DefaultLandTexture = 0;
	}

	if ( WaterVB1 ) {
		WaterVB1->Release();
		WaterVB1 = 0;
	}

	if ( WaterIB ) {
		WaterIB->Release();
		WaterIB = 0;
	}

	if(distantReflectionTexture) {
		distantReflectionTexture->Release();
		distantReflectionTexture=0;
	}

	if ( LoLandVB ) {
		LoLandVB->Release();
		LoLandVB = 0;
	}

	if ( LoLandIB ) {
		LoLandIB->Release();
		LoLandIB = 0;
	}
/*
	if(WaterBlendVB) {
		WaterBlendVB->Release();
		WaterBlendVB=0;
	}

	if(WaterBlendTexture) {
		WaterBlendTexture->Release();
		WaterBlendTexture=0;
	}
*/
	if(waveRainTexture) {
		waveRainTexture->Release();
		waveRainTexture=0;
	}

	if(waveRippleTexture) {
		waveRippleTexture->Release();
		waveRippleTexture=0;
	}

	if(waveTempTexture) {
		waveTempTexture->Release();
		waveTempTexture=0;
	}

	UnloadDistantStatics();

	//Free instancing declaration
	if (HWInstDecl) {
		HWInstDecl->Release();
		HWInstDecl = 0;
	}

	WaterLoadedCount = 0;

	//Reset Mesh Collections
	StaticCollection.RemoveAll();
	GrassCollection.RemoveAll();
	MeshGroup::FlushMemoryPool();
	TextureGroup::FlushMemoryPool();

	delete QuadTree::m_node_pool;
	delete QuadTree::m_mesh_pool;
	QuadTree::m_node_pool = 0;
	QuadTree::m_mesh_pool = 0;
	delete DistantWorld.quadtree;
	DistantWorld.quadtree = 0;

	//Reset QuadTrees
	for (size_t i = 0; i < NearQuadTree.size(); ++i ) {
		delete NearQuadTree[i];
	}
	NearQuadTree.clear();
	NearStatics = 0;

	for (size_t i = 0; i < FarQuadTree.size(); ++i ) {
		delete FarQuadTree[i];
	}
	FarQuadTree.clear();
	FarStatics = 0;

	for (size_t i = 0; i < VeryFarQuadTree.size(); ++i ) {
		delete VeryFarQuadTree[i];
	}
	VeryFarQuadTree.clear();
	VeryFarStatics = 0;

	for (size_t i = 0; i < GrassQuadTree.size(); ++i ) {
		delete GrassQuadTree[i];
	}
	GrassQuadTree.clear();
	GrassStatics = 0;

	for (size_t i = 0; i < TreeQuadTree.size(); ++i ) {
		delete TreeQuadTree[i];
	}
	TreeQuadTree.clear();
	TreeStatics = 0;

	CurrentWorldSpace = -1;

	DEBUGSTRING("Distant land shut down complete\n");
}

void DL_BlendNearAndFarView() {
	DECLARE_MWBRIDGE

	if ( !g_DepthManager.IsInitialized() ) {
		return;
	}

	if(!Initialized) {
		if ( !DL_Init() ) return;
	}
	if((mwBridge->IsMenu() || LastFrameWasMenu) && !mwBridge->IsExterior() && !mwBridge->IntLikeExterior()) {
		return;
	}

	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"Blending distant and near views");

	//Begin effect
	UINT passes;
	DL_Effect->Begin(&passes,0);

	//Render full screen quad using the texture that was just created
	DL_Effect->BeginPass(PASS_BLENDDEPTH);

	//DL_Effect->SetMatrix( ehView, view );
	//DL_Effect->SetMatrix( ehProj, proj );
	DL_Effect->SetFloatArray(ehEyePos,(float*)&EyePos,3);
	DL_Effect->SetFloat(ehWaterLevel, mwBridge->WaterLevel());

	float view_dist = min( mwBridge->GetViewDistance(), 7168.0f );

	DL_Effect->SetFloat(ehBlendStart, view_dist - 700.0f );

	DL_Effect->SetFloat(ehBlendEnd, view_dist );
	DL_Effect->SetFloat( ehPixelWidth, PixelWidth );
	DL_Effect->SetFloat( ehPixelHeight, PixelHeight );
	DL_Effect->SetTexture( ehTex0, g_DepthManager.GetDepthTexture() );
	DL_Effect->SetTexture( ehTex3, distantRenderTexture1 );
	DL_Effect->CommitChanges();

	RealDevice->SetStreamSource(0,ShaderStruct.buffer,0,sizeof(MyVertex));
	RealDevice->SetFVF(MYVERTEXFORMAT);
	RealDevice->SetRenderState(D3DRS_CULLMODE,2);
	RealDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	DL_Effect->EndPass();
	DL_Effect->End();

	D3DPERF_EndEvent(); // Blending
}

float GetWindSpeed() {
	DECLARE_MWBRIDGE
	int wthr1 = mwBridge->GetCurrentWeather();
	int wthr2 = mwBridge->GetNextWeather();
	float ratio = mwBridge->GetWeatherRatio();
	if (ratio != 0 && wthr2 >= 0 && wthr2 <= 9) return (DL_Wind[wthr2] * ratio + DL_Wind[wthr1] * (1.0f - ratio));
	else if (wthr1 >= 0 && wthr1 <= 9) return DL_Wind[wthr1];
	return 0.0f;
}

float GetFogFactor() {
	DECLARE_MWBRIDGE
	int wthr1 = mwBridge->GetCurrentWeather();
	int wthr2 = mwBridge->GetNextWeather();
	float ratio = mwBridge->GetWeatherRatio();
	if (ratio != 0 && wthr2 >= 0 && wthr2 <= 9) return (DL_FogD[wthr2] * ratio + DL_FogD[wthr1] * (1.0f - ratio));
	else if (wthr1 >= 0 && wthr1 <= 9) return DL_FogD[wthr1];
	return 1.0f;
}

float GetFogOffset() {
	DECLARE_MWBRIDGE
	int wthr1 = mwBridge->GetCurrentWeather();
	int wthr2 = mwBridge->GetNextWeather();
	float ratio = mwBridge->GetWeatherRatio();
	if (ratio != 0 && wthr2 >= 0 && wthr2 <= 9) return (DL_FgOD[wthr2] * ratio + DL_FgOD[wthr1] * (1.0f - ratio));
	else if (wthr1 >= 0 && wthr1 <= 9) return DL_FgOD[wthr1];
	return 0.0f;
}

void DL_SubmitCullingTasks() {
	if (!(MGEFlags & USE_DISTANT_STATICS) || !UseDistantStatics) {
		return;
	}

	if ( g_cull_flags ) {
		if (DL_WaterReflect > 0) {
			g_cull_flags->static_reflected.Wait();
		}
		g_cull_flags->grass_d.Wait();
		g_cull_flags->static_f.Wait();
		g_cull_flags->static_n.Wait();
		g_cull_flags->static_vf.Wait();

		delete g_cull_flags;
	}
	g_cull_flags = new CullingFlags();

	g_vis_sets.ClearAll();

	if(DL_WaterReflect > 0) {
		//Reflected Statics
		Scheduler.ScheduleTask( new StaticCullTask(
			&g_cull_flags->static_reflected,
			&g_vis_sets.static_reflected,
			&ReflectView,
			&ReflectProj,
			RANGE_REFLECT,
			true
		) );
	}

	//Very Far Statics
	Scheduler.ScheduleTask( new StaticCullTask(
		&g_cull_flags->static_vf,
		&g_vis_sets.static_vf,
		&TransView,
		&WorldProj,
		RANGE_VERY_FAR,
		true
	) );

	//Far Statics
	Scheduler.ScheduleTask( new StaticCullTask(
		&g_cull_flags->static_f,
		&g_vis_sets.static_f,
		&TransView,
		&WorldProj,
		RANGE_FAR,
		true
	) );

	//Near Statics
	Scheduler.ScheduleTask( new StaticCullTask(
		&g_cull_flags->static_n,
		&g_vis_sets.static_n,
		&TransView,
		&WorldProj,
		RANGE_NEAR,
		true
	) );

	//Distant Grass
	Scheduler.ScheduleTask( new GrassCullTask(
		&g_cull_flags->grass_d,
		&g_vis_sets.grass_d,
		&TransView,
		&WorldProj,
		true,
		false,
		false
	) );
}

void DL_SubmitNearGrassCullingTask() {
	if (!(MGEFlags & USE_DISTANT_STATICS) || !UseDistantStatics) {
		return;
	}

	if ( g_cull_flag_grass_n) {
		delete g_cull_flag_grass_n;
	}
	g_cull_flag_grass_n = new ThreadFlag;

	g_vis_set_grass_n.RemoveAll();

	//Near Grass
	Scheduler.ScheduleTask( new GrassCullTask(
		g_cull_flag_grass_n,
		&g_vis_set_grass_n,
		&TransView,
		&TransProj,
		false,
		false,
		false
	) );
}

void DL_RenderLand() {
	if(!Initialized) {
		if ( !DL_Init() ) return;
	}
	if(!MyBackbuffer) {
		//DEBUGSTRING("Distant land render failed because the wrong backbuffer was set\n");
		return;
	}

	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"Distant Land");

	DECLARE_MWBRIDGE
	if (((MGEFlags & NO_INTERIOR_DL) && !mwBridge->IsExterior()) || (!DistantLandActive && !DL_Activate())) return;

	//if (IntCurCelladdr()!=0) { //d4w
	//	if (IntCurCelladdr()!=IntCurCelladdr_old) {
	//		char IntNameArray[64];
	//		IntName(IntNameArray);
	//		Interior_file << IntNameArray << "	" << short(read_byte(IntCurCelladdr() + 0x18))<< "	" << IntLikeExterior() << "	" << IntIllegSleep() << "	" << IntHasWater() << "	" << WaterLevel()
	//					  << "	" << short(IntAmbR()) << "	" << short(IntAmbG()) << "	" << short(IntAmbB())
	//					  << "	" << short(IntSunR()) << "	" << short(IntSunG()) << "	" << short(IntSunB())
	//					  << "	" << short(IntFogR()) << "	" << short(IntFogG()) << "	" << short(IntFogB()) << "	" << IntFogDens() << std::endl;
	//		IntCurCelladdr_old=IntCurCelladdr();
	//	}
	//}

	//Get view distance
	float viewDistance = min( mwBridge->GetViewDistance(), 7168.0f );

	//if(IsMenu()) {
	if (mwBridge->IsMenu() && (mwBridge->IsExterior() || mwBridge->IntLikeExterior())) { //d4w

		if ( abs(lastViewDistance - viewDistance) <= 0.0001f ) {
			//copy the rendering so far to the last distant render surface
			RealDevice->StretchRect(distantRenderSurface1,0,BackBuffer,0,D3DTEXF_NONE);
			LastFrameWasMenu = false;
			return;
		} else {
			lastViewDistance = viewDistance;
		}

	} else if (!mwBridge->IsExterior() && !mwBridge->IntLikeExterior()) {
		//some issue here on menu close causing one frame to be rendered differently
		if(mwBridge->IsMenu()) {
			LastFrameWasMenu = true;
			return;
		}
		else if(LastFrameWasMenu) {
			LastFrameWasMenu = false;
			return;
		}
	}

	//Update water timer
	TotalTime += FrameTime;

	//Update Weather
	D3DXVECTOR2 WindVec = mwBridge->GetWindVector();
	D3DXVec2Normalize(&WindVec, &WindVec);
	SmoothWindSpeed = GetWindSpeed();
	SmoothFogFactor = GetFogFactor();
	SmoothFogOffset = GetFogOffset();

	if ( WeatherRead == false ) {
		SmoothWindVec = WindVec;
		WeatherRead = true;
	} else {
		//Use a moving average to smooth out the weather values so they don't change suddenly
		float BlendFactor = FrameTime / 5.0f;
		if ( BlendFactor > 1.0f ) SmoothWindVec = WindVec;
		else SmoothWindVec = SmoothWindVec * (1.0f - BlendFactor) + WindVec * BlendFactor;
	}

	GrassTime += FrameTime * (SmoothWindSpeed * 2.0f + 0.25f);
	DL_Effect->SetFloat(ehTicks,GrassTime); //moved here from DL_RenderGrass


	DL_Effect->SetFloatArray(ehWindVec,SmoothWindVec * SmoothWindSpeed,2);


	//Record new time


	D3DXMATRIX tempmat;
	if(Zoom||Zoomed) {
		float zf = DL_NearStaticEnd * 3;
		//float zn=2000;
		float zn = viewDistance;
		CellProj=TransProj;
		CellProj._33=zf/(zf-zn);
		CellProj._43=-zn*zf/(zf-zn);
		zn=zf/10;
		//zf=500000;
		zf = DL_DrawDist;
		WorldProj=TransProj;
		WorldProj._33=zf/(zf-zn);
		WorldProj._43=-zn*zf/(zf-zn);
		Zoomed=Zoom;
	}
    SetPos=false;
#ifndef RELEASE
    bool bbb=false;
    if(bbb) {
		LoadDistantStatics();

        //ReloadShader();
    }
#endif
    //Set up shader

	/*DL_Effect->SetFloatArray(ehSunVec,(float*)&SunVec,3);
    DL_Effect->SetFloatArray(ehSunCol,(float*)&SunCol,3);
	D3DXVECTOR4 tAmbCol(SunAmb + AmbCol);
	DL_Effect->SetFloatArray(ehSunAmb,(float*)&tAmbCol,3);
	DL_Effect->SetInt(ehFogCol,FogColour);
	float FogColTmp[3] = { ((FogColour & 0xFF0000)>>16)/255.0f, ((FogColour & 0xFF00)>>8)/255.0f, ((FogColour & 0xFF)>>0)/255.0f };
	DL_Effect->SetFloatArray(ehFogCol2,FogColTmp, 3);
	DL_Effect->SetFloat(ehFogRange, (float)(Options.DL_AboveWaterFogEnd));
	DWORD ticks=GetTickCount();
	float time = float(ticks) / 1000.0f; //Calculate the time in seconds
	DL_Effect->SetFloat(ehTicks,time);
	float anim_length = 5.0f;
	float tex_blend = fmod(time,anim_length/32.0f) * (32.0f/anim_length);
	DL_Effect->SetFloat(ehTexBlend,tex_blend);
	int tex = int(fmod(time/anim_length, 1.0f) * 32.0f);
	DL_Effect->SetTexture( ehTex1,NewWaterTextures[tex] );
	int next_tex = tex + 1;
	if ( next_tex >= 32 ) {
		next_tex = 0;
	}
	DL_Effect->SetTexture( ehTex2,NewWaterTextures[next_tex]);
	DL_Effect->SetTexture(ehTex0,ShaderStruct.lastpass);
	float offset[2] = { -EyePos.x,-EyePos.y };
	DL_Effect->SetFloatArray(ehEyePos,(float*)&EyePos,3);
	DL_Effect->SetFloatArray(ehOffset,(float*)&offset,2);*/

	//Render land
	UINT passes;
	DL_Effect->Begin(&passes,0);
	DL_Effect->BeginPass(PASS_SETUP);
	DL_Effect->EndPass();

	//Set light and fog related shader constants
	DL_Effect->SetFloatArray(ehSunVec,(float*)&SunVec,3);
    DL_Effect->SetFloatArray(ehSunCol,(float*)&SunCol,3);
	D3DXVECTOR4 tAmbCol(SunAmb + AmbCol);
	DL_Effect->SetFloatArray(ehSunAmb,(float*)&tAmbCol,3);
	DL_Effect->SetFloatArray(ehSunPos,(float*)&SunPos,3);//phal
    DL_Effect->SetFloat(ehSunVis,SunVis);//phal

	if (DL_HasStatics()) {
		if (mwBridge->IsAboveWater()) {
			if (mwBridge->CellHasWeather()) {
				DL_Effect->SetFloat(ehFogStart, DistantFogStart());
				DL_Effect->SetFloat(ehFogRange, DistantFogEnd());
			} else {
				DL_Effect->SetFloat(ehFogStart, InteriorFogStart());
				DL_Effect->SetFloat(ehFogRange, InteriorFogEnd());
			}
		} else {
			DL_Effect->SetFloat(ehFogStart, UnderwaterFogStart());
			DL_Effect->SetFloat(ehFogRange, UnderwaterFogEnd());
		}
	} else {
		DL_Effect->SetFloat(ehFogStart, StandardFogStart());
		DL_Effect->SetFloat(ehFogRange, StandardFogEnd());
	}

	//Set some water related things here so they can't change between now and the time the near water is rendered
	float offset[2] = { -EyePos.x,-EyePos.y };
	DL_Effect->SetFloatArray(ehEyePos,(float*)&EyePos,3);
	DL_Effect->SetFloatArray(ehOffset,(float*)&offset,2);
	DL_Effect->SetFloatArray(ehPlayerPos,(float*)&PlayerPos,3);
	DL_Effect->SetFloat (ehCausticIntens, DL_WaterCaustics);
	DL_Effect->CommitChanges();

	if (mwBridge->CellHasWeather()) DL_Effect->SetFloatArray(ehSkyCol, (float*)mwBridge->CurSkyColVector(), 3); // MW real sky color
	else DL_Effect->SetFloatArray(ehSkyCol, (float*)&RGBVECTOR(FogColour), 3);
	DL_Effect->SetInt(ehFogCol, FogColour);
	//DL_Effect->SetFloatArray(ehFogCol2, (float*)CurFogColVector(), 3); // MW real fog color
	DL_Effect->SetFloatArray(ehFogCol2, (float*)&RGBVECTOR(FogColour), 3);

	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Water Reflection Texture");
	//Create reflection texture
	if ( ((MGEFlags & REFLECT_SKY) || DL_WaterReflect > 0) && mwBridge->WaterReflects() && DL_HasStatics()) {
		D3DXVECTOR3 refeye(EyePos.x, EyePos.y, -EyePos.z + 2 * mwBridge->WaterLevel());
		D3DXVECTOR3 lookat(EyeVec.x, EyeVec.y, -EyeVec.z);
		D3DXVECTOR3 up(TransView._12,TransView._22,-TransView._32);
		lookat*=100;
		lookat+=refeye;
		D3DXMatrixLookAtLH(&ReflectView, &refeye, &lookat, &up);

		//RealDevice->SetRenderTarget(0,ShaderStruct.lastpassSurface);
		RealDevice->SetRenderTarget(0,distantReflectionSurface);
		RealDevice->SetDepthStencilSurface(ReflectionZBuffer);

		if((MGEFlags & REFLECT_SKY) && mwBridge->IsAboveWater() && ( mwBridge->IsExterior() || mwBridge->IntLikeExterior() )) {
			if(!mwBridge->IsMenu()) RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET,FogColour,1,0);

			D3DXMATRIX SkyReflectView;
			D3DXVECTOR3 skyrefeye(EyePos.x, EyePos.y, EyePos.z);
			lookat.z = EyePos.z-100*EyeVec.z;
			D3DXMatrixLookAtLH(&SkyReflectView, &skyrefeye, &lookat, &up);

			D3DXMATRIX FogTranslate;
			D3DXMatrixTranslation(&FogTranslate,0,0,-160);

			RealDevice->SetTransform( D3DTS_VIEW, &SkyReflectView );

			//Draw recorded sky renders
			RealDevice->SetRenderState( D3DRS_ZENABLE, 0 );
			RealDevice->SetRenderState( D3DRS_ZWRITEENABLE, 0 );
			RealDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, 1 );
			RealDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			RealDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			RealDevice->SetRenderState( D3DRS_CULLMODE, 1 );

			PooledList<DrawRecorder::RecordedDrawCall>& SkyRenders =
				SkyDrawRecorder.GetList();

			if ( !SkyRenders.IsEmpty() ) {
				bool first_object = true;
				PooledList<DrawRecorder::RecordedDrawCall>::Item* item =
					SkyRenders.GetFirstItem();
				for ( /**/; item != SkyRenders.GetEndMarker(); item = item->next ) {
					DrawRecorder::RecordedDrawCall& ro = *item->data;

					//Prevent over-fogging near horizon by decreasing fog altitude
					if ( first_object ) {
						ro.worldTrans[0] = ro.worldTrans[0] * FogTranslate;
						first_object = false;
					}

					RealDevice->SetMaterial( ro.material );
					RealDevice->SetTexture( 0, ro.texture );
					//RealDevice->SetTexture( 1, ro.secondtexture );
					RealDevice->SetTextureStageState(0, D3DTSS_COLOROP, ro.colorOp);
					RealDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, ro.alphaOp);
					RealDevice->SetTransform( D3DTS_WORLDMATRIX(0), &ro.worldTrans[0] );
					RealDevice->SetFVF( ro.fvf );
					RealDevice->SetStreamSource( 0, ro.vBuffer, 0, ro.streamSpan );
					RealDevice->SetIndices( ro.indices );
					RealDevice->DrawIndexedPrimitive( ro.primType, ro.baseVertexIndex, ro.minIndex, ro.numVertices, ro.startIndex, ro.primitiveCount );
				}
			}
			SkyDrawRecorder.ClearRecordedDrawCalls();

			//Restore old render states
			RealDevice->SetTransform( D3DTS_VIEW, &TransView );
			RealDevice->SetRenderState( D3DRS_CULLMODE, 3 );
			RealDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
			RealDevice->SetTextureStageState(0, D3DTSS_COLOROP, 2);
			RealDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, 1);
		} else {
			DWORD SkyColInt = RGBVECTOR(mwBridge->CurSkyColVector());
			if( mwBridge->IsExterior() || mwBridge->IntLikeExterior() ) {
				RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET,SkyColInt,1,0);
			} else {
				RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET,FogColour,1,0);
			}
			//RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1,0);
		}

		if(DL_WaterReflect > 0) {
			//Use a closer near clipping plane
			ReflectProj=TransProj;
			float zn = 1.0f;
			float zf = DL_DrawDist;
			ReflectProj._33=zf/(zf-zn);
			ReflectProj._43=-zn*zf/(zf-zn);


			////Set clip plane

			//// Base position and normal of the plane
			//D3DXPLANE worldSpacePlane;
			//D3DXVECTOR3 planePoint( 0.0f, 0.0f, 0.0f);
			//D3DXVECTOR3 planeNormal( 0.0f, 0.0f, -1.0f);
			//D3DXPlaneFromPointNormal(&worldSpacePlane, &planePoint, &planeNormal);

			////normalize the plane which is required for the transforms
			//D3DXPLANE tempPlane = worldSpacePlane;
			//D3DXPlaneNormalize(&tempPlane, &tempPlane);

			////transform the plane into view space
			//D3DXPLANE viewSpacePlane, clipSpacePlane;
			//D3DXMATRIX  tempMatrix = ReflectView;
			//D3DXMatrixInverse(&tempMatrix, NULL, &tempMatrix);
			//D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
			//D3DXPlaneTransform(&viewSpacePlane, &tempPlane, &tempMatrix);

			//D3DXPlaneNormalize(&viewSpacePlane, &viewSpacePlane);

			////transform the plane into clip space, or post projection space
			//tempMatrix = ReflectProj;
			//D3DXMatrixInverse(&tempMatrix, NULL, &tempMatrix);
			//D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
			//D3DXPlaneTransform(&clipSpacePlane, &viewSpacePlane, &tempMatrix);

			//D3DXPlaneNormalize(&clipSpacePlane, &clipSpacePlane);

			/*
			//Draw the sky
			RealDevice->SetRenderState( D3DRS_ZENABLE, 0 );
			RealDevice->SetRenderState( D3DRS_ZWRITEENABLE, 0 );
			DL_DrawRecordedRenders( &ReflectView, &ReflectProj );
			*/


			//// Set clip plane
			//RealDevice->SetClipPlane( 0, clipSpacePlane);
			//RealDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 );

		D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Reflected Terrain");
		//Draw the landscape
		if (mwBridge->IsExterior()) {
			DL_Effect->BeginPass(PASS_REFLECTLAND);
			DistantWorld.Draw( &ReflectView, &ReflectProj );
			DL_Effect->EndPass();
		}
		D3DPERF_EndEvent(); //Reflected Terrain

		D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Reflected Statics");
			if ((MGEFlags & USE_DISTANT_STATICS) && DL_WaterReflect > 1 && UseDistantStatics) {
				if( g_cull_flags ) {
					g_cull_flags->static_reflected.Wait();
					RenderDistantStatics( g_vis_sets.static_reflected, &ReflectView, &ReflectProj, RANGE_REFLECT, false );
				}
			}
		}
		D3DPERF_EndEvent(); //Reflected Statics

		//RealDevice->StretchRect(ShaderStruct.lastpassSurface,0,distantReflectionSurface,0,D3DTEXF_NONE); //phal

		//RealDevice->SetRenderTarget(0,BackBuffer);
		//RealDevice->SetDepthStencilSurface(DepthBuffer);

		DL_Effect->SetTexture(ehTex3,distantReflectionTexture);

		////Disable clip plane
		//RealDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0 );
	}

	//Set the fog back to the real sky color
	DL_Effect->SetInt(ehFogCol, FogColour);
	//DL_Effect->SetFloatArray(ehFogCol2, (float*)CurFogColVector(), 3); // MW real fog color
	DL_Effect->SetFloatArray(ehFogCol2, (float*)&RGBVECTOR(FogColour), 3);

	D3DPERF_EndEvent(); // Reflection Texture

	WorldProj=TransProj;
	float zn = 1000.0f;
	float zf = DL_DrawDist;
	WorldProj._33 = zf/(zf-zn);
	WorldProj._43 = -zn*zf/(zf-zn);

	if ( g_DepthManager.IsInitialized() ) {
		//End the standard distant land effect
		DL_Effect->End();

		D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Distant Depth Texture");

		//Set the render target to the depth texture
		g_DepthManager.BeginDepthRenderTarget();

		// could be used to render lowest land plane into depth map,
		// not really needed though and could intersect with very deep sea floor - phal
		//DL_RenderLowestLand( &WorldProj, true );

		//Render the depth of the world mesh

		if ( mwBridge->IsExterior() ) {
			DistantWorld.DrawDepth( &TransView, &WorldProj, &g_DepthManager );

			if (DL_HasStatics()) {
				if(UseDistantStatics && (MGEFlags & USE_DISTANT_STATICS) && (MGEFlags & SHADER_DEPTH)) {
					//render the depth of distant statics to the depth texture
					if( g_cull_flags ) {
						g_DepthManager.BeginStaticDepthRendering(true);

						g_cull_flags->static_vf.Wait();
						RenderDistantStatics( g_vis_sets.static_vf, &TransView, &WorldProj, RANGE_DEPTH, false );
						g_cull_flags->static_f.Wait();
						RenderDistantStatics( g_vis_sets.static_f, &TransView, &WorldProj, RANGE_DEPTH, false );
						g_cull_flags->static_n.Wait();
						RenderDistantStatics( g_vis_sets.static_n, &TransView, &WorldProj, RANGE_DEPTH, false );

						g_DepthManager.EndDepthRendering();
					}
				}

				RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1,0);
			}
		}

		////Render depth of distant statics
		//if(Options.UseDistantStatics&&Options.NearStaticsReflect) {
		//	RenderDistantStatics( &TransView, &WorldProj, RANGE_DEPTH, false );
		//}

		g_DepthManager.EndDepthRenderTarget();

		D3DPERF_EndEvent(); // Distant Depth Texture

		//Restart standard distant land effect
		DL_Effect->Begin(&passes,0);
	}

	//Set the render target back to the back buffer
	RealDevice->SetRenderTarget(0,BackBuffer);
	RealDevice->SetDepthStencilSurface(DepthBuffer);

	//RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1,0);
	if (!LastFrameWasMenu) { //d4w_rev97
		RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1,0);
	}

	if (mwBridge->IsExterior()) { //d4w
		DL_RenderLowestLand( &WorldProj, false );//phal
		//Draw the world mesh
		DL_Effect->BeginPass(PASS_RENDERLAND);

		//RealDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		DistantWorld.Draw( &TransView, &WorldProj );

		//RealDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		DL_Effect->EndPass();
	}

	//Set up a clipping plane for the distant statics
	D3DXPLANE staticClip;

	zn = zn - 700.0f;
	staticClip.a = 0.0f;
	staticClip.b = 0.0f;
	staticClip.c = zf/(zf-zn);
	staticClip.d = -zn*zf/(zf-zn);

	//normalize the plane which is required for the transform
	D3DXPlaneNormalize(&staticClip, &staticClip);

	//transform the plane into clip space, or post projection space
	D3DXMATRIX tempMatrix = WorldProj;
	D3DXMatrixInverse(&tempMatrix, NULL, &tempMatrix);
	D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
	D3DXPlaneTransform(&staticClip, &staticClip, &tempMatrix);

	D3DXPlaneNormalize(&staticClip, &staticClip);


	//If the camera is below the water, draw the distant statics first
	//Blending will not be used, so don't copy the screen between draws
	bool staticsDrawn = false;
	if (true /*!mwBridge->IsAboveWater()*/) {
		D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Distant Statics");
		staticsDrawn = true;
		// Switch to static clip plane
		RealDevice->SetClipPlane( 0, staticClip);
		RealDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 );
		if ((MGEFlags & USE_DISTANT_STATICS) && UseDistantStatics) {
			if( g_cull_flags ) {
				g_cull_flags->static_vf.Wait();
				RenderDistantStatics( g_vis_sets.static_vf, &TransView, &WorldProj, RANGE_VERY_FAR, true );
				g_cull_flags->static_f.Wait();
				RenderDistantStatics( g_vis_sets.static_f, &TransView, &WorldProj, RANGE_FAR, true );
				g_cull_flags->static_n.Wait();
				RenderDistantStatics( g_vis_sets.static_n, &TransView, &WorldProj, RANGE_NEAR, true );
			}
		}

		D3DPERF_EndEvent(); // Distant Statics
	}

	//End effect, water will set up its own
	DL_Effect->End();

	//Draw distant water

	//Set a clip plane to keep the water that is drawn now from overlapping the water that will be drawn later
	D3DXPLANE waterClip;

	zn = viewDistance - 1.0f;
	zf = DL_DrawDist;

	waterClip.a = 0.0f;
	waterClip.b = 0.0f;
	waterClip.c = zf/(zf-zn);
	waterClip.d = -zn*zf/(zf-zn);

	//normalize the plane which is required for the transform
	D3DXPlaneNormalize(&waterClip, &waterClip);

	//transform the plane into clip space, or post projection space
	tempMatrix = WorldProj;
	D3DXMatrixInverse(&tempMatrix, NULL, &tempMatrix);
	D3DXMatrixTranspose(&tempMatrix, &tempMatrix);
	D3DXPlaneTransform(&waterClip, &waterClip, &tempMatrix);

	D3DXPlaneNormalize(&waterClip, &waterClip);

	//tempmat = TransView *WorldProj;
	//ViewFrustum vf(&tempmat);

	// Enable clip plane
	RealDevice->SetClipPlane( 0, waterClip);
	RealDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 );


	//D3DXMatrixTranslation(&WaterWorld,EyePos.x,EyePos.y,0); //moved to DL_RenderWater - phal

	//If statics have already been drawn, draw the grass now
	//Blending will not be used, so don't copy the screen before drawing
	//if ( staticsDrawn ) {
	if (staticsDrawn && UseDistantStatics) {
		//DL_RenderGrass(&TransView, &WorldProj, true, false );//phal
		if ( g_cull_flags) {
			g_cull_flags->grass_d.Wait();
			DL_RenderGrass( &TransView, &WorldProj, true, true, false );
		}
	}

	//DL_RenderWater(&WorldProj, true);
	//if (!LastFrameWasMenu) { //d4w_rev97
	if (!LastFrameWasMenu && DL_HasStatics()) { //phal
		DL_RenderWater(&WorldProj, true);
	}


	//If the distant statics haven't already been drawn, draw them now
	if ( !staticsDrawn) {

		DL_Effect->Begin( &passes, 0 );

		// Switch to static clip plane
		RealDevice->SetClipPlane( 0, staticClip);
		RealDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 );

		//if(Options.UseDistantStatics) {
		if ((MGEFlags & USE_DISTANT_STATICS) && UseDistantStatics) {
			D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Distant Statics");
			if( g_cull_flags ) {
				g_cull_flags->static_vf.Wait();
				RenderDistantStatics( g_vis_sets.static_vf, &TransView, &WorldProj, RANGE_VERY_FAR, true );
				g_cull_flags->static_f.Wait();
				RenderDistantStatics( g_vis_sets.static_f, &TransView, &WorldProj, RANGE_FAR, true );
				g_cull_flags->static_n.Wait();
				RenderDistantStatics( g_vis_sets.static_n, &TransView, &WorldProj, RANGE_NEAR, true );
			}
			D3DPERF_EndEvent();
		}
		DL_Effect->End();

		// Switch to back to water clip plane
		RealDevice->SetClipPlane( 0, waterClip);
		RealDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 );

		//DL_RenderGrass(&TransView, &WorldProj, true, true );
		//if (UseDistantStatics /*&& UseGrass*/) DL_RenderGrass(&TransView, &WorldProj, true, true); //phal
		if (UseDistantStatics /*&& UseGrass*/) {
			if( g_cull_flags ) {
				g_cull_flags->grass_d.Wait();
				DL_RenderGrass( &TransView, &WorldProj, true, true, false);
			}
		}
	}

	//Start culling for the next frame
	DL_SubmitCullingTasks();

	//Disable clip plane
	RealDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0 );

	//Done drawing, prepare to let Morrowind take over

	//Clear z-buffer
	//RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1,0);
	if (!LastFrameWasMenu) { //d4w_rev97
		RealDevice->Clear(0,0,D3DCLEAR_ZBUFFER,0,1,0);
	}

	//Copy what distant land rendered to a texture
	RealDevice->StretchRect(BackBuffer,0,distantRenderSurface1,0,D3DTEXF_NONE);

	//Set fog end based on water height
	if (DL_HasStatics()) {
		if (mwBridge->IsAboveWater()) {
			if (mwBridge->CellHasWeather()) {
				float FR = FogRange * SmoothFogFactor;
				RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(DistantFogStart()));
				RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(DistantFogEnd()));
			} else {
				RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(InteriorFogStart()));
				RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(InteriorFogEnd()));
			}
		} else {
			RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(UnderwaterFogStart()));
			RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(UnderwaterFogEnd()));
		}
	} else {
		RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(StandardFogStart()));
		RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(StandardFogEnd()));
	}

	LastFrameWasMenu=false; //d4w_rev97

	D3DPERF_EndEvent();  //Distant Land
}

void DL_RenderLowestLand( D3DXMATRIX* projection, bool renderdepth ) { //phal
	DECLARE_MWBRIDGE
	if(!DistantLandActive || !mwBridge->IsExterior()) return;

	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"Default Sea Floor");

	static const float zOffset = -2080.0f;

	//Set up transforms
	D3DXMATRIX LowestWorld;

	D3DXMatrixTranslation(&LowestWorld,EyePos.x,EyePos.y,zOffset);

	RealDevice->SetIndices(LoLandIB);
	RealDevice->SetStreamSource(0,LoLandVB,0,SIZEOFWVERT);
	RealDevice->SetFVF(DL_WATERFORMAT);

	//Draw flat land at the lowest possible depth which extends into infinity using water mesh
	if (renderdepth) {

		DL_Effect->End();
		g_DepthManager.SetWorldMatrix(&LowestWorld);
		g_DepthManager.SetViewMatrix(&TransView);
		g_DepthManager.SetProjectionMatrix(projection);
		g_DepthManager.CommitChanges();
		g_DepthManager.BeginStaticDepthRendering(false);

	} else {

		// Texture transform
		D3DXMATRIX T;
		T(0,0) = 0.5f;	T(0,1) =  0.0f;  T(0,2) = 0.0f;  T(0,3) = 0.0f;
		T(1,0) = 0.0f;	T(1,1) = -0.5f;  T(1,2) = 0.0f;  T(1,3) = 0.0f;
		T(2,0) = 0.0f;	T(2,1) =  0.0f;  T(2,2) = 0.5f;  T(2,3) = 0.0f;
		T(3,0) = 0.5f;	T(3,1) =  0.5f;  T(3,2) = 0.5f;  T(3,3) = 1.0f;

		D3DXMATRIX tempmat = LowestWorld*TransView*(*projection)*T;
		DL_Effect->SetMatrix(ehTexProj, &tempmat);

		DL_Effect->SetMatrix(ehWorld,&LowestWorld);
		DL_Effect->SetMatrix(ehView,&TransView);
		DL_Effect->SetMatrix(ehProj,projection);

		//set EyePos to relative position to land plane
		D3DXVECTOR3 relativeEyePos = EyePos;
		relativeEyePos.z -= zOffset;
		DL_Effect->SetFloatArray(ehEyePos,(float*)&relativeEyePos,3);

		DL_Effect->SetTexture( ehTex3,DefaultLandTexture );
		DL_Effect->CommitChanges();

		DL_Effect->BeginPass(PASS_LOWESTLAND);
	}



	RealDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,10);

	if (renderdepth) {
		g_DepthManager.EndDepthRendering();
		UINT passes;
		DL_Effect->Begin( &passes, 0 );
	} else {
		DL_Effect->EndPass();

		//set EyePos back to normal value
		DL_Effect->SetFloatArray(ehEyePos,(float*)&EyePos,3);
	}



	D3DPERF_EndEvent();
}

void DL_RenderWater( D3DXMATRIX* projection, bool distant ) {

	if (!Initialized) {
		return;
	}

	DECLARE_MWBRIDGE
	if (!DistantLandActive || ((MGEFlags & NO_INTERIOR_DL) && !mwBridge->IsExterior()) || !mwBridge->CellHasWater()) return;

	D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"MGE Water");

	//Set vertex format
	RealDevice->SetFVF(DL_WATERFORMAT);

	//Set up shader
	UINT passes;
	DL_Effect->Begin(&passes,0);

	DL_Effect->SetFloatArray(ehSunVec,(float*)&SunVec,3);
	if (!mwBridge->CellHasWeather()) {
		float IntAmb_K; //water color
		float IntSun_K; //fake Sun
		if (mwBridge->IsAboveWater()) { IntSun_K = 0.2f; IntAmb_K = 0.5f; }
		else { IntSun_K = 0.04f; IntAmb_K = 0.1f; }
		DL_Effect->SetFloatArray(ehSunCol, (float*)&RGBVECTOR(
			IntSun_K + (1.0f - IntSun_K) * ((float)mwBridge->IntSunR() / 255.0f),
			IntSun_K + (1.0f - IntSun_K) * ((float)mwBridge->IntSunG() / 255.0f),
			IntSun_K + (1.0f - IntSun_K) * ((float)mwBridge->IntSunB() / 255.0f)),
			3);
		DL_Effect->SetFloatArray(ehSunAmb, (float*)&RGBVECTOR(
			IntAmb_K + (1.0f - IntAmb_K) * (((float)mwBridge->IntAmbR() + (float)mwBridge->IntSunR()) / 255.0f),
			IntAmb_K + (1.0f - IntAmb_K) * (((float)mwBridge->IntAmbG() + (float)mwBridge->IntSunG()) / 255.0f),
			IntAmb_K + (1.0f - IntAmb_K) * (((float)mwBridge->IntAmbB() + (float)mwBridge->IntSunB()) / 255.0f)),
			3);
	} else {
		DL_Effect->SetFloatArray(ehSunCol,(float*)&SunCol,3);
		D3DXVECTOR4 tAmbCol(SunAmb + AmbCol);
		DL_Effect->SetFloatArray(ehSunAmb,(float*)&tAmbCol,3);
	}
	DL_Effect->SetInt(ehFogCol, FogColour);
	//DL_Effect->SetFloatArray(ehFogCol2, (float*)CurFogColVector(), 3); // MW real fog color
	DL_Effect->SetFloatArray(ehFogCol2, (float*)&RGBVECTOR(FogColour), 3);
	if (DL_HasStatics()) {
		if (mwBridge->IsAboveWater()) {
			if (mwBridge->CellHasWeather()) {
				DL_Effect->SetFloat(ehFogStart, DistantFogStart());
				DL_Effect->SetFloat(ehFogRange, DistantFogEnd());
			} else {
				DL_Effect->SetFloat(ehFogStart, InteriorFogStart());
				DL_Effect->SetFloat(ehFogRange, InteriorFogEnd());
			}
		} else {
			DL_Effect->SetFloat(ehFogStart, UnderwaterFogStart());
			DL_Effect->SetFloat(ehFogRange, UnderwaterFogEnd());
		}
	} else {
		DL_Effect->SetFloat(ehFogStart, StandardFogStart());
		DL_Effect->SetFloat(ehFogRange, StandardFogEnd());
	}

	//Render water texture, only once per frame
	if( !mwBridge->IsMenu() && distant == ((!(MGEFlags & NO_INTERIOR_DL) || mwBridge->IsExterior()) && DL_HasStatics()) ) {

		//Save render target
		IDirect3DSurface *oldRenderTarget;
		RealDevice->GetRenderTarget(0, &oldRenderTarget);

		RealDevice->SetFVF(MYVERTEXFORMAT);

		//calculate dynamic ripples
		if(MGEFlags & DYNAMIC_RIPPLES) {
			static bool resetRippleSurface = true;

			RealDevice->SetStreamSource(0,WaveVB,0,sizeof(MyVertex));

			static float remainingWaveTime=0;
			static const float waveStep=0.0167f; // time per wave frame
			remainingWaveTime += (FrameTime>0.5f)?(0.5f):(FrameTime);

			//number of wave iterations
			int numWaveSteps = (int)(remainingWaveTime/waveStep);
			remainingWaveTime -= numWaveSteps*waveStep;

			//rain ripples
			if(mwBridge->CellHasWeather()) {
				//reset surface when not needed next time
				resetRippleSurface = true;

				static double randomizer=0.546372819;
				float ripplePos[2];
				//DL_Effect->SetFloat(ehTicks,min(0.000017f/(FrameTime*FrameTime),0.1f));

				static float remainingRipples=0;

				// rain=4; thunderstorm=5; snow=8; blizzard=9;
				float rippleFrequency =
					(
						((mwBridge->GetCurrentWeather()==4)||(mwBridge->GetCurrentWeather()==5)||(mwBridge->GetCurrentWeather()==8)||(mwBridge->GetCurrentWeather()==9))
						?2.50f
						:0
					) * (1-mwBridge->GetWeatherRatio())
					+
					(
						((mwBridge->GetNextWeather()==4)||(mwBridge->GetNextWeather()==5)||(mwBridge->GetNextWeather()==8)||(mwBridge->GetNextWeather()==9))
						?2.50f
						:0
					) * mwBridge->GetWeatherRatio()
					- 1.50f
				;
				if(rippleFrequency<0) {
					rippleFrequency=0;
				} else {
					rippleFrequency *= 1.0f+0.5f*((mwBridge->GetCurrentWeather()==5)*(1-mwBridge->GetWeatherRatio())+(mwBridge->GetNextWeather()==5)*mwBridge->GetWeatherRatio());
					rippleFrequency *= 150.0f; // 150 drops per second for normal rain

					remainingRipples += rippleFrequency*((FrameTime>0.5f)?(0.5f):(FrameTime));
					RECT drop;

					while(remainingRipples >= 1.0f) {
						//place rain ripple at random location
						for(int i=0;i<2;i++) {
							randomizer = randomizer*(1337.134511337451+(double)rand()*0.0001) + 0.12351523;
							randomizer -= (double)((int)randomizer);
							ripplePos[i] = (float)randomizer*waveTexResolution;
						}

						drop.left = (int)(ripplePos[0]) - 2;
						drop.right = (int)(ripplePos[0]) + 2;
						drop.top = (int)(ripplePos[1]) - 1;
						drop.bottom = (int)(ripplePos[1]) + 1;
						RealDevice->ColorFill(waveRainSurface,&drop,0x80804040);

						drop.left = (int)(ripplePos[0]) - 1;
						drop.right = (int)(ripplePos[0]) + 1;
						drop.top = (int)(ripplePos[1]) - 2;
						drop.bottom = (int)(ripplePos[1]) + 2;
						RealDevice->ColorFill(waveRainSurface,&drop,0x80804040);

						drop.left = (int)(ripplePos[0]) - 1;
						drop.right = (int)(ripplePos[0]) + 1;
						drop.top = (int)(ripplePos[1]) - 1;
						drop.bottom = (int)(ripplePos[1]) + 1;
						RealDevice->ColorFill(waveRainSurface,&drop,0x80800000);

						remainingRipples -= 1;
					}
				}

				RealDevice->SetRenderTarget(0,waveRainSurface);
				DL_Effect->SetTexture( ehTex2,waveRainTexture );

				//apply wave equation numWaveSteps times
				DL_Effect->BeginPass(PASS_WAVE);
				for(int i=0; i<numWaveSteps; i++) RealDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );
				DL_Effect->EndPass();

			} else if(resetRippleSurface) {
				// no weather - clear rain ripples
				RealDevice->ColorFill(waveRainSurface,0,0x80808080);
				resetRippleSurface = false;
			}

			// calculate waves in ripple tex
			RealDevice->SetRenderTarget(0,waveRippleSurface);
			DL_Effect->SetTexture( ehTex2,waveRippleTexture );

			// move ripple texture with player
			// move it aligned with the texture pixels to prevent visible movement
			static int lastXpos = (int)floor(PlayerPos.x/waveTexWorldRes);
			static int lastYpos = (int)floor(PlayerPos.y/waveTexWorldRes);

			int newXpos = (int)floor(PlayerPos.x/waveTexWorldRes);
			int newYpos = (int)floor(PlayerPos.y/waveTexWorldRes);

			int shiftX = newXpos-lastXpos;
			int shiftY = newYpos-lastYpos;

			lastXpos = newXpos;
			lastYpos = newYpos;

			int shiftXp = (shiftX>0)?+shiftX:0;
			int shiftXn = (shiftX<0)?-shiftX:0;
			int shiftYp = (shiftY>0)?+shiftY:0;
			int shiftYn = (shiftY<0)?-shiftY:0;

			//shift texture by (shiftX,shiftY) pixels
			RECT source;
			source.left = 1+shiftXp;
			source.right = waveTexResolution - shiftXn;
			source.top = 1+shiftYp;
			source.bottom = waveTexResolution - shiftYn;

			RECT target;
			target.left = 1+shiftXn;
			target.right = waveTexResolution - shiftXp;
			target.top = 1+shiftYn;
			target.bottom = waveTexResolution - shiftYp;

			RealDevice->ColorFill(waveTempSurface,0,0x80808080);
			RealDevice->StretchRect(waveRippleSurface,&source,waveTempSurface,&target,D3DTEXF_NONE);
			//RealDevice->ColorFill(waveRippleSurface,0,0x80808080);
			RealDevice->StretchRect(waveTempSurface,0,waveRippleSurface,0,D3DTEXF_NONE);

			//create waves around the player
			float rippleOrigin[2];
			if(PlayerPos.z - mwBridge->WaterLevel() < 0 && PlayerPos.z - mwBridge->WaterLevel() > -128.0f*mwBridge->PlayerHeight()) {
				DL_Effect->BeginPass(PASS_PLAYERWAVE);
				for(int i=0; i<numWaveSteps; i++) {
					//interpolate between starting and ending point, so that low framerates do not cause less waves
					rippleOrigin[0] = -(float)i/(float)numWaveSteps/(float)waveTexResolution;
					rippleOrigin[1] = rippleOrigin[0] * shiftY + 0.5f;
					rippleOrigin[0] = rippleOrigin[0] * shiftX + 0.5f;
					DL_Effect->SetFloatArray(ehRippleOrigin,rippleOrigin,2);
					DL_Effect->CommitChanges();
					RealDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );
				}
				DL_Effect->EndPass();
			}

			//apply wave equation numWaveSteps times
			DL_Effect->BeginPass(PASS_WAVE);
			for(int i=0; i<numWaveSteps; i++) RealDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );
			DL_Effect->EndPass();

			// set wave texture world origin
			static float halfWaveTexWorldSize = 0.5f*waveTexWorldRes*waveTexResolution;
			rippleOrigin[0] = lastXpos*waveTexWorldRes - halfWaveTexWorldSize;
			rippleOrigin[1] = lastYpos*waveTexWorldRes - halfWaveTexWorldSize;
			DL_Effect->SetFloatArray(ehRippleOrigin,rippleOrigin,2);

			RealDevice->SetRenderTarget(0,oldRenderTarget);
		}
	}

	DL_Effect->SetTexture( ehTex1,NewWaterTexture );
	DL_Effect->SetTexture( ehTex2,waveRainTexture );
	DL_Effect->SetTexture( ehTex5,waveRippleTexture );


	//DL_Effect->SetTexture( ehTex0, ShaderStruct.lastpass ); //phal
	DL_Effect->SetTexture( ehTex0, distantReflectionTexture ); //phal
	//DL_Effect->SetTexture( ehTex4, depthTexture );

	//float offset[2] = { -EyePos.x,-EyePos.y };
	//DL_Effect->SetFloatArray(ehEyePos,(float*)&EyePos,3);
	//DL_Effect->SetFloatArray(ehOffset,(float*)&offset,2);

	// moved here from DL_RenderLand and added WaterLevel() translation - phal
	D3DXMatrixTranslation(&WaterWorld,EyePos.x,EyePos.y,mwBridge->WaterLevel());

	//Set up transforms
	D3DXMATRIX tempmat = WaterWorld*TransView*(*projection);
	DL_Effect->SetMatrix(ehWorld,&WaterWorld);
	DL_Effect->SetMatrix(ehView,&TransView);
	DL_Effect->SetMatrix(ehProj,projection);

	//set EyePos to relative position to water plane - phal
	D3DXVECTOR3 relativeEyePos = EyePos;
	relativeEyePos.z -= mwBridge->WaterLevel();
	DL_Effect->SetFloatArray(ehEyePos,(float*)&relativeEyePos,3);

	//DL_Effect->SetMatrix(ehWorldViewProj,&tempmat);

	D3DXMATRIX T;
	T(0,0) = 0.5f;	T(0,1) =  0.0f;  T(0,2) = 0.0f;  T(0,3) = 0.0f;
	T(1,0) = 0.0f;	T(1,1) = -0.5f;  T(1,2) = 0.0f;  T(1,3) = 0.0f;
	T(2,0) = 0.0f;	T(2,1) =  0.0f;  T(2,2) = 0.5f;  T(2,3) = 0.0f;
	T(3,0) = 0.5f;	T(3,1) =  0.5f;  T(3,2) = 0.5f;  T(3,3) = 1.0f;

	tempmat = WaterWorld*TransView*(*projection)*T;
	DL_Effect->SetMatrix(ehTexProj, &tempmat);

	//DL_Effect->BeginPass(0);
	//DL_Effect->EndPass();

	DL_Effect->SetTexture( ehTex3,ShaderStruct.thisframe );

	if(DL_WaterCaustics > 0 && !(distant && !(MGEFlags & SHADER_DEPTH)) ) {
		//Copy current back buffer contents to "this frame" surface to use for rendering caustics
		RealDevice->StretchRect(BackBuffer,0,ShaderStruct.thisframeSurface,0,D3DTEXF_NONE);

		if(distant) {
			DL_Effect->SetFloat(ehBlendStart, 0 );
			DL_Effect->SetFloat(ehBlendEnd, -1 );
		} else {
			// Render caustics in the MW rendering range
			float view_dist = min( mwBridge->GetViewDistance(), 7168.0f );
			DL_Effect->SetFloat(ehBlendStart, view_dist - 700.0f );
			DL_Effect->SetFloat(ehBlendEnd, view_dist );
		}

		// Pass depth texture containing both distant land and statics
		if ( g_DepthManager.IsInitialized() ) {
			DL_Effect->SetTexture( ehTex4, g_DepthManager.GetDepthTexture() );
		}

		DL_Effect->CommitChanges();

		D3DPERF_BeginEvent(D3DCOLOR_ARGB(255,255,0,0), L"Rendering caustic lights");

		RealDevice->SetStreamSource(0,ShaderStruct.buffer,0,sizeof(MyVertex));
		RealDevice->SetFVF(MYVERTEXFORMAT);

		DL_Effect->BeginPass(PASS_CAUSTICS);
		RealDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		DL_Effect->EndPass();

		D3DPERF_EndEvent(); // Caustics
	}

	if( g_DepthManager.IsInitialized() && distant ) {
		DL_Effect->SetTexture( ehTex4, g_DepthManager.GetDepthTexture() );
	}

	//Copy current back buffer contents to "this frame" surface to use for refraction
	RealDevice->StretchRect(BackBuffer,0,ShaderStruct.thisframeSurface,0,D3DTEXF_NONE);
	DL_Effect->CommitChanges();

	RealDevice->SetIndices(WaterIB);
	RealDevice->SetStreamSource(0,WaterVB1,0,SIZEOFWVERT);
	RealDevice->SetFVF(DL_WATERFORMAT);

	//Draw the water
	if (mwBridge->IsAboveWater()) {
		if ( ((MGEFlags & REFLECT_SKY) || DL_WaterReflect > 0) && mwBridge->WaterReflects() && DL_HasStatics()) {
			DL_Effect->BeginPass(PASS_SM2WATER);
			RealDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,numWaterVertices,0,numWaterTris);
			DL_Effect->EndPass();
		} else if (mwBridge->CellHasWater()) {
			DL_Effect->BeginPass(PASS_NONREFLECTIVEWATER);
			RealDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,numWaterVertices,0,numWaterTris);
			DL_Effect->EndPass();
		}
	} else {
		DL_Effect->BeginPass(PASS_UNDERWATER);
		RealDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,numWaterVertices,0,numWaterTris);
		DL_Effect->EndPass();
	}

	//End effect
	DL_Effect->End();

	//Restore old EyePos
	DL_Effect->SetFloatArray(ehEyePos,(float*)&EyePos,3);

	D3DPERF_EndEvent();
}
void DL_PlayerPos(float x,float y) {
	if(!Initialized) DL_Init();
    if(SetPos) return;
    SetPos=true;
    short xpos=(short)(floor(x/8192.0f));
    short ypos=(short)(floor(y/8192.0f));
    if(xpos!=OldX||ypos!=OldY) {
        if(TempX==xpos&&TempY==ypos) {
            ChangeCell(xpos,ypos);
        } else {
            TempX=xpos;
            TempY=ypos;
        }
    }
}
bool DL_Activate() {
	DECLARE_MWBRIDGE
	if (!(MGEFlags & USE_DISTANT_LAND) || !UsingDistantLand || DistantLandActive || ((MGEFlags & NO_INTERIOR_DL) && !mwBridge->IsExterior())) return false;
	if (!Initialized) {
		if (!DL_Init()) return false;
	}

	if(WaterLoadedCount<32) {
		DEBUGSTRING("Distantland activation failed because water textures are not yet loaded\n");
		if(DlLoadCount == 1) SETSTATUS("Distant Land will be activated once water is reloaded.");
		DlLoadCount = 2;	//mendres
		return false;
	}

	DistantLandActive=true;
	//Set fog end based on water height
	if (DL_HasStatics()) {
		if (mwBridge->IsAboveWater()) {
			if (mwBridge->CellHasWeather()) {
				RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(DistantFogStart()));
				RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(DistantFogEnd()));
				RealDevice->SetRenderState (D3DRS_FOGDENSITY, F2DW(1/DistantFogEnd ()));
			} else {
				RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(InteriorFogStart()));
				RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(InteriorFogEnd()));
				RealDevice->SetRenderState (D3DRS_FOGDENSITY, F2DW(1/InteriorFogEnd ()));
			}
		} else {
			RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(UnderwaterFogStart()));
			RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(UnderwaterFogEnd()));
			RealDevice->SetRenderState (D3DRS_FOGDENSITY, F2DW(1/UnderwaterFogEnd ()));
		}
	} else {
		RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(StandardFogStart()));
		RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(StandardFogEnd()));
	}
	for (int i = 0; i < 32; ++i) *MorrowWaterPtrs[i] = FakeWaterTexture;
	return true;
}
void DL_Deactivate() {
	if(!DistantLandActive) return;
	DistantLandActive=false;
	RealDevice->SetRenderState(D3DRS_FOGSTART, F2DW(StandardFogStart()));
	RealDevice->SetRenderState(D3DRS_FOGEND, F2DW(StandardFogEnd()));
	for (int i = 0; i < 32; ++i) {
		//if (MorrowWaterPtrs[i]) {
			*MorrowWaterPtrs[i] = RealWaterTextures[i];
		//}
	}
}

//create water and lowest land meshes
int DL_CreateMeshes() {
	DECLARE_MWBRIDGE
	WaterVertex *data;
	WORD *i;

	//Lowest land mesh
	if(FAILED(RealDevice->CreateVertexBuffer(8*SIZEOFWVERT,D3DUSAGE_WRITEONLY,DL_WATERFORMAT,D3DPOOL_DEFAULT,&LoLandVB,0))) {
		return 3;
	}
	mwBridge->ShowProgressBar(++DLProgressBar);
	LoLandVB->Lock(0,0,(void**)&data,0);
	//Outer Square
	data[3].x = 500000.0f;
	data[3].y = 500000.0f;
	data[3].z = -4.0f;
	data[2].x = -500000.0f;
	data[2].y = 500000.0f;
	data[2].z = -4.0f;
	data[1].x = -500000.0f;
	data[1].y = -500000.0f;
	data[1].z = -4.0f;
	data[0].x = 500000.0f;
	data[0].y = -500000.0f;
	data[0].z = -4.0f;
	//Inner square
	data[7].x = 8192.0f;
	data[7].y = 8192.0f;
	data[7].z = -4.0f;
	data[6].x = -8192.0f;
	data[6].y = 8192.0f;
	data[6].z = -4.0f;
	data[5].x = -8192.0f;
	data[5].y = -8192.0f;
	data[5].z = -4.0f;
	data[4].x = 8192.0f;
	data[4].y = -8192.0f;
	data[4].z = -4.0f;

	LoLandVB->Unlock();

	if(FAILED(RealDevice->CreateIndexBuffer(2*30,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&LoLandIB,0))) {
		return 4;
	}
	LoLandIB->Lock(0,0,(void**)&i,0);
	i[0]  = 7;  i[1]  = 6;  i[2]  = 4;
	i[3]  = 6;  i[4]  = 5;  i[5]  = 4;
	i[6]  = 5;  i[7]  = 0;  i[8]  = 4;
	i[9]  = 4;  i[10] = 0;  i[11] = 3;
	i[12] = 7;  i[13] = 4;  i[14] = 3;
	i[15] = 2;  i[16] = 7;  i[17] = 3;
	i[18] = 2;  i[19] = 6;  i[20] = 7;
	i[21] = 2;  i[22] = 1;  i[23] = 6;
	i[24] = 6;  i[25] = 1;  i[26] = 5;
	i[27] = 5;  i[28] = 1;  i[29] = 0;
	LoLandIB->Unlock();
	mwBridge->ShowProgressBar(++DLProgressBar);
	if(fineWaterMesh) {
		//use radial grid for water mesh
		//for vertex shaded water waves

		const static int	gridResR=120;
		const static int	gridResT=180;
		const static float	gridResTStep=6.283185f/(float)gridResT;
		const static float	maxrad = 6400.0f;
		const static float	borderrad = 500000.0f;
		const static float	minrad = 12;
		const static float	radgrow = pow(maxrad/minrad,1/(float)(gridResR-1));

		numWaterVertices = (gridResR+1)*gridResT;
		numWaterTris = 2*(gridResR)*gridResT;

		WaterVertex* data;
		if(FAILED(RealDevice->CreateVertexBuffer(numWaterVertices*SIZEOFWVERT,D3DUSAGE_WRITEONLY,DL_WATERFORMAT,D3DPOOL_DEFAULT,&WaterVB1,0))) {
			return 1;
		}
		WaterVB1->Lock(0,0,(void**)&data,0);

		float rad=0;
		for(int r=0; r<=gridResR; r++) {
			float rad = maxrad*(0.85f*powf((float)r/(float)(gridResR-1),3) + 0.15f*(float)r/(float)(gridResR-1));
			//if(r == 0) rad = 0;
			//else if(r == 1) rad = minrad;
			for(int t=0; t<gridResT; t++) {
				if(r==gridResR) rad = borderrad;
				float angle = gridResTStep*t - r*gridResTStep*0.5f;
				data[gridResT*r+t].x = rad*cosf(angle);
				data[gridResT*r+t].y = rad*sinf(angle);
				data[gridResT*r+t].z = -4.0f;
			}
			//rad *= radgrow;
		}

		WaterVB1->Unlock();

		if(FAILED(RealDevice->CreateIndexBuffer(2*3*numWaterTris,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&WaterIB,0))) {
			return 2;
		}
		WORD* i;
		WaterIB->Lock(0,0,(void**)&i,0);
		for(int r=gridResR-1; r>=0; r--) {
			for(int t=0; t<gridResT; t++) {
				int index = 6*(t + r*gridResT);
				i[index  ] = (r*gridResT)+t;
				i[index+1] = ((r+1)*gridResT)+t;
				i[index+2] = ((r+1)*gridResT)+((t+1)%gridResT);

				i[index+3] = ((r+1)*gridResT)+((t+1)%gridResT);
				i[index+4] = (r*gridResT)+((t+1)%gridResT);
				i[index+5] = (r*gridResT)+t;
			}
		}
		WaterIB->Unlock();

	} else {
		//Simply use loland mesh for water plane
		WaterVB1 = LoLandVB;
		WaterIB = LoLandIB;

		//Make sure that we don't prematurely delete the buffer now that's it's being
		//pointed to by two places
		LoLandVB->AddRef();
		LoLandIB->AddRef();

		numWaterVertices = 8;
		numWaterTris = 10;
	}
	mwBridge->ShowProgressBar(++DLProgressBar);
	return 0;
}