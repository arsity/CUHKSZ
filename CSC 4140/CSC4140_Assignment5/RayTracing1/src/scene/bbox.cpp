#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CGL
{

    bool BBox::intersect(const Ray& r, double& t0, double& t1) const
    {

        // TODO (Part 2.2):
        // Implement ray - bounding box intersection test
        // If the ray intersected the bouding box within the range given by
        // t0, t1, update t0 and t1 with the new intersection times.

        t0 = (min.x - r.o.x) / r.d.x;
        t1 = (max.x - r.o.x) / r.d.x;
        if (t0 > t1) std::swap(t0, t1);

        auto tymin = (min.y - r.o.y) / r.d.y;
        auto tymax = (max.y - r.o.y) / r.d.y;
        if (tymin > tymax) std::swap(tymin, tymax);
        if ((t0 > tymax) || (tymin > t1))
            return false;

        if (tymin > t0)
            t0 = tymin;
        if (tymax < t1)
            t1 = tymax;

        auto tzmin = (min.z - r.o.z) / r.d.z;
        auto tzmax = (max.z - r.o.z) / r.d.z;
        if (tzmin > tzmax) std::swap(tzmin, tzmax);
        if ((t0 > tzmax) || (tzmin > t1))
            return false;

        if (tzmin > t0)
            t0 = tzmin;
        if (tzmax < t1)
            t1 = tzmax;

        return true;
    }

    void BBox::draw(Color c, float alpha) const
    {

        glColor4f(c.r, c.g, c.b, alpha);

        // top
        glBegin(GL_LINE_STRIP);
        glVertex3d(max.x, max.y, max.z);
        glVertex3d(max.x, max.y, min.z);
        glVertex3d(min.x, max.y, min.z);
        glVertex3d(min.x, max.y, max.z);
        glVertex3d(max.x, max.y, max.z);
        glEnd();

        // bottom
        glBegin(GL_LINE_STRIP);
        glVertex3d(min.x, min.y, min.z);
        glVertex3d(min.x, min.y, max.z);
        glVertex3d(max.x, min.y, max.z);
        glVertex3d(max.x, min.y, min.z);
        glVertex3d(min.x, min.y, min.z);
        glEnd();

        // side
        glBegin(GL_LINES);
        glVertex3d(max.x, max.y, max.z);
        glVertex3d(max.x, min.y, max.z);
        glVertex3d(max.x, max.y, min.z);
        glVertex3d(max.x, min.y, min.z);
        glVertex3d(min.x, max.y, min.z);
        glVertex3d(min.x, min.y, min.z);
        glVertex3d(min.x, max.y, max.z);
        glVertex3d(min.x, min.y, max.z);
        glEnd();

    }

    std::ostream& operator<<(std::ostream& os, const BBox& b)
    {
        return os << "BBOX(" << b.min << ", " << b.max << ")";
    }

} // namespace CGL
