#pragma once
#include "vector4.h"

namespace Math
{
    class Plane
    {
    public:
        ///Construct plane from 3 points
        Plane(const Vector4& a, const Vector4& b, const Vector4& c);
        ~Plane();

        const float& d() const;
        Vector4 n() const;
        Vector4 p() const;

    private:
        Vector4 vec;
    };

}