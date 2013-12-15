#include "Common.h"
#include <cstdlib>

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

Vector3 common::monteCarloDirection()
{
    while (true)
    {
        Vector3 vector(randomFloat(-1.0f, 1.0f),
            randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
        float length = vector.length();
        // If length of vector is bad, do another loop to generate another random vector
        if ((length > 1.0f) || (length < 0.1f))
            continue;
        // Normalise vector before returning so only direction is returned
        return vector.normalise();
    }
    return Vector3();
}
