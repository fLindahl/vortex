#pragma once

#include <render/properties/graphicsproperty.h>
#include "surfacecollider.h"
namespace Physics
{
class RigidBody
{
public:
    RigidBody();
    ~RigidBody();

    void applyForce(const Math::vec4& dir, const float& magnitude);
    void applyForceAtPoint(const Math::vec4& dir, const float& magnitude, const Math::point& worldPos);


    void setGraphicsProperty(std::shared_ptr<Render::GraphicsProperty> gp) {this->gProperty = gp;}
    void setCollider(std::shared_ptr<SurfaceCollider> coll);


private:
    friend class PhysicsDevice;

    void initialize(const float& mass, const Math::mat4& bodyInertiaTensor);

    void update(const double& frameTime);
    bool collide();


    void calculateDerivedQuantities();


    //Constant quantities
    float mass;
    float massInv;
    Math::point massCenter;
    Math::mat4 inertiaTensor; // model coordiantes
    Math::mat4 invInertiaTensor; // model coordiantes

    //State Variables
    Math::point position; //Cm on worldspace
    Math::vector acceleration;
    Math::quaternion orientation; //q
    Math::vector linearMomentum; //p
    Math::vector angularMomentum; //L

    //derived quantities
    Math::mat4 R; //orientation matrix
    Math::vec4 linearVelocity; //v
    Math::vec4 angularVelocity; //w
    Math::mat4 transform;
    Math::mat4 invInertiaTensorWorld;

    //accumilative quantities
    Math::vector force;
    Math::vector torque;



    std::shared_ptr<Render::GraphicsProperty> gProperty;
    std::shared_ptr<SurfaceCollider> collider;
};

}