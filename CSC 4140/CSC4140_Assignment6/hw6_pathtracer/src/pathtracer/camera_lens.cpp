#include "camera.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "CGL/misc.h"
#include "CGL/vector2D.h"
#include "CGL/vector3D.h"

using std::cout;
using std::endl;
using std::max;
using std::min;
using std::ifstream;
using std::ofstream;

namespace CGL
{

    using Collada::CameraInfo;

    Ray Camera::generate_ray_for_thin_lens(double x, double y, double rndR, double rndTheta) const
    {

        // TODO Assignment 7: Part 4
        // compute position and direction of ray from the input sensor sample coordinate.
        // Note: use rndR and rndTheta to uniformly sample a unit disk.

        auto deg_hFov = hFov / 180 * PI;
        auto deg_vFov = vFov / 180 * PI;

        auto margin_left = -tan(0.5 * deg_hFov);
        auto margin_right = tan(0.5 * deg_hFov);
        auto margin_bottom = -tan(0.5 * deg_vFov);
        auto margin_top = tan(0.5 * deg_vFov);

        auto converted_x = margin_left + (margin_right - margin_left) * x;
        auto converted_y = margin_bottom + (margin_top - margin_bottom) * y;

        auto dir = Vector3D(converted_x, converted_y, -1).unit();

        // Uniformly sample the disk representing the thin lens at the lens' center.
        auto pLens =
            c2w * (lensRadius * sqrt(rndR) * Vector3D(cos(rndTheta), sin(rndTheta), 0));

        // Calculate pFocus, the point on the plane of focus. Convert the point to world space.
        // Start from (0, 0, 0) in the direction dir with the distance focalDistance
        auto pFocus = c2w * ((-focalDistance / dir.z) * dir);

        // Normalize the direction of the ray, perform the camera-to-world conversion for both its origin and direction, add pos to the ray's origin
        auto ray = Ray(pLens + pos, (pFocus - pLens).unit());
        ray.min_t = nClip;
        ray.max_t = fClip;

        return ray;
    }

} // namespace CGL
