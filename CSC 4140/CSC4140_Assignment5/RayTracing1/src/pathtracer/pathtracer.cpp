#include "pathtracer.h"

#include "scene/light.h"
#include "scene/sphere.h"
#include "scene/triangle.h"

using namespace CGL::SceneObjects;

namespace CGL
{

    PathTracer::PathTracer()
    {
        gridSampler = new UniformGridSampler2D();
        hemisphereSampler = new UniformHemisphereSampler3D();

        tm_gamma = 2.2f;
        tm_level = 1.0f;
        tm_key = 0.18;
        tm_wht = 5.0f;
    }

    PathTracer::~PathTracer()
    {
        delete gridSampler;
        delete hemisphereSampler;
    }

    void PathTracer::set_frame_size(size_t width, size_t height)
    {
        sampleBuffer.resize(width, height);
        sampleCountBuffer.resize(width * height);
    }

    void PathTracer::clear()
    {
        bvh = NULL;
        scene = NULL;
        camera = NULL;
        sampleBuffer.clear();
        sampleCountBuffer.clear();
        sampleBuffer.resize(0, 0);
        sampleCountBuffer.resize(0, 0);
    }

    void PathTracer::write_to_framebuffer(ImageBuffer& framebuffer, size_t x0,
        size_t y0, size_t x1, size_t y1)
    {
        sampleBuffer.toColor(framebuffer, x0, y0, x1, y1);
    }

    Vector3D
    PathTracer::estimate_direct_lighting_hemisphere(const Ray& r,
        const Intersection& isect)
    {
        // Estimate the lighting from this intersection coming directly from a light.
        // For this function, sample uniformly in a hemisphere.

        // Note: When comparing Cornel Box (CBxxx.dae) results to importance sampling, you may find the "glow" around the light source is gone.
        // This is totally fine: the area lights in importance sampling has directionality, however in hemisphere sampling we don't model this behaviour.

        // make a coordinate system for a hit point
        // with N aligned with the Z direction.
        Matrix3x3 o2w;
        make_coord_space(o2w, isect.n);
        Matrix3x3 w2o = o2w.T();

        // w_out points towards the source of the ray (e.g.,
        // toward the camera if this is a primary ray)
        const Vector3D hit_p = r.o + r.d * isect.t;
        const Vector3D w_out = w2o * (-r.d);

        // This is the same number of total samples as
        // estimate_direct_lighting_importance (outside of delta lights). We keep the
        // same number of samples for clarity of comparison.
        int num_samples = scene->lights.size() * ns_area_light;
        Vector3D L_out;

        // TODO (Part 3): Write your sampling loop here

        for (int i = 0; i < num_samples; i++)
        {
            auto w_in = hemisphereSampler->get_sample();
            auto f = isect.bsdf->f(w_out, w_in) * (2 * PI);
            auto wi = o2w * w_in;
            auto nextRay = Ray(hit_p, wi);
            nextRay.min_t = EPS_F;
            nextRay.max_t = INF_F - EPS_F;

            Intersection nextIsect{};
            if (!bvh->intersect(nextRay, &nextIsect)) continue;
            L_out += f * nextIsect.bsdf->get_emission() / (double)num_samples;
        }

        return L_out;
    }

    Vector3D
    PathTracer::estimate_direct_lighting_importance(const Ray& r,
        const Intersection& isect)
    {
        // Estimate the lighting from this intersection coming directly from a light.
        // To implement importance sampling, sample only from lights, not uniformly in
        // a hemisphere.

        // make a coordinate system for a hit point
        // with N aligned with the Z direction.
        Matrix3x3 o2w;
        make_coord_space(o2w, isect.n);
        Matrix3x3 w2o = o2w.T();

        // w_out points towards the source of the ray (e.g.,
        // toward the camera if this is a primary ray)
        const Vector3D hit_p = r.o + r.d * isect.t;
        const Vector3D w_out = w2o * (-r.d);
        Vector3D L_out;

        for (auto light : scene->lights)
        {
            if (!light->is_delta_light())
            {
                for (int i = 0; i < ns_area_light; i++)
                {
                    Vector3D wi;
                    double distToLight, pdf;

                    Vector3D lightIntensity = light->sample_L(hit_p, &wi, &distToLight, &pdf);

                    if (pdf == 0) continue;

                    auto f = isect.bsdf->f(w_out, w2o * wi);
                    auto nextRay = Ray(hit_p, wi);
                    nextRay.min_t = EPS_F;
                    nextRay.max_t = distToLight - EPS_F;

                    if (bvh->has_intersection(nextRay)) continue;
                    L_out +=
                        f * lightIntensity / pdf / (double)ns_area_light;
                }
            }
            else
            {
                Vector3D wi;
                double distToLight, pdf;

                Vector3D lightIntensity = light->sample_L(hit_p, &wi, &distToLight, &pdf);

                if (pdf == 0) continue;

                auto f = isect.bsdf->f(w_out, w2o * wi);
                auto nextRay = Ray(hit_p, wi);
                nextRay.min_t = EPS_F;
                nextRay.max_t = distToLight - EPS_F;

                if (bvh->has_intersection(nextRay)) continue;
                L_out += f * lightIntensity / pdf;
            }
        }

        return L_out;
    }

    Vector3D PathTracer::zero_bounce_radiance(const Ray& r,
        const Intersection& isect)
    {
        // TODO: Part 3, Task 2
        // Returns the light that results from no bounces of light
        // (e.g. the light emmitted from a light source)

        return isect.bsdf->get_emission();// * dot(isect.n, r.d.unit())

    }

    Vector3D PathTracer::one_bounce_radiance(const Ray& r,
        const Intersection& isect)
    {
        // TODO: Part 3, Task 3
        // Returns either the direct illumination by hemisphere or importance sampling
        // depending on `direct_hemisphere_sample`

        if (direct_hemisphere_sample)
            return estimate_direct_lighting_hemisphere(r, isect);
        else
            return estimate_direct_lighting_importance(r, isect);
    }

    Vector3D PathTracer::at_least_one_bounce_radiance(const Ray& r,
        const Intersection& isect)
    {
        Matrix3x3 o2w;
        make_coord_space(o2w, isect.n);
        Matrix3x3 w2o = o2w.T();

        Vector3D hit_p = r.o + r.d * isect.t;
        Vector3D w_out = w2o * (-r.d);

        Vector3D L_out(0, 0, 0);

        // TODO: Part 4, Task 2
        // Returns the one bounce radiance + radiance from extra bounces at this point.
        // Should be called recursively to simulate extra bounces.
        L_out = one_bounce_radiance(r, isect);

        if (r.depth == 0) return L_out;

        Vector3D w_in;
        double pdf;

        auto f = isect.bsdf->sample_f(w_out, &w_in, &pdf);
        auto wi = o2w * w_in;
        auto nextRay = Ray(hit_p, wi);
        nextRay.depth = r.depth - 1;
        nextRay.min_t = EPS_F;
        nextRay.max_t = INF_F - EPS_F;

        Intersection nextIsect{};
        if (bvh->intersect(nextRay, &nextIsect) && coin_flip(0.7))
        {
            L_out += (f * at_least_one_bounce_radiance(nextRay, nextIsect)) / pdf / 0.7;
        }

        return L_out;
    }

    Vector3D PathTracer::est_radiance_global_illumination(const Ray& r)
    {
        Intersection isect;
        Vector3D L_out;

        // You will extend this in assignment 3-2.
        // If no intersection occurs, we simply return black.
        // This changes if you implement hemispherical lighting for extra credit.

        // The following line of code returns a debug color depending
        // on whether ray intersection with triangles or spheres has
        // been implemented.
        //
        // REMOVE THIS LINE when you are ready to begin Part 3.

        if (!bvh->intersect(r, &isect))
            return envLight ? envLight->sample_dir(r) : L_out;

        // L_out = (isect.t == INF_D) ? debug_shading(r.d) : normal_shading(isect.n);

        // TODO (Part 3): Return the direct illumination.
        L_out = zero_bounce_radiance(r, isect);
        // L_out += one_bounce_radiance(r, isect);

        // TODO (Part 4): Accumulate the "direct" and "indirect"
        // parts of global illumination into L_out rather than just direct
        L_out += at_least_one_bounce_radiance(r, isect);

        return L_out;
    }

    void PathTracer::raytrace_pixel(size_t x, size_t y)
    {
        // TODO (Part 1.2):
        // Make a loop that generates num_samples camera rays and traces them
        // through the scene. Return the average Vector3D.
        // You should call est_radiance_global_illumination in this function.

        // TODO (Part 5):
        // Modify your implementation to include adaptive sampling.
        // Use the command line parameters "samplesPerBatch" and "maxTolerance"

//        int num_samples = ns_aa;          // total samples to evaluate
        Vector2D origin = Vector2D(x, y); // bottom left corner of the pixel

        // My code Start

        Vector3D radiance(0.0);

        size_t num_samples = 0;

        double s1 = 0, s2 = 0, miu, sigma;

        do
        {
            auto sample = origin + gridSampler->get_sample();
            auto r = camera->generate_ray(sample.x / sampleBuffer.w, sample.y / sampleBuffer.h);
            r.depth = max_ray_depth;

            auto newRadiance = est_radiance_global_illumination(r);
            radiance = (radiance * num_samples + newRadiance) / (num_samples + 1);

            num_samples++;

            s1 += newRadiance.illum();
            s2 += newRadiance.illum() * newRadiance.illum();

            if (num_samples % samplesPerBatch == 0)
            {
                miu = s1 / num_samples;
                sigma = sqrt((s2 - s1 / num_samples * s1) / (num_samples - 1));

                if (1.96 * sigma < maxTolerance * miu * sqrt(num_samples))
                    break;
            }

        } while (num_samples < ns_aa);

        sampleBuffer.update_pixel(radiance, x, y);
        sampleCountBuffer[x + y * sampleBuffer.w] = num_samples;

        // My code End


//        sampleBuffer.update_pixel(Vector3D(0.2, 1.0, 0.8), x, y);
//        sampleCountBuffer[x + y * sampleBuffer.w] = num_samples;

    }

    void PathTracer::autofocus(Vector2D loc)
    {
        Ray r = camera->generate_ray(loc.x / sampleBuffer.w, loc.y / sampleBuffer.h);
        Intersection isect;

        bvh->intersect(r, &isect);

        camera->focalDistance = isect.t;
    }

} // namespace CGL
