// include c++ libs
#include <iostream>
#include <string>
#include "SDL.h"
#include <cmath>

#include "objects/vehicle.h"
#include "graphics/color.hpp"
#include "graphics/canvas.h"

#define SCREEN_WIDTH   1280 // @todo: make this resizable for a set dimension from current screen
#define SCREEN_HEIGHT  720 
#define SCREEN_FPS     120.0

int main(int argc, char** argv) {
    Canvas canvas("Simulator", {SCREEN_WIDTH, SCREEN_HEIGHT});

    // const var
    const float frame_period = (1.0f / SCREEN_FPS) * 1000.0f; 
    const SDL_Color GREEN = {};

    bool is_running = true;
    SDL_Event event;

    // create car 
    dimension carDim = {100, 50};
    SDL_Texture * carTexture = canvas.createTexture(carDim, &DARK_GREY, &SOFT_WHITE);
    Vehicle car ({100, 100}, carDim, carTexture);

    // main loop
    while (is_running) {
        Uint64 t_frame_start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) is_running = false;
        
        // main logic
        canvas.clear();
        canvas.render(car);
        canvas.present();

        // cap fps
        Uint64 t_frame_end = SDL_GetPerformanceCounter();
        float t_elapsed = (t_frame_end - t_frame_start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(std::floor(frame_period - t_elapsed));
    }

    // deallocate
    // SDL_Quit();
}