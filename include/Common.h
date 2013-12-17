#ifndef DW_RAYTRACER_COMMON_H
#define DW_RAYTRACER_COMMON_H

#include "Vector3.h"

namespace raytracer {

namespace common
{

	/* Height of highest point in terrain in world coordinates. */
	static const float TERRAIN_MAX_HEIGHT = 100.0f;

    /* Generate floating point number >= min && <= max. */
    float randomFloat(float min, float max);
    /* Generate random 3D vector inside a circle using Monte Carlo method. */
    Vector3 monteCarloDirection();

}

}

#endif
