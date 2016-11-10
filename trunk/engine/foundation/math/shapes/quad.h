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

    vec4 v1;
    vec4 v2;
    vec4 v3;
    vec4 v4;
};

}