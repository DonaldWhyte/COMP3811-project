#include "Common.h"
#include <cstdlib>

using namespace raytracer;

float common::randomFloat(float min, float max)
{
    // Compute scalaing range
    float range = max - min;
    // Generate random number between [0..RAND_MAX]
    float randomNumber = static_cast<float>(rand());
    // Divide by maximum possible value to get in range [0..1]
    randomNumber /= static_cast<float>(RAND_MAX);
    // Scale by range and translate by minimum to get a random
    // float in the required range
    randomNumber *= range;
    randomNumber += min;
    return randomNumber;
}
