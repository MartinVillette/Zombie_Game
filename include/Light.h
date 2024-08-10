#ifndef LIGHT_H
#define LIGHT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Window;

class Light{
private:
    float* xPtr;
    float* yPtr;
    int range;
    int variation;
    int intensity;
    Window* window;
    float time;

public:
    Light(float* xPtr, float* yPtr, int range, int variation, int intensity, Window* window);
    void render();
};

#endif