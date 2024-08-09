#include "Light.h"
#include "Window.h"

Light::Light(float* xPtr, float* yPtr, int range, int variation, int intensity, Window* window)
    : xPtr(xPtr), yPtr(yPtr), range(range), variation(variation), intensity(intensity), window(window) {
}

void Light::render(){
    SDL_Texture* lightMask = window->getLightMask();
    SDL_Renderer* renderer = window->getRenderer();

    int torchRadius = 150;
    int gradientWidth = 50;
    int width = *window->getWidthPtr();
    int height = *window->getHeightPtr();

    for (int y = -torchRadius - gradientWidth; y <= torchRadius + gradientWidth; y++) {
        for (int x = -torchRadius - gradientWidth; x <= torchRadius + gradientWidth; x++) {
            int distanceSquared = x * x + y * y;
            int radiusSquared = torchRadius * torchRadius;
            int outerRadiusSquared = (torchRadius + gradientWidth) * (torchRadius + gradientWidth);

            Uint8 alpha;
            if (distanceSquared <= radiusSquared) {
                alpha = 0;
            } else if (distanceSquared <= outerRadiusSquared) {
                float distance = sqrt(static_cast<float>(distanceSquared));
                alpha = static_cast<Uint8>(180 * (distance - torchRadius) / gradientWidth);
            } else {
                alpha = 180;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
            int drawX = width / 2 + x;
            int drawY = height / 2 + y;
            SDL_RenderDrawPoint(renderer, drawX, drawY);
        }
    }
}