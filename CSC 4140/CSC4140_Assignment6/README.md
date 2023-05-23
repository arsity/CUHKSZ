# Assignment 6
120090645

Haopeng Chen

## Part I
$Depth = 0$, we can only receive light from source.

![](hw6_pathtracer/partI-1.png)

$Depth = 1$, now we can receive lights from one reflect.

![](hw6_pathtracer/partI-2.png)

$Depth = 2$, now we can see the mirror phenomenon of the sphere. 

![](hw6_pathtracer/partI-3.png)

$Depth = 3$, the refraction inside the sphere can reach the camera at last.

![](hw6_pathtracer/partI-4.png)

$Depth = 4$, the flare at the right corner of the blue wall.

![](hw6_pathtracer/partI-5.png)

$Depth = 5$

![](hw6_pathtracer/partI-6.png)

$Depth = 100$

![](hw6_pathtracer/partI-7.png)

## Part III
***The jpg of field***

![](hw6_pathtracer/pic.png)

***The probability debug image of field***

![](hw6_pathtracer/field_probability.png)

***bunny_unlit, uniform sampling***

![](hw6_pathtracer/partIII-1.png)

***bunny_unlit, importance sampling***

![](hw6_pathtracer/partIII-2.png)

***bunny, uniform sampling***

![](hw6_pathtracer/partIII-3.png)

***bunny, importance sampling***

![](hw6_pathtracer/partIII-4.png)

**Summary**: The noise of *importance sampling* is much smaller than *uniform sampling* since we check the intersection at the source to avoid meaningless rays.

## Part IV

**Differences between a pinhole camera model and a thin-lens camera mod

1. Design: In a pinhole camera, light passes through a small hole (or aperture) and forms an inverted image on the opposite side of the camera. In contrast, a thin-lens camera uses a lens to refract light and form an image on the opposite side of the lens.
2. Image formation: The pinhole camera model assumes that light travels in straight lines and the image is formed by the intersection of these lines with the image plane. In contrast, the thin-lens camera model assumes that light travels in curved paths through the lens and the image is formed by the refraction of light at the lens surface.
3. Depth of field: The pinhole camera model has an infinite depth of field, meaning that objects at different distances from the camera will all be in focus. In contrast, the thin-lens camera model has a limited depth of field, meaning that objects at different distances from the lens will only be in focus if they are within a certain range of distances.
4. Distortion: The pinhole camera model produces images that are free from distortion, while the thin-lens camera model can introduce distortion due to the curvature of the lens surface.

#### Different Depths
***depth = 4***

![](hw6_pathtracer/partIV-3.png)

***depth = 4.5***

![](hw6_pathtracer/partIV-4.png)

***depth = 5***

![](hw6_pathtracer/partIV-1.png)

***depth = 6***

![](hw6_pathtracer/partIV-2.png)

#### Different Apertures
***aperture = 0.1***

![](hw6_pathtracer/partIV-5.png)

***aperture = 0.2***

![](hw6_pathtracer/partIV-6.png)

***aperture = 0.3***

![](hw6_pathtracer/partIV-7.png)

***aperture = 0.4***

![](hw6_pathtracer/partIV-8.png)
