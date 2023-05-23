#include "student_code.h"
#include "mutablePriorityQueue.h"

using namespace std;

Vector2D lerp(const Vector2D& point1, const Vector2D& point2, double ratio)
{
    Vector2D result = (1 - ratio) * point1 + ratio * point2;
    return result;
}
Vector3D lerp(const Vector3D& point1, const Vector3D& point2, double ratio)
{
    Vector3D result = (1 - ratio) * point1 + ratio * point2;
    return result;
}
namespace CGL
{

    /**
     * Evaluates one step of the de Casteljau's algorithm using the given points and
     * the scalar parameter t (class member).
     *
     * @param points A vector of points in 2D
     * @return A vector containing intermediate points or the final interpolated vector
     */
    std::vector<Vector2D> BezierCurve::evaluateStep(std::vector<Vector2D> const& points)
    {
        double ratio = (double)(this->t);
        int size = (int)(points.size());
        std::vector<Vector2D> result;
        for (int i = 0; i < size - 1; i++)
        {
            result.push_back(lerp(points[i], points[i + 1], ratio));
        }
        return result;
        // return std::vector<Vector2D>();
    }

    /**
     * Evaluates one step of the de Casteljau's algorithm using the given points and
     * the scalar parameter t (function parameter).
     *
     * @param points    A vector of points in 3D
     * @param t         Scalar interpolation parameter
     * @return A vector containing intermediate points or the final interpolated vector
     */
    std::vector<Vector3D> BezierPatch::evaluateStep(std::vector<Vector3D> const& points, double t) const
    {
        int size = points.size();
        std::vector<Vector3D> result;
        for (int i = 0; i < size - 1; i++)
        {
            result.push_back(lerp(points[i], points[i + 1], t));
        }
        return result;
    }

    /**
     * Fully evaluates de Casteljau's algorithm for a vector of points at scalar parameter t
     *
     * @param points    A vector of points in 3D
     * @param t         Scalar interpolation parameter
     * @return Final interpolated vector
     */
    Vector3D BezierPatch::evaluate1D(std::vector<Vector3D> const& points, double t) const
    {
        int size = points.size();
        std::vector<Vector3D> result;
        for (int i = 0; i < size; i++)
        {
            result.push_back(points[i]);
        }
        for (int i = 0; i < size - 1; i++)
        {
            result = evaluateStep(result, t);
        }
        Vector3D point = result[0];
        return point;
    }

    /**
     * Evaluates the Bezier patch at parameter (u, v)
     *
     * @param u         Scalar interpolation parameter
     * @param v         Scalar interpolation parameter (along the other axis)
     * @return Final interpolated vector
     */
    Vector3D BezierPatch::evaluate(double u, double v) const
    {
        std::vector<Vector3D> list;
        Vector3D result;
        int size = this->controlPoints[0].size();
        for (int i = 0; i < size; i++)
        {
            list.push_back(evaluate1D(this->controlPoints[i], u));
        }
        result = evaluate1D(list, v);
        return result;
    }

    Vector3D Vertex::normal(void) const
    {
        // Returns an approximate unit normal at this vertex, computed by
        // taking the area-weighted average of the normals of neighboring
        // triangles, then normalizing.
        Vector3D N(0., 0., 0.);
        HalfedgeCIter h = halfedge();
        do
        {
            FaceCIter face = h->face();
            HalfedgeCIter h1 = face->halfedge();
            Vector3D pi = h1->vertex()->position;
            Vector3D pj = h1->next()->vertex()->position;
            Vector3D pk = h1->next()->next()->vertex()->position;
            Vector3D a = pi - pj;
            Vector3D b = pi - pk;
            double area = 0.5 * cross(a, b).norm();
            N += face->normal() * area;
            h = h->twin()->next();
        } while (h != halfedge() && !h->isBoundary());
        return N.unit();

    }

    EdgeIter HalfedgeMesh::flipEdge(EdgeIter e0)
    {
        // This method should flip the given edge and return an iterator to the flipped edge.
        //if the edge is on the boundary
        if (e0->isBoundary())
        {
            return e0;
        }
        else
        {
            //get the elements about this flip operation
            HalfedgeIter h1 = e0->halfedge();
            HalfedgeIter h2 = h1->twin();
            HalfedgeIter h1_1 = h1->next();
            HalfedgeIter h1_2 = h1->next()->next();
            HalfedgeIter h2_1 = h2->next();
            HalfedgeIter h2_2 = h2->next()->next();

            VertexIter v0 = h1->vertex();
            VertexIter v1 = h1->next()->next()->vertex();
            VertexIter v2 = h2->vertex();
            VertexIter v3 = h2->next()->next()->vertex();

            FaceIter f1 = h1->face();
            FaceIter f2 = h2->face();
            //if the neighbouring face is on the boundary
            if (f1->isBoundary() || f2->isBoundary())
            {
                return e0;
            }
            //set the elements
            h1->setNeighbors(h2_2, h2, v1, e0, f1);
            h2->setNeighbors(h1_2, h1, v3, e0, f2);
            h1_1->setNeighbors(h1, h1_1->twin(), v2, h1_1->edge(), f1);
            h1_2->setNeighbors(h2_1, h1_2->twin(), v1, h1_2->edge(), f2);
            h2_1->setNeighbors(h2, h2_1->twin(), v0, h2_1->edge(), f2);
            h2_2->setNeighbors(h1_1, h2_2->twin(), v3, h2_2->edge(), f1);

            v0->halfedge() = h2_1;
            v1->halfedge() = h1_2;
            v2->halfedge() = h1_1;
            v3->halfedge() = h2_2;

            f1->halfedge() = h1;
            f2->halfedge() = h2;
        }
        return e0;
    }

    VertexIter HalfedgeMesh::splitEdge(EdgeIter e0)
    {
        // This method should split the given edge and return an iterator to the newly inserted vertex.
        // The halfedge of this vertex should point along the edge that was split, rather than the new edges.
        VertexIter m = newVertex();
        if (e0->isBoundary())
        {
            HalfedgeIter h1 = e0->halfedge();
            if (h1->isBoundary())
            {
                h1 = h1->twin();
            }

            HalfedgeIter h2 = h1->twin();
            HalfedgeIter h1_1 = h1->next();
            HalfedgeIter h1_2 = h1->next()->next();
            HalfedgeIter h3 = newHalfedge();
            HalfedgeIter h4 = newHalfedge();
            HalfedgeIter h5 = newHalfedge();
            HalfedgeIter h6 = newHalfedge();

            VertexIter v0 = h1->vertex();
            VertexIter v1 = h1->next()->vertex();
            VertexIter v2 = h1->next()->next()->vertex();

            EdgeIter e1 = newEdge();
            EdgeIter e2 = newEdge();

            FaceIter f0 = h1->face();
            FaceIter f1 = newFace();

            //set elements
            if (!e0->isNew)
            {
                e0->newPosition = 0.5 * v1->position + 0.5 * v0->position;
            }
            m->isNew = true;
            m->halfedge() = h4;
            m->position = e0->newPosition;
            v0->halfedge() = h1;
            v1->halfedge() = h1_1;
            v2->halfedge() = h1_2;

            e0->newPosition = Vector3D(0, 0, 0);
            e0->halfedge() = h1;
            e1->halfedge() = h4;
            e2->halfedge() = h5;
            e0->isNew = false;
            e1->isNew = false;
            e2->isNew = true;
            e2->newPosition = 0.5 * (v2->position + m->position);

            f0->halfedge() = h1;
            f1->halfedge() = h4;

            h1->setNeighbors(h5, h2, v0, e0, f0);
            h2->vertex() = m;
            h3->vertex() = m;
            h4->setNeighbors(h1_1, h3, m, e1, f1);
            h5->setNeighbors(h1_2, h6, m, e2, f0);
            h6->setNeighbors(h4, h5, v2, e2, f1);
            h1_1->setNeighbors(h6, h1_1->twin(), v1, h1_1->edge(), f1);
            h1_2->setNeighbors(h1, h1_2->twin(), v2, h1_2->edge(), f0);

            return m;
        }
        else
        {
            //get the elements about this flip operation
            HalfedgeIter h1 = e0->halfedge();
            HalfedgeIter h2 = h1->twin();
            HalfedgeIter h1_1 = h1->next();
            HalfedgeIter h1_2 = h1->next()->next();
            HalfedgeIter h2_1 = h2->next();
            HalfedgeIter h2_2 = h2->next()->next();

            VertexIter v0 = h1->vertex();
            VertexIter v1 = h1->next()->next()->vertex();
            VertexIter v2 = h2->vertex();
            VertexIter v3 = h2->next()->next()->vertex();

            FaceIter f1 = h1->face();
            FaceIter f2 = h2->face();
            //if the neighbouring face is on the boundary
            if (f1->isBoundary() || f2->isBoundary())
            {
                return e0->halfedge()->vertex();
            }
            //create new elements
            FaceIter f3 = newFace();
            FaceIter f4 = newFace();
            EdgeIter e1 = newEdge();
            EdgeIter e2 = newEdge();
            EdgeIter e3 = newEdge();
            HalfedgeIter h3 = newHalfedge();
            HalfedgeIter h4 = newHalfedge();
            HalfedgeIter h5 = newHalfedge();
            HalfedgeIter h6 = newHalfedge();
            HalfedgeIter h7 = newHalfedge();
            HalfedgeIter h8 = newHalfedge();

            //set elements
            m->isNew = true;
            if (!e0->isNew)
            {
                e0->newPosition = 0.5 * v2->position + 0.5 * v0->position;
            }
            m->halfedge() = h3;
            m->position = e0->newPosition;
            v0->halfedge() = h1;
            v1->halfedge() = h1_2;
            v2->halfedge() = h2;
            v3->halfedge() = h2_2;

            f1->halfedge() = h1;
            f2->halfedge() = h2;
            f3->halfedge() = h1_1;
            f4->halfedge() = h2_1;

            e0->newPosition = Vector3D(0, 0, 0);
            e0->halfedge() = h1;
            e1->halfedge() = h2;
            e2->halfedge() = h5;
            e3->halfedge() = h7;
            e0->isNew = false;
            e1->isNew = false;
            e2->isNew = true;
            e2->newPosition = 0.5 * (v1->position + m->position);
            e3->isNew = true;
            e3->newPosition = 0.5 * (v3->position + m->position);

            h1->setNeighbors(h6, h3, v0, e0, f1);
            h2->setNeighbors(h8, h4, v2, e1, f2);
            h3->setNeighbors(h2_1, h1, m, e0, f4);
            h4->setNeighbors(h1_1, h2, m, e1, f3);
            h5->setNeighbors(h4, h6, v1, e2, f3);
            h6->setNeighbors(h1_2, h5, m, e2, f1);
            h7->setNeighbors(h3, h8, v3, e3, f4);
            h8->setNeighbors(h2_2, h7, m, e3, f2);
            h1_1->setNeighbors(h5, h1_1->twin(), v2, h1_1->edge(), f3);
            h1_2->setNeighbors(h1, h1_2->twin(), v1, h1_2->edge(), f1);
            h2_1->setNeighbors(h7, h2_1->twin(), v0, h2_1->edge(), f4);
            h2_2->setNeighbors(h2, h2_2->twin(), v3, h2_2->edge(), f2);
        }
        return m;
    }

    void MeshResampler::upsample(HalfedgeMesh& mesh)
    {
        // This routine should increase the number of triangles in the mesh using Loop subdivision.
        // One possible solution is to break up the method as listed below.

        // 1. Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule,
        // and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being
        // a vertex of the original mesh.
        for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++)
        {
            v->isNew = false;
            int num = v->degree();
            double u;
            if (num == 3)
            {
                u = 3.0 / 16.0;
            }
            else
            {
                u = 3.0 / (8.0 * num);
            }
            Vector3D sum(0, 0, 0);
            HalfedgeIter h = v->halfedge();
            if (!v->isBoundary())
            {
                do
                {
                    if (!h->face()->isBoundary())
                    {
                        sum += h->twin()->vertex()->position;
                    }
                    h = h->twin()->next();
                } while (h != v->halfedge());
                //v->newPosition=v->position;
                v->newPosition = (1 - num * u) * v->position + u * sum;
            }
            else
            {
                if (!h->face()->isBoundary())
                {
                    while (!h->face()->isBoundary())
                    {
                        h = h->twin()->next();
                    }
                }
                sum += 0.125 * h->twin()->vertex()->position;
                while (!h->next()->next()->isBoundary())
                {
                    h = h->next()->next()->twin();
                }
                h = h->next()->next()->twin();
                sum += 0.125 * h->twin()->vertex()->position;
                v->newPosition = 0.75 * v->position + sum;
            }
        }

        // 2. Compute the updated vertex positions associated with edges, and store it in Edge::newPosition.
        for (EdgeIter e = mesh.edgesBegin(); e != mesh.edgesEnd(); e++)
        {
            if (e->isBoundary())
            {
                VertexIter v1 = e->halfedge()->vertex();
                VertexIter v2 = e->halfedge()->twin()->vertex();
                e->isNew = true;
                e->newPosition = 0.5 * (v1->position + v2->position);
            }
            else
            {
                VertexIter v1 = e->halfedge()->vertex();
                VertexIter v2 = e->halfedge()->next()->next()->vertex();
                VertexIter v3 = e->halfedge()->twin()->vertex();
                VertexIter v4 = e->halfedge()->twin()->next()->next()->vertex();
                e->isNew = true;
                e->newPosition = 0.125 * (v2->position + v4->position) + 0.375 * (v1->position + v3->position);
            }
        }
        // 3. Split every edge in the mesh, in any order. For future reference, we're also going to store some
        // information about which subdivide edges come from splitting an edge in the original mesh, and which edges
        // are new, by setting the flat Edge::isNew. Note that in this loop, we only want to iterate over edges of
        // the original mesh---otherwise, we'll end up splitting edges that we just split (and the loop will never end!)
        EdgeIter start = mesh.edgesBegin();
        EdgeIter end = mesh.edgesEnd();
        int count = 0;
        int limit = mesh.nEdges();
        for (EdgeIter e = start; e != end; e++)
        {
            count++;
            if (count > limit)
            {
                count = 0;
                break;
            }
            VertexIter v = mesh.splitEdge(e);
        }
        // 4. Flip any new edge that connects an old and new vertex.
        start = mesh.edgesBegin();
        end = mesh.edgesEnd();
        bool flag1 = false;
        bool flag2 = false;
        for (EdgeIter e = start; e != end; e++)
        {
            flag1 = e->halfedge()->vertex()->isNew;
            flag2 = e->halfedge()->twin()->vertex()->isNew;
            if (e->isNew)
            {
                e->isNew = false;
                if (((flag1 && (!flag2)) || ((!flag1) && flag2)))
                {
                    EdgeIter new_edge = mesh.flipEdge(e);
                }
            }

        }
        // 5. Copy the new vertex positions into final Vertex::position.
        for (VertexIter v = mesh.verticesBegin(); v != mesh.verticesEnd(); v++)
        {
            if (!v->isNew)
            {
                v->position = v->newPosition;
                v->newPosition = Vector3D(0, 0, 0);
                v->isNew = false;
            }
        }
    }
}
