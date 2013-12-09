#ifndef DW_RAYTRACER_SHAPELOADERS_H
#define DW_RAYTRACER_SHAPELOADERS_H

#include "Shape.h"

namespace shapeloaders
{

    Shape* getTerrainFromHeightmap(const std::string& filename,
        float cellWidth, float maxHeight);
    Shape* getMeshFromOBJ(const std::string& filename,
        const Vector3& translation, float scaleFactor = 1.0f);

}

#endif
