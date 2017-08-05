#include "config.h"
#include "foundation/math/line.h"
#include "physicsserver.h"
#include "foundation/math/math.h"
#include "surfacecollider.h"
#include "render/properties/graphicsproperty.h"
#include "render/server/resourceserver.h"
#include "fysik/colliderproperty.h"

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
    Math::plane plane;
    Math::vec4 planeHit;

    Math::point a;
    Math::point b;
    Math::point c;
    Math::point d;

    bool contact = false;
    //t = 1.0f is the furthest we can hit something
    float closestDistance = 1.0f;
    // distance is between 0.0f -> 1.0f
    float distance;

    //for each object, check bbox for collision
    for (auto& entity : this->physicsEntities)
    {
		if(ray.IntersectAABB(entity.property->GetOwner()->GetBBox()))
        {
			//TODO: This needs to be different per collider type!
            Util::Array<Physics::ColliderFace>& faces = entity.property.downcast<Property::Collider>()->GetCollider()->GetFaceList();

            Math::line modelSpaceRay = ray;
            Math::mat4 invModel = Math::mat4::inverse(entity.property->GetOwner()->GetTransform());
            modelSpaceRay.transform(invModel);

            for (auto face : faces)
            {
                plane.constructFromPoints(face.p0, face.p1, face.p2);
                if(modelSpaceRay.Intersect(planeHit, plane))
                {
                    if(isPointWithinBounds(planeHit, face.p0,face.p1,face.p2, plane.n()))
                    {
                        distance = planeHit.w();
                        if(distance <= closestDistance)
                        {
                            closestDistance = distance;
                            planeHit.w() = 1.0f;
                            out.point = Math::mat4::transform(planeHit, entity.property->GetOwner()->GetTransform());
                            out.surfaceNormal = Math::mat4::transform(plane.n(), Math::mat4::transpose(invModel));
                            out.object = entity.property->GetOwner();
                            contact = true;
                        }
                    }
                }
            }
        }
    }
	
	return contact;
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

Math::mat4 PhysicsServer::CalculateInertiaTensor(Ptr<BaseCollider> collider, const float& mass)
{
	/*
    switch (collider->GetShape())
    {
        case CAPSULE:
        {
            _error("Inertia tensor CAPSULE not implemented!");
            assert(false);
            return Math::mat4();
        }
        case BOX:
        {
            float d = 0.083333333f * mass;
            Math::vec4 boxExtents = collider->getbbox().maxPoint - collider->getbbox().minPoint;
            float sqX = (boxExtents.x() * boxExtents.x());
            float sqY = (boxExtents.y() * boxExtents.y());
            float sqZ = (boxExtents.z() * boxExtents.z());

            Math::mat4 inertiaTensor = Math::mat4(
                d * (sqY + sqZ), 0, 0, 0,
                0, d * (sqX + sqZ), 0, 0,
                0, 0, d * (sqX + sqY), 0,
                0, 0, 0, 1
            );
            return inertiaTensor;
        }
        case SPHERE:
        {
			_error("Inertia tensor SPHERE not implemented!");
            assert(false);
            return Math::mat4();
        }
        case SURFACE:
        {
            //printf("Inertia tensor SURFACE not implemented!\n");
			//printf("Using box inertia tensor...\n\n");
			//TODO: Calculate specific inertia tensor for mesh
			float d = 0.083333333f * mass;
			Math::vec4 boxExtents = collider->getbbox().maxPoint - collider->getbbox().minPoint;
			float sqX = (boxExtents.x() * boxExtents.x());
			float sqY = (boxExtents.y() * boxExtents.y());
			float sqZ = (boxExtents.z() * boxExtents.z());

			Math::mat4 inertiaTensor = Math::mat4(
				d * (sqY + sqZ), 0, 0, 0,
				0, d * (sqX + sqZ), 0, 0,
				0, 0, d * (sqX + sqY), 0,
				0, 0, 0, 1
				);
			return inertiaTensor;            
        }
        default:
        {
			_error("Invalid Shape! No Inertia Tensor found!");
            assert(false);
            return Math::mat4();
        }
    }
	*/

	float d = 0.083333333f * mass;
	Math::vec4 boxExtents = collider->getbbox().maxPoint - collider->getbbox().minPoint;
	float sqX = (boxExtents.x() * boxExtents.x());
	float sqY = (boxExtents.y() * boxExtents.y());
	float sqZ = (boxExtents.z() * boxExtents.z());

	Math::mat4 inertiaTensor = Math::mat4(
		d * (sqY + sqZ), 0, 0, 0,
		0, d * (sqX + sqZ), 0, 0,
		0, 0, d * (sqX + sqY), 0,
		0, 0, 0, 1
		);
	return inertiaTensor;
}

void PhysicsServer::AddDynamicEntity(Ptr<Property::Rigidbody> p)
{
	uint UID = p->GetOwner()->GetID();
	
	size_t index;
	for (index = 0; index < this->physicsEntities.Size(); index++)
	{
		if (this->physicsEntities[index].entityID == UID)
		{
			_warning("PhysicsServer::AddDynamicEntity() >> Dynamic Entity already added to list!");
			return;
		}
	}

	PhysicsEntity PE;

	PE.entityID = UID;
	PE.property = p.upcast<Game::BaseProperty>();
	PE.type = PhysicsType::Dynamic;
	
	this->physicsEntities.Append(PE);
}

void PhysicsServer::RemoveDynamicEntity(Ptr<Property::Rigidbody> p)
{
	uint UID = p->GetOwner()->GetID();

	size_t index;
	for (index = 0; index < this->physicsEntities.Size(); index++)
	{
		if (this->physicsEntities[index].entityID == UID)
		{
			this->physicsEntities.RemoveIndexSwap(index);
			return;
		}
	}

	_warning("PhysicsServer::RemoveDynamicEntity() >> Tried to remove dynamic entity that could not be found in list!");
	return;
}

void PhysicsServer::AddStaticEntity(Ptr<Property::Collider> p)
{
	uint UID = p->GetOwner()->GetID();

	size_t index;
	for (index = 0; index < this->physicsEntities.Size(); index++)
	{
		if (this->physicsEntities[index].entityID == UID)
		{
			_warning("PhysicsServer::AddStaticEntity() >> Static Entity already added to list!");
			return;
		}
	}

	PhysicsEntity PE;

	PE.entityID = UID;
	PE.property = p.upcast<Game::BaseProperty>();
	PE.type = PhysicsType::Static;

	this->physicsEntities.Append(PE);
}

void PhysicsServer::RemoveStaticEntity(Ptr<Property::Collider> p)
{
	uint UID = p->GetOwner()->GetID();

	size_t index;
	for (index = 0; index < this->physicsEntities.Size(); index++)
	{
		if (this->physicsEntities[index].entityID == UID)
		{
			this->physicsEntities.RemoveIndexSwap(index);
			return;
		}
	}

	_warning("PhysicsServer::RemoveStaticEntity() >> Tried to remove static entity that could not be found in list!");
	return;
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
		if (shape == Physics::ColliderShape::SURFACE)
		{
			Ptr<SurfaceCollider> coll = SurfaceCollider::Create();
			coll->SetShape(shape);
			coll->CookMeshData(Render::ResourceServer::Instance()->LoadMesh(meshName.c_str()));
			return coll.downcast<BaseCollider>();
		}
		else
		{
			printf("ERROR: We only support surface colliders so far!");
			assert(false);
		}
		return nullptr;
	}

}

}