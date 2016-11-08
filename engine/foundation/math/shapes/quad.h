#pragma once

#include <foundation/math/vector4.h>
#include <render/properties/graphicsproperty.h>
namespace Math
{

class Quad
{
public:
    Quad()
    {

    }
    ~Quad()
    {

    }

    Vector4 v1;
    Vector4 v2;
    Vector4 v3;
    Vector4 v4;
};

}