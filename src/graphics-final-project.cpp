#include "Image.h"
#include <iostream>

int main(int argc, char** argv)
{
    Image test(1920, 1080);
    Colour colour1(1.0f, 0.0f, 0.0f);
    Colour colour2(0.0f, 0.0f, 1.0f);
    for (int i = 0; (i < 512); i++)
        for (int j = 0; (j < 256); j++)
            test.set(i, j, colour1);
    for (int i = 0; (i < 512); i++)
        for (int j = 256; (j < 512); j++)
            test.set(i, j, colour2);
    test.toFile("test.tga");

    return 0;
}
