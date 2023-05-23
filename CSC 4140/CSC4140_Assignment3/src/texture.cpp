#include "texture.h"
#include "CGL/color.h"

#include <cmath>
#include <algorithm>

namespace CGL
{

    Color Texture::sample(const SampleParams& sp)
    {
        // TODO: Task 6: Fill this in.
        // define variables
        float level;
        Color color, next_color, result_color;
        result_color = Color(1, 0, 1);
        if (sp.psm == P_NEAREST)
        {
            // level sampling method
            if (sp.lsm == L_ZERO)
            {
                level = 0.f;
                result_color = sample_nearest(sp.p_uv, (int)level);
            }
            else if (sp.lsm == L_NEAREST)
            {
                level = get_level(sp);
                if (level - floor(level) < 0.5)
                    result_color = sample_nearest(sp.p_uv, floor(level));
                else
                    result_color = sample_nearest(sp.p_uv, ceil(level));
            }
            else if (sp.lsm == L_LINEAR)
            {
                level = get_level(sp);
                color = sample_nearest(sp.p_uv, floor(level));
                next_color = sample_nearest(sp.p_uv, ceil(level));
                result_color = color * (ceil(level) - level) + next_color * (level - floor(level));
            }
        }
        else if (sp.psm == P_LINEAR)
        {
            if (sp.lsm == L_ZERO)
            {
                level = 0.f;
                result_color = sample_bilinear(sp.p_uv, (int)level);
            }
            else if (sp.lsm == L_NEAREST)
            {
                level = get_level(sp);
                if (level - floor(level) < 0.5)
                    result_color = sample_bilinear(sp.p_uv, floor(level));
                else
                    result_color = sample_bilinear(sp.p_uv, ceil(level));
            }
            else if (sp.lsm == L_LINEAR)
            {
                level = get_level(sp);
                color = sample_bilinear(sp.p_uv, floor(level));
                next_color = sample_bilinear(sp.p_uv, ceil(level));
                result_color = color * (ceil(level) - level) + next_color * (level - floor(level));
            }
        }
        // return magenta for invalid level
        return result_color;
    }

    float Texture::get_level(const SampleParams& sp)
    {
        // TODO: Task 6: Fill this in.
        float level;
        int width = mipmap[0].width;
        int height = mipmap[0].height;
        level = max(sqrt(
                pow(sp.p_dx_uv.x * width - sp.p_uv.x * width, 2)
                + pow(sp.p_dx_uv.y * height - sp.p_uv.y * height, 2)),
            sqrt(
                pow(sp.p_dy_uv.x * width - sp.p_uv.x * width, 2)
                + pow(sp.p_dy_uv.y * height - sp.p_uv.y * height, 2)));
        level = log2(level);
        return level;
    }

    Color MipLevel::get_texel(int tx, int ty) const
    {
        return Color(&texels[tx * 3 + ty * width * 3]);
    }

    Color Texture::sample_nearest(const Vector2D& uv, int level)
    {
        // TODO: Task 5: Fill this in.
        const auto& mip = mipmap[level];
        // get the coordinate
        const auto x = uv.x * mip.width;
        const auto y = uv.y * mip.height;
        const float near[4] = { static_cast<float>(floor(x)), static_cast<float>(ceil(x)),
                                static_cast<float>(floor(y)), static_cast<float>(ceil(y)) };
        int near_x, near_y;
        float distance = MAXFLOAT;
        float temp_distance;

        for (int i = 0; i < 4; i++)
        {
            temp_distance = pow(near[i / 2] - x, 2) + pow(near[i % 2 + 2] - y, 2);
            // compare and update the sample point to nearest one
            if (temp_distance < distance)
            {
                distance = temp_distance;
                near_x = near[i / 2];
                near_y = near[i % 2 + 2];
            }
        }
        Color near_color = Color(1, 0, 1);
        near_color = mip.get_texel(near_x, near_y);
        // return magenta for invalid level
        return near_color;
    }
    // define lerp function
    Color lerp(float ratio, Color u0, Color u1)
    {
        Color result_color;
        result_color = (1 - ratio) * u0 + ratio * u1;
        return result_color;
    }
    Color Texture::sample_bilinear(const Vector2D& uv, int level)
    {
        // TODO: Task 5: Fill this in.
        auto& mip = mipmap[level];
        Color bilinear_color = Color(1, 0, 1);
        const auto x = uv.x * mip.width;
        const auto y = uv.y * mip.height;
        auto x0 = floor(x);
        auto x1 = ceil(x);
        auto y0 = floor(y);
        auto y1 = ceil(y);
        float s = x - x0;
        float t = y1 - y;
        Color u0 = lerp(s, mip.get_texel(x0, y1), mip.get_texel(x1, y1));
        Color u1 = lerp(s, mip.get_texel(x0, y0), mip.get_texel(x1, y0));
        bilinear_color = lerp(t, u0, u1);
        // return magenta for invalid level
        return bilinear_color;
    }

    /****************************************************************************/

    // Helpers

    inline void uint8_to_float(float dst[3], unsigned char* src)
    {
        uint8_t* src_uint8 = (uint8_t*)src;
        dst[0] = src_uint8[0] / 255.f;
        dst[1] = src_uint8[1] / 255.f;
        dst[2] = src_uint8[2] / 255.f;
    }

    inline void float_to_uint8(unsigned char* dst, float src[3])
    {
        uint8_t* dst_uint8 = (uint8_t*)dst;
        dst_uint8[0] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[0])));
        dst_uint8[1] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[1])));
        dst_uint8[2] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[2])));
    }

    void Texture::generate_mips(int startLevel)
    {

        // make sure there's a valid texture
        if (startLevel >= mipmap.size())
        {
            std::cerr << "Invalid start level";
        }

        // allocate sublevels
        int baseWidth = mipmap[startLevel].width;
        int baseHeight = mipmap[startLevel].height;
        int numSubLevels = (int)(log2f((float)max(baseWidth, baseHeight)));

        numSubLevels = min(numSubLevels, kMaxMipLevels - startLevel - 1);
        mipmap.resize(startLevel + numSubLevels + 1);

        int width = baseWidth;
        int height = baseHeight;
        for (int i = 1; i <= numSubLevels; i++)
        {

            MipLevel& level = mipmap[startLevel + i];

            // handle odd size texture by rounding down
            width = max(1, width / 2);
            // assert (width > 0);
            height = max(1, height / 2);
            // assert (height > 0);

            level.width = width;
            level.height = height;
            level.texels = vector<unsigned char>(3 * width * height);
        }

        // create mips
        int subLevels = numSubLevels - (startLevel + 1);
        for (int mipLevel = startLevel + 1; mipLevel < startLevel + subLevels + 1;
             mipLevel++)
        {

            MipLevel& prevLevel = mipmap[mipLevel - 1];
            MipLevel& currLevel = mipmap[mipLevel];

            int prevLevelPitch = prevLevel.width * 3; // 32 bit RGB
            int currLevelPitch = currLevel.width * 3; // 32 bit RGB

            unsigned char* prevLevelMem;
            unsigned char* currLevelMem;

            currLevelMem = (unsigned char*)&currLevel.texels[0];
            prevLevelMem = (unsigned char*)&prevLevel.texels[0];

            float wDecimal, wNorm, wWeight[3];
            int wSupport;
            float hDecimal, hNorm, hWeight[3];
            int hSupport;

            float result[3];
            float input[3];

            // conditional differentiates no rounding case from round down case
            if (prevLevel.width & 1)
            {
                wSupport = 3;
                wDecimal = 1.0f / (float)currLevel.width;
            }
            else
            {
                wSupport = 2;
                wDecimal = 0.0f;
            }

            // conditional differentiates no rounding case from round down case
            if (prevLevel.height & 1)
            {
                hSupport = 3;
                hDecimal = 1.0f / (float)currLevel.height;
            }
            else
            {
                hSupport = 2;
                hDecimal = 0.0f;
            }

            wNorm = 1.0f / (2.0f + wDecimal);
            hNorm = 1.0f / (2.0f + hDecimal);

            // case 1: reduction only in horizontal size (vertical size is 1)
            if (currLevel.height == prevLevel.height)
            {
                // assert (currLevel.height == 1);

                for (int i = 0; i < currLevel.width; i++)
                {
                    wWeight[0] = wNorm * (1.0f - wDecimal * i);
                    wWeight[1] = wNorm * 1.0f;
                    wWeight[2] = wNorm * wDecimal * (i + 1);

                    result[0] = result[1] = result[2] = 0.0f;

                    for (int ii = 0; ii < wSupport; ii++)
                    {
                        uint8_to_float(input, prevLevelMem + 3 * (2 * i + ii));
                        result[0] += wWeight[ii] * input[0];
                        result[1] += wWeight[ii] * input[1];
                        result[2] += wWeight[ii] * input[2];
                    }

                    // convert back to format of the texture
                    float_to_uint8(currLevelMem + (3 * i), result);
                }

                // case 2: reduction only in vertical size (horizontal size is 1)
            }
            else if (currLevel.width == prevLevel.width)
            {
                // assert (currLevel.width == 1);

                for (int j = 0; j < currLevel.height; j++)
                {
                    hWeight[0] = hNorm * (1.0f - hDecimal * j);
                    hWeight[1] = hNorm;
                    hWeight[2] = hNorm * hDecimal * (j + 1);

                    result[0] = result[1] = result[2] = 0.0f;
                    for (int jj = 0; jj < hSupport; jj++)
                    {
                        uint8_to_float(input, prevLevelMem + prevLevelPitch * (2 * j + jj));
                        result[0] += hWeight[jj] * input[0];
                        result[1] += hWeight[jj] * input[1];
                        result[2] += hWeight[jj] * input[2];
                    }

                    // convert back to format of the texture
                    float_to_uint8(currLevelMem + (currLevelPitch * j), result);
                }

                // case 3: reduction in both horizontal and vertical size
            }
            else
            {

                for (int j = 0; j < currLevel.height; j++)
                {
                    hWeight[0] = hNorm * (1.0f - hDecimal * j);
                    hWeight[1] = hNorm;
                    hWeight[2] = hNorm * hDecimal * (j + 1);

                    for (int i = 0; i < currLevel.width; i++)
                    {
                        wWeight[0] = wNorm * (1.0f - wDecimal * i);
                        wWeight[1] = wNorm * 1.0f;
                        wWeight[2] = wNorm * wDecimal * (i + 1);

                        result[0] = result[1] = result[2] = 0.0f;

                        // convolve source image with a trapezoidal filter.
                        // in the case of no rounding this is just a box filter of width 2.
                        // in the general case, the support region is 3x3.
                        for (int jj = 0; jj < hSupport; jj++)
                            for (int ii = 0; ii < wSupport; ii++)
                            {
                                float weight = hWeight[jj] * wWeight[ii];
                                uint8_to_float(input, prevLevelMem +
                                                      prevLevelPitch * (2 * j + jj) +
                                                      3 * (2 * i + ii));
                                result[0] += weight * input[0];
                                result[1] += weight * input[1];
                                result[2] += weight * input[2];
                            }

                        // convert back to format of the texture
                        float_to_uint8(currLevelMem + currLevelPitch * j + 3 * i, result);
                    }
                }
            }
        }
    }

}
