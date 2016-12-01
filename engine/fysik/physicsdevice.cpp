#include "config.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <foundation/util/fixedarray.h>
#include <foundation/math/math.h>
#include "physicsdevice.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/resources/meshresource.h"
#include <list>


namespace Physics
{

int LineCase(Util::Array<SupportPoint>& simplex, Math::point& D)
{
    //LINE CASE

    const SupportPoint& A = simplex[1];
    const SupportPoint& B = simplex[0];

    Math::point AO = -A.pointDiff;
    Math::point AB = B.pointDiff - A.pointDiff;

    //is this ray in the same direction as the origin?
    // is ray.direction to origin > 0?
    if (Math::point::dot3(AB, AO) > 0)
    {
        //new search direction is perpendicular to the edge that points towards the origin
        D = Math::point::cross3(AB, AO);
        D = Math::point::cross3(D, AB);

        SupportPoint nA = A;
        SupportPoint nB = B;

        //simplex = [A, B]
        simplex.Clear();
        simplex.Append(nB);
        simplex.Append(nA);
    }
    else
    {
        D = AO;

        SupportPoint nA = A;
        //simplex = [A]
        simplex.Clear();
        simplex.Append(nA);

    }
    return 0;
}

int TriangleCase(Util::Array<SupportPoint>& simplex, Math::point& D)
{
    // TRIANGLE CASE
    const SupportPoint& A = simplex[2];
    const SupportPoint& B = simplex[1];
    const SupportPoint& C = simplex[0];

    /* This seems redundant
    if (Math::point::equal3_all(A.pointDiff, B.pointDiff) || Math::point::equal3_all(A.pointDiff, C.pointDiff))
    {
        //Area of triangle is 0, this means we cant have an intersection.
        return -1;
    }
    */

    Math::point AO = -A.pointDiff;
    Math::point AB = B.pointDiff - A.pointDiff;
    Math::point AC = C.pointDiff - A.pointDiff;

    //Triangle Normal
    Math::point ABC = Math::vec4::cross3(AB, AC);

    //AB Edge Normal
    Math::point ABxABC = Math::vec4::cross3(AB, ABC);

    //AC Edge Normal
    Math::point ABCxAC = Math::vec4::cross3(ABC, AC);

    if (Math::point::dot3(ABCxAC, AO) > 0)
    {
        if(Math::point::dot3(AC, AO) > 0)
        {
            //it's in AC direction
            D = Math::vec4::cross3(AC, AO);
            D = Math::vec4::cross3(D, AC);

            SupportPoint nA = A;
            SupportPoint nC = C;

            //simplex = [A, C]
            simplex.Clear();
            simplex.Append(nC);
            simplex.Append(nA);
        }
        else
        {
            //Magical Star case
            if (Math::point::dot3(AB, AO) > 0)
            {
                //new search direction is perpendicular to the edge that points towards the origin
                D = Math::point::cross3(AB, AO);
                D = Math::point::cross3(D, AB);

                SupportPoint nA = A;
                SupportPoint nB = B;

                //simplex = [A, B]
                simplex.Clear();
                simplex.Append(nB);
                simplex.Append(nA);
            }
            else
            {
                D = AO;
                SupportPoint nA = A;

                //simplex = [A]
                simplex.Clear();
                simplex.Append(nA);
            }
        }
    }
    else
    {
        if (Math::point::dot3(ABxABC, AO) > 0)
        {
            //Magical Star case
            if (Math::point::dot3(AB, AO) > 0)
            {
                //new search direction is perpendicular to the edge that points towards the origin
                D = Math::point::cross3(AB, AO);
                D = Math::point::cross3(D, AB);

                SupportPoint nA = A;
                SupportPoint nB = B;

                simplex.Clear();
                simplex.Append(nB);
                simplex.Append(nA);
            }
            else
            {
                D = AO;

                SupportPoint nA = A;

                simplex.Clear();
                simplex.Append(nA);
            }
        }
        else
        {
            //origin is within the triangle.
            if(Math::point::dot3(ABC, AO) > 0)
            {
                //New search direction is the triangle normal
                D = ABC;

                SupportPoint nA = A;
                SupportPoint nB = B;
                SupportPoint nC = C;

                //simplex = [A, B, C]
                simplex.Clear();
                simplex.Append(nC);
                simplex.Append(nB);
                simplex.Append(nA);
            }
            else
            {
                //New search direction is the reverse normal of the triangle
                D = -ABC;

                SupportPoint nA = A;
                SupportPoint nB = B;
                SupportPoint nC = C;

                //simplex = [A, C, B]
                simplex.Clear();
                simplex.Append(nB);
                simplex.Append(nC);
                simplex.Append(nA);

            }
        }
    }
    return 0;
}

int TetrahedronCase(Util::Array<SupportPoint>& simplex, Math::point& D)
{
    //1. Check which of three faces that's closest to origin
    //2. do the triangle case.

    const SupportPoint& a = simplex[3];
    const SupportPoint& b = simplex[2];
    const SupportPoint& c = simplex[1];
    const SupportPoint& d = simplex[0];

    /* This seems redundant
    if (Math::point::equal3_all(a.pointDiff, b.pointDiff) ||
        Math::point::equal3_all(a.pointDiff, c.pointDiff) ||
        Math::point::equal3_all(a.pointDiff, d.pointDiff) ||
        Math::point::equal3_all(b.pointDiff, c.pointDiff) ||
        Math::point::equal3_all(b.pointDiff, d.pointDiff) ||
        Math::point::equal3_all(c.pointDiff, d.pointDiff)
        )
    {
        //volume is 0, this means we cant have an intersection.
        return -1;
    }
    */

    Math::point AO = -a.pointDiff;
    Math::point AB = b.pointDiff - a.pointDiff;
    Math::point AC = c.pointDiff - a.pointDiff;
    Math::point AD = d.pointDiff - a.pointDiff;

    Math::point ABC = Math::vector::cross3(AB, AC);
    Math::point ACD = Math::vector::cross3(AC, AD);
    Math::point ADB = Math::vector::cross3(AD, AB);

    if (Math::point::dot3(ABC, AO) < 0)
    {
        if(Math::point::dot3(ADB, AO) < 0)
        {
            if(Math::point::dot3(ACD, AO) < 0)
            {
                //intersection
                return 1;
            }
            else
            {
                // origin is behind ACD.
                // do the triangle case on ACD
                //simplex = [A, C, D];

                //D = ACD;

                SupportPoint nA = a;
                SupportPoint nC = c;
                SupportPoint nD = d;

                simplex.Clear();
                simplex.Append(nD);
                simplex.Append(nC);
                simplex.Append(nA);
            }
        }
        else
        {
            // origin is behind ADB.
            // do the triangle case on ADB
            //points = [A, D, B];
            //D = ADB;

            SupportPoint nA = a;
            SupportPoint nB = b;
            SupportPoint nD = d;

            simplex.Clear();
            simplex.Append(nB);
            simplex.Append(nD);
            simplex.Append(nA);
        }
    }
    else
    {
        // origin is in direction of ABC.
        // do the triangle case on ABC
        //points = [A, B, C];
        //D = ABC;

        SupportPoint nA = a;
        SupportPoint nB = b;
        SupportPoint nC = c;

        simplex.Clear();
        simplex.Append(nC);
        simplex.Append(nB);
        simplex.Append(nA);
    }


    return TriangleCase(simplex, D);
    // We might've missed AB, AC, and AD Voronoi regions
}

PhysicsDevice::PhysicsDevice()
{
    time = glfwGetTime();
}

void PhysicsDevice::Solve()
{
    //-----TIME-----
    double currentTime = glfwGetTime();
    this->frameTime = currentTime - time;
    //--------------

    for(auto rigidbody : this->rigidBodies)
    {
		rigidbody->applyForce(Math::vec4(0.0f, -1.0f, 0.0f, 0.0f), 0.01f);
        rigidbody->update(this->frameTime);
    }

    BroadPhase();
    NarrowPhase();

    //-----TIME-----
    this->time = currentTime;
    //--------------
}
void PhysicsDevice::AddRigidBody(std::shared_ptr<RigidBody> rBody)
{
    this->rigidBodies.Append(rBody);
}

bool PhysicsDevice::GJKEPA(Game::PhysicsEntity* E1, Game::PhysicsEntity* E2, PhysicsCollision& collisionData)
{
    //initialize vars
    //Start point
    Math::mat4 mat = Math::mat4::inverse(E1->GetGraphicsProperty()->getModelMatrix());

    //Point list
    Util::Array<SupportPoint> simplex = Util::Array<SupportPoint>();
    simplex.Reserve(5);// Reserve 5 because we want 4 but don't want to grow our list...

    Math::point pointA = Support(Math::vec4(0.0f,1.0f,0.0f,1.0f), E1);
    Math::point pointB = Support(Math::vec4(0.0f,-1.0f,0.0f,1.0f), E2);

    Math::point diff = pointA - pointB;

    SupportPoint S;

    S.pointDiff = diff;
    S.pointA = pointA;

    simplex.Append(S);

    //Initial direction
    Math::point D = -diff;

    //Max iterations will be the largest list of vertices considering that we should probably converge faster than we run out of vertices.
    size_t maxIterations = Math::max(E1->GetGraphicsProperty()->getModelInstance()->GetMesh()->OBJvertexBuffer.Size(), E2->GetGraphicsProperty()->getModelInstance()->GetMesh()->OBJvertexBuffer.Size());

    bool collision = false;

    size_t i;
    for (i = 0; i < maxIterations; ++i)
    {
        // new point is the point furthest away in direction towards origin.
        pointA = Support(D, E1);
        pointB = Support(-D, E2);
        diff = pointA - pointB;

        // if the dotproduct of A with our search direction is less than zero, we can immediately say our shape does not contain origin, thus our objects are not intersecting
        if (Math::vec4::dot3(diff, D) < 0)
        {
            return false;
        }

        S.pointDiff = diff;
        S.pointA = pointA;

        simplex.Append(S);

        int res = DoSimplex(simplex, D);
        if(res == 1)
        {
            collision = true;
            break;
        }
        else if(res == -1)
        {
            return false;
        }
        else
        {
            continue;
        }
    }

    if(i >= maxIterations)
    {
        return false;
    }

    //We have a collision.
    //Use EPA to extrapolate collision depth, collision normal and contact point

    // list because we will be removing and adding a lot
    std::list<Triangle> triangles;
    std::list<Edge> edges;

    // Add edge lambda function. Consider moving this to a static function as we won't ever change this.
    auto addEdge = [&](const SupportPoint &a,const SupportPoint &b)->void
    {
        for(auto it = edges.begin(); it != edges.end(); it++)
        {
            if(it->points[0] == b && it->points[1] == a)
            {
                //opposite edge found, remove it and do not add new one
                edges.erase(it);
                return;
            }
        }
        edges.emplace_back(a,b);
    };

    // add the GJK simplex triangles to the list
    triangles.emplace_back(simplex[0], simplex[1], simplex[2]);
    triangles.emplace_back(simplex[0], simplex[2], simplex[3]);
    triangles.emplace_back(simplex[0], simplex[3], simplex[1]);
    triangles.emplace_back(simplex[1], simplex[3], simplex[2]);

    while(true)
    {
        // find closest triangle to origin
        std::list<Triangle>::iterator entry_cur_triangle_it = triangles.begin();
        float entry_cur_dst = FLT_MAX;
        for(auto it = triangles.begin(); it != triangles.end(); it++)
        {
            float dst = (float)fabs(Math::point::dot3(it->faceNormal, it->points[0].pointDiff));
            if(dst < entry_cur_dst)
            {
                entry_cur_dst = dst;
                entry_cur_triangle_it = it;
            }
        }

        // get the next support point in front of the current triangle, away from the origin
        pointA = Support(entry_cur_triangle_it->faceNormal, E1);
        pointB = Support(-entry_cur_triangle_it->faceNormal, E2);
        diff = pointA - pointB;

        //S is our new support point
        S.pointDiff = diff;
        S.pointA = pointA;

        if((Math::vec4::dot3(entry_cur_triangle_it->faceNormal, S.pointDiff) - entry_cur_dst < 0.001f))
        {
            // Find Contact point
            // calculate the barycentric coordinates of the closest triangle with respect to
            // the projection of the origin onto the triangle
            float bary_u,bary_v,bary_w;
            Math::barycentric(entry_cur_triangle_it->faceNormal * entry_cur_dst,
                        entry_cur_triangle_it->points[0].pointDiff,
                        entry_cur_triangle_it->points[1].pointDiff,
                        entry_cur_triangle_it->points[2].pointDiff,
                        bary_u,
                        bary_v,
                        bary_w);

            // collision point on object a in world space
            Math::point wcolpoint((entry_cur_triangle_it->points[0].pointA * bary_u)+
                                  (entry_cur_triangle_it->points[1].pointA * bary_v)+
                                  (entry_cur_triangle_it->points[2].pointA * bary_w)
                                 );

            // collision normal
            Math::point wcolnormal = -entry_cur_triangle_it->faceNormal;

            // penetration depth
            float wpendepth = entry_cur_dst;

            collisionData.point = wcolpoint;
            collisionData.normal = wcolnormal;
            collisionData.penetrationDepth = wpendepth;

            return true;

        }

        for(auto it = triangles.begin(); it != triangles.end();)
        {
            // can this face be "seen" by S?
            if(Math::vec4::dot3(it->faceNormal, (S.pointDiff - it->points[0].pointDiff)) > 0)
            {
                addEdge(it->points[0],it->points[1]);
                addEdge(it->points[1],it->points[2]);
                addEdge(it->points[2],it->points[0]);
                it = triangles.erase(it);
                continue;
            }
            it++;
        }

        // create new triangles from the edges in the edge list
        for(auto it = edges.begin(); it != edges.end(); it++) {
            triangles.emplace_back(S, it->points[0], it->points[1]);
        }

        edges.clear();
    }
}

int PhysicsDevice::DoSimplex(Util::Array<SupportPoint>& simplex, Math::point& D)
{

    switch (simplex.Size())
    {
        case 2:
        {
            return LineCase(simplex, D);
        }
        case 3:
        {
            return TriangleCase(simplex, D);
        }
        case 4:
        {
            return TetrahedronCase(simplex, D);
        }
        default:
        {
            //We only have a point. This shouldn't happen!
            return 0;
        }

    }
}

int cmpAABBs(const void* a, const void* b)
{
    const int axis = PhysicsDevice::Instance()->GetCurrentAABBSortAxis();
    float minA = (*(Game::PhysicsEntity**)a)->GetGraphicsProperty()->getbbox().minPoint[axis];
    float minB = (*(Game::PhysicsEntity**)b)->GetGraphicsProperty()->getbbox().minPoint[axis];

    if(minA < minB) return -1;
    if(minA > minB) return 1;
    return 0;
}

void PhysicsDevice::BroadPhase()
{
    Util::Array<Game::PhysicsEntity*>& physicsEntities = Physics::PhysicsServer::Instance()->physicsEntities;
    PCEntities.Clear();

    std::qsort(&physicsEntities[0], physicsEntities.Size(), sizeof(Game::PhysicsEntity*), cmpAABBs);

    float sum[3] = {0.0f, 0.0f, 0.0f};
    float sqsum[3] = {0.0f, 0.0f, 0.0f};
    float v[3];

    for (int i = 0; i < physicsEntities.Size(); ++i)
    {
        Math::point p = (physicsEntities[i]->GetGraphicsProperty()->getbbox().minPoint + physicsEntities[i]->GetGraphicsProperty()->getbbox().maxPoint) * 0.5f;

        for (int c = 0; c < 3; ++c)
        {
            sum[c] += p[c];
            sqsum[c] += p[c] * p[c];
        }

        for (int j = i+1; j < physicsEntities.Size(); j++)
        {
            if((physicsEntities[j]->GetGraphicsProperty()->getbbox().minPoint[sortAxis] > physicsEntities[i]->GetGraphicsProperty()->getbbox().maxPoint[sortAxis]))
            {
                break;
            }
            if(physicsEntities[i]->GetGraphicsProperty()->getbbox().intersects(physicsEntities[j]->GetGraphicsProperty()->getbbox()))
            {
                this->PCEntities.Append(std::make_pair(physicsEntities[i], physicsEntities[j]));
            }
        }
    }

    // Compute Variance
    const float div = 1 / physicsEntities.Size();
    v[0] = sqsum[0] - sum[0] * sum[0] * div;
    v[1] = sqsum[1] - sum[1] * sum[1] * div;
    v[2] = sqsum[2] - sum[2] * sum[2] * div;

    // update axis sorted to be the one with greatest AABB variance
    this->sortAxis = 0;

    if (v[1] > v[0])
        this->sortAxis = 1;
    if (v[2] > v[this->sortAxis])
        this->sortAxis = 2;
}

void PhysicsDevice::NarrowPhase()
{
    this->hasCollision = false;
    PhysicsCollision collData;
    for(auto pair : this->PCEntities)
    {
        if(GJKEPA(pair.first, pair.second, collData))
        {
            Game::PhysicsEntity* E1 = pair.first;
            Game::PhysicsEntity* E2 = pair.second;

            Game::RigidBodyEntity* rbe = dynamic_cast<Game::RigidBodyEntity*>(E1);

            if(rbe != nullptr)
                rbe->GetRigidBody()->applyForceAtPoint(collData.normal, 0.1f, collData.point);

            rbe = dynamic_cast<Game::RigidBodyEntity*>(E2);

            if(rbe != nullptr)
                rbe->GetRigidBody()->applyForceAtPoint(-collData.normal, 0.1f, collData.point);

            this->hasCollision = true;
        }
    }
}

Math::point PhysicsDevice::Support(const Math::point &dir, Game::PhysicsEntity *entity)
{
    Math::mat4 rot = entity->GetGraphicsProperty()->getModelMatrix();
    rot.set_position(Math::vec4(0.0f,0.0f,0.0f,1.0f));

    Math::point D = Math::mat4::transform(dir, Math::mat4::inverse(rot));

    Util::Array<Render::MeshResource::OBJVertex>& vertbuffer = entity->GetGraphicsProperty()->getModelInstance()->GetMesh()->OBJvertexBuffer;
    Math::point p = Math::point(vertbuffer[0].pos[0], vertbuffer[0].pos[1], vertbuffer[0].pos[2]);
    float max = Math::vector::dot3(p, D);

    Math::point temp;

    for(int i = 1; i < vertbuffer.Size(); ++i)
    {
        temp = Math::point(vertbuffer[i].pos[0], vertbuffer[i].pos[1], vertbuffer[i].pos[2]);
        float t = Math::vector::dot3(temp, D);
        if(t >= max)
        {
            p = temp;
            max = t;
        }
    }

    return Math::mat4::transform(p, entity->GetGraphicsProperty()->getModelMatrix());
}

}