// include c++ libs
#include <iostream>
#include <string>
#include "SDL.h"
#include <cmath>

#include "core/engine.h"


#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720 
#define SCREEN_FPS     120.0

dimension ScreenDim;
int main(int argc, char** argv) {
    ScreenDim = {SCREEN_WIDTH, SCREEN_HEIGHT};
    Engine mainEngine("Simulator");

    // const var
    const float frame_period = (1.0f / SCREEN_FPS) * 1000.0f; 
    const SDL_Color GREEN = {};

    bool is_running = true;
    SDL_Event event;
    Uint8* keyboard;


    bool isDown = false;
    // main loop
    while (is_running) {
        Uint64 t_frame_start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {is_running = false;}
            mainEngine.handleEvent(event);
        }

        // main logic
        mainEngine.run();

        // cap fps
        Uint64 t_frame_end = SDL_GetPerformanceCounter();
        float t_elapsed = (t_frame_end - t_frame_start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        if (std::floor(frame_period - t_elapsed) >= 0)
            SDL_Delay(std::floor(frame_period - t_elapsed));
    }

    // deallocate
    SDL_Quit();
}