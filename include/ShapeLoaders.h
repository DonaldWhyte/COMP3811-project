#ifndef DW_RAYTRACER_SHAPELOADERS_H
#define DW_RAYTRACER_SHAPELOADERS_H

#include "Shape.h"

namespace raytracer {

namespace shapeloaders
{

    /* Load a uniform grid of vertices that represent terrain.
     * The height values for each point on the grid is determined
     * by a heightmap (image). */
    Shape* getTerrainFromHeightmap(const std::string& filename,
        float cellWidth, float maxHeight, const Vector3& offset,
        Texture* texture = NULL, bool useOctree = true);

    /* Load a textured sky box with the specified size.
     * 'skyBoxTextures' should contain exactly SIX elements,
     * with correspond to the textures of the six faces of
     * of the sky box. The following index mapping is used:
     *     0 => front
     *     1 => left
     *     2 => back
     *     3 => right
     *     4 => top
     *     5 => bottom
     * If NULL is used, then that face of the sky box is not
     * textured.
    */
    Shape* getSkyBox(float size, const std::vector<Texture*>& skyBoxTextures);

}

}

#endif
