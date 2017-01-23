#pragma once
#include <cfloat>
#include <GL/glew.h>
#include <render/graphics/camera.h>
#include "point.h"
#include "matrix4.h"

namespace Math
{

struct bbox
{
//public:
    bbox();
    ~bbox();

    ///Sets minPoint and maxPoint to float max and float min respectively so that we always are able to extend them.
    void begin_extend();

    ///End extend. Makes sure we've actually changed min and max points.
    void end_extend();

    ///Extends this bounding box to encapsule given point.
    void extend(const point& p);

    ///Get min point that this bounding box encapsulates
    const point& get_minpoint() const { return this->minPoint; }
    ///Get max point that this bounding box encapsulates
    const point& get_maxpoint() const { return this->maxPoint; }

    ///Get specific value
    float get_minx() const { return this->minPoint.x(); }
    float get_miny() const { return this->minPoint.y(); }
    float get_minz() const { return this->minPoint.z(); }
    float get_maxx() const { return this->maxPoint.x(); }
    float get_maxy() const { return this->maxPoint.y(); }
    float get_maxz() const { return this->maxPoint.z(); }

    point corner_point(const int& index) const;

    ///transforms bounds and extends this boundingbox to encapsulate the result
    void transform(const mat4 &t);

    ///TEMPORARY
    void debugRender();

    void get_quad(const index_t& index, Math::point& a, Math::point& b, Math::point& c, Math::point& d);

    point minPoint;
    point maxPoint;
    bool intersects(const bbox &box) const;
};

inline bbox::bbox() :
    minPoint(-0.5f, -0.5f, -0.5f),
    maxPoint(+0.5f, +0.5f, +0.5f)
{
    //empty
}

inline bbox::~bbox()
{
    //empty
}

inline void bbox::begin_extend()
{
    // FLT_MIN & FLT_MAX doesn't work here unforunately. it yields some wierd results.
	this->minPoint = point(1000000.0f, 1000000.0f, 1000000.0f);
	this->maxPoint = point(-1000000.0f, -1000000.0f, -1000000.0f);
}

inline void bbox::end_extend()
{
	if (this->minPoint == point(1000000.0f, 1000000.0f, 1000000.0f) && this->maxPoint == point(-1000000.0f, -1000000.0f, -1000000.0f))
    {
        this->minPoint = point(0.0f,0.0f,0.0f);
        this->maxPoint = point(0.0f,0.0f,0.0f);
    }
}

inline void bbox::extend(const point &p)
{
    this->minPoint = vec4::minimize(this->minPoint, p);
    this->maxPoint = vec4::maximize(this->maxPoint, p);
}

inline void bbox::transform(const mat4& t)
{
    point temp;
	point min = point(1000000.0f, 1000000.0f, 1000000.0f);
	point max = point(-1000000.0f, -1000000.0f, -1000000.0f);
    int i;

    for (i = 0; i < 8; ++i)
    {
        temp = mat4::transform(corner_point(i), t);
        max = point::maximize(temp, max);
        min = point::minimize(temp, min);
    }

    this->minPoint = min;
    this->maxPoint = max;
}

inline void bbox::get_quad(const index_t& index, Math::point& a, Math::point& b, Math::point& c, Math::point& d)
{
    assert((index >= 0) && (index < 6));
    switch (index)
    {
        case 0:
            a = corner_point(0);
            b = corner_point(1);
            c = corner_point(2);
            d = corner_point(3);
            return;

        case 1:
            a = corner_point(0);
            b = corner_point(6);
            c = corner_point(5);
            d = corner_point(1);
            return;

        case 2:
            a = corner_point(3);
            b = corner_point(2);
            c = corner_point(4);
            d = corner_point(7);
            return;

        case 3:
            a = corner_point(5);
            b = corner_point(6);
            c = corner_point(7);
            d = corner_point(4);
            return;

        case 4:
            a = corner_point(1);
            b = corner_point(5);
            c = corner_point(4);
            d = corner_point(2);
            return;

        default:
            a = corner_point(0);
            b = corner_point(3);
            c = corner_point(7);
            d = corner_point(6);
            return;
    }
}

inline void bbox::debugRender()
{
    glUseProgram(0);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf((GLfloat*)&Graphics::MainCamera::Instance()->getView().mat.m[0][0]);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf((GLfloat*)&Graphics::MainCamera::Instance()->getProjection().mat.m[0][0]);

	glLineWidth(5.0f);
    glBegin(GL_LINES);

    Math::vec4 v0 = this->corner_point(0);
    Math::vec4 v1 = this->corner_point(1);
    Math::vec4 v2 = this->corner_point(2);
    Math::vec4 v3 = this->corner_point(3);
    Math::vec4 v4 = this->corner_point(4);
    Math::vec4 v5 = this->corner_point(5);
    Math::vec4 v6 = this->corner_point(6);
    Math::vec4 v7 = this->corner_point(7);

    glColor3f(0.5f, 0.5f, 0.5f);
	

    glVertex4f(v0[0], v0[1], v0[2], v0[3]);
    glVertex4f(v1[0], v1[1], v1[2], v1[3]);

    glVertex4f(v1[0], v1[1], v1[2], v1[3]);
    glVertex4f(v2[0], v2[1], v2[2], v2[3]);

    glVertex4f(v2[0], v2[1], v2[2], v2[3]);
    glVertex4f(v3[0], v3[1], v3[2], v3[3]);

    glVertex4f(v3[0], v3[1], v3[2], v3[3]);
    glVertex4f(v0[0], v0[1], v0[2], v0[3]);

    glVertex4f(v0[0], v0[1], v0[2], v0[3]);
    glVertex4f(v6[0], v6[1], v6[2], v6[3]);

    glVertex4f(v3[0], v3[1], v3[2], v3[3]);
    glVertex4f(v7[0], v7[1], v7[2], v7[3]);

    glVertex4f(v2[0], v2[1], v2[2], v2[3]);
    glVertex4f(v4[0], v4[1], v4[2], v4[3]);

    glVertex4f(v1[0], v1[1], v1[2], v1[3]);
    glVertex4f(v5[0], v5[1], v5[2], v5[3]);

    glVertex4f(v5[0], v5[1], v5[2], v5[3]);
    glVertex4f(v4[0], v4[1], v4[2], v4[3]);

    glVertex4f(v4[0], v4[1], v4[2], v4[3]);
    glVertex4f(v7[0], v7[1], v7[2], v7[3]);

    glVertex4f(v7[0], v7[1], v7[2], v7[3]);
    glVertex4f(v6[0], v6[1], v6[2], v6[3]);

    glVertex4f(v6[0], v6[1], v6[2], v6[3]);
    glVertex4f(v5[0], v5[1], v5[2], v5[3]);

    glEnd();
}

inline bool bbox::intersects(const bbox& box) const
{
    bool lt = vec4::less3_any(this->maxPoint, box.minPoint);
    bool gt = vec4::greater3_any(this->minPoint, box.maxPoint);
    return !(lt || gt);
}

}