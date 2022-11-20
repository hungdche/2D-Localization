// include c++ libs
#include <iostream>
#include <string>
#include "SDL.h"
#include <cmath>

#include "core/math.hpp"
#include "objects/obstacle.h"
#include "objects/vehicle.h"
#include "graphics/color.hpp"
#include "graphics/canvas.h"

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720 
#define SCREEN_FPS     120.0

dimension ScreenDim;
int main(int argc, char** argv) {
    ScreenDim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    Canvas canvas("Simulator", ScreenDim);

    // const var
    const float frame_period = (1.0f / SCREEN_FPS) * 1000.0f; 
    const SDL_Color GREEN = {};

    bool is_running = true;
    SDL_Event event;
    Uint8* keyboard;

    // create car 
    Vehicle * car = canvas.GenerateCar({20, 20});

    bool isDown = false;
    // main loop
    while (is_running) {
        Uint64 t_frame_start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {is_running = false;}
            car->handleEvent(event);
            canvas.handleEvent(event);
        }

        // main logic
        canvas.clear();
        car->move();
        canvas.render(*car);
        canvas.present();

        // cap fps
        Uint64 t_frame_end = SDL_GetPerformanceCounter();
        float t_elapsed = (t_frame_end - t_frame_start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        if (std::floor(frame_period - t_elapsed) >= 0)
            SDL_Delay(std::floor(frame_period - t_elapsed));
    }

    // deallocate
    SDL_Quit();
}