#include "Light.h"
#include "Window.h"
#include <iostream>
#include <cmath>

Light::Light(float* xPtr, float* yPtr, int range, int variation, int intensity, Window* window)
    : xPtr(xPtr), yPtr(yPtr), range(range), variation(variation), intensity(intensity), window(window), time(0) {
}

void Light::render() {
    SDL_Renderer* renderer = window->getRenderer();
    int width = *window->getWidthPtr();
    int height = *window->getHeightPtr();

    // Load the spot image texture (consider caching this texture)
    SDL_Surface* spotSurface = IMG_Load("assets/spot.png");
    SDL_Texture* spotTexture = SDL_CreateTextureFromSurface(renderer, spotSurface);
    SDL_FreeSurface(spotSurface);

    // Set the blending mode for the spot texture
    SDL_SetTextureBlendMode(spotTexture, SDL_BLENDMODE_ADD);

    // Calculate the position to center the spot on the light's position
    time += 0.05f;  // Increment time (controls speed of variation)
    float variationFactor = 1.0f + (std::sin(time) * variation / 100.0f);
    int layerRange = (int) (range * variationFactor);

    SDL_Rect spotRect;
    spotRect.x = width/2 - layerRange;
    spotRect.y = height/2 - layerRange;
    spotRect.w = layerRange * 2;
    spotRect.h = layerRange * 2;

    // Draw the spot texture with varying intensity
    // for (int i = 0; i < 3; i++) {
        
    //     int size = range * 2 - i * 20;
    //     spotRect.x = width/2 - size/2;
    //     spotRect.y = height/2 - size/2;
    //     spotRect.w = size;
    //     spotRect.h = size;
        
    //     int layerIntensity = intensity - i * 30;
    //     SDL_SetTextureColorMod(spotTexture, layerIntensity, layerIntensity, layerIntensity);
    //     SDL_RenderCopy(renderer, spotTexture, NULL, &spotRect);
    // }

    SDL_SetTextureColorMod(spotTexture, intensity, intensity, intensity);

    SDL_RenderCopy(renderer, spotTexture, NULL, &spotRect);

    // Clean up
    SDL_DestroyTexture(spotTexture);
}