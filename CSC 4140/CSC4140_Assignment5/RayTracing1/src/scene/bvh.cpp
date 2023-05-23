#include "bvh.h"

#include "CGL/CGL.h"
#include "triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL
{
    namespace SceneObjects
    {

        BVHAccel::BVHAccel(const std::vector<Primitive*>& _primitives,
            size_t max_leaf_size)
        {

            primitives = std::vector<Primitive*>(_primitives);
            root = construct_bvh(primitives.begin(), primitives.end(), max_leaf_size);
        }

        BVHAccel::~BVHAccel()
        {
            if (root)
                delete root;
            primitives.clear();
        }

        BBox BVHAccel::get_bbox() const
        {
            return root->bb;
        }

        void BVHAccel::draw(BVHNode* node, const Color& c, float alpha) const
        {
            if (node->isLeaf())
            {
                for (auto p = node->start; p != node->end; p++)
                {
                    (*p)->draw(c, alpha);
                }
            }
            else
            {
                draw(node->l, c, alpha);
                draw(node->r, c, alpha);
            }
        }

        void BVHAccel::drawOutline(BVHNode* node, const Color& c, float alpha) const
        {
            if (node->isLeaf())
            {
                for (auto p = node->start; p != node->end; p++)
                {
                    (*p)->drawOutline(c, alpha);
                }
            }
            else
            {
                drawOutline(node->l, c, alpha);
                drawOutline(node->r, c, alpha);
            }
        }

        BVHNode* BVHAccel::construct_bvh(std::vector<Primitive*>::iterator start,
            std::vector<Primitive*>::iterator end,
            size_t max_leaf_size)
        {

            // TODO (Part 2.1):
            // Construct a BVH from the given vector of primitives and maximum leaf
            // size configuration. The starter code build a BVH aggregate with a
            // single leaf node (which is also the root) that encloses all the
            // primitives.

            BBox bbox;

            for (auto p = start; p != end; p++)
            {
                BBox bb = (*p)->get_bbox();
                bbox.expand(bb);
            }

            BVHNode* node = new BVHNode(bbox);
            node->start = start;
            node->end = end;

            if (end - start <= max_leaf_size)
            {
                node->l = nullptr;
                node->r = nullptr;
                return node;
            }

            // find longest axis
            Vector3D diag = bbox.extent;
            int axis = 0;
            if (diag.y > diag.x)
                axis = 1;

            if (diag.z > diag.y && diag.z > diag.x)
                axis = 2;


            // sort primitives along longest axis
            sort(start, end, [axis](Primitive* a, Primitive* b)
            {
              BBox bb1 = a->get_bbox();
              BBox bb2 = b->get_bbox();
              return bb1.centroid()[axis] < bb2.centroid()[axis];
            });

            // split primitives into two sets and build children
            auto mid = start + (end - start) / 2;

            node->l = construct_bvh(start, mid, max_leaf_size);
            node->r = construct_bvh(mid, end, max_leaf_size);

            return node;

        }

        bool BVHAccel::has_intersection(const Ray& ray, BVHNode* node) const
        {
            // TODO (Part 2.3):
            // Fill in the intersect function.
            // Take note that this function has a short-circuit that the
            // Intersection version cannot, since it returns as soon as it finds
            // a hit, it doesn't actually have to find the closest hit.

            // simply tests whether there is an intersection between the input ray and any primitives in the input BVH
            double t0, t1;

            if (node->bb.intersect(ray, t0, t1))
            {
                if (node->isLeaf())
                {
                    for (auto p = node->start; p != node->end; p++)
                    {
                        total_isects++;
                        if ((*p)->has_intersection(ray))
                            return true;
                    }
                    return false;
                }
                return has_intersection(ray, node->l) || has_intersection(ray, node->r);
            }
            return false;
        }

        bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode* node) const
        {
            // TODO (Part 2.3):
            // Fill in the intersect function.

            bool hit = false;
            double t0, t1;

            if (node->bb.intersect(ray, t0, t1))
            {
                if (node->isLeaf())
                {
                    for (auto p = node->start; p != node->end; p++)
                    {
                        total_isects++;
                        if ((*p)->intersect(ray, i))
                            hit = true;
                    }
                    return hit;
                }

                bool hit_l = intersect(ray, i, node->l);
                bool hit_r = intersect(ray, i, node->r);

                return hit_l || hit_r;
            }
            return false;
        }

    } // namespace SceneObjects
} // namespace CGL
