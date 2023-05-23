# Assignment 5

Haopeng Chen 120090645

## Part 1

From a camera to emit many random rays through a pixel, use the *Moller-Trumbore* algorithm to test for intersection
with triangles.

### Images

![](./RayTracing1/p1.png)

## Part 2

We always pick the longest axis of the bounding box as the splitting axis. We can use the middle value of the centroids
of the bounding boxes to pick the splitting point.

### Images
![](./RayTracing1/p2.png)

## Part 3

In raw direct lightning, we generate rays from object to lights, which has small probability to hit and leads to much
noise.
If we change to generate rays from light to object, we can get a better result.

### Images
**light samplings**
![](./RayTracing1/p3_1.png)
![](./RayTracing1/p3_2.png)
![](./RayTracing1/p3_3.png)
![](./RayTracing1/p3_4.png)
**uniform hemisphere samplings**
![](./RayTracing1/p3_5.png)
## Part 4

In indirect lightning, we combine reflection from object and diffuse lightning from light. We can use the *Russian
Roulette* to
control the probability of reflection. 

### Images
**different `may_ray_depth`**
![](./RayTracing1/p4_1.png)
![](./RayTracing1/p4_2.png)
![](./RayTracing1/p4_3.png)
![](./RayTracing1/p4_5.png)
![](./RayTracing1/p4_5.png)

**different sample per pixel rates**
![](./RayTracing1/p4_6.png)
![](./RayTracing1/p4_7.png)
![](./RayTracing1/p4_8.png)
![](./RayTracing1/p4_9.png)
![](./RayTracing1/p4_10.png)
![](./RayTracing1/p4_11.png)
![](./RayTracing1/p4_12.png)

## Part 5
We check the error per batch, and if the error is under max tolerance. We stop the iteration.

### Images
![](./RayTracing1/p5.png)
![](./RayTracing1/p5_rate.png)