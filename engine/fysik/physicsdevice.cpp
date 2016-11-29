#include "config.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <foundation/util/fixedarray.h>
#include <foundation/math/math.h>
#include "physicsdevice.h"
#include "render/properties/graphicsproperty.h"
#include "render/resources/modelinstance.h"
#include "render/resources/meshresource.h"

namespace Physics
{

int LineCase(Util::Array<Math::point>& simplex, Math::point& D)
{
    //LINE CASE

    const Math::point& A = simplex[1];
    const Math::point& B = simplex[0];

    Math::point AO = -A;
    Math::point AB = B - A;

    //is this ray in the same direction as the origin?
    // is ray.direction to origin > 0?
    if (Math::point::dot3(AB, AO) > 0)
    {
        //new search direction is perpendicular to the edge that points towards the origin
        D = Math::point::cross3(AB, AO);
        D = Math::point::cross3(D, AB);

        Math::point nA = A;
        Math::point nB = B;

        //simplex = [A, B]
        simplex.Clear();
        simplex.Append(nB);
        simplex.Append(nA);
    }
    else
    {
        D = AO;

        Math::point nA = A;
        //simplex = [A]
        simplex.Clear();
        simplex.Append(nA);

    }
    return 0;
}

int TriangleCase(Util::Array<Math::point>& simplex, Math::point& D)
{
    // TRIANGLE CASE
    const Math::point& A = simplex[2];
    const Math::point& B = simplex[1];
    const Math::point& C = simplex[0];

    if (Math::point::equal3_all(A, B) || Math::point::equal3_all(A, C))
    {
        //Area of triangle is 0, this means we cant have an intersection.
        return -1;
    }

    Math::point AO = -A;
    Math::point AB = B - A;
    Math::point AC = C - A;

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

            Math::point nA = A;
            Math::point nC = C;

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

                Math::point nA = A;
                Math::point nB = B;

                //simplex = [A, B]
                simplex.Clear();
                simplex.Append(nB);
                simplex.Append(nA);
            }
            else
            {
                D = AO;
                Math::point nA = A;

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

                Math::point nA = A;
                Math::point nB = B;

                simplex.Clear();
                simplex.Append(nB);
                simplex.Append(nA);
            }
            else
            {
                D = AO;

                Math::point nA = A;

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

                Math::point nA = A;
                Math::point nB = B;
                Math::point nC = C;

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

                Math::point nA = A;
                Math::point nB = B;
                Math::point nC = C;

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

int TetrahedronCase(Util::Array<Math::point>& simplex, Math::point& D)
{
    //1. Check which of three faces that's closest to origin
    //2. do the triangle case.

    const Math::point& a = simplex[3];
    const Math::point& b = simplex[2];
    const Math::point& c = simplex[1];
    const Math::point& d = simplex[0];

    if (Math::point::equal3_all(a, b) ||
        Math::point::equal3_all(a, c) ||
        Math::point::equal3_all(a, d) ||
        Math::point::equal3_all(b, c) ||
        Math::point::equal3_all(b, d) ||
        Math::point::equal3_all(c, d)
        )
    {
        //volume is 0, this means we cant have an intersection.
        return -1;
    }

    Math::point AO = -a;
    Math::point AB = b - a;
    Math::point AC = c - a;
    Math::point AD = d - a;

    Math::point ABC = Math::vector::cross3(AB, AC);
    Math::point ACD = Math::vector::cross3(AC, AD);
    Math::point ADB = Math::vector::cross3(AD, AB);


    // SIGN (A == 0 ? 0 : (A < 0 ? -1 : 1));

    float B_on_ACD = Math::vec4::dot3(ACD, AB);
    float C_on_ADB = Math::vec4::dot3(ADB, AC);
    float D_on_ABC = Math::vec4::dot3(ABC, AD);

    /*
    // whether origin is on same side of ACD, ADB, ABC as B, C, D
    // respectively
    bool AB_O = Math::vec4::dot3(ACD, AO) > B_on_ACD;
    bool AC_O = Math::vec4::dot3(ADB, AO) > C_on_ADB;
    bool AD_O = Math::vec4::dot3(ABC, AO) > D_on_ABC;

    if(AB_O && AC_O && AD_O)
    {
        return true;
    }
    else if(!AB_O)
    {
        Math::point nA = a;
        Math::point nC = c;
        Math::point nD = d;

        simplex.Clear();
        simplex.Append(nD);
        simplex.Append(nC);
        simplex.Append(nA);
    }
    else if(!AC_O)
    {
        Math::point nA = a;
        Math::point nB = b;
        Math::point nD = d;

        simplex.Clear();
        simplex.Append(nB);
        simplex.Append(nD);
        simplex.Append(nA);
    }
    else
    {
        Math::point nA = a;
        Math::point nB = b;
        Math::point nC = c;

        simplex.Clear();
        simplex.Append(nC);
        simplex.Append(nB);
        simplex.Append(nA);
    }
*/

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

                Math::point nA = a;
                Math::point nC = c;
                Math::point nD = d;

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

            Math::point nA = a;
            Math::point nB = b;
            Math::point nD = d;

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

        Math::point nA = a;
        Math::point nB = b;
        Math::point nC = c;

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

bool PhysicsDevice::GJK(Game::PhysicsEntity* E1, Game::PhysicsEntity* E2)
{
    //initialize vars
    //Start point
    Math::mat4 mat = Math::mat4::inverse(E1->GetGraphicsProperty()->getModelMatrix());

    Math::point S = Support(Math::vec4(0.0f,1.0f,0.0f,1.0f), E2) - Support(Math::vec4(0.0f,-1.0f,0.0f,1.0f), E1);

    //Point list
    Util::Array<Math::point> simplex = Util::Array<Math::point>();
    simplex.Reserve(5);// Reserve 5 because we want 4 but don't want to grow our list...

    simplex.Append(S);

    //Initial direction
    Math::point D = -S;

    //Our variable point.
    Math::point A;

    //Max iterations will be the largest list of vertices considering that we should probably converge faster than we run out of vertices.
    size_t maxIterations = Math::max(E1->GetGraphicsProperty()->getModelInstance()->GetMesh()->OBJvertexBuffer.Size(), E2->GetGraphicsProperty()->getModelInstance()->GetMesh()->OBJvertexBuffer.Size());

    for (size_t i = 0; i < maxIterations; ++i)
    {
        //D = Math::point::transform(D, Math::mat4::inverse(E1->GetGraphicsProperty()->getModelMatrix()));
        // new point is the point furthest away in direction towards origin.
        A = Support(D, E2) - Support(-D, E1);

        // if the dotproduct of A with our search direction is less than zero, we can immediately say our shape does not contain origin, thus our objects are not intersecting
        if (Math::vec4::dot3(A, D) < 0)
        {
            return false;
        }

        simplex.Append(A);

        int res = DoSimplex(simplex, D);
        if(res == 1)
        {
            return true;
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

    return false;
}

int PhysicsDevice::DoSimplex(Util::Array<Math::point>& simplex, Math::point& D)
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
    for(auto pair : this->PCEntities)
    {
        if(GJK(pair.first, pair.second))
        {
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