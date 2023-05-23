#include "sphere.h"

#include <cmath>

#include "pathtracer/bsdf.h"
#include "util/sphere_drawing.h"

namespace CGL
{
    namespace SceneObjects
    {

        bool Sphere::test(const Ray& r, double& t1, double& t2) const
        {

            // TODO (Part 1.4):
            // Implement ray - sphere intersection test.
            // Return true if there are intersections and writing the
            // smaller of the two intersection times in t1 and the larger in t2.

            auto a = dot(r.d, r.d);
            auto b = 2 * dot(r.d, r.o - this->o);
            auto c = dot(r.o - this->o, r.o - this->o) - this->r2;
            auto delta = b * b - 4 * a * c;
            if (delta < 0)
                return false;
            auto t1_ = (-b - sqrt(delta)) / (2 * a);
            auto t2_ = (-b + sqrt(delta)) / (2 * a);
            if (t1_ > t2_)
                std::swap(t1_, t2_);

            if (t2_ < 0 || t2_ < r.min_t || t1_ > r.max_t)
                return false;

            // We only use the smaller t
            // t2 does not affect anything
            if (t1_ < r.min_t)
                t1 = t2_;
            else
                t1 = t1_;
            t2 = INF_D;

            return true;
        }

        bool Sphere::has_intersection(const Ray& r) const
        {

            // TODO (Part 1.4):
            // Implement ray - sphere intersection.
            // Note that you might want to use the the Sphere::test helper here.

            double t1, t2;
            return test(r, t1, t2);

        }

        bool Sphere::intersect(const Ray& r, Intersection* i) const
        {

            // TODO (Part 1.4):
            // Implement ray - sphere intersection.
            // Note again that you might want to use the the Sphere::test helper here.
            // When an intersection takes place, the Intersection data should be updated
            // correspondingly.

            double t1, t2;
            if (!test(r, t1, t2))
                return false;

            i->t = t1;
            i->n = (r.o + t1 * r.d - this->o).unit();
            i->primitive = this;
            i->bsdf = get_bsdf();

            r.max_t = t1;

            return true;
        }

        void Sphere::draw(const Color& c, float alpha) const
        {
            Misc::draw_sphere_opengl(o, r, c);
        }

        void Sphere::drawOutline(const Color& c, float alpha) const
        {
            // Misc::draw_sphere_opengl(o, r, c);
        }

    } // namespace SceneObjects
} // namespace CGL
