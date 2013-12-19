#ifndef DW_RAYTRACER_GUI_DEMOSCENE_H
#define DW_RAYTRACER_GUI_DEMOSCENE_H

#include "Raytracer.h"
#include "BoundingShape.h"
#include "Sphere.h"
#include "ShapeLoaders.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "TerrainHeightTexture.h"
#include "Line.h"
#include "Raytracer.h"
#include "Common.h"
#include "Octree.h"

namespace raytracer { namespace gui {

/* This contains all the ifnormation required to render the demo scene,
 * with each possible viewpoint and variant of terrain. */
struct DemoScene
{
	Raytracer* renderer; // object which performs the actual rendering
	std::vector<Camera> cameras; // every viewpoint the scene has
	ShapeList terrainVariants; // contains each terrain to be rendered
	// An element at index i is a BoundingShape which contains lines that
	// visualise the octree used to partition the terrain contained in
	// terrainVariants[i].
	ShapeList octreeLines; 
};

DemoScene constructDemoScene();

} }

#endif
