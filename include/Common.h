#ifndef DW_RAYTRACER_COMMON_H
#define DW_RAYTRACER_COMMON_H

#include "Vector3.h"

namespace raytracer {

namespace common
{

	/* Height of highest point in terrain in world coordinates. */
	static const float TERRAIN_MAX_HEIGHT = 100.0f;
	/* Size of individual cell in vertex grid of terrain. */
	static const float TERRAIN_CELL_SIZE = 10.0f;
	/* Total size of one side of the sky box. */
	static const float SKYBOX_SIZE = 200.0f;	

    /* Generate floating point number >= min && <= max. */
    float randomFloat(float min, float max);
    /* Generate random 3D vector inside a circle using Monte Carlo method. */
    Vector3 monteCarloDirection();

}

}

#endif
