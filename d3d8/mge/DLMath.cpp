#include "DLMath.h"

//-----------------------------------------------------------------------------
// BoundingSphere class
//-----------------------------------------------------------------------------

BoundingSphere::BoundingSphere() {
	center.x = 0.0f;
	center.y = 0.0f;
	center.z = 0.0f;
	radius = 0.0f;
}

//-----------------------------------------------------------------------------

BoundingSphere::BoundingSphere( const BoundingSphere & sphere ) {
	*this = sphere;
}

//-----------------------------------------------------------------------------

BoundingSphere & BoundingSphere::operator=( const BoundingSphere & sphere ) {
	center.x = sphere.center.x;
	center.y = sphere.center.y;
	center.z = sphere.center.z;
	radius = sphere.radius;

	return *this;
}

//-----------------------------------------------------------------------------

BoundingSphere::~BoundingSphere() {};

//-----------------------------------------------------------------------------

BoundingSphere & BoundingSphere::operator+=( const BoundingSphere & rh ) {
	//This code is based from the math described on this page:
	//http://xengine.sourceforge.net/docs/classXEngine_1_1Math_1_1Sphere.html
	//However, the formula for "coef" appears to have had the subtraction
	//reversed.

	//This will be defined as returning the minimum bounding sphere which
	//contains the two spheres being added together.

	BoundingSphere result;
	D3DXVECTOR3 vect = rh.center - center;
	float dist = D3DXVec3Length(&vect);		

	//Check if the sphere centers are almost the same
	if ( dist <= 0.001f ) {
		//Return the larger of the two radii
		if ( rh.radius < radius ) {
			radius = rh.radius;
			center = rh.center;
		}
		return *this;
	}
	
	//Interval for first sphere is [-r0, r0]
	//Interval for  second sphere is [d - r1, d + r1]
	if ( -radius < dist - rh.radius && radius > dist + rh.radius ) {
		//Right hand sphere is contained in this one.
		result.center = center;
		result.radius = radius;
	} else if ( dist - rh.radius < -radius && dist + rh.radius > radius ) {
		//This sphere is contained in right hand one
		result.center = rh.center;
		result.radius = rh.radius;
	} else {
		//One sphere is not contained in the other.

		//radius is (dist + r0 + r1) / 2
		//center is on the line C0 + coef * norm_vect
		//where coef = (dist + r1 - r0) / 2

		result.radius = (dist + radius + rh.radius) / 2.0f;
		D3DXVECTOR3 norm_vect;
		D3DXVec3Normalize(&norm_vect, &vect);

		float coef = (dist + rh.radius - radius) / 2.0f;
		result.center = center + coef * norm_vect;
	}

	center = result.center;
	radius = result.radius;

	return *this;
}

//-----------------------------------------------------------------------------

BoundingSphere BoundingSphere::operator+( const BoundingSphere & rh ) {
	return BoundingSphere(*this) += rh;
}

//-----------------------------------------------------------------------------
// BoundingBox class
//-----------------------------------------------------------------------------

BoundingBox::BoundingBox() {
	corners[0].x = 0.0f;  corners[0].y = 0.0f;  corners[0].z = 0.0f;
	corners[1].x = 0.0f;  corners[1].y = 0.0f;  corners[1].z = 0.0f;
	corners[2].x = 0.0f;  corners[2].y = 0.0f;  corners[2].z = 0.0f;
	corners[3].x = 0.0f;  corners[3].y = 0.0f;  corners[3].z = 0.0f;
	corners[4].x = 0.0f;  corners[4].y = 0.0f;  corners[4].z = 0.0f;
	corners[5].x = 0.0f;  corners[5].y = 0.0f;  corners[5].z = 0.0f;
	corners[6].x = 0.0f;  corners[6].y = 0.0f;  corners[6].z = 0.0f;
	corners[7].x = 0.0f;  corners[7].y = 0.0f;  corners[7].z = 0.0f;
}

//-----------------------------------------------------------------------------

BoundingBox::BoundingBox( const BoundingBox & rh ) {
	*this = rh;
}

//-----------------------------------------------------------------------------

BoundingBox::~BoundingBox() {}

//-----------------------------------------------------------------------------

BoundingBox& BoundingBox::operator=( const BoundingBox & rh ) {
	corners[0].x = rh.corners[0].x;
	corners[0].y = rh.corners[0].y;
	corners[0].z = rh.corners[0].z;

	corners[1].x = rh.corners[1].x;
	corners[1].y = rh.corners[1].y;
	corners[1].z = rh.corners[1].z;

	corners[2].x = rh.corners[2].x;
	corners[2].y = rh.corners[2].y;
	corners[2].z = rh.corners[2].z;

	corners[3].x = rh.corners[3].x;
	corners[3].y = rh.corners[3].y;
	corners[3].z = rh.corners[3].z;

	corners[4].x = rh.corners[4].x;
	corners[4].y = rh.corners[4].y;
	corners[4].z = rh.corners[4].z;

	corners[5].x = rh.corners[5].x;
	corners[5].y = rh.corners[5].y;
	corners[5].z = rh.corners[5].z;

	corners[6].x = rh.corners[6].x;
	corners[6].y = rh.corners[6].y;
	corners[6].z = rh.corners[6].z;

	corners[7].x = rh.corners[7].x;
	corners[7].y = rh.corners[7].y;
	corners[7].z = rh.corners[7].z;

	return *this;
}

//-----------------------------------------------------------------------------

BoundingBox::BoundingBox( const D3DXVECTOR3 & min, const D3DXVECTOR3 & max ) {
	Set( min, max );
}

//-----------------------------------------------------------------------------

void BoundingBox::Set( const D3DXVECTOR3 & min, const D3DXVECTOR3 & max ) {
	corners[0].x = min.x;  corners[0].y = min.y;  corners[0].z = min.z;
	corners[1].x = min.x;  corners[1].y = min.y;  corners[1].z = max.z;
	corners[2].x = min.x;  corners[2].y = max.y;  corners[2].z = min.z;
	corners[3].x = min.x;  corners[3].y = max.y;  corners[3].z = max.z;
	corners[4].x = max.x;  corners[4].y = min.y;  corners[4].z = min.z;
	corners[5].x = max.x;  corners[5].y = min.y;  corners[5].z = max.z;
	corners[6].x = max.x;  corners[6].y = max.y;  corners[6].z = min.z;
	corners[7].x = max.x;  corners[7].y = max.y;  corners[7].z = max.z;
}

//-----------------------------------------------------------------------------

void BoundingBox::Transform( const D3DXMATRIX & m ) {
	D3DXVECTOR4 out;

	D3DXVec3Transform( &out, &corners[0], &m );
	corners[0].x = out.x;  corners[0].y = out.y;  corners[0].z = out.z;

	D3DXVec3Transform( &out, &corners[1], &m );
	corners[1].x = out.x;  corners[1].y = out.y;  corners[1].z = out.z;

	D3DXVec3Transform( &out, &corners[2], &m );
	corners[2].x = out.x;  corners[2].y = out.y;  corners[2].z = out.z;

	D3DXVec3Transform( &out, &corners[3], &m );
	corners[3].x = out.x;  corners[3].y = out.y;  corners[3].z = out.z;

	D3DXVec3Transform( &out, &corners[4], &m );
	corners[4].x = out.x;  corners[4].y = out.y;  corners[4].z = out.z;

	D3DXVec3Transform( &out, &corners[5], &m );
	corners[5].x = out.x;  corners[5].y = out.y;  corners[5].z = out.z;

	D3DXVec3Transform( &out, &corners[6], &m );
	corners[6].x = out.x;  corners[6].y = out.y;  corners[6].z = out.z;

	D3DXVec3Transform( &out, &corners[7], &m );
	corners[7].x = out.x;  corners[7].y = out.y;  corners[7].z = out.z;
}

//-----------------------------------------------------------------------------

//void BoundingBox::Report() const {
//	log_file << "Bounding Box:" << endl;
//	for ( size_t c = 0; c < 8; ++c ) {
//		log_file << "    (" << corners[c].x << ", " << corners[c].y <<
//			", " << corners[c].z << ")" << endl;
//	}
//}

//-----------------------------------------------------------------------------
// ViewFrustum class
//-----------------------------------------------------------------------------

ViewFrustum::ViewFrustum(D3DXMATRIX* viewProj) {
	D3DXMATRIX viewProjection = *viewProj;

	// Near plane 
	frustum[0].a = viewProjection._13;
	frustum[0].b = viewProjection._23;
	frustum[0].c = viewProjection._33;
	frustum[0].d = viewProjection._43;

	// Far plane 
	frustum[1].a = viewProjection._14 - viewProjection._13;
	frustum[1].b = viewProjection._24 - viewProjection._23;
	frustum[1].c = viewProjection._34 - viewProjection._33;
	frustum[1].d = viewProjection._44 - viewProjection._43;

	// Left plane 
	frustum[2].a = viewProjection._14 + viewProjection._11;
	frustum[2].b = viewProjection._24 + viewProjection._21;
	frustum[2].c = viewProjection._34 + viewProjection._31;
	frustum[2].d = viewProjection._44 + viewProjection._41;

	// Right plane 
	frustum[3].a = viewProjection._14 - viewProjection._11;
	frustum[3].b = viewProjection._24 - viewProjection._21;
	frustum[3].c = viewProjection._34 - viewProjection._31;
	frustum[3].d = viewProjection._44 - viewProjection._41;

	// Top plane 
	frustum[4].a = viewProjection._14 - viewProjection._12;
	frustum[4].b = viewProjection._24 - viewProjection._22;
	frustum[4].c = viewProjection._34 - viewProjection._32;
	frustum[4].d = viewProjection._44 - viewProjection._42;

	// Bottom plane 
	frustum[5].a = viewProjection._14 + viewProjection._12;
	frustum[5].b = viewProjection._24 + viewProjection._22;
	frustum[5].c = viewProjection._34 + viewProjection._32;
	frustum[5].d = viewProjection._44 + viewProjection._42;


	// Normalize planes 
	for(int i = 0;i < 6;i++) {
		D3DXPlaneNormalize(&frustum[i], &D3DXPLANE(frustum[i]));
	}
}

//-----------------------------------------------------------------------------

ViewFrustum::Containment ViewFrustum::ContainsSphere(
	const BoundingSphere & sphere ) const {

	float dist[6];

	bool intersects = false;

	dist[0] = D3DXPlaneDotCoord(&frustum[0], &sphere.center);
	if ( dist[0] + sphere.radius < 0 ) { return OUTSIDE; }

	dist[1] = D3DXPlaneDotCoord(&frustum[1], &sphere.center);
	if ( dist[1] + sphere.radius < 0 ) { return OUTSIDE; }

	dist[2] = D3DXPlaneDotCoord(&frustum[2], &sphere.center);
	if ( dist[2] + sphere.radius < 0 ) { return OUTSIDE; }

	dist[3] = D3DXPlaneDotCoord(&frustum[3], &sphere.center);
	if ( dist[3] + sphere.radius < 0 ) { return OUTSIDE; }

	dist[4] = D3DXPlaneDotCoord(&frustum[4], &sphere.center);
	if ( dist[4] + sphere.radius < 0 ) { return OUTSIDE; }

	dist[5] = D3DXPlaneDotCoord(&frustum[5], &sphere.center);
	if ( dist[5] + sphere.radius < 0 ) { return OUTSIDE; }

	if ( fabs(dist[0]) < sphere.radius ) { return INTERSECTS; }
	if ( fabs(dist[1]) < sphere.radius ) { return INTERSECTS; }
	if ( fabs(dist[2]) < sphere.radius ) { return INTERSECTS; }
	if ( fabs(dist[3]) < sphere.radius ) { return INTERSECTS; }
	if ( fabs(dist[5]) < sphere.radius ) { return INTERSECTS; }

	return INSIDE;
}

//-----------------------------------------------------------------------------

ViewFrustum::Containment ViewFrustum::ContainsBox(
	const BoundingBox & box ) const {

	//Test each of the corners of the box against each frustum plane
	for ( size_t f = 0; f < 6; ++f ) {
		size_t outside_count = 0;
		for ( size_t c = 0; c < 8; ++c ) {
			if ( D3DXPlaneDotCoord(&frustum[f], &box.corners[c]) < 0 ) {
				++outside_count;
			}
			if ( outside_count == 8 ) {
				return OUTSIDE;
			}
		}
	}

	//box must be inside.  Note that the box could actually have been rejected
	//by mistake if one of the corners of the frustum penetrates one of its
	//planes, but hopefully this won't be too much of a problem
	return INSIDE;
}

//-----------------------------------------------------------------------------
