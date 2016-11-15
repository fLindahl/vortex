#include "config.h"
#include "foundation/math/line.h"
#include "physicsserver.h"

namespace Physics
{
PhysicsServer::PhysicsServer()
{

}

bool PhysicsServer::Raycast(PhysicsHit& out, const Math::vec4 &position, const Math::vec4 &direction, const float& length)
{
    //Create line from position, in length * direction
    Math::line ray = Math::line(position, direction, length);

    Math::plane plane;
    Math::bbox* box;
    Math::vec4 planeHit;

    Math::point a;
    Math::point b;
    Math::point c;
    Math::point d;



    //for each object, check bbox for collision
    for (auto property : this->properties)
    {
        box = &property->getbbox();

        for (index_t i = 0; i < 6; ++i)
        {
            //get all points on a face
            box->get_quad(i,a,b,c,d);
            plane.constructFromPoints(a, b, c);

            if(ray.Intersect(planeHit, plane))
            {
                //check if hit is within the area.
                if(isPointWithinBounds(planeHit, a,b,c,d, plane.n()))
                {
                    bool contact = false;
                    //Length is the furthest we can hit something
                    float closestDistance = length;
                    float distance;
                    Util::Array<Physics::SurfaceCollider::ColliderFace>& faces = property->getCollider()->GetFaceList();

                    Math::line modelSpaceRay = ray;
                    Math::mat4 invModel = Math::mat4::inverse(property->getModelMatrix());
                    modelSpaceRay.transform(invModel);

                    for (auto face : faces)
                    {
                        plane.constructFromPoints(face.p0, face.p1, face.p2);
                        if(modelSpaceRay.Intersect(planeHit, plane))
                        {
                            if(isPointWithinBounds(planeHit, face.p0,face.p1,face.p2, plane.n()))
                            {
                                distance = (modelSpaceRay.p - planeHit).length3();
                                if(distance < closestDistance)
                                {
                                    closestDistance = distance;
                                    out.point = Math::mat4::transform(planeHit, property->getModelMatrix());
                                    out.surfaceNormal = Math::mat4::transform(plane.n(), Math::mat4::transpose(invModel));
                                    out.object = property;
                                    contact = true;
                                }
                            }
                        }
                    }
                    return contact;
                }
            }
        }
    }

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

}