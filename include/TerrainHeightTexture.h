#ifndef DW_RAYTRACER_TERRAINHEIGHTTEXTURE_H
#define DW_RAYTRACER_TERRAINHEIGHTTEXTURE_H

#include "Texture.h"

namespace raytracer {

/* Texture which is a blend of multiple images, where the blending
 * is based on the height of the point on some terrain.
 * Note that this class assumes all of the images given to it
 * are the same size. Giving images with different sizes may
 * produce unexpected results. 
 *
 * Credit to the following page for this idea:
 * http://www.catalinzima.com/xna/tutorials/4-uses-of-vtf/terrain-rendering-using-heightmaps/ */
class TerrainHeightTexture : public Texture
{

public:
	TerrainHeightTexture(Image* lowTex, Image* medTex, Image* highTex, Image* vHighTex);
    const Colour& getTexel(float u, float v) const;
    /* Updates the weights of each texture image. */
    void updateWeights(float height);
	
private:
	static const unsigned int NUM_TEXTURES = 4;

	Image* sourceImages[NUM_TEXTURES];
	float imageWeights[NUM_TEXTURES];
		
};

}

#endif
