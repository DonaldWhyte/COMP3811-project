# Raytracer Experiments

Final project for CR31 module Computer Graphics for University of Leeds.

## Features

Raytracing:

* Casting rays through each pixel
* Sphere and triangle intersections
* Surface colours and textures

Advanced Raytracing:

* Local illumination with own implementation of Phong lighting
* Multiple light sources possible
* Reflection and refraction rays
* Shadows cast
* Multisampling (uniform/grid and random)

Terrain Rendering:

* Loading heightfield from image file (uses intensity of red for height)
* Uses central differencing to compute surface normals
* Uses layered, blended textures to provide detailed/varying surfaces
  based on height (the higher the terrain, the more rocky the surface
  becomes, for example)

Geometric Optimisation:

* Octree structure used to optimise terrain rendering
* Octree is dynamically constructed by simply adding more
* shapes to the Octree, with subdivisons being created as necessary
* Visualisation of Octree regions possible.
  Parameters for good visualisation of Octrees is below:
        Sampling: Uniform Multisampling with 2 Samples
        Dimensions: 200 x 200
        All effects disabled
        Terrain: Low/Shallow
        Viewpoint: Camera 2
        Octree enabled and visible

Other features:

* Save rendered images to files
* Configure different parameters of the scene

## Instructions

This program demonstrates the basic raytracing and terrain engine I developed
through a demo scene. This scene consists of four spheres, terrain and a sky
box. The four spheres consist of:

* one purely reflective
* one a mixture of refraction/felection
* one slightly reflection (yellow)
* one purely opaque

The terrain can be varied between three types - varied, shallow and high peaks.
These demonstrate the effects the different structures have one equations.

There are two lights - one far away in the sky and one which is on the yellow
sphere. These can be enabled/disabled using the check boxes on the right.

Since the terrain is wrapped in an Octree structure to increase rendering
times, it is possible to enable/disable this acceleration structure using
the check box "Use Octree". It is possible to see how the Octree has
split the space up into regions by checking "Show Octree". Bear in mind
doing this will drasticly reduce rendering times.

## Parameters for Especially Nice Looking Images1

| **Property** | **Value** |
| --- | --- |
| **Sampling** | Uniform Multisampling with 3 Samples |
| **Dimensions** | 1000 x 1000 | 
| **Terrain** | Varied |
| **Viewpoint** | Camera 1 |
| All effects and lights enabled | |
| Octree enabled (but not visible) | |

| **Property** | **Value** |
| --- | --- |
| **Sampling** | Uniform Multisampling with 3 Samples |
| **Dimensions** | 1000 x 1000 | 
| **Terrain** | High Peaks |
| **Viewpoint** | Camera 2 |
| All effects and lights enabled.| |
| Octree enabled (but not visible) | |

| **Property** | **Value** |
| --- | --- |
| **Sampling** | Single Sampling |
| **Dimensions** | 1000 x 1000 | 
| **Terrain** | Varied |
| **Viewpoint** | Camera 1 |
| All effects and only sky light enabled | |
| Octree enabled (but not visible) | |

## Credit

Credit goes to the following sources for the invaluable help they gave:

* Realistic Raytracing (Peter Shirley, R. Keith Morley)
* http://steve.hollasch.net/cgindex/render/refraction.txt
* http://www.catalinzima.com/xna/tutorials/4-uses-of-vtf/terrain-rendering-using-heightmaps/
* http://www.cs.jhu.edu/~cohen/RendTech99/Lectures/Ray_Tracing.bw.pdf
* https://github.com/jelmervdl/raytracer/blob/master/scene.cpp
