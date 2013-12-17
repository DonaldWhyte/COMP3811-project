#include "TerrainHeightTexture.h"
#include <cmath>

using namespace raytracer;

TerrainHeightTexture::TerrainHeightTexture(Image* lowTex, Image* medTex,
	Image* highTex, Image* vHighTex)
{
	// Store given images
	sourceImages[0] = lowTex;
	sourceImages[1] = medTex;
	sourceImages[2] = highTex;
	sourceImages[3] = vHighTex;	
	// Initially, each texture has an even weight
	float weight = 1.0f / NUM_TEXTURES;
	for (unsigned int i = 0; (i < NUM_TEXTURES); i++)
		imageWeights[i] = weight;
}

const Colour& TerrainHeightTexture::getTexel(float u, float v) const
{
	// Compute which pixel to make from each texture
    int width = sourceImages[0]->getWidth();
    int height = sourceImages[0]->getHeight();	
    int pixelX = static_cast<int>(u * width) % width;
    int pixelY = static_cast<int>(v * height) % height;	
	// Retrieve weighted colours from each image, summing them together
	Colour sum;
	for (unsigned int i = 0; (i < NUM_TEXTURES); i++)
		sum += imageWeights[i] * sourceImages[i]->get(pixelX, pixelY);
	return sum;
}

float saturate(float val)
{
	if (val < 0.0f) val = 0.0f;
	else if (val > 1.0f) val = 1.0f;
	return val;
}

void TerrainHeightTexture::updateWeights(float height)
{
	// Compute weights of each image based on height
	imageWeights[0] = saturate(1.0f - fabs(height - 0.0f) / 0.2f);
	imageWeights[1] = saturate(1.0f - fabs(height - 0.3f) / 0.25f);
	imageWeights[2] = saturate(1.0f - fabs(height - 0.6f) / 0.25f);
	imageWeights[3] = saturate(1.0f - fabs(height - 0.9f) / 0.25f);
	// Normalise weightings so they all sum up to 1
	float totalWeight = 0.0f;
	for (unsigned int i = 0; (i < NUM_TEXTURES); i++)
		totalWeight += imageWeights[i];
	for (unsigned int i = 0; (i < NUM_TEXTURES); i++)
		imageWeights[i] /= totalWeight;
}
