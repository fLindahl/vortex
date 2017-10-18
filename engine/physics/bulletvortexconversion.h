#pragma once

#include "foundation/math/vector.h"
#include "foundation/math/point.h"
#include "foundation/math/matrix4.h"
#include "btBulletCollisionCommon.h"

//------------------------------------------------------------------------------
/**
    Bullet to Vortex
*/
Math::vector    Bt2VortexVector(const btVector3 &btV);
Math::point     Bt2VortexPoint(const btVector3 &btV);
Math::mat4		Bt2VortexMatrix3x3(const btMatrix3x3 &m);
Math::mat4		Bt2VortexTransform(const btTransform &t);

//------------------------------------------------------------------------------
/**
    Vortex to Bullet
*/
btMatrix3x3     Vortex2BtM33(const Math::mat4 &m);
btTransform     Vortex2BtTransform(const Math::mat4 &m);
btVector3       Vortex2BtVector3(const Math::point &p);


//------------------------------------------------------------------------------
/**
*/
inline
Math::vector Bt2VortexVector(const btVector3 &btV)
{
    //return Math::vector(btV.x(), btV.y(), btV.z());
	return Math::vector(btV.get128());
}

//------------------------------------------------------------------------------
/**
*/
inline
Math::point Bt2VortexPoint(const btVector3 &btV)
{
    return Math::point(btV.get128());
}

//------------------------------------------------------------------------------
/**
*/
inline
Math::mat4 Bt2VortexMatrix3x3(const btMatrix3x3 &m)
{
		/*
		return Math::mat4(Math::vec4(m.getRow(0).x(), m.getRow(1).x(), m.getRow(2).x(), 0.0f),
							  Math::vec4(m.getRow(0).y(), m.getRow(1).y(), m.getRow(2).y(), 0.0f),
							  Math::vec4(m.getRow(0).z(), m.getRow(1).z(), m.getRow(2).z(), 0.0f),
							  Math::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		*/
		return Math::mat4(m.getColumn(0).get128(), m.getColumn(1).get128(), m.getColumn(2).get128(), m.getColumn(3).get128());
}

//------------------------------------------------------------------------------
/**
*/
inline
Math::mat4 Bt2VortexTransform(const btTransform &t)
{
    const btMatrix3x3 &rot = t.getBasis();
    Math::mat4 m = Bt2VortexMatrix3x3(rot);
    m.set_position(Bt2VortexPoint(t.getOrigin()));
    return m;
}

//------------------------------------------------------------------------------
/**
*/
inline
btMatrix3x3 Vortex2BtM33(const Math::mat4 &m)
{
    return btMatrix3x3(m.getrow0().x(), m.getrow1().x(), m.getrow2().x(), 
                       m.getrow0().y(), m.getrow1().y(), m.getrow2().y(), 
                       m.getrow0().z(), m.getrow1().z(), m.getrow2().z());
}

//------------------------------------------------------------------------------
/**
*/
inline
btTransform Vortex2BtTransform(const Math::mat4 &m)
{
    btTransform transform;
    transform.setBasis(Vortex2BtM33(m));
    transform.setOrigin(Vortex2BtVector3(m.get_position()));
    return transform;
}

//------------------------------------------------------------------------------
/**
*/
inline
btVector3 Vortex2BtVector3(const Math::point &p)
{
    return btVector3(p.x(), p.y(), p.z());
}