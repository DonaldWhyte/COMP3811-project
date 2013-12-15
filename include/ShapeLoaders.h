#ifndef DW_RAYTRACER_SHAPELOADERS_H
#define DW_RAYTRACER_SHAPELOADERS_H

#include "Shape.h"

namespace shapeloaders
{

    Shape* getTerrainFromHeightmap(const std::string& filename,
        float cellWidth, float maxHeight, const Vector3& offset,
        Texture* texture = NULL);
    /* TODO: add comment */
    Shape* getSkyBox(float size, const std::vector<Texture*>& skyBoxTexture);
    Shape* getMeshFromOBJ(const std::string& filename,
        const Vector3& translation, float scaleFactor = 1.0f);

}

#endif
