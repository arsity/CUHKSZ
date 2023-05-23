#include "rasterizer.h"

using namespace std;

namespace CGL
{

    RasterizerImp::RasterizerImp(PixelSampleMethod psm, LevelSampleMethod lsm,
        size_t width, size_t height,
        unsigned int sample_rate)
    {
        this->psm = psm;
        this->lsm = lsm;
        this->width = width;
        this->height = height;
        this->sample_rate = sample_rate;

        sample_buffer.resize(width * height, Color::White);
    }

    // Used by rasterize_point and rasterize_line
    void RasterizerImp::fill_pixel(size_t x, size_t y, Color c)
    {
        // TODO: Task 2: You might need to this function to fix points and lines (such as the black rectangle border in test4.svg)
        // NOTE: You are not required to implement proper supersampling for points and lines
        // It is sufficient to use the same color for all supersamples of a pixel for points and lines (not triangles)
        // case when the point on line
        sample_buffer[y * width + x] = c;
    }

    // Rasterize a point: simple example to help you start familiarizing
    // yourself with the starter code.
    //
    void RasterizerImp::rasterize_point(float x, float y, Color color)
    {
        // fill in the nearest pixel
        int sx = (int)floor(x);
        int sy = (int)floor(y);

        // check bounds
        if (sx < 0 || sx >= width)
            return;
        if (sy < 0 || sy >= height)
            return;

        fill_pixel(sx, sy, color);
    }

    // Rasterize a line.
    void RasterizerImp::rasterize_line(float x0, float y0, float x1, float y1, Color color)
    {
        if (x0 > x1)
        {
            swap(x0, x1);
            swap(y0, y1);
        }

        float pt[] = { x0, y0 };
        float m = (y1 - y0) / (x1 - x0);
        float dpt[] = { 1, m };
        int steep = abs(m) > 1;
        if (steep)
        {
            dpt[0] = x1 == x0 ? 0 : 1 / abs(m);
            dpt[1] = x1 == x0 ? (y1 - y0) / abs(y1 - y0) : m / abs(m);
        }

        while (floor(pt[0]) <= floor(x1) && abs(pt[1] - y0) <= abs(y1 - y0))
        {
            rasterize_point(pt[0], pt[1], color);
            pt[0] += dpt[0];
            pt[1] += dpt[1];
        }
    }

    // check whethter the point in the triangle or not
    bool RasterizerImp::check_inside(float x0, float y0, float x1, float y1, float x2, float y2, float px, float py)
    {
        // check whether point p inside triangle
        float x_ab = x1 - x0;
        float x_bc = x2 - x1;
        float x_ca = x0 - x2;

        float y_ab = y1 - y0;
        float y_bc = y2 - y1;
        float y_ca = y0 - y2;

        float x_ap = px - x0;
        float x_bp = px - x1;
        float x_cp = px - x2;

        float y_ap = py - y0;
        float y_bp = py - y1;
        float y_cp = py - y2;

        float state_1 = x_ab * y_ap - y_ab * x_ap;
        float state_2 = x_bc * y_bp - y_bc * x_bp;
        float state_3 = x_ca * y_cp - y_ca * x_cp;

        return (state_1 >= 0 && state_2 >= 0 && state_3 >= 0) || (state_1 <= 0 && state_2 <= 0 && state_3 <= 0);
    }

    // Rasterize a triangle.
    void RasterizerImp::rasterize_triangle(float x0, float y0,
        float x1, float y1,
        float x2, float y2,
        Color color)
    {
        // TODO: Task 1: Implement basic triangle rasterization here, no supersampling
        // find bounding box

        auto multiple = ::sqrt(get_sample_rate());

        size_t min_x = floor(min(x0, min(x1, x2)));
        size_t max_x = ceil(max(x0, max(x1, x2)));
        size_t min_y = floor(min(y0, min(y1, y2)));
        size_t max_y = ceil(max(y0, max(y1, y2)));

        if (get_sample_rate() == 1)
        {
            for (auto x = min_x; x < max_x; x++)
            {
                for (auto y = min_y; y < max_y; y++)
                {
                    if (check_inside(x0, y0, x1, y1, x2, y2,
                        float(x) + 0.5,
                        float(y) + 0.5))
                        rasterize_point(x, y, color);
                }
            }
        }
        else
        {
            for (auto x = min_x; x < max_x; x++)
            {
                for (auto y = min_y; y < max_y; y++)
                {
                    auto count = 0;
                    for (auto i = 0; i < multiple; ++i)
                    {
                        for (auto j = 0; j < multiple; ++j)
                        {
                            if (check_inside(x0, y0, x1, y1, x2, y2,
                                float(x) + float(i) / multiple,
                                float(y) + float(j) / multiple))
                                count++;
                        }
                    }
                    auto ratio = float(count) / get_sample_rate();
                    if (ratio == 1)
                    {
                        rasterize_point(x, y, color);
                    }
                    else if (ratio > 0 && sample_buffer[y * width + x] != Color::Black)
                    {
                        auto new_color = (color + Color(-1, -1, -1)) * ratio + sample_buffer[y * width + x];
                        rasterize_point(x, y, new_color);
                    }
                }
            }
        }
    }

    void RasterizerImp::barycentric(
        const Vector2D& a, const Vector2D& b, const Vector2D& c, const Vector2D& p,
        float& u, float& v, float& w)
    {
        auto v0 = b - a, v1 = c - a, v2 = p - a;
        auto d00 = dot(v0, v0);
        auto d01 = dot(v0, v1);
        auto d11 = dot(v1, v1);
        auto d20 = dot(v2, v0);
        auto d21 = dot(v2, v1);
        auto denom = d00 * d11 - d01 * d01;
        v = (d11 * d20 - d01 * d21) / denom;
        w = (d00 * d21 - d01 * d20) / denom;
        u = 1.0f - v - w;
    }

    void RasterizerImp::rasterize_interpolated_color_triangle(
        float x0, float y0, Color c0,
        float x1, float y1, Color c1,
        float x2, float y2, Color c2)
    {
        // TODO: Task 4: Rasterize the triangle, calculating barycentric coordinates and using them to interpolate vertex colors across the triangle
        // Hint: You can reuse code from rasterize_triangle
        // find bounding box
        int min_x = floor(min(x0, min(x1, x2)));
        int max_x = ceil(max(x0, max(x1, x2)));

        int min_y = floor(min(y0, min(y1, y2)));
        int max_y = ceil(max(y0, max(y1, y2)));
        // loop over the super pixel in bounding box

        for (int x = min_x; x < max_x; x++)
        {
            for (int y = min_y; y < max_y; y++)
            {
                if (check_inside(x0, y0, x1, y1, x2, y2, x + 0.5, y + 0.5))
                {
                    auto u = 0.f;
                    auto v = 0.f;
                    auto w = 0.f;
                    auto a = Vector2D(x0, y0);
                    auto b = Vector2D(x1, y1);
                    auto c = Vector2D(x2, y2);
                    auto p = Vector2D(x, y);
                    barycentric(a, b, c, p, u, v, w);
                    Color new_color = u * c0 + v * c1 + w * c2;
                    rasterize_point(x, y, new_color);
                }
            }
        }
    }

    void RasterizerImp::rasterize_textured_triangle(
        float x0, float y0, float u0, float v0,
        float x1, float y1, float u1, float v1,
        float x2, float y2, float u2, float v2,
        Texture& tex)
    {
        // TODO: Task 5: Fill in the SampleParams struct and pass it to the tex.sample function.
        // TODO: Task 6: Set the correct barycentric differentials in the SampleParams struct.
        // Hint: You can reuse code from rasterize_triangle/rasterize_interpolated_color_triangle
        SampleParams sp;
        Color color;
        float u, v, w;
        // get the vector of vertex
        auto a = Vector2D(x0, y0);
        auto b = Vector2D(x1, y1);
        auto c = Vector2D(x2, y2);

        sp.psm = this->psm;
        sp.lsm = this->lsm;
        // find bounding box
        int min_x = floor(min(x0, min(x1, x2)));
        int max_x = ceil(max(x0, max(x1, x2)));
        int min_y = floor(min(y0, min(y1, y2)));
        int max_y = ceil(max(y0, max(y1, y2)));
        //
        for (int x = min_x; x < max_x; x++)
        {
            for (int y = min_y; y < max_y; y++)
            {
                if (check_inside(x0, y0, x1, y1, x2, y2, x + 0.5, y + 0.5))
                {
                    barycentric(a, b, c, Vector2D(x, y), u, v, w);
                    sp.p_uv = Vector2D{ u0 * u + u1 * v + u2 * w, v0 * u + v1 * v + v2 * w };

                    barycentric(a, b, c, Vector2D{ static_cast<double>(x + 1), static_cast<double>(y) }, u, v, w);
                    sp.p_dx_uv = Vector2D{ u0 * u + u1 * v + u2 * w, v0 * u + v1 * v + v2 * w };

                    barycentric(a, b, c, Vector2D{ static_cast<double>(x), static_cast<double>(y + 1) }, u, v, w);
                    sp.p_dy_uv = Vector2D{ u0 * u + u1 * v + u2 * w, v0 * u + v1 * v + v2 * w };

                    color = tex.sample(sp);
                    rasterize_point(floor(x), floor(y), color);
                }
            }
        }
    }

    void RasterizerImp::set_sample_rate(unsigned int rate)
    {
        // TODO: Task 2: You may want to update this function for supersampling support

        this->sample_rate = rate;
        this->sample_buffer.resize(width * height, Color::White);
    }

    void RasterizerImp::set_framebuffer_target(unsigned char* rgb_framebuffer,
        size_t width, size_t height)
    {
        // TODO: Task 2: You may want to update this function for supersampling support

        this->width = width;
        this->height = height;
        this->rgb_framebuffer_target = rgb_framebuffer;

        this->sample_buffer.resize(width * height, Color::White);
    }

    void RasterizerImp::clear_buffers()
    {
        std::fill(rgb_framebuffer_target, rgb_framebuffer_target + 3 * width * height, 255);
        std::fill(sample_buffer.begin(), sample_buffer.end(), Color::White);
    }

    // This function is called at the end of rasterizing all elements of the
    // SVG file.  If you use a supersample buffer to rasterize SVG elements
    // for antialising, you could use this call to fill the target framebuffer
    // pixels from the supersample buffer data.
    //
    void RasterizerImp::resolve_to_framebuffer()
    {
        // TODO: Task 2: You will likely want to update this function for supersampling support
        Color col{};
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; ++y)
            {
                col = sample_buffer[y * width + x];

                for (int k = 0; k < 3; ++k)
                {
                    this->rgb_framebuffer_target[3 * (y * width + x) + k] = (&col.r)[k] * 255;
                }
            }
        }
    }

    Rasterizer::~Rasterizer()
    = default;

} // CGL
