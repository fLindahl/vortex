#include "config.h"
#include "foundation/math/line.h"
#include "physicsserver.h"
#include "foundation/math/math.h"
#include "convexhullcollider.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/resourceserver.h"
#include "physics/colliderproperty.h"
#include "btBulletDynamicsCommon.h"
#include "physicsdevice.h"
#include "bulletvortexconversion.h"
#include "raycallbacks.h"

namespace Physics
{
PhysicsServer::PhysicsServer()
{
	
}


bool PhysicsServer::Raycast(PhysicsHit& out, const Math::vec4 &position, const Math::vec4 &direction, const float& length)
{
	//Create line from position, in length * direction
	Math::line ray = Math::line(position, direction, length);
	return this->Raycast(out, ray);
}

bool PhysicsServer::Raycast(PhysicsHit& out, const Math::line& ray)
{
	// setup filter
	short int filter = btBroadphaseProxy::AllFilter;
	/*
	short int excludes = excludeSet.GetCollideBits();
	filter &= ~excludes;
	*/
	btDiscreteDynamicsWorld *world = PhysicsDevice::Instance()->dynamicsWorld;
	
	_assert(world);

	btVector3 fromBt = Vortex2BtVector3(ray.start());
	btVector3 toBt = Vortex2BtVector3(ray.end());
	/*
	BulletClosestRayResultCallback cb(fromBt, toBt, excludeSet);
	cb.m_collisionFilterMask = filter;
	cb.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	world->rayTest(fromBt, toBt, cb);
	if (cb.hasHit())
	{
		Math::vector v = Bt2NebVector(cb.m_hitPointWorld);
		Math::vector n = Bt2NebVector(cb.m_hitNormalWorld);
		Ptr<Contact> p = Contact::Create();
		p->SetPoint(v);
		p->SetNormalVector(n);
		p->SetType(Contact::RayCheck);
		PhysicsObject * obj = (PhysicsObject*)cb.m_collisionObject->getUserPointer();
		if (obj)
		{
			PhysicsUserData *user = obj->GetUserData();
			if (user)
				p->SetCollisionObject(user->physicsObject);
		}
		points.Append(p);
	}
	*/
	return false;
}


bool PhysicsServer::isPointWithinBounds(const Math::point& p,
                                        const Math::point& a,
                                        const Math::point& b,
                                        const Math::point& c,
                                        const Math::point& d,
                                        const Math::vec4& surfaceNormal)
{
    const Math::vec4 edgeN = Math::vec4::normalize(b - a);
    const Math::vec4 edgeE = Math::vec4::normalize(c - b);
    const Math::vec4 edgeS = Math::vec4::normalize(d - c);
    const Math::vec4 edgeW = Math::vec4::normalize(a - d);

    const Math::vec4 hitToN = Math::vec4::normalize(a - p);
    const Math::vec4 hitToE = Math::vec4::normalize(b - p);
    const Math::vec4 hitToS = Math::vec4::normalize(c - p);
    const Math::vec4 hitToW = Math::vec4::normalize(d - p);

    const Math::vec4 edgeNormalN = Math::vec4::cross3(edgeN, surfaceNormal);
    const Math::vec4 edgeNormalE = Math::vec4::cross3(edgeE, surfaceNormal);
    const Math::vec4 edgeNormalS = Math::vec4::cross3(edgeS, surfaceNormal);
    const Math::vec4 edgeNormalW = Math::vec4::cross3(edgeW, surfaceNormal);

    if((Math::vec4::dot3(hitToN, edgeNormalN) > 0.0f) &&
        (Math::vec4::dot3(hitToE, edgeNormalE) > 0.0f) &&
        (Math::vec4::dot3(hitToS, edgeNormalS) > 0.0f) &&
        (Math::vec4::dot3(hitToW, edgeNormalW) > 0.0f)
        )
    {
        return true;
    }

    return false;
}

bool PhysicsServer::isPointWithinBounds(const Math::point& p,
                                        const Math::point& a,
                                        const Math::point& b,
                                        const Math::point& c,
                                        const Math::vec4& surfaceNormal)
{
    const Math::vec4 edgeN = Math::vec4::normalize(b - a);
    const Math::vec4 edgeE = Math::vec4::normalize(c - b);
    const Math::vec4 edgeW = Math::vec4::normalize(a - c);

    const Math::vec4 hitToN = Math::vec4::normalize(a - p);
    const Math::vec4 hitToE = Math::vec4::normalize(b - p);
    const Math::vec4 hitToW = Math::vec4::normalize(c - p);

    const Math::vec4 edgeNormalN = Math::vec4::cross3(edgeN, surfaceNormal);
    const Math::vec4 edgeNormalE = Math::vec4::cross3(edgeE, surfaceNormal);
    const Math::vec4 edgeNormalW = Math::vec4::cross3(edgeW, surfaceNormal);

    if((Math::vec4::dot3(hitToN, edgeNormalN) > 0.0f) &&
        (Math::vec4::dot3(hitToE, edgeNormalE) > 0.0f) &&
        (Math::vec4::dot3(hitToW, edgeNormalW) > 0.0f)
        )
    {
        return true;
    }

    return false;
}

void PhysicsServer::AddDynamicEntity(Ptr<Property::Rigidbody> p)
{
	
}

void PhysicsServer::RemoveDynamicEntity(Ptr<Property::Rigidbody> p)
{
	
}

void PhysicsServer::AddStaticEntity(Ptr<Property::Collider> p)
{
	
}

void PhysicsServer::RemoveStaticEntity(Ptr<Property::Collider> p)
{
	
}

Ptr<Physics::BaseCollider> PhysicsServer::LoadCollider(const std::string& meshName, const Physics::ColliderShape& shape)
{
	if (this->collidersByMeshName.count(meshName) > 0)
	{
		//Mesh has already been cooked and we#ll just return that one.
		return this->collidersByMeshName[meshName];
	}
	else
	{
		//We need to create a physics collider and cook the mesh
		if (shape == Physics::ColliderShape::CONVEXHULL)
		{
			Ptr<ConvexHullCollider> coll = ConvexHullCollider::Create();
			coll->SetShape(shape);
			coll->CookMeshData(Render::ResourceServer::Instance()->LoadMesh(meshName.c_str()));
			return coll.downcast<BaseCollider>();
		}
		else
		{
			printf("ERROR: We only support convex hull colliders so far!");
			assert(false);
		}
		return nullptr;
	}

}

}